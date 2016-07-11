/*
 * This file is part of the Corax operating system.
 * Copyright (C) 2016 Matthias Kruk
 *
 * Corax is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Corax is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Corax.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <corax/types.h>
#include <debug.h>

#define ABS(x)  (((x) < 0) ? (-(x)) : (x))

#define VGA_BLACK           0
#define VGA_BLUE            1
#define VGA_GREEN           2
#define VGA_CYAN            3
#define VGA_RED             4
#define VGA_MAGENTA         5
#define VGA_BROWN           6
#define VGA_LIGHT_GRAY      7
#define VGA_GRAY            8
#define VGA_LIGHT_BLUE      9
#define VGA_LIGHT_GREEN     0xa
#define VGA_LIGHT_CYAN      0xb
#define VGA_LIGHT_RED       0xc
#define VGA_LIGHT_MAGENTA   0xd
#define VGA_YELLOW          0xe
#define VGA_WHITE           0xf

#define VGA_FONT(c)         ((u16_t)(((c) & 0xf) << 8))
#define VGA_BACKGROUND(c)   ((u16_t)(((c) & 0xf) << 12))

#define VGA_BUFFER_ADDR    ((void*)0xb8000)
#define VGA_BUFFER_COLS    80
#define VGA_BUFFER_ROWS    24
#define VGA_ROW_BYTES      (VGA_BUFFER_COLS << 1)
#define VGA_SCROLL_SIZE    (VGA_BUFFER_COLS * (VGA_BUFFER_ROWS - 1))
#define VGA_SCROLL_BYTES   (VGA_BUFFER_SIZE << 1)
#define VGA_BUFFER_SIZE    (VGA_BUFFER_COLS * VGA_BUFFER_ROWS)
#define VGA_BUFFER_BYTES   (VGA_BUFFER_SIZE << 1)

static const char alpha_lc[] = "0123456789abcdef";
static const char alpha_uc[] = "0123456789ABCDEF";
static const char nullstr[] = "(null)";

static u16_t* const vga_buffer = VGA_BUFFER_ADDR;
static u16_t vga_attrs = VGA_FONT(VGA_LIGHT_GRAY) | VGA_BACKGROUND(VGA_BLACK);
static i32_t vga_offset = VGA_BUFFER_SIZE;

#define VGA_SET_FONT(c)         vga_attrs = (vga_attrs & 0xf0ff) | VGA_FONT(c)
#define VGA_SET_BACKGROUND(c)   vga_attrs = (vga_attrs & 0x0fff) | VGA_BACKGROUND(c)

i64_t _i64div(i64_t dividend, i64_t divisor) {
    i64_t quotient;
    int sign;

    if(divisor == 0) {
        PANIC("Can't divide by zero");
    }

    quotient = 0;
    sign = 1;

    if(dividend < 0) {
        sign = -sign;
        dividend = -dividend;
    }

    if(divisor < 0) {
        sign = -sign;
        divisor = -divisor;
    }

    while(dividend >= divisor) {
        dividend -= divisor;
        quotient++;
    }

    return(sign > 0 ? quotient : -quotient);
}

i64_t _i64mod(i64_t dividend, i64_t divisor) {
    if(divisor == 0) {
        PANIC("Can't divide by zero");
    }

    if(dividend < 0) {
        dividend = -dividend;
    }

    if(divisor < 0) {
        divisor = -divisor;
    }

    while(dividend >= divisor) {
        dividend -= divisor;
    }

    return(dividend);
}

u64_t _u64div(u64_t dividend, u64_t divisor) {
    u64_t quotient = 0;

    if(divisor == 0) {
        PANIC("Can't divide by zero");
    }

    while(dividend >= divisor) {
        dividend -= divisor;
        quotient++;
    }
    return(quotient);
}

u64_t _u64mod(u64_t dividend, u64_t divisor) {
    if(divisor == 0) {
        PANIC("Can't divide by zero");
    }

    while(dividend >= divisor) {
        dividend -= divisor;
    }

    return(dividend);
}

void* memset(void *dst, int val, u32_t n) {
    char *ptr;

    ptr = dst;
    while(n--) {
        *ptr++ = val;
    }
    return(dst);
}

void* memcpy(void *dst, const void *src, u32_t n) {
    char *d;
    const char *s;

    d = dst;
    s = src;
    while(n--) {
        *d++ = *s++;
    }
    return(dst);
}

static void _putchar(int c) {
    if(c == '\n') {
        vga_offset += VGA_BUFFER_COLS - (vga_offset % VGA_BUFFER_COLS);
    } else {
        if(vga_offset >= VGA_BUFFER_SIZE) {
            vga_offset = VGA_SCROLL_SIZE;
            memcpy((void*)vga_buffer, (void*)vga_buffer + VGA_ROW_BYTES, VGA_SCROLL_BYTES);
            memset((void*)vga_buffer + VGA_SCROLL_BYTES, 0, VGA_ROW_BYTES);
        }

        vga_buffer[vga_offset++] = vga_attrs | (c & 0xff);
    }

    return;
}

int dbg_printf(const char *format, ...) {
    int n;
    char c;
    char size;
    u32_t *arg;
    int mode;
    u32_t flags;
    int pad;
    const char *fmt;

    #define MODE_NORMAL 0
    #define MODE_CONV   1
    #define FLAG_SIGN   (1 << 31)
    #define FLAG_PAD    (1 << 30)
    #define FLAG_LCHEX  (1 << 29)

    n = 0;
    fmt = format;
    arg = ((u32_t*)&format) + 1;
    mode = 0;
    flags = 0;
    size = 0;
    pad = 0;

    while((c = *fmt++)) {
        if(c == '%') {
            if(mode == MODE_CONV) {
                _putchar('%');
                n++;
                mode = MODE_NORMAL;
            } else {
                mode = MODE_CONV;
                flags = 0;
                size = 0;
                pad = 0;
            }
            continue;
        }

        if(mode == MODE_CONV) {
            switch(c) {
                case '+':
                    flags |= FLAG_SIGN;
                    break;
                case '*':
                    /* TODO: Implement formats like %*s */
                    flags = (flags & 0xffff0000) | ((int)*arg++ & 0xffff);
                    break;
                case 'h':
                    if(size == 0) {
                        size = 2;
                    } else if(size == 2) {
                        size = 1;
                    }
                    break;
                case 'l':
                    if(size == 0) {
                        size = 4;
                    } else if(size == 4) {
                        case 'L': /* for double-precision floats, i.e. %Lf */
                        size = 8;
                    }
                    break;
                case '0':
                    if(flags & FLAG_PAD) {
                        pad *= 10;
                    } else {
                        flags |= FLAG_PAD;
                    }
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    pad *= 10;
                    pad += (c - '0');
                    break;
                case 'd':
                case 'i': {
                    i64_t i_arg;
                    char sstr[20];
                    int i;
                    char sign;

                    switch(size) {
                        case 1:
                            i_arg = (i8_t)(*arg++ & 0xff);
                            break;
                        case 2:
                            i_arg = (i16_t)(*arg++ & 0xffff);
                            break;
                        case 0:
                        case 4:
                            i_arg = (i32_t)(*arg++ & 0xffffffff);
                            break;
                        case 8:
                            i_arg = *((i64_t*)arg);
                            arg += 2;
                            break;
                        default:
                            PANIC("Invalid argument width in format string\n");
                    }

                    sign = (i_arg < 0) ? '-' : ((flags & FLAG_SIGN) ? '+' : 0);
                    i = 0;

                    do {
                        i64_t m;

                        m = _i64mod(i_arg, 10);
                        sstr[i++] = '0' + ABS(m);
                        i_arg = _i64div(i_arg, 10);
                    } while(i_arg != 10);

                    if(sign) {
                        _putchar(sign);
                        n++;
                    }

                    if(flags & FLAG_PAD) {
                        pad -= i;

                        while(pad-- > 0) {
                            _putchar('0');
                            n++;
                        }
                    }

                    for(i--; i >= 0; i--) {
                        _putchar(sstr[i]);
                        n++;
                    }
                    mode = MODE_NORMAL;
                    break;
                }
                case 'u': {
                    u64_t u_arg;
                    char sstr[20];
                    int i;

                    i = 0;

                    switch(size) {
                        case 1:
                            u_arg = (u8_t)(*arg++ & 0xff);
                            break;
                        case 2:
                            u_arg = (u16_t)(*arg++ & 0xffff);
                            break;
                        case 0:
                        case 4:
                            u_arg = (u32_t)(*arg++ & 0xffffffff);
                            break;
                        case 8:
                            u_arg = *((u64_t*)arg);
                            arg += 2;
                            break;
                        default:
                            PANIC("Invalid argument width in format string\n");
                    }

                    do {
                        sstr[i++] = '0' + _u64mod(u_arg, 10);
                        u_arg = _u64div(u_arg, 10);
                    } while(u_arg > 0);

                    if(flags & FLAG_SIGN) {
                        _putchar('+');
                        n++;
                    }

                    if(flags & FLAG_PAD) {
                        pad -= i;

                        while(pad-- > 0) {
                            _putchar('0');
                            n++;
                        }
                    }

                    for(i--; i >= 0; i--) {
                        _putchar(sstr[i]);
                        n++;
                    }

                    mode = MODE_NORMAL;
                    break;
                }
                case 'p':
                    _putchar('0');
                    _putchar('x');
                    n += 2;
                case 'x':
                    flags |= FLAG_LCHEX;
                case 'X': {
                    union {
                        u64_t   u64[1];
                        u32_t   u32[2];
                        u16_t   u16[4];
                        u8_t    u8[8];
                    } value;
                    char sstr[16];
                    const char *alpha;
                    int i, j;

                    switch(size) {
                        case 1:
                            value.u8[7] = (u8_t)(*arg++ & 0xff);
                            break;
                        case 2:
                            value.u16[3] = (u16_t)(*arg++ & 0xffff);
                            break;
                        case 0:
                            size = 4;
                        case 4:
                            value.u32[1] = (u32_t)(*arg++ & 0xffffffff);
                            break;
                        case 8:
                            value.u64[0] = *((u64_t*)arg);
                            arg += 2;
                            break;
                        default:
                            PANIC("Invalid argument width in format string\n");
                    }
                    alpha = (flags & FLAG_LCHEX) ? alpha_lc : alpha_uc;

                    for(i = 7, j = 0; i >= (8 - size); i--) {
                        sstr[j++] = alpha[(value.u8[i] >> 4) & 0xf];
                        sstr[j++] = alpha[value.u8[i] & 0xf];
                    }

                    for(i = 0; sstr[i] == '0' && i < j; i++);

                    if(flags & FLAG_PAD) {
                        pad -= (j - i);

                        while(pad-- > 0) {
                            _putchar('0');
                            n++;
                        }
                    }

                    for(; i < j; i++) {
                        _putchar(sstr[i]);
                        n++;
                    }

                    mode = MODE_NORMAL;
                    break;
                }
                case 's': {
                    char *str;

                    str = *arg ? (char*)*arg++ : (char*)nullstr;

                    while((c = *str++)) {
                        _putchar(c);
                        n++;
                    }

                    mode = MODE_NORMAL;
                    break;
                }
                default:
                    mode = MODE_NORMAL;
                    break;
            }
        } else {
            _putchar(c);
            n++;
        }
    }

    #undef MODE_NORMAL
    #undef MODE_CONV
    #undef FLAG_SIGN
    #undef FLAG_PAD
    #undef FLAG_LCHEX

    return(n);
}

void dbg_panic(const char *func, const char *file, i32_t line, const char *msg) {
    /* vga_attrs = 0x0400; *//* VGA_FONT(VGA_RED) | VGA_BACKGROUND(VGA_BLACK); */
    VGA_SET_FONT(VGA_RED);

    dbg_printf("PANIC: Kernel bug in %s, %s:%u: %s\n", func, file, line, msg);

    while(1) {
        asm volatile("cli");
        asm volatile("hlt");
    }
}
