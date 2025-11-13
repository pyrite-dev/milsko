/* $Id$ */
#include <Mw/Milsko.h>

#if defined(_WIN32)
void* MwMutexCreate(void) {
	return CreateEvent(NULL, FALSE, TRUE, NULL);
}

void MwMutexDestroy(void* handle) {
	CloseHandle(handle);
}

void MwMutexLock(void* handle) {
	WaitForSingleObject(handle, INFINITE);
}

void MwMutexUnlock(void* handle) {
	SetEvent(handle);
}
#elif defined(__unix__)
#include <pthread.h>

void* MwMutexCreate(void) {
	pthread_mutex_t* m = malloc(sizeof(*m));

	pthread_mutex_init(m, NULL);

	return m;
}

void MwMutexDestroy(void* handle) {
	pthread_mutex_destroy(handle);

	free(handle);
}

void MwMutexLock(void* handle) {
	pthread_mutex_lock(handle);
}

void MwMutexUnlock(void* handle) {
	pthread_mutex_unlock(handle);
}
#endif
