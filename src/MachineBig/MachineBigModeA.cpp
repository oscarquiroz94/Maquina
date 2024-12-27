#include "ManualDR6.h"

ManualDR6::ManualDR6() :
    tEnfriar(false, false, TemposNames::MANUAL_ENFRIAR),
    tAgitar(false, false, TemposNames::MANUAL_AGITAR)   
{
    Atmega::printlnClass(F("  ManualDR6 instance created"));
}

void ManualDR6::accion_lectura_emergencia
(   Datasets& dts,
    Entradas& sensEmergen)
{
    Atmega::printlnDebug(F("ManualDR6: lectura emergencia"));

#if defined(BOARD_6_5) || defined(BOARD_6_0)
    SensorAnalogico* sens = (SensorAnalogico*)&sensEmergen;
    dts.op->analogInputs.emergencia = sens->read(ptr_func_volt);

    if (dts.op->analogInputs.emergencia >= 1.5) 
        dts.op->confirm.isEmergencia = false;
    else    
        dts.op->confirm.isEmergencia = true;

#elif defined(BOARD_7X)
    SensorDigital* sens = (SensorDigital*)&sensEmergen;
    dts.op->confirm.isEmergencia = sens->read();
#endif
}
    
void ManualDR6::accion_lectura_exhosto
(   Datasets& dts,
    Entradas& sensExhostoTambor)
{
    Atmega::printlnDebug(F("ManualDR6: lectura exhosto"));

#if defined(BOARD_7_0)
    SensorAnalogico* sens = (SensorAnalogico*)&sensExhostoTambor;
    dts.op->analogInputs.motores = sens->read(ptr_func_volt);
#elif defined(BOARD_7_1)
    SensorDigital* sens = (SensorDigital*)&sensExhostoTambor;
    dts.op->analogInputs.motores = 4.0 * sens->read();
#endif

    //! La entrada corresponde a dos confirmaciones en serie
    if (dts.op->analogInputs.motores >= 1.5)
    {
        dts.op->confirm.isExhosto = true;
        dts.op->confirm.isTambor = true;
    }else
    {
        dts.op->confirm.isExhosto = false;
        dts.op->confirm.isTambor = false;
    }
}

void ManualDR6::accion_lectura_quemador
(   Datasets& dts,
    QuemadorBig& quemador)
{
    Atmega::printlnDebug(F("ManualDR6: lectura quemador"));

    //! Se lee el estado del pin
    dts.op->confirm.isQuemadorON = quemador.get_estado();
}

void ManualDR6::accion_lectura_motores
(   Datasets& dts,
    MotorFijo& motorAgitador,
    MotorFijo& motorEnfriar)
{
    Atmega::printlnDebug(F("ManualDR6: lectura motores"));

    dts.op->confirm.isAgitarON       = motorAgitador.get_estado();
    dts.op->confirm.isEnfriamientoON = motorEnfriar.get_estado();
}

//=======================================

void ManualDR6::accion_lectura_potencias
(   Datasets& dts,
    Potenciometro& pottambor,
    Potenciometro& potsopla,
    Potenciometro& potquem)
{
    IOperacionSmall::accion_lectura_potencias(dts, pottambor);

    Atmega::printlnDebug(F("ManualDR6: lectura potenciometros"));

    dts.op->analogInputs.pS = potsopla.read();

    if (dts.op->confirm.isTemperaOK)
    {
        dts.op->analogInputs.pQ = potquem.read();
    }else
    {
        dts.op->analogInputs.pQ = 0.0;
    }
    
}

//=======================================

void ManualDR6::accion_motores
(   Datasets& dts, 
    MotorVariable& motorTambor,
    MotorVariable& motorSoplador  )
{
    Atmega::printlnDebug(F("ManualDR6: accion motores"));

    //! Motores no deben encenderse automaticamente
    if (dts.op->digitalInputs.onrun)
    {
        motorTambor.set_estado(true);
        motorSoplador.set_estado(true);

    }else if (!dts.op->digitalInputs.onrun && 
            dts.op->analogInputs.BT < dts.stack->getU16().maxTemperSinTambor &&
            dts.op->analogInputs.ET < dts.stack->getU16().maxTemperSinTambor)
    {
        motorTambor.set_estado(false);
        motorSoplador.set_estado(false);
    }
}

