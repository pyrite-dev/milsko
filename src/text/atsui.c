#ifdef USE_ATSUI
#include <Mw/Milsko.h>

#include <ATSUnicode.h>
#include <Gestalt.h>
#include <Fonts.h>
#include <Folders.h>
#include <Resources.h>
#include <Script.h>

/* ATSUI (Mac OS 8.5 and later), loaded at runtime. classic mac os cannot use a font from memory, so each font
 * gets written to a temporary font file (an sfnt resource and a FOND for it), which stays open in the resource
 * chain where the Font Manager finds it */

#define MwFix(x) ((Fixed)(x) << 16)

static struct atsuitbl {
	void* lib;

	OSStatus (*ATSUCreateStyle)(ATSUStyle* style);
	OSStatus (*ATSUDisposeStyle)(ATSUStyle style);
	OSStatus (*ATSUSetAttributes)(ATSUStyle style, ItemCount count, const ATSUAttributeTag tag[], const ByteCount size[], const ATSUAttributeValuePtr value[]);
	OSStatus (*ATSUCreateTextLayoutWithTextPtr)(ConstUniCharArrayPtr text, UniCharArrayOffset offset, UniCharCount length, UniCharCount total, ItemCount runs, const UniCharCount run_lengths[], ATSUStyle styles[], ATSUTextLayout* layout);
	OSStatus (*ATSUDisposeTextLayout)(ATSUTextLayout layout);
	OSStatus (*ATSUDrawText)(ATSUTextLayout layout, UniCharArrayOffset offset, UniCharCount length, ATSUTextMeasurement x, ATSUTextMeasurement y);
	OSStatus (*ATSUMeasureText)(ATSUTextLayout layout, UniCharArrayOffset start, UniCharCount length, ATSUTextMeasurement* before, ATSUTextMeasurement* after, ATSUTextMeasurement* ascent, ATSUTextMeasurement* descent);
	OSStatus (*ATSUFONDtoFontID)(short fond, Style style, ATSUFontID* font);

	/* in InterfaceLib since 8.6 */
	void* interfacelib;
	Boolean (*IsAntiAliasedTextEnabled)(SInt16* min_size);
} atbl;

struct _MwFLFont {
	ATSUStyle style;
	int	  ascent;
	int	  descent;

	FSSpec spec;
	short  resfile;
	short  family;
};

static UniChar* to_utf16(const char* text, UniCharCount* length) {
	UniChar* out = malloc((strlen(text) * 2 + 1) * sizeof(*out));
	int	 n   = 0;

	if(out == NULL) return NULL;

	while(*text != 0) {
		int cp;
		int l = MwUTF8ToUTF32(text, &cp);

		if(l == 0) break;
		text += l;

		if(cp >= 0x10000) {
			cp -= 0x10000;
			out[n++] = 0xd800 + (cp >> 10);
			out[n++] = 0xdc00 + (cp & 0x3ff);
		} else {
			out[n++] = cp;
		}
	}

	*length = n;

	return out;
}

/* *chars has to be freed after the layout */
static int make_layout(MwFLFont font, const char* text, ATSUTextLayout* layout, UniChar** chars) {
	UniCharCount length;
	UniCharCount runs[1];
	ATSUStyle    styles[1];

	if((*chars = to_utf16(text, &length)) == NULL) return 0;
	if(length == 0) {
		free(*chars);
		return 0;
	}

	runs[0]	  = length;
	styles[0] = font->style;
	if(atbl.ATSUCreateTextLayoutWithTextPtr(*chars, 0, length, length, 1, runs, styles, layout) != noErr) {
		free(*chars);
		return 0;
	}

	return 1;
}

static int ATSUI_MwDrawText(MwWidget handle, MwFLFont font, MwPoint* point, const char* text, MwColor color) {
	ATSUTextLayout	      layout;
	UniChar*	      chars;
	RGBColor	      rgb;
	ATSUAttributeTag      tag[1]   = {kATSUColorTag};
	ByteCount	      size[1]  = {sizeof(rgb)};
	ATSUAttributeValuePtr value[1] = {&rgb};
	int		      top;

	/* ATSUI draws into the current QuickDraw port */
	if(handle->lowlevel->common.type != MwLLBackendQuickDraw) return 1;

	rgb.red	  = color->common->red * 0x101;
	rgb.green = color->common->green * 0x101;
	rgb.blue  = color->common->blue * 0x101;
	atbl.ATSUSetAttributes(font->style, 1, tag, size, value);

	if(!make_layout(font, text, &layout, &chars)) return 0;

	if(MwLLQuickdrawBeginText(handle->lowlevel)) {
		top = point->y - (font->ascent + font->descent) / 2;

		TextMode(srcOr);
		atbl.ATSUDrawText(layout, kATSUFromTextBeginning, kATSUToTextEnd, MwFix(point->x), MwFix(top + font->ascent));

		MwLLQuickdrawEndText(handle->lowlevel);
	}

	atbl.ATSUDisposeTextLayout(layout);
	free(chars);

	return 0;
}

