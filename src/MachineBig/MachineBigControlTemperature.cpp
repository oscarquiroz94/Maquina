#include "ControlTemperaturaDR6.h"

double ControlTemperaturaDR6::regular(double temp_actual, double temp_deseada)
{
    double error = temp_deseada - temp_actual;
    double accion = pid.calcular_accion_control(error);
    Atmega::serial_print("accionpura: ");Atmega::serial_print(accion);

    accion = 5.0 * (int)(accion / 5.0);
    if (accion > 100.0) accion = 100.0;
    if (accion < 0.0) accion = 0.0;

    return accion;
}