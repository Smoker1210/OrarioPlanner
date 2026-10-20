#ifndef SHOPCONFIG_H
#define SHOPCONFIG_H
#include <array>
#include <string>

namespace shiftplanner{
    class shopConfig {
    public:
        struct OrarioGiorno{
            // (ore * 60) + minuti = minuti totali
            bool aperto = true;
            unsigned int aperturaMinuti = 510;
            unsigned int chiusuraMinuti = 1230;
            bool checkIntervallo(unsigned int inizio, unsigned int fine) const;
            OrarioGiorno();
            OrarioGiorno(bool,unsigned int, unsigned int);
        };
    private:
            std::array<OrarioGiorno,7> orariNegozio;
            int incasso = 0;
            double maxOre = 0.0;
            std::pair<int, int> coppiaSettimanaAnno = {0,0};
            std::string note;
    public:
            shopConfig();
            const OrarioGiorno& getOrarioGiorno(size_t giorno) const;
            void setOrarioGiorno(size_t giorno, const OrarioGiorno& orario);
            int getIncasso() const;
            void setIncasso(int);
            double getMaxOre() const;
            void setMaxOre(double);
            void setCoppiaSettimanaAnno(int settimana, int anno);
            std::pair<int, int> getCoppiaSettimanaAnno() const;
            const std::string& getNote() const;
            void setNote(std::string nuovaNota);
    };
}


#endif