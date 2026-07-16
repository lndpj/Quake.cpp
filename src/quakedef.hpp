// quakedef.h -- primary header for client
#pragma once

//#define	GLTEST			// experimental stuff

#define QUAKE_GAME // as opposed to utilities

#undef VERSION
#define VERSION 1.09


//define	PARANOID			// speed sapping error checking

#define GAMENAME "id1"

#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <stdexcept>
#include "core_types.hpp"

#if !defined(_WIN32)
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

typedef int errno_t;

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

#ifndef STRUNCATE
#define STRUNCATE 80
#endif

inline errno_t strcpy_s(char* dest, size_t destsz, const char* src) {
    if (!dest || destsz == 0) return EINVAL;
    if (!src) {
        dest[0] = '\0';
        return EINVAL;
    }
    size_t src_len = strlen(src);
    if (src_len >= destsz) {
        dest[0] = '\0';
        return ERANGE;
    }
    memcpy(dest, src, src_len + 1);
    return 0;
}

inline errno_t strncpy_s(char* dest, size_t destsz, const char* src, size_t count) {
    if (!dest || destsz == 0) return EINVAL;
    if (!src) {
        dest[0] = '\0';
        return EINVAL;
    }
    if (count == _TRUNCATE) {
        size_t src_len = strlen(src);
        if (src_len >= destsz) {
            memcpy(dest, src, destsz - 1);
            dest[destsz - 1] = '\0';
            return STRUNCATE;
        }
        memcpy(dest, src, src_len + 1);
        return 0;
    } else {
        size_t src_len = strlen(src);
        size_t copy_len = (src_len < count) ? src_len : count;
        if (copy_len >= destsz) {
            dest[0] = '\0';
            return ERANGE;
        }
        memcpy(dest, src, copy_len);
        dest[copy_len] = '\0';
        return 0;
    }
}

inline errno_t strcat_s(char* dest, size_t destsz, const char* src) {
    if (!dest || destsz == 0) return EINVAL;
    if (!src) return EINVAL;
    size_t dest_len = strlen(dest);
    if (dest_len >= destsz) {
        dest[0] = '\0';
        return EINVAL;
    }
    size_t src_len = strlen(src);
    if (dest_len + src_len >= destsz) {
        dest[0] = '\0';
        return ERANGE;
    }
    memcpy(dest + dest_len, src, src_len + 1);
    return 0;
}

inline int sprintf_s(char* dest, size_t destsz, const char* format, ...) {
    if (!dest || destsz == 0) return -1;
    va_list args;
    va_start(args, format);
    int ret = vsnprintf(dest, destsz, format, args);
    va_end(args);
    if (ret < 0 || (size_t)ret >= destsz) {
        dest[0] = '\0';
        return -1;
    }
    return ret;
}

inline int vsprintf_s(char* dest, size_t destsz, const char* format, va_list args) {
    if (!dest || destsz == 0) return -1;
    int ret = vsnprintf(dest, destsz, format, args);
    if (ret < 0 || (size_t)ret >= destsz) {
        dest[0] = '\0';
        return -1;
    }
    return ret;
}

inline errno_t fopen_s(FILE** pFile, const char* filename, const char* mode) {
    if (!pFile || !filename || !mode) return EINVAL;
    *pFile = fopen(filename, mode);
    if (*pFile == nullptr) {
        return errno;
    }
    return 0;
}

inline errno_t strerror_s(char* buf, size_t bufsz, int errnum) {
    if (!buf || bufsz == 0) return EINVAL;
    const char* msg = strerror(errnum);
    if (!msg) {
        buf[0] = '\0';
        return EINVAL;
    }
    size_t msg_len = strlen(msg);
    if (msg_len >= bufsz) {
        memcpy(buf, msg, bufsz - 1);
        buf[bufsz - 1] = '\0';
        return ERANGE;
    }
    memcpy(buf, msg, msg_len + 1);
    return 0;
}

