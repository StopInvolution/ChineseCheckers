#ifndef SETPLAYERNUMWINDOW_H
#define SETPLAYERNUMWINDOW_H

#include <QDialog>

namespace Ui {
class SetPlayerNumWindow;
}

class SetPlayerNumWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SetPlayerNumWindow(QWidget *parent = nullptr);
    ~SetPlayerNumWindow();

private:
    Ui::SetPlayerNumWindow *ui;
};

#endif // SETPLAYERNUMWINDOW_H
