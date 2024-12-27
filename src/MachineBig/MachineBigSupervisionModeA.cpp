#include "SupervisionDR6.h"

SupervisionDR6::SupervisionDR6() :
    alerta_emergencia(AlertasNames::EMERGENCY),
    alerta_nomotores(AlertasNames::NO_MOTORES),
    alerta_maxtemper(AlertasNames::MAX_TEMP),
    alerta_nosensor(AlertasNames::NO_SENSOR),
    alerta_maxror(AlertasNames::MAX_ROR),
    alerta_noagit(AlertasNames::NO_AGIT),
    alerta_noenfr(AlertasNames::NO_ENFR),
    tNoMotores(false, false, TemposNames::SUPER_MOTORES),
    tNoAgitador(false, false, TemposNames::SUPER_AGITADOR),
    tNoEnfriador(false, false, TemposNames::SUPER_ENFRIADOR)
{
    Atmega::printlnClass(F(" SupervisionDR6: instance created"));
}

void SupervisionDR6::configure(IRoaster& roaster)
{
    ISupervision::configure(roaster);
}

void SupervisionDR6::supervisar(IRoaster& roaster)
{
    verify_temperatura(roaster);
    verify_sensor_tempera(roaster);
    verify_rate_of_rise(roaster);
    verify_emergencia(roaster);
    verify_motores(roaster);
    verify_agitador(roaster);
    verify_enfriador(roaster);
    verify_descarga(roaster);
    verify_quemador(roaster);
    verify_alertas(roaster);

    ISupervision::verify_alertas(roaster);
}

void SupervisionDR6::verify_temperatura(IRoaster& roaster)
{
    Atmega::printlnDebug(F("SupervisionDR6: verify temperatura"));

    Datasets& dts = roaster.get_datasets();
    if (dts.stack == nullptr) {check(__func__, "dts.stack"); return;}
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}

    double temp_bt = dts.op->analogInputs.BT;
    double temp_max_bt = static_cast<double>(dts.stack->getU16().maxtemperGrano);
    double temp_et = dts.op->analogInputs.ET;
    double temp_max_et = static_cast<double>(dts.stack->getU16().maxTemperAire);

    if (temp_bt > temp_max_bt || temp_et > temp_max_et)
    {
        gestoralertas.agregar(alerta_maxtemper);
        dts.op->confirm.isTemperaOK = false;

    }else if (temp_bt < temp_max_bt - 10.0 && temp_et < temp_max_et - 10.0)
    {
        gestoralertas.eliminar(alerta_maxtemper);
        dts.op->confirm.isTemperaOK = true;
    }
}

void SupervisionDR6::verify_quemador(IRoaster& roaster)
{
    Atmega::printlnDebug(F("SupervisionDR6: verify quemador"));

    INextion* nx = roaster.get_nextion_instance(roaster.get_type());
    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}
    if (nx == nullptr) {check(__func__, "nx"); return;}

    if (dts.op->confirm.isQuemadorON)
    {
        nx->nxcom.send(9, PSTRr("quemad"), true);
    }
    else
    {
        nx->nxcom.send(9, PSTRr("quemad"), false);
    }
}

void SupervisionDR6::verify_sensor_tempera(IRoaster& roaster)
{
    Atmega::printlnDebug(F("SupervisionDR6: verify sensor temperatura"));

    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}

    if (dts.op->analogInputs.ET <= 0.0 || 
        dts.op->analogInputs.BT <= 0.0)
    {
        gestoralertas.agregar(alerta_nosensor);
        //! Agregado 24/05/03, tempera no es ok si falla sensor
        dts.op->confirm.isTemperaOK = false;
    }
    else if(dts.op->analogInputs.ET > 0.0 && 
            dts.op->analogInputs.BT > 0.0)
    {
        gestoralertas.eliminar(alerta_nosensor);
    }
}

void SupervisionDR6::verify_rate_of_rise(IRoaster& roaster)
{
    Atmega::printlnDebug(F("SupervisionDR6: verify rate of rise"));
    QuemadorBig* quemador = (QuemadorBig*)roaster.get_quemador_instance();
    Potenciometro* potquem = roaster.get_potenciom_instance(Subsystem::POTEQUEM);
    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}
    if (dts.stack == nullptr) {check(__func__, "dts.stack"); return;}
    if (quemador == nullptr) {check(__func__, "quemador"); return;}
    if (potquem == nullptr) {check(__func__, "portquem"); return;}

    if (dts.op->rorBT.mean > dts.stack->getU16().maxRoR)
    {
        quemador->set_minimo(true);
        potquem->disable();
        gestoralertas.agregar(alerta_maxror);

    }else if (dts.op->rorBT.mean < dts.stack->getU16().maxRoR - 1.0)
    {
        quemador->set_minimo(false);
        potquem->enable();
        gestoralertas.eliminar(alerta_maxror);
    }
}

void SupervisionDR6::verify_emergencia(IRoaster& roaster)
{
    Atmega::printlnDebug(F("SupervisionDR6: verify emergencia"));

    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}

    if(dts.op->confirm.isEmergencia) 
    {
        gestoralertas.agregar(alerta_emergencia);
    }else
    {
        gestoralertas.eliminar(alerta_emergencia);
    }
}

