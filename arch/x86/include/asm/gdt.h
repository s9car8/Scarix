
#define GDT_ENTRY_BOOT_CS       0x2
#define GDT_ENTRY_BOOT_DS       (GDT_ENTRY_BOOT_CS + 1)
#define GDT_ENTRY_BOOT_TSS      (GDT_ENTRY_BOOT_DS + 1)

#define BOOT_CS                 (GDT_ENTRY_BOOT_CS << 3)
#define BOOT_DS                 (GDT_ENTRY_BOOT_DS << 3)
#define BOOT_TSS                (GDT_ENTRY_BOOT_TSS << 3)
