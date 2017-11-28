#include <smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;


const TSTP::Time DATA_PERIOD = 1 * 1000000;
const TSTP::Time DATA_EXPIRY = 2 * DATA_PERIOD;
const TSTP::Time INTEREST_EXPIRY = 2ull * 60 * 60 * 1000000;

IF<Traits<USB>::enabled, USB, UART>::Result io;

template<typename T>
class Printer: public Smart_Data_Common::Observer
{
public:
    Printer(T * t) : _data(t) {
        _data->attach(this);
    }
    ~Printer() { _data->detach(this); }

    void update(Smart_Data_Common::Observed * obs) {
        print(_data->db_record());
    }

    void print(const Smart_Data_Common::DB_Record & d) {
        for(unsigned int i = 0; i < sizeof(Smart_Data_Common::DB_Record); i++)
            io.put(reinterpret_cast<const char *>(&d)[i]);
    }

private:
    T * _data;
};

int main()
{
    // Get epoch time from serial
    TSTP::Time epoch = 0;
    char c = io.get();
    if(c != 'X') {
        epoch += c - '0';
        c = io.get();
        while(c != 'X') {
            epoch *= 10;
            epoch += c - '0';
            c = io.get();
        }
        TSTP::epoch(epoch);
    }

    GPIO led('C', 3, GPIO::OUT);

    // Interest center points
    TSTP::Coordinates center(10, 10, 0);

    // Regions of interest
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region(center, 5000, start, end);

    // Data of interest
    Switch data_switch(region, DATA_EXPIRY, DATA_PERIOD);
    Mass_Sensor data_spot(region, DATA_EXPIRY, DATA_PERIOD);

    // Event-driven actuators
    Printer<Mass_Sensor> p11(&data_spot);

    // Time-triggered actuators
    while(TSTP::now() < end) {
        Alarm::delay(DATA_PERIOD);

        Switch::Value state = data_switch;

        data_switch = !state;
        led.set(!state);
    }

    return 0;
}
