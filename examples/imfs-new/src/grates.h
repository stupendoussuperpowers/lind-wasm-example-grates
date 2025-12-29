#define __IN(size)	__attribute__((annotate("in:" #size)))
#define __OUT(size)	__attribute__((annotate("out:" #size)))
#define __INOUT(size)	__attribute__((annotate("inout:" #size)))

#define __REGISTER(sysnum)	__attribute__((annotate("syscall:" #sysnum)))

