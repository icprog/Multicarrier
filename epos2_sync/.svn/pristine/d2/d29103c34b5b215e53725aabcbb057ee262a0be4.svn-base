#include <smart_data.h>
#include <usb.h>

using namespace EPOS;

USB io;

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
    // Get epoch time from USB
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

    Alarm::delay(1000000);

    // Interest center points
    TSTP::Coordinates center(0, 600, 100);

    const TSTP::Time DATA_PERIOD = 5 * 1000000;
    const TSTP::Time DATA_EXPIRY = DATA_PERIOD;
    const TSTP::Time INTEREST_EXPIRY = 2ull * 60 * 60 * 1000000;

    // Regions of interest
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region(center, 200 * 100, start, end);

    // Data of interest
    Temperature data_temperature(region, DATA_EXPIRY, DATA_PERIOD);

    // Event-driven actuators
    Printer<Temperature> p0(&data_temperature);

    // And just let Smart_Data work!
    Thread::self()->suspend();

    return 0;
}
