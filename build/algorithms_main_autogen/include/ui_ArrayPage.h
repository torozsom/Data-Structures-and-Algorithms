/********************************************************************************
** Form generated from reading UI file 'ArrayPage.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARRAYPAGE_H
#define UI_ARRAYPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArrayPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGridLayout *gridLayout;
    QPushButton *btnBubbleSort;
    QPushButton *btnImpBubbleSort;
    QPushButton *btnInsertSort1;
    QPushButton *btnInsertSort2;
    QPushButton *btnQuickSort;
    QPushButton *btnMergeSort;
    QPushButton *btnHeapSort;
    QPushButton *btnBinSort1;
    QPushButton *btnBinSort2;
    QPushButton *btnRadixSortLSD;
    QPushButton *btnRadixSortMSD;
    QPushButton *btnLinearSearch;
    QPushButton *btnBinarySearch;
    QPushButton *btnTODO;

    void setupUi(QWidget *ArrayPage)
    {
        if (ArrayPage->objectName().isEmpty())
            ArrayPage->setObjectName("ArrayPage");
        ArrayPage->resize(800, 500);
        ArrayPage->setMinimumSize(QSize(800, 500));
        ArrayPage->setMaximumSize(QSize(800, 500));
        verticalLayout = new QVBoxLayout(ArrayPage);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(ArrayPage);
        label->setObjectName("label");
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(false);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("\n"
"                            color: yellow;\n"
"                        "));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setHorizontalSpacing(24);
        gridLayout->setVerticalSpacing(16);
        gridLayout->setContentsMargins(16, 16, 16, 16);
        btnBubbleSort = new QPushButton(ArrayPage);
        btnBubbleSort->setObjectName("btnBubbleSort");
        btnBubbleSort->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnBubbleSort->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnBubbleSort, 0, 0, 1, 1);

        btnImpBubbleSort = new QPushButton(ArrayPage);
        btnImpBubbleSort->setObjectName("btnImpBubbleSort");
        btnImpBubbleSort->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnImpBubbleSort->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnImpBubbleSort, 1, 0, 1, 1);

        btnInsertSort1 = new QPushButton(ArrayPage);
        btnInsertSort1->setObjectName("btnInsertSort1");
        btnInsertSort1->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnInsertSort1->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnInsertSort1, 2, 0, 1, 1);

        btnInsertSort2 = new QPushButton(ArrayPage);
        btnInsertSort2->setObjectName("btnInsertSort2");
        btnInsertSort2->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnInsertSort2->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnInsertSort2, 3, 0, 1, 1);

        btnQuickSort = new QPushButton(ArrayPage);
        btnQuickSort->setObjectName("btnQuickSort");
        btnQuickSort->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnQuickSort->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnQuickSort, 4, 0, 1, 1);

        btnMergeSort = new QPushButton(ArrayPage);
        btnMergeSort->setObjectName("btnMergeSort");
        btnMergeSort->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnMergeSort->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnMergeSort, 5, 0, 1, 1);

        btnHeapSort = new QPushButton(ArrayPage);
        btnHeapSort->setObjectName("btnHeapSort");
        btnHeapSort->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnHeapSort->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnHeapSort, 6, 0, 1, 1);

        btnBinSort1 = new QPushButton(ArrayPage);
        btnBinSort1->setObjectName("btnBinSort1");
        btnBinSort1->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnBinSort1->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnBinSort1, 0, 1, 1, 1);

        btnBinSort2 = new QPushButton(ArrayPage);
        btnBinSort2->setObjectName("btnBinSort2");
        btnBinSort2->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnBinSort2->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnBinSort2, 1, 1, 1, 1);

        btnRadixSortLSD = new QPushButton(ArrayPage);
        btnRadixSortLSD->setObjectName("btnRadixSortLSD");
        btnRadixSortLSD->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnRadixSortLSD->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnRadixSortLSD, 2, 1, 1, 1);

        btnRadixSortMSD = new QPushButton(ArrayPage);
        btnRadixSortMSD->setObjectName("btnRadixSortMSD");
        btnRadixSortMSD->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnRadixSortMSD->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnRadixSortMSD, 3, 1, 1, 1);

        btnLinearSearch = new QPushButton(ArrayPage);
        btnLinearSearch->setObjectName("btnLinearSearch");
        btnLinearSearch->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnLinearSearch->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnLinearSearch, 4, 1, 1, 1);

        btnBinarySearch = new QPushButton(ArrayPage);
        btnBinarySearch->setObjectName("btnBinarySearch");
        btnBinarySearch->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: black;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnBinarySearch->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnBinarySearch, 5, 1, 1, 1);

        btnTODO = new QPushButton(ArrayPage);
        btnTODO->setObjectName("btnTODO");
        btnTODO->setStyleSheet(QString::fromUtf8("\n"
"                                    QPushButton {\n"
"                                    padding: 8px 12px;\n"
"                                    color: red;\n"
"                                    background: gray;\n"
"                                    font-size: 16px;\n"
"                                    border: 1px solid white;\n"
"                                    border-radius: 6px;\n"
"                                    }\n"
"\n"
"                                    QPushButton:hover {\n"
"                                    background: #c4c4c4;\n"
"                                    }\n"
"\n"
"                                    QPushButton:pressed {\n"
"                                    background: #d5e9fd;\n"
"                                    font-weight: bold;\n"
"                                    }\n"
"                                "));
        btnTODO->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(btnTODO, 6, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(ArrayPage);

        QMetaObject::connectSlotsByName(ArrayPage);
    } // setupUi

    void retranslateUi(QWidget *ArrayPage)
    {
        ArrayPage->setWindowTitle(QCoreApplication::translate("ArrayPage", "Dynamic Array", nullptr));
        label->setText(QCoreApplication::translate("ArrayPage", "Choose the algorithm to animate over the Dynamic Array:", nullptr));
        btnBubbleSort->setText(QCoreApplication::translate("ArrayPage", "Bubble Sort", nullptr));
        btnImpBubbleSort->setText(QCoreApplication::translate("ArrayPage", "Improved Bubble Sort", nullptr));
        btnInsertSort1->setText(QCoreApplication::translate("ArrayPage", "Insertion Sort #1", nullptr));
        btnInsertSort2->setText(QCoreApplication::translate("ArrayPage", "Insertion Sort #2", nullptr));
        btnQuickSort->setText(QCoreApplication::translate("ArrayPage", "Quick Sort", nullptr));
        btnMergeSort->setText(QCoreApplication::translate("ArrayPage", "Merge Sort", nullptr));
        btnHeapSort->setText(QCoreApplication::translate("ArrayPage", "HeapSort", nullptr));
        btnBinSort1->setText(QCoreApplication::translate("ArrayPage", "Bin Sort #1", nullptr));
        btnBinSort2->setText(QCoreApplication::translate("ArrayPage", "Bin Sort #2", nullptr));
        btnRadixSortLSD->setText(QCoreApplication::translate("ArrayPage", "Radix Sort #LSD", nullptr));
        btnRadixSortMSD->setText(QCoreApplication::translate("ArrayPage", "Radix Sort #MSD", nullptr));
        btnLinearSearch->setText(QCoreApplication::translate("ArrayPage", "Linear Search", nullptr));
        btnBinarySearch->setText(QCoreApplication::translate("ArrayPage", "Binary Search", nullptr));
        btnTODO->setText(QCoreApplication::translate("ArrayPage", "TODO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ArrayPage: public Ui_ArrayPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARRAYPAGE_H
