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
    containers::DynamicArray<QGraphicsTextItem*> labels_;

    static constexpr size_t CELL_WIDTH = 55;
    static constexpr size_t CELL_HEIGHT = 55;


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

        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(2);

        for (size_t i = 0; i < values.size(); ++i) {
            auto rect = scene_->addRect(0, 0, CELL_WIDTH, CELL_HEIGHT, pen,
                                        QBrush{Qt::white});
            rect->setPos(i * CELL_WIDTH, 0);
            cells_.addLast(rect);

            auto text = scene_->addText(QString::number(values[i]));
            text->setDefaultTextColor(Qt::black);

            QFont font = text->font();
            font.setPointSizeF(static_cast<qreal>(CELL_HEIGHT) *
                               0.3); // scale with cell size
            text->setFont(font);

            QRectF textRect = text->boundingRect();
            const qreal textX =
                (CELL_WIDTH - textRect.width()) / 2 + i * CELL_WIDTH;
            const qreal textY = (CELL_HEIGHT - textRect.height()) / 2;
            text->setPos(textX, textY);
            labels_.addLast(text);
        }

        const QPolygonF polygon{QPointF{0.0, 0.0}, QPointF{-10.0, -20.0},
                                QPointF{10.0, -20.0}};

        arrow_ = scene_->addPolygon(polygon, pen, QBrush{Qt::cyan});
        setArrowPosition(0);

        // Reserve space above the cells for swap animations so that
        // the view's origin doesn't shift when items move upward.
        const qreal width = values.size() * CELL_WIDTH;
        scene_->setSceneRect(0, -20, width, CELL_HEIGHT + 20);
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

        const qreal x = index * CELL_WIDTH + CELL_WIDTH / 2;
        arrow_->setPos(x, 0);
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
        auto* label1 = labels_[first];
        auto* label2 = labels_[second];

        const qreal x1 = rect1->pos().x();
        const qreal x2 = rect2->pos().x();

        const QPointer anim = new QVariantAnimation(this);
        anim->setDuration(500);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);

        connect(
            anim, &QVariantAnimation::valueChanged, this,
            [=](const QVariant& val) {
                const qreal t = val.toReal();
                const qreal dy = (t < 0.5) ? (-20.0 * (t / 0.5))
                                           : (-20.0 * (1 - (t - 0.5) / 0.5));

                rect1->setPos(x1 + (x2 - x1) * t, dy);
                rect2->setPos(x2 + (x1 - x2) * t, -dy);

                label1->setPos(
                    rect1->pos().x() +
                        (CELL_WIDTH - label1->boundingRect().width()) / 2,
                    dy + (CELL_HEIGHT - label1->boundingRect().height()) / 2);
                label2->setPos(
                    rect2->pos().x() +
                        (CELL_WIDTH - label2->boundingRect().width()) / 2,
                    -dy + (CELL_HEIGHT - label2->boundingRect().height()) / 2);
            });

        connect(anim, &QVariantAnimation::finished, this, [=, this] {
            rect1->setPos(x2, 0);
            rect2->setPos(x1, 0);
            label1->setPos(x2 + (CELL_WIDTH - label1->boundingRect().width()) /
                                    2,
                           (CELL_HEIGHT - label1->boundingRect().height()) / 2);
            label2->setPos(x1 + (CELL_WIDTH - label2->boundingRect().width()) /
                                    2,
                           (CELL_HEIGHT - label2->boundingRect().height()) / 2);

            std::swap(cells_[first], cells_[second]);
            std::swap(labels_[first], labels_[second]);

            anim->deleteLater();
        });

        anim->start();
    }


    ~ArrayWidget() override = default;
};


} // namespace ui

#endif // ARRAY_WIDGET_HPP
