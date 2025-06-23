#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include "a_tree.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow
     * @param parent Parent widget pointer - can be nullptr for top-level window
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow
     */
    ~MainWindow();

private slots:
    /**
     * @brief Slot triggered when Add Root button is clicked
     * Creates a new root item in the tree widget
     */
    void OnAddRootButtonClicked();

    /**
     * @brief Slot triggered when Load Demo button is clicked
     * Loads predefined demo data into the tree widget
     */
    void OnLoadDemoButtonClicked();

    /**
     * @brief Slot triggered when Clear All button is clicked
     * Removes all items from the tree widget
     */
    void OnClearAllButtonClicked();

    /**
     * @brief Slot triggered when Edit Mode checkbox state changes
     * Enables or disables edit functionality in the tree widget
     */
    void OnEditModeToggled(bool checked);

    /**
     * @brief Slot triggered when tree widget signals an item was edited
     * @param itemText New text of the edited item
     */
    void OnTreeItemEdited(const QString &itemText);

    /**
     * @brief Slot triggered when tree widget signals an item was deleted
     * @param itemText Text of the deleted item
     */
    void OnTreeItemDeleted(const QString &itemText);

private:
    /**
     * @brief Sets up the user interface components
     * Creates and arranges all UI elements including layouts, buttons, and tree widget
     */
    void SetupUserInterface();

    /**
     * @brief Applies dark theme styling to all UI components
     * Sets colors, fonts, and visual appearance for professional look
     */
    void SetApplicationStyle();

    /**
     * @brief Connects signals and slots between UI components
     * Establishes communication between buttons, checkboxes, and tree widget
     */
    void ConnectSignalsAndSlots();

    // UI Components
    A_Tree *TreeWidget;              // Main tree widget - displays hierarchical data structure with checkboxes
    QWidget *CentralWidget;          // Central widget container - holds all main UI components
    QVBoxLayout *MainLayout;         // Main vertical layout - organizes UI components top to bottom
    QHBoxLayout *ButtonLayout;       // Button layout - arranges control buttons horizontally
    QHBoxLayout *ControlLayout;      // Control layout - arranges control elements horizontally

    // Control Buttons
    QPushButton *AddRootButton;      // Add Root button - creates new root level items in tree
    QPushButton *LoadDemoButton;     // Load Demo button - populates tree with sample data
    QPushButton *ClearAllButton;     // Clear All button - removes all items from tree

    // Control Elements
    QLabel *TitleLabel;              // Title label - displays application title
    QCheckBox *EditModeCheckbox;     // Edit mode checkbox - enables/disables item editing (true = editable, false = read-only)
    QLabel *StatusLabel;             // Status label - shows current operation status and feedback
    QGroupBox *ControlGroup;         // Control group - organizes control elements visually

    // State Variables
    int RootItemCounter;             // Root item counter - tracks number of root items created (range: 1 to INT_MAX)
    bool IsEditModeEnabled;          // Edit mode flag - indicates if editing is currently allowed (true = enabled, false = disabled)
};

#endif // MAINWINDOW_H
