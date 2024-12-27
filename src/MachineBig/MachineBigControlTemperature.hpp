#ifndef __CONTROLTEMPERATURADR6
#define __CONTROLTEMPERATURADR6

#include "Controlador/ControlPid.h"
#include "Operacion/Operativos.h"
#include "Utilities/Calculos.h"
#include "Controlador/EmuladorPlanta.h"

class ControlTemperaturaDR6
{
    public:
        //ToDo: En una DR12 -> gain = 25, tao = 3846
        ControlTemperaturaDR6(double kp, double ki, double kd) : 
            pid(kp, ki, kd, 0.2){}

        bool   get_is_windup() {return pid.get_is_windup();}
        double regular(double temp_actual, double temp_deseada);

        ~ControlTemperaturaDR6(){}
    
    private:
        ControlPid pid;
        
};



#endif