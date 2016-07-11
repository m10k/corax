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

#ifndef __ASSEMBLY_H
#define __ASSEMBLY_H

#ifdef __ASSEMBLY_SOURCE

#define FUNC_PUBLIC(name)   .global name \
                            LABEL(name)

#define FUNC(name)          LABEL(name)
#define LABEL(name)         name:

#endif /* __ASSEMBLY_SOURCE */

#endif /* __ASSEMBLY_H */