void SupervisionDR6::verify_agitador(IRoaster& roaster)
{
    INextion* nx = roaster.get_nextion_instance(roaster.get_type());
    NextionDR6* nxdr6 = (NextionDR6*)nx;
    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}
    if (nx == nullptr) {check(__func__, "nx"); return;}
    if (nxdr6 == nullptr) {check(__func__, "nxdr6"); return;}

    Atmega::printlnDebug(F("SupervisionDR6: verify agitador"));

    if (dts.op->confirm.isAgitarON)
    {
        nx->nxcom.send(9, PSTRr("agitar"), true);
    }else
    {
        nx->nxcom.send(9, PSTRr("agitar"), false);
    }

    //-------------------

    if (dts.op->digitalInputs.onagitar)
    {
        tNoAgitador.alive();
    }else
    {
        tNoAgitador.kill();
    }

    //--------------------

    if (dts.op->digitalInputs.onagitar &&
        !dts.op->confirm.isAgitarON    &&
        nxdr6->get_enable_superagit()     &&
        tNoAgitador.tiempo(5000))
    {
        gestoralertas.agregar(alerta_noagit);
    }

    //! 15062024: si no habia orden de encender no confirmaba
    if(!dts.op->digitalInputs.onagitar ||
        dts.op->confirm.isAgitarON)
    {
        gestoralertas.eliminar(alerta_noagit);
    }
}

void SupervisionDR6::verify_enfriador(IRoaster& roaster)
{
    INextion* nx = roaster.get_nextion_instance(roaster.get_type());
    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}
    if (nx == nullptr) {check(__func__, "nx"); return;}

    Atmega::printlnDebug(F("SupervisionDR6: verify enfriador"));

    if(dts.op->confirm.isEnfriamientoON)
    {
        nx->nxcom.send(9, PSTRr("enfriar"), true);
    }else
    {
        nx->nxcom.send(9, PSTRr("enfriar"), false);
    }

    //-------------------

    if (dts.op->digitalInputs.onenfriamiento)
    {
        tNoEnfriador.alive();
    }else
    {
        tNoEnfriador.kill();
    }

    //--------------------

    if (dts.op->digitalInputs.onenfriamiento &&
        !dts.op->confirm.isEnfriamientoON    &&
        tNoEnfriador.tiempo(5000))
    {
        gestoralertas.agregar(alerta_noenfr);
    }
    
    //! 15062024: si no habia orden de encender no confirmaba
    if(!dts.op->digitalInputs.onenfriamiento ||
        dts.op->confirm.isEnfriamientoON)
    {
        gestoralertas.eliminar(alerta_noenfr);
    }
}

void SupervisionDR6::verify_descarga(IRoaster& roaster)
{
    INextion* nx = roaster.get_nextion_instance(roaster.get_type());
    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}
    if (nx == nullptr) {check(__func__, "nx"); return;}

    Atmega::printlnDebug(F("SupervisionDR6: verify descarga"));

    if (dts.op->confirm.isEnfriamientoON &&
        dts.op->confirm.isAgitarON)
    {
        nx->nxcom.send(9, PSTRr("descargar"), true);
    }else if (!dts.op->confirm.isEnfriamientoON &&
            !dts.op->confirm.isAgitarON)
    {
        nx->nxcom.send(9, PSTRr("descargar"), false);
    }
}

void SupervisionDR6::verify_motores(IRoaster& roaster)
{
    INextion* nx = roaster.get_nextion_instance(roaster.get_type());
    Datasets& dts = roaster.get_datasets();
    if (dts.op == nullptr) {check(__func__, "dts.op"); return;}
    if (nx == nullptr) {check(__func__, "nx"); return;}

    Atmega::printlnDebug(F("SupervisionDR6: verify motores"));

    if (dts.op->confirm.isTambor && dts.op->confirm.isExhosto)
    {
        nx->nxcom.send(9, PSTRr("run"), true);
    }else
    {
        nx->nxcom.send(9, PSTRr("run"), false);
    }

    //-----------------

    if (dts.op->digitalInputs.onrun)
    {
        tNoMotores.alive();
    }else
    {
        tNoMotores.kill();
    }

    //-----------------

    if ( dts.op->digitalInputs.onrun &&
        (!dts.op->confirm.isTambor || !dts.op->confirm.isExhosto) &&
        tNoMotores.tiempo(5000))
    {
        gestoralertas.agregar(alerta_nomotores);
    }

    //! 15062024: si no habia orden de encender no confirmaba
    if(!dts.op->digitalInputs.onrun ||
        (dts.op->confirm.isTambor && dts.op->confirm.isExhosto))
    {
        gestoralertas.eliminar(alerta_nomotores);
    }
}

void SupervisionDR6::verify_alertas(IRoaster& roaster)
{
    Atmega::printlnDebug(F("SupervisionDR6: verify alertas"));
}

void SupervisionDR6::check(const char* func, const char* entity)
{
    Atmega::printDebug(F("***SupervisionDR6, "));
    Atmega::printDebug(func);
    Atmega::printDebug(F(": Puntero a "));
    Atmega::printDebug(entity);
    Atmega::printlnDebug(F(" no existe ***"));
}