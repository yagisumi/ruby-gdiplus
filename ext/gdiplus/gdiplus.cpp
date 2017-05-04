#include "ruby_gdiplus.h"

VALUE mGdiplus;
VALUE eGdiplus;

int gdip_refcount = 0;
bool gdip_end_flag = false;
static ULONG_PTR gdiplus_token = 0;

const char *GpStatusStrs[22] = {
    "Ok",
    "GenericError",
    "InvalidParameter",
    "OutOfMemory",
    "ObjectBusy",
    "InsufficientBuffer",
    "NotImplemented",
    "Win32Error",
    "WrongState",
    "Aborted",
    "FileNotFound",
    "ValueOverflow",
    "AccessDenied",
    "UnknownImageFormat",
    "FontFamilyNotFound",
    "FontStyleNotFound",
    "NotTrueTypeFont",
    "UnsupportedGdiplusVersion",
    "GdiplusNotInitialized",
    "PropertyNotFound",
    "PropertyNotSupported",
    "ProfileNotFound"
};

#ifdef GDIPLUS_DEBUG
void
dp(const char *fmt, ...)
{
    char buf[128] = {0};
    va_list list;
    va_start(list, fmt);
    _vsnprintf(buf, 127, fmt, list);
    OutputDebugString(buf);
    va_end(list);
}
#endif

static void
gdiplus_init()
{
    dp("GdiplusStartup");
    GdiplusStartupInput gsi;
    Status status = GdiplusStartup(&gdiplus_token, &gsi, NULL);
    if (gdiplus_token == 0 || status != Ok) {
        rb_raise(eGdiplus, "GdiplusStartup error");
    }
}

void
gdiplus_shutdown(){
    dp("%s (gdip_refcount: %d)", __func__, gdip_refcount);
    if (gdip_end_flag && gdip_refcount == 0) {
        dp("GdiplusShutdown");
        GdiplusShutdown(gdiplus_token);
    }
}

static void
gdiplus_end(VALUE self)
{
    gdip_end_flag = true;
    gdiplus_shutdown();
}

extern "C" void
Init_gdiplus(void)
{
    mGdiplus = rb_define_module("Gdiplus");
    eGdiplus = rb_define_class_under(mGdiplus, "GdiplusError", rb_eException);



    gdiplus_init();
    rb_set_end_proc(gdiplus_end, Qnil);
}
