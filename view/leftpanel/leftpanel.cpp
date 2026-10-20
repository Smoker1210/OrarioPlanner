#include "leftpanel.h"
#include "utility/utility.h"
#include <QLabel>
#include <QBoxLayout>
#include <QFormLayout>
#include <QDate>
#include <QLocale>
#include <QTextBrowser>
namespace GUI{
leftpanel::leftpanel(QWidget *parent): QWidget{parent}{
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    layoutPrincipale->setContentsMargins(0,10,0,10);
    QLocale localeIt(QLocale::Italian, QLocale::Italy);
    QColor colorLinea = palette().color(QPalette::Mid);
    QString coloreHex = colorLinea.name();
    QDate oggi = QDate::currentDate();
    QLabel* data = new QLabel(localeIt.toString(oggi, "dddd d MMMM yyyy"), this);
    QHBoxLayout* sublayout = new QHBoxLayout();
    //font sottotitolo
    QFont fontSottotitolo = data->font();
    fontSottotitolo.setBold(1);
    fontSottotitolo.setPointSize(16);
    //
    sublayout->addStretch();
    sublayout->addWidget(data);
    sublayout->addStretch();
    layoutPrincipale->addLayout(sublayout);
    layoutPrincipale->addWidget(creaLineaH(coloreHex, 1, this));
    info = new QWidget();
    setupInfo();
    layoutPrincipale->addWidget(info);
    layoutPrincipale->addWidget(creaLineaH(coloreHex, 1, this));
    //
    dettaglioDip = new employeeDetailWidget(this);
    layoutPrincipale->addWidget(dettaglioDip);
    layoutPrincipale->addWidget(creaLineaH(coloreHex, 1, this));
    //
    sublayout = new QHBoxLayout();
    tastoCheckRules = new QPushButton("Controlla integrità tabella", this);
    sublayout->addSpacing(10);
    sublayout->addWidget(tastoCheckRules);
    sublayout->addSpacing(10);
    layoutPrincipale->addLayout(sublayout);
    //
    QHBoxLayout* sublayout2 = new QHBoxLayout();
    tastoCleanTable = new QPushButton("Resetta Stato", this);
    tastoCleanTable->setToolTip("Annulla la precedente verifica");
    tastoCleanTable->setDisabled(true);
    sublayout2->addSpacing(10);
    sublayout2->addWidget(tastoCleanTable);
    sublayout2->addSpacing(10);
    layoutPrincipale->addLayout(sublayout2);
    QFormLayout* integrityCheck = new QFormLayout();
    integrityCheck->setContentsMargins(10,0,10,0);
    status = new QLabel("In attesa di verifica...", this);
    status->setStyleSheet(QString("color: %1;").arg(hexAttesaVerifica));
    integrityCheck->addRow("Stato tabella: ", status);
    casellaErrori = new QTextBrowser();
    casellaErrori->setFrameShape(QAbstractScrollArea::Shape::NoFrame);
    casellaErrori->setFrameShadow(QAbstractScrollArea::Shadow::Plain);
    casellaErrori->setStyleSheet("background-color:transparent;");
    layoutPrincipale->addLayout(integrityCheck);
    layoutPrincipale->addWidget(casellaErrori);

    layoutPrincipale->addStretch();
    layoutPrincipale->addWidget(creaLineaH(coloreHex, 1, this));
    QHBoxLayout* layoutRow = new QHBoxLayout();
    layoutRow->setContentsMargins(5,0,5,0);
    setupBottomRow(layoutRow);
    layoutPrincipale->addLayout(layoutRow);
    //connect
    connect(tastoCheckRules, &QPushButton::clicked, this, [this](){
        emit this->onCheckRulesPressed(errori);
        if(!casellaErrori->toPlainText().isEmpty()){
            tastoCleanTable->setEnabled(true);
            isTableStateResettable=true;
        }
    });
    connect(casellaErrori, &QTextBrowser::textChanged, this, [this](){
        if(!this->casellaErrori->toPlainText().isEmpty()){
            status->setText("Errore integrità");
            status->setStyleSheet(QString("color: %1;").arg(hexErrore));
        }
        else{
            status->setText("Regolare");
            status->setStyleSheet(QString("color: %1;").arg(hexRegolare));
            emit this->cleanCriticalDay();
        }
    });
    connect(tastoCleanTable, &QPushButton::clicked, this, [this](){
        isTableStateResettable=false;
        emit this->cleanCriticalDay();
        emit this->requestClearAnomalies();
        casellaErrori->setText("");
        refreshIntegrityCheck();
        tastoCleanTable->setDisabled(true);
    });
    connect(this, &leftpanel::sendDipInfo, dettaglioDip, &employeeDetailWidget::acceptInfoDip);
    connect(dettaglioDip, &employeeDetailWidget::requestOreImpiegate, this, &leftpanel::requestOreImpiegate);
    connect(this, &leftpanel::requestFillEmployeeDetail, dettaglioDip, &employeeDetailWidget::fillLabels);
    connect(this, &leftpanel::clearEmployeeDetails, dettaglioDip, &employeeDetailWidget::clearSelection);
}

void leftpanel::setIntegrityCheckEnabled(bool value){
    tastoCheckRules->setEnabled(value);
    if(isTableStateResettable)
        tastoCleanTable->setEnabled(value);
}

QFrame* leftpanel::creaLineaH(const QString& colore, int spessore, QWidget* parent) {
    QFrame* linea = new QFrame(parent);
    linea->setFrameShape(QFrame::HLine);
    linea->setFrameShadow(QFrame::Plain);
    linea->setFixedHeight(spessore);

    // Configura lo stile via QSS usando i parametri passati
    QString style = QString("background-color: %1; border: none;")
                        .arg(colore);
    linea->setStyleSheet(style);

    return linea;
}

void leftpanel::setupInfo(){
    QVBoxLayout* layoutInfo = new QVBoxLayout(info);
    QFormLayout* lista = new QFormLayout();
    anno = new QLabel("1970", info);
    settimana = new QLabel("0", info);
    incassoPrevisto = new QLabel("0", info);
    redditivita = new QLabel("0", info);
    oreMassime = new QLabel("0", info);
    oreUsate = new QLabel("0", info);
    lista->addRow("Anno: ", anno);
    lista->addRow("Settimana: ", settimana);
    lista->addRow("Redditività: ", redditivita);
    lista->addRow("Incasso: ", incassoPrevisto);
    lista->addRow("Ore suggerite: ", oreMassime);
    lista->addRow("Ore impiegate: ", oreUsate);
    layoutInfo->addLayout(lista);
    //aggiungere altro idealmente (non so cosa ancora)
}

void leftpanel::setupBottomRow(QHBoxLayout* layout){
    tastoConfig = new QPushButton("Impostazioni");
    tastoDipendenti = new QPushButton("Dipendenti");
    layout->addWidget(tastoDipendenti);
    layout->addStretch();
    layout->addWidget(tastoConfig);
    connect(tastoConfig, &QPushButton::clicked, this, &leftpanel::onConfigPressed);
    connect(tastoDipendenti, &QPushButton::clicked, this, &leftpanel::onDipendentiPressed);
}

void leftpanel::updateView(int incasso, double oreMax, double oreImpiegate, int settimanaNuova, int annoNuovo){
    anno->setText(QString::number(annoNuovo));
    settimana->setText(QString::number(settimanaNuova));
    incassoPrevisto->setText(QString::number(incasso));
    oreMassime->setText(QString::number(oreMax, 'f', 2));
    oreUsate->setText(QString::number(oreImpiegate, 'f', 2));
    double redd = 0;
    if(oreMax>0){
    redd = incasso / oreMax;
    }
    redditivita->setText(QString::number(redd, 'f', 2));
}

//eventualmente sarebbe da usare il tipo dell'errore
QString leftpanel::buildDayView(int giorno, const std::vector<shiftplanner::anomaly>& anomalie){
    QString day = QString::fromStdString("<b>" +shiftplanner::utility::intToGiorno(giorno)+ "</b>" + "<br>");
    int contatore = 0;
    for(const auto& anomalia : anomalie){
        if(anomalia.giorno == giorno){
            day.append(QString::fromStdString(anomalia.descrizione +", Ora: " + shiftplanner::utility::minutiToOraString(anomalia.minutoInizio) + "<br>"));
            contatore++;
        }
    }
    if(contatore <=0){
        day = "";
    }
    return day;
}

void leftpanel::acceptAnomalies(const std::vector<shiftplanner::anomaly>& anomalie){
    if(!anomalie.empty()){
        QString fullLine = "";
        for(int i = 0; i<7; ++i){
            fullLine+=buildDayView(i, anomalie);
        }
        casellaErrori->setText(fullLine);
    }
    else{
        casellaErrori->setText("");
    }
}

void leftpanel::checkErrorList(){
    if(!errori.empty()){
        QString fullLine;
        for(const auto& issue : errori){
            fullLine += QString::fromStdString(issue);
            fullLine += "<br>";
        }
        casellaErrori->setText(fullLine);
    }
    else{
        casellaErrori->setText("");
    }
}

void leftpanel::refreshIntegrityCheck(){
    status->setText("In attesa di verifica...");
    status->setStyleSheet(QString("color: %1;").arg(hexAttesaVerifica));
}
}