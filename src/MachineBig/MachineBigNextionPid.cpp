#include "NextionDR6_PIDs.h"

void NextionDR6_PIDs::receive(Datasets& dts)
{
    if(nxcom.comparar(nxcom.comando,"REGU,"))
    {
        char *listaValores = strtok(nxcom.comando,",");

        listaValores = strtok(NULL,",");
        dts.op->analogInputs.temp_deseada = Atmega::str2int(listaValores);

    }

#ifdef PID_TEMPERA
    if(nxcom.compararEqual(nxcom.comando,"TED"))
    {
        dts.op->request.resetPid = true;

        if (dts.op->digitalInputs.onquem && !dts.op->confirm.isTostion)
        {
            if (dts.op->isted()) 
            {
                if (dts.op->last.manual) dts.op->setManualmode();
                else if (dts.op->last.semi) dts.op->setSemimode();
                else if (dts.op->last.automatico) dts.op->setAutomode();
            }
            else dts.op->setTedmode();
        }
    }
#endif

    if(nxcom.compararEqual(nxcom.comando,"TARAR"))
    {
        dts.op->request.tarar_presiones = true;
    }

    NextionDR6::receive(dts);
}

void NextionDR6_PIDs::send_stack(const Datasets& dts)
{
    NextionDR6::send_stack(dts);

#ifdef PID_TEMPERA
    nxcom.send(0, PSTRr("visb5"), true);  // boton TED
    nxcom.send(0, PSTRr("visn11"), true); // valor TED
#endif
#ifndef PID_GAS
    nxcom.send(13, PSTRr("x0"), dts.stack->getD().KP * 10000.0);
    nxcom.send(13, PSTRr("x1"), dts.stack->getD().KI * 10000.0);
    nxcom.send(13, PSTRr("x2"), dts.stack->getD().KD * 10000.0);
#endif

}

void NextionDR6_PIDs::set_last_conf(Datasets& dts)
{
    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    char *listaValores = strtok(nxcom.comando,",");

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxRoR = (uint16_t)Atmega::str2int(listaValores);  // Valor maximo de RoR

    listaValores = strtok(NULL,",");
    dts.stack->getU16().suavidadBelimo = (uint16_t)Atmega::str2int(listaValores);  // Suavidad Belimo

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxTemperSinTambor = (uint16_t)Atmega::str2int(listaValores);  // Maxima temperatura sin tambor

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxTemperAire = (uint16_t)Atmega::str2int(listaValores);  // Maxima temperatura del aire

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxtemperGrano = (uint16_t)Atmega::str2int(listaValores);  // Maxima temperatura del grano

    listaValores = strtok(NULL,",");
    dts.stack->getD().KP = (double)(Atmega::str2int(listaValores)) / 10000.0;  // Kp pid temperatura

    listaValores = strtok(NULL,",");
    dts.stack->getD().KI = (double)(Atmega::str2int(listaValores)) / 10000.0;  // Ki pid temperatura

    listaValores = strtok(NULL,",");
    dts.stack->getD().KD = (double)(Atmega::str2int(listaValores)) / 10000.0;  // Kd pid temperatura

    dts.stack->save();

    if(dts.stack->getB().ingles) nxcom.send(13, PSTRr("t7"), PSTRr("Saved"));
    else nxcom.send(13, PSTRr("t7"), PSTRr("Guardado"));
    Atmega::retardo(2000);
    nxcom.send(13, PSTRr("t7"), PSTRr(""));
}

void NextionDR6_PIDs::check(const char* func, const char* entity)
{
    Atmega::printDebug(F("***NextionDR6_PIDs, "));
    Atmega::printDebug(func);
    Atmega::printDebug(F(": Puntero a "));
    Atmega::printDebug(entity);
    Atmega::printlnDebug(F(" no existe ***"));
}