#ifndef __DR6ACCESSOR
#define __DR6ACCESSOR

#include "RoasterDR6_confirmMotores.h"

class RoasterDR6_access : public RoasterDR6_confirmMotores
{
    public:
        Entradas&  get_sens_exhosto() {return sensExhosto;}
        Entradas&  get_sens_emergen() {return sensEmergen;}
        Entradas&  get_sens_falla_tambor() {return sensFallaTambor;}
        MotorFijo& get_motor_agitador() {return motoragitador;}
        MotorFijo& get_motor_enfriador() {return motorenfriador;}
};



#endif