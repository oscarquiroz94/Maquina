#include "NextionDR6.h"

void NextionDR6::send(const Operativos& op)
{
    INextion::send(op);

    Atmega::printlnDebug(F("NextionDR6: send operativos"));

    nxcom.send(0, PSTRr("n2"), op.mapInputs.pQ);
    nxcom.send(0, PSTRr("n3"), op.mapInputs.pT);
    nxcom.send(0, PSTRr("n4"), op.mapInputs.pS);
}

void NextionDR6::requestHandler(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: handler"));

    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}
    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    if (tshowmsg.tiempo(10000)) nxcom.send(9, PSTRr("t0"), PSTRr(""));

    if (dts.op->request.activarSocketNextion)
    {
        nxcom.send(0, PSTRr("visSocket"), true);
        nxcom.send(3, PSTRr("visSocket"), true);
        nxcom.send(0, PSTRr("visWifiON"), false);
        nxcom.send(3, PSTRr("visCrear"), false);
        nxcom.send(3, PSTRr("visDescon"), true);
        dts.op->request.activarSocketNextion = false;
    }

    if (dts.op->request.desactivarSocketNextion)
    {
        nxcom.send(0, PSTRr("visSocket"), false);
        nxcom.send(3, PSTRr("visSocket"), false);
        nxcom.send(3, PSTRr("visCrear"), true);
        nxcom.send(3, PSTRr("t0"), "");
        dts.op->request.desactivarSocketNextion = false;
    }

    if (dts.op->request.leerSesionSocket)
    {
        nxcom.send_no_ptsr(3, "t1", dts.op->chars.ssid); 
        nxcom.send_no_ptsr(3, "t2", dts.op->chars.pass); 

        strcpy(dts.stack->getC().ssid, dts.op->chars.ssid);
        strcpy(dts.stack->getC().pass, dts.op->chars.pass);

        dts.stack->save();

        memset(dts.op->chars.ssid, 0, (int)sizeof(dts.op->chars.ssid));
        memset(dts.op->chars.pass, 0, (int)sizeof(dts.op->chars.pass));

        dts.op->request.leerSesionSocket = false;
    }

    if (dts.op->request.sendComandoToNextion)
    {
        nxcom.send_no_ptsr(3, "t0", dts.op->chars.comando);
        dts.op->request.sendComandoToNextion = false;
        memset(dts.op->chars.comando, 0, sizeof(dts.op->chars.comando));
    }

    INextion::requestHandler(dts);
}

