#ifndef __TEDDR6
#define __TEDDR6

#include "ManualDR6_confirmMotores.h"

class TED_DR6 : public ManualDR6_confirmMotores
{
    public:
        TED_DR6() :
            pidgas(1.5, 0.5, 0.0),
            pidtemp(1.5, 0.01, 0.0) {}

        virtual uint8_t get_type() const {return System::CLASS_TED_DR6;}

        virtual void accion_potencia_quemador
            (   Datasets& dts, 
                QuemadorBig& quemador,
                Calculos& calculo,
                ControlPresionGas* pid_gas = nullptr) override;

        virtual void accion_lectura_potencias
            (   Datasets& dts,
                Potenciometro& pottambor,
                Potenciometro& potsopla,
                Potenciometro& potquem) override;

        //! TEMPORAL para pruebas
        // virtual void accion_lectura_temperaturas
        // (   Datasets& dts, 
        //     SensorAnalogico& sensAire, 
        //     SensorAnalogico& sensGrano,
        //     Calculos& calculo) override 
        // {
        //     calculo.rate_of_rise(dts);
        // }

        ~TED_DR6(){}

    private:
        ControlPresionGasDR6 pidgas;
        ControlTemperaturaDR6 pidtemp;
};

#endif