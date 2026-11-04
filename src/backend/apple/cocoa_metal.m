#include <Mw/Milsko.h>
#include "applegraphics.h"

/* easy shortcut: nothing before 64-bit macs ever supported metal */
#if defined(__x86_64__) || defined(__arm64__)

/* if has __has_include__ */
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 101100
/* if has metal */
#if __has_include(<Metal/Metal.h>) && __has_include(<QuartzCore/CAMetalLayer.h>)
#define HAS_METAL_HEADERS 1
#endif
#endif
#ifndef HAS_METAL_HEADERS
#define HAS_METAL_HEADERS 0
#endif

#if HAS_METAL_HEADERS
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#define ID(a) id<a>
#define PTR(a) a*
#define PIXEL_FORMAT_ARG MTLPixelFormatBGRA8Unorm
#else
#define ID(a) id
#define PTR(a) id
/* without the headers, setPixelFormat: resolves to the NSOpenGL one taking a pointer */
#define PIXEL_FORMAT_ARG ((void*)80UL)
#define MTLPixelFormatBGRA8Unorm 80UL
#define MTLLoadActionDontCare 0UL
#define MTLStoreActionStore 1UL
#define MTLPrimitiveTypeTriangle 3UL
typedef struct MTLRegion {
	struct {
		unsigned long x, y, z;
	} origin;
	struct {
		unsigned long width, height, depth;
	} size;
} MTLRegion;
#endif

/* points on the stack before polygon() needs to allocate */
#define STACK_CROSSINGS 64

static const char* shader_source =
    "#include <metal_stdlib>\n"
    "using namespace metal;\n"
    "struct Vertex {\n"
    "	float4 position [[position]];\n"
    "	float2 uv;\n"
    "};\n"
    "vertex Vertex mw_vertex(uint id [[vertex_id]]) {\n"
    "	Vertex out;\n"
    "	float2 uv = float2((id << 1) & 2, id & 2);\n"
    "	out.position = float4(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0, 0.0, 1.0);\n"
    "	out.uv = uv;\n"
    "	return out;\n"
    "}\n"
    "fragment float4 mw_fragment(Vertex in [[stage_in]], texture2d<float> buffer [[texture(0)]]) {\n"
    "	constexpr sampler nearest(coord::normalized, address::clamp_to_edge, filter::nearest);\n"
    "	return buffer.sample(nearest, in.uv);\n"
    "}\n";

static struct {
	void* metalLib;
	void* quartzLib;

	id (*MTLCreateSystemDefaultDevice)(void);

	Class CAMetalLayerClass;
	Class MTLRenderPassDescriptorClass;
	Class MTLRenderPipelineDescriptorClass;
	Class MTLTextureDescriptorClass;

	ID(MTLDevice) device;
	ID(MTLCommandQueue) queue;
	ID(MTLRenderPipelineState) pipeline;
} mtl;

typedef struct CocoaMetalContext {
	ID(MTLTexture) texture;
	int texture_width;
	int texture_height;
} CocoaMetalContext;

/* the area that can be drawn to as [x0, x1) and [y0, y1) */
static void drawable_area(MwLL handle, int* x0, int* y0, int* x1, int* y1) {
	*x0 = 0;
	*y0 = 0;
	*x1 = handle->cocoa.width;
	*y1 = handle->cocoa.height;

	if(handle->cocoa.has_clip) {
		MwRect* r = &handle->cocoa.clip;

		if(r->x > *x0) {
			*x0 = r->x;
		}
		if(r->y > *y0) {
			*y0 = r->y;
		}
		if(r->x + (int)r->width < *x1) {
			*x1 = r->x + r->width;
		}
		if(r->y + (int)r->height < *y1) {
			*y1 = r->y + r->height;
		}
	}
}

static MwU32 pack_color(MwLLColor color) {
	return 0xff000000u | ((MwU32)color->common.red << 16) | ((MwU32)color->common.green << 8) | (MwU32)color->common.blue;
}

/* drawable_area() is read when drawing, so there is nothing to do here */
static void apply_clip(MwLL handle) {
	(void)handle;
}

