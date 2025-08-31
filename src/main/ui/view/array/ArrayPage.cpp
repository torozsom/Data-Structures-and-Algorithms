#include "ArrayPage.h"


namespace ui {


/// Swap the current content widget with the provided one inside content_
void ArrayPage::replaceContent(QWidget* container, QWidget* newChild) {
    QPointer boxLayout{container->layout()};
    if (!boxLayout) {
        boxLayout = new QVBoxLayout(container);
        boxLayout->setContentsMargins(0, 0, 0, 0);
    }
    while (boxLayout->count() > 0) {
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
    if (!content_) content_ = this;
    replaceContent(content_, view);
    animator_ = nullptr;
    animator_ = animator;
    if (auto* w = window())
        w->setWindowTitle("Dynamic Array - Linear Search");
}


/// Show binary search animation
void ArrayPage::showBinarySearch() {
    auto [view, animator] = createBinarySearchAnimation();
    if (!content_) content_ = this;
    replaceContent(content_, view);
    animator_ = nullptr;
    animator_ = animator;
    if (auto* w = window())
        w->setWindowTitle("Dynamic Array - Linear Search");
}


void ArrayPage::connectButtonActions() {
    //connect(uiForm_->btnLinearSearch, &QPushButton::clicked, this, &ArrayPage::hide);
    connect(uiForm_->btnLinearSearch, &QPushButton::clicked, this, &ArrayPage::showLinearSearch);
    connect(uiForm_->btnBinarySearch, &QPushButton::clicked, this, &ArrayPage::showBinarySearch);
}


/// Constructor that initializes the UI and creates the menu
ArrayPage::ArrayPage(QWidget* parent)
    : QWidget(parent), uiForm_(new Ui::ArrayPage) {
    uiForm_->setupUi(this);
    connectButtonActions();
}


/// Destructor that cleans up the UI form
ArrayPage::~ArrayPage() { delete uiForm_; };


} // namespace ui
