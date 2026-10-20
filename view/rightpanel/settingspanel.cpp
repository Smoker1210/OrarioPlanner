#include "settingspanel.h"
#include <QBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>
#include <QTimeEdit>
#include <QKeyEvent>
#include <QFormLayout>
namespace GUI{
settingsPanel::settingsPanel(const shiftplanner::shopConfig& ConfigNegozio, QWidget *parent): QWidget{parent}, newConfig(ConfigNegozio){
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    QLabel* Titolo = new QLabel("Impostazioni",this);
    QFont fontTitolo = Titolo->font();
    fontTitolo.setBold(1);
    fontTitolo.setPointSize(24);
    Titolo->setFont(fontTitolo);
    Titolo->setAlignment(Qt::AlignCenter);
    layoutPrincipale->addWidget(Titolo);
    //parte alta
    QLabel* sezioneOrari = new QLabel("Orari Punto Vendita", this);
    QFont fontSottotitolo = sezioneOrari->font();
    fontSottotitolo.setBold(1);
    fontSottotitolo.setPointSize(16);
    sezioneOrari->setFont(fontSottotitolo);
    QVBoxLayout* layoutTabellaOrari = new QVBoxLayout();
    layoutTabellaOrari->addWidget(sezioneOrari);
    setupTable();
    layoutTabellaOrari->addWidget(tabellaOrari);
    tabellaOrari->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layoutPrincipale->addLayout(layoutTabellaOrari);
    //parte centrale
    QHBoxLayout* MiddleRow = new QHBoxLayout();
    //widget sinistro
    QVBoxLayout* layoutValoriUtili = new QVBoxLayout();
    QLabel* sezioneValoriUtili = new QLabel("Valori utili", this);
    sezioneValoriUtili->setFont(fontSottotitolo);
    layoutValoriUtili->addWidget(sezioneValoriUtili);
    layoutValoriUtili->addSpacing(10);
    QFormLayout* sezioneInfo = new QFormLayout();
    //redditivita
    redditivita = new QLabel("0", this);
    sezioneInfo->addRow("Redditività: ", redditivita);
    //incasso
    editIncasso = new QSpinBox(this);
    editIncasso->setSingleStep(10000);
    editIncasso->setMinimum(0);
    editIncasso->setMaximum(200000);
    sezioneInfo->addRow("Incasso previsto: ", editIncasso);
    //maxore
    editOreMax = new QDoubleSpinBox(this);
    editOreMax->setDecimals(2);
    editOreMax->setMinimum(0);
    editOreMax->setMaximum(500);
    sezioneInfo->addRow("Ore suggerite: ", editOreMax);
    //settimana
    editSettimana = new QSpinBox(this);
    editSettimana->setMinimum(0);
    editSettimana->setMaximum(53);
    editSettimana->setToolTip("In genere non serve modificare la settimana ma lascio la possibilità di farlo");
    sezioneInfo->addRow("Numero Settimana: ", editSettimana);
    layoutValoriUtili->addLayout(sezioneInfo);
    MiddleRow->addLayout(layoutValoriUtili);
    //
    MiddleRow->addStretch();
    layoutPrincipale->addLayout(MiddleRow);
    layoutPrincipale->addStretch();
    //bottom row
    QHBoxLayout* bottomRow = new QHBoxLayout();
    saveButton = new QPushButton("Salva", this);
    bottomRow->addWidget(saveButton);
    bottomRow->addStretch();
    backButton = new QPushButton("Indietro", this);
    bottomRow->addWidget(backButton);
    layoutPrincipale->addLayout(bottomRow);
    setFocusPolicy(Qt::StrongFocus);

    //connect varie
    connect(editIncasso, qOverload<int>(&QSpinBox::valueChanged),this, &settingsPanel::updateRedditivita);
    connect(editOreMax, qOverload<double>(&QDoubleSpinBox::valueChanged),this, &settingsPanel::updateRedditivita);
    connect(backButton, &QPushButton::clicked, this, &settingsPanel::onBackPressed);
    connect(saveButton, &QPushButton::clicked, this, &settingsPanel::saveAll);
}

void settingsPanel::updateRedditivita(){
    if(editOreMax && editOreMax->value()!=0){
        redditivita->setText(QString::number(editIncasso->value()/editOreMax->value(), 'f', 1));
    }
}

void settingsPanel::saveAll(){
    for(size_t i = 0 ; i<7; i++){
        QWidget* cella = tabellaOrari->cellWidget(i,1);
        QCheckBox* aperto = cella ? cella->findChild<QCheckBox*>() : nullptr;
        bool isAperto = aperto ? aperto->isChecked() : false;

        QTimeEdit* orarioApertura = qobject_cast<QTimeEdit*>(tabellaOrari->cellWidget(i,2));
        unsigned int apertura = 0;
        if(orarioApertura){
            QTime t = orarioApertura->time();
            apertura = (t.hour()*60) + t.minute();
        }

        QTimeEdit* orarioChiusura = qobject_cast<QTimeEdit*>(tabellaOrari->cellWidget(i,3));
        unsigned int chiusura = 0;
        if(orarioApertura){
            QTime t = orarioChiusura->time();
            chiusura = (t.hour()*60) + t.minute();
        }

        newConfig.setOrarioGiorno(i, shiftplanner::shopConfig::OrarioGiorno(isAperto, apertura, chiusura));
    }
    emit onSavePressed(newConfig, editIncasso->value(), editOreMax->value(), editSettimana->value());
}

void settingsPanel::keyPressEvent(QKeyEvent* event){
    if(event->key() ==  Qt::Key_Escape){
        emit onBackPressed();
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}

void settingsPanel::setupTable(){
    tabellaOrari = new QTableWidget(7,4,this);
    QStringList headers = {"Giorno", "Aperto", "Apertura", "Chiusura"};
    //size policy junk
    tabellaOrari->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabellaOrari->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabellaOrari->setHorizontalHeaderLabels(headers);
    tabellaOrari->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabellaOrari->verticalHeader()->setVisible(false);
    tabellaOrari->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //
    QStringList giorni = {"Lunedì", "Martedì", "Mercoledì", "Giovedì", "Venerdì", "Sabato", "Domenica"};
    for(size_t i=0; i <7; ++i){
        QTableWidgetItem* itemGiorno = new QTableWidgetItem(giorni[i]);
        itemGiorno->setFlags(itemGiorno->flags() & ~Qt::ItemIsEditable);
        itemGiorno->setFlags(Qt::ItemIsEnabled);
        itemGiorno->setBackground(QBrush(Qt::transparent));
        tabellaOrari->setItem(i,0,itemGiorno);
        auto infoGiorno = newConfig.getOrarioGiorno(i);

        // 2. CheckBox Aperto/Chiuso
        QCheckBox* checkAperto = new QCheckBox();
        checkAperto->setChecked(infoGiorno.aperto);
        QWidget* checkWidget = new QWidget();
        QHBoxLayout* checkLayout = new QHBoxLayout(checkWidget);
        checkLayout->addWidget(checkAperto);
        checkLayout->setAlignment(Qt::AlignCenter);
        checkLayout->setContentsMargins(0, 0, 0, 0);
        tabellaOrari->setCellWidget(i, 1, checkWidget);

        // 3. QTimeEdit Apertura
        int ore = infoGiorno.aperturaMinuti / 60;
        int minuti = infoGiorno.aperturaMinuti % 60;
        QTimeEdit* timeApertura = new QTimeEdit(QTime(ore, minuti));
        timeApertura->setDisplayFormat("HH:mm");
        tabellaOrari->setCellWidget(i, 2, timeApertura);

        // 4. QTimeEdit Chiusura
        ore = infoGiorno.chiusuraMinuti / 60;
        minuti = infoGiorno.chiusuraMinuti % 60;
        QTimeEdit* timeChiusura = new QTimeEdit(QTime(ore, minuti));
        timeChiusura->setDisplayFormat("HH:mm");
        tabellaOrari->setCellWidget(i, 3, timeChiusura);

        // Disabilita/Abilita i campi orario in base al CheckBox
        QObject::connect(checkAperto, &QCheckBox::toggled, [timeApertura, timeChiusura](bool checked) {
            timeApertura->setEnabled(checked);
            timeChiusura->setEnabled(checked);
        });
        timeApertura->setEnabled(infoGiorno.aperto);
        timeChiusura->setEnabled(infoGiorno.aperto);
    }
    //size policy junk
    tabellaOrari->adjustSize();
    int altezzaHeader = tabellaOrari->horizontalHeader()->height();
    int altezzaRighe = 0;
    for (int i =0; i<7; ++i){
        altezzaRighe += tabellaOrari->rowHeight(i);
    }
    tabellaOrari->setFixedHeight(altezzaHeader + altezzaRighe + 6);
    //
    }

    void settingsPanel::refreshTable(){
        for(size_t i = 0; i<7; ++i){
            const shiftplanner::shopConfig::OrarioGiorno& orario = newConfig.getOrarioGiorno(i);
            QWidget* cella = tabellaOrari->cellWidget(i,1);
            QCheckBox* aperto = cella ? cella->findChild<QCheckBox*>() : nullptr;
            if(aperto){
            aperto->setChecked(orario.aperto);
            }
            int ore = 0;
            int minuti = 0;
            QTimeEdit* orarioApertura = qobject_cast<QTimeEdit*>(tabellaOrari->cellWidget(i,2));
            if(orarioApertura){
                ore = orario.aperturaMinuti / 60;
                minuti = orario.aperturaMinuti % 60;
                orarioApertura->setTime(QTime(ore, minuti));
            }

            QTimeEdit* orarioChiusura = qobject_cast<QTimeEdit*>(tabellaOrari->cellWidget(i,3));
            if(orarioApertura){
                ore = orario.chiusuraMinuti / 60;
                minuti = orario.chiusuraMinuti % 60;
                orarioChiusura->setTime(QTime(ore, minuti));
            }
        }
    }

    void settingsPanel::setConfig(const shiftplanner::shopConfig& newConfigNegozio){
        newConfig = newConfigNegozio;
    }

    void settingsPanel::refreshInfo(){
        if(newConfig.getMaxOre()!=0){
        redditivita->setText(QString::number(newConfig.getIncasso()/newConfig.getMaxOre(), 'f', 1));
        }
        editIncasso->setValue(newConfig.getIncasso());
        editOreMax->setValue(newConfig.getMaxOre());
        editSettimana->setValue(newConfig.getCoppiaSettimanaAnno().first);
    }
    void settingsPanel::refreshAll(){
        refreshTable();
        refreshInfo();
    }

}
