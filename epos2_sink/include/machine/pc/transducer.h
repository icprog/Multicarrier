// EPOS PC Smart Transducer Declarations

#ifndef __pc_transducer_h
#define __pc_transducer_h

#include <smart_data.h>

#include <keyboard.h>

__BEGIN_SYS

typedef TSTP::Region Region;
typedef TSTP::Coordinates Coordinates;

class Keyboard_Sensor: public Keyboard
{
public:
    static const unsigned int UNIT = TSTP::Unit::Acceleration;
    static const unsigned int NUM = TSTP::Unit::I32;
    static const int ERROR = 0; // Unknown

    static const bool INTERRUPT = true;
    static const bool POLLING = false;

    typedef Keyboard::Observer Observer;
    typedef Keyboard::Observed Observed;

public:
    Keyboard_Sensor() {}

    static void sense(unsigned int dev, Smart_Data<Keyboard_Sensor> * data) {
        if(ready_to_get())
            data->_value = get();
        else
            data->_value = -1;
    }

    static void actuate(unsigned int dev, Smart_Data<Keyboard_Sensor> * data, const Smart_Data<Keyboard_Sensor>::Value & command) {}
};

class Mass_Sensor
{
public:
    static const unsigned int UNIT = TSTP::Unit::Mass;
    static const unsigned int NUM = TSTP::Unit::I32;
    static const int ERROR = 0; // Unknown

    static const bool INTERRUPT = false;
    static const bool POLLING = true;

   typedef _UTIL::Observed Observed;
   typedef _UTIL::Observer Observer;

public:
   Mass_Sensor() {}

   static void attach(void * x) { }
   static void detach(void * x) { }

   static void sense(unsigned int dev, Smart_Data<Mass_Sensor> * data) {
    //    if(ready_to_get())
        //    data->_value = get();
        //    data->_value = -42;
    //    else
           data->_value = 4200000000000;
   }

   static void actuate(unsigned int dev, Smart_Data<Mass_Sensor> * data, const Smart_Data<Keyboard_Sensor>::Value & command) { }
};

typedef Smart_Data<Keyboard_Sensor> Acceleration;
typedef Smart_Data<Mass_Sensor> Mass_Sensor;

__END_SYS

#endif
