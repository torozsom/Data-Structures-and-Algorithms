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

/// Struct to hold the components needed for an array animation
struct ArrayAnimation {
    ArrayWidget* view;
    QObject* animator;
};


ArrayAnimation createLinearSearchAnimation();

ArrayAnimation createBinarySearchAnimation();

ArrayAnimation createBubbleSortAnimation();


} // namespace ui


#endif // ARRAY_ANIMATIONS_H
