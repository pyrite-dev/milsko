/* $Id$ */
#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

#include "../../external/pl_mpeg.h"
#include "../../external/miniaudio.h"
#include "../../external/stb_ds.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

typedef struct buffer {
	unsigned char* buffer;
	int seek;
	int size;
} buffer_t;

typedef struct frame {
	unsigned char* buffer;
	int width;
	int height;
	int count;
} frame_t;

MwWidget wWindow, wOpengl;
MwWidget bPlay, bPause;
plm_t* plm;
GLuint tex;
buffer_t* buffers = NULL;
frame_t* frames = NULL;
ma_mutex mutex;
ma_event event;
ma_device device;
ma_device_config config;
int paused = 0;
int quit = 0;
int wait = 16;
int v = 0, r = 1;
#ifdef _WIN32
HANDLE thread;
#else
pthread_t thread;
#endif

#ifdef _WIN32
static DWORD WINAPI thread_routine(void* arg){
#else
static void* thread_routine(void* arg){
#endif
	(void)arg;

	ma_event_wait(&event);

	while(!plm_has_ended(plm) && !quit){
		ma_mutex_lock(&mutex);
		if(arrlen(frames) > 1000 || paused){
			ma_mutex_unlock(&mutex);
#ifdef _WIN32
			Sleep(1);
#else
			usleep(1000);
#endif
		}else{
			ma_mutex_unlock(&mutex);
			v = 0;
			plm_decode(plm, wait / 1000.0);
		}
	}

#ifdef _WIN32
	return 0;
#else
	return NULL;
#endif
}

static void data_callback(ma_device* device, void* out, const void* in, ma_uint32 frame){
	int fsz = sizeof(float) * frame * 2;
	int bsz = 0;

	(void)device;
	(void)in;

	memset(out, 0, sizeof(float) * frame * 2);

	ma_mutex_lock(&mutex);
	while(!paused && fsz > 0 && arrlen(buffers) > 0){
		int sz = 0;
		if(fsz > (buffers[0].size - buffers[0].seek)){
			sz = buffers[0].size - buffers[0].seek;
		}else{
			sz = fsz;
		}
		memcpy(bsz + (unsigned char*)out, buffers[0].buffer + buffers[0].seek, sz);

		buffers[0].seek += sz;
		fsz -= sz;
		bsz += sz;
		if(buffers[0].seek >= buffers[0].size){
			free(buffers[0].buffer);
			arrdel(buffers, 0);
		}
	}
	ma_mutex_unlock(&mutex);
}

double a = 0;
static void tick(MwWidget handle, void* user, void* call){
	(void)handle;
	(void)user;
	(void)call;

	ma_mutex_lock(&mutex);
	if(!paused && arrlen(frames) > 0){
		double f = (1000.0 / wait) / plm_get_framerate(plm);

		if(frames[0].count == 0){
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frames[0].width, frames[0].height, 0, GL_RGB, GL_UNSIGNED_BYTE, frames[0].buffer);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		frames[0].count++;
		if(f >= 1 && frames[0].count >= (int)f + (int)a){
			if(a > 1) a--;
			a += f - (int)f;
			free(frames[0].buffer);
			arrdel(frames, 0);
		}else if(f < 1){
			free(frames[0].buffer);
			arrdel(frames, 0);
		}
	}
	ma_mutex_unlock(&mutex);

	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 1, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(0, 1); glVertex2f(0, 1);
		glTexCoord2f(1, 1); glVertex2f(1, 1);
		glTexCoord2f(1, 0); glVertex2f(1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	MwOpenGLSwapBuffer(wOpengl);

	if(r){
		ma_event_signal(&event);
		r = 0;
	}
}

static void video_cb(plm_t* self, plm_frame_t* frame, void* user){
	unsigned char* data = malloc(frame->width * frame->height * 3);
	frame_t f;

	(void)self;
	(void)user;

	if(v){
		free(data);
		return;
	}else{
		v = 1;
	}

	plm_frame_to_rgb(frame, data, frame->width * 3);
	f.buffer = data;
	f.width = frame->width;
	f.height = frame->height;
	f.count = 0;

	ma_mutex_lock(&mutex);
	arrput(frames, f);
	ma_mutex_unlock(&mutex);
}

static void audio_cb(plm_t* self, plm_samples_t* samples, void* user){
	buffer_t buf;

	(void)self;
	(void)user;

	buf.size = sizeof(float) * samples->count * 2;
	buf.seek = 0;
	buf.buffer = malloc(sizeof(float) * samples->count * 2);
	memcpy(buf.buffer, samples->interleaved, sizeof(float) * samples->count * 2);

	ma_mutex_lock(&mutex);
	arrput(buffers, buf);
	ma_mutex_unlock(&mutex);
}

static void button_play(MwWidget handle, void* user, void* call){
	(void)handle;
	(void)user;
	(void)call;

	ma_mutex_lock(&mutex);
	paused = 0;
	ma_mutex_unlock(&mutex);
}

static void button_pause(MwWidget handle, void* user, void* call){
	(void)handle;
	(void)user;
	(void)call;

	ma_mutex_lock(&mutex);
	paused = 1;
	ma_mutex_unlock(&mutex);
}

int main(int argc, char** argv) {
	MwSizeHints sh;
#ifdef _WIN32
	DWORD thid;
#else
	void* ret;
#endif
	int scw = 640, sch = 480;

	if(argc != 2) return 1;

	plm = plm_create_with_filename(argv[1]);
	if(plm == NULL) return 1;

	wait = 1000.0 / plm_get_framerate(plm);

	sh.min_width = sh.max_width = 5 + scw + 5 + 64 + 5;
	sh.min_height = sh.max_height = 5 + sch + 5;

	wWindow = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, sh.min_width, sh.min_height,
				   MwNtitle, "mpeg player",
				   MwNsizeHints, &sh,
				   MwNwaitMS, wait,
				   NULL);
	wOpengl = MwCreateWidget(MwOpenGLClass, "opengl", wWindow, 5, 5, scw, sch);
	bPlay	= MwVaCreateWidget(MwButtonClass, "play", wWindow, 5 + scw + 5, 5, 64, 24,
				   MwNtext, "Play",
				   NULL);
	bPause	= MwVaCreateWidget(MwButtonClass, "pause", wWindow, 5 + scw + 5, 5 + 24 + 5, 64, 24,
				   MwNtext, "Pause",
				   NULL);
	
	MwAddUserHandler(bPlay, MwNactivateHandler, button_play, NULL);
	MwAddUserHandler(bPause, MwNactivateHandler, button_pause, NULL);

	plm_set_loop(plm, 1);
	plm_set_audio_enabled(plm, 1);
	plm_set_audio_stream(plm, 0);
	
	plm_set_video_decode_callback(plm, video_cb, NULL);
	plm_set_audio_decode_callback(plm, audio_cb, NULL);

	MwOpenGLMakeCurrent(wOpengl);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	ma_event_init(&event);
	ma_mutex_init(&mutex);
	config = ma_device_config_init(ma_device_type_playback);
	config.playback.format = ma_format_f32;
	config.playback.channels = 2;
	config.sampleRate = plm_get_samplerate(plm);
	config.dataCallback = data_callback;
	ma_device_init(NULL, &config, &device);
	ma_device_start(&device);

#ifdef _WIN32
	thread = CreateThread(NULL, 0, thread_routine, NULL, 0, &thid);
#else
	pthread_create(&thread, NULL, thread_routine, NULL);
#endif

	MwAddUserHandler(wWindow, MwNtickHandler, tick, NULL);

	MwLoop(wWindow);

	ma_mutex_lock(&mutex);
	quit = 1;
	ma_mutex_unlock(&mutex);

#ifdef _WIN32
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
#else
	pthread_join(thread, &ret);
#endif
	plm_destroy(plm);
	ma_device_uninit(&device);
	ma_mutex_uninit(&mutex);
	ma_event_uninit(&event);
}
