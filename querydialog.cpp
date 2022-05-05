#include "querydialog.h"
#include "ui_querydialog.h"
/**
 * Obviously, using an additional querydialog makes no sense.
 * This file is deserted.
*/
QueryDialog::QueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
    ui->label->setText("???");
}

QueryDialog::QueryDialog(QWidget *parent, const char* txt) :
    QDialog(parent),
    ui(new Ui::QueryDialog),
    question(txt)
{
    ui->setupUi(this);
    ui->label->setText(question);
    connect(ui->buttonBox, SIGNAL(QDialogButtonBox::rejected), this, SLOT(close()));
}

QueryDialog::~QueryDialog()
{
    delete ui;
}
