#ifndef EMPLOYEEPANEL_H
#define EMPLOYEEPANEL_H
#include "../model/employee/employee.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include "employeeeditorwidget.h"
#include "employeerowwidget.h"
namespace GUI{
class employeePanel : public QWidget
{
    Q_OBJECT
private:
    std::vector<const shiftplanner::employee*> listaDipendenti;
    const shiftplanner::employee* dipendenteFocused;
    QLabel* labelOre;
    QLabel* labelTotDip;
    unsigned int oreTot;
    unsigned int nDip;
    QPushButton* newEmployeeButton;
    QListWidget* employeeList;
    QPushButton* deleteEmployeeButton;
    QPushButton* editEmployeeButton;
    employeeEditorWidget* editContainer;
    QPushButton* backButton;
    bool askToDelete();
private slots:
    void onEmployeeClicked(QListWidgetItem* item);
protected:
    void keyPressEvent(QKeyEvent* event) override;
public:
    explicit employeePanel(QWidget *parent = nullptr);
    //funzione che aggiunge il dipendente sia nel lista interna del pannello dipendenti che nel widgetList
    void addDip(const shiftplanner::employee* dipendente);
    //funzione che aggiunge il dipendente unicamente nel widgetList
    void addDipToWidget(const shiftplanner::employee* dip);
    void removeDip(const std::string& key);
    // il replace viene usato in caso la modifica di un dipendente mantenga la stessa chiave
    void replaceDip(const std::string& key, const shiftplanner::employee* nuovoDip);

public slots:
    void refreshEmployeeList();
    void receiveNuovoDipendenteFromWidget(shiftplanner::employee* nuovoDip);
    void clearLista();
    void updateStats();
    void onAttivoChangedOnList(const GUI::employeeRowWidget* row, bool state);
    void clearSelectedEmployee();

signals:
    void sendNuovoDipendenteFromWidget(shiftplanner::employee* nuovoDip);
    void onBackPressed();
    void onDeletePressed(const shiftplanner::employee* target);
    void onEditPressed(const shiftplanner::employee* target);
    void onDeleteFromEdit(const shiftplanner::employee* target);
    void sendModificaDipAttivo(const std::string& key, bool state);
    void requestSave();
};
}
#endif // EMPLOYEEPANEL_H
