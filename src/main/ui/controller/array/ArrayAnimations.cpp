#include "ArrayAnimations.h"
#include "BinarySearchAnimator.hpp"
#include "DynamicArray.hpp"
#include "LinearSearchAnimator.hpp"


namespace ui {


/**
 * @brief Creates an animation for linear search on a dynamic array of integers.
 *
 * This function initializes a dynamic array with integer values from 1 to 10,
 * sets a target value to search for, and creates an ArrayWidget to visualize
 * the array. It then creates a LinearSearchAnimator to perform the search
 * animation. The function connects signals from the animator to update the
 * window title based on whether the target is found or not.
 *
 * @return An ArrayAnimation struct containing the ArrayWidget and
 * LinearSearchAnimator.
 */
ArrayAnimation createLinearSearchAnimation() {
    containers::DynamicArray values{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    constexpr int target = 10;

    auto* view = new ArrayWidget(values);
    view->resize(600, 120);

    // Parent animator to the view to ensure automatic deletion (using QPointer)
    auto* animator = new LinearSearchAnimator(values, target, view, view);

    QObject::connect(
        animator, &LinearSearchAnimator::elementFound, view,
        [view](const std::size_t index) {
            if (auto* win = view->window())
                win->setWindowTitle(
                    QString("Found %1 at index %2").arg(target).arg(index));
        });

    QObject::connect(animator, &LinearSearchAnimator::elementNotFound, view,
                     [view] {
                         if (auto* win = view->window())
                             win->setWindowTitle(
                                 QString("Element %1 not found").arg(target));
                     });

    return {view, animator};
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
 * @return An ArrayAnimation struct containing the ArrayWidget and
 * BinarySearchAnimator.
 */
ArrayAnimation createBinarySearchAnimation() {
    containers::DynamicArray values{1.6, 2.5, 3.4,  4.8,  5.9,  6.2, 7.7,
                                    8.1, 9.0, 10.9, 11.2, 12.3, 13.4};
    constexpr double target = 12.3;

    auto* view = new ArrayWidget(values);
    view->resize(600, 120);

    // Parent animator to the view to ensure automatic deletion (using QPointer)
    auto* animator = new BinarySearchAnimator(values, target, view, view);

    QObject::connect(
        animator, &BinarySearchAnimator::elementFound, view,
        [view](const std::size_t index) {
            if (auto* win = view->window())
                win->setWindowTitle(
                    QString("Found %1 at index %2").arg(target).arg(index));
        });

    QObject::connect(animator, &BinarySearchAnimator::elementNotFound, view,
                     [view] {
                         if (auto* win = view->window())
                             win->setWindowTitle(
                                 QString("Element %1 not found").arg(target));
                     });

    return {view, animator};
}


} // namespace ui
