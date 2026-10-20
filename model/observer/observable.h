#ifndef OBSERVABLE_H
#define OBSERVABLE_H
#include <vector>
#include "./observer.h"

namespace shiftplanner{

class observable{
    protected:
    std::vector<ObserverInterface*> observers;
    void notifyObservers();
    public:
    virtual ~observable() =default;
    void registerObserver(ObserverInterface*);
    void removeObserver(ObserverInterface*);
};

}

#endif