#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPointer>
#include <QPushButton>

#include "./ui_MainWindow.h"
#include "ArrayAnimations.h"
#include "ArrayPage.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


namespace ui {


/**
 * @brief Main entry window for the application.
 *
 * Hosts the initial menu with buttons to launch the various algorithm
 * demonstrations. The central widget is replaced with the appropriate page
 * when an option is selected.
 */
class MainWindow final : public QMainWindow {
    Q_OBJECT

    Ui::MainWindow* uiForm_;

    QPointer<QWidget> page_{};
    QPointer<QWidget> centralContent_{};


  private slots:
    void on_btnDynamicArray_clicked();


  private:
    void connectButtonActions();


  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
};


} // namespace ui


#endif // MAIN_WINDOW_H
