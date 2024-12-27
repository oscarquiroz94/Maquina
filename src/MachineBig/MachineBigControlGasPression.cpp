#include "ControlPresionGasDR6.h"



double ControlPresionGasDR6::regular(Operativos& op)
{
    // ToDo: la maquina podria calibrar maxpresion llevando belimo a maximo 
    // Convertir de porcentaje a presion
    presion_deseada = maxpresion * (porcent_pres_deseada / 100.0);

    error = presion_deseada - op.analogInputs.pres_gas_out;

    accion = pid.calcular_accion_control(error);

    //! Simulador de planta (temporal)
    //op.analogInputs.pres_gas_out = emuladorvalvula.emular(accion);

    double output = Calculos::map_double(accion, 0.0, 100.0, 0.0, 100.0);
    if(output > 100.0) output = 100.0;
    if(output < 0.0) output = 0.0;

    //Retorna potencia de 0 a 100%
    return output;
}