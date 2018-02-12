/*
 *  Copyright (C) 2002-2015  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DOSBOX_MEM_H
#define DOSBOX_MEM_H

#ifndef DOSBOX_DOSBOX_H
#include "dosbox.h"
#endif

/* MinGW implements some MSVC idioms, so always test for MinGW first. */
#if defined(__MINGW32__)

# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#  define htobe16(x) htons(x)
#  define htole16(x) (x)
#  define be16toh(x) ntohs(x)
#  define le16toh(x) (x)

#  define htobe32(x) htonl(x)
#  define htole32(x) (x)
#  define be32toh(x) ntohl(x)
#  define le32toh(x) (x)

#  define htobe64(x) htonll(x)
#  define htole64(x) (x)
#  define be64toh(x) ntohll(x)
#  define le64toh(x) (x)

# elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#  define htobe16(x) (x)
#  define htole16(x) __builtin_bswap16(x)
#  define be16toh(x) (x)
#  define le16toh(x) __builtin_bswap16(x)

#  define htobe32(x) (x)
#  define htole32(x) __builtin_bswap32(x)
#  define be32toh(x) (x)
#  define le32toh(x) __builtin_bswap32(x)

#  define htobe64(x) (x)
#  define htole64(x) __builtin_bswap64(x)
#  define be64toh(x) (x)
#  define le64toh(x) __builtin_bswap64(x)

# else
#  error Unexpected __BYTE_ORDER__

# endif /* __MINGW__ __BYTE_ORDER__ */

#elif defined(_MSC_VER)

# if BYTE_ORDER == LITTLE_ENDIAN

# define htobe16(x) htons(x)
# define htole16(x) (x)
# define be16toh(x) ntohs(x)
# define le16toh(x) (x)

# define htobe32(x) htonl(x)
# define htole32(x) (x)
# define be32toh(x) ntohl(x)
# define le32toh(x) (x)

# define htobe64(x) htonll(x)
# define htole64(x) (x)
# define be64toh(x) ntohll(x)
# define le64toh(x) (x)

# elif BYTE_ORDER == BIG_ENDIAN

# define htobe16(x) (x)
# define htole16(x) __builtin_bswap16(x)
# define be16toh(x) (x)
# define le16toh(x) __builtin_bswap16(x)

# define htobe32(x) (x)
# define htole32(x) __builtin_bswap32(x)
# define be32toh(x) (x)
# define le32toh(x) __builtin_bswap32(x)

# define htobe64(x) (x)
# define htole64(x) __builtin_bswap64(x)
# define be64toh(x) (x)
# define le64toh(x) __builtin_bswap64(x)

# else
# error Unexpected BYTE_ORDER.

# endif /* _MSC_VER BYTE_ORDER */

#elif defined(__APPLE__)
 /* This is a simple compatibility shim to convert
 * BSD/Linux endian macros to the Mac OS X equivalents. */
#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#elif defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>

#elif defined(__OpenBSD__)

#include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#include <sys/endian.h>

#define be16toh(x) betoh16(x)
#define le16toh(x) letoh16(x)

#define be32toh(x) betoh32(x)
#define le32toh(x) letoh32(x)

#define be64toh(x) betoh64(x)
#define le64toh(x) letoh64(x)

#endif

typedef Bit8u *HostPt;		/* host (virtual) memory address aka ptr */

typedef Bit32u PhysPt;		/* guest physical memory pointer */
typedef Bit32u LinearPt;	/* guest linear memory address */
typedef Bit32u RealPt;		/* guest real-mode memory address (16:16 -> seg:offset) */
typedef Bit16u SegmentVal;	/* guest segment value */

typedef Bit32s MemHandle;

#define MEM_PAGESIZE 4096

extern HostPt MemBase;
HostPt GetMemBase(void);

bool MEM_A20_Enabled(void);
void MEM_A20_Enable(bool enable);

