#include <sys/time.h>
#include <unistd.h>

#include "time.h"

uint64_t getCurrentMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return 1000000 * tv.tv_sec + tv.tv_usec;;
}
