#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

#include "ArrayAnimations.h"


namespace ui {


/// Main window of the application
class MainWindow final : public QMainWindow {

    Q_OBJECT
    SearchAnimator* animator_{};


private slots:
  /// Show linear search animation
  void showLinearSearch() {
    auto [view, animator] = createLinearSearchAnimation();
    if (auto* old = takeCentralWidget())
        old->deleteLater();
    animator_ = nullptr;
    setCentralWidget(view);
    animator_ = std::move(animator);
    setWindowTitle("Algorithms Visualizer - Linear Search");
}

    /// Show binary search animation
    void showBinarySearch() {
    auto [view, animator] = createBinarySearchAnimation();
    if (auto* old = takeCentralWidget())
        old->deleteLater();
    animator_ = nullptr;
    setCentralWidget(view);
    animator_ = std::move(animator);
    setWindowTitle("Algorithms Visualizer - Binary Search");
}


private:
    /// Create the menu bar
    void createMenu() {
        auto* animations = menuBar()->addMenu("&Animations");
        const auto* linear = animations->addAction("Linear Search");
        const auto* binary = animations->addAction("Binary Search");

        connect(linear, &QAction::triggered, this,
                &MainWindow::showLinearSearch);
        connect(binary, &QAction::triggered, this,
                &MainWindow::showBinarySearch);
    }


public:
    /// Constructor
    explicit MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        createMenu();
        setWindowTitle("Algorithms Visualizer");
    }

    /// Destructor
    ~MainWindow() override { delete animator_; };
};


} // namespace ui


#endif // MAIN_WINDOW_HPP
