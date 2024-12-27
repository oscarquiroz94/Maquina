#ifndef __ATMEGAH
#define __ATMEGAH

#include "main.h"
#include "Compilationtype.h"

#ifndef DEPLOY
	#include <stdint.h>
	#include <string.h>
	#include <iostream>
	#include <chrono>
	#include <thread>
	#include <iomanip>
    #include <mutex>
    #include <deque>
    #include <condition_variable>

	#define HIGH 0x1
	#define LOW 0x0
	#define INPUT 0x0
	#define OUTPUT 0x1
	#define INPUT_PULLUP 0x2

	using String = std::string;

	class __FlashStringHelper;
	#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))
#else
	#include <Arduino.h>
#endif


class Atmega
{
public:
	static bool show_debug;
	static bool show_out;
	static bool show_in;
	static bool show_op;
	static bool show_nx;
	static bool show_wbs;
	static bool show_dts;
private:
	static uint8_t count_pin_IN;
	static uint8_t count_pin_ANAIN;
	static uint8_t count_pin_OUT;
	

public:
	static inline int freeeRam()
	{
#ifdef DEPLOY
		extern int __heap_start, *__brkval;
		int v;
		return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
#else
		return 0;
#endif
	}

	static inline void serial_begin(unsigned long vel)
	{
#ifdef DEPLOY
		Serial.begin(vel);
#endif
	}

	//----------------------

	static inline void serial1_begin(unsigned long vel)
	{
#ifdef DEPLOY
		Serial1.begin(vel);
#endif
	}

	//----------------------

	static inline void serial2_begin(unsigned long vel)
	{
#ifdef DEPLOY
		Serial2.begin(vel);
#endif
	}

	//----------------------

	static inline void serial2_begin(unsigned long vel, const uint8_t tipo)
	{
#ifdef DEPLOY
		Serial2.begin(vel, tipo);
#endif
	}

	//----------------------

	//----------------------

	static inline void serial_timeout(const uint16_t t)
	{
#ifdef DEPLOY
		Serial.setTimeout(t);
#endif
	}

	//----------------------

	static inline void serial2_timeout(const uint16_t t)
	{
#ifdef DEPLOY
		Serial2.setTimeout(t);
#endif
	}

	//----------------------

	//**
	//*********************** Serial to Nextion *******************************
	//**
	static inline int serial1_read()
	{
#ifdef DEPLOY
		return Serial1.read();
#else
		return 0;
#endif
	}

	static inline void serial1_print(const __FlashStringHelper *text)
	{
		char buffer[100];
		strncpy(buffer, (const char *)text, 100);
#ifdef DEPLOY
		Serial1.print(text);
		if (show_nx) Serial.print(text);
#elif defined(EMULATION) && defined(EXTERNAL_INTERFACES)
		serial_nxcom.writeString(buffer);
		if (show_nx) std::cout << buffer;
#endif
#ifdef UNIT_TEST
		cmd_list_atm_to_nextion.push_back(buffer);
#endif
	}

	static inline void serial1_print(const char *td)
	{
#ifdef DEPLOY
		Serial1.print(td);
		if (show_nx) Serial.print(td);
#elif defined(EMULATION) && defined(EXTERNAL_INTERFACES)
		serial_nxcom.writeString(td);
		if (show_nx) std::cout << td;
#endif
#ifdef UNIT_TEST
		cmd_list_atm_to_nextion.push_back(td);
#endif
	}

	static inline void serial1_print(char *td)
	{
#ifdef DEPLOY
		Serial1.print(td);
		if (show_nx) Serial.print(td);
#elif defined(EMULATION) && defined(EXTERNAL_INTERFACES)
		serial_nxcom.writeString(td);
		if (show_nx) std::cout << td;
#endif
#ifdef UNIT_TEST
		cmd_list_atm_to_nextion.push_back(td);
#endif
	}

	//----------------------

