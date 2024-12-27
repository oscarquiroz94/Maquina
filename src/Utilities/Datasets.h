#ifndef __DATASETS
#define __DATASETS

#include "Operacion/Operativos.h"
#include "Operacion/Stack.h"

//Estructura de punteros a los datos
struct Datasets{
    public:
        Operativos* op;
        Stack* stack;
};

#endif