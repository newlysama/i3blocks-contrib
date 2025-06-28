#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef unsigned long long ulli;

/**
 * get_values :
 *   - ifaces[0..num_ifaces-1] = noms d’interfaces à surveiller (si num_ifaces==0 → toutes sauf lo)
 *   - *t = epoch courant
 *   - *rx = somme des octets reçus
 *   - *tx = somme des octets envoyés
 */
static void get_values(const char **ifaces, int num_ifaces,
                       time_t *t, ulli *rx, ulli *tx) {
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) {
        perror("fopen /proc/net/dev");
        exit(EXIT_FAILURE);
    }

    char line[512];
    *rx = *tx = 0;

    // on saute les deux lignes d'en-tête
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        // interface: octets_rx ... octets_tx ...
        char *colon = strchr(line, ':');
        if (!colon) continue;

        // isole le nom d’interface
        *colon = '\0';
        char *ifname = line;
        while (*ifname == ' ') ifname++;

        // décider si on l’inclut
        int include = (num_ifaces == 0);
        for (int i = 0; i < num_ifaces; i++) {
            if (strcmp(ifaces[i], ifname) == 0) {
                include = 1;
                break;
            }
        }
        if (!include || strcmp(ifname, "lo") == 0)
            continue;

        // on parse octets rx et tx
        ulli r, s;
        sscanf(colon + 1,
               "%llu %*u %*u %*u %*u %*u %*u %*u %llu",
               &r, &s);
        *rx += r;
        *tx += s;
    }
    fclose(fp);

    *t = time(NULL);
    if (!*t) {
        perror("time()");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    // on peut passer l’interface en argument, ex: ./bandwidth eno1
    const char *interfaces[4];
    int num_ifaces = 0;
    if (argc > 1) {
        interfaces[num_ifaces++] = argv[1];
    }

    time_t t0, t1;
    ulli rx0, tx0, rx1, tx1;

    get_values((const char**)interfaces, num_ifaces, &t0, &rx0, &tx0);
    sleep(1);
    get_values((const char**)interfaces, num_ifaces, &t1, &rx1, &tx1);

    if (rx1 < rx0 || tx1 < tx0) {
        fprintf(stderr, "Network counters wrapped or reset.\n");
        return 1;
    }

    double dt = (double)(t1 - t0);
    double rx_gbps = (rx1 - rx0) * 8.0 / 1e9 / dt;
    double tx_gbps = (tx1 - tx0) * 8.0 / 1e9 / dt;

    printf(" %.2f Gbit/s -  %.2f Gbit/s\n", rx_gbps, tx_gbps);
    return 0;
}
