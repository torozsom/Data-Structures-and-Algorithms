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


ArrayAnimation createLinearSearchAnimation(const containers::DynamicArray<double>& values, double target);

ArrayAnimation createBinarySearchAnimation(const containers::DynamicArray<double>& values, double target);


ArrayAnimation createBubbleSortAnimation(containers::DynamicArray<double>& values);

ArrayAnimation createImprovedBubbleSortAnimation(containers::DynamicArray<double>& values);

ArrayAnimation createInsertSortLSAnimation(containers::DynamicArray<double>& values);

ArrayAnimation createInsertSortBSAnimation(containers::DynamicArray<double>& values);

ArrayAnimation createQuickSortAnimation(containers::DynamicArray<double>& values);

ArrayAnimation createMergeSortAnimation(containers::DynamicArray<double>& values);

ArrayAnimation createHeapSortAnimation(containers::DynamicArray<double>& values);


} // namespace ui


#endif // ARRAY_ANIMATIONS_H
