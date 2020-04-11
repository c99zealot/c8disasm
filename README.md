# Chip8disasm

## usage:
```
cc chip8disasm.c
./a.out <chip8 rom>
```
## notes:
- program counter values are displayed to the left of the disassembly in chip8disasm output
- ret instructions are shown with the address they would return to, e.g. `|  0x210  |    ret (0x205)`