void NextionDR6::receive(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: receive"));

    //Si llega S,sesion,clave,  --> sesion y clave socket
    //Si existe "S," en comando
    if(nxcom.comparar(nxcom.comando, "S,"))
    {  
        this->crear_socket(dts);
    }

    //Si llega DESOCK --> Desconectar socket
    if(nxcom.compararEqual(nxcom.comando,"DESOCK"))
    {
        this->eliminar_socket(dts);
    }
    
    //Si llega CV,230,160,1500,2000,200,600,300,5000 
    //airemin 2.3, tambormin 1.6, quemin 15.0, airemax 20.0, tambormax 2.0, quemax 6.0, ventq_min 3.0, ventq_max 50.0 
    if(nxcom.comparar(nxcom.comando,"CV,"))
    {
        this->set_cv(dts);
    }

    // Si llega NXV,20230708 ---> version de nextion
    if(nxcom.comparar(nxcom.comando, "NXV,"))
    {  
        this->set_nxversion(dts);
    }

    //Si llega OTHEROR --> cambiar tasa de ror de 30 a 60 o viceversa
    if(nxcom.compararEqual(nxcom.comando, "OTHEROR"))
    {
        this->other_ror(dts);
    }

    //Si llega CHANA --> Canal PT100 por defecto
    if(nxcom.compararEqual(nxcom.comando, "CHANA"))
    {   
        this->set_channelA(dts);
    }  

    //Si llega CHANB --> Canal PT100 por defecto
    if(nxcom.compararEqual(nxcom.comando, "CHANB"))
    {
        this->set_channelB(dts);
    } 

    if(nxcom.compararEqual(nxcom.comando, "ARTISAN"))
    {
        this->set_artisan(dts);
    }
    
    if(nxcom.compararEqual(nxcom.comando, "CROPSTER"))
    {
        this->set_cropster(dts);
    }

    if(nxcom.compararEqual(nxcom.comando,"MANU"))
    {
        set_manual(dts);
    }

    if(nxcom.compararEqual(nxcom.comando,"AUTO"))
    {
        set_auto(dts);
    }

    if(nxcom.compararEqual(nxcom.comando,"ONRUN"))
    {
        set_run(dts, true);
    }

    if(nxcom.compararEqual(nxcom.comando,"OFRUN"))
    {
        set_run(dts, false);
    }

    if(nxcom.compararEqual(nxcom.comando,"ONCARG"))
    {
        request_charge(dts);
    }

    if(nxcom.compararEqual(nxcom.comando,"ONQUEM"))
    {
        set_quemador(dts, true);
    }

    if(nxcom.compararEqual(nxcom.comando,"OFQUEM"))
    {
        set_quemador(dts, false);
    }

    if(nxcom.compararEqual(nxcom.comando,"ONENFR"))
    {
        set_enfriador(dts, true);
    }

    if(nxcom.compararEqual(nxcom.comando,"OFENFR"))
    {
        set_enfriador(dts, false);
    }

    //Evento de boton descargar
    if(nxcom.compararEqual(nxcom.comando,"ONDESC"))
    {
        request_drop(dts);
    }

    if(nxcom.compararEqual(nxcom.comando,"OFDESC"))
    {
        quit_descarga(dts);
    }

    if(nxcom.compararEqual(nxcom.comando,"ONAGIT"))
    {
        set_agitador(dts, true);
    }

    if(nxcom.compararEqual(nxcom.comando,"OFAGIT"))
    {
        set_agitador(dts, false);
    }

    if(nxcom.compararEqual(nxcom.comando,"ONAGIT-OFALERT"))
    {
        set_agitador_nosuperv(dts, true);
    }

    if(nxcom.comparar(nxcom.comando,"TIAC,"))
    {
        set_tiac(dts);
    }

    if(nxcom.comparar(nxcom.comando,"SECURE,"))
    {
        set_last_conf(dts);
    }

    if(nxcom.comparar(nxcom.comando,"ENGLISH"))
    {
        dts.stack->getB().ingles = true;
        dts.stack->save();
    }

    if(nxcom.comparar(nxcom.comando,"SPANISH"))
    {
        dts.stack->getB().ingles = false;
        dts.stack->save();
    }

    if(nxcom.comparar(nxcom.comando,"ALAOFF"))
    {
        dts.op->digitalInputs.offvisalarma = true;
    }

    INextion::receive(dts);
}

// void NextionDR6::init_charge(Datasets& dts)
// {
//     // bool ready = false;
//     // set_carga(dts, dts.op->issemi(), ready);
    
// }

void NextionDR6::set_enfriador(Datasets& dts, bool state)
{
    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}
    dts.op->digitalInputs.onenfriamiento = state;
}

void NextionDR6::set_agitador(Datasets& dts, bool state)
{
    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}
    dts.op->digitalInputs.onagitar = state;
    enable_super_agit = true;
}

void NextionDR6::set_agitador_nosuperv(Datasets& dts, bool state)
{
    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}
    dts.op->digitalInputs.onagitar = state;
    enable_super_agit = false;
}

void NextionDR6::set_quemador(Datasets& dts, bool state)
{
    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}
    dts.op->digitalInputs.onquem = state;
}

void NextionDR6::set_run(Datasets& dts, bool state)
{
    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}
    dts.op->digitalInputs.onrun = state;
}

