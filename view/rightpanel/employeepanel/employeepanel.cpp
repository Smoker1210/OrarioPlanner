#include "employeepanel.h"
#include <QBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QMessageBox>
#include "../model/utility/utility.h"
#include <algorithm>

namespace GUI{
employeePanel::employeePanel(QWidget *parent): QWidget{parent}, oreTot(0), nDip(0){
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    QLabel* Titolo = new QLabel("Gestione Dipendenti",this);
    QFont fontTitolo = Titolo->font();
    fontTitolo.setBold(1);
    fontTitolo.setPointSize(24);
    Titolo->setFont(fontTitolo);
    QFont fontSottotitolo = fontTitolo;
    fontSottotitolo.setPointSize(16);
    Titolo->setAlignment(Qt::AlignCenter);
    layoutPrincipale->addWidget(Titolo);
    //parte alta layout: info generali, schermata creazione/modifica dipendenti
    QHBoxLayout* layoutSuperiore = new QHBoxLayout();
    QWidget* containerSupSx = new QWidget(this);
    QVBoxLayout* layoutSupSx = new QVBoxLayout(containerSupSx);
    QLabel* sottotitolo = new QLabel("Info Generali", containerSupSx);
    sottotitolo->setFont(fontSottotitolo);
    layoutSupSx->addWidget(sottotitolo);
    QFormLayout* informazioni = new QFormLayout();
    labelTotDip = new QLabel("0", containerSupSx);
    informazioni->addRow("Numero dipendenti attivi: ", labelTotDip);
    labelOre = new QLabel("0", containerSupSx);
    informazioni->addRow("Ore totali contratti: ", labelOre);
    layoutSupSx->addLayout(informazioni);
    layoutSuperiore->addWidget(containerSupSx);
    editContainer = new employeeEditorWidget(this);
    layoutSuperiore->addWidget(editContainer);
    editContainer->hide();
    layoutPrincipale->addLayout(layoutSuperiore);
    //parte bassa layout: lista dipendenti
    QHBoxLayout* topListRow = new QHBoxLayout();
    topListRow->setContentsMargins(8,4,8,4);
    QLabel* labelAttivo = new QLabel("Attivo", this);
    QLabel* labelnome = new QLabel("Nome", this);
    QLabel* labelcognome = new QLabel("Cognome", this);
    QLabel* labelore = new QLabel("Ore", this);
    QLabel* labelruolo = new QLabel("Ruolo", this);
    QLabel* labelCategoriaProtetta = new QLabel("Categoria Protetta");
    topListRow->addWidget(labelAttivo, 1);
    topListRow->addWidget(labelnome, 1);
    topListRow->addWidget(labelcognome, 1);
    topListRow->addWidget(labelore, 1);
    topListRow->addWidget(labelruolo, 1);
    topListRow->addWidget(labelCategoriaProtetta, 1);
    topListRow->addStretch(2);
    layoutPrincipale->addLayout(topListRow);
    QHBoxLayout* bottomRow = new QHBoxLayout();
    employeeList = new QListWidget(this);
    bottomRow->addWidget(employeeList,3);
    QVBoxLayout* latoDestro = new QVBoxLayout();
    newEmployeeButton = new QPushButton("+ Nuovo Dipendente",this);
    latoDestro->addWidget(newEmployeeButton);
    editEmployeeButton = new QPushButton("Modifica Dipendente", this);
    latoDestro->addWidget(editEmployeeButton);
    editEmployeeButton->setDisabled(true);
    deleteEmployeeButton = new QPushButton("- Rimuovi Dipendente", this);
    deleteEmployeeButton->setEnabled(0);
    deleteEmployeeButton->setStyleSheet("QPushButton {"
                                        "   color: #bf0000;"          // Rosso acceso quando è abilitato
                                        "}"
                                        "QPushButton:hover {"
                                        "   color: #e60000;"          // Rosso più brillante al passaggio del mouse
                                        "}"
                                        "QPushButton:disabled {"
                                        "   color: #664444;"          // Rosso scuro/sbiadito quando è disabilitato
                                        "}");
    latoDestro->addWidget(deleteEmployeeButton);
    backButton = new QPushButton("Indietro", this);
    latoDestro->addStretch();
    latoDestro->addWidget(backButton);
    bottomRow->addLayout(latoDestro,1);
    layoutPrincipale->addLayout(bottomRow);
    //layoutPrincipale->addStretch();
    //connect
    connect(editContainer, &employeeEditorWidget::sendNuovoDipendente, this, &employeePanel::receiveNuovoDipendenteFromWidget);
    connect(newEmployeeButton, &QPushButton::clicked, this, [this](){
        editContainer->show();
    });
    connect(backButton, &QPushButton::clicked, this, [this](){
        clearSelectedEmployee();
        emit this->onBackPressed();
        emit this->requestSave();
    });
    connect(employeeList, &QListWidget::itemClicked, this, &employeePanel::onEmployeeClicked);
    connect(deleteEmployeeButton, &QPushButton::clicked, this, [this](){
        if(dipendenteFocused){
            bool commit = askToDelete();
            if (commit){
            emit this->onDeletePressed(dipendenteFocused);
            dipendenteFocused=nullptr;
            editEmployeeButton->setDisabled(true);
            deleteEmployeeButton->setDisabled(true);
            }
        }
    });
    connect(editEmployeeButton, &QPushButton::clicked, this, [this](){
        if(dipendenteFocused){
            editContainer->acceptDipPtr(dipendenteFocused);
            editContainer->show();
        }
    });
    //gestisce la delete della copia originale del dipendente post modifica
    connect(editContainer, &employeeEditorWidget::sendDeleteRequestFromEdit, this, [this](const shiftplanner::employee* target){
        if(target){
            emit this->onDeleteFromEdit(target);
            dipendenteFocused=nullptr;
            editEmployeeButton->setDisabled(true);
            deleteEmployeeButton->setDisabled(true);
        }
    });
}

bool employeePanel::askToDelete(){
    QString str("Rimozione: " + QString::fromStdString(dipendenteFocused->getNome()) + " " + QString::fromStdString(dipendenteFocused->getCognome()));
    QMessageBox::StandardButton risposta = QMessageBox::question(
        this, QString(str),
        QString(tr("Sei sicuro di voler rimuovere il dipendente? \n (Questa azione è irreversibile)")), QMessageBox::Yes | QMessageBox::Cancel
    );
    if (risposta == QMessageBox::Yes){
        return true;
    }
    else if(risposta == QMessageBox::Cancel){
        return false;
    }
    return true;
}

void employeePanel::clearLista(){
    nDip=0;
    oreTot=0;
    listaDipendenti.clear();
    refreshEmployeeList();
}

// void employeePanel::refreshEmployeeList(){
//     employeeList->clear();
//     for(const auto& dip : listaDipendenti){
//         if(dip->isAttivo()){
//         nDip++;
//         oreTot += dip->getOreSett();
//         }
//         std::string upperRuolo = dip->getRuolo();
//         shiftplanner::utility::upperFirstLetter(upperRuolo);
//         QListWidgetItem* item = new QListWidgetItem();
//         employeeRowWidget* rowWidget = new employeeRowWidget(QString::fromStdString(dip->getNome()), QString::fromStdString(dip->getCognome()),
//                                                          dip->getOreSett(), QString::fromStdString(upperRuolo), dip->isAttivo(), dip->isCategoriaProtetta());
//         item->setSizeHint(rowWidget->sizeHint());
//         item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(dip)));
//         employeeList->addItem(item);
//         employeeList->setItemWidget(item, rowWidget);
//         connect(rowWidget, &employeeRowWidget::onAttivoChanged, this, &employeePanel::onAttivoChangedOnList);
//     }
// }

void employeePanel::refreshEmployeeList(){
    employeeList->clear();
    for(const auto& dip : listaDipendenti){
        if(dip)
        addDipToWidget(dip);
    }
}

void employeePanel::addDipToWidget(const shiftplanner::employee* dip){
    if(dip->isAttivo()){
        nDip++;
        oreTot += dip->getOreSett();
    }
    std::string upperRuolo = dip->getRuolo();
    shiftplanner::utility::upperFirstLetter(upperRuolo);
    QListWidgetItem* item = new QListWidgetItem();
    employeeRowWidget* rowWidget = new employeeRowWidget(QString::fromStdString(dip->getNome()), QString::fromStdString(dip->getCognome()),
                                                         dip->getOreSett(), QString::fromStdString(upperRuolo), dip->isAttivo(), dip->isCategoriaProtetta());
    item->setSizeHint(rowWidget->sizeHint());
    item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(dip)));
    employeeList->addItem(item);
    employeeList->setItemWidget(item, rowWidget);
    connect(rowWidget, &employeeRowWidget::onAttivoChanged, this, &employeePanel::onAttivoChangedOnList);
}

