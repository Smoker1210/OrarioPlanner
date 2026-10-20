#include "newemployeewidget.h"
#include <QBoxLayout>
#include "../model/utility/utility.h"
namespace GUI{
newEmployeeWidget::newEmployeeWidget(QWidget *parent): QWidget{parent}, nuovoDipendente(nullptr){
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    Titolo = new QLabel("Aggiunta nuovo dipendente",this);
    QFont fontTitolo = Titolo->font();
    fontTitolo.setBold(1);
    fontTitolo.setPointSize(16);
    Titolo->setFont(fontTitolo);
    layoutPrincipale->addWidget(Titolo);

    QHBoxLayout* keyRow = new QHBoxLayout();
    NomeField = new QLineEdit(this);
    NomeField->setPlaceholderText("Nome");
    keyRow->addWidget(NomeField);
    CognomeField = new QLineEdit(this);
    CognomeField->setPlaceholderText("Cognome");
    keyRow->addWidget(CognomeField);
    layoutPrincipale->addLayout(keyRow);

    QHBoxLayout* rowOre = new QHBoxLayout();
    QLabel* labelOre = new QLabel("Ore Contrattuali: ",this);
    rowOre->addWidget(labelOre);
    OreField = new QSpinBox(this);
    OreField->setMinimum(0.0);
    OreField->setMaximum(60.0);
    OreField->setSingleStep(5);
    rowOre->addWidget(OreField);
    layoutPrincipale->addLayout(rowOre);

    QHBoxLayout* bottomRow = new QHBoxLayout();
    RuoloField = new QComboBox(this);
    RuoloField->setPlaceholderText("Ruolo");
    QStringList ruoliList = {"Responsabile", "Viceresponsabile", "Fiducia", "Addetto"};
    RuoloField->addItems(ruoliList);
    bottomRow->addWidget(RuoloField, 1);
    CategoriaProtField = new QCheckBox(this);
    CategoriaProtField->setChecked(false);
    QHBoxLayout* catProtRow = new QHBoxLayout();
    QLabel* catprotlab = new QLabel("Categoria Protetta:", this);
    catProtRow->addWidget(catprotlab);
    catProtRow->addWidget(CategoriaProtField);
    bottomRow->addLayout(catProtRow, 1);
    layoutPrincipale->addLayout(bottomRow);

    QHBoxLayout* buttonRow = new QHBoxLayout();
    saveButton = new QPushButton("Salva dipendente", this);
    saveButton->setDisabled(true);
    buttonRow->addWidget(saveButton);
    cancelButton = new QPushButton("Annulla", this);
    buttonRow->addWidget(cancelButton);
    layoutPrincipale->addLayout(buttonRow);
    //connects
    connect(saveButton, &QPushButton::clicked, this, &newEmployeeWidget::packageDipendente);
    connect(NomeField, &QLineEdit::textChanged, this, &newEmployeeWidget::tryEnableSave);
    connect(CognomeField, &QLineEdit::textChanged, this, &newEmployeeWidget::tryEnableSave);
    connect(RuoloField, qOverload<int>(&QComboBox::currentIndexChanged),this, &newEmployeeWidget::tryEnableSave);
    connect(cancelButton, &QPushButton::clicked, this, &newEmployeeWidget::sendCancel);
}

void newEmployeeWidget::assembleDipendente(){
    std::string correct = RuoloField->currentText().toStdString();
    shiftplanner::utility::lowerFirstLetter(correct);
    nuovoDipendente = new shiftplanner::employee(NomeField->text().toStdString(), CognomeField->text().toStdString(), OreField->value(),
                                                 shiftplanner::employee::toRuolo(correct), CategoriaProtField->checkState(), true);
}

void newEmployeeWidget::packageDipendente(){
    assembleDipendente();
    if(nuovoDipendente){
        emit sendDipendente(nuovoDipendente);
        nuovoDipendente=nullptr;
    }
}

void newEmployeeWidget::tryEnableSave(){
    if(!NomeField->text().isEmpty() && !CognomeField->text().isEmpty() && !RuoloField->currentText().isEmpty()){
        saveButton->setEnabled(true);
    }
    else
        saveButton->setDisabled(true);
}

void newEmployeeWidget::clearView(){
    NomeField->clear();
    CognomeField->clear();
    OreField->setValue(0);
    RuoloField->setCurrentIndex(-1);
    CategoriaProtField->setChecked(false);
    nuovoDipendente=nullptr;
}





}