void NextionDR6::set_nxversion(Datasets& dts)
{
    if (version_nx_seteada) return;
    
    Atmega::printlnDebug(F("NextionDR6: set nx version"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    char *listaValores = strtok(nxcom.comando,",");

    listaValores = strtok(NULL,",");
    dts.stack->getU32().versionNXT = Atmega::str2int(listaValores);

    dts.stack->save();

    this->send_stack(dts);

    version_nx_seteada = true;
}

void NextionDR6::crear_socket(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR1: crear socket"));

    nxcom.send(3, PSTRr("t0"), PSTRr("Espere..."));
    nxcom.send(3, PSTRr("visCrear"), false);

    INextion::crear_socket(dts);
}

void NextionDR6::eliminar_socket(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR1: eliminar socket"));

    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}

    nxcom.send(3, PSTRr("t0"), PSTRr("Desconectando..."));
    nxcom.send(3, PSTRr("visDescon"), false);
    dts.op->digitalInputs.resetESP32 = true;
    nxcom.send(3, PSTRr("visCrear"), true);
    nxcom.send(0, PSTRr("visSocket"), false);
    nxcom.send(3, PSTRr("visSocket"), false);
    nxcom.send(3, PSTRr("t0"), "");
}

void NextionDR6::set_cv(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: set cv"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    INextion::set_cv(dts);

    if(dts.stack->getB().ingles) nxcom.send(4, PSTRr("t7"), PSTRr("Saved"));
    else nxcom.send(4, PSTRr("t7"), PSTRr("Guardado"));
    Atmega::retardo(1500);
    nxcom.send(4, PSTRr("t7"), PSTRr(""));

    dts.stack->save();
}

void NextionDR6::other_ror(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR1: other ror"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    if(dts.stack->getU8().proyeccionRoR == 60){  
        dts.stack->getU8().proyeccionRoR = 30; 
        nxcom.send(PSTRr("page12.b3.bco=50712"));
        nxcom.send(PSTRr("page12.b3.bco2=50712"));
        nxcom.send(PSTRr("page12.b2.bco=21675"));
        nxcom.send(PSTRr("page12.b2.bco2=21675"));
        if(dts.stack->getB().ingles) nxcom.send(12, PSTRr("t7"), PSTRr("Saved"));
        else nxcom.send(12, PSTRr("t7"), PSTRr("Guardado"));
        Atmega::retardo(2000);
        nxcom.send(12, PSTRr("t7"), PSTRr(""));
    }else if(dts.stack->getU8().proyeccionRoR == 30) {
        dts.stack->getU8().proyeccionRoR = 60; 
        nxcom.send(PSTRr("page12.b2.bco=50712"));
        nxcom.send(PSTRr("page12.b2.bco2=50712"));
        nxcom.send(PSTRr("page12.b3.bco=21675"));
        nxcom.send(PSTRr("page12.b3.bco2=21675"));
        if(dts.stack->getB().ingles) nxcom.send(12, PSTRr("t7"), PSTRr("Saved"));
        else nxcom.send(12, PSTRr("t7"), PSTRr("Guardado"));
        Atmega::retardo(2000);
        nxcom.send(12, PSTRr("t7"), PSTRr(""));
    }

    dts.stack->save();
}

void NextionDR6::set_channelA(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: channel A"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    dts.stack->getB().defaultChannel = true;

    nxcom.send(PSTRr("page12.b7.bco=50712"));
    nxcom.send(PSTRr("page12.b7.bco2=50712"));
    nxcom.send(PSTRr("page12.b6.bco=21675"));
    nxcom.send(PSTRr("page12.b6.bco2=21675"));

    dts.stack->save();
}

void NextionDR6::set_channelB(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: channel B"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    dts.stack->getB().defaultChannel = false;

    nxcom.send(PSTRr("page12.b6.bco=50712"));
    nxcom.send(PSTRr("page12.b6.bco2=50712"));
    nxcom.send(PSTRr("page12.b7.bco=21675"));
    nxcom.send(PSTRr("page12.b7.bco2=21675"));

    dts.stack->save();
}

void NextionDR6::set_artisan(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: set artisan"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    Atmega::serial2_begin(115200);    //ARTISAN
    Atmega::serial2_timeout(100);  //number of milliseconds to wait for the next char before aborting timed read
    
    dts.stack->getB().set_artisan();
    
    nxcom.send(PSTRr("page12.b4.bco=21675"));
    nxcom.send(PSTRr("page12.b4.bco2=21675"));
    nxcom.send(PSTRr("page12.b5.bco=57083"));
    nxcom.send(PSTRr("page12.b5.bco2=57083"));

    dts.stack->save();
}

