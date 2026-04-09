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

    const QPointer btnBack = new QPushButton("Back", bottomBar);
    connect(btnBack, &QPushButton::clicked, this, &ArrayPage::restoreUI);
    btnBack->setCursor(Qt::PointingHandCursor);
    hbox->addWidget(btnBack);

    currentSpeedMultiplier_ = 1;
    const QPointer btnSpeed = new QPushButton("Speed 1x", bottomBar);
    btnSpeed->setCursor(Qt::PointingHandCursor);
    connect(btnSpeed, &QPushButton::clicked, this, [this, btnSpeed]() {
        switch (currentSpeedMultiplier_) {
            case 1:
                currentSpeedMultiplier_ = 2;
                break;
            case 2:
                currentSpeedMultiplier_ = 3;
                break;
            case 3:
                currentSpeedMultiplier_ = 4;
                break;
            case 4:
                currentSpeedMultiplier_ = 5;
                break;
            default:
                currentSpeedMultiplier_ = 1;
                break;
        }
        btnSpeed->setText(QString("Speed %1x").arg(currentSpeedMultiplier_));
        emit speedChanged(currentSpeedMultiplier_);
    });

    hbox->addWidget(btnSpeed);
    vbox->addWidget(bottomBar, 0);

    replaceContent(content_, container);

    animator_ = animator;

    if (animator_) {
        if (auto* sortAnim = qobject_cast<SortAnimator*>(animator_))
            connect(this, &ArrayPage::speedChanged, sortAnim, &SortAnimator::setSpeedMultiplier);
        else if (auto* searchAnim = qobject_cast<SearchAnimator*>(animator_))
            connect(this, &ArrayPage::speedChanged, searchAnim, &SearchAnimator::setSpeedMultiplier);
    }

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
    auto values = generateRandomDoubleArray();
    const double target = values[18];
    auto [view, animator] = createLinearSearchAnimation(values, target);
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
    auto values = generateSortedDoubleArray();
    const double target = values[18];
    auto [view, animator] = createBinarySearchAnimation(values, target);
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
    auto values = generateRandomDoubleArray();
    auto [view, animator] = createBubbleSortAnimation(values);
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
    auto values = generateRandomDoubleArray();
    auto [view, animator] = createImprovedBubbleSortAnimation(values);
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
    auto values = generateRandomDoubleArray();
    auto [view, animator] = createInsertSortLSAnimation(values);
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
    auto values = generateRandomDoubleArray();
    auto [view, animator] = createInsertSortBSAnimation(values);
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
    auto values = generateRandomDoubleArray();
    auto [view, animator] = createQuickSortAnimation(values);
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
    auto values = generateRandomDoubleArray();
    auto [view, animator] = createMergeSortAnimation(values);
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
    auto values = generateRandomDoubleArray();
    auto [view, animator] = createHeapSortAnimation(values);
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


/**
 * @brief Generates a DynamicArray of random double values.
 *
 * This function creates a DynamicArray of the specified size and fills it with
 * random double values uniformly distributed between 10.0 and 150.0. The random
 * number generator is seeded with a non-deterministic random device to ensure
 * different values each time the function is called.
 *
 * @param size The number of random double values to generate (default is 25).
 * @return A DynamicArray containing the generated random double values.
 */
containers::DynamicArray<double> generateRandomDoubleArray(const size_t size) {
    containers::DynamicArray<double> arr;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution dist(10.0, 150.0);

    for(size_t i = 0; i < size; ++i)
        arr.addLast(dist(rng));

    return arr;
}


/**
 * @brief Generates a DynamicArray of sorted double values.
 *
 * This function creates a DynamicArray of the specified size and fills it with
 * sorted double values starting from 10.0 and increasing by 4.5 for each
 * subsequent element. The resulting array will contain values in ascending
 * order, making it suitable for testing algorithms that require sorted input.
 *
 * @param size The number of sorted double values to generate (default is 25).
 * @return A DynamicArray containing the generated sorted double values.
 */
containers::DynamicArray<double> generateSortedDoubleArray(const size_t size) {
    containers::DynamicArray<double> arr;
    double current = 10.0;
    for(size_t i = 0; i < size; ++i) {
        arr.addLast(current);
        current += 4.5;
    }
    return arr;
}

} // namespace ui
