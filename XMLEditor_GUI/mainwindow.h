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

    void on_Save_Button_clicked();

    void on_Decompress_Button_clicked();

    void on_Compress_Button_clicked();

    void on_ConvertToJSON_Button_clicked();

    void on_Minify_Button_clicked();

    void on_Validate_Button_clicked();

    void on_MostInfluential_clicked();

    void on_MostActive_clicked();

    void on_MutualFollowers_clicked();

    void on_Suggest_clicked();

    void on_SearchByWord_clicked();

    void on_SearchByTopic_clicked();

    void on_pushButton_4_clicked();

    void on_DrawGraph_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
