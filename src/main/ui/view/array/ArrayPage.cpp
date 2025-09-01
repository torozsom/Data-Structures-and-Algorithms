#include "ArrayPage.h"


namespace ui {


/**
 * @brief Replaces the content of a given container widget with a new child
 * widget.
 *
 * This function clears any existing layout and widgets within the container,
 * then adds the new child widget to it. If the container does not have a
 * layout, a new QVBoxLayout is created for it.
 *
 * @param container The QWidget container whose content is to be replaced.
 * @param newChild The new QWidget to be added to the container.
 */
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


/**
 * @brief Show linear search animation
 *
 * Creates and displays the linear search animation within a container that
 * includes a bottom bar with a "Back" button. The animation view expands to
 * fill the available space, and clicking the "Back" button restores the
 * original UI.
 */
void ArrayPage::showLinearSearch() {
    auto [view, animator] = createLinearSearchAnimation();
    if (!content_)
        content_ = this;

    // Build a container with the animation and a bottom-left button
    const QPointer container = new QWidget(content_);
    container->setAutoFillBackground(true);

    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    // Ensure the animation view expands and covers the area
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(view, 1); // animation takes all available space

    const QPointer bottomBar = new QWidget(container);
    bottomBar->setAutoFillBackground(true);
    const QPointer hbox = new QHBoxLayout(bottomBar);
    hbox->setContentsMargins(8, 8, 8, 8);
    hbox->setSpacing(8);

    const QPointer btn = new QPushButton("Back", bottomBar);
    connect(btn, &QPushButton::clicked, this, &ArrayPage::restoreUI);
    btn->setCursor(Qt::PointingHandCursor);
    hbox->addWidget(btn);
    hbox->addStretch();

    vbox->addWidget(bottomBar, 0);

    replaceContent(content_, container);

    animator_ = nullptr;
    animator_ = animator;
    if (auto* w = window())
        w->setWindowTitle("Dynamic Array - Linear Search");
}


/**
 * @brief Show binary search animation
 *
 * Creates and displays the binary search animation within a container that
 * includes a bottom bar with a "Back" button. The animation view expands to
 * fill the available space, and clicking the "Back" button restores the
 * original UI.
 */
void ArrayPage::showBinarySearch() {
    auto [view, animator] = createBinarySearchAnimation();
    if (!content_)
        content_ = this;

    // Build a container with the animation and a bottom-left button
    const QPointer container = new QWidget(content_);
    container->setAutoFillBackground(true);

    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(view, 1);

    const QPointer bottomBar = new QWidget(container);
    bottomBar->setAutoFillBackground(true);

    const QPointer hbox = new QHBoxLayout(bottomBar);
    hbox->setContentsMargins(8, 8, 8, 8);
    hbox->setSpacing(8);

    const QPointer btn = new QPushButton("Back", bottomBar);
    connect(btn, &QPushButton::clicked, this, &ArrayPage::restoreUI);
    btn->setCursor(Qt::PointingHandCursor);
    hbox->addWidget(btn);
    hbox->addStretch();

    vbox->addWidget(bottomBar, 0);

    replaceContent(content_, container);

    animator_ = nullptr;
    animator_ = animator;
    if (auto* w = window())
        w->setWindowTitle("Dynamic Array - Binary Search");
}


/**
 * @brief Show bubble sort animation
 */
void ArrayPage::showBubbleSort() {
    auto [view, animator] = createBubbleSortAnimation();
    if (!content_)
        content_ = this;

    const QPointer container = new QWidget(content_);
    container->setAutoFillBackground(true);

    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(view, 1);

    const QPointer bottomBar = new QWidget(container);
    bottomBar->setAutoFillBackground(true);
    const QPointer hbox = new QHBoxLayout(bottomBar);
    hbox->setContentsMargins(8, 8, 8, 8);
    hbox->setSpacing(8);

    const QPointer btn = new QPushButton("Back", bottomBar);
    connect(btn, &QPushButton::clicked, this, &ArrayPage::restoreUI);
    btn->setCursor(Qt::PointingHandCursor);
    hbox->addWidget(btn);
    hbox->addStretch();

    vbox->addWidget(bottomBar, 0);

    replaceContent(content_, container);

    animator_ = nullptr;
    animator_ = animator;
    if (auto* w = window())
        w->setWindowTitle("Dynamic Array - Bubble Sort");
}


/**
 * @brief Restores the original UI of the ArrayPage.
 *
 * This function clears any existing layout and widgets within the ArrayPage,
 * then reinitializes the UI components and reconnects button actions. It also
 * resets the window title to "Dynamic Array".
 */
void ArrayPage::restoreUI() {
    if (auto* lay = layout()) {
        while (lay->count() > 0) {
            const auto* item = lay->takeAt(0);
            if (auto* w = item->widget())
                w->deleteLater();
            delete item;
        }
        delete lay;
    }

    uiForm_->setupUi(this);
    connectButtonActions();

    if (auto* w = window())
        w->setWindowTitle("Dynamic Array");
}


/// Connects button click signals to their respective slot functions
void ArrayPage::connectButtonActions() {
    connect(uiForm_->btnLinearSearch, &QPushButton::clicked, this,
            &ArrayPage::showLinearSearch);
    connect(uiForm_->btnBinarySearch, &QPushButton::clicked, this,
            &ArrayPage::showBinarySearch);
    connect(uiForm_->btnBubbleSort, &QPushButton::clicked, this,
            &ArrayPage::showBubbleSort);
}


/// Constructor that initializes the UI and creates the menu
ArrayPage::ArrayPage(QWidget* parent)
    : QWidget(parent), uiForm_(new Ui::ArrayPage) {
    uiForm_->setupUi(this);
    connectButtonActions();
}


/// Destructor that cleans up the UI form
ArrayPage::~ArrayPage() { delete uiForm_; }


} // namespace ui
