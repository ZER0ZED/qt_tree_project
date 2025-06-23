#ifndef A_TREE_H
#define A_TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QMouseEvent>

class A_Tree : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for A_Tree widget
     * @param parent Parent widget pointer - can be nullptr for standalone widget
     */
    explicit A_Tree(QWidget *parent = nullptr);

    /**
     * @brief Destructor for A_Tree widget
     */
    ~A_Tree();

    // Main functionality methods
    /**
     * @brief Adds a new root item to the tree
     * @param text Display text for the new root item - should not be empty
     * @return Pointer to created tree item - nullptr if creation failed
     */
    QTreeWidgetItem* AddRootItem(const QString &text);

    /**
     * @brief Adds a new child item to specified parent
     * @param parent Parent item to add child to - must be valid existing item
     * @param text Display text for the new child item - should not be empty
     * @return Pointer to created child item - nullptr if creation failed or parent invalid
     */
    QTreeWidgetItem* AddChildItem(QTreeWidgetItem *parent, const QString &text);

    /**
     * @brief Deletes specified item and all its children
     * @param item Tree item to delete - must be valid existing item
     */
    void DeleteItem(QTreeWidgetItem *item);

    /**
     * @brief Edits the text of specified item (only if edit mode enabled)
     * @param item Tree item to edit - must be valid existing item
     */
    void EditItem(QTreeWidgetItem *item);

    /**
     * @brief Loads predefined demonstration data into the tree
     * Clears existing data and populates with sample hierarchical structure
     */
    void LoadDemoData();

    /**
     * @brief Removes all items from the tree
     * Clears the entire tree structure and resets internal state
     */
    void ClearAllItems();

    // Getters and Setters
    /**
     * @brief Sets the edit mode state for the tree widget
     * @param enabled Edit mode flag - true allows editing, false makes items read-only
     */
    void SetEditMode(bool enabled);

    /**
     * @brief Gets the current edit mode state
     * @return Current edit mode state - true if editing enabled, false if read-only
     */
    bool GetEditMode() const;

    /**
     * @brief Gets the checked state of specified item
     * @param item Tree item to check - must be valid existing item
     * @return Checkbox state - true if checked, false if unchecked or item invalid
     */
    bool GetItemCheckedState(QTreeWidgetItem *item) const;

    /**
     * @brief Sets the checked state of specified item
     * @param item Tree item to modify - must be valid existing item
     * @param checked New checkbox state - true for checked, false for unchecked
     */
    void SetItemCheckedState(QTreeWidgetItem *item, bool checked);

signals:
    /**
     * @brief Signal emitted when an item is successfully edited
     * @param itemText New text of the edited item
     */
    void ItemEdited(const QString &itemText);

    /**
     * @brief Signal emitted when an item is successfully deleted
     * @param itemText Text of the deleted item (captured before deletion)
     */
    void ItemDeleted(const QString &itemText);

    /**
     * @brief Signal emitted when an item's checked state changes
     * @param itemText Text of the item whose state changed
     * @param checked New checked state of the item
     */
    void ItemCheckStateChanged(const QString &itemText, bool checked);

private slots:
    /**
     * @brief Handles item state changes (primarily checkbox changes)
     * @param item Tree item that changed - guaranteed to be valid
     * @param column Column that changed - should be 0 for text/checkbox column
     */
    void OnItemChanged(QTreeWidgetItem *item, int column);

    /**
     * @brief Shows context menu at specified position
     * @param pos Position where right-click occurred - in widget coordinates
     */
    void ShowContextMenu(const QPoint &pos);

    /**
     * @brief Context menu action to add root item
     */
    void OnAddRootAction();

    /**
     * @brief Context menu action to add child item
     */
    void OnAddChildAction();

    /**
     * @brief Context menu action to edit selected item
     */
    void OnEditAction();

    /**
     * @brief Context menu action to delete selected item
     */
    void OnDeleteAction();

private:
    /**
     * @brief Sets up the user interface components
     * Creates and configures the tree widget and layout
     */
    void SetupUserInterface();

    /**
     * @brief Applies styling to the tree widget
     * Sets colors, fonts, and visual appearance for consistency
     */
    void SetTreeWidgetStyle();

    /**
     * @brief Sets up the context menu and its actions
     * Creates right-click menu with add, edit, delete options
     */
    void SetupContextMenu();

    /**
     * @brief Connects internal signals and slots
     * Establishes communication between tree widget and internal handlers
     */
    void ConnectSignalsAndSlots();

    /**
     * @brief Updates child items to match parent's checked state
     * @param parentItem Parent item whose children should be updated - must be valid
     */
    void UpdateChildrenCheckState(QTreeWidgetItem *parentItem);

    /**
     * @brief Updates parent item's checked state based on children
     * @param childItem Child item that triggered the update - must be valid
     */
    void UpdateParentCheckState(QTreeWidgetItem *childItem);

    /**
     * @brief Calculates appropriate check state for parent based on children
     * @param parentItem Parent item to calculate state for - must be valid
     * @return Calculated check state - Checked, Unchecked, or PartiallyChecked
     */
    Qt::CheckState CalculateParentCheckState(QTreeWidgetItem *parentItem) const;

    // UI Components
    QTreeWidget *TreeWidget;         // Main tree display widget - shows hierarchical data with checkboxes
    QVBoxLayout *MainLayout;         // Layout container - organizes tree widget within this widget

    // Context Menu Components
    QMenu *ContextMenu;              // Right-click context menu - provides item manipulation options
    QAction *AddRootAction;          // Context menu action - adds new root level item
    QAction *AddChildAction;         // Context menu action - adds child to selected item
    QAction *EditAction;             // Context menu action - edits selected item text
    QAction *DeleteAction;           // Context menu action - deletes selected item

    // State Variables
    QTreeWidgetItem *CurrentItem;    // Currently selected/right-clicked item - can be nullptr if no selection
    bool IsEditModeEnabled;          // Edit mode flag - true allows editing, false makes read-only
    bool IsInternalUpdate;           // Internal update flag - prevents recursive signal handling during updates
};

#endif // A_TREE_H