static int ATSUI_MwTextWidth(MwFLFont font, const char* text) {
	ATSUTextLayout	    layout;
	UniChar*	    chars;
	ATSUTextMeasurement before, after, ascent, descent;
	int		    width = 0;

	if(!make_layout(font, text, &layout, &chars)) return 0;

	if(atbl.ATSUMeasureText(layout, kATSUFromTextBeginning, kATSUToTextEnd, &before, &after, &ascent, &descent) == noErr) width = (after - before + 0xffff) >> 16;

	atbl.ATSUDisposeTextLayout(layout);
	free(chars);

	return width;
}

static int ATSUI_MwTextHeight(MwFLFont font, int count) {
	return (font->ascent + font->descent) * count;
}

static unsigned int read16(const unsigned char* p) {
	return (p[0] << 8) | p[1];
}

static unsigned long read32(const unsigned char* p) {
	return ((unsigned long)p[0] << 24) | ((unsigned long)p[1] << 16) | ((unsigned long)p[2] << 8) | p[3];
}

static void write16(unsigned char* p, unsigned int v) {
	p[0] = v >> 8;
	p[1] = v;
}

static void write32(unsigned char* p, unsigned long v) {
	p[0] = v >> 24;
	p[1] = v >> 16;
	p[2] = v >> 8;
	p[3] = v;
}

/* glyph for a code point in a format 4 cmap subtable */
static unsigned int format4_glyph(const unsigned char* sub, int cp) {
	unsigned int	     segs = read16(sub + 6) / 2;
	const unsigned char* end  = sub + 14;
	const unsigned char* start = end + segs * 2 + 2;
	const unsigned char* delta = start + segs * 2;
	const unsigned char* range = delta + segs * 2;
	unsigned int	     i;

	for(i = 0; i < segs; i++) {
		unsigned int glyph;

		if(cp > (int)read16(end + i * 2)) continue;
		if(cp < (int)read16(start + i * 2)) return 0;

		if(read16(range + i * 2) == 0) return (cp + read16(delta + i * 2)) & 0xffff;

		glyph = read16(range + i * 2 + read16(range + i * 2) + (cp - read16(start + i * 2)) * 2);
		return glyph == 0 ? 0 : (glyph + read16(delta + i * 2)) & 0xffff;
	}

	return 0;
}

/* classic mac os only takes TrueType fonts with a macintosh (platform 1, encoding 0) cmap subtable, which many
 * fonts do not have anymore. build one from the unicode subtable, as a new cmap table appended to the font.
 * returns a new copy of the font, or NULL if it already has one or cannot get one */