	static inline void serial1_print(const char td)
	{
#ifdef DEPLOY
		Serial1.print(td);
		if (show_nx) Serial.print(td);
#elif defined(EMULATION) && defined(EXTERNAL_INTERFACES)
		serial_nxcom.writeChar(td);
		if (show_nx) std::cout << td;
#endif
#ifdef UNIT_TEST
		char buffer[100];
		strncpy(buffer, (const char *)td, 100);
		cmd_list_atm_to_nextion.push_back(buffer);
#endif
	}

	//----------------------

	static inline void serial1_write(char v)
	{

#ifdef DEPLOY
		Serial1.write(v);
		if (show_nx) 
		{
			static int count = 0;
			count++;
			if (count == 3)
			{
				Serial.println();
				count = 0;
			}
		}
#elif defined(EMULATION) && defined(EXTERNAL_INTERFACES)
		serial_nxcom.writeChar(v);

		if (show_nx) 
		{
			static int count = 0;
			std::cout << v;
			count++;
			if (count == 3)
			{
				printlnDebug();
				count = 0;
			}
		}
#endif
	}

	//----------------------

	static inline bool serial1_available()
	{
#ifdef DEPLOY
		return Serial1.available();
#else
		return false;
#endif
	}

	//
	//*************************  Serial to Artisan  *****************************
	//

	static inline void serial3_begin(unsigned long vel)
	{
#ifdef DEPLOY
		Serial3.begin(vel);
#endif
	}

	//--------------------

	static inline void serial3_timeout(const uint16_t t)
	{
#ifdef DEPLOY
		Serial3.setTimeout(t);
#endif
	}

	//---------------------

	static inline void serial3_println(const char *td)
	{
#ifdef DEPLOY
		Serial3.println(td);
		// Serial.println(td);
#else
		if(show_debug)
		{
			std::cout << td << std::endl;
		}
		Websocket::iComando_toSend = td;
#ifdef UNIT_TEST
		cmd_list_esp_to_artisan.push_back(td);
#endif
#endif
	}

	//----------------------

	static inline bool serial3_available()
	{
#ifdef DEPLOY
		return Serial3.available();
#else
		return true;
#endif
	}

	//----------------------

	static inline int serial3_read()
	{
#ifdef DEPLOY
		return Serial3.read();
#else
		return 0;
#endif
	}

	//
	//*************************  Serial debug  *****************************
	//

	static inline void serial_print(double num, uint16_t precision)
	{
#ifdef DEPLOY
		Serial.print(num, precision);
#else
		std::cout << (uint16_t)num;
#endif
	}

	//----------------------

	static inline void serial_print(const uint8_t num)
	{
#ifdef DEPLOY
		Serial.print(num);
#else
		std::cout << (uint16_t)num;
#endif
	}

	//-------------------

	template <class TipodatoSerial>
	static inline void serial_print(const TipodatoSerial text)
	{
#ifdef DEPLOY
		Serial.print(text);
#else
		std::cout << text;
#endif
	}

	//---------------------

	static inline void serial_print(const __FlashStringHelper *text)
	{
#ifdef DEPLOY
		Serial.print(text);
#else
		char buffer[100];
		strncpy(buffer, (const char *)text, 100);
		std::cout << buffer;
#endif
	}

	//----------------------

	static inline void serial_println()
	{
#ifdef DEPLOY
		Serial.println();
#else
		std::cout << std::endl;
#endif
	}

	//----------------------

	static inline void serial_println(const uint8_t num)
	{
#ifdef DEPLOY
		Serial.println(num);
#else
		std::cout << (uint16_t)num << std::endl;
#endif
	}

	//-----------------------

	template <class TipodatoSerial>
	static inline void serial_println(const TipodatoSerial text)
	{
#ifdef DEPLOY
		Serial.println(text);
#else
		std::cout << text << std::endl;
#endif
	}

	//---------------------

	static inline void serial_println(double value, uint16_t precision)
	{
#ifdef DEPLOY
		Serial.println(value, precision);
#else
		std::cout << value << std::endl;
#endif
	}

	//---------------------

	static inline void serial_println(const __FlashStringHelper *text)
	{
#ifdef DEPLOY
		Serial.println(text);
#else
		char buffer[100];
		strncpy(buffer, (const char *)text, 100);
		std::cout << buffer << std::endl;
#endif
	}

