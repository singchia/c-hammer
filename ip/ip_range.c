#include <stdbool.h>
#include <stdio.h>
#include "ip_range.h"
#include "ip.h"

bool ip_in_range(unsigned int ip, IPRangeConf *ip_range_conf)
{
    unsigned int i;
    for (i = 0; i < ip_range_conf->range_num; i++) {
        if (ip_range_conf->ip_range[i].start_ip <= ip &&
                ip <= ip_range_conf->ip_range[i].end_ip) {
            return true;
        }
    }
    return false;
}

bool ip_port_in_range(unsigned int ip, unsigned short port,
        IPRangeConf *ip_range_conf)
{
    unsigned int i, j;
    for (i = 0; i < ip_range_conf->range_num; i++) {
        if (ip_range_conf->ip_range[i].start_ip <= ip &&
                ip <= ip_range_conf->ip_range[i].end_ip) {

            for (j = 0; j < ip_range_conf->ip_range[i].port_num; j++) {
                if (port == ip_range_conf->ip_range[i].ports[j]) {
                    return true;
                }
            }
        }
    }
    return false;
}

void print_ip_range(IPRangeConf* ip_range_conf)
{
    char start[32] = {0};
    char end[32]   = {0};

    unsigned int i;
    for (i = 0; i < ip_range_conf->range_num; i++){
        ip_n2s(ip_range_conf->ip_range[i].start_ip, start, sizeof(start));
        ip_n2s(ip_range_conf->ip_range[i].end_ip, end, sizeof(end));
        printf("start ip: %-16s end ip: %s\n", start, end);
    }
    return;
}

