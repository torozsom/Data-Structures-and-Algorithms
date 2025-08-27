#include <QApplication>
#include <QString>

#include "ArrayWidget.hpp"
#include "DynamicArray.hpp"
#include "LinearSearchAnimator.hpp"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    const data_structs::DynamicArray values{3, 7, 2, 9, 5, 1, 8, 4, 6};
    constexpr int target = 4; // change to a value not in the list to see "not found"

    // Create the view widget that renders the array
    ui::ArrayWidget view(values);
    view.setWindowTitle("Algorithms Visualizer - Linear Search");
    view.resize(600, 120);
    view.show();

    // Start the animator; it will drive the view via timer callbacks
    const ui::LinearSearchAnimator animator(values, target, &view, &view);

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
