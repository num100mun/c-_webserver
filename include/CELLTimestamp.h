
//
// Created by 18746 on 2023/11/9.
//

#ifndef C___WEBSERVER2_0_CELLTIMESTAMP_H
#define C___WEBSERVER2_0_CELLTIMESTAMP_H


#include <chrono>
using namespace std::chrono;
class CELLTime{
public:
    static time_t getNowInMilliSec(){
        return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    }
};
class CELLTimestamp
{
public:
    CELLTimestamp(){update();}
    ~CELLTimestamp(){}
    void update(){_begin = high_resolution_clock ::now();}
    double getElapsedSecond(){return this->getElapsedTimeInMicroSec() * 0.000001;}
    double getElapsedTimeInMillSec(){return this->getElapsedTimeInMicroSec() * 0.001;}
    long long getElapsedTimeInMicroSec(){return duration_cast<microseconds>(high_resolution_clock ::now() - _begin).count();}
protected:
    time_point<high_resolution_clock> _begin;
};


#endif //C___WEBSERVER2_0_CELLTIMESTAMP_H
