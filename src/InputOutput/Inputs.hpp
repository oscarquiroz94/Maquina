#ifndef __ENTRADAS
#define __ENTRADAS

#include "Utilities/ADS1115.h"
#include "Utilities/MeanFilterLib.h"
#include "Utilities/VariantFilter.h"
#include "InterfazInputs.h"
#include "Emulacion/ConectorInputsGui.h"

#include "Utilities/Atmega.h"
#include "Compilationtype.h"

#ifndef DEPLOY
#include <math.h>
#include <stdio.h>
#include <string.h>
#endif

//extern ConectorInputsGui conector_ingui;

class Entradas 
{
    public:
        Entradas(uint8_t pin, uint8_t name) 
        {
            Atmega::printlnClass(F("Entrada: instance created"));
            _pin = pin;
            _name = name;
        }
        
        void pinmode(int8_t mode) 
        {
            modo = mode;
            Atmega::pinmode(_pin, (uint8_t)modo);
        }

        int8_t getmode() {return modo;}
        uint8_t get_name() const {return _name;}

        ~Entradas()
        {
            Atmega::printlnClass(F("Entrada: instance deleted"));
        }
    
    protected:
        uint8_t _pin = 0;
        uint8_t _name = 0;
        int8_t modo = -1;
        InterfazInputs interfaz;
};

class EntradaAnalogica : public Entradas
{
    public:
        EntradaAnalogica(uint8_t pin, uint8_t name) : Entradas(pin, name)
        {
            Atmega::printClass(F(" Entrada analogica, pin: "));
            Atmega::printClass(pin);
            Atmega::printClass(F(" name: "));
            Atmega::printClass(name);
            Atmega::printlnClass(F(" created"));
        }

        double read()
        {
            if (modo == -1)
            {
                Atmega::serial_print(F("*** PINMODE NO DEFINIDO, pin: "));
                Atmega::serial_println(_pin);
            } 
            return interfaz.anaread(_pin, _name);
        }
        ~EntradaAnalogica(){}
    
};

class SensorDigital : public Entradas
{
    public:

        SensorDigital(uint8_t pin, uint8_t name, bool logic) : Entradas(pin, name)
        {
            Atmega::printClass(F(" Sensor digital, pin: "));
            Atmega::printClass(pin);
            Atmega::printClass(F(" name: "));
            Atmega::printClass(name);
            Atmega::printlnClass(F(" created"));
            
            _logic = logic;
            //conector_ingui.registrar(name);
        }
        bool read()
        {
            if (modo == -1)
            {
                Atmega::serial_print(F("*** PINMODE NO DEFINIDO, pin: "));
                Atmega::serial_println(_pin);
            } 
            return interfaz.digiread(_pin, _name, _logic);
        }

        ~SensorDigital(){}

    private:
        bool _logic = false;
};


class SensorAnalogico : public Entradas
{
    public:
        SensorAnalogico
        (   const uint8_t  mplex, 
            const uint8_t  pin, 
            ADS1115& chip, 
            const uint8_t  window, 
            const uint8_t  name);

        double read(double (*ptr_func)(double));
        bool pollAlertReadyPin();

        //For testing
        uint8_t get_mplex() const {return multiplex;}
        uint8_t get_pin()   const {return pinready;}
        uint8_t get_chip()  const {return chip;}
        void    set_offset(double value) {offset = value;}
        double  get_offset() {return offset;}

        ~SensorAnalogico(){}

    protected:
        
        ADS1115* ads = nullptr;
        uint8_t chip      = 0;
        uint8_t pinready  = 0;
        uint8_t multiplex = 0; 
        MeanFilter<double> meanFilter; 

    private:
        int count = 0;
        double offset = 0.0;
};

class SensorAnalogicoFiltroVariable : public SensorAnalogico
{
    public:
        SensorAnalogicoFiltroVariable
        (   const uint8_t  mplex, 
            const uint8_t  pin, 
            ADS1115& chip, 
            const uint8_t  window, 
            const uint8_t  name) :
        SensorAnalogico(mplex, pin, chip, window, name) , meanFilter(window) {}

    protected:
        VariantFilter meanFilter;
};



#endif