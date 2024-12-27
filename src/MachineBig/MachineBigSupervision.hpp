#ifndef __SUPERDR6PID
#define __SUPERDR6PID

#include "SupervisionDR6.h"

class SupervisionDR6_PIDs : public SupervisionDR6
{
    public:
        SupervisionDR6_PIDs() {}

        void supervisar(IRoaster& roaster) override
        {
            verify_ted(roaster);

            SupervisionDR6::supervisar(roaster);
        }

        void verify_ted(IRoaster& roaster)
        {
            Atmega::printlnDebug(F("SupervisionDR6_PIDs: verify ted"));

            INextion* nx = roaster.get_nextion_instance(roaster.get_type());
            Datasets& dts = roaster.get_datasets();
            if (dts.op == nullptr) {check(__func__, "dts.op"); return;}
            if (nx == nullptr) {Atmega::printlnDebug(F("*** Puntero a nx no existe ***")); return;}

            if (dts.op->isted())
            {
                nx->nxcom.send(PSTRr("page0.b5.bco=21675"));
                nx->nxcom.send(PSTRr("page0.b5.bco2=21675"));
            }
            else
            {
                nx->nxcom.send(PSTRr("page0.b5.bco=50712"));
                nx->nxcom.send(PSTRr("page0.b5.bco2=50712"));
            }

            //!removido 20241101
            //if (!dts.op->confirm.isTemperaOK) dts.op->setTedmode(false);
            
        }

        //Deshabilitar en modo TED
        void verify_rate_of_rise(IRoaster& roaster) override 
        {
            Atmega::printlnDebug(F("SupervisionDR6_PIDs: verify rate of rise"));

            Datasets& dts = roaster.get_datasets();
            if (!dts.op->isted()) SupervisionDR6::verify_rate_of_rise(roaster);
        }

        ~SupervisionDR6_PIDs() {}
};

#endif