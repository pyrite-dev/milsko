#include <Mw/Milsko.h>

#if defined(_WIN32)
long MwTimeGetTick(void) {
	return MwLL_PFN_timeGetTime();
}

void MwTimeSleep(int ms) {
	Sleep(ms);
}
#elif defined(__APPLE__)
#include <mach/mach_time.h>

long MwTimeGetTick(void) {
	mach_timebase_info_data_t tb;

	mach_timebase_info(&tb);

	return (long)((double)mach_absolute_time() * tb.numer / tb.denom / 1000000.0);
}
void MwTimeSleep(int ms) {
	struct timespec ts;

	ts.tv_sec  = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000 * 1000;

	nanosleep(&ts, NULL);
}
#elif defined(macintosh)
long MwTimeGetTick(void) {
	UnsignedWide us;

	Microseconds(&us);

	return (long)((((unsigned long long)us.hi << 32) | us.lo) / 1000);
}

void MwTimeSleep(int ms) {
	EventRecord ev;
	long	    end = MwTimeGetTick() + ms;

	/* an empty event mask lets the Process Manager run other applications while we wait,
	 * without taking any events out of our queue */
	while(MwTimeGetTick() < end) {
		long ticks = (end - MwTimeGetTick()) * 60 / 1000;

		WaitNextEvent(0, &ev, ticks > 0 ? ticks : 0, NULL);
		if(EventAvail(everyEvent, &ev)) break;
	}
}
#elif defined(__unix__) || defined(__HAIKU__)
long MwTimeGetTick(void) {
	struct timespec ts;
	long		n = 0;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	n += ts.tv_nsec / 1000 / 1000;
	n += ts.tv_sec * 1000;

	return n;
}

void MwTimeSleep(int ms) {
	struct timespec ts;

	ts.tv_sec  = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;

	nanosleep(&ts, NULL);
}
#endif
