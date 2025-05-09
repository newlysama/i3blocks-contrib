#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>

int main() {
    struct statvfs buf;

    if (statvfs("/", &buf) != 0) {
        perror("statvfs");
        return 1;
    }

    unsigned long block_size = buf.f_frsize;
    unsigned long total_blocks = buf.f_blocks;
    unsigned long free_blocks = buf.f_bfree;
    unsigned long used_blocks = total_blocks - free_blocks;

    double total_gb = (double)(block_size * total_blocks) / (1024 * 1024 * 1024);
    double used_gb  = (double)(block_size * used_blocks)  / (1024 * 1024 * 1024);

    double percentage = (used_gb * 100) / total_gb;

    printf("STOR %.2f%% - %.2f GiB / %.2f GiB\n", percentage,  used_gb, total_gb);
    return 0;
}