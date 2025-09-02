#ifndef LINEAR_SEARCH_ANIMATOR_HPP
#define LINEAR_SEARCH_ANIMATOR_HPP

#include "DynamicArrayAlgorithms.hpp"
#include "SearchAnimator.hpp"


namespace ui {


/**
 * @brief Class to animate the linear search algorithm on a dynamic array.
 *
 * This class extends SearchAnimator to specifically visualize the linear
 * search process. It uses a functor to perform the linear search and collects
 * the steps for animation.
 */
class LinearSearchAnimator final : public SearchAnimator {

    Q_OBJECT

    /// Functor to perform linear search
    struct LinearSearchFn {
        template <typename Type, typename Callback>
        size_t operator()(const containers::DynamicArray<Type>& array,
                          const Type& target, Callback&& callback) const {
            return array_algorithms::LinearSearch(
                array, target, static_cast<Callback&&>(callback));
        }
    };


  public:
    /// Default constructor
    explicit LinearSearchAnimator(QObject* parent = nullptr)
        : SearchAnimator(parent) {}


    /**
     * Constructs a LinearSearchAnimator that animates the linear search process
     * on the provided dynamic array for the specified target value.
     *
     * @tparam Type The type of elements in the array.
     *
     * @param array The dynamic array to search within.
     * @param target The target value to search for.
     * @param widget The ArrayWidget to visualize the search process.
     * @param parent The parent QObject (default is nullptr).
     */
    template <typename Type>
    LinearSearchAnimator(const containers::DynamicArray<Type>& array,
                         const Type& target, ArrayWidget* widget,
                         QObject* parent = nullptr)
        : SearchAnimator(array, target, widget, LinearSearchFn{}, 500, parent) {
    }


    ~LinearSearchAnimator() override = default;
};

} // namespace ui

#endif // LINEAR_SEARCH_ANIMATOR_HPP
