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

#ifndef __APIC_H
#define __APIC_H

#define APIC_BASE           0xfee00000

#define APIC_ID             (APIC_BASE + 0x20)
#define APIC_VERSION        (APIC_BASE + 0x30)
#define APIC_TPR            (APIC_BASE + 0x80)
#define APIC_APR            (APIC_BASE + 0x90)
#define APIC_PPR            (APIC_BASE + 0xa0)
#define APIC_EOIR           (APIC_BASE + 0xb0)
#define APIC_RRD            (APIC_BASE + 0xc0)
#define APIC_LDR            (APIC_BASE + 0xd0)
#define APIC_DFR            (APIC_BASE + 0xe0)
#define APIC_SVR            (APIC_BASE + 0xf0)

#define APIC_ISR0           (APIC_BASE + 0x100)
#define APIC_ISR1           (APIC_BASE + 0x110)
#define APIC_ISR2           (APIC_BASE + 0x120)
#define APIC_ISR3           (APIC_BASE + 0x130)
#define APIC_ISR4           (APIC_BASE + 0x140)
#define APIC_ISR5           (APIC_BASE + 0x150)
#define APIC_ISR6           (APIC_BASE + 0x160)
#define APIC_ISR7           (APIC_BASE + 0x170)

#define APIC_TMR0           (APIC_BASE + 0x180)
#define APIC_TMR1           (APIC_BASE + 0x190)
#define APIC_TMR2           (APIC_BASE + 0x1a0)
#define APIC_TMR3           (APIC_BASE + 0x1b0)
#define APIC_TMR4           (APIC_BASE + 0x1c0)
#define APIC_TMR5           (APIC_BASE + 0x1d0)
#define APIC_TMR6           (APIC_BASE + 0x1e0)
#define APIC_TMR7           (APIC_BASE + 0x1f0)

#define APIC_IRR0           (APIC_BASE + 0x200)
#define APIC_IRR1           (APIC_BASE + 0x210)
#define APIC_IRR2           (APIC_BASE + 0x220)
#define APIC_IRR3           (APIC_BASE + 0x230)
#define APIC_IRR4           (APIC_BASE + 0x240)
#define APIC_IRR5           (APIC_BASE + 0x250)
#define APIC_IRR6           (APIC_BASE + 0x260)
#define APIC_IRR7           (APIC_BASE + 0x270)

#define APIC_ESR            (APIC_BASE + 0x280)

#define APIC_ICR_LOW        (APIC_BASE + 0x300)
#define APIC_ICR_HIGH       (APIC_BASE + 0x310)

#define APIC_LVT_CMCI       (APIC_BASE + 0x2f0)
#define APIC_LVT_TIMER      (APIC_BASE + 0x320)
#define APIC_LVT_THERMAL    (APIC_BASE + 0x330)
#define APIC_LVT_PERF       (APIC_BASE + 0x340)
#define APIC_LVT_LINT0      (APIC_BASE + 0x350)
#define APIC_LVT_LINT1      (APIC_BASE + 0x360)
#define APIC_LVT_ERROR      (APIC_BASE + 0x370)

#define APIC_TIMER_INITCNT  (APIC_BASE + 0x380)
#define APIC_TIMER_CURCNT   (APIC_BASE + 0x390)
#define APIC_TIMER_DIVISOR  (APIC_BASE + 0x3e0)

#define APIC_TIMER_ONESHOT  0
#define APIC_TIMER_PERIODIC 0x20000
#define APIC_SVR_ENABLE     0x100

/* definitions for the IO-APIC */

#define IOAPIC_LINES        24

#define IOAPIC_BASE         0xfec00000

#define IOAPIC_INDEX        (IOAPIC_BASE + 0x00)
#define IOAPIC_DATA         (IOAPIC_BASE + 0x10)
#define IOAPIC_IRQ_PAR      (IOAPIC_BASE + 0x20)
#define IOAPIC_EOIR         (IOAPIC_BASE + 0x40)

#define IOAPIC_IDX_IDR      0
#define IOAPIC_IDX_VERSION  1
#define IOAPIC_IDX_RTL      0x10
#define IOAPIC_IDX_RTH      0x11

#define IMCR_COMM           0x22
#define IMCR_DATA           0x23
#define IMCR_ADDR           0x70

#define IMCR_APIC           1
#define IMCR_I8259          0

#endif /* __APIC_H */
