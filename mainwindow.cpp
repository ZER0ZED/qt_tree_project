#include "mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , TreeWidget(nullptr)           // Tree widget pointer - initialized to null, will hold the main tree component
    , CentralWidget(nullptr)        // Central widget pointer - initialized to null, will hold all main UI elements
    , MainLayout(nullptr)           // Main layout pointer - initialized to null, will organize UI vertically
    , ButtonLayout(nullptr)         // Button layout pointer - initialized to null, will organize buttons horizontally
    , ControlLayout(nullptr)        // Control layout pointer - initialized to null, will organize controls horizontally
    , AddRootButton(nullptr)        // Add root button pointer - initialized to null, will create new root items
    , LoadDemoButton(nullptr)       // Load demo button pointer - initialized to null, will load sample data
    , ClearAllButton(nullptr)       // Clear all button pointer - initialized to null, will remove all items
    , TitleLabel(nullptr)           // Title label pointer - initialized to null, will display app title
    , EditModeCheckbox(nullptr)     // Edit mode checkbox pointer - initialized to null, will control edit permissions
    , StatusLabel(nullptr)          // Status label pointer - initialized to null, will show operation feedback
    , ControlGroup(nullptr)         // Control group pointer - initialized to null, will group control elements
    , RootItemCounter(1)            // Root item counter - starts at 1, increments with each new root item created
    , IsEditModeEnabled(false)      // Edit mode flag - starts disabled, prevents accidental editing
{
    SetupUserInterface();
    SetApplicationStyle();
    ConnectSignalsAndSlots();

    // Set window properties
    setWindowTitle("Tree Widget Application");
    setGeometry(100, 100, 800, 600);
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    // Qt handles cleanup automatically through parent-child relationships
    // No manual cleanup needed for UI components
}

void MainWindow::SetupUserInterface()
{
    /**
     * Creates and organizes all user interface components
     * Establishes the main layout structure and initializes all widgets
     */

    // Create central widget and main layout
    CentralWidget = new QWidget(this);                    // Central container - holds all UI components
    setCentralWidget(CentralWidget);

    MainLayout = new QVBoxLayout(CentralWidget);          // Main vertical layout - organizes components top to bottom
    MainLayout->setContentsMargins(15, 15, 15, 15);      // Set margins around the layout (15px on all sides)
    MainLayout->setSpacing(10);                           // Set spacing between layout items (10px)

    // Create title label
    TitleLabel = new QLabel("Tree Widget Management System", this);  // Title display - shows application name
    TitleLabel->setAlignment(Qt::AlignCenter);            // Center align the title text
    TitleLabel->setObjectName("titleLabel");              // Set object name for styling
    MainLayout->addWidget(TitleLabel);

    // Create control group for edit mode and status
    ControlGroup = new QGroupBox("Control Panel", this);  // Control group - organizes control elements visually
    ControlLayout = new QHBoxLayout(ControlGroup);        // Horizontal layout for control elements

    EditModeCheckbox = new QCheckBox("Enable Edit Mode", this);  // Edit toggle - controls whether items can be edited
    EditModeCheckbox->setChecked(IsEditModeEnabled);       // Set initial state to match internal flag

    StatusLabel = new QLabel("Ready", this);              // Status display - shows current operation status
    StatusLabel->setObjectName("statusLabel");            // Set object name for styling

    ControlLayout->addWidget(EditModeCheckbox);           // Add edit checkbox to control layout
    ControlLayout->addStretch();                          // Add flexible space to push status to right
    ControlLayout->addWidget(StatusLabel);                // Add status label to control layout

    MainLayout->addWidget(ControlGroup);                  // Add control group to main layout

    // Create tree widget
    TreeWidget = new A_Tree(this);                        // Main tree component - displays hierarchical data
    TreeWidget->SetEditMode(IsEditModeEnabled);           // Set initial edit mode state
    MainLayout->addWidget(TreeWidget);                    // Add tree to main layout

    // Create button layout
    ButtonLayout = new QHBoxLayout();                     // Horizontal layout for action buttons
    ButtonLayout->setSpacing(10);                         // Set spacing between buttons (10px)

    // Create control buttons
    AddRootButton = new QPushButton("Add Root Item", this);      // Add root button - creates new root level items
    LoadDemoButton = new QPushButton("Load Demo Data", this);    // Load demo button - populates with sample data
    ClearAllButton = new QPushButton("Clear All Items", this);   // Clear button - removes all tree items

    // Add buttons to layout
    ButtonLayout->addWidget(AddRootButton);
    ButtonLayout->addWidget(LoadDemoButton);
    ButtonLayout->addWidget(ClearAllButton);
    ButtonLayout->addStretch();                           // Add flexible space to left-align buttons

    MainLayout->addLayout(ButtonLayout);                  // Add button layout to main layout
}

