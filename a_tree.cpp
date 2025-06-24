#include "a_tree.h"

A_Tree::A_Tree(QWidget *parent)
    : QWidget(parent)
    , TreeWidget(nullptr)           // Tree widget pointer - initialized to null, will hold the QTreeWidget component
    , MainLayout(nullptr)           // Layout pointer - initialized to null, will organize the tree widget
    , ContextMenu(nullptr)          // Context menu pointer - initialized to null, will hold right-click menu
    , AddRootAction(nullptr)        // Add root action pointer - initialized to null, will add root items
    , AddChildAction(nullptr)       // Add child action pointer - initialized to null, will add child items
    , EditAction(nullptr)           // Edit action pointer - initialized to null, will edit selected items
    , DeleteAction(nullptr)         // Delete action pointer - initialized to null, will delete selected items
    , CurrentItem(nullptr)          // Current item pointer - initialized to null, tracks right-clicked item
    , IsEditModeEnabled(false)      // Edit mode flag - starts disabled to prevent accidental modifications
    , IsInternalUpdate(false)       // Internal update flag - starts false, prevents recursive signal handling
{
    SetupUserInterface();
    SetTreeWidgetStyle();
    SetupContextMenu();
    ConnectSignalsAndSlots();
}

A_Tree::~A_Tree()
{
    // Qt handles cleanup automatically through parent-child relationships
    // No manual cleanup needed for UI components
}

void A_Tree::SetupUserInterface()
{
    /**
     * Creates and configures the main tree widget and layout
     * Establishes the basic structure for displaying hierarchical data
     */

    // Create main layout
    MainLayout = new QVBoxLayout(this);                   // Main layout - organizes components vertically
    MainLayout->setContentsMargins(0, 0, 0, 0);          // Remove margins for full widget usage
    MainLayout->setSpacing(0);                            // Remove spacing for seamless appearance

    // Create tree widget
    TreeWidget = new QTreeWidget(this);                   // Main tree component - displays hierarchical data
    TreeWidget->setHeaderLabel("Tree Structure");         // Set column header text
    TreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);  // Enable custom context menu
    TreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);  // Allow single item selection only
    TreeWidget->setRootIsDecorated(true);                 // Show expand/collapse decorations for root items
    TreeWidget->setAlternatingRowColors(true);            // Alternate row colors for better readability
    TreeWidget->setAnimated(true);                        // Enable smooth animations for expand/collapse

    MainLayout->addWidget(TreeWidget);                    // Add tree widget to layout
}

void A_Tree::SetTreeWidgetStyle()
{
    /**
     * Applies consistent dark theme styling to the tree widget
     * Ensures visual consistency with the main application theme
     */

    QString _treeStyle =
        "QTreeWidget {"
        "    background-color: #2D2D2D;"                  // Dark background for tree area
        "    color: #FFFFFF;"                             // White text color
        "    border: 2px solid #3F3F3F;"                  // Subtle border around tree
        "    border-radius: 8px;"                         // Rounded corners
        "    selection-background-color: #FF8C00;"        // Light blue selection background
        "    selection-color: #2D2D2D;"                   // Black text on selection for contrast
        "    font-family: 'Segoe UI', Arial, sans-serif;" // Consistent font family
        "    font-size: 11px;"                            // Standard font size
        "    outline: none;"                              // Remove focus outline
        "}"

        "QTreeWidget::item {"
        "    padding: 6px 4px;"                           // Padding inside each item
        "    border: none;"                               // No individual item borders
        "    min-height: 24px;"                           // Minimum height for touch-friendly interface
        "}"

        "QTreeWidget::item:selected {"
        "    background-color: #FF8C00;"                  // Selected item background
        "    color: #FFFFFF;"                             // Selected item text color
        "    border-radius: 4px;"                         // Rounded selection
        "}"

        "QTreeWidget::item:hover {"
        "    background-color: #3A3A3A;"                  // Hover background color
        "    border-radius: 4px;"                         // Rounded hover effect
        "}"

        "QTreeWidget::item:selected:hover {"
        "    background-color: #FF8C00;"                  // Selected + hover background
        "}"


        "QHeaderView::section {"
        "    background-color: #3F3F3F;"                  // Header background
        "    color: #FF8C00;"                             // Header text color
        "    border: 1px solid #555555;"                  // Header border
        "    padding: 8px 12px;"                          // Header padding
        "    font-weight: bold;"                          // Bold header text
        "    font-size: 12px;"                            // Slightly larger header font
        "}";

    TreeWidget->setStyleSheet(_treeStyle);                // Apply styling to tree widget
}

