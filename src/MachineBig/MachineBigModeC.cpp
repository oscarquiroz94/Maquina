#include "TED_DR6.h"

void TED_DR6::accion_potencia_quemador
(   Datasets& dts, 
    QuemadorBig& quemador,
    Calculos& calculo,
    ControlPresionGas* pid_gas)
{
    Atmega::printlnDebug(F("TED_DR6: accion potencia quemador"));

    quemador.set_potencia_controlada(dts, *pid_gas, dts.op->analogInputs.pQ);
}

void TED_DR6::accion_lectura_potencias
(   Datasets& dts,
    Potenciometro& pottambor,
    Potenciometro& potsopla,
    Potenciometro& potquem)
{
    // IOperacionSmall::accion_lectura_potencias(dts, pottambor);

    // Atmega::printlnDebug(F("TED_DR6: lectura potenciometros"));

    // dts.op->analogInputs.pS = potsopla.read();

    // //! No se lee potenciometro quemador en modo TED
    // Precalentador pretemp(dts.op->analogInputs.temp_deseada, dts.op->analogInputs.BT);
    // dts.op->analogInputs.pQ = pidtemp.regular(dts.op->analogInputs.BT, pretemp.get_setpoint());

    // Atmega::serial_print(" accion: ");Atmega::serial_print(dts.op->analogInputs.pQ);
    // Atmega::serial_print(" setpoint: ");Atmega::serial_print(pretemp.get_setpoint());
    // if (pidtemp.get_is_windup()) Atmega::serial_println("  windup");
    // else Atmega::serial_println();
}   