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
    int i;
    while(1) {
        // new malloc
        int new_num = 5;
        IPRange *ip_range = (IPRange*)malloc(sizeof(IPRange)*new_num);
        if (ip_range == NULL) {
            return NULL;
        }
        for (i=0; i<new_num; i++) {
            ip_range[i].start_ip = 200;
            ip_range[i].end_ip = 200;
        }

        // old free
        if (ip_range_conf.range_num > 0 && ip_range_conf.ip_range) {
            ip_range_conf.range_num = 0;
            free(ip_range_conf.ip_range);
            ip_range_conf.ip_range = NULL;
        }

        // assignment
        ip_range_conf.ip_range = ip_range;
        ip_range_conf.range_num = new_num;
    }
    return NULL;
}

void *thread2(void *ptr)
{
    bool ret1, ret2;
    while(1) {
        ret1 = ip_in_range(100, &ip_range_conf);
        ret2 = ip_in_range(200, &ip_range_conf);
        if (!ret1 && !ret2) {
            printf("warning!!!\n");
        }
    }
    return NULL;
}

int init()
{
    int num = 2;
    int i = 0;
    ip_range_conf.ip_range = (IPRange*)malloc(sizeof(IPRange)*num);
    if (ip_range_conf.ip_range == NULL) {
        return -1;
    }
    ip_range_conf.range_num = num;
    for (i=0; i<num; i++) {
        ip_range_conf.ip_range[i].start_ip = 100;
        ip_range_conf.ip_range[i].end_ip = 100;
    }
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
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t1, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
