#ifndef __MOTOR
#define __MOTOR

#include "Utilities/Atmega.hpp"
#include "Utilities/Temporizer.hpp"
#include "InputOutput/Outputs.hpp"


class Motor
{
    public:
        Motor(const uint8_t onoff, const uint8_t name);
        
        //Las clases derivadas deben implementar
        virtual void set_estado(bool st) = 0;

        bool get_estado() const {return state;}
        //void temporizar(bool flag) { temporized = flag;}

        uint8_t get_name() const {return name;}
        SalidaDigital& getpin() {return out_onoff;}

        virtual ~Motor()
        {
            Atmega::printlnClass(F("Motor: instance deleted"));
        }
    
    protected:
        SalidaDigital out_onoff;
        uint8_t name = 0;
        bool state = false;
        bool temporized = false;
        bool has_started = false;
        Temporizer timer2off;
};

class MotorVariable : public Motor
{
    public:
        MotorVariable(const uint8_t onoff, const uint8_t pwm, const uint8_t name);

        void set_estado(bool st);
        
        void set_potencia(const double pow);
        double get_potencia() const {return potencia;}

        void set_min_porcent(double val);
        uint8_t get_min_porcent() const {return min_porcent;}
        
        void set_max_porcent(double val);
        uint8_t get_max_porcent() const {return max_porcent;}

        void set_slow_down(bool val){slow_down = val;}

        ~MotorVariable()
        {
            Atmega::printlnClass(F(" MotorVariable: instance deleted"));
        }
        
    private:

        double min_porcent = -1.0;
        double max_porcent = -1.0;

        SalidaAnalogica out_pwm;
        Temporizer tempodown;
        
        bool slow_down = false;
        double potencia = 0;
        
        void decrease();
};

class MotorFijo : public Motor
{
    public:
        MotorFijo (const uint8_t onoff, const uint8_t name) : Motor(onoff, name) 
        {
            Atmega::printClass(F(" Motor Fijo: pin "));
            Atmega::printClass(onoff);
            Atmega::printClass(F(" name: "));
            Atmega::printClass(name);
            Atmega::printlnClass(F(" created"));
        }

        ~MotorFijo()
        {
            Atmega::printlnClass(F(" MotorFijo: instance deleted"));
        }

        bool get_istimer_running();
        void set_estado_temporizado(bool& flag, uint32_t tiempo);
        void set_estado(bool flag);
};

class MotorInvertible
{
    public:
        MotorInvertible
        (   const uint8_t onoff_1, 
            const uint8_t onoff_2, 
            const uint8_t pwm, 
            const uint8_t name_1,
            const uint8_t name_2,
            bool sentido)
        {
            _onoff_1 = onoff_1;
            _onoff_2 = onoff_2;
            _pwm = pwm;
            _name_1 = name_1;
            _name_2 = name_2;
            _sentido = sentido;
            if (!_sentido) motor = new MotorVariable(_onoff_1, _pwm, _name_1);
            else motor = new MotorVariable(_onoff_2, _pwm, _name_2);
        }

        MotorVariable* get_instance() {return motor;}
        
        
        void invertir()
        {
            if (motor == nullptr) return;

            motor->set_estado(false);
            //Atmega::retardo(1000);
            if (motor->get_name() == _name_1)
            {
                delete motor;
                Atmega::printDebug("NAME 1: ");
                Atmega::printlnDebug((uint16_t)_name_1);
                motor = new MotorVariable(_onoff_2, _pwm, _name_2);
            }
            else if (motor->get_name() == _name_2)
            {
                delete motor;
                Atmega::printDebug("NAME 2: ");
                Atmega::printlnDebug((uint16_t)_name_2);
                motor = new MotorVariable(_onoff_1, _pwm, _name_1);
            }
        }

        ~MotorInvertible()
        {
            if (motor != nullptr) delete motor;
        }

    private:
        MotorVariable* motor = nullptr;
        uint8_t _onoff_1; 
        uint8_t _onoff_2; 
        uint8_t _pwm;
        uint8_t _name_1;
        uint8_t _name_2;
        bool _sentido;
};


#endif