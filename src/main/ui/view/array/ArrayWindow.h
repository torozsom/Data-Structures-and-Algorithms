#ifndef ARRAY_WINDOW_HPP
#define ARRAY_WINDOW_HPP


#include <QMainWindow>

#include "./ui_ArrayWindow.h"
#include "ArrayAnimations.h"
#include "MainWindow.h"

#include <QMenuBar>
#include <QPointer>


QT_BEGIN_NAMESPACE
namespace Ui {
class ArrayWindow;
}
QT_END_NAMESPACE


namespace ui {


class ArrayWindow final : public QMainWindow {
    Q_OBJECT

    Ui::ArrayWindow* uiForm_;
    QPointer<SearchAnimator> animator_{};


  private slots:
    void showLinearSearch();
    void showBinarySearch();


  private:
    void createMenu();


  public:
    explicit ArrayWindow(QWidget* parent = nullptr);
    ~ArrayWindow() override;
};


} // namespace ui


#endif // ARRAY_WINDOW_HPP
