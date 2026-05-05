#include <Mw/Milsko.h>
#include <sys/mman.h>

void MwLLWaylandFramebufferSetup(struct _MwLLWayland* wayland) {
	MwLLWaylandBufferSetup(&wayland->framebuffer, wayland->ww, wayland->wh);

	wayland->front_cs    = cairo_image_surface_create_for_data(wayland->framebuffer.buf_back, CAIRO_FORMAT_ARGB32, wayland->ww, wayland->wh, 4 * wayland->ww);
	wayland->front_cairo = cairo_create(wayland->front_cs);

	memset(wayland->framebuffer.buf_back, 0, wayland->framebuffer.buf_size);
	if(wayland->configured)
		wl_surface_attach(wayland->framebuffer.surface, wayland->framebuffer.shm_buffer, 0, 0);
	wl_surface_commit(wayland->framebuffer.surface);

	MwLLWaylandHangUntilConfigured((MwLL)wayland);
	MwLLWaylandBufferUpdate((MwLL)wayland, &wayland->framebuffer);
};
void MwLLWaylandFramebufferDestroy(struct _MwLLWayland* wayland) {
	MwLLWaylandBufferDestroy(&wayland->framebuffer);
	cairo_destroy(wayland->front_cairo);
	cairo_surface_destroy(wayland->front_cs);
};

void MwLLWaylandBackbufferSetup(struct _MwLLWayland* wayland) {
	if(wayland->type != MwLL_WAYLAND_TOPLEVEL) {
		return;
	}
	MwU32 w = wayland->ww;
	MwU32 h = wayland->wh;
	if(!wayland->has_decorations) {
		w += (CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT);
		h += (CSD_BORDER_FRAME_TOP + CSD_BORDER_FRAME_BOTTOM);
	}
	MwLLWaylandBufferSetup(&wayland->backbuffer, w, h);

	wayland->back_cs    = cairo_image_surface_create_for_data(wayland->backbuffer.buf_back, CAIRO_FORMAT_ARGB32, w, h, 4 * w);
	wayland->back_cairo = cairo_create(wayland->back_cs);

	memset(wayland->backbuffer.buf_back, 255, wayland->backbuffer.buf_size);
	if(wayland->configured)
		wl_surface_attach(wayland->backbuffer.surface, wayland->backbuffer.shm_buffer, 0, 0);
	wl_surface_commit(wayland->backbuffer.surface);
	MwLLWaylandHangUntilConfigured((MwLL)wayland);
	MwLLWaylandBufferUpdate((MwLL)wayland, &wayland->backbuffer);
};
void MwLLWaylandBackbufferDestroy(struct _MwLLWayland* wayland) {
	MwLLWaylandBufferDestroy(&wayland->backbuffer);
	cairo_destroy(wayland->back_cairo);
	cairo_surface_destroy(wayland->back_cs);
};

void MwLLWaylandBufferSetup(struct _MwLLWaylandShmBuffer* buffer, MwU32 width, MwU32 height) {
	int  stride	      = width * 4;
	char temp_name[]      = "/tmp/milsko-wl-shm-XXXXXX";
	char temp_name_back[] = "/tmp/milsko-wl-shm-back-XXXXXX";

	buffer->buf_size = width * height * 4;

	buffer->fd	= mkstemp(temp_name);
	buffer->fd_back = mkstemp(temp_name_back);

	unlink(temp_name);
	unlink(temp_name_back);

	if(posix_fallocate(buffer->fd, 0, buffer->buf_size) != 0) {
		printf("failure setting up wl_shm: could not fallocate. %s.\n", strerror(errno));
		close(buffer->fd);
		return;
	}
	if(posix_fallocate(buffer->fd_back, 0, buffer->buf_size) != 0) {
		printf("failure setting up wl_shm: could not fallocate. %s.\n", strerror(errno));
		close(buffer->fd_back);
		return;
	}
	if(ftruncate(buffer->fd, buffer->buf_size) != 0) {
		printf("failure setting up wl_shm: could not truncate. %s.\n", strerror(errno));
		close(buffer->fd);
		return;
	}
	if(ftruncate(buffer->fd_back, buffer->buf_size) != 0) {
		printf("failure setting up wl_shm: could not truncate. %s.\n", strerror(errno));
		close(buffer->fd_back);
		return;
	}

	buffer->buf	 = mmap(NULL, buffer->buf_size, PROT_WRITE, MAP_SHARED, buffer->fd, 0);
	buffer->buf_back = mmap(NULL, buffer->buf_size, PROT_WRITE, MAP_SHARED, buffer->fd_back, 0);

	fsync(buffer->fd);
	fsync(buffer->fd_back);

	if(!(buffer->shm_pool = wl_shm_create_pool(buffer->shm, buffer->fd, buffer->buf_size))) {
		printf("failure setting up wl_shm: could not create pool.\n");
	}
	if(!(buffer->shm_pool_back = wl_shm_create_pool(buffer->shm, buffer->fd_back, buffer->buf_size))) {
		printf("failure setting up wl_shm: could not create pool.\n");
	}
	buffer->shm_buffer	= wl_shm_pool_create_buffer(buffer->shm_pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);
	buffer->shm_buffer_back = wl_shm_pool_create_buffer(buffer->shm_pool_back, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);
	buffer->setup		= MwTRUE;
}

void MwLLWaylandBufferDestroy(struct _MwLLWaylandShmBuffer* buffer) {
	if(!buffer->setup) {
		return;
	}
	if(buffer->buf) munmap(buffer->buf, buffer->buf_size);
	if(buffer->buf_back) munmap(buffer->buf_back, buffer->buf_size);
	if(buffer->shm_buffer) wl_buffer_destroy(buffer->shm_buffer);
	if(buffer->shm_buffer_back) wl_buffer_destroy(buffer->shm_buffer_back);
	if(buffer->shm_pool) wl_shm_pool_destroy(buffer->shm_pool);
	if(buffer->shm_pool_back) wl_shm_pool_destroy(buffer->shm_pool_back);
	close(buffer->fd);
	close(buffer->fd_back);
	buffer->setup = MwFALSE;
}
