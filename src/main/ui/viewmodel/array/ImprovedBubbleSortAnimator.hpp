#ifndef IMPROVED_BUBBLE_SORT_ANIMATOR_HPP
#define IMPROVED_BUBBLE_SORT_ANIMATOR_HPP


#include "ArrayWidget.hpp"
#include "DynamicArray.hpp"
#include "DynamicArrayAlgorithms.hpp"
#include "SortAnimator.hpp"


namespace ui {


class ImprovedBubbleSortAnimator final : public SortAnimator {
    Q_OBJECT

    struct ImprovedBubbleSortFn {
        template <typename Type, typename Callback>
        void operator()(containers::DynamicArray<Type>& array,
                        Callback&& callback) const {
            array_algorithms::ImprovedBubbleSort(
                array, std::forward<Callback>(callback));
        }
    };


  public:
    /// Default constructor
    explicit ImprovedBubbleSortAnimator(QObject* parent = nullptr)
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
    ImprovedBubbleSortAnimator(containers::DynamicArray<Type>& array,
                               ArrayWidget* widget, QObject* parent = nullptr)
        : SortAnimator(array, widget, ImprovedBubbleSortFn{}, 600, parent) {}

    /// Destructor
    ~ImprovedBubbleSortAnimator() override = default;
};

} // namespace ui

#endif // IMPROVED_BUBBLE_SORT_ANIMATOR_HPP
