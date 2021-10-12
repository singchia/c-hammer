#ifndef _IP_RANGE_
#define _IP_RANGE_

#include <stdbool.h>

typedef struct IPRange {
    // cidr
    unsigned int start_ip;
    unsigned int end_ip;

    // ports
    unsigned int port_num;
    unsigned short *ports;
} IPRange;

typedef struct IPRangeConf {
    unsigned int range_num;
    IPRange *ip_range;
} IPRangeConf;

bool ip_in_range(unsigned int ip, IPRangeConf *ip_range_conf);

bool ip_port_in_range(unsigned int ip, unsigned short port,
        IPRangeConf *ip_range_conf);

void print_ip_range(IPRangeConf* ip_range_conf);

#endif
