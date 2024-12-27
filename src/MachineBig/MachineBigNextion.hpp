#ifndef __NEXTIONDR6
#define __NEXTIONDR6

#include "Nextions/INextion.h"

class NextionDR6 : public INextion
{
    public:
        NextionDR6()
        {
            Atmega::printlnClass(F(" NextionDR6 instance created"));
        }

        virtual void send(const Operativos& op);
        virtual void send_stack(const Datasets& dts);
        virtual void receive(Datasets& dts);
        virtual void requestHandler(Datasets& dts);
        virtual uint8_t get_type() const {return System::CLASS_NX_DR6;}


        bool get_enable_superagit() {return enable_super_agit;}

        ~NextionDR6()
        {
            Atmega::printlnClass(F(" NextionDR6 instance deleted"));
        }

    protected:

        virtual void set_auto(Datasets& dts);
        virtual void quit_descarga(Datasets& dts);
        virtual void set_run(Datasets& dts, bool state);
        virtual void set_quemador(Datasets& dts, bool state);
        virtual void set_enfriador(Datasets& dts, bool state);
        virtual void set_agitador(Datasets& dts, bool state);
        virtual void set_agitador_nosuperv(Datasets& dts, bool state);
        virtual void set_last_conf(Datasets& dts);
        virtual void set_tiac(Datasets& dts);

        void crear_socket(Datasets& dts);
        void eliminar_socket(Datasets& dts);
        void set_cv(Datasets& dts);
        void other_ror(Datasets& dts);
        void set_channelA(Datasets& dts);
        void set_channelB(Datasets& dts);
        void set_artisan(Datasets& dts);
        void set_cropster(Datasets& dts);
        void set_manual(Datasets& dts);
        
        void set_nxversion(Datasets& dts);

        //Permitir acivar agitador sin supervision
        bool enable_super_agit = false;
        
    private:
        void check(const char* func, const char* entity);
        

};

#endif