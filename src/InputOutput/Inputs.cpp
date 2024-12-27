#include "Entradas.h"


SensorAnalogico::SensorAnalogico 
(   const uint8_t mplex, 
    const uint8_t pin, 
    ADS1115& chip, 
    const uint8_t window, 
    const uint8_t name
) : 
    Entradas(pin, name),
    ads(&chip), 
    pinready(pin), 
    multiplex(mplex), 
    meanFilter(window)
{
    Atmega::printClass(F(" SensorAnalogico: "));
    Atmega::printClass(_name);
    Atmega::printClass(F(", chip:"));
    Atmega::printClass((uint16_t)ads->getAddress());
    Atmega::printClass(F(", pinready:"));
    Atmega::printClass(pinready);
    Atmega::printClass(F(", window:"));
    Atmega::printClass(window);
    Atmega::printClass(F(" created"));
    Atmega::printlnClass(F(""));
}

double SensorAnalogico::read(double (*ptr_func)(double)){
    double adc_BT = 0.0, conversion = 0.0;
#ifdef DEPLOY
    ads->setMultiplexer(multiplex);
    ads->triggerConversion();
    if(pollAlertReadyPin()){
      adc_BT = (double)ads->getConversion(false);  // False para esperar fin de conversion por el pin alert
    } 
    conversion = ptr_func(adc_BT) - offset;
#endif
    return interfaz.read(_name, meanFilter, conversion) - offset;
    
}


bool SensorAnalogico::pollAlertReadyPin(){
#ifdef DEPLOY
    for (uint32_t i = 0; i<100000; i++){
        if (!Atmega::digiRead(pinready, ads->getAddress())) {
            //_flag_ErrAlert_2 = false;
            return true;
        }
    }
    Atmega::serial_print("Falla esperando al pin alert ADS: "); 
    Atmega::serial_println(_name);
    //_flag_ErrAlert_2 = true;
    return false;
#else   
    return true;
#endif
}

