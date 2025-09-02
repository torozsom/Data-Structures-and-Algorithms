/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *titleLabel;
    QPushButton *btnDynamicArray;
    QPushButton *btnQueue;
    QPushButton *btnStack;
    QPushButton *btnLinkedList;
    QPushButton *btnBinaryTree;
    QPushButton *btnBST;
    QPushButton *btnMaxHeap;
    QPushButton *btnMinHeap;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 500);
        MainWindow->setMinimumSize(QSize(800, 500));
        MainWindow->setMaximumSize(QSize(800, 500));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setHorizontalSpacing(24);
        gridLayout->setVerticalSpacing(16);
        gridLayout->setContentsMargins(16, 16, 16, 16);
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setEnabled(true);
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setStyleSheet(QString::fromUtf8("\n"
"                                color: yellow;\n"
"                            "));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(titleLabel, 0, 0, 1, 2);

        btnDynamicArray = new QPushButton(centralwidget);
        btnDynamicArray->setObjectName("btnDynamicArray");
        btnDynamicArray->setCursor(QCursor(Qt::PointingHandCursor));
        btnDynamicArray->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnDynamicArray->setFlat(true);

        gridLayout->addWidget(btnDynamicArray, 1, 0, 1, 1);

        btnQueue = new QPushButton(centralwidget);
        btnQueue->setObjectName("btnQueue");
        btnQueue->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnQueue->setCursor(QCursor(Qt::PointingHandCursor));
        btnQueue->setFlat(true);

        gridLayout->addWidget(btnQueue, 2, 0, 1, 1);

        btnStack = new QPushButton(centralwidget);
        btnStack->setObjectName("btnStack");
        btnStack->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnStack->setCursor(QCursor(Qt::PointingHandCursor));
        btnStack->setFlat(true);

        gridLayout->addWidget(btnStack, 3, 0, 1, 1);

        btnLinkedList = new QPushButton(centralwidget);
        btnLinkedList->setObjectName("btnLinkedList");
        btnLinkedList->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnLinkedList->setCursor(QCursor(Qt::PointingHandCursor));
        btnLinkedList->setFlat(true);

        gridLayout->addWidget(btnLinkedList, 4, 0, 1, 1);

        btnBinaryTree = new QPushButton(centralwidget);
        btnBinaryTree->setObjectName("btnBinaryTree");
        btnBinaryTree->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnBinaryTree->setCursor(QCursor(Qt::PointingHandCursor));
        btnBinaryTree->setFlat(true);

        gridLayout->addWidget(btnBinaryTree, 1, 1, 1, 1);

        btnBST = new QPushButton(centralwidget);
        btnBST->setObjectName("btnBST");
        btnBST->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnBST->setCursor(QCursor(Qt::PointingHandCursor));
        btnBST->setFlat(true);

        gridLayout->addWidget(btnBST, 2, 1, 1, 1);

        btnMaxHeap = new QPushButton(centralwidget);
        btnMaxHeap->setObjectName("btnMaxHeap");
        btnMaxHeap->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnMaxHeap->setCursor(QCursor(Qt::PointingHandCursor));
        btnMaxHeap->setFlat(true);

        gridLayout->addWidget(btnMaxHeap, 3, 1, 1, 1);

        btnMinHeap = new QPushButton(centralwidget);
        btnMinHeap->setObjectName("btnMinHeap");
        btnMinHeap->setStyleSheet(QString::fromUtf8("\n"
"                                QPushButton {\n"
"                                padding: 8px 12px;\n"
"                                color: black;\n"
"                                background: gray;\n"
"                                font-size: 16px;\n"
"                                border: 1px solid white;\n"
"                                border-radius: 6px;\n"
"                                }\n"
"\n"
"                                QPushButton:hover {\n"
"                                background: #c4c4c4;\n"
"                                }\n"
"\n"
"                                QPushButton:pressed {\n"
"                                background: #d5e9fd;\n"
"                                font-weight: bold;\n"
"                                }\n"
"                            "));
        btnMinHeap->setCursor(QCursor(Qt::PointingHandCursor));
        btnMinHeap->setFlat(true);

        gridLayout->addWidget(btnMinHeap, 4, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Algorithms Visualizer", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Data Structures & Algorithms", nullptr));
        btnDynamicArray->setText(QCoreApplication::translate("MainWindow", "Dynamic Array", nullptr));
        btnQueue->setText(QCoreApplication::translate("MainWindow", "Queue", nullptr));
        btnStack->setText(QCoreApplication::translate("MainWindow", "Stack", nullptr));
        btnLinkedList->setText(QCoreApplication::translate("MainWindow", "Linked List", nullptr));
        btnBinaryTree->setText(QCoreApplication::translate("MainWindow", "Binary Tree", nullptr));
        btnBST->setText(QCoreApplication::translate("MainWindow", "Binary Search Tree", nullptr));
        btnMaxHeap->setText(QCoreApplication::translate("MainWindow", "Max Heap", nullptr));
        btnMinHeap->setText(QCoreApplication::translate("MainWindow", "Min Heap", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
