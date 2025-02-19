#ifndef RTT_LOG_H_
#define RTT_LOG_H_

#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

#define LOG_PROTO(type, format, ...)                       \
        SEGGER_RTT_printf(0, "%s" format "\r\n", \
                          type,                                   \
                          ##__VA_ARGS__)
#define LOGERROR(format, ...) LOG_PROTO("E:", format, ##__VA_ARGS__)
#define RTTLOG(format, ...) SEGGER_RTT_printf(0, format, ##__VA_ARGS__)
#endif