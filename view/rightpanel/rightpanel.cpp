#include "rightpanel.h"
#include <QBoxLayout>
#include <QLabel>

namespace GUI{
rightpanel::rightpanel(const shiftplanner::shiftManager& gestore,QWidget* parent): QWidget(parent), manager(gestore) {
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    stackDestro = new QStackedWidget(this);
    //page 0 - tabella orari
    pageOrari = new shiftPanel(manager.getGrid(),manager.getListaDip(),stackDestro);
    stackDestro->addWidget(pageOrari);
    //page 1 - schermata dipendenti
    pageDipendenti = new employeePanel(stackDestro);
    fillEmployeePanel();
    stackDestro->addWidget(pageDipendenti);
    //page 2 - schermata impostazioni
    pageImpostazioni = new settingsPanel(manager.getConfig(),stackDestro);
    stackDestro->addWidget(pageImpostazioni);
    //page 3 - schermata aiuto
    pageAiuto = new helpPanel(stackDestro);
    stackDestro->addWidget(pageAiuto);
    //page 4 - schermata dipendenti vuoti
    pageNoDipendenti = new noEmployeePanel(stackDestro);
    stackDestro->addWidget(pageNoDipendenti);

    layoutPrincipale->addWidget(stackDestro);

    //settingsPanel
    connect(pageImpostazioni, &settingsPanel::onBackPressed, [this](){
        this->setPageShiftTable();
    });
    connect(pageImpostazioni, &settingsPanel::onSavePressed, this, &rightpanel::sendNewInfo);
    //employeePanel
    connect(pageDipendenti, &employeePanel::sendNuovoDipendenteFromWidget, this, &rightpanel::sendNuovoDipendente);
    connect(pageDipendenti, &employeePanel::onBackPressed, [this](){
        this->setPageShiftTable();
    });
    connect(pageDipendenti, &employeePanel::onDeleteFromEdit, this, &rightpanel::sendDeleteFromEdit);
    connect(pageDipendenti, &employeePanel::onDeletePressed, this, &rightpanel::sendDipendenteToDelete);
    connect(pageDipendenti, &employeePanel::sendModificaDipAttivo, this, &rightpanel::sendModificaDipAttivo);
    connect(pageDipendenti, &employeePanel::requestSave, this, &rightpanel::requestSave);
    //shiftPanel
    connect(pageOrari, &shiftPanel::sendNewShift, this, &rightpanel::sendNewShift);
    connect(pageOrari, &shiftPanel::requestAttivoBool, this, [this](const std::string& key,bool* value){
        if(!value) return;
        auto dip = manager.getDipendente(key);
        if(dip){
            *value = dip->isAttivo();
        }
    });
    connect(pageOrari, &shiftPanel::requestCatProtettaBool, this, [this](const std::string& key,bool* value){
        if(!value) return;
        auto dip = manager.getDipendente(key);
        if(dip){
            *value = dip->isCategoriaProtetta();
        }
    });
    connect(this, &rightpanel::sendAnomalies, pageOrari, &shiftPanel::markAnomalies);
    connect(this, &rightpanel::cleanCriticalDays, pageOrari, &shiftPanel::cleanCriticalDays);
    connect(pageOrari, &shiftPanel::sendDipInfo, this, &rightpanel::sendDipInfo);
    connect(pageOrari, &shiftPanel::requestSettimanaAnnoPair, this, [this](std::pair<int,int>& settimanaAnno){
        settimanaAnno = manager.getConfig().getCoppiaSettimanaAnno();
    });
    connect(pageOrari, &shiftPanel::requestInfoUtili, this, [this](std::tuple<double,double,int>& infoUtili){
        const shiftplanner::shopConfig& config = this->manager.getConfig();
        double redd = 0;
        config.getMaxOre()>0? redd=config.getIncasso()/config.getMaxOre():redd=0;
        infoUtili = {redd,manager.calcolaOreTotali(), config.getIncasso()};
    });
    connect(pageOrari, &shiftPanel::sendUpdatedNotes, this, &rightpanel::sendUpdatedNotes);
    connect(pageOrari, &shiftPanel::requestEmptyAnomalies, this, &rightpanel::requestEmptyAnomalies);
    //helpPanel
    connect(pageAiuto, &helpPanel::onBackPressed, this, &rightpanel::setPageShiftTable);
    connect(this, &rightpanel::onPageOrariNotFocused, pageOrari, &shiftPanel::clearEmployeeSelection);
    //
    refreshPages();
    setPageShiftTable();
}

void rightpanel::refreshPages(){
    pageImpostazioni->setConfig(manager.getConfig());
    pageImpostazioni->refreshAll();
    fillEmployeePanel();
    pageOrari->refreshTable();
    pageOrari->updateHorizontalHeaders(manager.getConfig().getCoppiaSettimanaAnno().first, manager.getConfig().getCoppiaSettimanaAnno().second);
    pageOrari->updateNotes(manager.getConfig().getNote());
}

void rightpanel::setPageShiftTable(){
    //semplice flip flop condizionale se si arriva ad avere la tabella vuota
    if(manager.countActiveEmployees()!=0){
        stackDestro->setCurrentIndex(0);
        emit this->onPageOrariFocused();
    }
    else{
        stackDestro->setCurrentIndex(4);
        emit this->onPageOrariNotFocused();
    }
}
void rightpanel::setPageEmployee(){
    stackDestro->setCurrentIndex(1);
    pageDipendenti->setFocus();
    emit this->onPageOrariNotFocused();
}
void rightpanel::setPageConfig(){
    pageImpostazioni->setConfig(manager.getConfig());
    pageImpostazioni->refreshTable();
    stackDestro->setCurrentIndex(2);
    pageImpostazioni->setFocus();
    emit this->onPageOrariNotFocused();
}
void rightpanel::setPageHelp(){
    stackDestro->setCurrentIndex(3);
    emit this->onPageOrariNotFocused();
}

// void rightpanel::setPageNoEmployees(){
//     if(manager.countActiveEmployees()==0 && stackDestro->currentIndex()==0){
//     stackDestro->setCurrentIndex(4);
//     emit this->onPageOrariNotFocused();
//     }
//     else setPageShiftTable();
// }

void rightpanel::fillEmployeePanel(){
    if(pageDipendenti){
        pageDipendenti->clearLista();
        for(size_t i = 0; i<manager.getDipendentiAttiviSize(); ++i){
            pageDipendenti->addDip(manager.getDipendente(i));
        }
        pageDipendenti->updateStats();
    }
}

void rightpanel::updateShiftHorizontalHeaders(int settimana, int anno){
    pageOrari->updateHorizontalHeaders(settimana,anno);
}

}