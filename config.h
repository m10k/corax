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

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_KERNEL_STACK_SIZE    4096
#define CONFIG_USER_STACK_SIZE      4096

#define CONFIG_APIC     1

#define CONFIG_SMP      0
#define CONFIG_SMP_CPUS 1

/* sanity checks */

#if !defined(CONFIG_SMP_CPUS) || CONFIG_SMP_CPUS == 0
#error "CONFIG_SMP_CPUS can't be undefined or zero"
#endif /* !defined(CONFIG_SMP_CPUS) */

#if CONFIG_SMP == 0 && CONFIG_SMP_CPUS > 1
#error "CONFIG_SMP_CPUS must be 1 if compiling without SMP support"
#endif /* CONFIG_SMP == 0 && CONFIG_SMP_CPUS > 1 */

#endif /* __CONFIG_H */
