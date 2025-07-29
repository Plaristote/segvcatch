/***************************************************************************
 *   Copyright (C) 2009 by VisualData                                      *
 *                                                                         *
 *   Redistributed under LGPL license terms.                               *
 ***************************************************************************/

#include "segvcatch.h"
#include "exceptdefs.h"

#include <stdexcept>
#include <string>

using namespace std;

namespace {

segvcatch::handler handler_segv = 0;
segvcatch::handler handler_ctrlc = 0;
segvcatch::handler handler_fpe = 0;

#if defined __GNUC__ && __linux

#ifdef __i386__
#include "i386-signal.h"
#endif /*__i386__*/

#ifdef __x86_64__
#include "x86_64-signal.h"
#endif /*__x86_64__*/

#ifdef __ARM_ARCH
#include "aarch-signal.h"
#endif /*__ARM_ARCH*/

#endif /*defined __GNUC__ && __linux*/

void default_segv(const segvcatch::hardware_exception_info &info) {
    throw segvcatch::segmentation_fault("Segmentation fault", info);
}

void default_fpe(const segvcatch::hardware_exception_info &info) {
    throw segvcatch::floating_point_error("Floating point error", info);
}

void handle_segv(const segvcatch::hardware_exception_info &info) {
    if (handler_segv)
        handler_segv(info);
}

void handle_fpe(const segvcatch::hardware_exception_info &info) {
    if (handler_fpe)
        handler_fpe(info);
}

void handle_ctrlc(const segvcatch::hardware_exception_info &info) {
    if (handler_ctrlc)
        handler_ctrlc(info);
}

#if defined(HANDLE_SEGV) || defined(HANDLE_FPE)

#include <execinfo.h>

/* Unblock a signal.  Unless we do this, the signal may only be sent
   once.  */
static void unblock_signal(int signum __attribute__((__unused__))) {
#ifdef _POSIX_VERSION
    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, signum);
    sigprocmask(SIG_UNBLOCK, &sigs, NULL);
#endif
}
#endif

#ifdef HANDLE_SEGV

#ifdef __ARM_ARCH
void *retnptr = nullptr;
segvcatch::hardware_exception_info hwinfo;

static void call_handle_segv() { handle_segv(hwinfo); }

static void call_handle_ctrlc() { handle_ctrlc(hwinfo); }

#ifdef __aarch64__
static void __attribute__((naked)) in_context_signal_handler() {
    handle_segv(hwinfo);

    // This code should not be executed
    default_segv(hwinfo);
    asm("");
}

static void __attribute__((naked)) in_context_signal_handler_ctrlc() {
    handle_ctrlc(hwinfo);

    // This code should not be executed
    default_segv(hwinfo);
    asm("");
}
#else
static void __attribute__((naked)) in_context_signal_handler() {
    // save return address
    asm("ldr r0, %0\npush {r0}\n" : "=m"(retnptr) : : "memory");

    // call _handle_segv
    asm("mov pc, %0" : : "r"(call_handle_segv) : "memory");
}

static void __attribute__((naked)) in_context_signal_handler_ctrlc() {
    // save return address
    asm("ldr r0, %0\npush {r0}\n" : "=m"(retnptr) : : "memory");

    // call _handle_ctrlc
    asm("mov pc, %0" : : "r"(call_handle_ctrlc) : "memory");
}
#endif

SIGNAL_HANDLER(catch_segv) {
    unblock_signal(SIGSEGV);
    MAKE_THROW_FRAME(nullp);
    ucontext_t *context = (ucontext_t *)_p;

#ifdef __aarch64__
    retnptr = reinterpret_cast<void *>(context->uc_mcontext.pc);
    context->uc_mcontext.pc = (uintptr_t)in_context_signal_handler;
#else
    retnptr = reinterpret_cast<void *>(context->uc_mcontext.arm_pc);
    context->uc_mcontext.arm_pc = (uintptr_t)in_context_signal_handler;
#endif

    hwinfo.addr = retnptr;
}
#else
void *retnptr = nullptr;
segvcatch::hardware_exception_info hwinfo;

static void call_handle_segv() { handle_segv(hwinfo); }

static void call_handle_fpe() { handle_fpe(hwinfo); }

static void call_handle_ctrlc() { handle_ctrlc(hwinfo); }

static void __attribute__((naked)) in_context_signal_handler() {
    // save return address
    asm("push %0\n\n" : "=m"(retnptr) : : "memory");

    // call _handle_segv
    asm("jmp *%0" : : "r"(call_handle_segv) : "memory");

    asm("int $3\n");
}

static void __attribute__((naked)) in_context_signal_handler_ctrlc() {
    // save return address
    asm("push %0\n\n" : "=m"(retnptr) : : "memory");

    // call _handle_segv
    asm("jmp *%0" : : "r"(call_handle_ctrlc) : "memory");

    asm("int $3\n");
}

static void __attribute__((naked)) in_context_signal_handler_fpe() {
    // save return address
    asm("push %0\n\n" : "=m"(retnptr) : : "memory");

    // call _handle_segv
    asm("jmp *%0" : : "r"(call_handle_fpe) : "memory");

    asm("int $3\n");
}

