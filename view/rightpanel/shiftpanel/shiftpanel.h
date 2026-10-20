#ifndef SHIFTPANEL_H
#define SHIFTPANEL_H

#include <QWidget>
#include "../model/shift/shift.h"
#include "employee/employee.h"
#include "shiftmanager/failsafeRules/RulesCard.h"
#include <QTableWidget>
#include <QPushButton>

namespace GUI {

class shiftPanel : public QWidget
{
    Q_OBJECT
private:
    const std::map<std::string, std::array<shiftplanner::shift,7>>& gridShift;
    QTableWidget* shiftTable;
    // segna il giorno critico che presenta un'anomalia d'integrità(1 a 7), se è uguale a -1 non ci sono anomalie
    std::array<bool,7> criticalDays;
    QLineEdit* nota;
    QPushButton* ExportButton;
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
public:
    explicit shiftPanel(const std::map<std::string, std::array<shiftplanner::shift,7>>& grid,QWidget *parent = nullptr);
    static QDate getLunediDaSettimana(int settimana, int anno);

public slots:
    void refreshTable();
    void onShiftChange(QTableWidgetItem* item);
    void updateOreRiga(int row);
    void markAnomalies(const std::vector<shiftplanner::anomaly>& anomalie);
    void cleanCriticalDays();
    void updateHorizontalHeaders(int settimana, int anno);
    void onDipendentePressed(const QTableWidgetItem* item);
    void clearEmployeeSelection();
    void onExportPressed();
    void updateNotes(const std::string& nuovaNota);

signals:
    void sendNewShift(const std::string& key, int giorno, const shiftplanner::shift& turno);
    void requestAttivoBool(const std::string& key,bool* value);
    void requestCatProtettaBool(const std::string& key,bool* value);
    void sendDipInfo(const std::string dipKey);
    void requestSettimanaAnnoPair(std::pair<int,int>& settimanaAnno);
    void sendUpdatedNotes(std::string);
    //chiamato quando viene ridisegnata la tabella (quindi per un'aggiunta o rimozione dipendente)
    void requestEmptyAnomalies();
    void checkIfEmployeeIsEmpty();
};

} // namespace GUI

#endif // SHIFTPANEL_H
