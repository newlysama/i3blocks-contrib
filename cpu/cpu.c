#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SNAPSHOT_PATH "/tmp/cpu_usage.stat"

static int read_snapshot(unsigned long long *last_total, unsigned long long *last_idle) {
    FILE *f = fopen(SNAPSHOT_PATH, "r");
    if (!f) return -1;
    if (fscanf(f, "%llu %llu", last_total, last_idle) != 2) {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

static int write_snapshot(unsigned long long total, unsigned long long idle) {
    FILE *f = fopen(SNAPSHOT_PATH, "w");
    if (!f) return -1;
    fprintf(f, "%llu %llu\n", total, idle);
    fclose(f);
    return 0;
}

int main(void) {
    unsigned long long last_total = 0, last_idle = 0;
    read_snapshot(&last_total, &last_idle);

    FILE *f = fopen("/proc/stat", "r");
    if (!f) { perror("fopen"); return 1; }
    char cpu_label[5];
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;

    fscanf(f, "%4s %llu %llu %llu %llu %llu %llu %llu %llu",
           cpu_label,
           &user, &nice, &system, &idle,
           &iowait, &irq, &softirq, &steal);
    fclose(f);

    unsigned long long total = user + nice + system + idle + iowait + irq + softirq + steal;
    unsigned long long delta_total = total   - last_total;
    unsigned long long delta_idle  = (idle + iowait) - last_idle;

    double usage = 0.0;
    if (delta_total > 0)
        usage = (double)(delta_total - delta_idle) * 100.0 / (double)delta_total;

    // save for next time
    write_snapshot(total, idle + iowait);

    printf("CPU %.2f%%\n", usage);
    return 0;
}
