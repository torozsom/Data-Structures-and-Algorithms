#ifndef ARRAYWIDGET_H
#define ARRAYWIDGET_H


#include <QGraphicsRectItem>
#include <QGraphicsView>

#include "DynamicArray.hpp"

namespace ui {

/**
 * @class ArrayWidget
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
    explicit ArrayWidget(const data_structs::DynamicArray<int>& values,
                         QWidget* parent = nullptr);

    void highlightIndex(std::size_t index) const;
    void setArrowPosition(std::size_t index) const;
    void markFound(std::size_t index) const;
    void markNotFound() const;
};

} // namespace ui

#endif // ARRAYWIDGET_H
