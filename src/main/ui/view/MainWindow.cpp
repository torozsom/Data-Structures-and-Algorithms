#include "MainWindow.h"

#include "./array/ui_ArrayPage.h"


namespace ui {


/**
 * @brief Slot invoked when the Dynamic Array button is clicked.
 *
 * Lazily creates the ArrayPage and replaces the main window's central widget
 * with it so that array animations can be shown.
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