static unsigned char* add_mac_cmap(const unsigned char* data, unsigned int size, unsigned int* new_size) {
	unsigned int	     tables, i, subs, cmap_size, pad, at;
	const unsigned char* entry = NULL;
	const unsigned char* cmap;
	const unsigned char* unicode = NULL;
	unsigned char*	     out;
	unsigned char*	     ncmap;
	unsigned long	     sum;

	if(size < 12) return NULL;
	tables = read16(data + 4);
	for(i = 0; i < tables && 12 + i * 16 + 16 <= size; i++) {
		if(memcmp(data + 12 + i * 16, "cmap", 4) == 0) entry = data + 12 + i * 16;
	}
	if(entry == NULL || read32(entry + 8) + read32(entry + 12) > size) return NULL;

	cmap = data + read32(entry + 8);
	subs = read16(cmap + 2);
	for(i = 0; i < subs; i++) {
		const unsigned char* rec = cmap + 4 + i * 8;
		unsigned int	     platform = read16(rec), encoding = read16(rec + 2);

		if(platform == 1 && encoding == 0) return NULL;
		if(read16(cmap + read32(rec + 4)) == 4 && ((platform == 3 && encoding == 1) || platform == 0)) unicode = cmap + read32(rec + 4);
	}
	if(unicode == NULL) return NULL;

	/* old records and subtables shifted by the new record, then a format 6 subtable for the 256 MacRoman codes */
	cmap_size = read32(entry + 12) + 8 + 10 + 256 * 2;
	pad	  = (4 - size % 4) % 4;
	if((out = malloc(size + pad + cmap_size + 3)) == NULL) return NULL;

	memcpy(out, data, size);
	memset(out + size, 0, pad);
	ncmap = out + size + pad;

	write16(ncmap, 0);
	write16(ncmap + 2, subs + 1);
	at = 4;
	for(i = 0; i <= subs; i++) {
		/* records are sorted by platform and encoding: the new one goes before any of platform 3 */
		const unsigned char* rec = i < subs ? cmap + 4 + i * 8 : NULL;

		if(at == 4 + i * 8 && (rec == NULL || read16(rec) > 1)) {
			write16(ncmap + at, 1);
			write16(ncmap + at + 2, 0);
			write32(ncmap + at + 4, read32(entry + 12) + 8);
			at += 8;
		}
		if(rec != NULL) {
			memcpy(ncmap + at, rec, 4);
			write32(ncmap + at + 4, read32(rec + 4) + 8);
			at += 8;
		}
	}
	memcpy(ncmap + at, cmap + 4 + subs * 8, read32(entry + 12) - (4 + subs * 8));
	at += read32(entry + 12) - (4 + subs * 8);

	write16(ncmap + at, 6);
	write16(ncmap + at + 2, 10 + 256 * 2);
	write16(ncmap + at + 4, 0);
	write16(ncmap + at + 6, 0);
	write16(ncmap + at + 8, 256);
	for(i = 0; i < 256; i++) write16(ncmap + at + 10 + i * 2, format4_glyph(unicode, MwMacRomanToUTF32(i)));

	/* point the directory at the new table */
	memset(ncmap + cmap_size, 0, 3);
	for(sum = 0, i = 0; i < cmap_size; i += 4) sum += read32(ncmap + i);
	write32(out + (entry - data) + 4, sum);
	write32(out + (entry - data) + 8, size + pad);
	write32(out + (entry - data) + 12, cmap_size);

	*new_size = size + pad + cmap_size;

	return out;
}

/* a family ID no installed or loaded font uses */
static short free_family(void) {
	Str255 name;
	short  id;

	for(id = 16383; id > 1024; id--) {
		GetFontName(id, name);
		if(name[0] == 0) return id;
	}

	return 0;
}

static int install_font(MwFLFont font, unsigned char* data, unsigned int size) {
	static int serial = 0;
	short	   vref;
	long	   dir;
	Str63	   name;
	Handle	   sfnt, fond;
	short	   sfnt_id;
	short*	   f;
	unsigned char* fixed;
	unsigned int   fixed_size;
	OSErr	       err;

	if(FindFolder(kOnSystemDisk, kTemporaryFolderType, kCreateFolder, &vref, &dir) != noErr) return 0;

	sprintf((char*)name + 1, "Milsko font %d", ++serial);
	name[0] = strlen((char*)name + 1);

	/* fnfErr is expected, the spec is still valid */
	FSMakeFSSpec(vref, dir, name, &font->spec);
	FSpDelete(&font->spec);

	FSpCreateResFile(&font->spec, 'MwSk', 'FFIL', smSystemScript);
	if(ResError() != noErr) return 0;
	if((font->resfile = FSpOpenResFile(&font->spec, fsRdWrPerm)) == -1) {
		FSpDelete(&font->spec);
		return 0;
	}
	UseResFile(font->resfile);

	if((fixed = add_mac_cmap(data, size, &fixed_size)) != NULL) {
		data = fixed;
		size = fixed_size;
	}
	err = PtrToHand(data, &sfnt, size);
	free(fixed);
	if(err != noErr) goto fail;
	sfnt_id = Unique1ID('sfnt');
	AddResource(sfnt, 'sfnt', sfnt_id, name);
	if(ResError() != noErr) {
		DisposeHandle(sfnt);
		goto fail;
	}

	if((font->family = free_family()) == 0 || (fond = NewHandleClear(60)) == NULL) goto fail;

	/* the smallest FOND there can be: no tables, one association pointing at the outline */
	f     = (short*)*fond;
	f[0]  = 0x4000;	       /* ffFlags: no character width table */
	f[1]  = font->family; /* ffFamID */
	f[2]  = 0;	       /* ffFirstChar */
	f[3]  = 255;	       /* ffLastChar */
	f[4]  = 0x0c00;	       /* ffAscent, 4.12 fixed */
	f[5]  = -0x0400;       /* ffDescent */
	f[6]  = 0;	       /* ffLeading */
	f[7]  = 0x1000;	       /* ffWidMax */
	f[25] = 2;	       /* ffVersion */
	f[26] = 0;	       /* number of associations - 1 */
	f[27] = 0;	       /* size 0: an outline font */
	f[28] = 0;	       /* style */
	f[29] = sfnt_id;

	AddResource(fond, 'FOND', font->family, name);
	if(ResError() != noErr) {
		DisposeHandle(fond);
		goto fail;
	}

	UpdateResFile(font->resfile);
	if(ResError() != noErr) goto fail;

	/* written out now, the Font Manager loads it from the file when it needs it */
	ReleaseResource(sfnt);
	ReleaseResource(fond);

	return 1;

fail:
	CloseResFile(font->resfile);
	FSpDelete(&font->spec);
	return 0;
}

