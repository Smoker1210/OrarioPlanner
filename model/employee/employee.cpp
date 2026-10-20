#include "employee.h"
namespace shiftplanner {
    employee::employee(const std::string& nome, const std::string& cognome, unsigned int oresettimanali, ruolo ruoloattuale, bool listaSpeciale, bool active):
    nome(nome), cognome(cognome), oresettimanali(oresettimanali), ruolonegozio(ruoloattuale), categoriaProtetta(listaSpeciale), attivo(active){}
    std::string employee::getNome() const{
        return nome;
    }
    void employee::setNome(const std::string& nuovonome){
        nome = nuovonome;
    }
    std::string employee::getCognome() const{
        return cognome;
    }
    void employee::setCognome(const std::string& nuovocognome){
        cognome = nuovocognome;
    }
    const std::string employee::getKey() const{
        return nome + "_" + cognome; 
    }
    unsigned int employee::getOreSett() const{
        return oresettimanali;
    }
    void employee::setOreSett(unsigned int ore){
        oresettimanali = ore;
    }
    std::string employee::getRuolo() const{
        switch (ruolonegozio){
            case ruolo::responsabile:
                return "responsabile";
                break;
            case ruolo::viceresponsabile:
                return "viceresponsabile";
                break;
            case ruolo::fiducia:
                return "fiducia";
                break;
            case ruolo::addetto:
                return "addetto";
                break;
            default:
                return "addetto";
                break;
        }
    }
    void employee::setRuolo(ruolo nuovoruolo){
        ruolonegozio = nuovoruolo;
    }
    ruolo employee::toRuolo(const std::string& str){
        if(str == "responsabile")
            return ruolo::responsabile;
        if(str == "viceresponsabile")
            return ruolo::viceresponsabile;
        if(str == "fiducia")
            return ruolo::fiducia;
        if(str == "addetto")
            return ruolo::addetto;
        return ruolo::addetto;
    }

    void employee::setCategoriaProtetta(bool value){
        categoriaProtetta = value;
    }
    bool employee::isCategoriaProtetta() const{
        return categoriaProtetta;
    }
    void employee::setAttivo(bool value){
        attivo = value;
    }
    bool employee::isAttivo() const{
        return attivo;
    }
}