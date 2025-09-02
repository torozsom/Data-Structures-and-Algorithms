#include "controls.h"


int runApp(int& argc, char** argv) {
    QApplication app{argc, argv};
    ui::MainWindow window;
    window.show();
    return QApplication::exec();
}
