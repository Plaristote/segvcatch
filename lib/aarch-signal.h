#pragma once
#ifdef __ARM_ARCH

#  include <signal.h>
#  include <unistd.h>
#  include <sys/syscall.h>
#  include <ucontext.h>

#  define HANDLE_SEGV 1
//#  define HANDLE_FPE 1

#  define SIGNAL_HANDLER(_name)					\
static void _Jv_##_name (int, siginfo_t *,			\
			 void *_p __attribute__ ((__unused__)))

extern "C"
{
  struct kernel_sigaction
  {
    void (*k_sa_sigaction)(int,siginfo_t *,void *);
    unsigned long k_sa_flags;
    void (*k_sa_restorer) (void);
    sigset_t k_sa_mask;
  };
}

#  define MAKE_THROW_FRAME(_exception)

#  define RESTORE(name, syscall) RESTORE2 (name, syscall)
#ifdef __aarch64__
#  define RESTORE2(name, syscall)			\
asm						\
  (						\
   ".text\n"					\
   ".byte 0\n" \
   ".align 16\n"				\
   "__" #name ":\n"				\
   "	mov x8, " #syscall "\n"		\
   "	svc 0\n"				\
   );
#else
#  define RESTORE2(name, syscall)			\
asm						\
  (						\
   ".text\n"					\
   ".align 16\n"				\
   "__" #name ":\n"				\
   "	mov r7, " #syscall "\n"		\
   "	swi 0\n"				\
   );
#endif

/* The return code for realtime-signals.  */
RESTORE (asm_restore_rt, __NR_rt_sigreturn)
void asm_restore_rt (void) asm ("__asm_restore_rt")
  __attribute__ ((visibility ("hidden")));

#  define INIT_SEGV						\
do								\
  {								\
    struct kernel_sigaction act;				\
    act.k_sa_sigaction = _Jv_catch_segv;			\
    sigemptyset (&act.k_sa_mask);				\
    act.k_sa_flags = SA_SIGINFO|0x4000000;			\
    act.k_sa_restorer = asm_restore_rt;				\
    syscall (SYS_rt_sigaction, SIGSEGV, &act, NULL, _NSIG / 8);	\
  }								\
while (0)

#  define INIT_FPE						\
do								\
  {								\
    struct kernel_sigaction act;				\
    act.k_sa_sigaction = _Jv_catch_fpe;				\
    sigemptyset (&act.k_sa_mask);				\
    act.k_sa_flags = SA_SIGINFO|0x4000000;			\
    act.k_sa_restorer = asm_restore_rt;				\
    syscall (SYS_rt_sigaction, SIGFPE, &act, NULL, _NSIG / 8);	\
  }								\
while (0)

#endif
