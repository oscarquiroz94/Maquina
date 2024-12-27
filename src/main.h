
//**************** NOT INCLUDE ANY HEADER  ******************

#ifndef __MAIN
#define __MAIN

#include "Compilationtype.h"

/*
 * Uncomment the Roaster to use
 */
// #define ROASTER_DRM
// #define ROASTER_DR1
//#define ROASTER_DR1_digital
#define ROASTER_DR3
//#define ROASTER_DR3_digital
//#define ROASTER_DR6
// #define ROASTER_DR6_confirmMotores
//#define ROASTER_DR12
// #define ROASTER_DR12_perillas
//#define ROASTER_DR25
// #define ROASTER_DR25_perillas // inicio y quemador
// #define ROASTER_DR40
// #define ROASTER_DR70


/*
 * Uncomment the PCB version
 */
// #define BOARD_6_0
// #define BOARD_6_5
#define BOARD_7X
    //#define BOARD_7_0
    #define BOARD_7_1


/*
 * Uncomment the mode enabled
 * Only applies if the machine is DR6 or later
 */
//#define MAQUINA_AUTO    //Replicacion y control neumatico
//#define MAQUINA_SEMI // Solo replicacion de curvas

/*
 * Uncomment the peripheric device used
 */
// #define CARGADOR_NEUMATICO
//#define CARGADOR_DE_VACIO
//#define DESPEDREGADOR
//#define NO_PISTONES_TOLVCOMP

/*
 * Uncomment for PID controller
 */
//#define PID_GAS  // Maquinas digitales
//#define PID_TEMPERA // TED

/*
 * Behaviour
 */
// #define DISABLE_SECURITIES
// #define PORTATIL_USB    //Cropster o Artisan USB

/*
 * Debug variable options written on the console
 * Those can be initialized at Atmega.h
 */
// LOGNULL // Ocultar trazas 
// LOG1    // Mostrar trazas Debug
// LOG2    // Mostrar trazas pines OUT
// LOG3    // Mostrar trazas pines IN
// LOG4    // Mostrar valores operativos
// LOG5    // Mostrar trazas Nextion
// LOG6    // Mostrar trazas websocket

// #define MODOSD         // Habilitar el almacenamiento SD
// #define SHOWCLASES     // Mostrar trazas create delete
// #define SHOW_ANALOG_IN
// #define SHOW_PWM        // Mostrar trazas de pines PWM



//! **********************  DANGEROUS!!  **********************
//! ***************  TOMAR FOTO CONFIGURACION  ****************

//#define OVERRIDE // Override stack

//! ***********************************************************



//************* CONSTANT DEFINITIONS ****************
//*************     DO NOT TOUCH     ****************


#ifdef DEPLOY
#define ADS_MUX_P0 ADS1115_MUX_P0_NG
#define ADS_MUX_P1 ADS1115_MUX_P1_NG
#define ADS_MUX_P2 ADS1115_MUX_P2_NG
#define ADS_MUX_P3 ADS1115_MUX_P3_NG
#define PSTRr(s) PSTR(s)
#define PROGMEMr //! No se necesita por ahora
#else
#define ADS_MUX_P0 0x00
#define ADS_MUX_P1 0x01
#define ADS_MUX_P2 0x02
#define ADS_MUX_P3 0x03
#define PROGMEMr
#define PSTRr(s) s
#endif

#define ISR1 2
#define ISR2 3

#define ROJA 0
#define AMARRILLA 1
#define NINGUNA 2

enum Pines
{
#if defined(BOARD_6_0)
    PIN_PWM_SOPLA = 9,
    PIN_PWM_QUEM = 8,
    PIN_POTENCIOM_SOPLAD = 54,
    PIN_POTENCIOM_TAMBOR = 55,
    PIN_POTENCIOM_QUEMAD = 56,
    PIN_GAS = 30,
    PIN_AGITADOR = 31,
    PIN_ENFRIADOR = 32,
    PIN_TOLVA = 33,
    PIN_COMPUERTA = 47,
    PIN_PWM_AIRCOMBUS = 6,
    PIN_PWM_TAMB = 7,
    PIN_SOPLADOR = 49,
    PIN_TAMBOR = 48,
    PIN_INICIO = 29,
    PIN_CONFIR_TOLVA = 26,
    PIN_CONFIR_COMPU = 28,
    PIN_CONFIR_AGIT = 27,

