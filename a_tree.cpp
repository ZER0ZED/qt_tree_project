#include "a_tree.h"

A_Tree::A_Tree(QWidget *parent)
    : QTreeWidget(parent), m_currentItem(nullptr), m_editFlag(false)
{
    // Set up the tree widget
    setHeaderLabel("Tree Items");
    setContextMenuPolicy(Qt::CustomContextMenu);

    // Enable checkboxes for all items
    setSelectionMode(QAbstractItemView::SingleSelection);

    // Setup context menu
    setupContextMenu();

    // Connect signals
    connect(this, &QTreeWidget::itemChanged, this, &A_Tree::onItemChanged);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &A_Tree::showContextMenu);
}

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
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setText(0, text);
    item->setCheckState(0, Qt::Unchecked);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

    addTopLevelItem(item);
    return item;
}

QTreeWidgetItem* A_Tree::addChildItem(QTreeWidgetItem *parent, const QString &text)
{
    if (!parent) return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
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
        updateParentCheckState(parent->child(0));
    } else {
        int index = indexOfTopLevelItem(item);
        takeTopLevelItem(index);
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
    blockSignals(true);

    Qt::CheckState state = item->checkState(0);

    // Update all children to match parent state
    if (state != Qt::PartiallyChecked) {
        updateChildrenCheckState(item);
    }

    // Update parent states up the tree
    updateParentCheckState(item);

    blockSignals(false);
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
    m_currentItem = itemAt(pos);

    // Enable/disable actions based on context
    m_addChildAction->setEnabled(m_currentItem != nullptr);
    m_editAction->setEnabled(m_currentItem != nullptr);
    m_deleteAction->setEnabled(m_currentItem != nullptr);

    m_contextMenu->exec(mapToGlobal(pos));
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
