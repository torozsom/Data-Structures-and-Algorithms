#include "ArrayAnimations.h"

#include "ImprovedBubbleSortAnimator.hpp"

#include <QBoxLayout>
#include <QLabel>


namespace ui {


template <typename Type>
QWidget* makeTargetHeader(const Type& target, QWidget* parent = nullptr) {
    constexpr int w = 40;
    constexpr int h = 40;

    QPointer header = new QWidget(parent);
    const QPointer hbox = new QHBoxLayout(header);
    hbox->setContentsMargins(8, 8, 8, 8);
    hbox->setSpacing(8);

    const QPointer label = new QLabel("Target element:", header);
    label->setStyleSheet("color: yellow; font-weight: bold;");

    // Build a small graphics view that renders a single cell like the array
    const QPointer gv = new QGraphicsView(header);
    gv->setFrameShape(QFrame::NoFrame);
    gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gv->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    const QPointer scene = new QGraphicsScene(gv);
    gv->setScene(scene);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);

    scene->addRect(0, 0, w, h, pen, QBrush{Qt::white});

    QPointer text = scene->addText(QString::number(target));
    text->setDefaultTextColor(Qt::black);
    QFont font = text->font();
    font.setPointSizeF(static_cast<qreal>(h) * 0.35);
    text->setFont(font);

    const QRectF tr = text->boundingRect();
    text->setPos((w - tr.width()) / 2.0, (h - tr.height()) / 2.0);

    scene->setSceneRect(0, 0, w, h);
    gv->setFixedSize(w + 2, h + 2);

    hbox->addWidget(label, 0, Qt::AlignLeft | Qt::AlignVCenter);
    hbox->addWidget(gv, 0, Qt::AlignLeft | Qt::AlignVCenter);
    hbox->addStretch(1);

    return header;
}


/**
 * @brief Creates an animation for linear search on a dynamic array of integers.
 *
 * This function initializes a dynamic array with integer values from 1 to 10,
 * sets a target value to search for, and creates an ArrayWidget to visualize
 * the array. It then creates a LinearSearchAnimator to perform the search
 * animation. The function connects signals from the animator to update the
 * window title based on whether the target is found or not.
 *
 * @return An ArrayAnimation struct containing the container view and
 * LinearSearchAnimator.
 */
ArrayAnimation createLinearSearchAnimation() {
    containers::DynamicArray values{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    constexpr int target = 8;

    // Build the core view
    QPointer arrayView = new ArrayWidget(values);

    // Build a container with a header row showing the target
    const QPointer container = new QWidget();
    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    vbox->addWidget(makeTargetHeader(target, container), 0);
    arrayView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(arrayView, 1);

    const QPointer animator =
        new LinearSearchAnimator(values, target, arrayView, arrayView);

    QObject::connect(
        animator, &LinearSearchAnimator::elementFound, arrayView,
        [arrayView](const size_t index) {
            if (auto* win = arrayView->window())
                win->setWindowTitle(
                    QString("Found %1 at index %2").arg(target).arg(index));
        });

    QObject::connect(animator, &LinearSearchAnimator::elementNotFound,
                     arrayView, [arrayView] {
                         if (auto* win = arrayView->window())
                             win->setWindowTitle(
                                 QString("Element %1 not found").arg(target));
                     });

    return {container, animator};
}


/**
 * @brief Creates an animation for binary search on a dynamic array of doubles.
 *
 * This function initializes a dynamic array with sorted double values,
 * sets a target value to search for, and creates an ArrayWidget to visualize
 * the array. It then creates a BinarySearchAnimator to perform the search
 * animation. The function connects signals from the animator to update the
 * window title based on whether the target is found or not.
 *
 * @return An ArrayAnimation struct containing the container view and
 * BinarySearchAnimator.
 */
ArrayAnimation createBinarySearchAnimation() {
    containers::DynamicArray values{1.6, 2.5, 3.4, 4.8, 5.9,
                                    6.2, 7.7, 8.1, 9.1, 10.9};
    constexpr double target = 9.1;

    QPointer arrayView = new ArrayWidget(values);

    const QPointer container = new QWidget();
    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    vbox->addWidget(makeTargetHeader(target, container), 0);
    arrayView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(arrayView, 1);

    const QPointer animator =
        new BinarySearchAnimator(values, target, arrayView, arrayView);

    QObject::connect(
        animator, &BinarySearchAnimator::elementFound, arrayView,
        [arrayView](const size_t index) {
            if (auto* win = arrayView->window())
                win->setWindowTitle(
                    QString("Found %1 at index %2").arg(target).arg(index));
        });

    QObject::connect(animator, &BinarySearchAnimator::elementNotFound,
                     arrayView, [arrayView] {
                         if (auto* win = arrayView->window())
                             win->setWindowTitle(
                                 QString("Element %1 not found").arg(target));
                     });

    return {container, animator};
}


/**
 * @brief Creates an animation for bubble sort on a dynamic array of integers.
 *
 * Initializes an unsorted integer array, sets up an ArrayWidget to visualize
 * it and instantiates a BubbleSortAnimator. The animator drives the
 * step-by-step sorting animation on the provided widget.
 *
 * @return An ArrayAnimation struct containing the container view and
 * BubbleSortAnimator.
 */
ArrayAnimation createBubbleSortAnimation() {
    containers::DynamicArray values{6, 4, 9, 3, 3, 6, 2, 1, 7, 6};

    const QPointer arrayView = new ArrayWidget(values);

    const QPointer container = new QWidget();
    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    // No target for sorting; keep header area empty
    arrayView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(arrayView, 1);

    const QPointer animator =
        new BubbleSortAnimator(values, arrayView, arrayView);

    return {container, animator};
}


/**
 * @brief Creates an animation for bubble sort on a dynamic array of integers.
 *
 * Initializes an unsorted integer array, sets up an ArrayWidget to visualize
 * it and instantiates a ImprovedBubbleSortAnimator. The animator drives the
 * step-by-step sorting animation on the provided widget.
 *
 * @return An ArrayAnimation struct containing the container view and
 * ImprovedBubbleSortAnimator.
 */
ArrayAnimation createImprovedBubbleSortAnimation() {
    containers::DynamicArray values{6.6, 4.3, 9.7, 3.2, 3.2,
                                    6.6, 2.4, 1.0, 7.0, 6.6};

    const QPointer arrayView = new ArrayWidget(values);

    const QPointer container = new QWidget();
    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    // No target for sorting; keep header area empty
    arrayView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(arrayView, 1);

    const QPointer animator =
        new ImprovedBubbleSortAnimator(values, arrayView, arrayView);

    return {container, animator};
}


/**
 * @brief Creates an animation for insertion sort with linear search on a dynamic array of integers.
 *
 * Initializes an unsorted integer array, sets up an ArrayWidget to visualize
 * it and instantiates an InsertSortLSAnimator. The animator drives the
 * step-by-step sorting animation on the provided widget.
 *
 * @return An ArrayAnimation struct containing the container view and
 * InsertSortLSAnimator.
 */
ArrayAnimation createInsertSortLSAnimation() {
    containers::DynamicArray values{8, 3, 5, 4, 7, 6, 2, 1, 9, 0};

    const QPointer arrayView = new ArrayWidget(values);

    const QPointer container = new QWidget();
    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    // No target for sorting; keep header area empty
    arrayView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(arrayView, 1);

    const QPointer animator =
        new InsertSortLSAnimator(values, arrayView, arrayView);

    return {container, animator};
}


} // namespace ui
