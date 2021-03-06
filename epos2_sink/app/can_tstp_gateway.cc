#include <smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 1 * 1000000;
const TSTP::Time DATA_EXPIRY = 2 * DATA_PERIOD;
const TSTP::Time INTEREST_EXPIRY = 2ull * 60 * 60 * 1000000;

IF<Traits<USB>::enabled, USB, UART>::Result io;

OStream cout;

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
        int x = *_data;
        // cout << x << endl;
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
    //When connected to the gateway python script
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
    TSTP::Coordinates center(0, 20, 10);

    // Regions of interest
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region(center, 5000, start, end);

    // Data of interest
    Switch data_switch(region, DATA_EXPIRY, DATA_PERIOD);
    CAN_Data data_can(region, DATA_EXPIRY, DATA_PERIOD);

    // Event-driven actuators
    Printer<CAN_Data> p11(&data_can);

    // Time-triggered actuators
    while(TSTP::now() < end) {
        Alarm::delay(DATA_PERIOD);

        Switch::Value state = data_switch;

        data_switch = !state;
        led.set(!state);
    }

    return 0;
}