SIGNAL_HANDLER(catch_segv) {
    unblock_signal(SIGSEGV);
    MAKE_THROW_FRAME(nullp);
    ucontext_t *context = (ucontext_t *)_p;

    hwinfo.addr = (void *)context->uc_mcontext.gregs[REG_RIP];

    retnptr = (void *)context->uc_mcontext.gregs[REG_RIP];
    context->uc_mcontext.gregs[REG_RIP] = (greg_t)in_context_signal_handler;
}
#endif
#endif

#ifdef __ARM_ARCH
SIGNAL_HANDLER(catch_ctrlc) {
    unblock_signal(SIGINT);
    MAKE_THROW_FRAME(nullp);
    ucontext_t *context = (ucontext_t *)_p;

#ifdef __aarch64__
    retnptr = context->uc_mcontext.pc;
    context->uc_mcontext.pc = (uintptr_t)in_context_signal_handler_ctrlc;
#else
    retnptr = reinterpret_cast<void *>(context->uc_mcontext.arm_pc);
    context->uc_mcontext.arm_pc = (uintptr_t)in_context_signal_handler_ctrlc;
#endif

    hwinfo.addr = retnptr;
}
#else
SIGNAL_HANDLER(catch_ctrlc) {
    unblock_signal(SIGINT);
    MAKE_THROW_FRAME(nullp);
    ucontext_t *context = (ucontext_t *)_p;

    hwinfo.addr = (void *)context->uc_mcontext.gregs[REG_RIP];

    retnptr = (void *)context->uc_mcontext.gregs[REG_RIP];
    context->uc_mcontext.gregs[REG_RIP] = (greg_t)in_context_signal_handler_ctrlc;
}
#endif

#ifdef HANDLE_FPE
#ifdef __ARM_ARCH

static void call_handle_fpe() { handle_fpe(hwinfo); }

static void __attribute__((naked)) in_context_signal_handler_fpe() {
    // save return address
    asm("ldr r0, %0\npush {r0}\n" : "=m"(retnptr) : : "memory");

    // call _handle_segv
    asm("mov pc, %0" : : "r"(call_handle_fpe) : "memory");
}

SIGNAL_HANDLER(catch_fpe) {
    unblock_signal(SIGFPE);
    MAKE_THROW_FRAME(nullp);
    ucontext_t *context = (ucontext_t *)_p;

#ifdef __aarch64__
    retnptr = context->uc_mcontext.pc;
    context->uc_mcontext.pc = (uintptr_t)in_context_signal_handler_fpe;
#else
    retnptr = reinterpret_cast<void *>(context->uc_mcontext.arm_pc);
    context->uc_mcontext.arm_pc = (uintptr_t)in_context_signal_handler_fpe;
#endif

    hwinfo.addr = retnptr;
    // handle_fpe(hwinfo);
}
#else
SIGNAL_HANDLER(catch_fpe) {
    unblock_signal(SIGFPE);
#ifdef HANDLE_DIVIDE_OVERFLOW
    HANDLE_DIVIDE_OVERFLOW;
#else
    MAKE_THROW_FRAME(arithexception);
#endif
    ucontext_t *context = (ucontext_t *)_p;

    hwinfo.addr = (void *)context->uc_mcontext.gregs[REG_RIP];

    retnptr = (void *)context->uc_mcontext.gregs[REG_RIP];
    context->uc_mcontext.gregs[REG_RIP] = (greg_t)in_context_signal_handler_fpe;
}
#endif
#endif

#ifdef WIN32
#include <windows.h>

static LONG CALLBACK win32_exception_handler(LPEXCEPTION_POINTERS e) {
    if (e->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        handle_segv();
        return EXCEPTION_CONTINUE_EXECUTION;
    } else if (e->ExceptionRecord->ExceptionCode ==
               EXCEPTION_INT_DIVIDE_BY_ZERO) {
        handle_fpe();
        return EXCEPTION_CONTINUE_EXECUTION;
    } else
        return EXCEPTION_CONTINUE_SEARCH;
}
#endif
} // namespace

namespace segvcatch {

void init_segv(handler h) {
    if (h)
        handler_segv = h;
    else
        handler_segv = default_segv;
#ifdef HANDLE_SEGV
    INIT_SEGV;
#endif

#ifdef WIN32
    SetUnhandledExceptionFilter(win32_exception_handler);
#endif
}

void init_ctrlc(handler h) {
    handler_ctrlc = h;

    INIT_CTRLC;
}

void init_fpe(handler h) {
    if (h)
        handler_fpe = h;
    else
        handler_fpe = default_fpe;
#ifdef HANDLE_FPE
    INIT_FPE;
#endif

#ifdef WIN32
    SetUnhandledExceptionFilter(win32_exception_handler);
#endif
}

void volatile causes_segv() {
    volatile int *ptr = nullptr;
    *ptr = 0;
}

void donot_optimize_away() {
    // Do nothing
}

} // namespace segvcatch
