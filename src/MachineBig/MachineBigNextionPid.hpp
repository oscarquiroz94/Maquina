#ifndef __NEXTIONDR6_PIDTEMPERA
#define __NEXTIONDR6_PIDTEMPERA

#include "NextionDR6.h"

class NextionDR6_PIDs : public NextionDR6
{
    public:
        NextionDR6_PIDs() {}

        virtual void receive(Datasets& dts) override;

        virtual void set_last_conf(Datasets& dts) override;

        virtual void send_stack(const Datasets& dts) override;

        ~NextionDR6_PIDs() {}

    private:
        void check(const char* func, const char* entity);
};


#endif