	//----------------------

	static inline bool serial_available()
	{
#ifdef DEPLOY
		return Serial.available();
#else
		return true;
#endif
	}

	//----------------------

	static inline int serial_read()
	{
#ifdef DEPLOY
		return Serial.read();
#else
		return 0;
#endif
	}

	static inline void serial_flush()
	{
#ifdef DEPLOY
		Serial.flush();
#endif
	}

	//*=====================================   PRINT DEBUG  ==========================

	static inline void printlnDebug(uint8_t data)
	{
		if(show_debug)
		{
#ifdef DEPLOY
			Serial.println(data);
#else
			auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			auto timestr = ctime(&timenow);
			timestr[strlen(timestr) - 1] = '\0';
			std::cout << (uint16_t)data << std::endl;
#endif
		}
	}

	//----------------------

	template <class TipodatoSerial>
	static inline void printlnDebug(TipodatoSerial data)
	{
		if(show_debug)
		{
#ifdef DEPLOY
			Serial.println(data);
#else
			auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			auto timestr = ctime(&timenow);
			timestr[strlen(timestr) - 1] = '\0';
			std::cout << data << std::endl;
#endif
		}
	}

	//----------------------

	static inline void printlnDebug()
	{
		if(show_debug)
		{
#ifdef DEPLOY
			Serial.println();
#else
			std::cout << std::endl;
#endif
		}
	}

	//----------------------

	static inline void printDebug(uint8_t data)
	{
		if(show_debug)
		{
#ifdef DEPLOY
			Serial.print(data);
#else
			std::cout << (uint16_t)data;
#endif
		}
	}

	//----------------------

	template <class Tipodato>
	static inline void printDebug(Tipodato text)
	{
		if(show_debug)
		{
#ifdef DEPLOY
			Serial.print(text);
#else
			std::cout << text;
#endif
		}
	}

	//----------------------

	static inline void printDebug(const __FlashStringHelper *text)
	{
		if(show_debug)
		{
#ifdef DEPLOY
			Serial.print(text);
#else
			char buffer[100];
			strncpy(buffer, (const char *)text, 100);
			std::cout << buffer;
#endif
		}
	}

	/// @brief
	/// @param text
	static inline void printlnDebug(const __FlashStringHelper *text)
	{
		if(show_debug)
		{
#ifdef DEPLOY
			Serial.println(text);
#else
			char buffer[100];
			strncpy(buffer, (const char *)text, 100);
			std::cout << buffer << std::endl;
#endif
		}
	}

	//*****************************************************************************

	/// @brief
	/// @param constructor
	/// @param nameclass
	/// @param nameinstance
	/// @param nameatributo1
	/// @param atributo1 uint_8
	/// @param nameatributo2
	/// @param atributo2 unit8_t
	static inline void printlnClass(const bool constructor,
									const __FlashStringHelper *nameclass,
									const uint8_t nameinstance,
									const __FlashStringHelper *nameatributo1,
									const uint8_t atributo1 = -1,
									const __FlashStringHelper *nameatributo2 = nullptr,
									const uint8_t atributo2 = -1)
	{
#ifdef SHOWCLASES
#ifdef DEPLOY

#else
		char buffer[100];
		if (!constructor)
		{

			strncpy(buffer, (const char *)nameclass, 100);
			std::cout << buffer << ": instance deleted";
		}
		else
		{
			strncpy(buffer, (const char *)nameclass, 100);
			std::cout << buffer << ": instance created";
			std::cout << ", name: " << (uint16_t)nameinstance;

			if (atributo1 != -1)
			{
				strncpy(buffer, (const char *)nameatributo1, 100);
				std::cout << " " << buffer << ": " << (uint16_t)atributo1;
			}

			if (atributo2 != -1 && nameatributo2 != nullptr)
			{
				strncpy(buffer, (const char *)nameatributo2, 100);
				std::cout << " " << buffer << ": " << (uint16_t)atributo2;
			}
		}
		std::cout << std::endl;
#endif
#endif
	}

