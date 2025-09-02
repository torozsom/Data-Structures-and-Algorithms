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


/**
 * @brief Page displaying array algorithm animations and controls.
 *
 * Hosts buttons to launch specific algorithm visualizations and replaces its
 * content area with the selected animation view. The page can be restored to
 * its original state via the provided Back buttons.
 */
class ArrayPage final : public QWidget {
    Q_OBJECT

    Ui::ArrayPage* uiForm_;

    QPointer<QObject> animator_{};
    QPointer<QWidget> content_{};


  private:
    void connectButtonActions();
    void setupAndShowAnimation(QWidget* view, QObject* animator,
                               const QString& windowTitle);


  private slots:
    void showLinearSearch();
    void showBinarySearch();
    void showBubbleSort();
    void showImprovedBubbleSort();
    void showInsertSortLS();
    void restoreUI();


  public:
    explicit ArrayPage(QWidget* parent = nullptr);
    static void replaceContent(QWidget* container, QWidget* newChild);
    ~ArrayPage() override;
};


} // namespace ui


#endif // ARRAY_WINDOW_HPP