static void create_context(MwLL handle, int w, int h) {
	unsigned char* buffer = malloc((size_t)w * h * 4);
	int	       y;

	if(buffer == NULL) {
		return;
	}
	memset(buffer, 0xff, (size_t)w * h * 4);

	if(handle->cocoa.context == NULL) {
		handle->cocoa.context = calloc(1, sizeof(CocoaMetalContext));
		if(handle->cocoa.context == NULL) {
			free(buffer);
			return;
		}
	}

	if(handle->cocoa.buffer != NULL) {
		int cw = handle->cocoa.width;
		int ch = handle->cocoa.height;

		if(w < cw) {
			cw = w;
		}
		if(h < ch) {
			ch = h;
		}

		for(y = 0; y < ch; y++) {
			memcpy(buffer + (size_t)y * w * 4, handle->cocoa.buffer + (size_t)y * handle->cocoa.width * 4, cw * 4);
		}
	}
	free(handle->cocoa.buffer);

	handle->cocoa.buffer = buffer;
	handle->cocoa.width  = w;
	handle->cocoa.height = h;
}

static void destroy_context(MwLL handle) {
	CocoaMetalContext* c = handle->cocoa.context;

	if(c != NULL) {
		[c->texture release];
		free(c);
	}
	free(handle->cocoa.buffer);
}

/* the view asks for this when it becomes layer-backed */
static id make_layer(void) {
	PTR(CAMetalLayer) layer = [mtl.CAMetalLayerClass layer];

	[layer setDevice:mtl.device];
	[layer setPixelFormat:PIXEL_FORMAT_ARG];
	[layer setFramebufferOnly:YES];

	return layer;
}

static int upload_buffer(MwLL handle) {
	CocoaMetalContext* c = handle->cocoa.context;
	MTLRegion	   region;

	if(c->texture == nil || c->texture_width != handle->cocoa.width || c->texture_height != handle->cocoa.height) {
		id descriptor = [mtl.MTLTextureDescriptorClass texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:(unsigned long)handle->cocoa.width height:(unsigned long)handle->cocoa.height mipmapped:NO];

		[c->texture release];
		c->texture = [mtl.device newTextureWithDescriptor:descriptor];
		if(c->texture == nil) {
			return 1;
		}
		c->texture_width  = handle->cocoa.width;
		c->texture_height = handle->cocoa.height;
	}

	region.origin.x	   = 0;
	region.origin.y	   = 0;
	region.origin.z	   = 0;
	region.size.width  = handle->cocoa.width;
	region.size.height = handle->cocoa.height;
	region.size.depth  = 1;

	[c->texture replaceRegion:region mipmapLevel:0UL withBytes:handle->cocoa.buffer bytesPerRow:(unsigned long)handle->cocoa.width * 4];

	return 0;
}

static void present(MwLL handle, NSRect bounds) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	CocoaMetalContext* c	= handle->cocoa.context;
	NSView*		   view = MwLLCocoaGetView(handle);
	PTR(CAMetalLayer) layer = (id)[view layer];
	double scale		= 1;
	CGSize size;
	ID(CAMetalDrawable) drawable;
	PTR(MTLRenderPassDescriptor) pass;
	PTR(MTLRenderPassColorAttachmentDescriptor) attachment;
	ID(MTLCommandBuffer) command;
	ID(MTLRenderCommandEncoder) encoder;

	/* older SDKs would treat the returned float as an object, so those stay at 1x */
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if([view window] != nil) {
		scale = [[view window] backingScaleFactor];
	}
#endif
	size.width  = bounds.size.width * scale;
	size.height = bounds.size.height * scale;

	if(size.width < 1 || size.height < 1 || upload_buffer(handle) != 0) {
		[pool release];
		return;
	}

	[layer setContentsScale:scale];
	[layer setDrawableSize:size];

	drawable = [layer nextDrawable];
	if(drawable == nil) {
		[pool release];
		return;
	}

	pass	   = [mtl.MTLRenderPassDescriptorClass renderPassDescriptor];
	attachment = [[pass colorAttachments] objectAtIndexedSubscript:0UL];
	[attachment setTexture:[drawable texture]];
	[attachment setLoadAction:MTLLoadActionDontCare];
	[attachment setStoreAction:MTLStoreActionStore];

	command = [mtl.queue commandBuffer];
	encoder = [command renderCommandEncoderWithDescriptor:pass];
	[encoder setRenderPipelineState:mtl.pipeline];
	[encoder setFragmentTexture:c->texture atIndex:0UL];
	[encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0UL vertexCount:3UL];
	[encoder endEncoding];
	[command presentDrawable:drawable];
	[command commit];

	/* the texture is overwritten by the next present(), the GPU must be done with it */
	[command waitUntilCompleted];

	[pool release];
}

static void sort_crossings(double* crossings, int count) {
	int i, j;

	for(i = 1; i < count; i++) {
		double v = crossings[i];

		for(j = i - 1; j >= 0 && crossings[j] > v; j--) {
			crossings[j + 1] = crossings[j];
		}
		crossings[j + 1] = v;
	}
}

