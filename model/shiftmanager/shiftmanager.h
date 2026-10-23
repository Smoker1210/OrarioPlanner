#ifndef SHIFTMANAGER_H
#define SHIFTMANAGER_H
#include "../shift/shift.h"
#include "../employee/employee.h"
#include "../observer/observable.h"
#include "./failsaferules/RulesCard.h"
#include <map>
#include <array>
#include <vector>
#include <memory>

namespace shiftplanner{
    class shiftManager : public observable{
        private:
            std::vector<employee> DipendentiAttivi;
            std::map<std::string, std::array<shift,7>> gridTurni;
            shopConfig orariSettimanali;
            std::vector<std::unique_ptr<GenericRule>> regoleFailsafe;
            std::vector<const std::string*> sortedKeys;
        public:
            shiftManager();
            void aggiungiDipendente(const employee&);
            void rimuoviDipendente(const std::string&);
            void impostaTurno(const std::string&, int, const shift&);
            //imposta tutti gli shift dell'array nella map per ogni dipendente ad Assente
            void clearTabella();
            //clear di ogni elemento tranne config
            void clear();
            double calcolaOreDipendente(const std::string&) const;
            double calcolaOreTotali() const;
            size_t getDipendentiAttiviSize() const;
            employee* getDipendente(unsigned int);
            const employee* getDipendente(unsigned int) const;
            employee* getDipendente(const std::string& key);
            const employee* getDipendente(const std::string& key) const;
            void addRegola(std::unique_ptr<GenericRule>);
            GenericRule* getRegola(unsigned int) const;
            std::vector<anomaly> checkRegoleFailsafe(int giorno) const;
            shopConfig& getConfig();
            const shopConfig& getConfig() const;
            void setConfig(const shopConfig&);
            const std::map<std::string, std::array<shift,7>>& getGrid() const;
            void sortList();
            int countActiveEmployees() const;
            const std::vector<employee>& getListaDip() const;
    };
}

#endif