    // No estan definidas para 6_0, pero DR70 requiere inicializarlas
    PIN_BUZZER = 0,
    PIN_ASPER = 0,
    PIN_CO2 = 0,
    PIN_INCEND = 0,
    PIN_CARGADOR = 0,
    PIN_DESPEDREG = 0,
    PIN_CONFIR_DTA = 0,
    PIN_CONFIR_DTB = 0,
    PIN_CONFIR_CTA = 0,
    PIN_CONFIR_CTB = 0,
    PIN_COMP_ENFR = 0,
    PIN_FALLA_EXHOST = 0,
    PIN_FALLA_TAMBOR = 0,
    PIN_FALLA_ENFR = 0,
    PIN_CONFIR_ENFR = 0,

#elif defined(BOARD_6_5)
    PIN_PWM_SOPLA = 9,
    PIN_PWM_QUEM = 8,
    PIN_POTENCIOM_SOPLAD = 54,
    PIN_POTENCIOM_TAMBOR = 55,
    PIN_POTENCIOM_QUEMAD = 56,
    PIN_GAS = 30,
    PIN_AGITADOR = 31,
    PIN_ENFRIADOR = 32,
    PIN_TOLVA = 33,
    PIN_COMPUERTA = 34,
    PIN_COMP_ENFR = 37,
    PIN_PWM_AIRCOMBUS = 6,
    PIN_PWM_TAMB = 7,
    PIN_SOPLADOR = 49,
    PIN_TAMBOR = 48,
    PIN_INICIO = 29,

    // Mismo pin diferente proposito en diferente maquina
    PIN_CONFIR_TOLVA = 46,
    PIN_FALLA_EXHOST = 46,
    PIN_FALLA_TAMBOR = 22,
    PIN_CONFIR_COMPU = 22,
    PIN_CONFIR_AGIT = 23,
    PIN_FALLA_ENFR = 25,
    PIN_CONFIR_ENFR = 25,

    // No estan definidas para 6_5, pero DR70 requiere inicializarlas
    PIN_BUZZER = 0,
    PIN_ASPER = 0,
    PIN_CO2 = 0,
    PIN_INCEND = 0,
    PIN_CARGADOR = 0,
    PIN_DESPEDREG = 0,
    PIN_CONFIR_DTA = 0,
    PIN_CONFIR_DTB = 0,
    PIN_CONFIR_CTA = 0,
    PIN_CONFIR_CTB = 0,

#elif defined(BOARD_7X)
    PIN_PWM_SOPLA = 9,
    PIN_PWM_QUEM = 8,
    PIN_POTENCIOM_SOPLAD = 54,
    PIN_POTENCIOM_TAMBOR = 55,
    PIN_POTENCIOM_QUEMAD = 56,
    PIN_GAS = 30,
    PIN_AGITADOR = 31,
    PIN_ENFRIADOR = 32,
    PIN_TOLVA = 33,
    PIN_COMPUERTA = 34,
    PIN_CARGADOR = 35,
    PIN_DESPEDREG = 36,
    PIN_COMP_ENFR = 37,
    PIN_CO2 = 43,
    PIN_PWM_AIRCOMBUS = 6,
    PIN_PWM_TAMB = 7,
    PIN_SOPLADOR = 49,
    PIN_TAMBOR = 48,
    PIN_ASPER = 47,
    PIN_BUZZER = 39,
    PIN_CONFIR_PARO = 44,
    PIN_CONFIR_COMP_ENFR = 41,
    PIN_INCEND = 40,
    PIN_CONFIR_TOLVA = 46,
    PIN_CONFIR_COMPU = 22,
    PIN_FALLA_TAMBOR = 22,
    PIN_CONFIR_AGIT = 23,
    PIN_CONFIR_ENFR = 25,
    PIN_FALLA_ENFR = 25,
    PIN_CONFIR_CTB = 26,
    PIN_CONFIR_CTA = 28,
    PIN_CONFIR_DTB = 27,
    PIN_CONFIR_DTA = 45,
    PIN_FALLA_EXHOST = 46,

#if defined(BOARD_7_1)
    PIN_CONFIRM_EXHOSTAMBOR = 69,
#endif
#endif

    PIN_RST_ESP = 24,
    PIN_CHIP_SD = 53,
    PIN_ISR2 = 3,
    PIN_ISR1 = 2
};

