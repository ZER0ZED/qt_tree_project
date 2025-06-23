#include <QApplication>
#include <QMainWindow>
#include "a_tree.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("A_Tree Widget Demo");
        setGeometry(100, 100, 600, 400);

        // Set dark gray background for the main window
        setStyleSheet("QMainWindow { background-color: #2c2c2c; }");

        // Create the tree widget as central widget
        m_tree = new A_Tree(this);
        setCentralWidget(m_tree);
    }

private:
    A_Tree *m_tree;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Option 1: Set application-wide dark theme (uncomment if you want this)
    // app.setStyleSheet("QWidget { background-color: #2c2c2c; color: white; }");

    MainWindow window;
    window.show();

    return app.exec();
}
