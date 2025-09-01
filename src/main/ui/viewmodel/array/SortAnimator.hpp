#ifndef SORT_ANIMATOR_HPP
#define SORT_ANIMATOR_HPP


#include <QObject>


namespace ui {


/**
 * @brief Base class for animating sorting algorithms on an ArrayWidget.
 *
 * Collects instrumentation steps from a sorting function and replays them
 * with timed updates on the associated widget.
 */
class SortAnimator : public QObject {

    Q_OBJECT

  protected:
    enum class StepType { Compare, Swap, MarkSorted };

    struct Step {
        StepType type;
        size_t first;
        size_t second;
    };

    QPointer<ArrayWidget> widget_{};
    QTimer timer_{};
    containers::DynamicArray<Step> steps_{};
    size_t current_ = 0;


  protected slots:
    /**
     * @brief Perform the next step in the animation sequence.
     *
     * Executes the recorded operation (comparison, swap or marking an element
     * as sorted) and advances the internal step counter. Stops the timer when
     * all steps have been processed.
     */
    void nextStep() {
        if (current_ >= steps_.size()) {
            timer_.stop();
            return;
        }

        switch (const auto [type, a, b] = steps_[current_++]; type) {
        case StepType::Compare:
            if (widget_)
                widget_->highlightIndices(a, b);
            break;
        case StepType::Swap:
            if (widget_)
                widget_->swapCells(a, b);
            break;
        case StepType::MarkSorted:
            if (widget_)
                widget_->markSorted(a);
            break;
        }
    }


  private:
    /**
     * @brief Collects steps from the provided sorting function.
     *
     * Runs the algorithm with an instrumentation callback that records each
     * comparison, swap or "sorted" event for later playback.
     *
     * @tparam Type Type of elements in the array.
     * @tparam SortFunc Callable sorting routine.
     * @param array Array to be sorted.
     * @param sortFunc Sorting function invoked to produce steps.
     * @param intervalMs Interval between animation frames in milliseconds.
     */
    template <typename Type, typename SortFunc>
    void collectSteps(containers::DynamicArray<Type>& array,
                      SortFunc&& sortFunc, const size_t intervalMs) {
        steps_.clear();
        current_ = 0;

        // Callback expected by the sorting function:
        // code: 0=Compare(a,b), 1=Swap(a,b), 2=MarkSorted(a,_)
        auto step_cb = [this](const size_t code, const size_t a,
                              const size_t b) {
            switch (code) {
            case 0:
                steps_.emplaceLast(Step{StepType::Compare, a, b});
                break;
            case 1:
                steps_.emplaceLast(Step{StepType::Swap, a, b});
                break;
            case 2:
                steps_.emplaceLast(Step{StepType::MarkSorted, a, b});
                break;
            default:
                break;
            }
        };

        // Run the sort with instrumentation to collect steps
        sortFunc(array, step_cb);

        // Configure timer interval for playback
        timer_.setInterval(static_cast<int>(intervalMs));
    }


  public:
    /// Default constructor
    explicit SortAnimator(QObject* parent = nullptr) : QObject(parent) {}

    /**
     * Constructor that starts the animation immediately
     *
     * @tparam Type The type of the elements in the array
     * @param array The array to be sorted
     * @param widget The widget to display the animation
     * @param sortFunc The sorting function to use
     * @param intervalMs The interval between steps in milliseconds
     * @param parent The parent QObject
     */
    template <typename Type, typename SortFunc>
    SortAnimator(containers::DynamicArray<Type>& array, ArrayWidget* widget,
                 SortFunc&& sortFunc, const size_t intervalMs,
                 QObject* parent = nullptr)
        : QObject(parent), widget_(widget) {
        widget_->hideArrow();
        collectSteps(array, std::forward<SortFunc>(sortFunc), intervalMs);
        connect(&timer_, &QTimer::timeout, this, &SortAnimator::nextStep);
        timer_.start();
    }

    /// Destructor
    ~SortAnimator() override = default;
};

} // namespace ui

#endif // SORT_ANIMATOR_HPP
