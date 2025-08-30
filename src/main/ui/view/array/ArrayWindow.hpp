#ifndef ARRAY_WINDOW_HPP
#define ARRAY_WINDOW_HPP


#include <QMainWindow>

#include "./ui_ArrayWindow.h"
#include "ArrayAnimations.h"
#include "MainWindow.hpp"

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
    /// Show linear search animation
    void showLinearSearch() {
        auto [view, animator] = createLinearSearchAnimation();
        if (auto* old = takeCentralWidget())
            old->deleteLater();
        animator_ = nullptr;
        setCentralWidget(view);
        animator_ = animator;
        setWindowTitle("Algorithms Visualizer - Linear Search");
    }

    /// Show binary search animation
    void showBinarySearch() {
        auto [view, animator] = createBinarySearchAnimation();
        if (auto* old = takeCentralWidget())
            old->deleteLater();
        animator_ = nullptr;
        setCentralWidget(view);
        animator_ = animator;
        setWindowTitle("Algorithms Visualizer - Binary Search");
    }


  private:
    /// Create the menu bar
    void createMenu() {
        auto* animations = menuBar()->addMenu("Animations");
        const auto* linear = animations->addAction("Linear Search");
        const auto* binary = animations->addAction("Binary Search");

        connect(linear, &QAction::triggered, this,
                &ArrayWindow::showLinearSearch);
        connect(binary, &QAction::triggered, this,
                &ArrayWindow::showBinarySearch);
    }


  public:
    explicit ArrayWindow(QWidget* parent = nullptr)
        : QMainWindow(parent), uiForm_(new Ui::ArrayWindow) {
        uiForm_->setupUi(this);
        resize(800, 200);
        createMenu();
    }

    ~ArrayWindow() override { delete uiForm_; }
};

} // namespace ui


#endif // ARRAY_WINDOW_HPP
