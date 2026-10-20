#include "observable.h"
#include "observer.h"

namespace shiftplanner{

void observable::registerObserver(ObserverInterface* nuovo){
    if(nuovo!=nullptr)
    observers.push_back(nuovo);
}

void observable::removeObserver(ObserverInterface* p){
    for(auto it=observers.begin(); it!=observers.end(); ++it){
        if(p == *it)
        it = observers.erase(it);
    }
}

void observable::notifyObservers(){
    for(auto it= observers.begin(); it!=observers.end(); ++it){
        if(*it!=nullptr){
            (*it)->update();
        }
    }
}

}