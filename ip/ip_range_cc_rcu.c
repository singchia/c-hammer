#include <pthread.h> 
#include <stdlib.h>
#include <stdio.h>
#include <urcu-qsbr.h>
#include "ip_range.h"

IPRanges *ip_ranges = NULL;

void *thread1(void *ptr)
{
    IPRanges *local_ip_ranges = NULL;
    IPRanges *old_ip_ranges = NULL;
    int num = 5;
    int i;
    while(1) {
        local_ip_ranges = (IPRanges*)malloc(sizeof(IPRanges));
        if (local_ip_ranges == NULL) {
            return NULL;
        }
        local_ip_ranges->ip_ranges = (IPRange*)malloc(sizeof(IPRange)*num);
        local_ip_ranges->range_num = num;

        for (i=0; i<num; i++) {
            local_ip_ranges->ip_ranges[i].start_ip = 200;
            local_ip_ranges->ip_ranges[i].end_ip = 200;
        }
        old_ip_ranges = rcu_xchg_pointer(&ip_ranges, local_ip_ranges);
        synchronize_rcu();
        if (old_ip_ranges) {
            free(old_ip_ranges->ip_ranges);
            free(old_ip_ranges);
        }
    }
}

void *thread2(void *ptr)
{
    IPRanges *local_ip_ranges = NULL;
    bool ret1, ret2;
    rcu_register_thread();
    while(1) {
        rcu_read_lock();
        local_ip_ranges = rcu_dereference(ip_ranges);
        if (local_ip_ranges) {
            ret1 = ip_in_range(100, local_ip_ranges);
            ret2 = ip_in_range(200, local_ip_ranges);
            if (!ret1 && !ret2) {
                printf("warning %d %d\n",
                        local_ip_ranges->range_num, local_ip_ranges->ip_ranges[0].start_ip);
            }
        }
        rcu_read_unlock();
        rcu_quiescent_state();
    }
    rcu_unregister_thread();
}

int init()
{
    int num = 2;
    int i;
    ip_ranges = (IPRanges*)malloc(sizeof(IPRanges));
    if (ip_ranges == NULL) {
        printf("malloc failed\n");
        return -1;
    }

    ip_ranges->ip_ranges = (IPRange*)malloc(sizeof(IPRange)*num);
    ip_ranges->range_num = num;
    for (i=0; i<num; i++) {
        ip_ranges->ip_ranges[i].start_ip = 100;
        ip_ranges->ip_ranges[i].end_ip = 100;
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
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sleep(5);
    return 0;
}
