#ifndef JSONFACTORY_H
#define JSONFACTORY_H
#include <QJsonObject>
#include "./shiftmanager/shiftmanager.h"
#include "./shopconfig/shopconfig.h"

namespace shiftplanner{
namespace JSON{

//Classe che si occupa di leggere i JsonObject e deserializzarli in shopconfig, lista dipendenti e tabella oraria settimanale
class JsonFactory
{
public:
    JsonFactory() = delete;
    static void extracted(shopConfig &result, QJsonArray &arrayOrari);
    static shopConfig createConfig(const QJsonObject &json);
    static employee createEmployee(const QJsonObject& json);
    static shift createShift(const QJsonObject& json);
};

}
}

#endif // JSONFACTORY_H
