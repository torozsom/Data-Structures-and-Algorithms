//
// Created by toron on 2025. 08. 26..
//

#ifndef ALGORITHMS_LINEARSEARCHANIMATOR_HPP
#define ALGORITHMS_LINEARSEARCHANIMATOR_HPP

#include "ArrayWidget.h"
#include "DynamicArrayAlgorithms.hpp"

#include <QObject>
#include <QTimer>
#include <cstddef>


namespace ui {

/**
 * @brief Animates a linear search over an ArrayWidget.
 *
 *  The animator collects the steps of a callback driven linear search and
 *  replays them over time using a QTimer.  Each step triggers a visual
 *  update on the supplied ArrayWidget.  When the searched element is either
 *  found or determined to be absent, a corresponding signal is emitted.
 */
class LinearSearchAnimator final : public QObject {
    Q_OBJECT

    enum class StepType { Visit, Found, NotFound };

    struct Step {
        StepType type;
        std::size_t index;
    };

    ArrayWidget* widget_{};
    QTimer timer_{};
    data_structs::DynamicArray<Step> steps_{};
    std::size_t current_ = 0;


  signals:
    /// Emitted when the target element was found in the array.
    void elementFound(std::size_t index);
    /// Emitted when the target element is not present in the array.
    void elementNotFound();


  private slots:
    /// Advances the animation to the next step.
    void nextStep() {
        if (current_ >= steps_.size()) {
            timer_.stop();
            return;
        }

        switch (const auto& [type, index] = steps_[current_++]; type) {
        case StepType::Visit:
            if (widget_)
                widget_->highlightIndex(index);
            break;

        case StepType::Found:
            if (widget_)
                widget_->markFound(index);
            timer_.stop();
            emit elementFound(index);
            break;

        case StepType::NotFound:
            if (widget_)
                widget_->markNotFound();
            timer_.stop();
            emit elementNotFound();
            break;

        default:
            break;
        }
    }


  private:
    /**
     * @brief Collects the steps of a linear search over the given array for the
     * target element. Each visited index is recorded as a Visit step, and
     * either a Found or NotFound step is recorded at the end.
     *
     * @tparam Type The element type of the array.
     * @param array The array to search in.
     * @param target The element to search for.
     */
    template <typename Type>
    void collectSteps(const data_structs::DynamicArray<Type>& array,
                      const Type& target) {

        auto step_cb = [this](const std::size_t idx) {
            steps_.emplaceLast(Step{StepType::Visit, idx});
        };

        const std::size_t result = linearSearch(array, target, step_cb);

        if (result < array.size())
            steps_.emplaceLast(Step{StepType::Found, result});
        else
            steps_.emplaceLast(Step{StepType::NotFound, result});

        timer_.setInterval(500);
    }


    /* * @brief Performs a linear search on the given array for the target
     * element, invoking the provided callback at each visited index.
     *
     * @tparam Type The element type of the array.
     * @tparam Callback The type of the callback function.
     *
     * @param array The array to search in.
     * @param target The element to search for.
     * @param callback A function to call with the index of each visited
     * element.
     * @return The index of the found element, or array.size() if not found.
     */
    template <typename Type, typename Callback>
    static std::size_t
    linearSearch(const data_structs::DynamicArray<Type>& array,
                 const Type& target, Callback&& callback) {
        return array_algorithms::LinearSearch(array, target, callback);
    }


  public:
    /// Default constructor.
    explicit LinearSearchAnimator(QObject* parent = nullptr)
        : QObject(parent) {}


    /**
     * @brief Constructs a LinearSearchAnimator that will animate a linear
     * search for the target element in the given array, updating the provided
     * ArrayWidget.
     *
     * @tparam Type The element type of the array.
     *
     * @param array The array to search in.
     * @param target The element to search for.
     * @param widget The ArrayWidget to update during the animation.
     * @param parent The parent QObject, defaulting to nullptr.
     */
    template <typename Type>
    LinearSearchAnimator(const data_structs::DynamicArray<Type>& array,
                         const Type& target, ArrayWidget* widget,
                         QObject* parent = nullptr)
        : QObject(parent), widget_(widget) {
        collectSteps(array, target);
        connect(&timer_, &QTimer::timeout, this,
                &LinearSearchAnimator::nextStep);
        timer_.start();
    }

    ~LinearSearchAnimator() override = default;
};

} // namespace ui

#endif // ALGORITHMS_LINEARSEARCHANIMATOR_HPP