inline int vfscanf_s_compat(FILE* stream, const char* format, va_list ap) {
    void* args[16] = { nullptr };
    int arg_count = 0;

    const char* p = format;
    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == '%') {
                p++;
                continue;
            }
            if (*p == '*') {
                p++;
            } else {
                while (*p && strchr("0123456789+-hljztL", *p)) {
                    p++;
                }
                char type = *p;
                if (type == '[') {
                    p++;
                    if (*p == '^') p++;
                    if (*p == ']') p++;
                    while (*p && *p != ']') {
                        p++;
                    }
                    type = '[';
                }
                if (*p) p++;
                
                void* ptr = va_arg(ap, void*);
                if (arg_count < 16) {
                    args[arg_count++] = ptr;
                }
                if (type == 's' || type == 'c' || type == '[') {
                    (void)va_arg(ap, unsigned int);
                }
                continue;
            }
        }
        p++;
    }

    switch (arg_count) {
        case 0:  return fscanf(stream, format);
        case 1:  return fscanf(stream, format, args[0]);
        case 2:  return fscanf(stream, format, args[0], args[1]);
        case 3:  return fscanf(stream, format, args[0], args[1], args[2]);
        case 4:  return fscanf(stream, format, args[0], args[1], args[2], args[3]);
        case 5:  return fscanf(stream, format, args[0], args[1], args[2], args[3], args[4]);
        case 6:  return fscanf(stream, format, args[0], args[1], args[2], args[3], args[4], args[5]);
        case 7:  return fscanf(stream, format, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
        case 8:  return fscanf(stream, format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
        default: return -1;
    }
}

inline int vsscanf_s_compat(const char* buffer, const char* format, va_list ap) {
    void* args[16] = { nullptr };
    int arg_count = 0;

    const char* p = format;
    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == '%') {
                p++;
                continue;
            }
            if (*p == '*') {
                p++;
            } else {
                while (*p && strchr("0123456789+-hljztL", *p)) {
                    p++;
                }
                char type = *p;
                if (type == '[') {
                    p++;
                    if (*p == '^') p++;
                    if (*p == ']') p++;
                    while (*p && *p != ']') {
                        p++;
                    }
                    type = '[';
                }
                if (*p) p++;
                
                void* ptr = va_arg(ap, void*);
                if (arg_count < 16) {
                    args[arg_count++] = ptr;
                }
                if (type == 's' || type == 'c' || type == '[') {
                    (void)va_arg(ap, unsigned int);
                }
                continue;
            }
        }
        p++;
    }

    switch (arg_count) {
        case 0:  return sscanf(buffer, format);
        case 1:  return sscanf(buffer, format, args[0]);
        case 2:  return sscanf(buffer, format, args[0], args[1]);
        case 3:  return sscanf(buffer, format, args[0], args[1], args[2]);
        case 4:  return sscanf(buffer, format, args[0], args[1], args[2], args[3]);
        case 5:  return sscanf(buffer, format, args[0], args[1], args[2], args[3], args[4]);
        case 6:  return sscanf(buffer, format, args[0], args[1], args[2], args[3], args[4], args[5]);
        case 7:  return sscanf(buffer, format, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
        case 8:  return sscanf(buffer, format, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
        default: return -1;
    }
}

inline int fscanf_s(FILE* stream, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int ret = vfscanf_s_compat(stream, format, ap);
    va_end(ap);
    return ret;
}

inline int sscanf_s(const char* buffer, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int ret = vsscanf_s_compat(buffer, format, ap);
    va_end(ap);
    return ret;
}

#define _SH_DENYNO 0
#define _S_IREAD 0400
#define _S_IWRITE 0200

inline int _sopen_s(int* pfd, const char* filename, int oflag, int shflag, int pmode) {
    (void)shflag;
    if (!pfd) return EINVAL;
    *pfd = open(filename, oflag, pmode);
    if (*pfd == -1) {
        return errno;
    }
    return 0;
}
#define _write write
#define _close close
#define _unlink unlink
#endif


inline void VID_LockBuffer(void) {}
inline void VID_UnlockBuffer(void) {}
#define UNALIGNED_OK 0

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
#define CACHE_SIZE 32 // used to align key data structures

#define UNUSED(x) (x = x) // for pesky compiler / lint warnings

#define MINIMUM_MEMORY 0x550000
#define MINIMUM_MEMORY_LEVELPAK (MINIMUM_MEMORY + 0x100000)

#define MAX_NUM_ARGVS 50

// up / down
#define PITCH 0

// left / right
#define YAW 1

// fall over
#define ROLL 2

#define MAX_QPATH 64   // max length of a quake game pathname
#define MAX_OSPATH 128 // max length of a filesystem pathname

#define ON_EPSILON 0.1 // point on plane side epsilon

#define MAX_MSGLEN 8000   // max length of a reliable message
#define MAX_DATAGRAM 1024 // max length of unreliable message

//
// per-level limits
//
#define MAX_EDICTS 600 // FIXME: ouch! ouch! ouch!
#define MAX_LIGHTSTYLES 64
#define MAX_MODELS 256 // these are sent over the net as bytes
#define MAX_SOUNDS 256 // so they cannot be blindly increased

#define SAVEGAME_COMMENT_LENGTH 39

#define MAX_STYLESTRING 64

//
// stats are integers communicated to the client by the server
//
#define MAX_CL_STATS 32
#define STAT_HEALTH 0
#define STAT_FRAGS 1
#define STAT_WEAPON 2
#define STAT_AMMO 3
#define STAT_ARMOR 4
#define STAT_WEAPONFRAME 5
#define STAT_SHELLS 6
#define STAT_NAILS 7
#define STAT_ROCKETS 8
#define STAT_CELLS 9
#define STAT_ACTIVEWEAPON 10
#define STAT_TOTALSECRETS 11
#define STAT_TOTALMONSTERS 12
#define STAT_SECRETS 13  // bumped on client side by svc_foundsecret
#define STAT_MONSTERS 14 // bumped by svc_killedmonster

// stock defines

#define IT_SHOTGUN 1
#define IT_SUPER_SHOTGUN 2
#define IT_NAILGUN 4
#define IT_SUPER_NAILGUN 8
#define IT_GRENADE_LAUNCHER 16
#define IT_ROCKET_LAUNCHER 32
#define IT_LIGHTNING 64
#define IT_SUPER_LIGHTNING 128
#define IT_SHELLS 256
#define IT_NAILS 512
#define IT_ROCKETS 1024
#define IT_CELLS 2048
#define IT_AXE 4096
#define IT_ARMOR1 8192
#define IT_ARMOR2 16384
#define IT_ARMOR3 32768
#define IT_SUPERHEALTH 65536
#define IT_KEY1 131072
#define IT_KEY2 262144
#define IT_INVISIBILITY 524288
#define IT_INVULNERABILITY 1048576
#define IT_SUIT 2097152
#define IT_QUAD 4194304
#define IT_SIGIL1 (1 << 28)
#define IT_SIGIL2 (1 << 29)
#define IT_SIGIL3 (1 << 30)
#define IT_SIGIL4 (1 << 31)

//===========================================
//rogue changed and added defines

#define RIT_SHELLS 128
#define RIT_NAILS 256
#define RIT_ROCKETS 512
#define RIT_CELLS 1024
#define RIT_AXE 2048
#define RIT_LAVA_NAILGUN 4096
#define RIT_LAVA_SUPER_NAILGUN 8192
#define RIT_MULTI_GRENADE 16384
#define RIT_MULTI_ROCKET 32768
#define RIT_PLASMA_GUN 65536
#define RIT_ARMOR1 8388608
#define RIT_ARMOR2 16777216
#define RIT_ARMOR3 33554432
#define RIT_LAVA_NAILS 67108864
#define RIT_PLASMA_AMMO 134217728
#define RIT_MULTI_ROCKETS 268435456
#define RIT_SHIELD 536870912
#define RIT_ANTIGRAV 1073741824
#define RIT_SUPERHEALTH 2147483648

//MED 01/04/97 added hipnotic defines
//===========================================
//hipnotic added defines
#define HIT_PROXIMITY_GUN_BIT 16
#define HIT_MJOLNIR_BIT 7
#define HIT_LASER_CANNON_BIT 23
#define HIT_PROXIMITY_GUN (1 << HIT_PROXIMITY_GUN_BIT)
#define HIT_MJOLNIR (1 << HIT_MJOLNIR_BIT)
#define HIT_LASER_CANNON (1 << HIT_LASER_CANNON_BIT)
#define HIT_WETSUIT (1 << (23 + 2))
#define HIT_EMPATHY_SHIELDS (1 << (23 + 3))

//===========================================

#define MAX_SCOREBOARD 16
#define MAX_SCOREBOARDNAME 32

#define SOUND_CHANNELS 8

// This makes anyone on id's net privileged
// Use for multiplayer testing only - VERY dangerous!!!
// #define IDGODS

#include "common.hpp"
#include "bspfile.hpp"
#include "vid.hpp"
#include "sys.hpp"
#include "zone.hpp"
#include "mathlib.hpp"

typedef struct {
    Vector3 origin;
    Vector3 angles;
    int modelindex;
    int frame;
    int colormap;
    int skin;
    int effects;
} entity_state_t;

#include "wad.hpp"
#include "draw.hpp"
#include "cvar.hpp"
#include "screen.hpp"
#include "network.hpp"
#include "protocol.hpp"
#include "cmd.hpp"
#include "sbar.hpp"
#include "audio.hpp"
#include "renderer.hpp"
#include "model.hpp"
#include "client.hpp"
#include "vm.hpp"
#include "server.hpp"
#include "rasterizer.hpp"

#include "input.hpp"
#include "world.hpp"
#include "keys.hpp"
#include "console.hpp"
#include "view.hpp"
#include "menu.hpp"
#include "crc.hpp"



//=============================================================================

#include "host.hpp"
