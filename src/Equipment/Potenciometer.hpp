#ifndef __POTENCIOMETRO
#define __POTENCIOMETRO

#include "Utilities/Atmega.h"
#include "InputOutput/Entradas.h"

class Potenciometro
{
    public:
        Potenciometro(const uint8_t pin_read, const uint8_t name) 
            : input(pin_read, name)
        {
            Atmega::printClass(F(" Potenciometro: instance created, pin: "));
            Atmega::printlnClass(pin_read);
        }

        double read(){
            if (!enablepot) return 0.0;

            double value = input.read() * 100.0 / 1023.0;

            if (value >= 98.0) value = 100.0;

            value = fraccion * (int)(value / fraccion); //Fraccionar

            return value;
        }

        void enable() {enablepot = true;};
        void disable() {enablepot = false;};
        void fraccionar(double val) {fraccion = val;}
        EntradaAnalogica& getpin() {return input;} 

        ~Potenciometro()
        {
            Atmega::printlnClass(F(" Potenciometro: instance deleted"));
        }
    
    private:
        EntradaAnalogica input;
        bool enablepot = true;
        double fraccion = 1.0;
};

#endif
