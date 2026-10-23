#include "shiftpanel.h"
#include <QBoxLayout>
#include <QHeaderView>
#include "./shiftdelegate.h"
#include "../model/utility/utility.h"
#include <QKeyEvent>
#include "shiftmanager/failsafeRules/RulesCard.h"
#include <QDate>
#include <QLabel>
#include <QFileDialog>
#include "../../pdfexporter/pdfexporter.h"
#include <QMessageBox>
#include <QStyleFactory>

namespace GUI {

shiftPanel::shiftPanel(const std::map<std::string, std::array<shiftplanner::shift,7>>& grid, const std::vector<shiftplanner::employee>& listaDip, QWidget *parent): QWidget{parent}, gridShift(grid), listaDipendenti(listaDip){
    criticalDays.fill(false);
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    shiftTable = new QTableWidget(gridShift.size(), 9,this);
    shiftTable ->setItemDelegate(new ShiftDelegate(this));
    shiftTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);
    QStringList giorni = {"","Lunedì", "Martedì", "Mercoledì", "Giovedì", "Venerdì", "Sabato", "Domenica", "Tot Ore"};
    shiftTable->setHorizontalHeaderLabels(giorni);
    shiftTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    shiftTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    shiftTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    shiftTable->verticalHeader()->setVisible(false);
    shiftTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    shiftTable->setWordWrap(true);
    QFont tableFont = shiftTable->font();
    tableFont.setPointSize(12);
    tableFont.setBold(false);
    shiftTable->setFont(tableFont);
    shiftTable->setFocusPolicy(Qt::StrongFocus);
    shiftTable->setStyle(QStyleFactory::create("Fusion"));
    //il primo refresh verrà effettuato dal rightpanel poiché c'era un problema di ordine di popolamento e connect di segnale per richiedere il bool attivo
    //refreshTable();
    //event filter per permettere di editare una cella usando il tasto enter
    shiftTable->installEventFilter(this);
    layoutPrincipale->addWidget(shiftTable);
    QHBoxLayout* notaRow = new QHBoxLayout();
    QLabel* notaLabel = new QLabel("Note:");
    nota = new QLineEdit(this);
    notaRow->addWidget(notaLabel);
    notaRow->addWidget(nota);
    ExportButton = new QPushButton("Esporta come PDF", this);
    notaRow->addWidget(ExportButton);
    layoutPrincipale->addLayout(notaRow);
    clearEmployeeSelection();
    connect(shiftTable, &QTableWidget::itemChanged, this, &shiftPanel::onShiftChange);
    connect(shiftTable, &QTableWidget::currentCellChanged, this, [this](int currentRow, int currentColumn, int previousRow, int previousColumn){
        if(currentRow!=previousRow && currentRow>=0){
            QTableWidgetItem* nameItem = shiftTable->item(currentRow,0);
            if(nameItem){
                emit this->onDipendentePressed(nameItem);
            }
        }
    });
    connect(ExportButton, &QPushButton::clicked, this, &shiftPanel::onExportPressed);
    connect(nota, &QLineEdit::textEdited, this, [this](){
        emit this->sendUpdatedNotes(nota->text().toStdString());
    });
}

void shiftPanel::clearEmployeeSelection(){
    shiftTable->clearSelection();
    shiftTable->setCurrentIndex(QModelIndex());
}

