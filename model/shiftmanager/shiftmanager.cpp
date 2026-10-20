#include "shiftmanager.h"
#include <algorithm>

namespace shiftplanner{
            shiftManager::shiftManager() : orariSettimanali(shopConfig()){
                addRegola(std::make_unique<coperturaMinima>(coperturaMinima(2)));
                std::vector<ruolo> ruoliAutorizzati = {ruolo::responsabile, ruolo::viceresponsabile};
                addRegola(std::make_unique<chiusuraAutorizzata>(chiusuraAutorizzata(ruoliAutorizzati, DipendentiAttivi)));
            }
            void shiftManager::aggiungiDipendente(const employee& nuovoDipendente){
                DipendentiAttivi.push_back(nuovoDipendente);
                std::array<shift,7> turniVuoti;
                turniVuoti.fill(shift());
                gridTurni[nuovoDipendente.getKey()] = turniVuoti;
                sortList();
                notifyObservers();
            }
            void shiftManager::rimuoviDipendente(const std::string& dipendenteKey){
                bool found = 0;
                for(auto it = DipendentiAttivi.begin(); it!=DipendentiAttivi.end() && !found; it++){
                    if (dipendenteKey == (*it).getKey()){
                        it = DipendentiAttivi.erase(it);
                        found = 1;
                    }
                }
                gridTurni.erase(dipendenteKey);
                sortList();
                notifyObservers();
            }
            void shiftManager::impostaTurno(const std::string& dipendenteKey, int giorno, const shift& nuovoTurno){
                if(giorno < 0 || giorno >= 7) return;
                if(gridTurni.find(dipendenteKey) == gridTurni.end()) return;
                gridTurni[dipendenteKey][giorno] = nuovoTurno;
                notifyObservers();
            }
            void shiftManager::clearTabella(){
                for(auto& pair: gridTurni){
                    pair.second.fill(shift());
                }
                notifyObservers();
            }
            void shiftManager::clear(){
                DipendentiAttivi.clear();
                gridTurni.clear();
                notifyObservers();
            }
            double shiftManager::calcolaOreDipendente(const std::string& dipendenteKey) const{
                auto it = gridTurni.find(dipendenteKey);
                if (it == gridTurni.end()) return 0.0;
                double tot = 0.0;
                for(const auto& shift : it->second){
                    tot += shift.oreTotali();
                }
                if(!getDipendente(dipendenteKey)->isCategoriaProtetta())
                    return tot;
                else
                    return tot/2;
            }
            double shiftManager::calcolaOreTotali() const{
                double totale = 0.0;
                for(const auto& pair : gridTurni){
                    //se il dipendente non è attivo non ha senso calcolarne le ore
                    if(getDipendente(pair.first)->isAttivo()){
                        for(const auto& shift : pair.second){
                            if(!getDipendente(pair.first)->isCategoriaProtetta())
                                totale += shift.oreTotali();
                            else
                                totale+= shift.oreTotali()/2;
                        }
                    }
                }
                return totale;
            }
            size_t shiftManager::getDipendentiAttiviSize() const{
                return DipendentiAttivi.size();
            }
            employee* shiftManager::getDipendente(unsigned int i){
                if (i >= getDipendentiAttiviSize())
                    return nullptr;
                else
                    return &(DipendentiAttivi[i]);
            }
            const employee* shiftManager::getDipendente(unsigned int i) const{
                if (i >= getDipendentiAttiviSize())
                    return nullptr;
                else
                    return &(DipendentiAttivi[i]);
            }

            employee* shiftManager::getDipendente(const std::string& key){
                for(auto& dip : DipendentiAttivi){
                    if(dip.getKey() == key)
                        return &dip;
                }
                return nullptr;
            }
            const employee* shiftManager::getDipendente(const std::string& key) const{
                for(const auto& dip : DipendentiAttivi){
                    if(dip.getKey() == key)
                        return &dip;
                }
                return nullptr;
            }
                        
            void shiftManager::addRegola(std::unique_ptr<GenericRule> nuovaRegola){
                regoleFailsafe.push_back(std::move(nuovaRegola));
            }
            GenericRule* shiftManager::getRegola(unsigned int i) const{
                if(i > regoleFailsafe.size()) return nullptr;
                else return regoleFailsafe[i].get();
            }
            std::vector<anomaly> shiftManager::checkRegoleFailsafe(int giorno) const{
                std::vector<anomaly> anomalie;
                for (const auto& regola : regoleFailsafe){
                    std::vector<anomaly> anomalieRegola = regola->validaGiorno(giorno, gridTurni, orariSettimanali);
                    if(!anomalieRegola.empty()){
                    anomalie.insert(anomalie.end(), anomalieRegola.begin(), anomalieRegola.end());
                    }
                }
                return anomalie;
            }
            shopConfig& shiftManager::getConfig(){
                return orariSettimanali;
            }
            const shopConfig& shiftManager::getConfig() const{
                return orariSettimanali;
            }
            void shiftManager::setConfig(const shopConfig& nuovo){
                orariSettimanali = nuovo;
            }
            const std::map<std::string, std::array<shift,7>>& shiftManager::getGrid() const{
                return gridTurni;
            }
            void shiftManager::sortList() {
                std::sort(DipendentiAttivi.begin(), DipendentiAttivi.end(), [](const auto& a, const auto& b) {
                    // Estraiamo il nome dai due oggetti/puntatori dipendente
                    const std::string& nomeA = a.getNome();
                    const std::string& nomeB = b.getNome();

                    // Confrontiamo in modo case-insensitive
                    return std::lexicographical_compare(
                        nomeA.begin(), nomeA.end(),
                        nomeB.begin(), nomeB.end(),
                        [](unsigned char c1, unsigned char c2) {
                            return std::tolower(c1) < std::tolower(c2);
                        }
                        );
                });
            }
            int shiftManager::countActiveEmployees() const{
                int contatore=0;
                for(const auto& dip : DipendentiAttivi){
                    if(dip.isAttivo()){
                        contatore++;
                    }
                }
                return contatore;
            }
}