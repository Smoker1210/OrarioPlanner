#ifndef LEFTPANEL_H
#define LEFTPANEL_H
#include "shiftmanager/failsafeRules/RulesCard.h"
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QTextBrowser>
#include "employeedetailwidget.h"
namespace GUI{
class leftpanel : public QWidget
{
    Q_OBJECT
private:
    QWidget* info;
    void setupInfo();
    void setupBottomRow(QHBoxLayout* layout);
    employeeDetailWidget* dettaglioDip;
    QLabel* anno;
    QLabel* settimana;
    QLabel* redditivita;
    QLabel* oreMassime;
    QLabel* oreUsate;
    QLabel* incassoPrevisto;
    QPushButton* tastoCheckRules;
    QPushButton* tastoCleanTable;
    QLabel* status;
    QPushButton* tastoDipendenti;
    QPushButton* tastoConfig;
    QTextBrowser* casellaErrori;
    static QFrame* creaLineaH(const QString& colore = "#2c2c2c", int spessore = 1, QWidget* parent = nullptr);
    std::vector<std::string> errori;
    //piccolo bool per controllare se è stata effettuata un controllo integrità, serve quando si toglie dal focus la tabella orari
    //per controllare se serve riaccendere o meno il tasto resetta stato
    bool isTableStateResettable = false;
    //costruisce il giorno con gli errori nel pannello sinistro, in caso in cui non ci siano anomalie nel giorno restituisce una stringa vuota
    QString buildDayView(int giorno, const std::vector<shiftplanner::anomaly>& anomalie);
public:
    explicit leftpanel(QWidget *parent = nullptr);
public slots:
    void updateView(int incasso, double oreMax, double oreImpiegate, int settimana, int anno);
    void checkErrorList();
    void refreshIntegrityCheck();
    void acceptAnomalies(const std::vector<shiftplanner::anomaly>& anomalie);
    void setIntegrityCheckEnabled(bool value);

signals:
    void onCheckRulesPressed(std::vector<std::string>& errori);
    void onDipendentiPressed();
    void onConfigPressed();
    void cleanCriticalDay();
    void requestClearAnomalies();
    void sendDipInfo(const shiftplanner::employee* dip);
    void requestOreImpiegate(const std::string& key, double& oreSettimanali);
    void requestFillEmployeeDetail();
    void clearEmployeeDetails();
};
}
#endif // LEFTPANEL_H