// funzione di refresh che pone la tabella in ordine alfabetico
// void shiftPanel::refreshTable(){
//     int i = 0;
//     double sumOre = 0;
//     shiftTable->blockSignals(true);
//     shiftTable->setRowCount(0);
//     shiftTable->setRowCount(gridShift.size());
//     //implemento la logica che differenzia il nominativo di due persone con stesso nome inserendoci il cognome
//     //essenzialmente do una prima ciclata dei dipendenti attivi con una mappa e poi controllo per il nome se ci sono doppioni, una specie di counting sort
//     std::unordered_map<std::string, int> frequenzaNomi;
//     for(const auto& pair : gridShift){
//         bool attivo = false;
//         emit this->requestAttivoBool(pair.first, &attivo);
//         if(!attivo) continue;
//         frequenzaNomi[shiftplanner::utility::extractNameFromKey(pair.first)]++;
//     }
//     for(const auto& pair : gridShift){
//         bool attivo = false;
//         emit this->requestAttivoBool(pair.first, &attivo);  //chiedo al rightpanel il bool attivo della key corrispondente, se false skippo completamente il dipendente
//         if(!attivo){
//             continue;
//         }
//         bool catProtetta = false;
//         emit this->requestCatProtettaBool(pair.first, &catProtetta); //chiedo al rightpanel il bool categoriaProtetta della key corrispondente, se true dimezzo le ore dei turni
//         QString nomeMostrato = QString::fromStdString(shiftplanner::utility::extractNameFromKey(pair.first));
//         if(frequenzaNomi[nomeMostrato.toStdString()] > 1){
//             nomeMostrato = QString::fromStdString(nomeMostrato.toStdString() + "/" + shiftplanner::utility::extractSurnameFromKey(pair.first));
//         }
//         QTableWidgetItem* itemDipendente = new QTableWidgetItem(nomeMostrato);
//         itemDipendente->setData(Qt::UserRole, QString::fromStdString(pair.first));
//         itemDipendente->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
//         itemDipendente->setFlags(itemDipendente->flags() & ~Qt::ItemIsEditable);
//         itemDipendente->setBackground(QBrush(Qt::transparent));
//         shiftTable->setItem(i,0,itemDipendente);
//         const std::array<shiftplanner::shift,7>& turni = pair.second;
//         int j=1;
//         for(const auto& turno : turni){
//             QTableWidgetItem* itemTurno = new QTableWidgetItem(QString::fromStdString(turno.getTesto()));
//             sumOre += turno.oreTotali();
//             itemTurno->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
//             itemTurno->setBackground(QBrush(Qt::transparent));
//             itemTurno->setTextAlignment(Qt::AlignCenter);
//             shiftTable->setItem(i,j,itemTurno);
//             j++;
//         }
//         if(catProtetta) sumOre = sumOre/2;
//         QTableWidgetItem* itemOre = new QTableWidgetItem(QString::number(sumOre));
//         itemOre->setTextAlignment(Qt::AlignCenter);
//         shiftTable->setItem(i,8,itemOre);
//         sumOre=0;
//         i++;
//     }
//     shiftTable->setRowCount(i);
//     shiftTable->blockSignals(false);
//     emit this->requestEmptyAnomalies();
//     emit this->checkIfEmployeeIsEmpty();
// }

