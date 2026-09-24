/* Original PNGs are compiled offline; no file I/O or PNG decoder on PSP. */
.section .rodata
.balign 16
.global pet_pixels
pet_pixels:
.incbin "assets/generated/pets.rgba4444"
#if !defined(__mips__)
.section .note.GNU-stack,"",@progbits
#endif
