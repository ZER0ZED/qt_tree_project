#ifndef A_TREE_H
#define A_TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class A_Tree : public QWidget
{
    Q_OBJECT

public:
    explicit A_Tree(QWidget *parent = nullptr);

    // Main functionality methods
    QTreeWidgetItem* addRootItem(const QString &text);
    QTreeWidgetItem* addChildItem(QTreeWidgetItem *parent, const QString &text);
    void deleteItem(QTreeWidgetItem *item);
    void editItem(QTreeWidgetItem *item);
    void loadDemoData();
    void clearAll();

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
    void onAddRootClicked();
    void onLoadDemoClicked();
    void onClearAllClicked();

private:
    void setupUI();
    void setupContextMenu();
    void setupCheckboxStyling();
    Qt::CheckState calculateParentCheckState(QTreeWidgetItem *parent);

    // UI Components
    QTreeWidget *m_treeWidget;
    QLabel *m_titleLabel;
    QPushButton *m_addRootBtn;
    QPushButton *m_loadDemoBtn;
    QPushButton *m_clearBtn;

    // Context menu
    QMenu *m_contextMenu;
    QAction *m_addRootAction;
    QAction *m_addChildAction;
    QAction *m_editAction;
    QAction *m_deleteAction;

    QTreeWidgetItem *m_currentItem; // Item that was right-clicked

    // Edit flag for items
    bool m_editFlag;

    // Root counter for adding items
    int m_rootCounter;
};

#endif // A_TREE_H
