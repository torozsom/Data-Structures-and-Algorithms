#ifndef MERGE_SORT_ANIMATOR_HPP
#define MERGE_SORT_ANIMATOR_HPP

#include <QObject>
#include <utility>

#include "ArrayWidget.hpp"
#include "DynamicArray.hpp"
#include "DynamicArrayAlgorithms.hpp"
#include "SortAnimator.hpp"


namespace ui {


/**
 * @brief Class to animate the merge sort algorithm on a
 * dynamic array.
 *
 * Collects sorting steps from the algorithm and plays them back on an
 * ArrayWidget to visualize the process.
 */
class MergeSortAnimator final : public SortAnimator {
    Q_OBJECT

    /// Functor to perform merge sort
    struct MergeSortFn {
        template <typename Type, typename Callback>
        void operator()(containers::DynamicArray<Type>& array,
                        Callback&& callback) const {
            array_algorithms::MergeSort(
                array, std::forward<Callback>(callback));
        }
    };


public:
    /// Constructor
    explicit MergeSortAnimator(QObject* parent = nullptr)
        : SortAnimator(parent) {}


    /**
     * Constructor that starts the animation immediately
     *
     * @tparam Type The type of the elements in the array
     * @param array The array to be sorted
     * @param widget The widget to display the animation
     * @param parent The parent QObject
     */
    template <typename Type>
    MergeSortAnimator(containers::DynamicArray<Type>& array,
                         ArrayWidget* widget, QObject* parent = nullptr)
        : SortAnimator(array, widget, MergeSortFn{}, 550,
                       parent) {}

    /// Destructor
    ~MergeSortAnimator() override = default;
};

} // namespace ui

#endif //MERGE_SORT_ANIMATOR_HPP
