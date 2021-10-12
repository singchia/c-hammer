#include <stdbool.h>
#include <stdio.h>
#include "ip_range.h"
#include "ip.h"

bool ip_in_range(unsigned int ip, IPRanges *ip_ranges)
{
    unsigned int i;
    for (i = 0; i < ip_ranges->range_num; i++) {
        if (ip_ranges->ip_ranges[i].start_ip <= ip &&
                ip <= ip_ranges->ip_ranges[i].end_ip) {
            return true;
        }
    }
    return false;
}

bool ip_port_in_range(unsigned int ip, unsigned short port,
        IPRanges *ip_ranges)
{
    unsigned int i, j;
    for (i = 0; i < ip_ranges->range_num; i++) {
        if (ip_ranges->ip_ranges[i].start_ip <= ip &&
                ip <= ip_ranges->ip_ranges[i].end_ip) {

            for (j = 0; j < ip_ranges->ip_ranges[i].port_num; j++) {
                if (port == ip_ranges->ip_ranges[i].ports[j]) {
                    return true;
                }
            }
        }
    }
    return false;
}

void print_ip_range(IPRanges* ip_ranges)
{
    char start[32] = {0};
    char end[32]   = {0};

    unsigned int i;
    for (i = 0; i < ip_ranges->range_num; i++){
        ip_n2s(ip_ranges->ip_ranges[i].start_ip, start, sizeof(start));
        ip_n2s(ip_ranges->ip_ranges[i].end_ip, end, sizeof(end));
        printf("start ip: %-16s end ip: %s\n", start, end);
    }
    return;
}

