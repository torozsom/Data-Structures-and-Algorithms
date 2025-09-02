#include "ArrayAnimations.h"
#include "ImprovedBubbleSortAnimator.hpp"
#include "InsertSortBSAnimator.hpp"
#include "MergeSortAnimator.hpp"
#include "QuickSortAnimator.hpp"

#include <QBoxLayout>
#include <QLabel>


namespace ui {


/**
 * @brief Creates a header widget displaying the target element for search
 * animations.
 *
 * This function constructs a QWidget containing a label and a graphical
 * representation of the target element. The target is displayed inside a
 * bordered rectangle, styled for visibility.
 *
 * @tparam Type The type of the target element (e.g., int, double).
 * @param target The target element to be displayed.
 * @param parent The parent widget (default is nullptr).
 * @return A pointer to the created QWidget containing the target header.
 */
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

    // Build a small graphics view that renders a single cell
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
 * @brief Creates an animation for search algorithms: Animator(array, target,
 * widget, parent)
 *
 * This function sets up the necessary UI components and animator for
 * visualizing search algorithms on a dynamic array. It creates an ArrayWidget
 * to display the array and a header to show the target element being searched
 * for. The function also connects signals from the animator to update the
 * window title based on whether the target is found or not.
 *
 * @tparam Animator The type of the search animator.
 * @tparam Values The type of the dynamic array values.
 * @tparam Target The type of the target element to search for.
 *
 * @param values The dynamic array containing the elements to be searched.
 * @param target The target element to search for in the array.
 * @return An ArrayAnimation struct containing the container view and animator.
 */
template <typename Animator, typename Values, typename Target>
static ArrayAnimation makeSearchAnimation(const Values& values,
                                          const Target& target) {
    QPointer arrayView = new ArrayWidget(values);

    const QPointer container = new QWidget();
    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    vbox->addWidget(makeTargetHeader(target, container), 0);
    arrayView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(arrayView, 1);

    QObject* animator = new Animator(values, target, arrayView, arrayView);

    if (auto* sa = qobject_cast<SearchAnimator*>(animator)) {
        QObject::connect(
            sa, &SearchAnimator::elementFound, arrayView,
            [arrayView, target](const size_t index) {
                if (auto* win = arrayView->window()) {
                    win->setWindowTitle(
                        QString("Found %1 at index %2").arg(target).arg(index));
                }
            });
        QObject::connect(
            sa, &SearchAnimator::elementNotFound, arrayView,
            [arrayView, target] {
                if (auto* win = arrayView->window()) {
                    win->setWindowTitle(
                        QString("Element %1 not found").arg(target));
                }
            });
    }

    return {container, animator};
}


/**
 * @brief Creates an animation for sort algorithms: Animator(array, widget,
 * parent)
 *
 * This function sets up the necessary UI components and animator for
 * visualizing sorting algorithms on a dynamic array. It creates an ArrayWidget
 * to display the array and instantiates the specified sorting animator.
 *
 * @tparam Animator The type of the sort animator.
 * @tparam Values The type of the dynamic array values.
 *
 * @param values The dynamic array containing the elements to be sorted.
 * @return An ArrayAnimation struct containing the container view and animator.
 */
template <typename Animator, typename Values>
static ArrayAnimation makeSortAnimation(Values& values) {
    const QPointer arrayView = new ArrayWidget(values);
    const QPointer container = new QWidget();
    const QPointer vbox = new QVBoxLayout(container);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    arrayView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(arrayView, 1);

    QObject* animator = new Animator(values, arrayView, arrayView);
    return {container, animator};
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
    const containers::DynamicArray values{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    constexpr int target = 8;
    return makeSearchAnimation<LinearSearchAnimator>(values, target);
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
    const containers::DynamicArray values{1.6, 2.5, 3.4, 4.8, 5.9,
                                          6.2, 7.7, 8.1, 9.1, 10.9};
    constexpr double target = 9.1;
    return makeSearchAnimation<BinarySearchAnimator>(values, target);
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
    return makeSortAnimation<BubbleSortAnimator>(values);
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
    containers::DynamicArray values{6.4, 4.3, 9.7, 3.2, 3.2,
                                    6.5, 2.4, 1.0, 7.0, 6.6};
    return makeSortAnimation<ImprovedBubbleSortAnimator>(values);
}


/**
 * @brief Creates an animation for insertion sort with linear search on a
 * dynamic array of integers.
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
    return makeSortAnimation<InsertSortLSAnimator>(values);
}


/**
 * @brief Creates an animation for insertion sort with binary search on a
 * dynamic array of integers.
 *
 * Initializes an unsorted integer array, sets up an ArrayWidget to visualize
 * it and instantiates an InsertSortBSAnimator. The animator drives the
 * step-by-step sorting animation on the provided widget.
 *
 * @return An ArrayAnimation struct containing the container view and
 * InsertSortBSAnimator.
 */
ArrayAnimation createInsertSortBSAnimation() {
    containers::DynamicArray values{8, 3, 5, 4, 7, 6, 2, 1, 9, 0};
    return makeSortAnimation<InsertSortBSAnimator>(values);
}


ArrayAnimation createQuickSortAnimation() {
    containers::DynamicArray values{6, 4, 9, 3, 3, 6, 2, 1, 7, 6};
    return makeSortAnimation<QuickSortAnimator>(values);
}


ArrayAnimation createMergeSortAnimation() {
    containers::DynamicArray values{6, 4, 9, 3, 3, 6, 2, 1, 7, 6};
    return makeSortAnimation<MergeSortAnimator>(values);
}


} // namespace ui
