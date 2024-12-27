#include "Motor.h"


Motor::Motor(const uint8_t pin, const uint8_t name) :
    out_onoff(pin, name),
    name(name),
    timer2off(false, true)
{
    Atmega::printlnClass(true, F("Motor"), name, F("pinonoff:"), pin);
    Atmega::printClass(F("Motor: instance created, pinonoff: "));
    Atmega::printClass(pin);
    Atmega::printClass(F(" name: "));
    Atmega::printlnClass(name);
}

bool MotorFijo::get_istimer_running() 
{   
    if (has_started)
    {
        has_started = false;
        return timer2off.getRun();   
    }else return true;
}

void MotorFijo::set_estado(bool flag)
{
    state = flag;
    if (state) out_onoff.high();
    else out_onoff.low();
}

void MotorFijo::set_estado_temporizado(bool& flag, uint32_t tiempo) 
{

    if (tiempo <= 0)
    {
        Atmega::serial_println(F("**** max tiempo agitar no configurado ****"));
        return;
    }
    flag = flag && !timer2off.tiempo(tiempo);
    if (!flag) timer2off.restart();
    else has_started = true;
    

    // Atmega::serial_print(timer2off.get_time());
    // Atmega::serial_print("  ");
    // Atmega::serial_println(flag);

    set_estado(flag);
}

MotorVariable::MotorVariable(const uint8_t pin, const uint8_t pwm, const uint8_t name) : 
    Motor(pin, name),
    out_pwm(pwm, name),
    tempodown(true, true, TemposNames::MOTOR_DOWN)
{
    Atmega::printlnClass(true, F("MotorVariable"), name, F("pinpwm"), pwm, F("pin"), pin);
    Atmega::printClass(F(" MotorVariable: instance created, pinpwm "));
    Atmega::printClass(pwm);
    Atmega::printClass(F(" pin: "));
    Atmega::printClass(pin);
    Atmega::printClass(F(" name: "));
    Atmega::printlnClass(name);
}

void MotorVariable::set_estado(const bool st)
{
    state = st;

    if (!state)
    {
        if (slow_down) decrease();
        out_onoff.low();
    }
    else out_onoff.high();
}

void MotorVariable::set_min_porcent(double val)
{ 
    min_porcent = val;
}

void MotorVariable::set_max_porcent(double val)
{ 
    max_porcent = val;
}

void MotorVariable::set_potencia(const double pow)
{
    potencia = pow;

    if (potencia > 100.0) potencia = 100.0;
    if (potencia < 0.0) potencia = 0.0;

    if (min_porcent < 0.0 || max_porcent < 0.0)
    {
        Atmega::serial_print(F("**** MotorVariable: "));
        Atmega::serial_print(name);
        Atmega::serial_println(F(" no configurado ****"));
    }

    if (min_porcent > 100.0 || max_porcent > 100.0)
    {
        Atmega::serial_print(F("**** MotorVariable: "));
        Atmega::serial_print(name);
        Atmega::serial_println(F(" mal configurado ****"));
    }

    uint8_t bvel = (uint8_t)Calculos::map_double
        (potencia, 0, 100, min_porcent*(255/100.0), max_porcent*(255/100.0));

    out_pwm.write(bvel);
}

void MotorVariable::decrease()
{
    //Calcular potencia mapeada actual para reducir desde ahi
    uint8_t bvel = (uint8_t)Calculos::map_double
        (potencia, 0, 100, min_porcent*(255/100.0), max_porcent*(255/100.0));
    int16_t safe_count = 0;

    if(bvel <= 0) return;

    while(true)
    {
        safe_count++;
        if(tempodown.tiempo(70)){
            out_pwm.write(bvel);
            bvel -= 5;
        }
        if (bvel <= 4) break;

        //**Obligatorio por safety
        //Evitar bloqueo si no se configura bien el temporizador
        //Maximo tiempo que tarda en reducir es
        //20 ciclos multiplicado por tiempo temporizador (70) = 1400ms
        if (safe_count > 2000) break;
        Atmega::retardo(1);
    }
}