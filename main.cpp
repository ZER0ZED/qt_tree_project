#include <QApplication>
#include "mainwindow.h"

/**
 * @brief Application entry point
 * @param argc Command line argument count
 * @param argv Command line arguments array
 * @return Application exit code (0 = success)
 */
int main(int argc, char *argv[])
{
    QApplication _app(argc, argv); // Qt application instance - manages the GUI application's control flow and main settings

    MainWindow _window; // Main application window instance - primary window containing all UI elements
    _window.show(); // Display the main window on screen

    return _app.exec(); // Start the application event loop and return exit code
}
