/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *Browse_Button;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_3;
    QGridLayout *gridLayout;
    QPushButton *ConvertToJSON_Button;
    QPushButton *Prettify_Button;
    QPlainTextEdit *plainTextEdit_2;
    QPushButton *Compress_Button;
    QPushButton *Minify_Button_2;
    QPushButton *Validate_Button;
    QPushButton *Decompress_Button;
    QLabel *label_4;
    QPushButton *Minify_Button;
    QPushButton *Save_Button;
    QMenuBar *menubar;
    QMenu *menuSave;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1036, 743);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(190, 10, 731, 601));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout->addWidget(lineEdit);

        Browse_Button = new QPushButton(layoutWidget);
        Browse_Button->setObjectName("Browse_Button");

        horizontalLayout->addWidget(Browse_Button);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        plainTextEdit = new QPlainTextEdit(layoutWidget);
        plainTextEdit->setObjectName("plainTextEdit");

        verticalLayout->addWidget(plainTextEdit);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        ConvertToJSON_Button = new QPushButton(layoutWidget);
        ConvertToJSON_Button->setObjectName("ConvertToJSON_Button");

        gridLayout->addWidget(ConvertToJSON_Button, 2, 0, 1, 4);

        Prettify_Button = new QPushButton(layoutWidget);
        Prettify_Button->setObjectName("Prettify_Button");

        gridLayout->addWidget(Prettify_Button, 0, 2, 1, 1);

        plainTextEdit_2 = new QPlainTextEdit(layoutWidget);
        plainTextEdit_2->setObjectName("plainTextEdit_2");

        gridLayout->addWidget(plainTextEdit_2, 4, 0, 1, 4);

        Compress_Button = new QPushButton(layoutWidget);
        Compress_Button->setObjectName("Compress_Button");

        gridLayout->addWidget(Compress_Button, 1, 3, 1, 1);

        Minify_Button_2 = new QPushButton(layoutWidget);
        Minify_Button_2->setObjectName("Minify_Button_2");

        gridLayout->addWidget(Minify_Button_2, 1, 0, 1, 1);

        Validate_Button = new QPushButton(layoutWidget);
        Validate_Button->setObjectName("Validate_Button");

        gridLayout->addWidget(Validate_Button, 0, 3, 1, 1);

        Decompress_Button = new QPushButton(layoutWidget);
        Decompress_Button->setObjectName("Decompress_Button");

        gridLayout->addWidget(Decompress_Button, 1, 1, 1, 2);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 3, 0, 1, 4, Qt::AlignmentFlag::AlignHCenter);

        Minify_Button = new QPushButton(layoutWidget);
        Minify_Button->setObjectName("Minify_Button");

        gridLayout->addWidget(Minify_Button, 0, 0, 1, 2);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_2->addLayout(verticalLayout);

        Save_Button = new QPushButton(layoutWidget);
        Save_Button->setObjectName("Save_Button");

        verticalLayout_2->addWidget(Save_Button);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1036, 25));
        menuSave = new QMenu(menubar);
        menuSave->setObjectName("menuSave");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSave->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Browse an XML file", nullptr));
        Browse_Button->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "File Content Browsed", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Choose an operation", nullptr));
        ConvertToJSON_Button->setText(QCoreApplication::translate("MainWindow", "Convert XML to JSON", nullptr));
        Prettify_Button->setText(QCoreApplication::translate("MainWindow", "Prettify", nullptr));
        Compress_Button->setText(QCoreApplication::translate("MainWindow", "Compress", nullptr));
        Minify_Button_2->setText(QCoreApplication::translate("MainWindow", "Minify", nullptr));
        Validate_Button->setText(QCoreApplication::translate("MainWindow", "Validate", nullptr));
        Decompress_Button->setText(QCoreApplication::translate("MainWindow", "Decompress", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Output of Operation on XML File", nullptr));
        Minify_Button->setText(QCoreApplication::translate("MainWindow", "Minify", nullptr));
        Save_Button->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        menuSave->setTitle(QCoreApplication::translate("MainWindow", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