void A_Tree::SetupContextMenu()
{
    /**
     * Creates and configures the right-click context menu
     * Provides user-friendly access to tree manipulation functions
     */

    ContextMenu = new QMenu(this);                        // Context menu container - holds all menu actions

    // Create menu actions
    AddRootAction = new QAction("Add Root Item", this);   // Add root action - creates new top-level item
    AddChildAction = new QAction("Add Child Item", this); // Add child action - creates child of selected item
    EditAction = new QAction("Edit Item", this);          // Edit action - modifies selected item text
    DeleteAction = new QAction("Delete Item", this);      // Delete action - removes selected item and children

    // Add actions to menu
    ContextMenu->addAction(AddRootAction);
    ContextMenu->addAction(AddChildAction);
    ContextMenu->addSeparator();                          // Visual separator between action groups
    ContextMenu->addAction(EditAction);
    ContextMenu->addAction(DeleteAction);

    // Connect menu actions to slots
    connect(AddRootAction, &QAction::triggered, this, &A_Tree::OnAddRootAction);
    connect(AddChildAction, &QAction::triggered, this, &A_Tree::OnAddChildAction);
    connect(EditAction, &QAction::triggered, this, &A_Tree::OnEditAction);
    connect(DeleteAction, &QAction::triggered, this, &A_Tree::OnDeleteAction);
}

void A_Tree::ConnectSignalsAndSlots()
{
    /**
     * Establishes internal signal-slot connections for tree widget functionality
     * Enables automatic handling of user interactions and state changes
     */

    // Connect tree widget signals to internal slots
    connect(TreeWidget, &QTreeWidget::itemChanged, this, &A_Tree::OnItemChanged);
    connect(TreeWidget, &QTreeWidget::customContextMenuRequested, this, &A_Tree::ShowContextMenu);
}

QTreeWidgetItem* A_Tree::AddRootItem(const QString &text)
{
    /**
     * Creates and adds a new root-level item to the tree
     * @param text Display text for the new item
     * @return Pointer to created item or nullptr if creation failed
     */

    if (text.trimmed().isEmpty()) {                       // Validate input text
        return nullptr;                                   // Return null for empty text
    }

    QTreeWidgetItem *_newItem = new QTreeWidgetItem();    // Create new tree item
    _newItem->setText(0, text.trimmed());                 // Set item text (trimmed)
    _newItem->setCheckState(0, Qt::Unchecked);            // Set initial checkbox state to unchecked

    // Set flags based on current edit mode state
    Qt::ItemFlags _flags = _newItem->flags() | Qt::ItemIsUserCheckable;
    if (IsEditModeEnabled) {
        _flags |= Qt::ItemIsEditable;                     // Add editable flag only if edit mode is enabled
    }
    _newItem->setFlags(_flags);                           // Apply the flags

    TreeWidget->addTopLevelItem(_newItem);                // Add item to tree as root level
    TreeWidget->expandItem(_newItem);                     // Expand the new item to show potential children

    return _newItem;                                      // Return pointer to created item
}

QTreeWidgetItem* A_Tree::AddChildItem(QTreeWidgetItem *parent, const QString &text)
{
    /**
     * Creates and adds a new child item to specified parent
     * @param parent Parent item to add child to
     * @param text Display text for the new child item
     * @return Pointer to created child item or nullptr if creation failed
     */

    if (!parent || text.trimmed().isEmpty()) {            // Validate parent and text
        return nullptr;                                   // Return null for invalid input
    }

    QTreeWidgetItem *_newChild = new QTreeWidgetItem();   // Create new child item
    _newChild->setText(0, text.trimmed());                // Set child text (trimmed)
    _newChild->setCheckState(0, Qt::Unchecked);           // Set initial checkbox state to unchecked

    // Set flags based on current edit mode state
    Qt::ItemFlags _flags = _newChild->flags() | Qt::ItemIsUserCheckable;
    if (IsEditModeEnabled) {
        _flags |= Qt::ItemIsEditable;                     // Add editable flag only if edit mode is enabled
    }
    _newChild->setFlags(_flags);                          // Apply the flags

    parent->addChild(_newChild);                          // Add child to parent item
    parent->setExpanded(true);                            // Expand parent to show new child

    // Update parent's check state based on new child
    UpdateParentCheckState(_newChild);                    // Update parent state

    return _newChild;                                     // Return pointer to created child
}

