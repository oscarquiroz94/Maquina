#ifndef __SALIDAS
#define __SALIDAS

#include "Utilities/Atmega.hpp"


class Output
{
    public:
        Output(const uint8_t _pin, const uint8_t _name) : pin(_pin), name(_name) {}

        void pinmode(int8_t mode) 
        {
            modo = mode;
            Atmega::pinmode(pin, mode);
        }
        int8_t getmode() {return modo;}

    protected:
        uint8_t pin = 0; 
        uint8_t name = 0; 
        int8_t modo = -1;
};


class DigitalOutput : public Output
{
    public:

        DigitalOutput(const uint8_t pin_onoff, const uint8_t name) 
            : Output(pin_onoff, name)
        {
            Atmega::printClass(F("DigitalOutput: instance created pin: "));
            Atmega::printClass(pin_onoff);
            Atmega::printClass(F(", name: "));
            Atmega::printlnClass(name);
            this->pin = pin_onoff;
            this->name = name;
        }

        void high() 
        {
            if (modo == -1)
            {
                Atmega::serial_print(F("*** PINMODE NO DEFINIDO, pin: "));
                Atmega::serial_println(pin);
            } 
            Atmega::digiWrite(pin, HIGH, name);
            Outputs::set_pin(name, true);
            state = true;
        }

        void low() 
        {
            if (modo == -1)
            {
                Atmega::serial_print(F("*** PINMODE NO DEFINIDO, pin: "));
                Atmega::serial_println(pin);
            } 
            Atmega::digiWrite(pin, LOW, name);
            state = false;
        } 

        void reset()
        {
            this->high();
            Atmega::retardo(200);
            this->low();
            Atmega::retardo(1500);
        }   

        bool get_state() const {return state;}

        ~DigitalOutput()
        {
            Atmega::printlnClass(F("DigitalOutput: instance deleted"));
        }

    private:
        
        bool state = false;
};

class AnalogOutput : public Output
{
    public:
        AnalogOutput(const uint8_t pin_pwm, const uint8_t name) 
            : Output(pin_pwm, name)
        {
            Atmega::printClass(F("AnalogOutput: instance created pin: "));
            Atmega::printClass(pin_pwm);
            Atmega::printClass(F(", name: "));
            Atmega::printlnClass(name);
            this->pin = pin_pwm;
            this->name = name;
        }
        void write(const int value)
        {
            Atmega::anaWrite(pin, value, name);
        }
        ~AnalogOutput()
        {
            Atmega::printlnClass(F("AnalogOutput: instance deleted"));
        }
};



#endif