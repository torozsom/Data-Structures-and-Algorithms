#ifndef ARRAY_ANIMATIONS_H
#define ARRAY_ANIMATIONS_H


#include <QObject>
#include <QWidget>
#include <QPointer>
#include <QBoxLayout>
#include <QLabel>

#include "SearchAnimator.hpp"
#include "SortAnimator.hpp"
#include "ArrayAlgorithms.hpp"
#include "ArrayAlgorithmFunctors.hpp"

namespace ui {


/// Holds the view and animator used to display an array algorithm.
struct ArrayAnimation {
    // Widget that renders the array and its animation.
    QWidget* view;
    // Animator controlling the visualized steps.
    QObject* animator;
};


ArrayAnimation createLinearSearchAnimation();

ArrayAnimation createBinarySearchAnimation();


ArrayAnimation createBubbleSortAnimation();

ArrayAnimation createImprovedBubbleSortAnimation();

ArrayAnimation createInsertSortLSAnimation();

ArrayAnimation createInsertSortBSAnimation();

ArrayAnimation createQuickSortAnimation();

ArrayAnimation createMergeSortAnimation();

ArrayAnimation createHeapSortAnimation();


} // namespace ui


#endif // ARRAY_ANIMATIONS_H