enum System
{
    CLASS_ROASTER,
    CLASS_NULL,
    CLASS_DRM,
    CLASS_DR1,
    CLASS_DR1_DIG,
    CLASS_DR3,
    CLASS_DR3_DIG,
    CLASS_DR6,
    CLASS_DR12,
    CLASS_DR12_PERILLAS,
    CLASS_DR25,
    CLASS_DR25_PERILLAS,
    CLASS_DR40,
    CLASS_DR70,
    CLASS_SMALL,
    CLASS_BIG,
    CLASS_BIGAUTO,
    CLASS_MANUAL_DRM,
    CLASS_MANUAL_DR1,
    CLASS_MANUAL_DR1_DIG,
    CLASS_MANUAL_DR3,
    CLASS_MANUAL_DR3_DIG,
    CLASS_MANUAL_DR6,
    CLASS_MANUAL_DR6_CM,
    CLASS_MANUAL_DR12,
    CLASS_MANUAL_DR12_PERILLAS,
    CLASS_MANUAL_DR25,
    CLASS_MANUAL_DR25_PERILLAS,
    CLASS_MANUAL_DR70,
    CLASS_MANUAL_SMALL,
    CLASS_SEMI_DR6,
    CLASS_SEMI_DR12,
    CLASS_SEMI_DR25,
    CLASS_SEMI_DR70,
    CLASS_TED_DR1,
    CLASS_TED_DR6,
    CLASS_AUTO_DR25,
    CLASS_AUTO_DR25_PERILLAS,
    CLASS_AUTO_DR70,
    CLASS_ALARMA_DR1,
    CLASS_ALARMA_DR3,
    CLASS_ALARMA_DR6,
    CLASS_ALARMA_DR12,
    CLASS_ALARMA_DR25,
    CLASS_ALARMA_DR70,
    CLASS_INCENDIO,
    CLASS_ALARMA_SMALL,
    CLASS_SUPER_SMALL,
    CLASS_SUPER_BIG,
    CLASS_NX_DRM,
    CLASS_NX_DR1,
    CLASS_NX_DR6,
    CLASS_NX_DR12,
    CLASS_NX_DR25,
    CLASS_NX_DR70
};

enum Subsystem
{
    NULLPIN,

    // Sensores analogicos
    SENSORAIRE,
    SENSOEXHOST,
    SENSCOMPENFR,
    SENSORPARO,
    SENSORCORRAGIT,
    SENSORPREAIRE1,
    SENSORPREAIRE2,
    SENSORPREGASIN,
    SENSORPREGASOUT,
    SENSORPRECO2,

    // Sensores digitales
    SENSORGRANO,
    SENSOTAMBOR,
    SENSORAGIT,
    SENSORENFR,
    SENSORCTA,
    SENSORCTB,
    SENSORDTA,
    SENSORDTB,
    SENSORTOLVA,
    SENSORCOMP,
    SENSORINCEN,
    SENSORINICIO,

    // Quemadores
    QUEMSMALL,
    QUEMBIG,

    // Motores
    MOTORTAMB,
    MOTORBKTAMB,
    MOTORSOPL,
    MOTORAGIT,
    MOTORENFR,
    MOTORVENTQUEM,
    MOTORCARG,
    MOTORDESP,

    // Potenciometros
    POTETAMB,
    POTESOPL,
    POTEQUEM,

    // Actuadores
    ACTTOLVA,
    ACTCOMP,
    ACTCOMPENFR,

    // Salidas
    RSTESP,
    MEMOSD,
    ASPER,
    BUZZER,
    CO2,

    ARTISANUSB,
    CROPSTERUSB,
    PORTATIL,
    NEXTION,
    DEBUG
};

enum AlertasNames
{
    SETUP = 1,
    EMERGENCY = 2,
    MAX_AGIT = 3,
    MAX_TEMP = 4,
    NO_SENSOR = 5,
    NO_MOTORES = 6,
    MAX_ROR = 7,
    NO_AGIT = 8,
    NO_ENFR = 9,
    NO_TBC = 10, // TBC Tolva baja cargador
    TAC = 11,
    NO_TBD = 12, // TBD Tolva Baja Desped
    TAD = 13,
    ARO = 14,
    INCENDIO = 15,
    BAJA_PRES = 16,
    NO_SP_PRES = 17,
    NO_SP_TEMP = 18
};

enum TemposNames
{
    PRESION_TC,
    PRESION_AWP,
    MANUAL_AGITAR,
    MANUAL_ENFRIAR,
    TOLVA,
    COMPUERTA,
    CARGADOR,
    DESPEDREGAD,
    DELAYDESPED,
    SUPER_MOTORES,
    SUPER_AGITADOR,
    SUPER_ENFRIADOR,
    MOTOR_DOWN,
    ARTISAN_SEND,
    CALC_ROR,
    MEMO_SAVE,
    OPERAT_SHOW,
    QUEM_DEVPQ,
    NEXTION_SHOWMSG,
    MESURE_CORR,
    ASPERSAR,
    VALVULACO2
};

#endif