#ifndef INSERTION_SORT_WITH_LINEAR_SEARCH_ANIMATOR_HPP
#define INSERTION_SORT_WITH_LINEAR_SEARCH_ANIMATOR_HPP


#include <QObject>
#include <QTimer>
#include <utility>

#include "ArrayWidget.hpp"
#include "DynamicArray.hpp"
#include "SortAnimator.hpp"


namespace ui {


/**
 * @brief Class to animate the insertion sort with linear search algorithm on a dynamic array.
 *
 * Collects sorting steps from the algorithm and plays them back on an
 * ArrayWidget to visualize the process.
 */
class InsertionSortWithLinearSearchAnimator final : public SortAnimator {
    Q_OBJECT

    /// Functor to perform insertion sort with linear search
    struct InsertionSortWithLinearSearchFn {
        template <typename Type, typename Callback>
        void operator()(containers::DynamicArray<Type>& array,
                        Callback&& callback) const {
            array_algorithms::InsertionSortWithLinearSearch(array,
                                                           std::forward<Callback>(callback));
        }
    };


  public:
    /// Constructor
    explicit InsertionSortWithLinearSearchAnimator(QObject* parent = nullptr)
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
    InsertionSortWithLinearSearchAnimator(containers::DynamicArray<Type>& array,
                                         ArrayWidget* widget, QObject* parent = nullptr)
        : SortAnimator(array, widget, InsertionSortWithLinearSearchFn{}, 800, parent) {}

    /// Destructor
    ~InsertionSortWithLinearSearchAnimator() override = default;
};


} // namespace ui


#endif // INSERTION_SORT_WITH_LINEAR_SEARCH_ANIMATOR_HPP