#include "./JsonVisitor.h"
#include "../utility/utility.h"
#include <QJsonArray>
namespace shiftplanner{
namespace JSON{

JsonVisitor::JsonVisitor() = default;

QJsonObject JsonVisitor::getObject() const{
    return object;
}

void JsonVisitor::visit(const shopConfig& config){
    QJsonObject configObj;
    QJsonArray arrayOrari;
    for(int i = 0; i<7; i++){
        const auto& orarioGiorno = config.getOrarioGiorno(i);
        QJsonObject giornoObj;
        giornoObj["giorno"] = i;
        giornoObj["aperto"] = orarioGiorno.aperto;
        giornoObj["apertura"] = QString::fromStdString(utility::minutiToOraString(orarioGiorno.aperturaMinuti));
        giornoObj["chiusura"] = QString::fromStdString(utility::minutiToOraString(orarioGiorno.chiusuraMinuti));
        arrayOrari.append(giornoObj);
    }
    configObj["orari_settimanali"] = arrayOrari;
    configObj["incasso"] = config.getIncasso();
    configObj["maxOre"] = config.getMaxOre();
    QJsonObject settimanaObj;
    settimanaObj["numero_settimana"] = config.getCoppiaSettimanaAnno().first;
    settimanaObj["anno"] = config.getCoppiaSettimanaAnno().second;
    settimanaObj["note"] = QString::fromStdString(config.getNote());
    configObj["info_settimana"]=settimanaObj;
    object["config_punto_vendita"] = configObj;
}

void JsonVisitor::visit(const shiftManager& manager){
    QJsonArray arrayDipendenti;
    for (size_t i = 0; i<manager.getDipendentiAttiviSize(); i++){
        JsonVisitor visitorDipendente;
        visitorDipendente.visit(*manager.getDipendente(i));
        arrayDipendenti.append(visitorDipendente.getObject());
    }
    object["lista_dipendenti"] = arrayDipendenti;
    QJsonObject gridObj;
    for(const auto& pair: manager.getGrid()){
        QJsonArray listaTurni;
        int i = 0;
        for(const auto& giorno : pair.second){
            QJsonObject shiftObj;
            shiftObj["giorno"] = i;
            shiftObj["tipo"] = QString::fromStdString(giorno.getTipo());
            shiftObj["testo_grezzo"] = QString::fromStdString(giorno.getTesto());
            listaTurni.append(shiftObj);
            i++;
        }
        gridObj[QString::fromStdString(pair.first)] = listaTurni;
    }
    object["griglia_settimanale"] = gridObj;
}

void JsonVisitor::visit(const employee& dipendente){
    object["nome"] = QString::fromStdString(dipendente.getNome());
    object["cognome"] = QString::fromStdString(dipendente.getCognome());
    object["ruolo"] = QString::fromStdString(dipendente.getRuolo());
    object["ore_settimanali"] = static_cast<int>(dipendente.getOreSett());
    object["categoria_protetta"] = dipendente.isCategoriaProtetta();
    object["attivo"] = dipendente.isAttivo();
}

}
}