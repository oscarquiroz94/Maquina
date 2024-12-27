#ifndef __MANUALDR6
#define __MANUALDR6

#include "NextionDR6.h"
#include "Operacion/IOperacionSmall.h"
#include "Operacion/IOperacionBig.h"
#include "Controlador/Precalentador.h"

class ManualDR6 : public IOperacionBig
{
    public:
        ManualDR6();

        virtual uint8_t get_type() const {return System::CLASS_MANUAL_DR6;}

        virtual void accion_lectura_emergencia
            (   Datasets& dts,
                Entradas& sensEmergen) override;
            
        virtual void accion_lectura_exhosto
            (   Datasets& dts,
                Entradas& sensExhosto) override;

        virtual void accion_lectura_quemador
            (   Datasets& dts,
                QuemadorBig& quemador) override;
        
        virtual void accion_lectura_motores
            (   Datasets& dts,
                MotorFijo& motorAgitador,
                MotorFijo& motorEnfriar);

        virtual void accion_lectura_potencias
            (   Datasets& dts,
                Potenciometro& pottambor,
                Potenciometro& potsopla,
                Potenciometro& potquem) override;

        virtual void accion_motores
            (   Datasets& dts, 
                MotorVariable& motorTambor,
                MotorVariable& motorSoplador  ) override;

        virtual void accion_potencias_motores
            (   Operativos& op, 
                MotorVariable& motorTambor,
                MotorVariable& motorSoplador ) override;

        virtual void accion_trimmers
            (   Datasets& dts, 
                MotorVariable& motorTambor,
                MotorVariable& motorSoplador,
                QuemadorBig& quemador  ) override;

        virtual void accion_quemador
            (   Datasets& dts, 
                QuemadorBig& quemador,
                MotorVariable& motortambor) override;

        virtual void accion_potencia_quemador
            (   Datasets& dts, 
                QuemadorBig& quemador,
                Calculos& calculo,
                ControlPresionGas* pid_gas = nullptr) override;

        virtual void accion_mapeo
            (   Datasets& dts, 
                Calculos& calculo   ) override;

        virtual void accion_agitar
            (   Datasets& dts, 
                MotorFijo& motorAgitador ) override;

        virtual void accion_enfriar
            (   Datasets& dts, 
                MotorFijo& motorEnfriar,
                bool& enfriardone) override;

        ~ManualDR6()
        {
            Atmega::printlnClass(F("  ManualDR6 instance deleted"));
        }

    private:
        Temporizador tEnfriar;
        Temporizador tAgitar;

        InterfazInputs interfaz;
        
};
#endif