void A_Tree::DeleteItem(QTreeWidgetItem *item)
{
    /**
     * Deletes specified item and all its children from the tree
     * @param item Tree item to delete
     */

    if (!item) {                                          // Validate item pointer
        return;                                           // Exit if item is null
    }

    QString _itemText = item->text(0);                    // Capture item text before deletion
    QTreeWidgetItem *_parent = item->parent();            // Get parent item for state update

    // Remove item from tree
    if (_parent) {
        _parent->removeChild(item);                       // Remove from parent if it has one
        // Update parent state after child removal
        if (_parent->childCount() > 0) {
            UpdateParentCheckState(_parent->child(0));    // Update using first remaining child
        }
    } else {
        int _index = TreeWidget->indexOfTopLevelItem(item);  // Find index of root item
        TreeWidget->takeTopLevelItem(_index);             // Remove root item from tree
    }

    delete item;                                          // Delete the item object
    emit ItemDeleted(_itemText);                          // Emit signal with deleted item text
}

void A_Tree::EditItem(QTreeWidgetItem *item)
{
    /**
     * Opens edit dialog for specified item if edit mode is enabled
     * @param item Tree item to edit
     */

    if (!item || !IsEditModeEnabled) {                    // Check item validity and edit mode
        return;                                           // Exit if item invalid or editing disabled
    }

    bool _inputAccepted = false;                          // Flag for user input acceptance
    QString _newText = QInputDialog::getText(             // Get new text from user
        this,
        "Edit Item",                                      // Dialog title
        "Enter new text for item:",                       // Dialog prompt
        QLineEdit::Normal,                                // Input mode
        item->text(0),                                    // Current text as default
        &_inputAccepted                                   // Output acceptance flag
        );

    if (_inputAccepted && !_newText.trimmed().isEmpty()) {
        IsInternalUpdate = true;                          // Set flag to prevent recursive signals
        item->setText(0, _newText.trimmed());             // Update item text
        IsInternalUpdate = false;                         // Clear flag

        emit ItemEdited(_newText.trimmed());              // Emit signal with new text
    }
}

void A_Tree::LoadDemoData()
{
    /**
     * Loads predefined demonstration data into the tree
     * Clears existing data and creates sample hierarchical structure
     */

    ClearAllItems();                                      // Clear any existing items

    // Create sample tree structure
    QTreeWidgetItem *_documentsRoot = AddRootItem("Documents");     // Documents root item
    if (_documentsRoot) {
        AddChildItem(_documentsRoot, "Work Projects");    // Work projects child
        AddChildItem(_documentsRoot, "Personal Files");   // Personal files child
        QTreeWidgetItem *_projectsFolder = AddChildItem(_documentsRoot, "Active Projects");  // Projects folder
        if (_projectsFolder) {
            AddChildItem(_projectsFolder, "Project Alpha"); // Nested project items
            AddChildItem(_projectsFolder, "Project Beta");
            AddChildItem(_projectsFolder, "Project Gamma");
        }
    }

    QTreeWidgetItem *_mediaRoot = AddRootItem("Media");   // Media root item
    if (_mediaRoot) {
        AddChildItem(_mediaRoot, "Photos");               // Photos child
        AddChildItem(_mediaRoot, "Videos");               // Videos child
        AddChildItem(_mediaRoot, "Music");                // Music child
        QTreeWidgetItem *_photoAlbums = AddChildItem(_mediaRoot, "Photo Albums");  // Photo albums folder
        if (_photoAlbums) {
            AddChildItem(_photoAlbums, "Vacation 2023");  // Nested album items
            AddChildItem(_photoAlbums, "Family Events");
            AddChildItem(_photoAlbums, "Work Presentations");
        }
    }

    QTreeWidgetItem *_settingsRoot = AddRootItem("System Settings");  // Settings root item
    if (_settingsRoot) {
        QTreeWidgetItem *_userSettings = AddChildItem(_settingsRoot, "User Preferences");  // User preferences
        if (_userSettings) {
            AddChildItem(_userSettings, "Display Settings");  // Nested preference items
            AddChildItem(_userSettings, "Privacy Settings");
            AddChildItem(_userSettings, "Notification Settings");
        }
        AddChildItem(_settingsRoot, "System Configuration");  // System config child
        AddChildItem(_settingsRoot, "Security Settings");     // Security settings child
    }
}

void A_Tree::ClearAllItems()
{
    /**
     * Removes all items from the tree and resets internal state
     */

    TreeWidget->clear();                                  // Clear all items from tree widget
    CurrentItem = nullptr;                                // Reset current item pointer
}

