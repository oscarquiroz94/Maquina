#ifndef __ARTISANDR6
#define __ARTISANDR6

#include "DR1/ESP_DR1.h"

// ESP_DR6 extiende mas propiedades de ESP_DR1
class ESP_DR6 : public ESP_DR1
{
    public:
        ESP_DR6()
        {
            Atmega::printlnClass(F("   ESP_DR6: instance created"));
        }

        virtual void send(const Operativos& op);
        virtual void receive(Datasets& dts);
        
        ~ESP_DR6()
        {
            Atmega::printlnClass(F("   ESP_DR6: instance deleted"));
        }

    protected:
        InvariantFilter buff_soplador;
        InvariantFilter buff_quemador;
};



#endif