#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_memory_kb(unsigned long *total_kb, unsigned long *avail_kb) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) return -1;
    char line[256];
    *total_kb = *avail_kb = 0;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "MemTotal: %lu kB", total_kb) == 1) continue;
        if (sscanf(line, "MemAvailable: %lu kB", avail_kb) == 1) break;
    }
    fclose(f);
    return (*total_kb && *avail_kb) ? 0 : -1;
}

int main(void) {
    unsigned long total, avail;
    if (get_memory_kb(&total, &avail) < 0) {
        fprintf(stderr, "Failed to read /proc/meminfo\n");
        return 1;
    }
    unsigned long used_kb = total - avail;
    double used_gb = used_kb / (1024.0 * 1024.0);
    double total_gb = total  / (1024.0 * 1024.0);
    double pct = (double)used_kb * 100.0 / (double)total;
    printf("RAM %.2f%% - %.2fG / %.2fG\n", pct, used_gb, total_gb);
    return 0;
}
