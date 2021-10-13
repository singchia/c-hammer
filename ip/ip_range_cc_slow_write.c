#include <pthread.h> 
#include <stdlib.h>
#include <stdio.h>
#include "ip_range.h"

/*
 * 验证赋值的原子性
 * gcc -o ip_range_test ip_range_test.c ip.c ip_range.c
 */

IPRangeConf ip_range_conf;

void *thread1(void *ptr)
{
    printf("%p", ip_range_conf.first_ip_ranges->ip_ranges);
    IPRangeConf *ip_range_conf = (IPRangeConf*)ptr;
    IPRanges *bak_ip_ranges = NULL;
    int i;
    while(1) {
        if (ip_range_conf->cur_ip_ranges == ip_range_conf->first_ip_ranges) {
            bak_ip_ranges = ip_range_conf->second_ip_ranges;
        } else if (ip_range_conf->cur_ip_ranges == ip_range_conf->second_ip_ranges) {
            bak_ip_ranges = ip_range_conf->first_ip_ranges;
        } else {
            printf("fatal!!!\n");
        }

        while((time(NULL) - ip_range_conf->latest_update_time) < 3) {
            sleep(1);
        }

        if (bak_ip_ranges->ip_ranges != NULL) {
            bak_ip_ranges->range_num = 0;
            free(bak_ip_ranges->ip_ranges);
            bak_ip_ranges->ip_ranges = NULL;
        }

        // new malloc
        int new_num = 5;
        IPRange *ip_ranges = (IPRange*)malloc(sizeof(IPRange)*new_num);
        if (ip_ranges == NULL) {
            return NULL;
        }
        for (i=0; i<new_num; i++) {
            ip_ranges[i].start_ip = 200;
            ip_ranges[i].end_ip = 200;
        }
        bak_ip_ranges->ip_ranges = ip_ranges;
        bak_ip_ranges->range_num = new_num;

        // switch
        ip_range_conf->cur_ip_ranges = bak_ip_ranges;
        ip_range_conf->latest_update_time = time(NULL);

    }
    return NULL;
}

void *thread2(void *ptr)
{
    IPRangeConf *ip_range_conf = (IPRangeConf*)ptr;
    bool ret1, ret2;
    while(1) {
        ret1 = ip_in_range(100, ip_range_conf->cur_ip_ranges);
        ret2 = ip_in_range(200, ip_range_conf->cur_ip_ranges);
        if (!ret1 && !ret2) {
            printf("warning!!!\n");
        } else {
            printf("succeed!!!\n");
        }
    }
    return NULL;
}

int init()
{
    int num = 2;
    int i = 0;
    IPRanges *first_ip_ranges = (IPRanges*)malloc(sizeof(IPRanges));
    first_ip_ranges->ip_ranges = (IPRange*)malloc(sizeof(IPRange)*num);
    if (first_ip_ranges->ip_ranges == NULL) {
        return -1;
    }
   
    first_ip_ranges->range_num = num;
    for (i=0; i<num; i++) {
        first_ip_ranges->ip_ranges[i].start_ip = 100;
        first_ip_ranges->ip_ranges[i].end_ip = 100;
    }
    ip_range_conf.first_ip_ranges = first_ip_ranges;
    ip_range_conf.cur_ip_ranges = first_ip_ranges;
    ip_range_conf.latest_update_time = 0;

    IPRanges *second_ip_ranges = (IPRanges*)malloc(sizeof(IPRanges));
    ip_range_conf.second_ip_ranges = second_ip_ranges;
    ip_range_conf.second_ip_ranges->ip_ranges = NULL;
    return 0;
}

int main(int argc, char **argv)
{
    int ret;
    ret = init();
    if (ret != 0) {
        return -1;
    }
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, (void*)&ip_range_conf);
    pthread_create(&t2, NULL, thread2, (void*)&ip_range_conf);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
