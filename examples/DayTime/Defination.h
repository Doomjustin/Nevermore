#ifndef NEVERMORE_DAY_TIME_DEFINATION_H
#define NEVERMORE_DAY_TIME_DEFINATION_H

#include <sys/types.h>
#include <netinet/in.h>


constexpr static in_port_t DayTimePort = 13333; 
constexpr static int MaxLine = 4096;
constexpr static const char* Quit = "quit";

#endif // NEVERMORE_DAY_TIME_DEFINATION_H