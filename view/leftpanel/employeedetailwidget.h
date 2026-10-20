#ifndef EMPLOYEEDETAILWIDGET_H
#define EMPLOYEEDETAILWIDGET_H
#include "employee/employee.h"
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
namespace GUI{
class employeeDetailWidget : public QWidget
{
    Q_OBJECT
private:
    const shiftplanner::employee* dipDettaglio;
    QStackedWidget* stack;
    QLabel* nomeCognome;
    QLabel* ruolo;
    QLabel* oreContrattuali;
    QLabel* oreLavorate;
    QLabel* catProtetta;

public:
    explicit employeeDetailWidget(QWidget *parent = nullptr);

public slots:
    void acceptInfoDip(const shiftplanner::employee* dip);
    void fillLabels();
    void clearSelection();

signals:
    void requestOreImpiegate(const std::string& key, double& oreSettimanali);
};
}
#endif // EMPLOYEEDETAILWIDGET_H
