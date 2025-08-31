#include "ArrayWindow.h"


namespace ui {


/// Show linear search animation
void ArrayWindow::showLinearSearch() {
    auto [view, animator] = createLinearSearchAnimation();
    if (auto* old = takeCentralWidget())
        old->deleteLater();
    animator_ = nullptr;
    setCentralWidget(view);
    animator_ = animator;
    setWindowTitle("Algorithms Visualizer - Linear Search");
}


/// Show binary search animation
void ArrayWindow::showBinarySearch() {
    auto [view, animator] = createBinarySearchAnimation();
    if (auto* old = takeCentralWidget())
        old->deleteLater();
    animator_ = nullptr;
    setCentralWidget(view);
    animator_ = animator;
    setWindowTitle("Algorithms Visualizer - Binary Search");
}


/// Create the menu bar
void ArrayWindow::createMenu() {
    auto* animations = menuBar()->addMenu("Animations");
    const auto* linear = animations->addAction("Linear Search");
    const auto* binary = animations->addAction("Binary Search");

    connect(linear, &QAction::triggered, this, &ArrayWindow::showLinearSearch);
    connect(binary, &QAction::triggered, this, &ArrayWindow::showBinarySearch);
}


/// Constructor that initializes the UI and creates the menu
ArrayWindow::ArrayWindow(QWidget* parent)
    : QMainWindow(parent), uiForm_(new Ui::ArrayWindow) {
    uiForm_->setupUi(this);
    resize(800, 200);
    createMenu();
}


/// Destructor that cleans up the UI form
ArrayWindow::~ArrayWindow() { delete uiForm_; };


} // namespace ui
