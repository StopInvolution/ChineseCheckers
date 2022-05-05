#ifndef INITWIDGET_H
#define INITWIDGET_H

#include <QWidget>

namespace Ui {
class initWidget;
}

class initWidget : public QWidget
{
    Q_OBJECT

public:
    explicit initWidget(QWidget *parent = nullptr);
    ~initWidget();

    signals:
    void start(int playerNum);

private slots:
    void on_pushButton_clicked();

private:
    bool valid_check(int); //check if the number of players is valid.
    Ui::initWidget *ui;
};

#endif // INITWIDGET_H
