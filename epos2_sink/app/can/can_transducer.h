#ifndef CAN_TRANSDUCER_H
#define CAN_TRANSDUCER_H

#include <smart_data.h>
#include <utility/observer.h>
#include <tstp.h>
#include <utility/ostream.h>
#include <gpio.h>

using namespace EPOS;

OStream cout;

class Can_Transducer
{
  public:
    static const unsigned int MAX_DEVICES = 4;

    typedef _UTIL::Observed Observed;
    typedef _UTIL::Observer Observer;

    static const unsigned int UNIT = TSTP::Unit::Length;
    static const unsigned int NUM = TSTP::Unit::I32;
    static const int ERROR = 0;

    static const bool INTERRUPT = false;
    static const bool POLLING = true;

    Can_Transducer(unsigned int dev) {}

    static void sense(unsigned int dev, Smart_Data<Can_Transducer> * data) {
        cout << "Realizando leitura" << endl;
        data->_value = 42; //Trocar isso pela leitura do sniffer
    }

    static void actuate(unsigned int dev, Smart_Data<Can_Transducer> * data, const Smart_Data<Can_Transducer>::Value & command) {}

    static void attach(Observer * obs) { /*_observed.attach(obs);*/ }
    static void detach(Observer * obs) { /*_observed.detach(obs);*/ }

};

typedef Smart_Data<Can_Transducer> SmartCAN;

#endif // CAN_TRANSDUCER_H
