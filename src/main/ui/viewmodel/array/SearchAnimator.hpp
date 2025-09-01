#ifndef SEARCH_ANIMATOR_HPP
#define SEARCH_ANIMATOR_HPP

#include <QObject>
#include <QTimer>
#include <cstddef>
#include <utility>

#include "ArrayWidget.hpp"


namespace ui {


/**
 * @brief Class to animate search algorithms on a dynamic array.
 *
 * This class provides functionality to visualize the steps of a search
 * algorithm on a dynamic array using an ArrayWidget. It collects the steps of
 * the search process and animates them using a timer.
 */
class SearchAnimator : public QObject {

    Q_OBJECT

  protected:
    enum class StepType { Visit, Found, NotFound };

    struct Step {
        StepType type;
        std::size_t index;
    };

    ArrayWidget* widget_{};
    QTimer timer_{};
    containers::DynamicArray<Step> steps_{};
    std::size_t current_ = 0;


  signals:
    void elementFound(std::size_t index);
    void elementNotFound();


  private slots:

    /** Advances to the next step in the animation sequence.
     *  Highlights the current index being visited, or marks the element as
     * found/not found. Stops the timer and emits appropriate signals when the
     * search concludes.
     */
    void nextStep() {
        if (current_ >= steps_.size()) {
            timer_.stop();
            return;
        }

        switch (const auto& [type, index] = steps_[current_++]; type) {
            case StepType::Visit:
                if (widget_) {
                    widget_->highlightIndex(index);
                    widget_->setArrowPosition(index);
                }
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


  protected:
    /// Default constructor for derived classes that will set up the animation
    /// later.
    explicit SearchAnimator(QObject* parent = nullptr) : QObject(parent) {}


    /**
     *  Constructor that sets up the search animation.
     *  Collects the steps of the search algorithm and starts the timer to
     * animate them.
     *
     *  @tparam Type The type of elements in the array.
     *  @tparam SearchFunc The type of the search function (should be callable).
     *
     *  @param array The dynamic array to search within.
     *  @param target The target value to search for.
     *  @param widget The ArrayWidget to visualize the search process.
     *  @param searchFunc The search function that performs the search and calls
     * back on each step.
     *  @param intervalMs The interval in milliseconds between animation steps
     * (default is 500ms).
     *  @param parent The parent QObject (default is nullptr).
     */
    template <typename Type, typename SearchFunc>
    SearchAnimator(containers::DynamicArray<Type>& array, const Type& target,
                   ArrayWidget* widget, SearchFunc&& searchFunc,
                   int intervalMs = 500, QObject* parent = nullptr)
        : QObject(parent), widget_(widget) {
        collectSteps(array, target, std::forward<SearchFunc>(searchFunc),
                     intervalMs);
        connect(&timer_, &QTimer::timeout, this, &SearchAnimator::nextStep);
        timer_.start();
    }


    /**
     * Collects the steps of the search algorithm by invoking the provided
     * search function. The search function should call the provided callback
     * with each index it visits.
     *
     * @tparam Type The type of elements in the array.
     * @tparam SearchFunc The type of the search function (should be callable).
     *
     * @param array The dynamic array to search within.
     * @param target The target value to search for.
     * @param searchFunc The search function that performs the search and calls
     * back on each step.
     * @param intervalMs The interval in milliseconds between animation steps.
     */
    template <typename Type, typename SearchFunc>
    void collectSteps(containers::DynamicArray<Type>& array, const Type& target,
                      SearchFunc&& searchFunc, const int intervalMs) {
        steps_.clear();
        current_ = 0;

        auto step_cb = [this](const std::size_t idx) {
            steps_.emplaceLast(Step{StepType::Visit, idx});
        };

        const std::size_t result = searchFunc(array, target, step_cb);

        if (result < array.size())
            steps_.emplaceLast(Step{StepType::Found, result});
        else
            steps_.emplaceLast(Step{StepType::NotFound, result});

        timer_.setInterval(intervalMs);
    }


  public:
    /// Destructor
    ~SearchAnimator() override = default;
};


} // namespace ui


#endif // SEARCH_ANIMATOR_HPP