static void remove_font(MwFLFont font) {
	CloseResFile(font->resfile);
	FSpDelete(&font->spec);
}

static void* ATSUI_MwFontLoad(unsigned char* data, unsigned int size, int px) {
	MwFLFont	      font  = malloc(sizeof(*font));
	Fixed		      fsize = MwFix(px);
	ATSUFontID	      id;
	ATSUAttributeTag      tag[2]   = {kATSUFontTag, kATSUSizeTag};
	ByteCount	      sizes[2] = {sizeof(id), sizeof(fsize)};
	ATSUAttributeValuePtr value[2] = {&id, &fsize};
	ATSUTextLayout	      layout;
	UniChar*	      chars;
	ATSUTextMeasurement   before, after, ascent, descent;
	SInt16		      min_size;

	if(font == NULL) return NULL;

	/* without smoothing these fonts look worse than the bitmap font, which gets used when this fails */
	if(atbl.IsAntiAliasedTextEnabled == NULL || !atbl.IsAntiAliasedTextEnabled(&min_size) || px < min_size) {
		free(font);
		return NULL;
	}

	if(!install_font(font, data, size)) {
		free(font);
		return NULL;
	}

	if(atbl.ATSUFONDtoFontID(font->family, normal, &id) != noErr || atbl.ATSUCreateStyle(&font->style) != noErr) {
		remove_font(font);
		free(font);
		return NULL;
	}
	atbl.ATSUSetAttributes(font->style, 2, tag, sizes, value);

	font->ascent  = px;
	font->descent = 0;
	if(make_layout(font, "Mg", &layout, &chars)) {
		if(atbl.ATSUMeasureText(layout, kATSUFromTextBeginning, kATSUToTextEnd, &before, &after, &ascent, &descent) == noErr) {
			font->ascent  = (ascent + 0xffff) >> 16;
			font->descent = (descent + 0xffff) >> 16;
		}

		atbl.ATSUDisposeTextLayout(layout);
		free(chars);
	}

	return font;
}

static void ATSUI_MwFontFree(void* handle) {
	MwFLFont font = handle;

	atbl.ATSUDisposeStyle(font->style);
	remove_font(font);
	free(font);
}

int MwFL_ATSUISetup(void) {
	long version;

	if(Gestalt(gestaltATSUVersion, &version) != noErr) return 1;
	if((atbl.lib = MwDynamicOpen("ATSUnicodeLib")) == NULL) return 1;

#define ATSUI_FUNC(x) \
	if((atbl.x = MwDynamicSymbol(atbl.lib, #x)) == NULL) { \
		MwDynamicClose(atbl.lib); \
		atbl.lib = NULL; \
		return 1; \
	}

	ATSUI_FUNC(ATSUCreateStyle);
	ATSUI_FUNC(ATSUDisposeStyle);
	ATSUI_FUNC(ATSUSetAttributes);
	ATSUI_FUNC(ATSUCreateTextLayoutWithTextPtr);
	ATSUI_FUNC(ATSUDisposeTextLayout);
	ATSUI_FUNC(ATSUDrawText);
	ATSUI_FUNC(ATSUMeasureText);
	ATSUI_FUNC(ATSUFONDtoFontID);

	if((atbl.interfacelib = MwDynamicOpen("InterfaceLib")) != NULL) atbl.IsAntiAliasedTextEnabled = MwDynamicSymbol(atbl.interfacelib, "IsAntiAliasedTextEnabled");

	MwFLDrawText   = ATSUI_MwDrawText;
	MwFLTextWidth  = ATSUI_MwTextWidth;
	MwFLTextHeight = ATSUI_MwTextHeight;
	MwFLFontLoad   = ATSUI_MwFontLoad;
	MwFLFontFree   = ATSUI_MwFontFree;
	return 0;
}

#endif