// void employeePanel::addDip(const shiftplanner::employee* dipendente){
//     if(dipendente){
//         if(dipendente->isAttivo()){
//             oreTot += dipendente->getOreSett();
//             nDip +=1;
//         }
//     listaDipendenti.push_back(dipendente);
//     std::string upperRuolo = dipendente->getRuolo();
//     shiftplanner::utility::upperFirstLetter(upperRuolo);
//     QListWidgetItem* item = new QListWidgetItem();
//     employeeRowWidget* rowWidget = new employeeRowWidget(QString::fromStdString(dipendente->getNome()), QString::fromStdString(dipendente->getCognome()),
//                                                          dipendente->getOreSett(), QString::fromStdString(upperRuolo), dipendente->isAttivo(), dipendente->isCategoriaProtetta());
//     item->setSizeHint(rowWidget->sizeHint());
//     item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(dipendente)));
//     employeeList->addItem(item);
//     employeeList->setItemWidget(item, rowWidget);
//     }
// }

void employeePanel::addDip(const shiftplanner::employee* dipendente){
    if(dipendente){
        listaDipendenti.push_back(dipendente);
        addDipToWidget(dipendente);
    }
}

void employeePanel::removeDip(const std::string& key){
    for(auto it = listaDipendenti.begin(); it!=listaDipendenti.end(); ++it){
        if((*it)->getKey() == key){
            if((*it)->isAttivo()){
                nDip-=1;
                oreTot-=(*it)->getOreSett();
            }
            it = listaDipendenti.erase(it);
            break;
        }
    }
    refreshEmployeeList();
}

