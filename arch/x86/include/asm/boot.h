

#define _BITUL(x)       (1 << (x))

#define X86_CR0_PE_BIT  0 /* Protection flag. */
#define X86_CR0_PE      _BITUL(X86_CR0_PE_BIT)
#define X86_CR0_CD_BIT  30 /* Caching flag. */
#define X86_CR0_CD      _BITUL(X86_CR0_CD_BIT)
#define X86_CR0_PG_BIT  31 /* Paging flag. */
#define X86_CR0_PG      _BITUL(X86_CR0_PG_BIT)

#define X86_CR4_PAE_BIT 5 /* Physical Address Extension */
#define X86_CR4_PAE     _BITUL(X86_CR4_PAE_BIT)
#define X86_CR4_PGE_BIT 7 /* Page Global Enable */
#define X86_CR4_PGE     _BITUL(X86_CR4_PGE_BIT)

#define MSR_EFER        0xc0000080 /* extended feature register */

#define _EFER_LME       8 /* long mode enable */
