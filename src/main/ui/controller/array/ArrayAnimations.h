#ifndef ARRAY_ANIMATIONS_H
#define ARRAY_ANIMATIONS_H


#include <QObject>
#include <QPointer>

#include "ArrayWidget.hpp"
#include "BinarySearchAnimator.hpp"
#include "BubbleSortAnimator.hpp"
#include "DynamicArray.hpp"
#include "LinearSearchAnimator.hpp"
#include "SearchAnimator.hpp"


namespace ui {

using std::size_t;

/// Holds the view and animator used to display an array algorithm.
struct ArrayAnimation {
    /// Widget that renders the array and its animation.
    ArrayWidget* view;
    /// Animator object controlling the visualized algorithm steps.
    QObject* animator;
};


ArrayAnimation createLinearSearchAnimation();

ArrayAnimation createBinarySearchAnimation();

ArrayAnimation createBubbleSortAnimation();


} // namespace ui


#endif // ARRAY_ANIMATIONS_H
