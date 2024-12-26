#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Browse_Button_clicked();

    void on_Prettify_Button_clicked();

    void on_Decompress_Button_2_clicked();

    void on_Save_Button_clicked();

    void on_Decompress_Button_clicked();

    void on_Compress_Button_clicked();

    void on_ConvertToJSON_Button_clicked();

    void on_Minify_Button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
