#ifndef QUICK_SORT_ANIMATOR_HPP
#define QUICK_SORT_ANIMATOR_HPP

#include <QObject>
#include <utility>

#include "ArrayWidget.hpp"
#include "DynamicArray.hpp"
#include "DynamicArrayAlgorithms.hpp"
#include "SortAnimator.hpp"


namespace ui {


/**
 * @brief Class to animate the insertion sort with linear search algorithm on a
 * dynamic array.
 *
 * Collects sorting steps from the algorithm and plays them back on an
 * ArrayWidget to visualize the process.
 */
class QuickSortAnimator final : public SortAnimator {
    Q_OBJECT

    /// Functor to perform insertion sort with binary search
    struct QuickSortFn {
        template <typename Type, typename Callback>
        void operator()(containers::DynamicArray<Type>& array,
                        Callback&& callback) const {
            array_algorithms::QuickSort(
                array, std::forward<Callback>(callback));
        }
    };


public:
    /// Constructor
    explicit QuickSortAnimator(QObject* parent = nullptr)
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
    QuickSortAnimator(containers::DynamicArray<Type>& array,
                         ArrayWidget* widget, QObject* parent = nullptr)
        : SortAnimator(array, widget, QuickSortFn{}, 600,
                       parent) {}

    /// Destructor
    ~QuickSortAnimator() override = default;
};

} // namespace ui

#endif //QUICK_SORT_ANIMATOR_HPP
