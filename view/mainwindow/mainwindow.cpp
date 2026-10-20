#include "mainwindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include "../model/JSON/JsonWriter.h"
#include "../model/JSON/JsonReader.h"
#include <QFileDialog>
#include <QCloseEvent>
#include <QBoxLayout>
#include <QFrame>
#include <QSplitter>
#include <QTimer>
#include <QDialog>
namespace GUI{

MainWindow::MainWindow(QWidget *parent): QMainWindow{parent}, CentralContainer(new QWidget(this)), dirtyBit(false){
    mainManager.registerObserver(this);
    currentFile = shiftplanner::JSON::JsonWriter::getStandardSavePath();
    blockUpdate=true;
    shiftplanner::JSON::JsonReader::load(mainManager, currentFile);
    createActions();
    setupMenu();
    setupCentralWidget();
    updateLeftInfo();
    dirtyBit=false;
    updateWindowTitle();
    blockUpdate=false;
    checkFirstAvvio();
}

void MainWindow::checkFirstAvvio(){
    //i valori di settimana e anno di base sono settati come rispettivamente 0 e 1970, se dopo aver caricato il json i valori sono quelli o è il primo avvio o c'è un errore
    if(mainManager.getConfig().getCoppiaSettimanaAnno().first == 0 && mainManager.getConfig().getCoppiaSettimanaAnno().second == 1970){
        //di base un orario settimanale viene creato per la settimana successiva, quindi aggiungo 1 al numero
        mainManager.getConfig().setCoppiaSettimanaAnno(QDate::currentDate().weekNumber()+1, QDate::currentDate().year());
        pannelloDestro->updateShiftHorizontalHeaders(mainManager.getConfig().getCoppiaSettimanaAnno().first, mainManager.getConfig().getCoppiaSettimanaAnno().second);
        QTimer::singleShot(500, this, [this]() {
            QMessageBox::information(this, "Primo Avvio",
                                     "Sembra essere la prima volta che avvii l'applicazione, consiglio vivamente di dare una letta"
                                     " alla sezione Aiuto prima di utilizzare il programma!");
        });
        Save();
        updateLeftInfo();
        pannelloDestro->setPageHelp();
    }
}

void MainWindow::update(){
    dirtyBit = true;
    updateWindowTitle();
    if(blockUpdate){
        return;
    }
    qDebug()<<contatore++<<" - update()";
    emit(onObserverNotified());
}

void MainWindow::createActions(){
    newAction = new QAction(tr("&Nuovo"), this);
    openAction = new QAction(tr("&Apri"), this);
    saveAction = new QAction(tr("&Salva"), this);
    exportAction = new QAction(tr("&Esporta JSON"), this);
    exitAction = new QAction(tr("&Esci"), this);
    newAction->setShortcut(QKeySequence::New);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    connect(newAction, &QAction::triggered, this, &MainWindow::New);
    connect(openAction, &QAction::triggered, this, &MainWindow::Open);
    connect(saveAction, &QAction::triggered, this, &MainWindow::Save);
    connect(exportAction, &QAction::triggered, this, &MainWindow::Export);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::setupMenu(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exportAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    actionHelp = menuBar()->addAction(tr("&Aiuto"));
}

//ritorna vero se l'utente accetta di salvare, rifiuta di salvare o il file non è stato modificato, ritorna falso se l'utente cancella l'operazione
bool MainWindow::askToSave(){
    if(dirtyBit){
        QMessageBox::StandardButton risposta = QMessageBox::question(
            this, QString(tr("Modifiche non salvate")), QString(tr("Sono presenti modifiche non salvate, vuoi salvare?")), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );
        if (risposta == QMessageBox::Yes){
            Save();
            return true;
        }
        else if(risposta == QMessageBox::No){
            return true;
        }
        else if(risposta == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

void MainWindow::New(){
    bool commit = false;
    QString str("Creazione nuova tabella oraria");
    QMessageBox::StandardButton risposta = QMessageBox::question(
        this, QString(str),
        QString(tr("Sei sicuro di voler creare una nuova tabella oraria? \n(Accettando svuoti l'attuale tabella irreversibilmente)")), QMessageBox::Yes | QMessageBox::Cancel
        );
    if (risposta == QMessageBox::Yes){
        commit = true;
    }
    else if(risposta == QMessageBox::Cancel){
        commit = false;
    }
    if(!commit){
        return;
    }
    mainManager.getConfig().setIncasso(0.0);
    mainManager.getConfig().setMaxOre(0.0);
    mainManager.getConfig().setNote("");
    mainManager.getConfig().setCoppiaSettimanaAnno(QDate::currentDate().weekNumber()+1, QDate::currentDate().year());
    pannelloDestro->updateShiftHorizontalHeaders(mainManager.getConfig().getCoppiaSettimanaAnno().first, mainManager.getConfig().getCoppiaSettimanaAnno().second);
    //
    blockUpdate=true;
    mainManager.clearTabella();
    emptyAnomalies();
    blockUpdate=false;
    update();
    //
    // l'idea è quella di mantenere il file orario_settimanale.json come hook standard costante, il problema di questo approccio è che se il file nuovo
    // contiene un config diverso e poi premi nuovo per poter avviare una nuova modifica avrai il nuovo dell'hook standard con gli orari del negozio del file precedentemente aperto
    // non saprei se considerarlo un qualcosa di negativo o positivo, è da valutare in futuro se va riguardato come comportamento in base a come il programma verrà usato
    if(currentFile == shiftplanner::JSON::JsonWriter::getStandardSavePath()){
        Save();
    }
    else{
        currentFile = shiftplanner::JSON::JsonWriter::getStandardSavePath();
        Save();
    }
    dirtyBit = false;
    updateWindowTitle();
}

void MainWindow::Save(){
    shiftplanner::JSON::JsonWriter::save(mainManager, currentFile);
    dirtyBit = false;
    updateWindowTitle();
}

void MainWindow::Open(){
    QString file = QFileDialog::getOpenFileName(this,tr("Seleziona JSON"), currentFile,tr("JSON (*.json);;Tutti i file (*)"));
    if(file.isEmpty())
        return;
    bool commit = askToSave();
    if(commit){
        blockUpdate=true;
        mainManager.clear();
        shiftplanner::JSON::JsonReader::load(mainManager, file);
        blockUpdate=false;
        update();
    }
    currentFile = file;
    dirtyBit = false;
    updateWindowTitle();
}

void MainWindow::Export(){
    QString pathutente = QFileDialog::getSaveFileName(this, tr("Esportazione orari"), QDir::homePath(), tr("JSON (*.json)"));
    if(!pathutente.isEmpty()){
        shiftplanner::JSON::JsonWriter::save(mainManager, pathutente);
        QMessageBox::information(this, tr("Esportazione completata"), tr("Esportazione della tabella oraria avvenuta con successo"));
    }
}

void MainWindow::updateWindowTitle(){
    QFileInfo fileInfo(currentFile);
    QString nomeFile = fileInfo.fileName();
    if(nomeFile.isEmpty()){
        nomeFile = "Senza Titolo";
    }
    setWindowTitle("OrarioPlanner - " + nomeFile + "[*]");
    setWindowFilePath(currentFile);
    setWindowModified(dirtyBit);
}

void MainWindow::setupCentralWidget(){
    QColor colorLinea = palette().color(QPalette::Mid);
    coloreHex = colorLinea.name();
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(CentralContainer);
    layoutPrincipale->setContentsMargins(0,0,0,0);
    layoutPrincipale->setSpacing(0);
    this->setCentralWidget(CentralContainer);
    QFrame* lineaH = new QFrame(CentralContainer);
    lineaH->setFrameShape(QFrame::HLine);
    lineaH->setFrameShadow(QFrame::Plain);
    lineaH->setFixedHeight(1);
    lineaH->setStyleSheet(QString("background-color:%1; border:none;").arg(coloreHex));
    layoutPrincipale->addWidget(lineaH);
    QSplitter* splitterH = new QSplitter(Qt::Horizontal, CentralContainer);
    splitterH->setStyleSheet(QString("QSplitter::handle {background-color: %1}").arg(coloreHex));
    splitterH->setHandleWidth(1);
    layoutPrincipale->addWidget(splitterH);
    pannelloDestro = new rightpanel(mainManager,splitterH);
    pannelloSinistro = new leftpanel(splitterH);
    splitterH->addWidget(pannelloSinistro);
    splitterH->addWidget(pannelloDestro);
    setupRightPanel();
    setupLeftPanel();
    //segnali e slot
    connect(this, &MainWindow::onObserverNotified, this, &MainWindow::updateLeftInfo);
    //set dello stretch dello splitter
    splitterH->setStretchFactor(0, 1);
    splitterH->setCollapsible(0, false);
    splitterH->setStretchFactor(1, 3);
    splitterH->setCollapsible(1, false);
}

void MainWindow::setupRightPanel(){
    connect(pannelloDestro, &rightpanel::sendNewInfo, this, &MainWindow::updateLeftPanel);
    connect(this, &MainWindow::onObserverNotified, pannelloDestro, &rightpanel::refreshPages);
    connect(pannelloDestro, &rightpanel::sendNuovoDipendente, this, &MainWindow::acceptNewDipendente);
    connect(pannelloDestro, &rightpanel::sendDipendenteToDelete, this, &MainWindow::deleteDipendente);
    connect(pannelloDestro, &rightpanel::sendDeleteFromEdit, this, &MainWindow::deleteFromEdit);
    connect(pannelloDestro, &rightpanel::sendModificaDipAttivo, this, &MainWindow::updateActiveStateOfDipendente);
    connect(pannelloDestro, &rightpanel::requestSave, this, [this](){
        if(dirtyBit)
            this->Save();
    });
    connect(pannelloDestro, &rightpanel::sendNewShift, this, &MainWindow::acceptNewShift);
    connect(this, &MainWindow::sendAnomalies, pannelloDestro, &rightpanel::sendAnomalies);
    connect(this, &MainWindow::cleanCriticalDays, pannelloDestro, &rightpanel::cleanCriticalDays);
    connect(actionHelp, &QAction::triggered, pannelloDestro, &rightpanel::setPageHelp);
    connect(pannelloDestro, &rightpanel::sendDipInfo, this, &MainWindow::onDipInfoRequest);
    connect(pannelloDestro, &rightpanel::onPageOrariNotFocused, this, [this](){
        this->pannelloSinistro->clearEmployeeDetails();
        this->pannelloSinistro->setIntegrityCheckEnabled(false);
    });
    connect(pannelloDestro, &rightpanel::onPageOrariFocused, this, [this](){
        this->pannelloSinistro->setIntegrityCheckEnabled(true);
    });
    connect(pannelloDestro, &rightpanel::sendUpdatedNotes, this, [this](std::string nuovaNota){
        this->dirtyBit = true;
        updateWindowTitle();
        mainManager.getConfig().setNote(nuovaNota);
    });
    connect(pannelloDestro, &rightpanel::requestEmptyAnomalies, this, &MainWindow::emptyAnomalies);
}

void MainWindow::setupLeftPanel(){
    connect(pannelloSinistro, &leftpanel::onConfigPressed, pannelloDestro, &rightpanel::setPageConfig);
    connect(pannelloSinistro, &leftpanel::onDipendentiPressed, pannelloDestro, &rightpanel::setPageEmployee);
    connect(pannelloSinistro, &leftpanel::onCheckRulesPressed, this, &MainWindow::checkFailsafeRules);
    connect(pannelloSinistro, &leftpanel::cleanCriticalDay, pannelloDestro, &rightpanel::cleanCriticalDays);
    connect(this, &MainWindow::sendAnomalies, pannelloSinistro, &leftpanel::acceptAnomalies);
    connect(pannelloSinistro, &leftpanel::requestClearAnomalies, this, [this](){
        anomalie.clear();
    });
    connect(this, &MainWindow::sendInfoDip, pannelloSinistro, &leftpanel::sendDipInfo);
    connect(pannelloSinistro, &leftpanel::requestOreImpiegate, this, [this](const std::string& key, double& oreImpiegate){
        oreImpiegate = mainManager.calcolaOreDipendente(key);
    });
    connect(this, &MainWindow::requestFillEmployeeDetail, pannelloSinistro, &leftpanel::requestFillEmployeeDetail);
}

void MainWindow::checkFailsafeRules(){
    int i;
    anomalie.clear();
    for(i = 0; i < 7; ++i){
        std::vector<shiftplanner::anomaly> anomalieGiornata = mainManager.checkRegoleFailsafe(i);
        if(!anomalieGiornata.empty()){
            anomalie.insert(anomalie.end(), anomalieGiornata.begin(), anomalieGiornata.end());
        }
    };
    emit this->cleanCriticalDays();
    emit this->sendAnomalies(anomalie);
}

void MainWindow::updateLeftPanel(shiftplanner::shopConfig& newConfig, int incasso, double maxOre, int settimana){
    mainManager.setConfig(newConfig);
    mainManager.getConfig().setMaxOre(maxOre);
    mainManager.getConfig().setIncasso(incasso);
    mainManager.getConfig().setCoppiaSettimanaAnno(settimana, mainManager.getConfig().getCoppiaSettimanaAnno().second);
    this->updateLeftInfo();
    Save();
    QMessageBox::information(this, "Modifiche salvate", "Modifiche salvate con successo!");
    pannelloDestro->setPageShiftTable();
}

void MainWindow::closeEvent(QCloseEvent* event){
    bool commit = askToSave();
    if(commit){
        event->accept();
    } else{
        event->ignore();
    }
}

void MainWindow::acceptNewDipendente(shiftplanner::employee* nuovoDip){
    if(nuovoDip){
        mainManager.aggiungiDipendente(*nuovoDip);
        Save();
    }
}

void MainWindow::deleteDipendente(const shiftplanner::employee* target){
    if(target){
        mainManager.rimuoviDipendente(target->getKey());
        Save();
    }
}

void MainWindow::deleteFromEdit(const shiftplanner::employee* target){
    if(target){
        mainManager.rimuoviDipendente(target->getKey());
    }
}

void MainWindow::updateActiveStateOfDipendente(const std::string& key, bool state){
    for(size_t i=0; i<mainManager.getDipendentiAttiviSize(); ++i){
        auto dip = mainManager.getDipendente(i);
        if(key == dip->getKey()){
            dip->setAttivo(state);
            update();
            dirtyBit = true;
            break;
        }
    }
}

void MainWindow::acceptNewShift(const std::string& key, int giorno, const shiftplanner::shift& turno){
    blockUpdate = true;
    double oreTotali = mainManager.calcolaOreTotali();
    mainManager.impostaTurno(key, giorno, turno);
    //ha senso promptare l'utente ad effettuare nuovamente la verifica solo se le ore cambiano
    if(oreTotali != mainManager.calcolaOreTotali()){
        pannelloSinistro->refreshIntegrityCheck();
    }
    updateLeftInfo();
    emit this->requestFillEmployeeDetail();
    blockUpdate = false;
}

void MainWindow::updateLeftInfo(){
    const auto& config = mainManager.getConfig();
    double oreTotali = mainManager.calcolaOreTotali();
    pannelloSinistro->updateView(config.getIncasso(), config.getMaxOre(), oreTotali, config.getCoppiaSettimanaAnno().first, config.getCoppiaSettimanaAnno().second);
    pannelloDestro->updateShiftHorizontalHeaders(config.getCoppiaSettimanaAnno().first, config.getCoppiaSettimanaAnno().second);
    if(mainManager.countActiveEmployees()==0){
        pannelloSinistro->setIntegrityCheckEnabled(false);
    }
}

void MainWindow::onDipInfoRequest(const std::string key){
    const shiftplanner::employee* dip = mainManager.getDipendente(key);
    if(dip){
        emit this->sendInfoDip(dip);
    }
}

void MainWindow::emptyAnomalies(){
    anomalie.clear();
    this->sendAnomalies(anomalie);
    pannelloSinistro->refreshIntegrityCheck();
}

void MainWindow::clearAllViews(){

}
}