void MainWindow::SetApplicationStyle()
{
    /**
     * Applies the original simple dark theme styling to all application components
     * Uses clean colors and minimal effects for better usability
     */

    QString _simpleThemeStyle =
        // Main window styling - simple dark gray
        "QMainWindow {"
        "    background-color: #2c2c2c;"                  // Original dark gray background
        "    color: white;"                               // White text color
        "}"

        // Central widget styling - consistent with original
        "QWidget {"
        "    background-color: #2c2c2c;"                  // Consistent dark background
        "    color: white;"                               // White text color
        "}"

        // Title label styling - orange accent like original
        "QLabel#titleLabel {"
        "    font-size: 16px;"                            // Original title size
        "    font-weight: bold;"                          // Bold text
        "    color: #FF8C00;"                             // Original orange accent color
        "    background-color: transparent;"              // Transparent background
        "    padding: 5px;"                               // Simple padding
        "}"

        // Status label styling - simple white text
        "QLabel#statusLabel {"
        "    color: white;"                               // Simple white text
        "    font-size: 11px;"                            // Standard font size
        "}"

        // Group box styling - minimal borders
        "QGroupBox {"
        "    font-weight: bold;"                          // Bold text for group title
        "    border: 1px solid #555555;"                  // Simple border
        "    border-radius: 4px;"                         // Slight rounding
        "    margin-top: 8px;"                            // Top margin
        "    padding-top: 8px;"                           // Top padding
        "    background-color: #3a3a3a;"                  // Original lighter background
        "}"

        "QGroupBox::title {"
        "    subcontrol-origin: margin;"                  // Title positioning
        "    left: 8px;"                                  // Left offset for title
        "    padding: 0 4px 0 4px;"                       // Simple padding around title
        "    color: #FF8C00;"                             // Orange title color like original
        "}"

        // Checkbox styling - simple and clean
        "QCheckBox {"
        "    color: white;"                               // White text
        "    spacing: 5px;"                               // Space between checkbox and text
        "}"

        // Button styling - original blue theme
        "QPushButton {"
        "    background-color: #3498db;"                  // Original blue background
        "    border: none;"                               // No border
        "    color: white;"                               // White text
        "    padding: 8px 16px;"                          // Original padding
        "    border-radius: 4px;"                         // Original border radius
        "    font-weight: bold;"                          // Bold text
        "}"

        "QPushButton:hover {"
        "    background-color: #2980b9;"                  // Original hover color
        "}"

        "QPushButton:pressed {"
        "    background-color: #21618c;"                  // Original pressed color
        "}";

    setStyleSheet(_simpleThemeStyle);                     // Apply the simple original style
}

