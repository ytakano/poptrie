#include "poptrie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

int
main(int argc, char *argv[])
{
    struct poptrie *poptrie;
    ssize_t readlen;
    size_t  linecap = 0;
    FILE *fp;
    char *linep = NULL;
    int sz1 = 19;
    int sz0 = 22;

    if (argc < 2) {
        printf("usage: %s file\n", argv[0]);
        return -1;
    }

    poptrie = poptrie_init(NULL, sz1, sz0);
    if ( NULL == poptrie ) {
        printf("failed initialize poptrie\n");
        return -1;
    }

    fp = fopen(argv[1], "r");
    if ( NULL == fp ) {
        return -1;
    }

    while ((readlen = getline(&linep, &linecap, fp)) != -1) {
        int prefix[4];
        int prefixlen;
        int nexthop[4];
        int ret;
        u32 addr1;
        u32 addr2;

        ret = sscanf(linep, "%d.%d.%d.%d/%d %d.%d.%d.%d", &prefix[0], &prefix[1],
                     &prefix[2], &prefix[3], &prefixlen, &nexthop[0],
                     &nexthop[1], &nexthop[2], &nexthop[3]);
        if ( ret < 0 ) {
            printf("failed sscanf\n");
            return -1;
        }

        addr1 = ((u32)prefix[0] << 24) + ((u32)prefix[1] << 16)
                + ((u32)prefix[2] << 8) + (u32)prefix[3];
        addr2 = ((u32)nexthop[0] << 24) + ((u32)nexthop[1] << 16)
                + ((u32)nexthop[2] << 8) + (u32)nexthop[3];

        poptrie_route_add(poptrie, addr1, prefixlen, (void *)(u64)addr2);
        if ( ret < 0 ) {
            printf("failed add route\n");
            return -1;
        }
    }

    free(linep);

    uint64_t nexthop_size = (poptrie->fib.n - 1) * sizeof(void*);
    printf("next hop: %llu\n", nexthop_size);

    uint64_t nodes_size = 0;
    for (int i = 0; i < (1 << sz1); i++) {
        if ((int)poptrie->nodes[i].base0 >=0 || (int)poptrie->nodes[i].base1 >= 0) {
            nodes_size += sizeof(struct poptrie_node);
        }
    }
    printf("internal node: %llu\n", nodes_size);

    uint64_t leaves_size = 0;
    for (int i = 0; i < (1 << sz0); i++) {
        if (poptrie->leaves[i] != 0xFFFF) {
            leaves_size += sizeof(uint16_t);
        }
    }
    printf("leaves: %llu\n", leaves_size);

    uint64_t direct_size = sizeof(u32) << POPTRIE_S;
    printf("direct pointing: %llu\n", direct_size);

    printf("total size: %llu\n", nexthop_size + nodes_size + leaves_size + direct_size);

    return 0;
}