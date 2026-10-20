#ifndef JSONVISITOR_H
#define JSONVISITOR_H
#include <QJsonObject>
#include "../shopconfig/shopconfig.h"
#include "../shiftmanager/shiftmanager.h"
#include "../employee/employee.h"

namespace shiftplanner{
namespace JSON{
//visitor concreto essenziale per la creazione dinamica di QJsonObject mappata in base alla classe
class JsonVisitor{
    private:
        QJsonObject object;

    public:
        JsonVisitor();
        QJsonObject getObject() const;
        void visit(const shopConfig&);
        void visit(const employee&);
        void visit(const shiftManager&);
};
}
}

#endif