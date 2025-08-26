#include <QApplication>
#include <QString>

#include "ArrayWidget.h"
#include "DynamicArray.hpp"
#include "LinearSearchAnimator.hpp"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Data to visualize
    int data[9] = {3, 7, 2, 9, 5, 1, 8, 4, 6};
    const data_structs::DynamicArray values(data, 9);
    constexpr int target = 5; // change to a value not in the list to see "not found"

    // Create the view widget that renders the array
    ui::ArrayWidget view(values);
    view.setWindowTitle("Algorithms Visualizer - Linear Search");
    view.resize(600, 120);
    view.show();

    // Prepare the array for the animator
    data_structs::DynamicArray<int> arr;
    for (const int v : values)
        arr.addLast(v);

    // Start the animator; it will drive the view via timer callbacks
    const ui::LinearSearchAnimator animator(arr, target, &view, &view);

    // Optionally react to end-of-search signals
    QObject::connect(
        &animator, &ui::LinearSearchAnimator::elementFound, &view,
        [&](const std::size_t index) {
            view.setWindowTitle(
                QString("Found %1 at index %2").arg(target).arg(index));
        });

    QObject::connect(
        &animator, &ui::LinearSearchAnimator::elementNotFound, &view, [&] {
            view.setWindowTitle(QString("Element %1 not found").arg(target));
        });

    return app.exec();
}
