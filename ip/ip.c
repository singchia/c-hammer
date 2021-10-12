#include <ctype.h>
#include <stdio.h>
#include "ip.h"

#define IPN2S(ip) ((unsigned char *)&ip)[3], \
    ((unsigned char *)&ip)[2], \
    ((unsigned char *)&ip)[1], \
    ((unsigned char *)&ip)[0]

void ip_n2s(unsigned int ip,
        char *addr, unsigned int len)
{
    snprintf(addr, len, "%u.%u.%u.%u", IPN2S(ip));
}

unsigned int ip_s2n(const char *ip)
{
    char c;
    c = *ip;
    unsigned int integer;
    int val;
    int i,j=0;

    for (j=0;j<4;j++) {
        if (!isdigit(c)){  //first char is 0
            return (0);
        }
        val=0;
        for (i=0;i<3;i++) {
            if (isdigit(c)) {
                val = (val * 10) + (c - '0');
                c = *++ip;
            } else
                break;
        }
        if(val<0 || val>255){
            return (0);
        }
        if (c == '.') {
            integer=(integer<<8) | val;
            c = *++ip;
        }
        else if(j==3 && c == '\0'){
            integer=(integer<<8) | val;
            break;
        }

    }
    if(c != '\0'){
        return (0);
    }
    return integer;
}