void NextionDR6::set_cropster(Datasets& dts)
{   
    Atmega::printlnDebug(F("NextionDR6: set crospter"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    Atmega::serial2_begin(38400, 0x0E);   //CROPSTER
    Atmega::serial2_timeout(1000);           //number of milliseconds to wait for the next char before aborting timed read
    
    dts.stack->getB().set_cropster();
    
    nxcom.send(PSTRr("page12.b5.bco=21675"));
    nxcom.send(PSTRr("page12.b5.bco2=21675"));
    nxcom.send(PSTRr("page12.b4.bco=57083"));
    nxcom.send(PSTRr("page12.b4.bco2=57083"));

    dts.stack->save();
}

void NextionDR6::send_stack(const Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: send_stack..."));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    // ** VALORES POR DEFECTO **
    nxcom.send(0, PSTRr("enableFunction"), true); //Habilitar pantalla de funciones
    nxcom.send(9, PSTRr("visb1"), true);   // *agitar*
    nxcom.send(9, PSTRr("visb2"), true);   // *enfriar*
    nxcom.send(9, PSTRr("visb3"), false);  // *cargar tostadora*
    nxcom.send(9, PSTRr("visb4"), true);  // *descarg tostadora*
    nxcom.send(9, PSTRr("visb5"), false);  // *auto*
    nxcom.send(9, PSTRr("visb6"), false);  // *manual*
    nxcom.send(9, PSTRr("visb7"), true);   // *run*
    nxcom.send(9, PSTRr("visb8"), true);   // *quemad*
    nxcom.send(9, PSTRr("visb9"), false);  // *descargar enfriador*
    nxcom.send(9, PSTRr("visb10"), false); // *cargar tolva*
    nxcom.send(9, PSTRr("visb11"), false); // *despedregar*
    nxcom.send(9, PSTRr("visb13"), false); // *precalentar*
#ifdef MAQUINA_AUTO
    //! envia en dr1 digital y no existe en esa nextion
    nxcom.send(9, PSTRr("visb14"), false);   // *aspersar* 
#endif

#ifdef MAQUINA_SEMI
    nxcom.send(0, PSTRr("enableFunction"), true); //Habilitar pantalla de funciones
    nxcom.send(9, PSTRr("visb1"), true);  // *agitar*
    nxcom.send(9, PSTRr("visb2"), true);  // *enfriar*
    nxcom.send(9, PSTRr("visb3"), true);  // *cargar tostadora*
    nxcom.send(9, PSTRr("visb4"), true);  // *descarg tostadora*
    nxcom.send(9, PSTRr("visb5"), true);  // *auto*
    nxcom.send(9, PSTRr("visb6"), true);  // *manual*
    nxcom.send(9, PSTRr("visb7"), true);  // *run*
    nxcom.send(9, PSTRr("visb8"), true);  // *quemad*
    nxcom.send(9, PSTRr("visb9"), false);  // *descargar enfriador*
    nxcom.send(9, PSTRr("visb10"), false);  // *cargar tolva*
    nxcom.send(9, PSTRr("visb11"), false);  // *despedregar*
    nxcom.send(9, PSTRr("visb13"), false);  // *precalentar*
#ifdef MAQUINA_AUTO
    //! envia en dr1 digital y no existe en esa nextion
    nxcom.send(9, PSTRr("visb14"), false);   // *aspersar* 
#endif
#endif
#ifdef DRC_25
    nxcom.send(9, PSTRr("visb4"), true);  // *descarg tostadora*
#endif

    nxcom.send(4, PSTRr("x6"), dts.stack->getD().minporcentsopla * 100.0);
    nxcom.send(4, PSTRr("x7"), dts.stack->getD().minporcenttamb * 100.0);
    nxcom.send(4, PSTRr("x8"), dts.stack->getD().minporcentquem * 100.0);
    nxcom.send(4, PSTRr("x9"), dts.stack->getD().maxporcentsopla * 100.0);
    nxcom.send(4, PSTRr("x10"), dts.stack->getD().maxporcenttamb * 100.0);
    nxcom.send(4, PSTRr("x11"), dts.stack->getD().maxporcentquem * 100.0);
    nxcom.send(4, PSTRr("x12"), dts.stack->getD().minporcentventquem * 100.0);
    nxcom.send(4, PSTRr("x13"), dts.stack->getD().maxporcentventquem * 100.0);

    if(dts.stack->getU8().proyeccionRoR == 60){
        nxcom.send(PSTRr("page12.b2.bco=50712"));
        nxcom.send(PSTRr("page12.b2.bco2=50712"));
        nxcom.send(PSTRr("page12.b3.bco=21675"));
        nxcom.send(PSTRr("page12.b3.bco2=21675"));
    }else if(dts.stack->getU8().proyeccionRoR == 30.0){
        nxcom.send(PSTRr("page12.b3.bco=50712"));
        nxcom.send(PSTRr("page12.b3.bco2=50712"));
        nxcom.send(PSTRr("page12.b2.bco=21675"));
        nxcom.send(PSTRr("page12.b2.bco2=21675"));
    }

    nxcom.send_no_ptsr(3, "t1", dts.stack->getC().ssid);
    nxcom.send_no_ptsr(3, "t2", dts.stack->getC().pass);

    nxcom.send(12, PSTRr("n0"), dts.stack->getU32().maxTiempoAgitar/1000.0);
    nxcom.send(12, PSTRr("n1"), dts.stack->getU32().maxTiempoEnfriar/1000.0);
    nxcom.send(12, PSTRr("n2"), dts.stack->getU32().maxTiempoTolva/1000.0);
    nxcom.send(12, PSTRr("n3"), dts.stack->getU32().maxTiempoCompuerta/1000.0);
    nxcom.send(13, PSTRr("n0"), dts.stack->getU16().maxRoR);
    nxcom.send(13, PSTRr("n4"), dts.stack->getU16().suavidadBelimo);
    nxcom.send(13, PSTRr("n5"), dts.stack->getU16().maxTemperSinTambor);
    nxcom.send(13, PSTRr("n6"), dts.stack->getU16().maxTemperAire);
    nxcom.send(13, PSTRr("n7"), dts.stack->getU16().maxtemperGrano);

    nxcom.send(6, PSTRr("h0"), (double)dts.stack->getU8().tablaQuem[0]);
    nxcom.send(6, PSTRr("h1"), (double)dts.stack->getU8().tablaQuem[1]);
    nxcom.send(6, PSTRr("h2"), (double)dts.stack->getU8().tablaQuem[2]);
    nxcom.send(6, PSTRr("h3"), (double)dts.stack->getU8().tablaQuem[3]);
    nxcom.send(6, PSTRr("h4"), (double)dts.stack->getU8().tablaQuem[4]);
    nxcom.send(6, PSTRr("h5"), (double)dts.stack->getU8().tablaQuem[5]);
    nxcom.send(6, PSTRr("h6"), (double)dts.stack->getU8().tablaQuem[6]);
    nxcom.send(6, PSTRr("h7"), (double)dts.stack->getU8().tablaQuem[7]);
    nxcom.send(6, PSTRr("h8"), (double)dts.stack->getU8().tablaQuem[8]);
    nxcom.send(6, PSTRr("h9"), (double)dts.stack->getU8().tablaQuem[9]);
    nxcom.send(6, PSTRr("h10"), (double)dts.stack->getU8().tablaQuem[10]);
    nxcom.send(6, PSTRr("h11"), (double)dts.stack->getU8().tablaQuem[11]);
    nxcom.send(6, PSTRr("h12"), (double)dts.stack->getU8().tablaQuem[12]);
    nxcom.send(6, PSTRr("h13"), (double)dts.stack->getU8().tablaQuem[13]);
    nxcom.send(6, PSTRr("h14"), (double)dts.stack->getU8().tablaQuem[14]);
    nxcom.send(6, PSTRr("h15"), (double)dts.stack->getU8().tablaQuem[15]);
    nxcom.send(6, PSTRr("h16"), (double)dts.stack->getU8().tablaQuem[16]);
    nxcom.send(6, PSTRr("h17"), (double)dts.stack->getU8().tablaQuem[17]);
    nxcom.send(6, PSTRr("h18"), (double)dts.stack->getU8().tablaQuem[18]);
    nxcom.send(6, PSTRr("h19"), (double)dts.stack->getU8().tablaQuem[19]);
    nxcom.send(6, PSTRr("h20"), (double)dts.stack->getU8().tablaQuem[20]);

    if(dts.stack->getB().get_artisan()){
        nxcom.send(PSTRr("page12.b4.bco=21675"));
        nxcom.send(PSTRr("page12.b4.bco2=21675"));
        nxcom.send(PSTRr("page12.b5.bco=57083"));
        nxcom.send(PSTRr("page12.b5.bco2=57083"));
    }else if(dts.stack->getB().get_cropster()){
        nxcom.send(PSTRr("page12.b5.bco=21675"));
        nxcom.send(PSTRr("page12.b5.bco2=21675"));
        nxcom.send(PSTRr("page12.b4.bco=57083"));
        nxcom.send(PSTRr("page12.b4.bco2=57083"));
    }

    if(dts.stack->getB().defaultChannel){
        nxcom.send(PSTRr("page12.b7.bco=50712"));
        nxcom.send(PSTRr("page12.b7.bco2=50712"));
        nxcom.send(PSTRr("page12.b6.bco=21675"));
        nxcom.send(PSTRr("page12.b6.bco2=21675"));
    }else{
        nxcom.send(PSTRr("page12.b6.bco=50712"));
        nxcom.send(PSTRr("page12.b6.bco2=50712"));
        nxcom.send(PSTRr("page12.b7.bco=21675"));
        nxcom.send(PSTRr("page12.b7.bco2=21675"));
    }

    nxcom.send(13, PSTRr("n1"), dts.stack->getU32().versionATM);
    nxcom.send(13, PSTRr("n2"), dts.stack->getU32().versionESP);
    nxcom.send(13, PSTRr("n3"), dts.stack->getU32().versionNXT);
}

void NextionDR6::set_tiac(Datasets& dts)
{
    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    char *listaValores = strtok(nxcom.comando,",");

    listaValores = strtok(NULL,",");
    dts.stack->getU32().maxTiempoAgitar = (uint32_t)Atmega::str2int(listaValores)*1000.0;  // Tiempo de agitacion

    listaValores = strtok(NULL,",");
    dts.stack->getU32().maxTiempoEnfriar = (uint32_t)Atmega::str2int(listaValores)*1000.0;  // Tiempo de enfriamiento

    listaValores = strtok(NULL,",");
    dts.stack->getU32().maxTiempoTolva = (uint32_t)Atmega::str2int(listaValores)*1000.0;  // Tiempo tolva abierta

    listaValores = strtok(NULL,",");
    dts.stack->getU32().maxTiempoCompuerta = (uint32_t)Atmega::str2int(listaValores)*1000.0;  // Tiempo compuerta abierta

    listaValores = strtok(NULL,",");
    dts.stack->getU16().umbralAgitador = (uint16_t)Atmega::str2int(listaValores);  // Umbral Agitador

    dts.stack->save();

    if(dts.stack->getB().ingles) nxcom.send(12, PSTRr("t7"), PSTRr("Saved"));
    else nxcom.send(12, PSTRr("t7"), PSTRr("Guardado"));
    Atmega::retardo(2000);
    nxcom.send(12, PSTRr("t7"), PSTRr(""));
}

void NextionDR6::set_last_conf(Datasets& dts)
{
    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    char *listaValores = strtok(nxcom.comando,",");

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxRoR = (uint16_t)Atmega::str2int(listaValores);  // Valor maximo de RoR

    listaValores = strtok(NULL,",");
    dts.stack->getU16().suavidadBelimo = (uint16_t)Atmega::str2int(listaValores);  // Suavidad Belimo

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxTemperSinTambor = (uint16_t)Atmega::str2int(listaValores);  // Maxima temperatura sin tambor

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxTemperAire = (uint16_t)Atmega::str2int(listaValores);  // Maxima temperatura del aire

    listaValores = strtok(NULL,",");
    dts.stack->getU16().maxtemperGrano = (uint16_t)Atmega::str2int(listaValores);  // Maxima temperatura del grano

    dts.stack->save();

    if(dts.stack->getB().ingles) nxcom.send(13, PSTRr("t7"), PSTRr("Saved"));
    else nxcom.send(13, PSTRr("t7"), PSTRr("Guardado"));
    Atmega::retardo(2000);
    nxcom.send(13, PSTRr("t7"), PSTRr(""));
}

void NextionDR6::set_manual(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: set manual"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}

    dts.op->confirm.isCargaArtisan = false;
    dts.op->confirm.isFirstCrack = false;

    nxcom.send(9, PSTRr("t0"), PSTRr(""));
    nxcom.send(9, PSTRr("cargar"), false);
    if (!dts.op->confirm.isSomeAlert) 
        nxcom.send(0, PSTRr("letrasviso"), PSTRr(""));

    tshowmsg.reset();

    dts.op->setManualmode();
}

void NextionDR6::set_auto(Datasets& dts)
{
    Atmega::printlnDebug(F("NextionDR6: set auto"));

    if (dts.stack == nullptr) {check(__func__,"dts.stack"); return;}
    dts.op->setSemimode();
}


void NextionDR6::quit_descarga(Datasets& dts)
{
    if (dts.op == nullptr) {check(__func__,"dts.op"); return;}
    dts.op->digitalInputs.onenfriamiento = false;
    dts.op->digitalInputs.onagitar = false;
    enable_super_agit = true;
    dts.op->confirm.isDrop = false;
}

void NextionDR6::check(const char* func, const char* entity)
{
    Atmega::printDebug(F("***NextionDR6, "));
    Atmega::printDebug(func);
    Atmega::printDebug(F(": Puntero a "));
    Atmega::printDebug(entity);
    Atmega::printlnDebug(F(" no existe ***"));
}