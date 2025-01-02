#include "mainwindow.h"
#include <QFileDialog>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <string>   // For std::string
#include <cctype>   // For isspace
#include <iostream> // For std::cout (optional)
#include <QStack>
#include "xml_tree.h"
#include <QString>
#include<unordered_map>
#include <QWidget>
#include <QDialog>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
