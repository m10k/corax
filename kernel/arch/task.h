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

#ifndef __TASK_H
#define __TASK_H

#include <config.h>

/* Definition of an architectural task - think of it
 * as a TCB. It is not a operating system process or
 * thread, but merely the information needed by the
 * CPU to perform context switches, etc.
 *
 * Specifically, we need to remember the task's page
 * directory and the stack pointer. The task's context
 * is on top of its stack, i.e. all we have to do is
 * load the task's stack pointer and execute an iret */

struct task {
    u32_t   t_esp;
    u32_t   t_cr3;

    u8_t    t_kstack[CONFIG_USER_STACK_SIZE];
} __attribute__((packed));

struct task* _task_prepare(struct task *t, u32_t eip, u32_t esp);
void _task_switch(struct task*);

#endif /* __TASK_H */
