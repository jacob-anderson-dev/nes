#include <stdio.h>

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Memory {
    static constexpr u32 MAX_MEMORY = 1024 * 64;
    Byte data[MAX_MEMORY];

    void initialize() {
        for (u32 i = 0; i < MAX_MEMORY; i++) {
            data[i] = 0;
        }
    }

    // Read 1 byte.
    Byte operator[](u32 address) const {
        return data[address];
    }

    // Write 1 byte.
    Byte& operator[](u32 address) {
        return data[address];
    }
};

struct Mos_6502 {

    // Memory pointers.
    Word
        program_counter,
        stack_pointer;

    // Registers.
    Byte
        accumulator,
        x,
        y;

    // Processor Status Flags.
    Byte
        carry_flag : 1,
        zero_flag : 1,
        interrupt_disable : 1,
        decimal_mode : 1,
        break_command : 1,
        overflow_flag : 1,
        negative_flag : 1;

    // Opcodes.
    static constexpr Byte
        INSTRUCTION_LDA_IM = 0xA9,
        INSTRUCTION_LDA_ZP = 0xA5;

    // Operation helper functions.
    void LDA_set_flags() {
        zero_flag = accumulator == 0;
        negative_flag = accumulator & 0b10000000;
    }

    Byte fetch_byte(u32& cycles, Memory& memory) {
        Byte data = memory[program_counter];
        program_counter++;
        cycles--;
        return data;
    }

    Byte read_byte(u32& cycles, Byte address, Memory& memory) {
        Byte data = memory[address];
        cycles--;
        return data;
    }

    // Fetch Decode Execute cycles.
    void reset(Memory& memory) {
        program_counter = 0xFFFC;
        stack_pointer = 0x0100;
        carry_flag = zero_flag = interrupt_disable = decimal_mode = break_command = overflow_flag = negative_flag = 0;
        accumulator = x = y = 0;
        memory.initialize();
    }

    void execute(u32 cycles, Memory& memory) {
        while (cycles > 0) {
            Byte instruction = fetch_byte(cycles, memory);

            // The Gauntlet of Instructions.
            switch (instruction) {
                case INSTRUCTION_LDA_IM: {
                    Byte value = fetch_byte(cycles, memory);
                    accumulator = value;
                    LDA_set_flags();
                } break;
                case INSTRUCTION_LDA_ZP: {
                    Byte zero_page_address = fetch_byte(cycles, memory);
                    accumulator = read_byte(cycles, zero_page_address, memory);
                    LDA_set_flags();
                } break;
                default: {
                    printf("Instruction not handled %d\n", instruction);
                } break;
            }
        }
    }
};

int main() {

    Memory memory;
    Mos_6502 cpu;

    cpu.reset(memory);

    // BEGIN: inline program
    memory[0xFFFC] = Mos_6502::INSTRUCTION_LDA_ZP;
    memory[0xFFFD] = 0x42;
    memory[0x0042] = 0x84;
    // END: inline program

    cpu.execute(3, memory);

    return 0;
}
