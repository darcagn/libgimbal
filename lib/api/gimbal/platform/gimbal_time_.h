#ifndef GIMBAL_TIME__H
#define GIMBAL_TIME__H

#include <time.h>


#if !defined(GBL_NO_TIMESPEC_)
#   if defined(ANDROID) || defined(__APPLE__) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(VITA) || defined(PSP)
#       define GBL_NO_TIMESPEC_
#   endif
#endif

#ifdef GBL_NO_TIMESPEC_

#define TIME_UTC 1

#if 0
struct timespec {
    time_t tv_sec;
    long   tv_nsec;
};
#endif

int timespec_get(struct timespec* pSpec, int base);

#endif

#endif // GIMBAL_TIME__H