void A_Tree::SetEditMode(bool enabled)
{
    /**
     * Sets the edit mode state for the tree widget
     * @param enabled True to enable editing, false to make read-only
     */

    IsEditModeEnabled = enabled;                          // Update internal edit mode flag

    // Update item flags for all items in tree
    QTreeWidgetItemIterator _iterator(TreeWidget);        // Iterator for all tree items
    while (*_iterator) {
        QTreeWidgetItem *_item = *_iterator;              // Current item in iteration
        if (IsEditModeEnabled) {
            _item->setFlags(_item->flags() | Qt::ItemIsEditable);  // Enable editing
        } else {
            _item->setFlags(_item->flags() & ~Qt::ItemIsEditable); // Disable editing
        }
        ++_iterator;                                      // Move to next item
    }
}

bool A_Tree::GetEditMode() const
{
    /**
     * Gets the current edit mode state
     * @return True if editing is enabled, false if read-only
     */

    return IsEditModeEnabled;                             // Return current edit mode state
}

bool A_Tree::GetItemCheckedState(QTreeWidgetItem *item) const
{
    /**
     * Gets the checked state of specified item
     * @param item Tree item to check
     * @return True if item is checked, false if unchecked or item invalid
     */

    if (!item) {                                          // Validate item pointer
        return false;                                     // Return false for invalid item
    }

    return item->checkState(0) == Qt::Checked;            // Return true if item is checked
}

void A_Tree::SetItemCheckedState(QTreeWidgetItem *item, bool checked)
{
    /**
     * Sets the checked state of specified item
     * @param item Tree item to modify
     * @param checked New checked state
     */

    if (!item) {                                          // Validate item pointer
        return;                                           // Exit if item is invalid
    }

    IsInternalUpdate = true;                              // Set flag to prevent recursive signals
    item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);  // Set checkbox state
    UpdateChildrenCheckState(item);                       // Update children to match
    UpdateParentCheckState(item);                         // Update parent based on siblings
    IsInternalUpdate = false;                             // Clear flag

    emit ItemCheckStateChanged(item->text(0), checked);   // Emit signal with state change
}

void A_Tree::OnItemChanged(QTreeWidgetItem *item, int column)
{
    /**
     * Handles item change events, primarily checkbox state changes
     * @param item Tree item that changed
     * @param column Column that changed (should be 0 for text/checkbox)
     */

    if (!item || column != 0 || IsInternalUpdate) {       // Validate parameters and check for internal update
        return;                                           // Exit if invalid or during internal update
    }

    IsInternalUpdate = true;                              // Set flag to prevent recursion

    Qt::CheckState _newState = item->checkState(0);       // Get new checkbox state

    // Update children to match parent state
    if (_newState != Qt::PartiallyChecked) {
        UpdateChildrenCheckState(item);                   // Update all children
    }

    // Update parent states up the tree
    UpdateParentCheckState(item);                         // Update parent chain

    IsInternalUpdate = false;                             // Clear flag

    // Emit signal for checkbox state changes
    bool _isChecked = (_newState == Qt::Checked);         // Convert to boolean
    emit ItemCheckStateChanged(item->text(0), _isChecked);  // Emit state change signal
}

void A_Tree::UpdateChildrenCheckState(QTreeWidgetItem *parentItem)
{
    /**
     * Updates all children of specified item to match parent's checked state
     * @param parentItem Parent item whose children should be updated
     */

    if (!parentItem) {                                    // Validate parent item
        return;                                           // Exit if parent is invalid
    }

    Qt::CheckState _parentState = parentItem->checkState(0);  // Get parent checkbox state
    if (_parentState == Qt::PartiallyChecked) {           // Don't update children for partial state
        return;                                           // Exit if parent is partially checked
    }

    // Update all direct children
    for (int _childIndex = 0; _childIndex < parentItem->childCount(); ++_childIndex) {
        QTreeWidgetItem *_child = parentItem->child(_childIndex);  // Get child at index
        _child->setCheckState(0, _parentState);           // Set child state to match parent
        UpdateChildrenCheckState(_child);                 // Recursively update grandchildren
    }
}

void A_Tree::UpdateParentCheckState(QTreeWidgetItem *childItem)
{
    /**
     * Updates parent item's checked state based on children's states
     * @param childItem Child item that triggered the update
     */

    if (!childItem) {                                     // Validate child item
        return;                                           // Exit if child is invalid
    }

    QTreeWidgetItem *_parent = childItem->parent();       // Get parent item
    if (!_parent) {                                       // Check if parent exists
        return;                                           // Exit if no parent (root item)
    }

    Qt::CheckState _newParentState = CalculateParentCheckState(_parent);  // Calculate new parent state
    _parent->setCheckState(0, _newParentState);           // Set calculated state

    UpdateParentCheckState(_parent);                      // Continue up the tree recursively
}

