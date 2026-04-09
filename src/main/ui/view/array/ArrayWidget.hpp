#ifndef ARRAY_WIDGET_HPP
#define ARRAY_WIDGET_HPP


#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QPen>
#include <QPointF>
#include <QPolygonF>
#include <QRectF>
#include <QString>
#include <QVariantAnimation>
#include <QWidget>
#include <utility>


#include "DynamicArray.hpp"


namespace ui {


/**
 * @class ArrayWidget
 *
 * @brief A widget that visualizes an array of elements as a series of cells
 * with an arrow indicating the current index being examined.
 *
 * The ArrayWidget displays each element in a rectangular cell arranged
 * horizontally. An arrow above the cells points to the current index being
 * examined. Cells can be highlighted to indicate the current focus of an
 * algorithm, and can be marked to show whether the target element was found or
 * not found.
 */
class ArrayWidget final : public QGraphicsView {

    QGraphicsScene* scene_;
    QGraphicsPolygonItem* arrow_;
    containers::DynamicArray<QGraphicsRectItem*> cells_;
    int animationDurationMs_ = 300;

    static constexpr qreal BAR_WIDTH = 25.0;
    static constexpr qreal BAR_SPACING = 5.0;
    static constexpr qreal MAX_BAR_HEIGHT = 200.0;
    static constexpr qreal ARROW_OFFSET_Y = 15.0;


  public:
    /**
     * @brief Constructs an ArrayWidget that visualizes the given array of
     * values.
     *
     * Each value is represented as a cell in a horizontal row, with an arrow
     * above indicating the current index being examined.
     *
     * @param values The array of integer values to visualize.
     * @param parent The parent QWidget, defaulting to nullptr.
     */
    template <typename Type>
    explicit ArrayWidget(const containers::DynamicArray<Type>& values,
                         QWidget* parent = nullptr)
        : QGraphicsView(parent), scene_(new QGraphicsScene(this)),
          arrow_(nullptr) {
        setScene(scene_);

        if (values.size() == 0) return;

        QPen pen(Qt::black);
        pen.setWidth(1);

        Type maxVal = values[0];
        for (const auto& v : values)
            if (v > maxVal)
                maxVal = v;

        if (maxVal == 0) maxVal = 1;

        for (size_t i = 0; i < values.size(); ++i) {
            const qreal height = (static_cast<qreal>(values[i]) / maxVal) * MAX_BAR_HEIGHT;
            const qreal xPos = i * (BAR_WIDTH + BAR_SPACING);
            const qreal yPos = MAX_BAR_HEIGHT - height;

            auto rect = scene_->addRect(0, 0, BAR_WIDTH, height, pen, QBrush{Qt::white});
            rect->setPos(xPos, yPos);
            cells_.addLast(rect);
        }

        const QPolygonF polygon{
            QPointF{0.0, 0.0},
            QPointF{-8.0, 15.0},
            QPointF{8.0, 15.0}
        };

        arrow_ = scene_->addPolygon(polygon, pen, QBrush{Qt::cyan});
        setArrowPosition(0);

        const qreal totalWidth = values.size() * (BAR_WIDTH + BAR_SPACING);
        scene_->setSceneRect(-10, -10, totalWidth + 20, MAX_BAR_HEIGHT + ARROW_OFFSET_Y + 30);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }


    /**
     * @brief Highlights the cell at the specified index by changing its color
     * to yellow, and resets all other cells to white. Also moves the arrow
     * above the highlighted cell.
     *
     * @param index The index of the cell to highlight.
     * @param color The color to highlight the cell with.
     */
    void highlightIndex(const size_t index,
                        const Qt::GlobalColor color = Qt::yellow) const {
        for (auto* cell : cells_)
            if (cell->brush().color() != Qt::green)
                cell->setBrush(QBrush{Qt::white});

        if (index < cells_.size() &&
            cells_[index]->brush().color() != Qt::green)
            cells_[index]->setBrush(QBrush{color});
    }


    /**
     * @brief Highlights two cells at the specified indices.
     *
     * Resets all other cells to white before applying the highlight color to
     * the provided indices.
     */
    void highlightIndices(const size_t first, const size_t second,
                          const Qt::GlobalColor color = Qt::yellow) const {
        for (auto* cell : cells_)
            if (cell->brush().color() != Qt::green)
                cell->setBrush(QBrush{Qt::white});

        if (first < cells_.size() &&
            cells_[first]->brush().color() != Qt::green)
            cells_[first]->setBrush(QBrush{color});
        if (second < cells_.size() &&
            cells_[second]->brush().color() != Qt::green)
            cells_[second]->setBrush(QBrush{color});
    }


    /**
     * @brief Moves the arrow above the cell at the specified index.
     *
     * If the index is out of bounds, the arrow is not moved.
     *
     * @param index The index of the cell above which to position the arrow.
     */
    void setArrowPosition(const size_t index) const {
        if (!arrow_ || index >= cells_.size())
            return;

        const qreal x = static_cast<qreal>(index) * (BAR_WIDTH + BAR_SPACING) + BAR_WIDTH / 2.0;
        constexpr qreal y = MAX_BAR_HEIGHT + ARROW_OFFSET_Y;
        arrow_->setPos(x, y);
    }


    /// Hides the arrow from view.
    void hideArrow() const { arrow_->hide(); }


    /**
     * @brief Marks the cell at the specified index as found by changing its
     * color to green. Does nothing if the index is out of bounds.
     *
     * @param index The index of the cell to mark as found.
     */
    void markFound(const size_t index) const {
        if (index < cells_.size())
            cells_[index]->setBrush(QBrush{Qt::green});
    }


    /// Marks all cells as not found by setting their color to red.
    void markNotFound() const {
        for (auto* cell : cells_)
            cell->setBrush(QBrush{Qt::red});
    }


    /// Marks the given index as sorted by coloring it green
    void markSorted(const size_t index) const { markFound(index); }


    void setAnimationDurationMs(const int ms) {
        animationDurationMs_ = ms;
    }


    /**
     * @brief Animates swapping of two cells by making them "float" and
     * exchanging positions.
     *
     * The two indexed cells move upward, cross over, and settle back in the
     * opposite locations. Indices outside the array are ignored.
     *
     * @param first Index of the first cell to swap.
     * @param second Index of the second cell to swap.
     */
    void swapCells(const size_t first, const size_t second) {
        if (first >= cells_.size() || second >= cells_.size() ||
            first == second)
            return;

        auto* rect1 = cells_[first];
        auto* rect2 = cells_[second];

        const qreal x1 = rect1->pos().x();
        const qreal x2 = rect2->pos().x();

        const QPointer anim = new QVariantAnimation(this);
        anim->setDuration(animationDurationMs_);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);

        connect(anim, &QVariantAnimation::valueChanged, this, [=](const QVariant& val) {
            const qreal t = val.toReal();
            rect1->setX(x1 + (x2 - x1) * t);
            rect2->setX(x2 + (x1 - x2) * t);
        });

        connect(anim, &QVariantAnimation::finished, this, [=, this] {
            rect1->setX(x2);
            rect2->setX(x1);

            std::swap(cells_[first], cells_[second]);
            anim->deleteLater();
        });

        anim->start();
    }


    ~ArrayWidget() override = default;
};


} // namespace ui

#endif // ARRAY_WIDGET_HPP
