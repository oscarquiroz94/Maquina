
#include "MachineBig.hpp"


#ifdef DEPLOY
#include <Wire.h>
#endif


MachineBig::MachineBig()
{

}
// RoasterDR6::RoasterDR6() :
//     //** adsX49 debe inicializarse antes de los sensores
//     adsX49(0x49),
//     quemador        (Pines::PIN_GAS, Pines::PIN_PWM_QUEM, Subsystem::QUEMBIG),
//     motorsoplador    (Pines::PIN_SOPLADOR,  Pines::PIN_PWM_SOPLA, Subsystem::MOTORSOPL),
//     motoragitador    (Pines::PIN_AGITADOR,  Subsystem::MOTORAGIT),
//     motorenfriador   (Pines::PIN_ENFRIADOR, Subsystem::MOTORENFR),
//     potenciomSoplador(Pines::PIN_POTENCIOM_SOPLAD, Subsystem::POTESOPL),
//     potenciomQuemador(Pines::PIN_POTENCIOM_QUEMAD, Subsystem::POTEQUEM)
//     , pin_isr1(Pines::PIN_ISR1, Subsystem::RSTESP)
// #if defined(BOARD_6_5) || defined(BOARD_6_0)
//     , sensEmergen      (ADS_MUX_P2, roaster.get_ads_instance().get_isr(), roaster.get_ads_instance(), 10, Subsystem::SENSORPARO)
//     , sensExhosto      (ADS_MUX_P2, ISR1, adsX49, 10, Subsystem::SENSOEXHOST)
// #elif defined(BOARD_7X)
//     , sensEmergen      (Pines::PIN_CONFIR_PARO, Subsystem::SENSORPARO, NEGADA)
// #if defined(BOARD_7_0)
//     //! Sin filtrar señal de confirmacion, este es Exhosto solamente
//     , sensExhosto      (ADS_MUX_P0, ISR1, adsX49, 1, Subsystem::SENSOEXHOST)
// #elif defined(BOARD_7_1)
//     , sensExhostoTambor (Pines::PIN_CONFIRM_EXHOSTAMBOR, Subsystem::SENSOEXHOST, POSITIVA)
// #endif
// #endif
// {
//     Atmega::printlnClass(F("RoasterDR6 instance created"));
//     Atmega::printlnClass(F("----------------------"));
    
//     potenciomQuemador.fraccionar(5.0);
// }

// void RoasterDR6::ejecutar(IOperacion* operacion)
// {
//     ManualDR6* tipoOperacion = (ManualDR6*)(operacion);

//     tipoOperacion->accion_lectura_temperaturas
//         (roaster.dts, 
//         roaster.sensTemperaAire, 
//         roaster.sensTemperaGrano, 
//         roaster.calculo);

//     tipoOperacion->accion_lectura_emergencia
//         (roaster.dts, 
//         sensEmergen);
    
//     tipoOperacion->accion_lectura_exhosto
//         (roaster.dts,
//         sensExhostoTambor);

//     tipoOperacion->accion_lectura_quemador
//         (roaster.dts,
//         quemador);

//     tipoOperacion->accion_lectura_motores
//         (roaster.dts,
//         motoragitador,
//         motorenfriador);

//     tipoOperacion->accion_lectura_potencias
//         (roaster.dts, 
//         roaster.potenciomTambor, 
//         potenciomSoplador, 
//         potenciomQuemador);

//     tipoOperacion->accion_trimmers(roaster.dts, roaster.motorTambor, motorsoplador, quemador);
//     tipoOperacion->accion_mapeo(roaster.dts, roaster.calculo);

//     tipoOperacion->accion_motores(roaster.dts, roaster.motorTambor, motorsoplador);
//     tipoOperacion->accion_potencias_motores(roaster.op, roaster.motorTambor, motorsoplador);

//     tipoOperacion->accion_quemador(roaster.dts, quemador, roaster.motorTambor);
//     tipoOperacion->accion_potencia_quemador(roaster.dts, quemador, roaster.calculo);

//     //En modo auto esta variable es leida antes de que se repita el ciclo
//     bool enfriardone = false;
//     tipoOperacion->accion_agitar(roaster.dts, motoragitador);
//     tipoOperacion->accion_enfriar(roaster.dts, motorenfriador, enfriardone);
// }

// void RoasterDR6::override_stack()
// {
//     Atmega::printlnDebug(F("RoasterDR6: override stack"));

//     roaster.override_stack();

