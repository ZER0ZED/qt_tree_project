#include "a_tree.h"

A_Tree::A_Tree(QWidget *parent)
    : QWidget(parent), m_currentItem(nullptr), m_editFlag(false), m_rootCounter(1)
{
    setupUI();
    setupContextMenu();
    // Removed setupCheckboxStyling() call

    // Connect tree widget signals
    connect(m_treeWidget, &QTreeWidget::itemChanged, this, &A_Tree::onItemChanged);
    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, this, &A_Tree::showContextMenu);

    // Connect button signals
    connect(m_addRootBtn, &QPushButton::clicked, this, &A_Tree::onAddRootClicked);
    connect(m_loadDemoBtn, &QPushButton::clicked, this, &A_Tree::onLoadDemoClicked);
    connect(m_clearBtn, &QPushButton::clicked, this, &A_Tree::onClearAllClicked);

    // Load initial demo data
    loadDemoData();
}

void A_Tree::setupUI()
{
    // Set dark gray background for the widget
    setStyleSheet("A_Tree { background-color: #3a3a3a; }");

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Create title label
    m_titleLabel = new QLabel("Tree Widget", this);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #FF8C00; background-color: transparent;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);

    // Create tree widget
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel("Tree Items");
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Set tree widget styling for dark theme
    m_treeWidget->setStyleSheet(
        "QTreeWidget {"
        "    background-color: #404040;"
        "    color: white;"
        "    border: 1px solid #555555;"
        "    selection-background-color: #0078d4;"
        "}"
        "QTreeWidget::item {"
        "    padding: 2px;"
        "    border: none;"
        "}"
        "QTreeWidget::item:selected {"
        "    background-color: #0078d4;"
        "}"
        "QTreeWidget::item:hover {"
        "    background-color: #4a4a4a;"
        "}"
        "QHeaderView::section {"
        "    background-color: #505050;"
        "    color: white;"
        "    border: 1px solid #666666;"
        "    padding: 4px;"
        "}"
        );

    mainLayout->addWidget(m_treeWidget);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_addRootBtn = new QPushButton("Add Root Item", this);
    m_loadDemoBtn = new QPushButton("Load Demo Data", this);
    m_clearBtn = new QPushButton("Clear All", this);

    // Style buttons
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #3498db;"
        "    border: none;"
        "    color: white;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #21618c;"
        "}";

    m_addRootBtn->setStyleSheet(buttonStyle);
    m_loadDemoBtn->setStyleSheet(buttonStyle);
    m_clearBtn->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(m_addRootBtn);
    buttonLayout->addWidget(m_loadDemoBtn);
    buttonLayout->addWidget(m_clearBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

// Removed setupCheckboxStyling() method entirely

void A_Tree::setupContextMenu()
{
    m_contextMenu = new QMenu(this);

    m_addRootAction = new QAction("Add Root Item", this);
    m_addChildAction = new QAction("Add Child Item", this);
    m_editAction = new QAction("Edit Item", this);
    m_deleteAction = new QAction("Delete Item", this);

    m_contextMenu->addAction(m_addRootAction);
    m_contextMenu->addAction(m_addChildAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_editAction);
    m_contextMenu->addAction(m_deleteAction);

    connect(m_addRootAction, &QAction::triggered, this, &A_Tree::addRootAction);
    connect(m_addChildAction, &QAction::triggered, this, &A_Tree::addChildAction);
    connect(m_editAction, &QAction::triggered, this, &A_Tree::editAction);
    connect(m_deleteAction, &QAction::triggered, this, &A_Tree::deleteAction);
}

QTreeWidgetItem* A_Tree::addRootItem(const QString &text)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, text);
    item->setCheckState(0, Qt::Unchecked);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

    m_treeWidget->addTopLevelItem(item);
    return item;
}

QTreeWidgetItem* A_Tree::addChildItem(QTreeWidgetItem *parent, const QString &text)
{
    if (!parent) return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, text);
    item->setCheckState(0, Qt::Unchecked);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

    parent->addChild(item);
    parent->setExpanded(true);

    // Update parent state after adding child
    updateParentCheckState(item);

    return item;
}

void A_Tree::deleteItem(QTreeWidgetItem *item)
{
    if (!item) return;

    QTreeWidgetItem *parent = item->parent();

    // Remove the item
    if (parent) {
        parent->removeChild(item);
        // Update parent state after removing child
        if (parent->childCount() > 0) {
            updateParentCheckState(parent->child(0));
        } else {
            updateParentCheckState(parent);
        }
    } else {
        int index = m_treeWidget->indexOfTopLevelItem(item);
        m_treeWidget->takeTopLevelItem(index);
    }

    delete item;
}

void A_Tree::editItem(QTreeWidgetItem *item)
{
    if (!item) return;

    bool ok;
    QString newText = QInputDialog::getText(this, "Edit Item",
                                            "Enter new text:",
                                            QLineEdit::Normal,
                                            item->text(0), &ok);

    if (ok && !newText.isEmpty()) {
        m_editFlag = true; // Set edit flag
        item->setText(0, newText);

        // You can store additional edit information in the item's data
        item->setData(0, Qt::UserRole, "edited");

        // Visual indication that item was edited (optional)
        QFont font = item->font(0);
        font.setItalic(true);
        item->setFont(0, font);

        m_editFlag = false;
    }
}

