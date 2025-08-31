#include "ArrayPage.h"


namespace ui {


/// Swap the current content widget with the provided one inside content_
void ArrayPage::replaceContent(QWidget* container, QWidget* newChild) {
    QPointer boxLayout{container->layout()};
    if (!boxLayout) {
        boxLayout = new QVBoxLayout(container);
        boxLayout->setContentsMargins(0, 0, 0, 0);
    }
    if (boxLayout->count() > 0) {
        const auto* item = boxLayout->takeAt(0);
        if (auto* old = item->widget())
            old->deleteLater();
        delete item;
    }
    boxLayout->addWidget(newChild);
}


/// Show linear search animation
void ArrayPage::showLinearSearch() {
    auto [view, animator] = createLinearSearchAnimation();
    replaceContent(content_, view);
    animator_ = nullptr;
    animator_ = animator;
    if (auto* w = window())
        w->setWindowTitle("Dynamic Array - Linear Search");
}


/// Show binary search animation
void ArrayPage::showBinarySearch() {
    auto [view, animator] = createBinarySearchAnimation();
    replaceContent(content_, view);
    animator_ = nullptr;
    animator_ = animator;
    if (auto* w = window())
        w->setWindowTitle("Dynamic Array - Linear Search");
}


/// Create the menu bar
void ArrayPage::createMenu() {
    auto* animations = menuBar_->addMenu("Animations");
    const auto* linear = animations->addAction("Linear Search");
    const auto* binary = animations->addAction("Binary Search");

    connect(linear, &QAction::triggered, this, &ArrayPage::showLinearSearch);
    connect(binary, &QAction::triggered, this, &ArrayPage::showBinarySearch);
}


/// Constructor that initializes the UI and creates the menu
ArrayPage::ArrayPage(QWidget* parent)
    : QWidget(parent), uiForm_(new Ui::ArrayPage) {
    uiForm_->setupUi(this);
    resize(800, 200);
    const QPointer root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);

    menuBar_ = new QMenuBar(this);
    root->setMenuBar(menuBar_);

    content_ = new QWidget(this);
    content_->setLayout(new QVBoxLayout(content_));
    content_->layout()->setContentsMargins(0, 0, 0, 0);
    root->addWidget(content_);

    createMenu();
}


/// Destructor that cleans up the UI form
ArrayPage::~ArrayPage() { delete uiForm_; };


} // namespace ui
