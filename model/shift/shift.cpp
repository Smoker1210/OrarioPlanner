#include "shift.h"
#include "../utility/utility.h"
#include "qDebug"

namespace shiftplanner{

    double shift::intervalloOrario::calcolaOre() const{
        if(fineMinuti<=inizioMinuti) return 0.0;
        return (fineMinuti-inizioMinuti) / 60.0;
    }
    bool shift::intervalloOrario::copreMinuto(unsigned int minutoGiorno) const{
        return minutoGiorno >= inizioMinuti && minutoGiorno < fineMinuti;
    }
    shift::shift() : tipoGiorno(shift::Tipo::Assente), testoGrezzo("-") {}
    bool shift::isLavorativo() const{
        if (tipoGiorno == shift::Tipo::Continuato || tipoGiorno == shift::Tipo::Spezzato)
            return true;
        else
            return false;
    }
    double shift::oreTotali() const{
        if(!isLavorativo()) return 0.0;
        double totale = 0.0;
        double totaleGiorno = 0.0;
        for (auto it = intervalli.begin(); it != intervalli.end(); it++){
            totaleGiorno = (*it).calcolaOre();
            if(tipoGiorno == shift::Tipo::Continuato && totaleGiorno >= 7){
                totaleGiorno -= 0.5;
            }
            totale += totaleGiorno;
            totaleGiorno = 0.0;
        }
        return totale;
    }
    bool shift::isPresenteInMinuto(unsigned int minutoGiorno) const{
        if(!isLavorativo()) return false;
        for (auto it = intervalli.begin(); it != intervalli.end(); it++){
            if ((*it).copreMinuto(minutoGiorno)) return true;
        }
        return false;
    }
    std::string shift::getTipo() const{
        switch(tipoGiorno){
            case shift::Tipo::Continuato:
                return "Continuato";
            case shift::Tipo::Assente:
                return "Assente";
            case shift::Tipo::Legge104:
                return "Legge104";
            case shift::Tipo::Spezzato:
                return "Spezzato";
            case shift::Tipo::Malattia:
                return "Malattia";
            case shift::Tipo::Riposo:
                return "Riposo";
            case shift::Tipo::Corso:
                return "Corso";
            case shift::Tipo::Trasferta:
                return "Trasferta";
            default:
                return "Assente";
        }
    }
    std::string shift::getTesto() const{
        return testoGrezzo;
    }
    void shift::setContinuato(unsigned int inizio, unsigned int fine, const std::string& testo){
        tipoGiorno = shift::Tipo::Continuato;
        testoGrezzo = testo;
        intervalli = {{inizio, fine}};
    }
    void shift::setSpezzato(unsigned int inizio1, unsigned int fine1, unsigned int inizio2, unsigned int fine2, const std::string& testo){
        tipoGiorno = shift::Tipo::Spezzato;
        testoGrezzo = testo;
        intervalli = {{inizio1, fine1}, {inizio2, fine2}};
    }
    //l'assenza può essere per tipo riposo, malattia, assente, l104, corso o trasferta
    void shift::setAssenza(Tipo tipoAssenza, const std::string& testo){
        tipoGiorno = tipoAssenza;
        testoGrezzo = testo;
    }

    shift shift::fromString(const std::string& testo){
        std::string cleanText = utility::cleanString(testo);
        shift result;
        if(cleanText.empty()){
            result.setAssenza(Tipo::Assente, "-");
            return result;
        }
        if (cleanText == "R") {
            result.setAssenza(Tipo::Riposo, cleanText);
            return result;
        }
        if (cleanText == "-" || cleanText == "A") {
            result.setAssenza(Tipo::Assente, cleanText);
            return result;
        }
        if (cleanText == "M") {
            result.setAssenza(Tipo::Malattia, cleanText);
            return result;
        }
        if (cleanText == "104") {
            result.setAssenza(Tipo::Legge104, cleanText);
            return result;
        }
        if (cleanText == "CORSO"){
            result.setAssenza(Tipo::Corso, cleanText);
            return result;
        }
        if (cleanText.length()>2 && cleanText.substr(0,2) == "T:"){
            std::string innerText = cleanText.substr(2);
            innerText = utility::cleanString(innerText);
            bool allspazi = std::all_of(innerText.begin(), innerText.end(), [](unsigned char c){return std::isspace(c);});
            bool contieneCifre = std::any_of(innerText.begin(), innerText.end(), [](unsigned char c){return std::isdigit(c);});
            //controlliamo che la stringa sede che viene indicata nella trasferta non sia vuota, contenga solo spazi o contenga numeri
            bool isValid = (!innerText.empty() && !allspazi && !contieneCifre);
            if(isValid){
            result.setAssenza(Tipo::Trasferta, cleanText);
            } else{
                result.setAssenza(Tipo::Assente, "-");
            }
            return result;
        }
        //nel caso non sia un giorno di assenza allora gestisco il caso per lo spezzato o per il continuato
        size_t posSlash = cleanText.find('/');
        if (posSlash != std::string::npos){
            std::string primoBlocco = cleanText.substr(0,posSlash);
            std::string secondoBlocco = "";
            if(posSlash + 1<cleanText.length()){
                secondoBlocco = cleanText.substr(posSlash+1);
            }
            auto [ini1, fin1, valid1] = utility::estraiOrari(primoBlocco);
            auto [ini2,fin2, valid2] = utility::estraiOrari(secondoBlocco);
            bool primoValido = (ini1 != 0 || fin1 != 0 || valid1);
            bool secondoValido = (ini2 != 0 || fin2 != 0 || valid2);
            if(primoValido && secondoValido){
                utility::cleanBloccoOrari(primoBlocco);
                utility::cleanBloccoOrari(secondoBlocco);
                if(!primoBlocco.empty() && !secondoBlocco.empty()){
                    cleanText = primoBlocco + " / " + secondoBlocco;
                }
                result.setSpezzato(ini1, fin1, ini2, fin2, cleanText);
                return result;
            }
            result.setAssenza(Tipo::Assente, "-");
            return result;
        }
        auto [ini,fin, valid] = utility::estraiOrari(cleanText);
        bool orariValidi = (ini != 0 || fin != 0 || valid);
        if(!orariValidi){
            result.setAssenza(Tipo::Assente, "-");
        } else{
            utility::cleanBloccoOrari(cleanText);
            result.setContinuato(ini,fin, cleanText);
        }

        return result;
    }
}