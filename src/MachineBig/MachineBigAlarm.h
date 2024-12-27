#ifndef __ALARMADR6
#define __ALARMADR6

#include "ManualDR6.h"

//AlarmaDR6 es un caso especifico de ManualDR6
class AlarmaDR6 : public ManualDR6
{
    public:
        AlarmaDR6()
        {
            Atmega::printlnClass(F("   AlarmaDR6: instance created"));
        }

        virtual uint8_t get_type() const {return System::CLASS_ALARMA_DR6;}

        //Se sobreescribe para evitar accion manual lectura potenciometros
        virtual void accion_lectura_potencias
        (   Datasets& dts,
            Potenciometro& pottambor,
            Potenciometro& potsopla,
            Potenciometro& potquem) override
        {
            Atmega::printlnDebug(F("AlarmaDR6: accion lectura potencias"));

            IOperacionSmall::accion_lectura_potencias(dts, pottambor);
            dts.op->analogInputs.pS = potsopla.read();
            dts.op->analogInputs.pQ = 0.0;
        }

        //Se sobreescribe para evitar accion manual quemador
        virtual void accion_quemador
        (   Datasets& dts,
            QuemadorBig& quemador,
            MotorVariable& motortambor) override
        {
            Atmega::printlnDebug(F("AlarmaDR6: accion quemador"));

            dts.op->digitalInputs.onquem = false;
            quemador.set_estado(false);

            //! Se lee el estado del pin
            dts.op->confirm.isQuemadorON = quemador.get_estado();
        }

        virtual void accion_motores
        (   Datasets& dts, 
            MotorVariable& motorTambor,
            MotorVariable& motorSoplador  ) override
        {
            Atmega::printlnDebug(F("AlarmaDR6: accion motores"));

            dts.op->digitalInputs.onrun = false;
            motorTambor.set_estado(false);
            motorSoplador.set_estado(false);
        }

        //Se sobreescribe para evitar accion manual agitar
        virtual void accion_agitar
        (   Datasets& dts,
            MotorFijo& motorAgitador ) override
        {
            Atmega::printlnDebug(F("AlarmaDR6: accion agitar"));

            dts.op->digitalInputs.onagitar = false;
            motorAgitador.set_estado(false);

            //! La confirmacion es el pin
            dts.op->confirm.isAgitarON = motorAgitador.get_estado();
        }

        //Se sobreescribe para evitar accion manual enfriar
        virtual void accion_enfriar
        (   Datasets& dts, 
            MotorFijo& motorEnfriar,
            bool& enfriardone) override
        {
            Atmega::printlnDebug(F("AlarmaDR6: accion enfriar"));

            dts.op->digitalInputs.onenfriamiento = false;
            motorEnfriar.set_estado(false);

            //! La confirmacion es el pin
            dts.op->confirm.isEnfriamientoON = motorEnfriar.get_estado();
        }

        ~AlarmaDR6()
        {
            Atmega::printlnClass(F("   AlarmaDR6: instance deleted"));
        }
};

#endif