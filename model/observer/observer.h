#ifndef OBSERVER_H
#define OBSERVER_H


namespace shiftplanner{

class ObserverInterface{
    public:
    virtual ~ObserverInterface() = default;
    virtual void update() =0;
};

}

#endif