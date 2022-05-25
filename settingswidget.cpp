#include "settingswidget.h"
#include "ui_settingswidget.h"


SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    this->edits[0] = ui->IP_1;
    this->edits[1] = ui->IP_2;
    this->edits[2] = ui->IP_3;
    this->edits[3] = ui->IP_4;
}

void SettingsWidget::init() {
    for(int i = 0; i<4; ++i)
        this->edits[i]->setText(Network::ip[i]);
    this->ui->port->setText(QString::number(Network::port));
    show();
}

void SettingsWidget::saveSettings()
{
    bool invalidIP = false, invalidPort = false;
    for(int i = 0; i<4; ++i) {
        bool tmp = true;
        int t = this->edits[i]->text().toInt(&tmp);
        if(tmp == false) invalidIP = true;
        if(invalidIP || t > 255 || t < 0){
            invalidIP = true;
            break;
        }
    }
    int x = this->ui->port->text().toInt(&invalidPort);
    invalidPort = !invalidPort;
    if(invalidPort || x < 0 || x > 65535) invalidPort = true;

    if(invalidPort || invalidIP) {
        QString message = "Invalid ";
        if(invalidPort) message.append("port ");
        if(invalidIP) message.append(invalidPort?"and IP ":"IP ");
        message.append("is entered, and all changes have been discarded.");
        QMessageBox::critical(this, tr("Invalid input"), message);
    }else{
        for(int i = 0; i<4; ++i)
            Network::ip[i] = this->edits[i]->text();
        Network::port = this->ui->port->text().toInt();
    }
    close();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}