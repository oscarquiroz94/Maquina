#include "Utilities/Alertas.h"


void AlertaDR6::send_alerta(INextion& nx, const uint8_t tipo)
{
    nx.nxcom.send(PSTRr("page 0"));
    nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)tipo);
    nx.nxcom.send(PSTRr("page0.b4.pco=0"));
    nx.nxcom.send(PSTRr("page0.timeraviso.en=1"));
    if (tipo == ROJA)
    {
        nx.nxcom.send(0, PSTRr("bloquear"), true);
    }
}

void AlertaDR6::mostrar(INextion& nx, Stack& st)
{
    switch (name)
    {
        case (uint8_t)AlertasNames::EMERGENCY :
            if(AlertasMostradas::emergencia_mostrada) return;
            Atmega::serial_println(F("SEGURIDAD DR6: Emergencia"));
            if(st.getB().ingles) nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Emergency Stop"));
            else nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Paro Emergencia"));
            send_alerta(nx, ROJA);
            AlertasMostradas::emergencia_mostrada = true;
            break;
        
        case AlertasNames::MAX_AGIT :
            break;

        case AlertasNames::NO_MOTORES :
            if(AlertasMostradas::nomotores_mostrada) return;
            Atmega::serial_println(F("SEGURIDAD DR6: Falla motores"));
            if(st.getB().ingles) nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Drum or exhaust not confirm"));
            else nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Tambor o exhosto no Confirma"));
            send_alerta(nx, AMARRILLA); 
            AlertasMostradas::nomotores_mostrada = true;
            break;

        case AlertasNames::MAX_TEMP :
            if(AlertasMostradas::maxtemp_mostrada) return;
            Atmega::serial_println(F("SEGURIDAD DR6: Temperatura maxima"));
            if(st.getB().ingles) nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Excedeed Temperature"));
            else nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Temperatura Excedida"));
            send_alerta(nx, AMARRILLA); 
            AlertasMostradas::maxtemp_mostrada = true;
            break;
        
        case AlertasNames::NO_SENSOR :
            if(AlertasMostradas::nosensor_mostrada) return;
            Atmega::serial_println(F("SEGURIDAD DR6: Sensor defectuoso"));
            if(st.getB().ingles) nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Defective Sensor"));
            else nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Sensor Defectuoso"));
            send_alerta(nx, AMARRILLA); 
            AlertasMostradas::nosensor_mostrada = true;
            break;

        case AlertasNames::MAX_ROR :
            if(AlertasMostradas::maxror_mostrada) return;
            Atmega::serial_println(F("SEGURIDAD DR6: Max ror"));
            if(st.getB().ingles) nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Max RoR"));
            else nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Maximo RoR"));
            send_alerta(nx, AMARRILLA); 
            AlertasMostradas::maxror_mostrada = true;
            break;

        case AlertasNames::NO_AGIT :
            if(AlertasMostradas::noagitar_mostrada) return;
            Atmega::serial_println(F("SEGURIDAD DR6: Agitador no confirma"));
            if(st.getB().ingles) nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Shaker not confirm"));
            else nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Agitador no confirma"));
            send_alerta(nx, AMARRILLA); 
            AlertasMostradas::noagitar_mostrada = true;
            break;

        case AlertasNames::NO_ENFR :
            if(AlertasMostradas::noenfriar_mostrada) return;
            Atmega::serial_println(F("SEGURIDAD DR6: Enfriador no confirma"));
            if(st.getB().ingles) nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Cooler not confirm"));
            else nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr("Enfriador no confirma"));
            send_alerta(nx, AMARRILLA); 
            AlertasMostradas::noenfriar_mostrada = true;
            break;

        default:
            Atmega::serial_println(F("AlertaDR6 no definida"));
            break;
    }
    
}

void AlertaDR6::ocultar(INextion& nx, Stack& st)
{
    //Se oculta una vez antes de sacarla del vector
    switch (name)
    {
        case AlertasNames::EMERGENCY :
            Atmega::serial_println(F("SEGURIDAD DR6: NO Emergencia"));
            nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)(NINGUNA));
            nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));
            nx.nxcom.send(0, PSTRr("bloquear"), false);
            break;
        
        case AlertasNames::MAX_AGIT :
            break;

        case AlertasNames::NO_MOTORES:
            Atmega::serial_println(F("SEGURIDAD DR6: Motores okay"));
            nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)(NINGUNA));
            nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));
            nx.nxcom.send(0, PSTRr("bloquear"), false);
            break;

        case AlertasNames::MAX_TEMP :
            Atmega::serial_println(F("SEGURIDAD DR6: Temperatura OKAY"));
            nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)(NINGUNA));
            nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));
            nx.nxcom.send(0, PSTRr("bloquear"), false);
            break;
        
        case AlertasNames::NO_SENSOR :
            Atmega::serial_println(F("SEGURIDAD DR6: Sensor okay"));
            nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)(NINGUNA));
            nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));
            nx.nxcom.send(0, PSTRr("bloquear"), false);
            break;

        case AlertasNames::MAX_ROR:
            Atmega::serial_println(F("SEGURIDAD DR6: ror okay"));
            nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)(NINGUNA));
            nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));
            nx.nxcom.send(0, PSTRr("bloquear"), false);
            break;

        case AlertasNames::NO_AGIT :
            Atmega::serial_println(F("SEGURIDAD DR6: Agitador confirmo"));
            nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)(NINGUNA));
            nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));
            nx.nxcom.send(0, PSTRr("bloquear"), false);
            break;

        case AlertasNames::NO_ENFR :
            Atmega::serial_println(F("SEGURIDAD DR6: Enfriador confirmo"));
            nx.nxcom.send(0, PSTRr("tipoAlarma"), (uint16_t)(NINGUNA));
            nx.nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));
            nx.nxcom.send(0, PSTRr("bloquear"), false);
            break;
        
        default:
            break;
    }
};