	static inline void printClass(const __FlashStringHelper *text)
	{
#ifdef SHOWCLASES
#ifdef DEPLOY
		Serial.print(text);
#else
		char buffer[100];
		strncpy(buffer, (const char *)text, 100);
		std::cout << buffer;
#endif
#endif
	}

	//----------------------

	static inline void printlnClass(const __FlashStringHelper *text)
	{
#ifdef SHOWCLASES
#ifdef DEPLOY
		Serial.println(text);
#else
		char buffer[100];
		strncpy(buffer, (const char *)text, 100);
		std::cout << buffer << std::endl;
#endif
#endif
	}

	//----------------------

	static inline void printClass(uint8_t data)
	{
#ifdef SHOWCLASES
#ifdef DEPLOY
		Serial.print(data);
#else
		std::cout << (uint16_t)data;
#endif
#endif
	}

	//-----------------------

	static inline void printlnClass(uint8_t data)
	{
#ifdef SHOWCLASES
#ifdef DEPLOY
		Serial.println(data);
#else
		std::cout << (uint16_t)data << std::endl;
#endif
#endif
	}

	//****************************  Serial to Cropster  *********************************

	static inline bool serial2_available()
	{
#ifdef DEPLOY
		return Serial2.available();
#else
		return true;
#endif
	}

	//----------------------

	static inline int serial2_read()
	{
#ifdef DEPLOY
		return Serial2.read();
#else
		return 0;
#endif
	}

	static inline void serial2_print(const char *text)
	{
#ifdef DEPLOY
		Serial2.print(text);
		Serial.print(text);
#else
		std::cout << text;
#endif
	}

	static inline void serial2_println(const char *text)
	{
#ifdef DEPLOY
		Serial2.println(text);
		Serial.println(text);
#else
		std::cout << text << std::endl;
#endif
	}

	//*************************************************************************************

	static inline unsigned long millisec()
	{
#ifdef DEPLOY
		return millis();
#else
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		return millis;
#endif
	}

	//----------------------

	static inline unsigned long microsec()
	{
#ifdef DEPLOY
		return micros();
#else
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
		return micros;
#endif
	}

	//----------------------

