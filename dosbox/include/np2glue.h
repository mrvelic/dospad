/* The purpose of this header is to provide defines and macros
 * to help port code from Neko Project II and match the typedefs
 * it uses. */

// GLUE TYPEDEFS
// WARNING: Windows targets will want to IFDEF some of these out as they will
//          conflict with the typedefs in windows.h
typedef uint32_t UINT32;
typedef int32_t SINT32;
typedef uint16_t UINT16;
typedef int16_t SINT16;
typedef uint8_t UINT8;
typedef int8_t SINT8;
typedef uint32_t UINT;
typedef uint32_t REG8; /* GLIB guint32 -> UINT -> REG8 */
#ifndef WIN32
typedef uint8_t BOOL;
#endif
typedef char OEMCHAR;
typedef void* NEVENTITEM;
typedef void* NP2CFG;
#define OEMTEXT(x) (x)
#define SOUNDCALL

#define LOADINTELWORD(x) host_readw((HostPt)(x))
#define STOREINTELWORD(x,y) host_writew((HostPt)(x),(y))

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* TODO: Move into another header */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef WIN32
static inline void ZeroMemory(void *p,size_t l) {
    memset(p,0,l);
}

static inline void FillMemory(void *p,size_t l,unsigned char c) {
    memset(p,c,l);
}
#endif

