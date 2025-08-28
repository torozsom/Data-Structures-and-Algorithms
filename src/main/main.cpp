#include <QApplication>
#include <QColor>
#include <QPalette>
#include <QStyleFactory>

#include "MainWindow.hpp"


int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor{53, 53, 53});
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor{42, 42, 42});
    darkPalette.setColor(QPalette::AlternateBase, QColor{66, 66, 66});
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor{53, 53, 53});
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Highlight, QColor{142, 45, 197}.lighter());
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);

    ui::MainWindow window;
    window.resize(600, 150);
    window.show();

    return QApplication::exec();
}