	static inline void retardo(const uint16_t t)
	{
#ifdef DEPLOY
		delay(t);
#else
		std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(t));
#endif
	}

	//----------------------

	static inline long str2int(const char *txt)
	{
#ifdef DEPLOY
		return String(txt).toInt();
#else
		return std::stoi(txt);
#endif
	}

	//----------------------

	static inline void anaWrite(uint8_t pin, int val, uint8_t name)
	{
#ifdef DEPLOY
		analogWrite(pin, val);
#ifdef SHOW_PWM
		uint16_t name16 = (uint16_t)name;
		uint16_t pin16 = (uint16_t)pin;
		if (name16 == MOTORTAMB)
		{
			serial_print(F("Motor tambor, pinpwm:   "));
			serial_print(pin16);
			serial_print(F(" val: "));
			serial_println(val);
		}
		else if (name16 == QUEMSMALL)
		{
			serial_print(F("Quemador small, pinpwm: "));
			serial_print(pin16);
			serial_print(F(" val: "));
			serial_println(val);
		}
		else if (name16 == MOTORSOPL)
		{
			serial_print(F("Motor soplador, pinpwm: "));
			serial_print(pin16);
			serial_print(F(" val: "));
			serial_println(val);
		}
		else if (name16 == QUEMBIG)
		{
			serial_print(F("Quemador big, pinpwm:   "));
			serial_print(pin16);
			serial_print(F(" val: "));
			serial_println(val);
		}
		else if (name16 == MOTORVENTQUEM)
		{
			serial_print(F("Motor quemador, pinpwm: "));
			serial_print(pin16);
			serial_print(F(" val: "));
			serial_println(val);
		}
		else
		{
			serial_print(F("*** ANAWRITE DESCONOCIDO ***"));
			serial_println(name16);
		}
#endif
#else
#ifdef SHOW_PWM
		uint16_t name16 = (uint16_t)name;
		uint16_t pin16 = (uint16_t)pin;
		if (name16 == MOTORTAMB)
			std::cout << "Motor tambor, pinpwm: " << pin16 << " val: " << val << std::endl;
		else if (name16 == QUEMSMALL)
			std::cout << "Quemador small, pinpwm: " << pin16 << " val: " << val << std::endl;
		else if (name16 == MOTORSOPL)
			std::cout << "Motor soplador, pinpwm: " << pin16 << " val: " << val << std::endl;
		else if (name16 == QUEMBIG)
			std::cout << "Quemador big, pinpwm: " << pin16 << " val: " << val << std::endl;
		else if (name16 == MOTORVENTQUEM)
			std::cout << "Motor quemador, pinpwm: " << pin16 << " val: " << val << std::endl;
		else
			std::cout << "*** ANAWRITE DESCONOCIDO *** -- " << name16 << std::endl;
#endif
#endif
	}

	//----------------------

	static inline int anaRead(uint8_t pin, uint8_t name)
	{
#ifdef DEPLOY
#ifdef SHOW_ANALOG_IN
		uint16_t name16 = (uint16_t)name;
		if (name16 == POTETAMB)
		{
			serial_print(F("Pin potenciometro tambor      "));
			serial_print((uint16_t)pin);
			serial_print(F(" val: "));
			serial_println(analogRead(pin));
			count_pin_ANAIN++;
		}
		else if (name16 == POTEQUEM)
		{
			serial_print(F("Pin potenciometro quemador    "));
			serial_print((uint16_t)pin);
			serial_print(F(" val: "));
			serial_println(analogRead(pin));
			count_pin_ANAIN++;
		}
		else if (name16 == POTESOPL)
		{
			serial_print(F("Pin potenciometro soplador    "));
			serial_print((uint16_t)pin);
			serial_print(F(" val: "));
			serial_println(analogRead(pin));
			count_pin_ANAIN++;
		}
		else
		{
			serial_print(F("*** ANALOG READ DESCONOCIDO **"));
			serial_println(name16);
		}
		if (count_pin_ANAIN == 3)
		{
			serial_println(F("---------------"));
			count_pin_ANAIN = 0;
		}
#endif
		return analogRead(pin);
#else
		return 0;
#endif
	}

	static inline bool digiRead(uint8_t pin, uint8_t name)
	{
#ifdef DEPLOY
		if (show_in)
		{
			uint16_t name16 = (uint16_t)name;
			if (name16 == SENSORAGIT)
			{
				serial_print(F("Pin confirm agitador       "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORENFR)
			{
				serial_print(F("Pin confirm enfriador      "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORCTA)
			{
				serial_print(F("Pin confirm Carg TolvaAlta "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORCTB)
			{
				serial_print(F("Pin confirm Carg TolvaBaja "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORDTA)
			{
				serial_print(F("Pin confirm Desp TolvaAlta "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORDTB)
			{
				serial_print(F("Pin confirm Desp TolvaBaja "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORTOLVA)
			{
				serial_print(F("Pin confirm tolva          "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORCOMP)
			{
				serial_print(F("Pin confirm compuerta      "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSCOMPENFR)
			{
				serial_print(F("Pin confirm comp enfriam   "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORINCEN)
			{
				serial_print(F("Pin confirm incendio       "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORPARO)
			{
				serial_print(F("Pin confirm emergencia     "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == SENSORINICIO)
			{
				serial_print(F("Pin confirm inicio         "));
				serial_print((uint16_t)pin);
				serial_print(F(" val: "));
				serial_println(digitalRead(pin));
				count_pin_IN++;
			}
			else if (name16 == 72)
			{
			}
			else if (name16 == 73)
			{
			}
			else
			{
				serial_print(F("*** DIGI READ DESCONOCIDO **"));
				serial_println(name16);
			}
			if (count_pin_IN == 12)
			{
				serial_println(F("---------------"));
				count_pin_IN = 0;
			}
		}
		return (bool)digitalRead(pin);
#else
		return true;
#endif
	}

	//----------------------

	static inline void digiWrite(uint8_t pin, uint8_t val, uint8_t name)
	{
#ifdef DEPLOY
		digitalWrite(pin, val);
		if (show_out)
		{
			uint16_t name16 = (uint16_t)name;
			uint16_t pin16 = (uint16_t)pin;
			uint16_t val16 = (uint16_t)val;
			if (name16 == MOTORTAMB)
			{
				serial_print(F("Motor tambor, pin:       "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == QUEMSMALL)
			{
				serial_print(F("Quemador small, pin:     "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == MOTORSOPL)
			{
				serial_print(F("Motor soplador, pin:     "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == QUEMBIG)
			{
				serial_print(F("Quemador big, pin:       "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == ACTCOMP)
			{
				serial_print(F("Actuador compuerta, pin: "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == ACTCOMPENFR)
			{
				serial_print(F("Actuador comp enfr, pin: "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == ACTTOLVA)
			{
				serial_print(F("Actuador tolva, pin:     "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == MOTORAGIT)
			{
				serial_print(F("Motor agitador, pin:     "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == MOTORENFR)
			{
				serial_print(F("Motor enfriador, pin:    "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == MOTORCARG)
			{
				serial_print(F("Motor cargador, pin:     "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == MOTORDESP)
			{
				serial_print(F("Motor desped, pin:       "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == ASPER)
			{
				serial_print(F("Aspersor, pin:           "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == BUZZER)
			{
				serial_print(F("Buzzer, pin:             "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == CO2)
			{
				serial_print(F("Gas CO2, pin:            "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == RSTESP)
			{
				serial_print(F("Reset ESP32, pin:        "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else if (name16 == MEMOSD)
			{
				serial_print(F("chipSD, pin:             "));
				serial_print(pin16);
				serial_print(F(" val: "));
				serial_println(val16);
				count_pin_OUT++;
			}
			else
			{
				serial_print(F("*** DIGI WRITE DESCONOCIDO **"));
				serial_println(name16);
			}
			if (count_pin_OUT == 13)
			{
				serial_println(F("---------------"));
				count_pin_OUT = 0;
			}
		}
#else
		if (show_out)
		{
			uint16_t name16 = (uint16_t)name;
			uint16_t pin16 = (uint16_t)pin;
			uint16_t val16 = (uint16_t)val;
			if (name16 == MOTORTAMB)
				std::cout << "Motor tambor, pin:   " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == QUEMSMALL)
				std::cout << "Quemador small, pin: " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == MOTORSOPL)
				std::cout << "Motor soplador, pin: " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == QUEMBIG)
				std::cout << "Quemador big, pin:   " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == ACTCOMP)
				std::cout << "Actuador compuerta, pin:    " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == ACTCOMPENFR)
				std::cout << "Actuador comp enfriar, pin: " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == ACTTOLVA)
				std::cout << "Actuador tolva, pin:  " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == MOTORAGIT)
				std::cout << "Motor agitador, pin:  " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == MOTORENFR)
				std::cout << "Motor enfriador, pin: " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == MOTORCARG)
				std::cout << "Motor cargador, pin:  " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == MOTORDESP)
				std::cout << "Motor despedreg, pin: " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == ASPER)
				std::cout << "Aspersor, pin:   " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == BUZZER)
				std::cout << "Buzzer, pin:     " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == CO2)
				std::cout << "Gas CO2, pin:    " << pin16 << " val: " << val16 << std::endl;
			else if (name16 == RSTESP)
			{
			}
			else if (name16 == MEMOSD)
			{
			}
			else
				std::cout << "*** DIGI WRITE DESCONOCIDO *** -- " << name16 << std::endl;
		}
#endif
	}

	//----------------------

	static inline uint16_t random()
	{
#ifdef DEPLOY
		return random();
#else
		return std::rand();
#endif
	}

	//----------------------

	static inline void pinmode(uint8_t pin, uint8_t dir)
	{
#ifdef DEPLOY
		pinMode(pin, dir);
#endif
	}

#ifdef DEPLOY
	static inline String str_atm(unsigned char value)
	{
		return String(value);
	}
#else
	template <typename T>
	static inline std::string str_atm(T value)
	{
		return std::to_string(value);
	}
#endif
};

#endif