/* even-odd fill, a pixel is filled if its center is inside */
static void polygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	double	stack[STACK_CROSSINGS];
	double* crossings = stack;
	MwU32	pixel	  = pack_color(color);
	int	x0, y0, x1, y1;
	int	y;

	if(points_count > STACK_CROSSINGS) {
		crossings = malloc(sizeof(*crossings) * points_count);
		if(crossings == NULL) {
			return;
		}
	}

	drawable_area(handle, &x0, &y0, &x1, &y1);

	for(y = y0; y < y1; y++) {
		MwU32* row   = (MwU32*)handle->cocoa.buffer + (size_t)y * handle->cocoa.width;
		int    count = 0;
		int    i;

		for(i = 0; i < points_count; i++) {
			MwPoint* a = &points[i];
			MwPoint* b = &points[(i + 1) % points_count];

			if((a->y <= y) != (b->y <= y)) {
				crossings[count++] = a->x + (y + 0.5 - a->y) * (b->x - a->x) / (b->y - a->y);
			}
		}

		sort_crossings(crossings, count);

		for(i = 0; i + 1 < count; i += 2) {
			int from = (int)ceil(crossings[i] - 0.5);
			int to	 = (int)ceil(crossings[i + 1] - 0.5);
			int x;

			if(from < x0) {
				from = x0;
			}
			if(to > x1) {
				to = x1;
			}
			for(x = from; x < to; x++) {
				row[x] = pixel;
			}
		}
	}

	if(crossings != stack) {
		free(crossings);
	}
}

/* both end points are drawn */
static void line(MwLL handle, MwPoint* points, MwLLColor color) {
	MwU32* buffer = (MwU32*)handle->cocoa.buffer;
	MwU32  pixel  = pack_color(color);
	int    x      = points[0].x;
	int    y      = points[0].y;
	int    dx     = abs(points[1].x - x);
	int    dy     = -abs(points[1].y - y);
	int    sx     = x < points[1].x ? 1 : -1;
	int    sy     = y < points[1].y ? 1 : -1;
	int    err    = dx + dy;
	int    x0, y0, x1, y1;

	drawable_area(handle, &x0, &y0, &x1, &y1);

	while(1) {
		int e2 = err * 2;

		if(x >= x0 && x < x1 && y >= y0 && y < y1) {
			buffer[(size_t)y * handle->cocoa.width + x] = pixel;
		}

		if(x == points[1].x && y == points[1].y) {
			break;
		}
		if(e2 >= dy) {
			err += dy;
			x += sx;
		}
		if(e2 <= dx) {
			err += dx;
			y += sy;
		}
	}
}

/* cocoa.m owns premultiplied and skips drawing while image is NULL */
static void update_pixmap(MwLLPixmap pixmap) {
	pixmap->cocoa.image = pixmap->cocoa.premultiplied;
}

static void destroy_pixmap(MwLLPixmap pixmap) {
	pixmap->cocoa.image = NULL;
}

static MwU32 blend_channel(MwU32 src, MwU32 dst, int shift, MwU32 inverse_alpha) {
	MwU32 s = (src >> shift) & 0xff;
	MwU32 d = (dst >> shift) & 0xff;

	return (s + (d * inverse_alpha + 127) / 255) << shift;
}

/* source-over with premultiplied alpha, scaled by nearest neighbor */
static void draw_pixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	MwU32* source = (MwU32*)pixmap->cocoa.premultiplied;
	int    x0, y0, x1, y1;
	int    x, y;

	drawable_area(handle, &x0, &y0, &x1, &y1);

	if(rect->x > x0) {
		x0 = rect->x;
	}
	if(rect->y > y0) {
		y0 = rect->y;
	}
	if(rect->x + (int)rect->width < x1) {
		x1 = rect->x + rect->width;
	}
	if(rect->y + (int)rect->height < y1) {
		y1 = rect->y + rect->height;
	}

	for(y = y0; y < y1; y++) {
		MwU32* row = (MwU32*)handle->cocoa.buffer + (size_t)y * handle->cocoa.width;
		long   sy  = (2L * (y - rect->y) + 1) * pixmap->common.height / (2L * rect->height);

		for(x = x0; x < x1; x++) {
			long  sx    = (2L * (x - rect->x) + 1) * pixmap->common.width / (2L * rect->width);
			MwU32 src   = source[sy * pixmap->common.width + sx];
			MwU32 alpha = src >> 24;

			if(alpha == 255) {
				row[x] = src;
			} else if(alpha != 0) {
				MwU32 inverse = 255 - alpha;
				MwU32 dst     = row[x];

				row[x] = blend_channel(src, dst, 24, inverse) | blend_channel(src, dst, 16, inverse) | blend_channel(src, dst, 8, inverse) | blend_channel(src, dst, 0, inverse);
			}
		}
	}
}