//=======================================

void ManualDR6::accion_trimmers
(   Datasets& dts, 
    MotorVariable& motorTambor,
    MotorVariable& motorSoplador,
    QuemadorBig& quemador  )
{
    IOperacionSmall::accion_trimmers(dts, motorTambor);

    Atmega::printlnDebug(F("ManualDR6: accion trimmers"));
    
    motorSoplador.set_min_porcent(dts.stack->getD().minporcentsopla);
    motorSoplador.set_max_porcent(dts.stack->getD().maxporcentsopla);
    quemador.setMinPorcent(dts.stack->getD().minporcentquem);
    quemador.setMaxPorcent(dts.stack->getD().maxporcentquem);
}

//=========================================

void ManualDR6::accion_quemador
(   Datasets& dts, 
    QuemadorBig& quemador, 
    MotorVariable& motortambor)
{
    Atmega::printlnDebug(F("ManualDR6: accion quemador"));

    //! Quemador se deshabilita inmediatamente al apagar tambor 20/10/2023
    // Se esta leyendo el estado del pin, no la confirmacion
    if (!motortambor.get_estado())
    {
        dts.op->digitalInputs.onquem = false;
    } 

    if(dts.op->digitalInputs.onquem && dts.op->confirm.isExhosto && dts.op->confirm.isTemperaOK) 
    {
        quemador.set_estado(true);
    }
    else 
    {
        quemador.set_estado(false);
    }

}

//==========================================

void ManualDR6::accion_potencia_quemador
(   Datasets& dts, 
    QuemadorBig& quemador,
    Calculos& calc,
    ControlPresionGas* pid_gas)
{
    Atmega::printlnDebug(F("ManualDR6: accion potencia quemador"));

    // Valores potenciometro en dts son suavizados por calc
    quemador.set_potencia_suavizada(dts, calc);

}

//==========================================

void ManualDR6::accion_potencias_motores
(   Operativos& op, 
    MotorVariable& motorTambor,
    MotorVariable& motorSoplador  )
{
    IOperacionSmall::accion_potencias(op, motorTambor);

    Atmega::printlnDebug(F("ManualDR6: accion potencia motores"));

    motorSoplador.set_potencia(op.analogInputs.pS);
}

//===========================================

void ManualDR6::accion_mapeo(Datasets& dts, Calculos& calculo)
{
    IOperacionSmall::accion_mapeo(dts, calculo);

    Atmega::printlnDebug(F("ManualDR6: accion mapeo"));

    dts.op->mapInputs.pS = calculo.map_double
        (dts.op->analogInputs.pS, 
            0.0, 
            100.0, 
            dts.stack->getD().minsopla, 
            dts.stack->getD().maxsopla);

    dts.op->mapInputs.pQ = calculo.map_double
        (dts.op->analogInputs.pQ, 
            0.0, 
            100.0, 
            dts.stack->getD().minquem,  
            dts.stack->getD().maxquem);
}

//============================================

void ManualDR6::accion_agitar(Datasets& dts, MotorFijo& motorAgitador)
{
    Atmega::printlnDebug(F("ManualDR6: accion agitar"));
    
    motorAgitador.set_estado_temporizado(
        dts.op->digitalInputs.onagitar, 
        dts.stack->getU32().maxTiempoAgitar);
}

void ManualDR6::accion_enfriar(Datasets& dts, MotorFijo& motorEnfriar, bool& enfriardone)
{
    Atmega::printlnDebug(F("ManualDR6: accion enfriar"));

    motorEnfriar.set_estado_temporizado(
        dts.op->digitalInputs.onenfriamiento,
        dts.stack->getU32().maxTiempoEnfriar);
        
    enfriardone = !motorEnfriar.get_istimer_running();
}