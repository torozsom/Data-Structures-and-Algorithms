#ifndef ARRAYWIDGET_HPP
#define ARRAYWIDGET_HPP


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


#include "DynamicArray.hpp"


namespace ui {


/**
 * @class ArrayWidget
 *
 * @brief A widget that visualizes an array of integers as a series of cells
 * with an arrow indicating the current index being examined.
 *
 * The ArrayWidget displays each integer in a rectangular cell arranged
 * horizontally. An arrow above the cells points to the current index being
 * examined. Cells can be highlighted to indicate the current focus of an
 * algorithm, and can be marked to show whether the target element was found or
 * not found.
 */
class ArrayWidget final : public QGraphicsView {

    QGraphicsScene* scene_;
    QGraphicsPolygonItem* arrow_;
    data_structs::DynamicArray<QGraphicsRectItem*> cells_;
    data_structs::DynamicArray<QGraphicsTextItem*> labels_;

    static constexpr int CELL_WIDTH = 40;
    static constexpr int CELL_HEIGHT = 40;

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
    explicit ArrayWidget(const data_structs::DynamicArray<Type>& values,
                         QWidget* parent = nullptr)
        : QGraphicsView(parent), scene_(new QGraphicsScene(this)),
          arrow_(nullptr) {
        setScene(scene_);

        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(2);
        for (std::size_t i = 0; i < values.size(); ++i) {
            auto rect = scene_->addRect(i * CELL_WIDTH, 0, CELL_WIDTH,
                                        CELL_HEIGHT, pen, QBrush(Qt::white));
            cells_.addLast(rect);

            auto text = scene_->addText(QString::number(values[i]));
            text->setDefaultTextColor(Qt::black);
            QRectF textRect = text->boundingRect();
            const qreal textX =
                i * CELL_WIDTH + (CELL_WIDTH - textRect.width()) / 2;
            const qreal textY = (CELL_HEIGHT - textRect.height()) / 2;
            text->setPos(textX, textY);
            labels_.addLast(text);
        }

        const QPolygonF polygon(
            {QPointF(0.0, 0.0), QPointF(-10.0, -20.0), QPointF(10.0, -20.0)});
        arrow_ = scene_->addPolygon(polygon, pen, QBrush(Qt::white));
        setArrowPosition(0);
    }


    /**
     * @brief Highlights the cell at the specified index by changing its color to
     * yellow, and resets all other cells to white. Also moves the arrow above the
     * highlighted cell.
     *
     * @param index The index of the cell to highlight.
     */
    void highlightIndex(const std::size_t index) const {
        for (auto* cell : cells_)
            cell->setBrush(QBrush(Qt::white));

        if (index < cells_.size())
            cells_[index]->setBrush(QBrush(Qt::yellow));

        setArrowPosition(index);
    }


    /**
     * @brief Moves the arrow above the cell at the specified index.
     *
     * If the index is out of bounds, the arrow is not moved.
     *
     * @param index The index of the cell above which to position the arrow.
     */
    void setArrowPosition(const std::size_t index) const {
        if (!arrow_ || index >= cells_.size())
            return;

        const qreal x = index * CELL_WIDTH + CELL_WIDTH / 2;
        arrow_->setPos(x, 0);
    }


    /**
     * @brief Marks the cell at the specified index as found by changing its color
     * to green. Does nothing if the index is out of bounds.
     *
     * @param index The index of the cell to mark as found.
     */
    void markFound(const std::size_t index) const {
        if (index < cells_.size())
            cells_[index]->setBrush(QBrush(Qt::green));
    }


    /// Marks all cells as not found by resetting their color to white.
    void markNotFound() const {
        for (auto* cell : cells_)
            cell->setBrush(QBrush(Qt::red));
    }


    ~ArrayWidget() override = default;
};

} // namespace ui

#endif // ARRAYWIDGET_HPP
