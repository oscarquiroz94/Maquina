#include "Alertas.h"

bool AlertasMostradas::emergencia_mostrada = false;
bool AlertasMostradas::maxagit_mostrada    = false;
bool AlertasMostradas::nomotores_mostrada  = false;
bool AlertasMostradas::maxtemp_mostrada    = false;
bool AlertasMostradas::nosensor_mostrada   = false;
bool AlertasMostradas::maxror_mostrada     = false;
bool AlertasMostradas::noagitar_mostrada   = false;
bool AlertasMostradas::noenfriar_mostrada  = false;
bool AlertasMostradas::notolvabajacarga_mostrada  = false;
bool AlertasMostradas::tolvaaltacarga_mostrada    = false;
bool AlertasMostradas::notolvabajadesped_mostrada = false;
bool AlertasMostradas::tolvaaltadesped_mostrada   = false;
bool AlertasMostradas::aroagitador_mostrada       = false;
bool AlertasMostradas::incendio_mostrada          = false;
bool AlertasMostradas::bajapresion_in             = false;
bool AlertasMostradas::presion_no_alcanzable      = false;
bool AlertasMostradas::tempera_no_alcanzable      = false;

void AlertasMostradas::reset()
{
    emergencia_mostrada = false;
    maxagit_mostrada    = false;
    nomotores_mostrada  = false;
    maxtemp_mostrada    = false;
    nosensor_mostrada   = false;
    maxror_mostrada     = false;
    noagitar_mostrada   = false;
    noenfriar_mostrada  = false;
    notolvabajacarga_mostrada  = false;
    tolvaaltacarga_mostrada    = false;
    notolvabajadesped_mostrada = false;
    tolvaaltadesped_mostrada   = false;
    aroagitador_mostrada       = false;
    incendio_mostrada          = false;
    bajapresion_in             = false;
    presion_no_alcanzable      = false;
    tempera_no_alcanzable      = false;
}