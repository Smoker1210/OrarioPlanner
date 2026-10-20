#include "employeedetailwidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include "../model/utility/utility.h"
namespace GUI{
employeeDetailWidget::employeeDetailWidget(QWidget *parent): QWidget{parent}, dipDettaglio(nullptr){
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    stack = new QStackedWidget(this);
    //page 0 - nessuna selezione
    QWidget* containerPage0 = new QWidget(stack);
    QVBoxLayout* layoutPage0 = new QVBoxLayout(containerPage0);
    QLabel* labelFoto = new QLabel(stack);
    QPixmap pixmap(":/resources/leftPanel/magnifying_glass.png");
    QPixmap scaled = pixmap.scaled(64,64,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    labelFoto->setPixmap(scaled);
    labelFoto->setAlignment(Qt::AlignCenter);
    stack->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QLabel* subtitle = new QLabel("Seleziona un dipendente per vederne i dettagli");
    layoutPage0->addWidget(labelFoto);
    layoutPage0->addWidget(subtitle);
    stack->addWidget(containerPage0);
    //page 1 - dipendente selezionato
    QWidget* containerPage1 = new QWidget(stack);
    QVBoxLayout* layoutPage1 = new QVBoxLayout(containerPage1);
    QFormLayout* infodip = new QFormLayout();
    nomeCognome = new QLabel(containerPage1);
    infodip->addRow("Nominativo: ", nomeCognome);
    ruolo = new QLabel(containerPage1);
    infodip->addRow("Ruolo: ", ruolo);
    oreContrattuali = new QLabel(containerPage1);
    infodip->addRow("Ore Contratto: ", oreContrattuali);
    oreLavorate = new QLabel(containerPage1);
    infodip->addRow("Ore Impiegate: ", oreLavorate);
    catProtetta = new QLabel("No",containerPage1);
    infodip->addRow("Categoria Protetta: ", catProtetta);
    layoutPage1->addLayout(infodip);
    stack->addWidget(containerPage1);
    layoutPrincipale->addWidget(stack);
    stack->setCurrentIndex(0);
}

void employeeDetailWidget::fillLabels(){
    nomeCognome->setText(QString::fromStdString(dipDettaglio->getNome() + " " + dipDettaglio->getCognome()));
    std::string role = dipDettaglio->getRuolo();
    shiftplanner::utility::upperFirstLetter(role);
    ruolo->setText(QString::fromStdString(role));
    oreContrattuali->setText(QString::number(dipDettaglio->getOreSett()));
    if(dipDettaglio->isCategoriaProtetta()){
        catProtetta->setText("Sì");
    }
    else catProtetta->setText("No");
    double oreImpiegate = 0.0;
    emit this->requestOreImpiegate(dipDettaglio->getKey(), oreImpiegate);
    if(dipDettaglio->isCategoriaProtetta()){
        oreLavorate->setText(QString::number(oreImpiegate*2) + " (" + QString::number(oreImpiegate) + ")");
    }
    else oreLavorate->setText(QString::number(oreImpiegate));
}

void employeeDetailWidget::clearSelection(){
    nomeCognome->clear();
    ruolo->clear();
    oreContrattuali->clear();
    oreLavorate->clear();
    catProtetta->setText("No");
    stack->setCurrentIndex(0);
    dipDettaglio=nullptr;
}

void employeeDetailWidget::acceptInfoDip(const shiftplanner::employee* dip){
    if(dip){
        dipDettaglio = dip;
        fillLabels();
        stack->setCurrentIndex(1);
    }
}

}