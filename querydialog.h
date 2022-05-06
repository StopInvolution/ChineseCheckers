#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H
/**
 * Obviously, using an additional querydialog makes no sense.
 * This file is deserted.
*/
#include <QDialog>

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = nullptr);
    explicit QueryDialog(QWidget *parent = nullptr, const char* txt = "");
    ~QueryDialog();

private:
    Ui::QueryDialog *ui;
    QString question;
};

#endif // QUERYDIALOG_H
