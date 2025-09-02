#include "ArrayPage.h"


namespace ui {


/// Connects button click signals to their respective slot functions
void ArrayPage::connectButtonActions() {
    connect(uiForm_->btnLinearSearch, &QPushButton::clicked, this,
            &ArrayPage::showLinearSearch);

    connect(uiForm_->btnBinarySearch, &QPushButton::clicked, this,
            &ArrayPage::showBinarySearch);

    connect(uiForm_->btnBubbleSort, &QPushButton::clicked, this,
            &ArrayPage::showBubbleSort);

    connect(uiForm_->btnImpBubbleSort, &QPushButton::clicked, this,
            &ArrayPage::showImprovedBubbleSort);

    connect(uiForm_->btnInsertSortLS, &QPushButton::clicked, this,
            &ArrayPage::showInsertSortLS);

    connect(uiForm_->btnInsertSortBS, &QPushButton::clicked, this,
            &ArrayPage::showInsertSortBS);

    connect(uiForm_->btnQuickSort, &QPushButton::clicked, this,
            &ArrayPage::showQuickSort);

    connect(uiForm_->btnMergeSort, &QPushButton::clicked, this,
            &ArrayPage::showMergeSort);

    connect(uiForm_->btnHeapSort, &QPushButton::clicked, this,
            &ArrayPage::showHeapSort);
}


/**
 * @brief Sets up and displays an animation view with a bottom bar.
 *
 * This function creates a container widget that holds the provided animation
 * view and a bottom bar with a "Back" button. The animation view expands to
 * fill the available space, and clicking the "Back" button restores the
 * original UI. The window title is also updated to reflect the current
 * animation.
 *
 * @param view The QWidget representing the animation view to be displayed.
 * @param animator The QObject responsible for controlling the animation.
 * @param windowTitle The title to set for the window when displaying the
 * animation.
 */
void ArrayPage::setupAndShowAnimation(QWidget* view, QObject* animator,
                                      const QString& windowTitle) {
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
        w->setWindowTitle(windowTitle);
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
    setupAndShowAnimation(view, animator, "Dynamic Array - Linear Search");
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
    setupAndShowAnimation(view, animator, "Dynamic Array - Binary Search");
}


/**
 * @brief Show bubble sort animation.
 *
 * Creates and displays the bubble sort animation within a container that
 * includes a bottom bar with a "Back" button. The animation view expands to
 * fill the available space, and clicking the "Back" button restores the
 * original UI.
 */
void ArrayPage::showBubbleSort() {
    auto [view, animator] = createBubbleSortAnimation();
    setupAndShowAnimation(view, animator, "Dynamic Array - Bubble Sort");
}


/**
 * @brief Show improved bubble sort animation.
 *
 * Creates and displays the improved bubble sort animation within a container
 * that includes a bottom bar with a "Back" button. The animation view expands
 * to fill the available space, and clicking the "Back" button restores the
 * original UI.
 */
void ArrayPage::showImprovedBubbleSort() {
    auto [view, animator] = createImprovedBubbleSortAnimation();
    setupAndShowAnimation(view, animator,
                          "Dynamic Array - Improved Bubble Sort");
}


/**
 * @brief Show the insertion sort animation with linear search.
 *
 * Creates and displays the insertion sort animation with linear search
 * within a container that includes a bottom bar with a "Back" button.
 * The animation view expands to fill the available space, and clicking
 * the "Back" button restores the original UI.
 */
void ArrayPage::showInsertSortLS() {
    auto [view, animator] = createInsertSortLSAnimation();
    setupAndShowAnimation(view, animator,
                          "Dynamic Array - Insertion Sort with Linear Search");
}


/**
 * @brief Show the insertion sort animation with binary search.
 *
 * Creates and displays the insertion sort animation with binary search
 * within a container that includes a bottom bar with a "Back" button.
 * The animation view expands to fill the available space, and clicking
 * the "Back" button restores the original UI.
 */
void ArrayPage::showInsertSortBS() {
    auto [view, animator] = createInsertSortBSAnimation();
    setupAndShowAnimation(view, animator,
                          "Dynamic Array - Insertion Sort with Binary Search");
}


/**
 * @brief Show quick sort animation.
 *
 * Creates and displays the quick sort animation within a container
 * that includes a bottom bar with a "Back" button. The animation view expands
 * to fill the available space, and clicking the "Back" button restores the
 * original UI.
 */
void ArrayPage::showQuickSort() {
    auto [view, animator] = createQuickSortAnimation();
    setupAndShowAnimation(view, animator, "Dynamic Array - Quick Sort");
}


/**
 * @brief Show merge sort animation.
 *
 * Creates and displays the merge sort animation within a container
 * that includes a bottom bar with a "Back" button. The animation view expands
 * to fill the available space, and clicking the "Back" button restores the
 * original UI.
 */
void ArrayPage::showMergeSort() {
    auto [view, animator] = createMergeSortAnimation();
    setupAndShowAnimation(view, animator, "Dynamic Array - Merge Sort");
}


/**
 * @brief Show heap sort animation.
 *
 * Creates and displays the heap sort animation within a container
 * that includes a bottom bar with a "Back" button. The animation view expands
 * to fill the available space, and clicking the "Back" button restores the
 * original UI.
 */
void ArrayPage::showHeapSort() {
    auto [view, animator] = createHeapSortAnimation();
    setupAndShowAnimation(view, animator, "Dynamic Array - Heap Sort");
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


/// Constructor that initializes the UI and creates the menu
ArrayPage::ArrayPage(QWidget* parent)
    : QWidget(parent), uiForm_(new Ui::ArrayPage) {
    uiForm_->setupUi(this);
    connectButtonActions();
}


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


/// Destructor that cleans up the UI form
ArrayPage::~ArrayPage() { delete uiForm_; }


} // namespace ui
