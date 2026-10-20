#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H
#include "../model/shopconfig/shopconfig.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QDoubleSpinBox>
namespace GUI{
class settingsPanel : public QWidget
{
    Q_OBJECT
private:
    // const shiftplanner::shopConfig* config;
    shiftplanner::shopConfig newConfig;
    QTableWidget* tabellaOrari;
    QPushButton* saveButton;
    QPushButton* backButton;
    QLabel* redditivita;
    QDoubleSpinBox* editOreMax;
    QSpinBox* editIncasso;
    QSpinBox* editSettimana;
    void setupTable();
protected:
    void keyPressEvent(QKeyEvent* event) override;
public:
    explicit settingsPanel(const shiftplanner::shopConfig& configNegozio, QWidget *parent = nullptr);
    void setConfig(const shiftplanner::shopConfig& configNegozio);
private slots:
    void saveAll();
    void updateRedditivita();
public slots:
    void refreshTable();
    void refreshInfo();
    void refreshAll();
signals:
    void onBackPressed();
    void onSavePressed(shiftplanner::shopConfig& newConfig, int incasso, double oreMax, int settimana);
};
}
#endif // SETTINGSPANEL_H
