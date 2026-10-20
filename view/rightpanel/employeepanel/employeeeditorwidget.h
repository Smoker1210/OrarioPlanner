#ifndef EMPLOYEEEDITORWIDGET_H
#define EMPLOYEEEDITORWIDGET_H
#include "newemployeewidget.h"
#include <QWidget>
#include <QStackedWidget>
#include "editemployeewidget.h"
namespace GUI{
class employeeEditorWidget : public QWidget
{
    Q_OBJECT
private:
    QStackedWidget* stack;
    newEmployeeWidget* widgetCreazione;
    editEmployeeWidget* widgetModifica;
public:
    explicit employeeEditorWidget(QWidget *parent = nullptr);

public slots:
    void hideSelf();
    // funzione che inizializza i parametri dell'edit widget
    void acceptDipPtr(const shiftplanner::employee* target);

signals:
    void sendNuovoDipendente(shiftplanner::employee* nuovoDip);
    void sendDeleteRequest(const shiftplanner::employee* target);
    // non devo salvare immediamente la delete causata da un edit poiché è differente da una delete scelta dall'utente (che è irreversibile)
    void sendDeleteRequestFromEdit(const shiftplanner::employee* target);
};
}
#endif // EMPLOYEEEDITORWIDGET_H
