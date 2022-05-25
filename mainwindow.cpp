#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //initialization
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/images/init_background.jpeg").scaled
                                              (this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
}

void MainWindow::on_pushButtonSingle_clicked() {
//    this->hide();
    this->close();
    emit startSingle();
}
void MainWindow::on_pushButtonMultiple_clicked() {
//    this->hide();
    this->close();
    emit startMultiple();
}
void MainWindow::on_ServerButton_clicked() {
    emit startServer();
}
void MainWindow::on_pushButtonSetting_clicked() {
    emit startSetting();
}

MainWindow::~MainWindow()
{
    delete ui;
}
