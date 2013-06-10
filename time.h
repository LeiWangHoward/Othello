#include <time.h>
#include <windows.h>
#ifndef _TIMEVAL_DEFINED /* also in winsock[2].h */
#define _TIMEVAL_DEFINED
struct timeval {
  long tv_sec;
  long tv_usec;
};
#define timerisset(tvp)	 ((tvp)->tv_sec || (tvp)->tv_usec)
#define timercmp(tvp, uvp, cmp) \
	(((tvp)->tv_sec != (uvp)->tv_sec) ? \
	((tvp)->tv_sec cmp (uvp)->tv_sec) : \
	((tvp)->tv_usec cmp (uvp)->tv_usec))
#define timerclear(tvp)	 (tvp)->tv_sec = (tvp)->tv_usec = 0
#endif /* _TIMEVAL_DEFINED */
/* Provided for compatibility with code that assumes that
   the presence of gettimeofday function implies a definition
   of struct timezone. */
#ifndef _TIMEZONE_DEFINED /* also in winsock[2].h */
#define _TIMEZONE_DEFINED
struct timezone
{
  int tz_minuteswest; /* of Greenwich */
  int tz_dsttime;     /* type of dst correction to apply */
};

/*
   Implementation as per:
   The Open Group Base Specifications, Issue 6
   IEEE Std 1003.1, 2004 Edition

   The timezone pointer arg is ignored.  Errors are ignored.
*/

#ifdef	__cplusplus

void  GetSystemTimeAsFileTime(FILETIME*);

inline int gettimeofday(struct timeval* p, void* tz /* IGNORED */)
{
	union {
	    long long ns100; /*time since 1 Jan 1601 in 100ns units */
		FILETIME ft;
	} now;

    GetSystemTimeAsFileTime( &(now.ft) );
    p->tv_usec=(long)((now.ns100 / 10LL) % 1000000LL );
    p->tv_sec= (long)((now.ns100-(116444736000000000LL))/10000000LL);
	return 0;
}

#else
    /* Must be defined somewhere else */
	int gettimeofday(struct timeval* p, void* tz /* IGNORED */);
#endif
#endif /* _TIMEVAL_DEFINED */