//     roaster.stack.getU8().tipoMaquina = get_type();
//     roaster.stack.getU16().maxRoR             = 30;
//     roaster.stack.getU16().maxTemperSinTambor = 80;
//     roaster.stack.getU16().maxtemperGrano     = 220;
//     roaster.stack.getU16().maxTemperAire      = 250;
//     roaster.stack.getD().minrpm   =  40.0;    // mapeo para visualizar RPM min tambor
//     roaster.stack.getD().maxrpm   =  70.0;    // mapeo para visualizar RPM max tambor
//     roaster.stack.getD().minsopla =  40.0;    // mapeo visual
//     roaster.stack.getD().maxsopla = 100.0;    // mapeo visual
//     roaster.stack.getD().minquem  =   0.0;    // mapeo visual
//     roaster.stack.getD().maxquem  = 100.0;    // mapeo visual
//     roaster.stack.getD().minporcenttamb     =  52.0;   // porcentaje minimo en PWM tambor
//     roaster.stack.getD().maxporcenttamb     =  86.0;   // porcentaje maximo en PWM tambor
//     roaster.stack.getD().minporcentsopla    =  38.0;   // porcentaje minimo en soplador
//     roaster.stack.getD().maxporcentsopla    =  78.0;   // porcentaje maximo en soplador
//     roaster.stack.getD().minporcentquem     =  0.0;   // 25.0 porcentaje minimo en quemador
//     roaster.stack.getD().maxporcentquem     =  100.0;   // 75.0 porcentaje maximo en quemador
//     roaster.stack.getD().minporcentventquem =  35.0;  //porcentaje minimo ventilador quemador
//     roaster.stack.getD().maxporcentventquem = 100.0;  //porcentaje maximo ventilador quemador

//     roaster.stack.getU32().maxTiempoAgitar  = 1L * 60L * 1000L;  // 1 minuto
//     roaster.stack.getU32().maxTiempoEnfriar = 5L * 60L * 1000L;  // 5 minutos

//     roaster.stack.getU16().suavidadBelimo = 200;
    
//     //! Se llama desde afuera
//     //roaster.stack.save();
// }

// void RoasterDR6::initialize_pins()
// {
//     roaster.initialize_pins();

//     potenciomSoplador.getpin().pinmode(INPUT);
//     potenciomQuemador.getpin().pinmode(INPUT);
//     pin_isr1.pinmode(INPUT_PULLUP);
//     motorsoplador.getpin().pinmode(OUTPUT);
//     motoragitador.getpin().pinmode(OUTPUT);
//     motorenfriador.getpin().pinmode(OUTPUT);
//     sensEmergen.pinmode(INPUT_PULLUP);
//     sensExhostoTambor.pinmode(INPUT_PULLUP);
//     quemador.getpin().pinmode(OUTPUT);

//     Atmega::printlnDebug(F("RoasterDR6: Piness BigBoard initialized"));
// }

// void RoasterDR6::initialize_wire()
// {
// #ifdef DEPLOY
//     roaster.initialize_wire();
//     Wire.beginTransmission( 0x49 );
//     Atmega::serial_println(F("Transmitiendo a 0x49"));
//     Atmega::serial_print( F("Comprobando escritura: " ));
//     if ( Wire.write( 0 ) != 1 ) Atmega::serial_println( F("   error" ));
//     else Atmega::serial_println(F(" OK!"));
//     Atmega::serial_println( F("Fin de transmision" ));
//     switch ( Wire.endTransmission() ) {
//         case 0:  
//             Atmega::serial_println( F("ADS en 0x49 Funcionando!!" )); 
//             break;

//         case 1:  
//             Atmega::serial_println( F("ADS en 0x49 Longitud larga para el buffer" )); 
//             break;

//         case 2:  
//             Atmega::serial_println( F("ADS en 0x49 Direccion de envio, NACK recibido" )); 
//             break;

//         case 3:  
//             Atmega::serial_println( F("ADS en 0x49 Envio de datos, NACK recibido" )); 
//             break;

//         case 4:  
//             Atmega::serial_println( F("ADS en 0x49 Otro error TWI" )); 
//             break;

//         default: 
//             Atmega::serial_println( F("ADS en 0x49 Error inesperado" )); 
//             break;
//     }
//     Atmega::serial_println(F("-----------------------------"));
// #endif
// }

// void RoasterDR6::initialize_ADS()
// {
//     roaster.initialize_ADS();
    
// #ifdef DEPLOY
//     adsX49.initADS();
// #endif
//     Atmega::printlnDebug(F("RoasterDR6: ADS x49 initialized"));
// }

// Motor* RoasterDR6::get_motor_instance(const uint8_t id)
// {
//     if (id == Subsystem::MOTORTAMB) 
//     {
//         return &roaster.motorTambor;
//     }
//     else if (id == Subsystem::MOTORSOPL)
//     {
//         return &motorsoplador;
//     }
//     else
//     {
//         Atmega::serial_println(F("Motor no definido"));
//         return nullptr;
//     }   
// }

// Potenciometro* RoasterDR6::get_potenciom_instance(const uint8_t id)
// {
//     if (id == Subsystem::POTEQUEM)
//     {
//         return &potenciomQuemador;
//     }
//     else if (id == Subsystem::POTESOPL)
//     {
//         return &potenciomSoplador;
//     }
//     else if (id == Subsystem::POTETAMB)
//     {
//         return &roaster.potenciomTambor;
//     }
//     else
//     {
//         Atmega::serial_println(F("Potenciometro no definido"));
//         return nullptr;
//     }
// }

// bool RoasterDR6::isvalidclass(IOperacion* operacion)
// {
//     if( operacion->get_type() == System::CLASS_MANUAL_DR6 ||
//         operacion->get_type() == System::CLASS_ALARMA_DR6 ||
//         operacion->get_type() == System::CLASS_SEMI_DR6 )
//     return true;
//     else return false;
// }

// RoasterDR6::~RoasterDR6()
// {
//     Atmega::printlnClass(F("RoasterDR6 instance deleted"));
// }
