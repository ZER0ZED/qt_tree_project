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

    MainWindow window;
    window.show();

    return app.exec();
}
