

#define _BITUL(x)       (1 << (x))

#define X86_CR0_PE_BIT  0 /* Protection flag. */
#define X86_CR0_PE      _BITUL(X86_CR0_PE_BIT)
#define X86_CR0_CD_BIT  30 /* Caching flag. */
#define X86_CR0_CD      _BITUL(X86_CR0_CD_BIT)
#define X86_CR0_PG_BIT  31 /* Paging flag. */
#define X86_CR0_PG      _BITUL(X86_CR0_PG_BIT)
