#ifndef __SUPERDR6
#define __SUPERDR6

#include "Utilities/Atmega.h"
#include "Gestores/ISupervision.h"
#include "Roasters/IRoasterBig.h"
#include "NextionDR6.h"


class SupervisionDR6 : public ISupervision
{
    public:
        SupervisionDR6();

        void verify_temperatura(IRoaster& roaster);
        void verify_sensor_tempera(IRoaster& roaster);
        virtual void verify_rate_of_rise(IRoaster& roaster);
        void verify_emergencia(IRoaster& roaster);
        virtual void verify_descarga(IRoaster& roaster);
        virtual void verify_motores(IRoaster& roaster);
        virtual void verify_agitador(IRoaster& roaster);
        virtual void verify_enfriador(IRoaster& roaster);
        void verify_alertas(IRoaster& roaster);
        virtual void verify_quemador(IRoaster& roaster);

        void supervisar(IRoaster& roaster);
        void configure(IRoaster& roaster);
        uint8_t get_type() const {return System::CLASS_SUPER_BIG;}
        
        ~SupervisionDR6()
        {
            Atmega::printlnClass(F(" SupervisionDR6: instance deleted"));
        }

    protected:
        void check(const char* func, const char* entity);

    private:
        AlertaDR6 alerta_emergencia;
        AlertaDR6 alerta_nomotores;
        AlertaDR6 alerta_maxtemper;
        AlertaDR6 alerta_nosensor;
        AlertaDR6 alerta_maxror;
        AlertaDR6 alerta_noagit;
        AlertaDR6 alerta_noenfr;

        //GestorAlertas<AlertaDR6> gestoralertasDR6;

        Temporizador  tNoMotores;
        Temporizador  tNoAgitador;
        Temporizador  tNoEnfriador;

        
};



#endif