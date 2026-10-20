#include "JsonReader.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <qjsonobject.h>
#include <QFile>
#include "JsonFactory.h"
#include "JsonWriter.h"
namespace shiftplanner{
namespace JSON{

bool JsonReader::load(shiftManager& manager, const QString& filename){
    QString path = filename.isEmpty()? JsonWriter::getStandardSavePath() : filename;

    QFile file(path);
    //gestione nel caso di assenza del file JSON default (es. primo avvio)
    if(!file.exists()){
        qDebug() << "File not found - generating default JSON file";
        JsonWriter::save(manager, path);
        return true;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "File is not accessible to read "<< file.error();
        return false;
    }
    QByteArray array = file.readAll();
    QJsonParseError errore;
    QJsonDocument doc = QJsonDocument::fromJson(array, &errore);
    if(errore.error != QJsonParseError::NoError){
        qWarning() << "Cannot Parse JSON Document: "<< errore.errorString();
        return false;
    }
    manager.clear();
    QJsonObject rootObject = doc.object();
    if (rootObject.contains("config_punto_vendita")){
        manager.setConfig(JsonFactory::createConfig(rootObject["config_punto_vendita"].toObject()));
    }
    if (rootObject.contains("lista_dipendenti")){
        QJsonArray arrayDipendenti = rootObject["lista_dipendenti"].toArray();
        for(auto it = arrayDipendenti.cbegin(); it!=arrayDipendenti.cend(); ++it){
            employee dip = JsonFactory::createEmployee((*it).toObject());
            manager.aggiungiDipendente(dip);
        }
    }
    if (rootObject.contains("griglia_settimanale")){
        QJsonObject gridObj = rootObject["griglia_settimanale"].toObject();
        for (auto it = gridObj.begin(); it != gridObj.end(); ++it){
            std::string key = it.key().toStdString();
            QJsonArray turniArray = it.value().toArray();
            for(auto shiftIt = turniArray.cbegin(); shiftIt != turniArray.cend(); ++shiftIt){
                QJsonObject shiftObj = (*shiftIt).toObject();
                int giorno = shiftObj["giorno"].toInt();
                shift s = JsonFactory::createShift(shiftObj);
                manager.impostaTurno(key,giorno,s);
            }
        }
    }
    file.close();
    return true;
}

}

}