void shiftPanel::refreshTable(){
    int i = 0;
    double sumOre = 0;
    shiftTable->blockSignals(true);
    shiftTable->setRowCount(0);
    shiftTable->setRowCount(gridShift.size());
    //implemento la logica che differenzia il nominativo di due persone con stesso nome inserendoci il cognome
    //essenzialmente do una prima ciclata dei dipendenti attivi con una mappa e poi controllo per il nome se ci sono doppioni, una specie di counting sort
    std::unordered_map<std::string, int> frequenzaNomi;
    for(const auto& pair : gridShift){
        bool attivo = false;
        emit this->requestAttivoBool(pair.first, &attivo);
        if(!attivo) continue;
        frequenzaNomi[shiftplanner::utility::extractNameFromKey(pair.first)]++;
    }
    for(const auto& dip : listaDipendenti){
        if(!dip.isAttivo()){
            continue;
        }
        QString nomeMostrato = QString::fromStdString(dip.getNome());
        if(frequenzaNomi[nomeMostrato.toStdString()] > 1){
            nomeMostrato = QString::fromStdString(nomeMostrato.toStdString() + "/" + dip.getCognome());
        }
        QTableWidgetItem* itemDipendente = new QTableWidgetItem(nomeMostrato);
        itemDipendente->setData(Qt::UserRole, QString::fromStdString(dip.getKey()));
        itemDipendente->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        itemDipendente->setFlags(itemDipendente->flags() & ~Qt::ItemIsEditable);
        itemDipendente->setBackground(QBrush(Qt::transparent));
        shiftTable->setItem(i,0,itemDipendente);
        const auto& pair = gridShift.find(dip.getKey());
        const std::array<shiftplanner::shift,7>& turni = pair->second;
        int j=1;
        for(const auto& turno : turni){
            QTableWidgetItem* itemTurno = new QTableWidgetItem(QString::fromStdString(turno.getTesto()));
            sumOre += turno.oreTotali();
            itemTurno->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            itemTurno->setBackground(QBrush(Qt::transparent));
            itemTurno->setTextAlignment(Qt::AlignCenter);
            shiftTable->setItem(i,j,itemTurno);
            j++;
        }
        if(dip.isCategoriaProtetta()) sumOre = sumOre/2;
        QTableWidgetItem* itemOre = new QTableWidgetItem(QString::number(sumOre));
        itemOre->setTextAlignment(Qt::AlignCenter);
        shiftTable->setItem(i,8,itemOre);
        sumOre=0;
        i++;
    }
    shiftTable->setRowCount(i);
    shiftTable->blockSignals(false);
    emit this->requestEmptyAnomalies();
    emit this->checkIfEmployeeIsEmpty();
}

void shiftPanel::updateOreRiga(int row){
    double sumOre = 0.0;
    for(int col = 1; col<=7; ++col){
        QTableWidgetItem* cella = shiftTable->item(row,col);
        if(cella){
            shiftplanner::shift s = shiftplanner::shift::fromString(cella->text().toStdString());
            sumOre+=s.oreTotali();
        }
    }
    QTableWidgetItem* itemOre = shiftTable->item(row, 8);
    if(!itemOre){
        itemOre = new QTableWidgetItem();
        itemOre->setTextAlignment(Qt::AlignCenter);
        shiftTable->setItem(row,8,itemOre);
    }
    //ricavo la key dalla colonna 0 della tabella (è nella cella del nome con data di categoria Qt::UserRole)
    bool CatProtetta = false;
    QTableWidgetItem* itemKey = shiftTable->item(row,0);
    QString key = itemKey->data(Qt::UserRole).toString();
    emit this->requestCatProtettaBool(key.toStdString(), &CatProtetta);
    if(CatProtetta) sumOre = sumOre/2;

    itemOre->setText(QString::number(sumOre));
}

void shiftPanel::onShiftChange(QTableWidgetItem* item){
    if(!item) return;
    int riga = item->row();
    int colonna = item->column();
    if(colonna == 0 || colonna ==8){
        return;
    }
    QTableWidgetItem* itemNome = shiftTable->item(riga,0);
    if(!itemNome) return;
    shiftTable->blockSignals(true);
    std::string keyDip = itemNome->data(Qt::UserRole).toString().toStdString();
    int giorno = colonna-1;
    shiftplanner::shift turno = shiftplanner::shift::fromString(item->text().toStdString());
    item->setText(QString::fromStdString(turno.getTesto()));
    updateOreRiga(riga);
    emit sendNewShift(keyDip, giorno, turno);
    shiftTable->blockSignals(false);
}

//bloccare i signals di shiftpanel non causa il cambiamento del dirty bit della mainwindow, non so bene perché onestamente
void shiftPanel::markAnomalies(const std::vector<shiftplanner::anomaly>& anomalie){
    shiftTable->blockSignals(true);
    for(const auto& anomalia : anomalie){
        int giorno = anomalia.giorno + 1;
        if(giorno == 0 || giorno == 8){
            shiftTable->blockSignals(false);
            return;
        }
        for(int riga = 0; riga < shiftTable->rowCount(); ++riga){
            shiftTable->model()->setData(
                shiftTable->model()->index(riga, giorno),
                QColor("#bf0016"),
                Qt::BackgroundRole
                );
    }
    }
    shiftTable->blockSignals(false);
}