void MainWindow::ConnectSignalsAndSlots()
{
    /**
     * Establishes communication between UI components using Qt's signal-slot mechanism
     * Connects button clicks, checkbox changes, and tree widget events to appropriate handlers
     */

    // Connect button signals to slots
    connect(AddRootButton, &QPushButton::clicked, this, &MainWindow::OnAddRootButtonClicked);
    connect(LoadDemoButton, &QPushButton::clicked, this, &MainWindow::OnLoadDemoButtonClicked);
    connect(ClearAllButton, &QPushButton::clicked, this, &MainWindow::OnClearAllButtonClicked);

    // Connect checkbox signal to slot
    connect(EditModeCheckbox, &QCheckBox::toggled, this, &MainWindow::OnEditModeToggled);

    // Connect tree widget signals to slots
    connect(TreeWidget, &A_Tree::ItemEdited, this, &MainWindow::OnTreeItemEdited);
    connect(TreeWidget, &A_Tree::ItemDeleted, this, &MainWindow::OnTreeItemDeleted);
}

void MainWindow::OnAddRootButtonClicked()
{
    /**
     * Handles Add Root button click event
     * Prompts user for item text and creates new root item in tree
     */

    bool _inputAccepted = false;                          // Flag indicating if user accepted input dialog
    QString _itemText = QInputDialog::getText(           // User input text for new item
        this,
        "Add Root Item",                                  // Dialog title
        "Enter text for new root item:",                  // Dialog prompt
        QLineEdit::Normal,                                // Input mode
        QString("Root Item %1").arg(RootItemCounter),     // Default text with counter
        &_inputAccepted                                   // Output flag for user acceptance
        );

    if (_inputAccepted && !_itemText.trimmed().isEmpty()) {
        TreeWidget->AddRootItem(_itemText.trimmed());     // Add the root item to tree widget
        RootItemCounter++;                                // Increment counter for next item
        StatusLabel->setText(QString("Added root item: %1").arg(_itemText.trimmed()));  // Update status
    }
}

void MainWindow::OnLoadDemoButtonClicked()
{
    /**
     * Handles Load Demo button click event
     * Loads predefined demonstration data into the tree widget
     */

    TreeWidget->LoadDemoData();                           // Load sample data into tree
    StatusLabel->setText("Demo data loaded successfully"); // Update status message
}

void MainWindow::OnClearAllButtonClicked()
{
    /**
     * Handles Clear All button click event
     * Confirms with user and removes all items from tree if confirmed
     */

    int _userResponse = QMessageBox::question(            // User's response to confirmation dialog
        this,
        "Clear All Items",                                // Dialog title
        "Are you sure you want to remove all items from the tree?\n\nThis action cannot be undone.",  // Dialog message
        QMessageBox::Yes | QMessageBox::No,               // Available buttons
        QMessageBox::No                                   // Default button (safer option)
        );

    if (_userResponse == QMessageBox::Yes) {
        TreeWidget->ClearAllItems();                      // Clear all items from tree
        RootItemCounter = 1;                              // Reset root item counter
        StatusLabel->setText("All items cleared");        // Update status message
    }
}

void MainWindow::OnEditModeToggled(bool checked)
{
    /**
     * Handles Edit Mode checkbox state change
     * Enables or disables editing functionality in the tree widget
     * @param checked New checkbox state (true = editing enabled, false = editing disabled)
     */

    IsEditModeEnabled = checked;                          // Update internal edit mode flag
    TreeWidget->SetEditMode(IsEditModeEnabled);           // Update tree widget edit mode

    QString _statusMessage = IsEditModeEnabled ?          // Status message based on new state
                                 "Edit mode enabled - items can be modified" :
                                 "Edit mode disabled - items are read-only";

    StatusLabel->setText(_statusMessage);                 // Update status label
}

void MainWindow::OnTreeItemEdited(const QString &itemText)
{
    /**
     * Handles tree item edited signal from tree widget
     * Updates status to show which item was modified
     * @param itemText Text of the item that was edited
     */

    StatusLabel->setText(QString("Item edited: %1").arg(itemText));  // Update status with edited item info
}

void MainWindow::OnTreeItemDeleted(const QString &itemText)
{
    /**
     * Handles tree item deleted signal from tree widget
     * Updates status to show which item was removed
     * @param itemText Text of the item that was deleted
     */

    StatusLabel->setText(QString("Item deleted: %1").arg(itemText)); // Update status with deleted item info
}