Qt::CheckState A_Tree::CalculateParentCheckState(QTreeWidgetItem *parentItem) const
{
    /**
     * Calculates appropriate check state for parent based on children's states
     * @param parentItem Parent item to calculate state for
     * @return Calculated check state (Checked, Unchecked, or PartiallyChecked)
     */

    if (!parentItem || parentItem->childCount() == 0) {   // Validate parent and check for children
        return Qt::Unchecked;                             // Return unchecked if no children
    }

    int _checkedCount = 0;                                // Count of checked children
    int _partialCount = 0;                                // Count of partially checked children
    int _totalCount = parentItem->childCount();           // Total number of children

    // Count children in each state
    for (int _childIndex = 0; _childIndex < _totalCount; ++_childIndex) {
        QTreeWidgetItem *_child = parentItem->child(_childIndex);  // Get child at index
        Qt::CheckState _childState = _child->checkState(0);       // Get child's check state

        if (_childState == Qt::Checked) {
            _checkedCount++;                              // Increment checked count
        } else if (_childState == Qt::PartiallyChecked) {
            _partialCount++;                              // Increment partial count
        }
    }

    // Determine parent state based on children counts
    if (_partialCount > 0 || (_checkedCount > 0 && _checkedCount < _totalCount)) {
        return Qt::PartiallyChecked;                      // Mixed states = partially checked
    } else if (_checkedCount == _totalCount) {
        return Qt::Checked;                               // All checked = fully checked
    } else {
        return Qt::Unchecked;                             // None checked = unchecked
    }
}

void A_Tree::ShowContextMenu(const QPoint &pos)
{
    /**
     * Displays context menu at specified position
     * @param pos Position where right-click occurred
     */

    CurrentItem = TreeWidget->itemAt(pos);                // Get item at click position

    // Enable/disable actions based on context
    AddChildAction->setEnabled(CurrentItem != nullptr);   // Enable add child only if item selected
    EditAction->setEnabled(CurrentItem != nullptr && IsEditModeEnabled);  // Enable edit only if item selected and edit mode on
    DeleteAction->setEnabled(CurrentItem != nullptr);     // Enable delete only if item selected

    ContextMenu->exec(TreeWidget->mapToGlobal(pos));      // Show context menu at global position
}

void A_Tree::OnAddRootAction()
{
    /**
     * Context menu action handler for adding root item
     */

    bool _inputAccepted = false;                          // Input acceptance flag
    QString _itemText = QInputDialog::getText(            // Get text from user
        this,
        "Add Root Item",                                  // Dialog title
        "Enter text for new root item:",                  // Dialog prompt
        QLineEdit::Normal,                                // Input mode
        "New Root Item",                                  // Default text
        &_inputAccepted                                   // Output acceptance flag
        );

    if (_inputAccepted && !_itemText.trimmed().isEmpty()) {
        AddRootItem(_itemText.trimmed());                 // Add the root item
    }
}

void A_Tree::OnAddChildAction()
{
    /**
     * Context menu action handler for adding child item
     */

    if (!CurrentItem) {                                   // Check if item is selected
        return;                                           // Exit if no item selected
    }

    bool _inputAccepted = false;                          // Input acceptance flag
    QString _itemText = QInputDialog::getText(            // Get text from user
        this,
        "Add Child Item",                                 // Dialog title
        "Enter text for new child item:",                 // Dialog prompt
        QLineEdit::Normal,                                // Input mode
        "New Child Item",                                 // Default text
        &_inputAccepted                                   // Output acceptance flag
        );

    if (_inputAccepted && !_itemText.trimmed().isEmpty()) {
        AddChildItem(CurrentItem, _itemText.trimmed());   // Add child to selected item
    }
}

void A_Tree::OnEditAction()
{
    /**
     * Context menu action handler for editing selected item
     */

    EditItem(CurrentItem);                                // Edit the currently selected item
}

void A_Tree::OnDeleteAction()
{
    /**
     * Context menu action handler for deleting selected item
     */

    if (!CurrentItem) {                                   // Check if item is selected
        return;                                           // Exit if no item selected
    }

    int _userResponse = QMessageBox::question(            // Confirm deletion with user
        this,
        "Delete Item",                                    // Dialog title
        QString("Are you sure you want to delete '%1' and all its children?\n\nThis action cannot be undone.")
            .arg(CurrentItem->text(0)),                   // Dialog message with item name
        QMessageBox::Yes | QMessageBox::No,               // Available buttons
        QMessageBox::No                                   // Default button (safer option)
        );

    if (_userResponse == QMessageBox::Yes) {
        DeleteItem(CurrentItem);                          // Delete the item if confirmed
        CurrentItem = nullptr;                            // Clear current item pointer
    }
}
