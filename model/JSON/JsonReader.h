#ifndef JSONREADER_H
#define JSONREADER_H
#include <QString>
#include "shiftmanager/shiftmanager.h"

namespace shiftplanner{
namespace JSON{
class JsonReader{
public:
    static bool load(shiftManager&, const QString& = "");
};
}
}


#endif