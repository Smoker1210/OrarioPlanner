#include "editemployeewidget.h"
#include "../model/utility/utility.h"

namespace GUI{
editEmployeeWidget::editEmployeeWidget(): selectedEmployee(nullptr) {
    Titolo->setText("Modifica Dipendente");
    saveButton->setText("Salva Modifiche");
}

void editEmployeeWidget::setEmployee(const shiftplanner::employee* dip){
    if(dip){
        selectedEmployee=dip;
        onEmployeePassed();
    }
}

void editEmployeeWidget::packageDipendente(){
    assembleDipendente();
    if(nuovoDipendente){
        if(selectedEmployee){
            emit sendDeleteRequest(selectedEmployee);
            selectedEmployee=nullptr;
        }
        emit sendDipendente(nuovoDipendente);
        nuovoDipendente=nullptr;
    }

}

void editEmployeeWidget::onEmployeePassed(){
    NomeField->setText(QString::fromStdString(selectedEmployee->getNome()));
    CognomeField->setText(QString::fromStdString(selectedEmployee->getCognome()));
    OreField->setValue(selectedEmployee->getOreSett());
    std::string str = selectedEmployee->getRuolo();
    shiftplanner::utility::upperFirstLetter(str);
    RuoloField->setCurrentText(QString::fromStdString(str));
    CategoriaProtField->setChecked(selectedEmployee->isCategoriaProtetta());
}

}