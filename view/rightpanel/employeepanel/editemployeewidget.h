#ifndef EDITEMPLOYEEWIDGET_H
#define EDITEMPLOYEEWIDGET_H

#include "newemployeewidget.h"

namespace GUI{
class editEmployeeWidget : public newEmployeeWidget
{
    Q_OBJECT
private:
    const shiftplanner::employee* selectedEmployee;
protected slots:
    //manda il nuovo dipendente assemblato dai field dati del widget
    void packageDipendente() override;
public:
    explicit editEmployeeWidget();
    void setEmployee(const shiftplanner::employee* employee);
public slots:
    //riempie i field del widget
    void onEmployeePassed();
signals:
    void sendDeleteRequest(const shiftplanner::employee* target);
};
}
#endif // EDITEMPLOYEEWIDGET_H
