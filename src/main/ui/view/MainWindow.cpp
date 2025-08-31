#include "MainWindow.h"

#include "./array/ui_ArrayPage.h"


namespace ui {


/**
 * @brief Slot for handling the click event of the dynamic array button.
 *
 * This function checks if the ArrayWindow is already created. If not, it
 * creates a new instance, sets it to delete on close, and connects its
 * destroyed signal to a lambda function that shows the main window again and
 * resets the window_ pointer to nullptr. Finally, it shows the ArrayWindow,
 * raises it to the front, activates it, and hides the main window.
 */
void MainWindow::on_btnDynamicArray_clicked() {
    if (!page_)
        page_ = new ArrayPage(this);

    if (!centralContent_)
        centralContent_ = takeCentralWidget();
    setCentralWidget(page_);
}


/**
 * @brief Connects button actions to their respective slots.
 *
 * This function connects the clicked signal of the dynamic array button
 * to the on_btnDynamicArray_clicked slot, enabling the button to trigger
 * the appropriate action when clicked.
 */
void MainWindow::connectButtonActions() {
    connect(uiForm_->btnDynamicArray, &QPushButton::clicked, this,
            &MainWindow::on_btnDynamicArray_clicked);
}


/**
 * @brief Constructor for the MainWindow class.
 *
 * This constructor initializes the main window, sets up the user interface,
 * and connects button actions to their respective slots.
 *
 * @param parent Pointer to the parent widget, default is nullptr.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), uiForm_(new Ui::MainWindow) {
    uiForm_->setupUi(this);
    connectButtonActions();
}


/// Destructor that cleans up the UI form.
MainWindow::~MainWindow() { delete uiForm_; }


} // namespace ui
