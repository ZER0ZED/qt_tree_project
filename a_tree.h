#ifndef A_TREE_H
#define A_TREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMenu>

class A_Tree : public QTreeWidget
{
    Q_OBJECT

public:
    explicit A_Tree(QWidget *parent = nullptr);

    // Main functionality methods
    QTreeWidgetItem* addRootItem(const QString &text);
    QTreeWidgetItem* addChildItem(QTreeWidgetItem *parent, const QString &text);
    void deleteItem(QTreeWidgetItem *item);
    void editItem(QTreeWidgetItem *item);

    // Checkbox state management
    void updateParentCheckState(QTreeWidgetItem *item);
    void updateChildrenCheckState(QTreeWidgetItem *item);

private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);
    void showContextMenu(const QPoint &pos);
    void addRootAction();
    void addChildAction();
    void editAction();
    void deleteAction();

private:
    void setupContextMenu();
    void setItemCheckState(QTreeWidgetItem *item, Qt::CheckState state);
    Qt::CheckState calculateParentCheckState(QTreeWidgetItem *parent);

    // Context menu
    QMenu *m_contextMenu;
    QAction *m_addRootAction;
    QAction *m_addChildAction;
    QAction *m_editAction;
    QAction *m_deleteAction;

    QTreeWidgetItem *m_currentItem; // Item that was right-clicked

    // Edit flag for items
    bool m_editFlag;
};

#endif // A_TREE_H