/* Memory management / EMS mapping */
HostPt MEM_GetBlockPage(void);
Bitu MEM_FreeTotal(void);			//Free 4 kb pages
Bitu MEM_FreeLargest(void);			//Largest free 4 kb pages block
Bitu MEM_TotalPages(void);			//Total amount of 4 kb pages
Bitu MEM_AllocatedPages(MemHandle handle); // amount of allocated pages of handle
MemHandle MEM_AllocatePages(Bitu pages,bool sequence);
MemHandle MEM_AllocatePages_A20_friendly(Bitu pages,bool sequence);
MemHandle MEM_GetNextFreePage(void);
PhysPt MEM_AllocatePage(void);
void MEM_ReleasePages(MemHandle handle);
bool MEM_ReAllocatePages(MemHandle & handle,Bitu pages,bool sequence);

MemHandle MEM_NextHandle(MemHandle handle);
MemHandle MEM_NextHandleAt(MemHandle handle,Bitu where);

/* 
	The folowing six functions are used everywhere in the end so these should be changed for
	Working on big or little endian machines 
*/

#if !defined(C_UNALIGNED_MEMORY)
/* meaning: we're probably being compiled for a processor that doesn't like unaligned WORD access,
            on such processors typecasting memory as uint16_t and higher can cause a fault if the
	    address is not aligned to that datatype when we read/write through it. */

static INLINE Bit8u host_readb(HostPt off) {
	return *off;
}
static INLINE Bit16u host_readw(HostPt off) {
	return (Bit16u)host_readb(off) + ((Bit16u)host_readb(off+1) << (Bit16u)8);
}
static INLINE Bit32u host_readd(HostPt off) {
	return (Bit32u)host_readw(off) + ((Bit32u)host_readw(off+2) << (Bit32u)16);
}
static INLINE Bit64u host_readq(HostPt off) {
	return (Bit64u)host_readd(off) + ((Bit64u)host_readd(off+4) << (Bit64u)32);
}

static INLINE void host_writeb(HostPt off,Bit8u val) {
	*off = val;
}
static INLINE void host_writew(HostPt off,Bit16u val) {
	host_writeb(off,(Bit8u)(val));
	host_writeb(off+1,(Bit8u)(val >> (Bit16u)8));
}
static INLINE void host_writed(HostPt off,Bit32u val) {
	host_writew(off,(Bit16u)(val));
	host_writew(off+2,(Bit16u)(val >> (Bit32u)16));
}
static INLINE void host_writeq(HostPt off,Bit64u val) {
	host_writed(off,(Bit32u)(val));
	host_writed(off+4,(Bit32u)(val >> (Bit64u)32));
}

#else

static INLINE Bit8u host_readb(HostPt off) {
	return *(Bit8u *)off;
}
static INLINE Bit16u host_readw(HostPt off) {
	return le16toh((*(Bit16u *)off)); // BSD endian.h
}
static INLINE Bit32u host_readd(HostPt off) {
	return le32toh((*(Bit32u *)off)); // BSD endian.h
}
static INLINE Bit64u host_readq(HostPt off) {
	return le64toh((*(Bit64u *)off)); // BSD endian.h
}

static INLINE void host_writeb(HostPt off,Bit8u val) {
	*(Bit8u *)(off) = val;
}
static INLINE void host_writew(HostPt off,Bit16u val) {
	*(Bit16u *)(off) = htole16(val);
}
static INLINE void host_writed(HostPt off,Bit32u val) {
	*(Bit32u *)(off) = htole32(val);
}
static INLINE void host_writeq(HostPt off,Bit64u val) {
	*(Bit64u *)(off) = htole64(val);
}

#endif


static INLINE void var_write(Bit8u * var, Bit8u val) {
	host_writeb((HostPt)var, val);
}

static INLINE void var_write(Bit16u * var, Bit16u val) {
	host_writew((HostPt)var, val);
}

static INLINE void var_write(Bit32u * var, Bit32u val) {
	host_writed((HostPt)var, val);
}

static INLINE void var_write(Bit64u * var, Bit64u val) {
	host_writeq((HostPt)var, val);
}

/* The Folowing six functions are slower but they recognize the paged memory system */

