#include <stdio.h>
#include <time.h>

#include "../utils/colors.h"

int main() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    if (tm == NULL) {
        perror("localtime");
        return 1;
    }

    printf(" %02d:%02d:%02d  %02d-%02d-%04d\n",
           tm->tm_hour,
           tm->tm_min,
           tm->tm_sec,
           tm->tm_mday,
           tm->tm_mon + 1,   // tm_mon is 0-based
           tm->tm_year + 1900); // tm_year is years since 1900

    return 0;
}
