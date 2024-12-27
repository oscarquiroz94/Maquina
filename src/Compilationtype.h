//**********************************************************

// Build the atmega project environment
// just for deployment
//* $ pio run -e atmega

// Build the test project if you are
// testing o emulating
//* $ pio run -e test
//* execute .pio\build\test\program.exe

// SERIAL_PORT_NX "\\\\.\\COM15"
// SERIAL_PORT_GUI "\\\\.\\COM10"


//***********************************************************

//#define UNIT_TEST
//#define EMULATION
#define DEPLOY

//***********************************************************

#ifdef EMULATION
#define EXTERNAL_INTERFACES
#endif

#ifdef DEPLOY
//#define INTEGRATED_TEST
#endif
