#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#include <QLineEdit>
#include "settings.h"
#include <QWidget>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

public slots:
    void init(bool doShow = true);

private:
    QLineEdit* edits[4];
    Ui::SettingsWidget *ui;
private slots:
    void saveSettings();
};

#endif // SETTINGSWIDGET_H
