#ifndef __MACHINEBIG
#define __MACHINEBIG

// #include "NextionDR6.h"
#include "Equipment/Motor.hpp"
// #include "ManualDR6.h"
// #include "AlarmaDR6.h"
// #include "SemiDR6.h"
// #include "ControlPresionGasDR6.h"
// #include "Operacion/IOperacion.h"
// #include "Roasters/Roaster.h"


class MachineBig
{
    public:
        MachineBig();

        Motor*         get_motor_instance(const uint8_t id);
        Potenciometro* get_potenciom_instance(const uint8_t id);
        Roaster&       get_roaster_base() { return roaster; }
        INextion*      get_nextion_instance(const uint8_t type) { return roaster.get_nextion_instance(type); }

        Datasets&      get_datasets() { return roaster.dts; }
        IOperacion*    get_operation_instance(const uint8_t type) { return roaster.get_operation_instance(type); }

        void ejecutar(IOperacion *operacion) override;

        virtual uint8_t get_type() const { return System::CLASS_DR6; }

        ~MachineBig();

    protected:
        Roaster       roaster;
        MotorVariable motorA;
        MotorFijo     motorB;

        Potenciometro potenciomA;

        SalidaDigital pin_isr1;
    #if defined(BOARD_6_5) || defined(BOARD_6_0)
        SensorAnalogico sensEmergen;
        SensorAnalogico sensExhosto;
    #elif defined(BOARD_7X)
        SensorDigital sensEmergen;
    #if defined(BOARD_7_0)
        SensorAnalogico sensExhosto;
    #elif defined(BOARD_7_1)
        SensorDigital sensExhostoTambor;
    #endif
    #endif

    private:
        bool isvalidclass(IOperacion *operacion);
};

#endif