// incbin.h またはソースコードの先頭
#define INCBIN(NAME, FILE) \
    __asm__(".section .rodata\n" \
            ".global incbin_" #NAME "_start\n" \
            ".type incbin_" #NAME "_start, @object\n" \
            ".align 4\n" \
            "incbin_" #NAME "_start:\n" \
            ".incbin \"" FILE "\"\n" \
            "\n" \
            ".global incbin_" #NAME "_end\n" \
            ".type incbin_" #NAME "_end, @object\n" \
            ".align 1\n" \
            "incbin_" #NAME "_end:\n" \
            ".byte 0\n" \
            ); \
    extern const char NAME[] asm("incbin_" #NAME "_start"); \
    extern const char NAME##_end[] asm("incbin_" #NAME "_end"); \
    extern const unsigned int NAME##_size asm("incbin_" #NAME "_size");
