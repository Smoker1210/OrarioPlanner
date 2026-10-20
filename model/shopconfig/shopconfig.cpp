#include "shopconfig.h"

namespace shiftplanner{
    bool shopConfig::OrarioGiorno::checkIntervallo(unsigned int inizio, unsigned int fine) const{
        if (!aperto) return false;
        return inizio >= aperturaMinuti && fine <= chiusuraMinuti;
    }

    shopConfig::shopConfig(){
        for(int i = 0; i < 6; i++){
            orariNegozio[i] = {true,510,1230};
        }
        orariNegozio[6] = {true, 510,1200};
        coppiaSettimanaAnno={0,1970};
    }
    shopConfig::OrarioGiorno::OrarioGiorno(): aperto(false), aperturaMinuti(0), chiusuraMinuti(0){}
    shopConfig::OrarioGiorno::OrarioGiorno(bool open,unsigned int start, unsigned int end){
        aperto = open;
        aperturaMinuti = start;
        chiusuraMinuti = end;
    }
    const shopConfig::OrarioGiorno& shopConfig::getOrarioGiorno(size_t giorno) const{
        return orariNegozio[giorno];
    }
    void shopConfig::setOrarioGiorno(size_t giorno, const shopConfig::OrarioGiorno& orario){
        if (giorno > 6) return;
        else{
            orariNegozio[giorno] = orario;
        }
    }
    int shopConfig::getIncasso() const{
        return incasso;
    }
    void shopConfig::setIncasso(int nuovo){
        if (nuovo >= 0)
            incasso = nuovo;
    }
    double shopConfig::getMaxOre() const{
        return maxOre;
    }
    void shopConfig::setMaxOre(double nuovo){
        if (nuovo >= 0)
            maxOre = nuovo;
    }
    void shopConfig::setCoppiaSettimanaAnno(int settimana, int anno){
        if(settimana >= 0 && anno>=1970)
        coppiaSettimanaAnno = {settimana, anno};
    }
    std::pair<int, int> shopConfig::getCoppiaSettimanaAnno() const{
        return coppiaSettimanaAnno;
    }

    void shopConfig::setNote(std::string nuoveNote){
        note = nuoveNote;
    }

    const std::string& shopConfig::getNote() const{
        return note;
    }
}