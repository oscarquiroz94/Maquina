#include "ESP_DR6.h"

void ESP_DR6::send(const Operativos& op)
{
    if( tsend.tiempo(200) ){

        //Aqui se llena el buffer, retorno no se usa
        buff_tambor.AddValue(op.mapInputs.pT);
        buff_soplador.AddValue(op.mapInputs.pS);
        buff_quemador.AddValue(op.mapInputs.pQ);

        Atmega::printDebug("TO PC -> ");

#ifdef DEPLOY
        String cadena =  "IN," + String((int)(op.analogInputs.ET * 10.0)) +
                                "," + String((int)(op.analogInputs.BT * 10.0)) +
                                "," + String((int)(get_value_no_change(buff_quemador))) +
                                "," + String((int)(get_value_no_change(buff_tambor))) +
                                "," + String((int)(get_value_no_change(buff_soplador))) +
                                "," + String((int)(op.rorBT.mean * 10.0)) +
                                "," + String((int)(diff_temp * 10.0)) + "\0";

#else
        std::string cadena =  "IN," + std::to_string((int)(op.analogInputs.ET * 10.0)) +
                                "," + std::to_string((int)(op.analogInputs.BT * 10.0)) +
                                "," + std::to_string((int)(get_value_no_change(buff_quemador))) +
                                "," + std::to_string((int)(get_value_no_change(buff_tambor))) +
                                "," + std::to_string((int)(get_value_no_change(buff_soplador))) +
                                "," + std::to_string((int)(op.rorBT.mean * 10.0)) +
                                "," + std::to_string((int)(diff_temp * 10.0)) + "\0";
        
#endif

        //! Si no funciona serial3_println, poner sin ln
        Atmega::serial3_println(&cadena[0]);
        //Atmega::serial_println();
    }
}

void ESP_DR6::receive(Datasets& dts)
{
    Atmega::printlnDebug(F("ESP_DR6: receive"));

    if (!dts.op) {Atmega::printlnDebug(F("***ESP_DR6: Puntero a dts.op no existe ***")); return;}

    if(pccom.comparar(pccom.comando,"PARAM"))
    {
        char *listaValores = strtok(pccom.comando,",");
        double value;

        listaValores = strtok(NULL,",");
        value = (double)Atmega::str2int(listaValores); // Porcentaje aire
        dts.op->analogInputs.buf_pS = (value < dts.stack->getD().minsopla) ? dts.stack->getD().minsopla : 100 * (value - dts.stack->getD().minsopla)/(dts.stack->getD().maxsopla - dts.stack->getD().minsopla);

        listaValores = strtok(NULL,",");
        value = (double)Atmega::str2int(listaValores);  // Porcentaje tambor
        dts.op->analogInputs.buf_pT = (value < dts.stack->getD().minrpm) ? dts.stack->getD().minrpm : 100 * (value - dts.stack->getD().minrpm)/(dts.stack->getD().maxrpm - dts.stack->getD().minrpm);

        listaValores = strtok(NULL,",");
        value = (double)Atmega::str2int(listaValores);  // Porcentaje quemador
        dts.op->analogInputs.buf_pQ = (value < dts.stack->getD().minquem) ? dts.stack->getD().minquem : 100 * (value - dts.stack->getD().minquem)/(dts.stack->getD().maxquem - dts.stack->getD().minquem);

        listaValores = strtok(NULL,",");
        dts.op->analogInputs.temp_deseada = (double)Atmega::str2int(listaValores);  // Valor TED

        if (dts.op->analogInputs.temp_deseada < 120.0) 
            dts.op->analogInputs.temp_deseada = 120.0;
        if (dts.op->analogInputs.temp_deseada > dts.stack->getU16().maxtemperGrano - 10) 
            dts.op->analogInputs.temp_deseada = dts.stack->getU16().maxtemperGrano - 10;
    }


#ifdef PID_TEMPERA
    if(pccom.compararEqual(pccom.comando, "ONTED"))
    {
        dts.op->request.resetPid = true;

        if (dts.op->digitalInputs.onquem && 
            !dts.op->confirm.isTostion && 
            !dts.op->isted() &&
            !dts.op->ismanual())
        {
            dts.op->setTedmode();
        }
    }

    if(pccom.compararEqual(pccom.comando, "OFFTED"))
    {
        if (dts.op->isted() && !dts.op->last.manual)
        {
            dts.op->request.resetPid = true;
            if (dts.op->last.manual) dts.op->setManualmode();
            else if (dts.op->last.semi) dts.op->setSemimode();
            else if (dts.op->last.automatico) dts.op->setAutomode();
        }
        
    }
#endif

    //* Aqui se hace memset
    ESP_DR1::receive(dts);
}

