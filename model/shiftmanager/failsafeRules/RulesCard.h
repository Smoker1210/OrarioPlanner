#ifndef RULECARD_H
#define RULECARD_H
#include "../../shift/shift.h"
#include <map>
#include "../../shopconfig/shopconfig.h"
#include "employee/employee.h"

namespace shiftplanner{
    struct anomaly{
        int giorno;
        int minutoInizio;
        std::string regolaInfranta; // {MIN_COVERAGE, MISSING_AUTHORIZED}
        std::string descrizione;
    };

    class GenericRule{
        public:
            virtual ~GenericRule() = default;
            virtual std::vector<anomaly> validaGiorno(int giorno, const std::map<std::string, std::array<shift,7>>& gridTurni, const shopConfig& config) const = 0;
    };

    //controlla che siano presenti almeno due persone per ogni mezz'ora di tutti i giorni lavorativi della settimana
    class coperturaMinima : public GenericRule{
        private:
            int minimoPersonale;
            std::string codiceErrore;
        public:
            coperturaMinima(int);
            void setMinimoPersonale(int);
            int getMinimoPersonale() const;
            std::vector<anomaly> validaGiorno(int giorno, const std::map<std::string, std::array<shift,7>>& gridTurni, const shopConfig& config) const override;
    };
//Controlla la presenza di almeno il responsabile o il vice nella chiusura di ciascun giorno
    class chiusuraAutorizzata : public GenericRule{
    private:
        std::vector<shiftplanner::ruolo> ruoliAutorizzati;
        const std::vector<shiftplanner::employee>& dipendentiAttivi;
        std::string codiceErrore;
    public:
        chiusuraAutorizzata(const std::vector<shiftplanner::ruolo>& vettoreRuoli, const std::vector<shiftplanner::employee>& dipendentiAttivi);
        void addRuoloAutorizzato(shiftplanner::ruolo ruolo);
        const std::vector<shiftplanner::ruolo>& getRuoliAutorizzati() const;
        void clearRuoliAutorizzati();
        std::vector<anomaly> validaGiorno(int giorno, const std::map<std::string, std::array<shift,7>>& gridTurni, const shopConfig& config) const override;
        bool isAuthorized(const std::string& ruoloDip = "") const;
    };
}


#endif