#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP


#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPointer>
#include <QPushButton>

#include "./ui_MainWindow.h"
#include "ArrayAnimations.h"
#include "ArrayWindow.hpp"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


namespace ui {


/// Main window of the application
class MainWindow final : public QMainWindow {

    Q_OBJECT

    Ui::MainWindow* uiForm_;
    QPointer<QMainWindow> window_{};


  private slots:
    void on_btnDynamicArray_clicked() {
        if (!window_) {
            window_ = new ArrayWindow(this);
            window_->setAttribute(Qt::WA_DeleteOnClose);
            connect(window_, &QObject::destroyed, this, [this] {
                this->show();
                window_ = nullptr;
            });
        }
        window_->show();
        window_->raise();
        window_->activateWindow();
        this->hide();
    }


  private:
    void connectButtonActions() {
        connect(uiForm_->btnDynamicArray, &QPushButton::clicked, this,
                &MainWindow::on_btnDynamicArray_clicked);
    }

  public:
    /// Constructor
    explicit MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent), uiForm_(new Ui::MainWindow) {
        uiForm_->setupUi(this);
        connectButtonActions();
    }

    /// Destructor
    ~MainWindow() override { delete uiForm_; }
};


} // namespace ui


#endif // MAIN_WINDOW_HPP
