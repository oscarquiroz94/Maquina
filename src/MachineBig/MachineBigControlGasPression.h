#ifndef __CONTROLPRESIONGASDR6
#define __CONTROLPRESIONGASDR6

#include "DR1_digital/ControlPresionGasDR1.h"
#include "Controlador/ControlPid.h"
#include "Controlador/EmuladorPlanta.h"

class ControlPresionGasDR6 : public ControlPresionGasDR1
{
    public:
        ControlPresionGasDR6(double kp, double ki, double kd) : 
            pid(kp, ki, kd, 0.2)
            //, emuladorvalvula(0.5, 0.2, 0.4) 
            {}

        double regular(Operativos& op) override;

        ~ControlPresionGasDR6(){}
    
    private:
        static constexpr double maxpresion = 50.0;

        ControlPid pid;
        //EmuladorPlanta emuladorvalvula;
};



#endif