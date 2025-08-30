#include "controls.h"


int runApp(int& argc, char** argv){
    QApplication app{argc, argv};

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    ui::MainWindow window;
    window.resize(600, 150);
    window.show();

    return QApplication::exec();
}
