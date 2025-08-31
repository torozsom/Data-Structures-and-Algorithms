#ifndef ARRAY_WINDOW_HPP
#define ARRAY_WINDOW_HPP


#include <QMenuBar>
#include <QPointer>
#include <QVBoxLayout>
#include <QWidget>

#include "./ui_ArrayPage.h"
#include "ArrayAnimations.h"
#include "MainWindow.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class ArrayPage;
}
QT_END_NAMESPACE


namespace ui {


class ArrayPage final : public QWidget {
    Q_OBJECT

    Ui::ArrayPage* uiForm_;

    QPointer<SearchAnimator> animator_{};
    QPointer<QWidget> content_{};


  private slots:
    void showLinearSearch();
    void showBinarySearch();

  private:
    void connectButtonActions();


  public:
    explicit ArrayPage(QWidget* parent = nullptr);
    static void replaceContent(QWidget* container, QWidget* newChild);
    ~ArrayPage() override;
};


} // namespace ui


#endif // ARRAY_WINDOW_HPP
