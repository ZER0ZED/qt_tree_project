#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include "a_tree.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("A_Tree Widget Demo");
        setGeometry(100, 100, 600, 400);

        // Create central widget
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // Create layout
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // Add title
        QLabel *titleLabel = new QLabel("Custom Tree Widget with Checkboxes", this);
        titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
        mainLayout->addWidget(titleLabel);

        // Create the tree widget
        m_tree = new A_Tree(this);
        mainLayout->addWidget(m_tree);

        // Add some demo buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        QPushButton *addRootBtn = new QPushButton("Add Root Item", this);
        QPushButton *loadDemoBtn = new QPushButton("Load Demo Data", this);
        QPushButton *clearBtn = new QPushButton("Clear All", this);

        buttonLayout->addWidget(addRootBtn);
        buttonLayout->addWidget(loadDemoBtn);
        buttonLayout->addWidget(clearBtn);
        buttonLayout->addStretch();

        mainLayout->addLayout(buttonLayout);

        // Add instructions
        QLabel *instructionsLabel = new QLabel(
            "Instructions:\n"
            "• Right-click to add/edit/delete items\n"
            "• Check/uncheck items to see tri-state behavior\n"
            "• Parent items show partial state when some children are checked\n"
            "• Edited items appear in italic", this);
        instructionsLabel->setStyleSheet("margin: 10px; padding: 10px; background-color: #f0f0f0; border-radius: 5px;");
        mainLayout->addWidget(instructionsLabel);

        // Connect buttons using lambda functions
        connect(addRootBtn, &QPushButton::clicked, [this]() {
            static int counter = 1;
            m_tree->addRootItem(QString("Root Item %1").arg(counter++));
        });

        connect(loadDemoBtn, &QPushButton::clicked, [this]() {
            loadDemoData();
        });

        connect(clearBtn, &QPushButton::clicked, [this]() {
            m_tree->clear();
        });

        // Load some initial demo data
        loadDemoData();
    }

private:
    void loadDemoData()
    {
        m_tree->clear();

        // Create some demo tree structure
        QTreeWidgetItem *root1 = m_tree->addRootItem("Documents");
        m_tree->addChildItem(root1, "Work Files");
        m_tree->addChildItem(root1, "Personal Files");
        QTreeWidgetItem *child = m_tree->addChildItem(root1, "Projects");
        m_tree->addChildItem(child, "Project A");
        m_tree->addChildItem(child, "Project B");

        QTreeWidgetItem *root2 = m_tree->addRootItem("Media");
        m_tree->addChildItem(root2, "Photos");
        m_tree->addChildItem(root2, "Videos");
        m_tree->addChildItem(root2, "Music");

        QTreeWidgetItem *root3 = m_tree->addRootItem("Settings");
        QTreeWidgetItem *userSettings = m_tree->addChildItem(root3, "User Settings");
        m_tree->addChildItem(userSettings, "Preferences");
        m_tree->addChildItem(userSettings, "Profile");
        m_tree->addChildItem(root3, "System Settings");
    }

    A_Tree *m_tree;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