Bit8u  mem_readb(PhysPt pt);
Bit16u mem_readw(PhysPt pt);
Bit32u mem_readd(PhysPt pt);

void mem_writeb(PhysPt pt,Bit8u val);
void mem_writew(PhysPt pt,Bit16u val);
void mem_writed(PhysPt pt,Bit32u val);

void phys_writes(PhysPt addr, const char* string, Bitu length);

static INLINE void phys_writeb(PhysPt addr,Bit8u val) {
	host_writeb(MemBase+addr,val);
}
static INLINE void phys_writew(PhysPt addr,Bit16u val){
	host_writew(MemBase+addr,val);
}
static INLINE void phys_writed(PhysPt addr,Bit32u val){
	host_writed(MemBase+addr,val);
}

static INLINE Bit8u phys_readb(PhysPt addr) {
	return host_readb(MemBase+addr);
}
static INLINE Bit16u phys_readw(PhysPt addr){
	return host_readw(MemBase+addr);
}
static INLINE Bit32u phys_readd(PhysPt addr){
	return host_readd(MemBase+addr);
}

/* These don't check for alignment, better be sure it's correct */

void MEM_BlockWrite(PhysPt pt,void const * const data,Bitu size);
void MEM_BlockRead(PhysPt pt,void * data,Bitu size);
void MEM_BlockWrite32(PhysPt pt,void * data,Bitu size);
void MEM_BlockRead32(PhysPt pt,void * data,Bitu size);
void MEM_BlockCopy(PhysPt dest,PhysPt src,Bitu size);
void MEM_StrCopy(PhysPt pt,char * data,Bitu size);

void mem_memcpy(PhysPt dest,PhysPt src,Bitu size);
Bitu mem_strlen(PhysPt pt);
void mem_strcpy(PhysPt dest,PhysPt src);

/* The folowing functions are all shortcuts to the above functions using physical addressing */

static INLINE Bit8u real_readb(Bit16u seg,Bit16u off) {
	return mem_readb((seg<<4)+off);
}
static INLINE Bit16u real_readw(Bit16u seg,Bit16u off) {
	return mem_readw((seg<<4)+off);
}
static INLINE Bit32u real_readd(Bit16u seg,Bit16u off) {
	return mem_readd((seg<<4)+off);
}

static INLINE void real_writeb(Bit16u seg,Bit16u off,Bit8u val) {
	mem_writeb(((seg<<4)+off),val);
}
static INLINE void real_writew(Bit16u seg,Bit16u off,Bit16u val) {
	mem_writew(((seg<<4)+off),val);
}
static INLINE void real_writed(Bit16u seg,Bit16u off,Bit32u val) {
	mem_writed(((seg<<4)+off),val);
}


static INLINE Bit16u RealSeg(RealPt pt) {
	return (Bit16u)(pt>>16);
}

static INLINE Bit16u RealOff(RealPt pt) {
	return (Bit16u)(pt&0xffff);
}

static INLINE PhysPt Real2Phys(RealPt pt) {
	return (RealSeg(pt)<<4) +RealOff(pt);
}

static INLINE PhysPt PhysMake(Bit16u seg,Bit16u off) {
	return (seg<<4)+off;
}

static INLINE RealPt RealMake(Bit16u seg,Bit16u off) {
	return (seg<<16)+off;
}

static INLINE void RealSetVec(Bit8u vec,RealPt pt) {
	mem_writed(vec<<2,pt);
}

static INLINE void RealSetVec(Bit8u vec,RealPt pt,RealPt &old) {
	old = mem_readd(vec<<2);
	mem_writed(vec<<2,pt);
}

/* convert physical address to 4:16 real pointer (example: 0xABCDE -> 0xA000:0xBCDE) */
static INLINE RealPt PhysToReal416(PhysPt phys) {
	return RealMake((phys>>4)&0xF000,phys&0xFFFF);
}

static INLINE RealPt RealGetVec(Bit8u vec) {
	return mem_readd(vec<<2);
}	

#endif