void shiftPanel::cleanCriticalDays(){
    shiftTable->blockSignals(true);
    for(int col = 1; col <= 7; ++col){
        for(int riga = 0; riga < shiftTable->rowCount(); ++riga){
            shiftTable->model()->setData(
                shiftTable->model()->index(riga, col),
                QVariant(),
                Qt::BackgroundRole
                );
        }
    }
    shiftTable->blockSignals(false);
    shiftTable->viewport()->update();
}

QDate shiftPanel::getLunediDaSettimana(int settimana, int anno){
    QDate data(anno, 1, 1);
    int giornoSettimana = data.dayOfWeek();
    if(giornoSettimana > 1){
        data = data.addDays(1-giornoSettimana);
        if(giornoSettimana>4){
            data = data.addDays(7);
        }
    }
    return data.addDays((settimana-1) * 7);
}

void shiftPanel::updateHorizontalHeaders(int settimana, int anno){
    QDate lunedi = getLunediDaSettimana(settimana, anno);
    QStringList giorni = {"Lunedì", "Martedì", "Mercoledì", "Giovedì", "Venerdì", "Sabato", "Domenica"};
    for(int i =0; i<7;++i){
        QDate giornoCorrente = lunedi.addDays(i);
        QString testoHeader = QString("%1 %2").arg(giorni[i]).arg(giornoCorrente.toString("dd/MM"));
        QTableWidgetItem* headerItem = shiftTable->horizontalHeaderItem(i+1);
        if(!headerItem){
            headerItem = new QTableWidgetItem();
            shiftTable->setHorizontalHeaderItem(i+1,headerItem);
        }
        headerItem->setText(testoHeader);
    }
}

void shiftPanel::onDipendentePressed(const QTableWidgetItem* item){
    if(!item) return;
    std::string key = item->data(Qt::UserRole).toString().toStdString();
    if(!key.empty()){
        emit this->sendDipInfo(key);
    }
}

bool shiftPanel::eventFilter(QObject* watched, QEvent* event){
    if(watched == shiftTable && event->type() == QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter){
            QModelIndex currentIndex = shiftTable->currentIndex();
            if(currentIndex.isValid()){
                QTableWidgetItem* item = shiftTable->item(currentIndex.row(), currentIndex.column());
                bool isEditable = item && (item->flags() & Qt::ItemIsEditable);
                bool isAlreadyEditing = (shiftTable->indexWidget(currentIndex)!=nullptr);
                if(isEditable && !isAlreadyEditing){
                shiftTable->edit(shiftTable->currentIndex());
                return true;
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void shiftPanel::onExportPressed(){
    std::pair<int,int> settimanaAnno = {0,0};
    emit this->requestSettimanaAnnoPair(settimanaAnno);
    std::tuple<double, double, int> infoUtili = {0,0,0};
    emit this->requestInfoUtili(infoUtili);
    QString defaultName = QString("Orario_Settimana_%1_%2.pdf").arg(settimanaAnno.first).arg(settimanaAnno.second);
    QString fileName = QFileDialog::getSaveFileName(this, "Esportazione Orario in PDF", defaultName, "Documenti PDF (*.pdf)");
    if(fileName.isEmpty()) return;
    QString testoNote = nota->text();
    bool successo = PdfExporter::esportaOrarioSettimanale(fileName, shiftTable, settimanaAnno.first, settimanaAnno.second, infoUtili, testoNote);
    if(successo){
        QMessageBox::information(this, "Esportazione avvenuta con successo", "il file PDF è stato salvato con successo!");
    } else{
        QMessageBox::critical(this, "Errore Esportazione", "Impossibile creare il file PDF, verificare i permessi della cartella");
    }
}

void shiftPanel::updateNotes(const std::string& nuovaNota){
    nota->setText(QString::fromStdString(nuovaNota));
}

} // namespace GUI
