#ifndef BINARY_SEARCH_ANIMATOR_HPP
#define BINARY_SEARCH_ANIMATOR_HPP

#include "DynamicArrayAlgorithms.hpp"
#include "SearchAnimator.hpp"


namespace ui {


class BinarySearchAnimator final : public SearchAnimator {

    Q_OBJECT

    /// Functor to perform binary search
    struct BinarySearchFn {
        template <typename Type, typename Callback>
        std::size_t operator()(const data_structs::DynamicArray<Type>& array,
                               const Type& target, Callback&& callback) const {
            return array_algorithms::BinarySearch(
                array, target, static_cast<Callback&&>(callback));
        }
    };

  public:
    /// Default constructor
    explicit BinarySearchAnimator(QObject* parent = nullptr)
        : SearchAnimator(parent) {}


    /**
     * Constructs a BinarySearchAnimator that animates the binary search process
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
    BinarySearchAnimator(data_structs::DynamicArray<Type>& array,
                         const Type& target, ArrayWidget* widget,
                         QObject* parent = nullptr)
        : SearchAnimator(array, target, widget, BinarySearchFn{}, 1000, parent) {
    }


    /// Destructor
    ~BinarySearchAnimator() override = default;
};

} // namespace ui

#endif // BINARY_SEARCH_ANIMATOR_HPP
