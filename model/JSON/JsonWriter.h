#ifndef JSONWRITER_H
#define JSONWRITER_H
#include "../shiftmanager/shiftmanager.h"
#include "JsonVisitor.h"

namespace shiftplanner{
namespace JSON{

constexpr const char* DefaultFile = "default.json";

class JsonWriter
{
public:
    JsonWriter() = delete;
    static QString getStandardSavePath();
    //metodo per la scrittura di un file json contenente: orari negozio, lista dipendenti, tabella orari settimanale
    static bool save(const shiftManager&, const QString& = "");
};


}
}

#endif // JSONWRITER_H
