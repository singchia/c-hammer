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

typedef struct IPRanges {
    unsigned int range_num;
    IPRange *ip_ranges;
} IPRanges;

typedef struct IPRangeConf {
    IPRanges *first_ip_ranges;
    IPRanges *second_ip_ranges;
    IPRanges *cur_ip_ranges;
    int latest_update_time;
} IPRangeConf;

bool ip_in_range(unsigned int ip, IPRanges *ip_ranges);

bool ip_port_in_range(unsigned int ip, unsigned short port,
        IPRanges *ip_ranges);

void print_ip_range(IPRanges* ip_ranges);

#endif
