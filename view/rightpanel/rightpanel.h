#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H
#include <QStackedWidget>
#include <QWidget>
#include "./employeepanel/employeepanel.h"
#include "./settingspanel.h"
#include "../model/shiftmanager/shiftmanager.h"
#include "./shiftpanel/shiftpanel.h"
#include "./helppanel.h"
#include "./noemployeepanel.h"
namespace GUI{
class rightpanel: public QWidget
{
    Q_OBJECT
private:
    QStackedWidget* stackDestro;
    employeePanel* pageDipendenti;
    settingsPanel* pageImpostazioni;
    shiftPanel* pageOrari;
    helpPanel* pageAiuto;
    noEmployeePanel* pageNoDipendenti;
    const shiftplanner::shiftManager& manager;
public:
    explicit rightpanel(const shiftplanner::shiftManager& gestore,QWidget* parent = nullptr);
public slots:
    //se la lista dei dipendenti attiva risulta vuota piuttosto di disegnare la tabella metto il widget che segnala di abilitarne almeno uno
    void setPageShiftTable();
    void setPageEmployee();
    void setPageConfig();
    void setPageHelp();
    // void setPageNoEmployees();
    void refreshPages();
    void fillEmployeePanel();
    void updateShiftHorizontalHeaders(int settimana, int anno);
signals:
    void sendNewInfo(shiftplanner::shopConfig& newConfig, int incasso, double maxOre, int settimana);
    void sendNuovoDipendente(shiftplanner::employee* nuovoDip);
    void sendDipendenteToDelete(const shiftplanner::employee* target);
    void sendDeleteFromEdit(const shiftplanner::employee* target);
    void sendModificaDipAttivo(const std::string& key, bool state);
    void requestSave();
    void sendNewShift(const std::string& key, int giorno, const shiftplanner::shift& turno);
    void sendAnomalies(const std::vector<shiftplanner::anomaly>& anomalie);
    void sendDipInfo(const std::string& key);
    void cleanCriticalDays();
    void onPageOrariNotFocused();
    void onPageOrariFocused();
    void sendUpdatedNotes(std::string nuovaNota);
    void requestEmptyAnomalies();
};
}
#endif // RIGHTPANEL_H
