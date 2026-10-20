#include "JsonWriter.h"
#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
namespace shiftplanner{
namespace JSON{

QString JsonWriter::getStandardSavePath(){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    return dir.filePath("orario_settimanale.json");
}

bool JsonWriter::save(const shiftManager& manager, const QString& filename){
    QString path = filename.isEmpty() ? getStandardSavePath() : filename;
    JsonVisitor visitor;
    visitor.visit(manager.getConfig());
    visitor.visit(manager);
    QJsonDocument doc(visitor.getObject());
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qWarning() << "File is not accessible to write "<<file.error();
        return false;
    }
    QByteArray datiJson = doc.toJson(QJsonDocument::Indented);
    qint64 byte = file.write(datiJson);
    if(byte == -1){
        qWarning() << "Error during data writing "<<file.errorString();
    } else{
        qDebug()<< "JSON saved successfully in: "<<path;
    }
    file.close();
    return true;
}

}
}