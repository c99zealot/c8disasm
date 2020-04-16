#include <stdio.h>
#include <stdint.h>

#define X ((current_instruction & 0x0f00) >> 8)
#define Y ((current_instruction & 0x00f0) >> 4)

#define N   (current_instruction & 0x000f)
#define NN  (current_instruction & 0x00ff)
#define NNN (current_instruction & 0x0fff)

FILE *input_file;
uint16_t current_byte, current_instruction = 1;
uint8_t file_buffer[4096];

int main(int argc, char **argv) {
        if (argc < 2) {
                fprintf(stderr, "usage: %s <chip8 rom>\n", argv[0]);
                return 1;
        }

        if (!((input_file = fopen(argv[1], "rb")) && fread(file_buffer, 1, 4096, input_file))) {
                fprintf(stderr, "failed to load file `%s`\n", argv[1]);
                return 1;
        }

        while (current_byte < 4096) {
                fprintf(stdout, "|  0x%X  |  ", current_byte + 512);

                // fetch next instruction
                current_instruction = (file_buffer[current_byte] << 8) | file_buffer[current_byte + 1];

                // decode and display fetched instruction
                switch (current_instruction & 0xF000) {
                        case 0x0000:
                                switch (NN) {
                                        case 0xEE:         // 0x00EE
                                                puts("ret");
                                                break;
                                        case 0xE0:
                                                puts("cls");
                                                break;
                                        default:
                                                if (current_instruction == 0) {
                                                        puts("<end of file>");
                                                        return 0;
                                                } else
                                                        puts("???");
                                }
                                break;
                        case 0x1000:        // 0x1NNN
                                printf("jmp 0x%X\n", NNN);
                                break;
                        case 0x2000:        // 0x2NNN
                                printf("call 0x%X\n", NNN);
                                break;
                        case 0x3000:        // 0x3XNN
                                printf("se V%X, 0x%X\n", X, NN);
                                break;
                        case 0x4000:        // 0x4XNN
                                printf("sne V%X, 0x%X\n", X, NN);
                                break;
                        case 0x5000:        // 0x5XY0
                                printf("se V%X, V%X\n", X, Y);
                                break;
                        case 0x6000:        // 0x6XNN
                                printf("mov V%X, 0x%X\n", X, NN);
                                break;
                        case 0x7000:        // 0x7XNN
                                printf("add V%X, 0x%X\n", X, NN);
                                break;
                        case 0x8000:
                                switch (N) {
                                        case 0x0:        // 0x8XY0
                                                printf("mov V%X, V%X\n", X, Y);
                                                break;
                                        case 0x1:        // 0x8XY1
                                                printf("or V%X, V%X\n", X, Y);
                                                break;
                                        case 0x2:        // 0x8XY2
                                                printf("and V%X, V%X\n", X, Y);
                                                break;
                                        case 0x3:        // 0x8XY3
                                                printf("xor V%X, V%X\n", X, Y);
                                                break;
                                        case 0x4:        // 0x8XY4
                                                printf("add V%X, V%X\n", X, Y);
                                                break;
                                        case 0x5:        // 0x8XY5
                                                printf("sub V%X, V%X\n", X, Y);
                                                break;
                                        case 0x6:        // 0x8YX6
                                                printf("shr V%X\n", X);
                                                break;
                                        case 0x7:        // 0x8XY7
                                                printf("subn V%X, V%X\n", X, Y);
                                                break;
                                        case 0xE:        // 0x8XYE
                                                printf("shl V%X\n", X);
                                                break;

                                        default:
                                                puts("???");
                                }
                                break;
                        case 0x9000:        // 0x9XY0
                                printf("sne V%X, V%X\n", X, Y);
                                break;
                        case 0xA000:        // 0xANNN
                                printf("mov I, 0x%X\n", NNN);
                                break;
                        case 0xB000:        // 0xBNNN
                                printf("vjmp 0x%X\n", NNN);
                                break;
                        case 0xC000:        // 0xCXNN
                                printf("rnd V%X, 0x%X\n", X, NN);
                                break;
                        case 0xD000:
                                printf("drw V%X, V%X, 0x%X\n", X, Y, N);
                                break;
                        case 0xE000:
                                switch(N) {
                                        case 0xE:       // 0xEX9E
                                                printf("skd V%X\n", X);
                                                break;
                                        case 0x1:       // 0xExA1
                                                printf("sku V%X\n", X);
                                                break;

                                        default:
                                                puts("???");
                                }
                                break;
                        case 0xF000:
                                switch(N) {
                                        case 0x7:       // 0xFX07
                                                printf("mov V%X, dtimer\n", X);
                                                break;
                                        case 0xA:       // 0xFX0A
                                                printf("wkp V%X\n", X);
                                                break;
                                        case 0x8:       // 0xFX18
                                                printf("mov stimer, V%X\n", X);
                                                break;
                                        case 0xE:       // 0xFX1E
                                                printf("add I, V%X\n", X);
                                                break;
                                        case 0x9:       // 0xFX29
                                                printf("ldf V%X\n", X);
                                                break;
                                        case 0x3:       // 0xFX33
                                                printf("bcd V%X\n", X);
                                                break;
                                        case 0x5:
                                                switch(Y) {
                                                        case 0x1:       // 0xFX15
                                                                printf("mov dtimer, V%X\n", X);
                                                                break;
                                                        case 0x5:       // 0xFX55
                                                                printf("ld V%X\n", X);
                                                                break;
                                                        case 0x6:       // 0xFX65
                                                                printf("st V%X\n", X);
                                                                break;

                                                        default:
                                                                puts("???");
                                                }
                                                break;

                                        default:
                                                puts("???");
                                }
                                break;

                        default:
                                puts("???");
                }
                
                current_byte += 2;
        }

        return 0;
}
