#include "JsonFactory.h"
#include "qjsonarray.h"
#include "utility/utility.h"

namespace shiftplanner{
namespace JSON{

shopConfig JsonFactory::createConfig(const QJsonObject& json){
    shopConfig result;
    QJsonArray arrayOrari = json["orari_settimanali"].toArray();
    for(auto it = arrayOrari.cbegin(); it!=arrayOrari.cend(); ++it){
        QJsonObject giorno = (*it).toObject();
        result.setOrarioGiorno(static_cast<size_t>(giorno["giorno"].toInt()),
                               shopConfig::OrarioGiorno(giorno["aperto"].toBool(), utility::stringOraToMinuti(giorno["apertura"].toString().toStdString()),
                                                        utility::stringOraToMinuti(giorno["chiusura"].toString().toStdString())));
    }
    result.setIncasso(json["incasso"].toDouble());
    result.setMaxOre(json["maxOre"].toDouble());
    QJsonObject InfoSettimanaObj = json["info_settimana"].toObject();
    result.setCoppiaSettimanaAnno(InfoSettimanaObj["numero_settimana"].toInt(), InfoSettimanaObj["anno"].toInt());
    result.setNote(InfoSettimanaObj["note"].toString().toStdString());
    return result;
}

employee JsonFactory::createEmployee(const QJsonObject& json){
    return employee(json["nome"].toString().toStdString(), json["cognome"].toString().toStdString(), static_cast<unsigned int>(json["ore_settimanali"].toInt()), employee::toRuolo(json["ruolo"].toString().toStdString()),
                    json["categoria_protetta"].toBool(), json["attivo"].toBool());
}

shift JsonFactory::createShift(const QJsonObject& json){
    return shift::fromString(json["testo_grezzo"].toString().toStdString());
}

}
}