static ID(MTLRenderPipelineState) create_pipeline(void) {
	NSError* error = nil;
	ID(MTLLibrary) library;
	ID(MTLFunction) vertex;
	ID(MTLFunction) fragment;
	ID(MTLRenderPipelineState) pipeline;
	PTR(MTLRenderPipelineDescriptor) descriptor;
	PTR(MTLRenderPipelineColorAttachmentDescriptor) attachment;

	library = [mtl.device newLibraryWithSource:[NSString stringWithUTF8String:shader_source] options:nil error:&error];
	if(library == nil) {
		printf("WARNING: Could not compile Metal shaders: %s\n", [[error description] UTF8String]);
		return nil;
	}

	vertex	 = [library newFunctionWithName:@"mw_vertex"];
	fragment = [library newFunctionWithName:@"mw_fragment"];

	descriptor = [[mtl.MTLRenderPipelineDescriptorClass alloc] init];
	[descriptor setVertexFunction:vertex];
	[descriptor setFragmentFunction:fragment];
	attachment = [[descriptor colorAttachments] objectAtIndexedSubscript:0UL];
	[attachment setPixelFormat:PIXEL_FORMAT_ARG];

	pipeline = [mtl.device newRenderPipelineStateWithDescriptor:descriptor error:&error];
	if(pipeline == nil) {
		printf("WARNING: Could not create Metal pipeline: %s\n", [[error description] UTF8String]);
	}

	[descriptor release];
	[fragment release];
	[vertex release];
	[library release];

	return pipeline;
}

static void destroy_device(void) {
	[mtl.queue release];
	[mtl.device release];
	mtl.queue  = nil;
	mtl.device = nil;
}

/* Returning non-zero makes the caller fall back to CoreGraphics. Metal being
 * absent is expected on most systems this supports, so that stays quiet. */
int MwLLCocoaMetalSetup(MwLLCocoaGraphics* graphics) {
	mtl.metalLib = MwDynamicOpen("/System/Library/Frameworks/Metal.framework/Metal");
	mtl.quartzLib = MwDynamicOpen("/System/Library/Frameworks/QuartzCore.framework/QuartzCore");
	if(mtl.metalLib == NULL || mtl.quartzLib == NULL) {
		return 1;
	}

	mtl.MTLCreateSystemDefaultDevice = MwDynamicSymbol(mtl.metalLib, "MTLCreateSystemDefaultDevice");
	if(mtl.MTLCreateSystemDefaultDevice == NULL) {
		return 1;
	}

#define METAL_CLASS(x) \
	mtl.x##Class = NSClassFromString(@ #x); \
	if(mtl.x##Class == nil) { \
		printf("WARNING: Could use Metal if " #x " was found.\n"); \
		return 1; \
	}

	METAL_CLASS(CAMetalLayer);
	METAL_CLASS(MTLRenderPassDescriptor);
	METAL_CLASS(MTLRenderPipelineDescriptor);
	METAL_CLASS(MTLTextureDescriptor);

#undef METAL_CLASS

	/* the framework existing does not mean this machine has a usable GPU, VMs often do not */
	mtl.device = mtl.MTLCreateSystemDefaultDevice();
	if(mtl.device == nil) {
		printf("WARNING: No Metal device found\n");
		return 1;
	}

	mtl.queue    = [mtl.device newCommandQueue];
	mtl.pipeline = mtl.queue != nil ? create_pipeline() : nil;
	if(mtl.pipeline == nil) {
		destroy_device();
		return 1;
	}

	graphics->create_context  = create_context;
	graphics->destroy_context = destroy_context;
	graphics->apply_clip	  = apply_clip;
	graphics->present	  = present;
	graphics->make_layer	  = make_layer;
	graphics->polygon	  = polygon;
	graphics->line		  = line;
	graphics->update_pixmap	  = update_pixmap;
	graphics->destroy_pixmap  = destroy_pixmap;
	graphics->draw_pixmap	  = draw_pixmap;


	return 0;
}

#else

int MwLLCocoaMetalSetup(MwLLCocoaGraphics* graphics) {
	(void)graphics;

	return 1;
}

#endif
