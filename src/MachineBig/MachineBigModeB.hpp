#ifndef __SEMIDR6
#define __SEMIDR6

#include "ManualDR6.h"

/*
    SemiDR6 es un caso especial de ManualDR6
*/
class SemiDR6 : public ManualDR6
{
    public:
        SemiDR6()
        {
            Atmega::printlnClass(F("   SemiDR6: instance created"));
        }

        virtual uint8_t get_type() const {return System::CLASS_SEMI_DR6;}

        virtual void accion_lectura_potencias
            (   Datasets& dts,
                Potenciometro& pottambor,
                Potenciometro& potsopla,
                Potenciometro& potquem) override
        {
            Atmega::printlnDebug(F("SemiDR6: accion lectura potencias"));

            dts.op->analogInputs.pT = dts.op->analogInputs.buf_pT;
            dts.op->analogInputs.pS = dts.op->analogInputs.buf_pS;
            dts.op->analogInputs.pQ = 5.0 * (int)(dts.op->analogInputs.buf_pQ / 5.0);
        }
        

        ~SemiDR6()
        {
            Atmega::printlnClass(F("   SemiDR6: instance deleted"));
        }
};


#endif