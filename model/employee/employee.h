#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <string>

namespace shiftplanner {
enum class ruolo{
    responsabile,
    viceresponsabile,
    fiducia,
    addetto
};

class employee{
    private:
    std::string nome;
    std::string cognome;
    unsigned int oresettimanali;
    ruolo ruolonegozio;
    bool categoriaProtetta;
    bool attivo;
    public:
    employee(const std::string& name, const std::string& surname, unsigned int hours, ruolo role, bool listaSpeciale, bool active);
    std::string getNome() const;
    void setNome(const std::string&);
    std::string getCognome() const;
    void setCognome(const std::string&);
    //la key di un dipendente lo identifica univocamente, è formata dal nome e dal cognome separati da un underscore
    const std::string getKey() const;
    unsigned int getOreSett() const;
    void setOreSett(unsigned int);
    std::string getRuolo() const;
    void setRuolo(ruolo);
    static ruolo toRuolo(const std::string&);
    void setCategoriaProtetta(bool value);
    bool isCategoriaProtetta() const;
    void setAttivo(bool value);
    bool isAttivo() const;
};
}
#endif