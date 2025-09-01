#ifndef BUBBLE_SORT_ANIMATOR_HPP
#define BUBBLE_SORT_ANIMATOR_HPP


#include <QObject>
#include <QTimer>
#include <utility>

#include "ArrayWidget.hpp"
#include "DynamicArray.hpp"
#include "SortAnimator.hpp"


namespace ui {


/**
 * @brief Class to animate the bubble sort algorithm on a dynamic array.
 *
 * Collects sorting steps from the algorithm and plays them back on an
 * ArrayWidget to visualize the process.
 */
class BubbleSortAnimator final : public SortAnimator {
    Q_OBJECT

    /// Functor to perform bubble sort
    struct BubbleSortFn {
        template <typename Type, typename Callback>
        void operator()(containers::DynamicArray<Type>& array,
                        Callback&& callback) const {
            array_algorithms::BubbleSort(array,
                                         std::forward<Callback>(callback));
        }
    };


  public:
    /// Constructor
    explicit BubbleSortAnimator(QObject* parent = nullptr)
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
    BubbleSortAnimator(containers::DynamicArray<Type>& array,
                       ArrayWidget* widget, QObject* parent = nullptr)
        : SortAnimator(array, widget, BubbleSortFn{}, 600, parent) {}

    /// Destructor
    ~BubbleSortAnimator() override = default;
};


} // namespace ui


#endif // BUBBLE_SORT_ANIMATOR_HPP
