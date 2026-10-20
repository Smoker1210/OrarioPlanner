#include "employeeeditorwidget.h"
#include <QBoxLayout>
namespace GUI{
employeeEditorWidget::employeeEditorWidget(QWidget *parent): QWidget{parent}{
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    stack = new QStackedWidget(this);
    widgetCreazione = new newEmployeeWidget(stack);
    stack->addWidget(widgetCreazione);


    layoutPrincipale->addWidget(stack);
    layoutPrincipale->addStretch();
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    auto sp = this->sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    this->setSizePolicy(sp);
    stack->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    widgetModifica = new editEmployeeWidget();
    stack->addWidget(widgetModifica);
    //connects
    connect(widgetCreazione, &newEmployeeWidget::sendDipendente, this, [this](shiftplanner::employee* nuovoDip){
        hideSelf();
        emit sendNuovoDipendente(nuovoDip);
    });
    connect(widgetCreazione, &newEmployeeWidget::sendCancel, this, &employeeEditorWidget::hideSelf);
    connect(widgetModifica, &newEmployeeWidget::sendDipendente, this, [this](shiftplanner::employee* nuovoDip){
        hideSelf();
        emit sendNuovoDipendente(nuovoDip);
    });
    connect(widgetModifica, &newEmployeeWidget::sendCancel, this, &employeeEditorWidget::hideSelf);
    connect(widgetModifica, &editEmployeeWidget::sendDeleteRequest, this, &employeeEditorWidget::sendDeleteRequestFromEdit);
}

void employeeEditorWidget::hideSelf(){
    widgetCreazione->clearView();
    hide();
}

void employeeEditorWidget::acceptDipPtr(const shiftplanner::employee* target){
    if(target && widgetModifica){
        widgetModifica->setEmployee(target);
        stack->setCurrentIndex(1);
    }
}

}