#ifndef ARRAY_ANIMATIONS_H
#define ARRAY_ANIMATIONS_H


#include "ArrayWidget.hpp"
#include "SearchAnimator.hpp"


namespace ui {


/// Struct to hold the components needed for an array animation
struct ArrayAnimation {
    ArrayWidget* view;
    SearchAnimator* animator;
};

ArrayAnimation createLinearSearchAnimation();

ArrayAnimation createBinarySearchAnimation();


} // namespace ui


#endif // ARRAY_ANIMATIONS_H
