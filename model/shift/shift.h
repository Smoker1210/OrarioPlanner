#ifndef SHIFT_H
#define SHIFT_H
#include <string>
#include <vector>
namespace shiftplanner{

class shift{
public:
    // enum di tutti i possibili turni, le ferie sono omesse poiché semplicemente un dipendente in ferie non appare nella tabella oraria
    enum class Tipo {
        Continuato, //lavoro a blocco unico
        Spezzato, //lavoro a due blocchi separati
        Riposo, // R
        Malattia, // M
        Legge104, // L104
        Corso, // corso di qualsiasi genere
        Assente // indica il giorno di semplice assenza, è diverso dal riposo e viene indicato da "-"
    };
private:
    struct intervalloOrario{
    unsigned int inizioMinuti = 0;
    unsigned int fineMinuti = 0;
    double calcolaOre() const;
    bool copreMinuto(unsigned int) const;    
};
Tipo tipoGiorno;
std::string testoGrezzo; //la string digitata dall'utente nella tabella dell'interfaccia
std::vector<intervalloOrario> intervalli; //nel caso di orario continuato ci sarà un solo elemento di classe intervalloOrario, per lo spezzato invece saranno 2
public:
    shift();
    bool isLavorativo() const;
    //calcole le ore totali del turno, in caso di continuato con >=7 ore totali sottraiamo 0.5 per la pausa obbligatoria
    double oreTotali() const;
    bool isPresenteInMinuto(unsigned int) const;
    std::string getTipo() const;
    std::string getTesto() const;
    void setContinuato(unsigned int, unsigned int, const std::string&);
    void setSpezzato(unsigned int, unsigned int, unsigned int, unsigned int, const std::string&);
    //l'assenza può essere per tipo riposo, malattia, assente o l104
    void setAssenza(Tipo, const std::string&);
    //la funzione che tiene in piedi l'intera logica dei turni, senza di questa non funziona minimamente la tabella degli orari
    static shift fromString(const std::string&);
};
}

#endif