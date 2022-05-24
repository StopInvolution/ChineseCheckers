#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>
#include <QBrush>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    signals:
    void startSingle();
    void startMultiple();
    void startServer();
    void startSetting();

private:
    Ui::MainWindow *ui;

private slots:
    void on_pushButtonSingle_clicked();
    void on_pushButtonMultiple_clicked();
    void on_ServerButton_clicked();
    void on_pushButtonSetting_clicked();
};

#endif // MAINWINDOW_H
