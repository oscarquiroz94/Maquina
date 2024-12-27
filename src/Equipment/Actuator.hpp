#ifndef __ACTUADOR
#define __ACTUADOR

#include "InputOutput/Salidas.h"

class Actuador
{
    public:
        Actuador(const uint8_t pin_onoff, const uint8_t name) 
            : output(pin_onoff, name){}

        void open()  { output.high();  }
        void close() { output.low(); } 
        SalidaDigital& getpin() {return output;}
        bool get_state() const { return output.get_state(); }

        ~Actuador() {}

    private:
        SalidaDigital output;
};

#endif