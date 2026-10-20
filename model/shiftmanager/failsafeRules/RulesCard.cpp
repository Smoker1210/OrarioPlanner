#include "RulesCard.h"


namespace shiftplanner {
    std::vector<anomaly> coperturaMinima::validaGiorno(int giorno, const std::map<std::string, std::array<shift,7>>& grid,const shopConfig& config) const{
        const auto& orario = config.getOrarioGiorno(giorno);
        std::vector<anomaly> anomalie;
        if(!orario.aperto) return anomalie;
        unsigned int min = 0;
        for (min = orario.aperturaMinuti; min<orario.chiusuraMinuti; min+=30){
            int presenti = 0;
            for(const auto& pair : grid){
                if(pair.second[giorno].isPresenteInMinuto(min)) presenti++;
            }
            if (presenti < minimoPersonale){
                std::string messaggio = "Dipendenti Minimi non raggiunti: " + std::to_string(presenti) + "/" + std::to_string(minimoPersonale);
                anomaly anomalia = {giorno, static_cast<int>(min), codiceErrore, messaggio};
                anomalie.push_back(anomalia);
            }
        }
        return anomalie;
    }
    void coperturaMinima::setMinimoPersonale(int value){
        if (value>=0)
        minimoPersonale=value;
    }
    int coperturaMinima::getMinimoPersonale() const{
        return minimoPersonale;
    }
    coperturaMinima::coperturaMinima(int valore): minimoPersonale(valore) {
        codiceErrore = "MIN_COVERAGE";
    }

    //---------------------------------------------------------------------------//

    chiusuraAutorizzata::chiusuraAutorizzata(const std::vector<shiftplanner::ruolo>& vettoreRuoli, const std::vector<shiftplanner::employee>& activeEmployees):
        ruoliAutorizzati(vettoreRuoli), dipendentiAttivi(activeEmployees){
        codiceErrore = "MISSING_AUTHORIZED";
    }
    void chiusuraAutorizzata::addRuoloAutorizzato(shiftplanner::ruolo ruolo){
        ruoliAutorizzati.push_back(ruolo);
    }
    const std::vector<shiftplanner::ruolo>& chiusuraAutorizzata::getRuoliAutorizzati() const{
        return ruoliAutorizzati;
    }
    void chiusuraAutorizzata::clearRuoliAutorizzati(){
        ruoliAutorizzati.clear();
    }

    bool chiusuraAutorizzata::isAuthorized(const std::string& ruoloDip) const{
        shiftplanner::ruolo role = shiftplanner::employee::toRuolo(ruoloDip);
        for(const auto& ruolo : ruoliAutorizzati){
            if(role == ruolo)
                return true;
        }
        return false;
    }

    std::vector<anomaly> chiusuraAutorizzata::validaGiorno(int giorno, const std::map<std::string, std::array<shift,7>>& gridTurni, const shopConfig& config) const {
        const auto& orario = config.getOrarioGiorno(giorno);
        std::vector<anomaly> anomalie;
        int contatore = 0;
        if(!orario.aperto) return anomalie;
        unsigned int min = orario.chiusuraMinuti-30;
        for(const auto& pair : gridTurni){
            const shiftplanner::employee* dipendenteFocused = nullptr;
            //associazione della chiave sia nella mappa che nella lista dei dipendenti attivi
            for(const auto& dip : dipendentiAttivi){
                if(!dip.isAttivo()) continue;
                if(pair.first == dip.getKey()){
                    dipendenteFocused = &dip;
                }
            }
            //controllo intanto che il puntatore sia valido (quindi l'associazione abbia avuto successo),
            //che il dipendente sia presente nella mezz'ora della chiusura e che abbia il ruolo necessario per poter effettuare la chiusura
            if(dipendenteFocused && isAuthorized(dipendenteFocused->getRuolo()) && pair.second[giorno].isPresenteInMinuto(min)){
                contatore++;
            }
        }
        if(contatore<1){
            std::string descrizione = "Chiusura priva di personale autorizzato";
            anomalie.push_back(anomaly{giorno, static_cast<int>(min), codiceErrore, descrizione});
        }
        return anomalie;
    }
}