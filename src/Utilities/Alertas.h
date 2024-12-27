#ifndef __ALERTAS
#define __ALERTAS

#include "Utilities/Atmega.h"
#include "Compilationtype.h"
#include "Nextions/INextion.h"
#include "Operacion/Stack.h"

struct AlertasMostradas
{
    //! Si se agrega aqui, agregar tambien en alertas cpp
    static bool emergencia_mostrada;
    static bool maxagit_mostrada;
    static bool nomotores_mostrada;
    static bool maxtemp_mostrada;
    static bool nosensor_mostrada;
    static bool maxror_mostrada;
    static bool noagitar_mostrada;
    static bool noenfriar_mostrada;
    static bool notolvabajacarga_mostrada;
    static bool tolvaaltacarga_mostrada;
    static bool notolvabajadesped_mostrada;
    static bool tolvaaltadesped_mostrada;
    static bool aroagitador_mostrada;
    static bool incendio_mostrada;
    static bool bajapresion_in;
    static bool presion_no_alcanzable;
    static bool tempera_no_alcanzable;

    static void reset();
};

class Alerta
{
public:
    Alerta() = default;
    Alerta(uint8_t name) : name(name) {}

    virtual void mostrar(INextion &nx, Stack &st) { Atmega::serial_println(F("Mostrar no valido")); }
    virtual void ocultar(INextion &nx, Stack &st) { Atmega::serial_println(F("Ocultar no valido")); }

    uint8_t get_name() const { return name; }

    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator==(const Alerta &alert) const { return this->name == alert.name; }
    bool operator<(const Alerta &alert) const { return this->name < alert.name; }

    ~Alerta() {}

protected:
    uint8_t name = 0;
};

class AlertaDRM : public Alerta
{
public:
    AlertaDRM() = default;
    AlertaDRM(uint8_t name) : Alerta(name) {}
    void mostrar(INextion &nx, Stack &st);
    void ocultar(INextion &nx, Stack &st);
    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator!=(const AlertaDRM &alert) { return this->name != alert.name; }
    bool operator==(const AlertaDRM &alert) const { return this->name == alert.name; }
    bool operator<(const AlertaDRM &alert) const { return this->name < alert.name; }
};

class AlertaDR1 : public Alerta
{
public:
    AlertaDR1() = default;
    AlertaDR1(uint8_t name) : Alerta(name) {}
    void mostrar(INextion &nx, Stack &st);
    void ocultar(INextion &nx, Stack &st);
    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator!=(const AlertaDR1 &alert) { return this->name != alert.name; }
    bool operator==(const AlertaDR1 &alert) const { return this->name == alert.name; }
    bool operator<(const AlertaDR1 &alert) const { return this->name < alert.name; }
};

class AlertaDR6 : public Alerta
{
public:
    AlertaDR6() = default;
    AlertaDR6(uint8_t name) : Alerta(name) {}
    void mostrar(INextion &nx, Stack &st);
    void ocultar(INextion &nx, Stack &st);
    void soloDR6() { Atmega::serial_println("solo dr6"); }
    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator!=(const AlertaDR6 &alert) { return this->name != alert.name; }
    bool operator==(const AlertaDR6 &alert) const { return this->name == alert.name; }
    bool operator<(const AlertaDR6 &alert) const { return this->name < alert.name; }

protected:
    void send_alerta(INextion &nx, const uint8_t tipo);
};

class AlertaDR1_digital : public AlertaDR6
{
public:
    AlertaDR1_digital() = default;
    AlertaDR1_digital(uint8_t name) : AlertaDR6(name) {}
    void mostrar(INextion &nx, Stack &st);
    void ocultar(INextion &nx, Stack &st);
    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator!=(const AlertaDR1_digital &alert) { return this->name != alert.name; }
    bool operator==(const AlertaDR1_digital &alert) const { return this->name == alert.name; }
    bool operator<(const AlertaDR1_digital &alert) const { return this->name < alert.name; }
};

class AlertaDR12 : public AlertaDR6
{
public:
    AlertaDR12() = default;
    AlertaDR12(uint8_t name) : AlertaDR6(name) {}
    void mostrar(INextion &nx, Stack &st);
    void ocultar(INextion &nx, Stack &st);
    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator!=(const AlertaDR12 &alert) { return this->name != alert.name; }
    bool operator==(const AlertaDR12 &alert) const { return this->name == alert.name; }
    bool operator<(const AlertaDR12 &alert) const { return this->name < alert.name; }
};

class AlertaDR25 : public AlertaDR12
{
public:
    AlertaDR25() = default;
    AlertaDR25(uint8_t name) : AlertaDR12(name) {}

    void mostrar(INextion &nx, Stack &st);
    void ocultar(INextion &nx, Stack &st);

    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator!=(const AlertaDR25 &alert) { return this->name != alert.name; }
    bool operator==(const AlertaDR25 &alert) const { return this->name == alert.name; }
    bool operator<(const AlertaDR25 &alert) const { return this->name < alert.name; }
};

class AlertaDR70 : public AlertaDR25
{
public:
    AlertaDR70() = default;
    AlertaDR70(uint8_t name) : AlertaDR25(name) {}

    void mostrar(INextion &nx, Stack &st);
    void ocultar(INextion &nx, Stack &st);

    bool operator==(const uint8_t &name) const { return this->name == name; }
    bool operator!=(const uint8_t &name) const { return this->name != name; }
    bool operator!=(const AlertaDR70 &alert) { return this->name != alert.name; }
    bool operator==(const AlertaDR70 &alert) const { return this->name == alert.name; }
    bool operator<(const AlertaDR70 &alert) const { return this->name < alert.name; }
};

#endif