void employeePanel::replaceDip(const std::string& key, const shiftplanner::employee* nuovoDip){
    bool found = false;
    for(auto it = listaDipendenti.begin(); it!=listaDipendenti.end(); ++it){
        if((*it)->getKey() == key){
            it = listaDipendenti.insert(it, nuovoDip);
            ++it;
            it = listaDipendenti.erase(it);
            found = true;
            break;
        }
    }
    if(!found)
        removeDip(key);
    else
        refreshEmployeeList();
}

void employeePanel::receiveNuovoDipendenteFromWidget(shiftplanner::employee* nuovoDip){
    if(nuovoDip){
        addDip(nuovoDip);
        emit sendNuovoDipendenteFromWidget(nuovoDip);
    }
}

void employeePanel::keyPressEvent(QKeyEvent* event){
    if(event->key() ==  Qt::Key_Escape){
        clearSelectedEmployee();
        emit onBackPressed();
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}

void employeePanel::onEmployeeClicked(QListWidgetItem* item){
    if(!item) return;
    quintptr ptr = item->data(Qt::UserRole).value<quintptr>();
    const auto* dipendente = reinterpret_cast<const shiftplanner::employee*>(ptr);
    if(dipendente){
        editEmployeeButton->setEnabled(true);
        deleteEmployeeButton->setEnabled(true);
        dipendenteFocused = dipendente;
    }
}

void employeePanel::updateStats(){
    labelOre->setText(QString::number(oreTot));
    labelTotDip->setText(QString::number(nDip));
}

void employeePanel::clearSelectedEmployee(){
    employeeList->clearSelection();
    editEmployeeButton->setDisabled(true);
    deleteEmployeeButton->setDisabled(true);
    editContainer->hideSelf();
    dipendenteFocused=nullptr;
}

void employeePanel::onAttivoChangedOnList(const GUI::employeeRowWidget* row, bool state){
    for(int i = 0; i< employeeList->count(); ++i){
        QListWidgetItem* item = employeeList->item(i);
        if(employeeList->itemWidget(item) == row){
            quintptr ptr = item->data(Qt::UserRole).value<quintptr>();
            const auto* dip = reinterpret_cast<const shiftplanner::employee*>(ptr);
            if (dip){
                sendModificaDipAttivo(dip->getKey(), state);
            }
            break;
        }
    }
}

}