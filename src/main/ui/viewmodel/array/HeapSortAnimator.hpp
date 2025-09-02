#ifndef HEAP_SORT_ANIMATOR_HPP
#define HEAP_SORT_ANIMATOR_HPP


#include <QObject>
#include <utility>

#include "ArrayWidget.hpp"
#include "DynamicArray.hpp"
#include "DynamicArrayAlgorithms.hpp"
#include "SortAnimator.hpp"


namespace ui {


/**
 * @brief Class to animate the heap sort algorithm on a
 * dynamic array.
 *
 * Collects sorting steps from the algorithm and plays them back on an
 * ArrayWidget to visualize the process.
 */
class HeapSortAnimator final : public SortAnimator {
    Q_OBJECT

    /// Functor to perform heap sort
    struct HeapSortFn {
        template <typename Type, typename Callback>
        void operator()(containers::DynamicArray<Type>& array,
                        Callback&& callback) const {
            array_algorithms::HeapSort(
                array, std::forward<Callback>(callback));
        }
    };


public:
    /// Constructor
    explicit HeapSortAnimator(QObject* parent = nullptr)
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
    HeapSortAnimator(containers::DynamicArray<Type>& array,
                         ArrayWidget* widget, QObject* parent = nullptr)
        : SortAnimator(array, widget, HeapSortFn{}, 550,
                       parent) {}

    /// Destructor
    ~HeapSortAnimator() override = default;
};

} // namespace ui

#endif //HEAP_SORT_ANIMATOR_HPP