void A_Tree::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (!item || column != 0 || m_editFlag) return;

    // Block signals to prevent infinite recursion
    m_treeWidget->blockSignals(true);

    Qt::CheckState state = item->checkState(0);

    // Update all children to match parent state
    if (state != Qt::PartiallyChecked) {
        updateChildrenCheckState(item);
    }

    // Update parent states up the tree
    updateParentCheckState(item);

    m_treeWidget->blockSignals(false);
}

void A_Tree::updateChildrenCheckState(QTreeWidgetItem *item)
{
    if (!item) return;

    Qt::CheckState parentState = item->checkState(0);
    if (parentState == Qt::PartiallyChecked) return;

    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);
        child->setCheckState(0, parentState);
        updateChildrenCheckState(child); // Recursive for nested children
    }
}

void A_Tree::updateParentCheckState(QTreeWidgetItem *item)
{
    if (!item) return;

    QTreeWidgetItem *parent = item->parent();
    if (!parent) return;

    Qt::CheckState newState = calculateParentCheckState(parent);
    parent->setCheckState(0, newState);

    // Continue up the tree
    updateParentCheckState(parent);
}

Qt::CheckState A_Tree::calculateParentCheckState(QTreeWidgetItem *parent)
{
    if (!parent || parent->childCount() == 0) {
        return Qt::Unchecked;
    }

    int checkedCount = 0;
    int partialCount = 0;
    int totalCount = parent->childCount();

    for (int i = 0; i < totalCount; ++i) {
        QTreeWidgetItem *child = parent->child(i);
        Qt::CheckState childState = child->checkState(0);

        if (childState == Qt::Checked) {
            checkedCount++;
        } else if (childState == Qt::PartiallyChecked) {
            partialCount++;
        }
    }

    // Determine parent state based on children
    if (partialCount > 0 || (checkedCount > 0 && checkedCount < totalCount)) {
        return Qt::PartiallyChecked; // "Glowing box" effect
    } else if (checkedCount == totalCount) {
        return Qt::Checked;
    } else {
        return Qt::Unchecked;
    }
}

void A_Tree::showContextMenu(const QPoint &pos)
{
    m_currentItem = m_treeWidget->itemAt(pos);

    // Enable/disable actions based on context
    m_addChildAction->setEnabled(m_currentItem != nullptr);
    m_editAction->setEnabled(m_currentItem != nullptr);
    m_deleteAction->setEnabled(m_currentItem != nullptr);

    m_contextMenu->exec(m_treeWidget->mapToGlobal(pos));
}

void A_Tree::addRootAction()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Add Root Item",
                                         "Enter item text:",
                                         QLineEdit::Normal,
                                         "New Root", &ok);

    if (ok && !text.isEmpty()) {
        addRootItem(text);
    }
}

void A_Tree::addChildAction()
{
    if (!m_currentItem) return;

    bool ok;
    QString text = QInputDialog::getText(this, "Add Child Item",
                                         "Enter item text:",
                                         QLineEdit::Normal,
                                         "New Child", &ok);

    if (ok && !text.isEmpty()) {
        addChildItem(m_currentItem, text);
    }
}

void A_Tree::editAction()
{
    editItem(m_currentItem);
}

void A_Tree::deleteAction()
{
    if (!m_currentItem) return;

    int ret = QMessageBox::question(this, "Delete Item",
                                    QString("Are you sure you want to delete '%1'?")
                                        .arg(m_currentItem->text(0)),
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        deleteItem(m_currentItem);
        m_currentItem = nullptr;
    }
}

void A_Tree::loadDemoData()
{
    m_treeWidget->clear();

    // Create some demo tree structure
    QTreeWidgetItem *root1 = addRootItem("Documents");
    addChildItem(root1, "Work Files");
    addChildItem(root1, "Personal Files");
    QTreeWidgetItem *child = addChildItem(root1, "Projects");
    addChildItem(child, "Project A");
    addChildItem(child, "Project B");

    QTreeWidgetItem *root2 = addRootItem("Media");
    addChildItem(root2, "Photos");
    addChildItem(root2, "Videos");
    addChildItem(root2, "Music");

    QTreeWidgetItem *root3 = addRootItem("Settings");
    QTreeWidgetItem *userSettings = addChildItem(root3, "User Settings");
    addChildItem(userSettings, "Preferences");
    addChildItem(userSettings, "Profile");
    addChildItem(root3, "System Settings");
}

void A_Tree::clearAll()
{
    m_treeWidget->clear();
}

// Button slot methods
void A_Tree::onAddRootClicked()
{
    addRootItem(QString("Root Item %1").arg(m_rootCounter++));
}

void A_Tree::onLoadDemoClicked()
{
    loadDemoData();
}

void A_Tree::onClearAllClicked()
{
    clearAll();
}
