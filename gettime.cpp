
#ifdef WINDOWS
#include <windows.h>
double getTime()
{
     LARGE_INTEGER tim, freq;
     QueryPerformanceCounter(&tim);
     QueryPerformanceFrequency(&freq);
     double milliseconds = 1000.0 * (double)tim.QuadPart / (double)freq.QuadPart;
     return milliseconds;
}
#else
#include <unistd.h>
#include <sys/time.h>
double getTime()
{
    struct timeval  tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    double milliseconds = 1000.0 * ((double)tv.tv_sec + (double)tv.tv_usec/1000000.0);
    return milliseconds;
}
#endif
