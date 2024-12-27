#ifndef __TEMPORIZER
#define __TEMPORIZER

#include "Utilities/Atmega.hpp"

class Temporizer
{
public:
    // false, true para iniciar contador desde cero y ejecutar accion
    // true, true para ejecutar accion y luego iniciar contador para otra ejecucion
    // false, false solo iniciar timer despues de darle vida
    // true, false sin efecto
    Temporizer(bool run, bool en, uint8_t name) : _running(run), _enable(en), _name(name)
    {
        Atmega::printClass(F("Temporizer instance created: "));
        Atmega::printlnClass(name);
    }

    Temporizer(bool run, bool en) : _running(run), _enable(en) {}

    virtual inline bool time(unsigned long time)
    {
        if (!_running && _enable)
        {
            _contador = Atmega::millisec();
            _running = true;
        }
        if (Atmega::millisec() - _contador >= time && _running && _enable)
        {
            _running = false;
            _lock = true; // bloquear el otro metodo
            return true;
        }
        return false;
    }

    inline bool time(unsigned long time, bool once)
    {
        if (!_lock)
            return time(time);
        else
            return false;
    }

    inline unsigned long get_time()
    {
        if (_running)
            return Atmega::millisec() - _contador;
        else
            return 0L;
    }

    inline void kill()
    {
        _enable = false;
        _running = false;
    }
    inline void restart() { _running = false; }
    inline void alive() { _enable = true; }
    inline bool getRun() { return _running; }
    inline bool is_alive() { return _enable; }
    inline uint8_t get_name() { return _name; }
    inline void reset() { _contador = Atmega::millisec(); }
    inline void unlock() { _lock = false; }

    ~Temporizer()
    {
        Atmega::printlnClass(F("Temporizer: instance deleted"));
    }

protected:
    bool _running;
    bool _enable;
    uint8_t _name = 0;
    unsigned long _contador = 0L;
    bool _lock = false;
};

class TemporizerMicro : public Temporizer
{
public:
    TemporizerMicro(bool run, bool en, uint8_t name) : Temporizer(run, en, name) {}
    TemporizerMicro(bool run, bool en) : Temporizer(run, en) {}

    inline bool time(unsigned long time)
    {
        if (!_running && _enable)
        {
            _contador = Atmega::microsec();
            _running = true;
        }
        if (Atmega::microsec() - _contador >= time && _running && _enable)
        {
            _running = false;
            return true;
        }
        return false;
    }

    ~TemporizerMicro() {}
};

#endif