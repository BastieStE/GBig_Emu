#include <cpu.h>

void load_bios(const char *bios_filename) {
    FILE *bios_file = fopen(bios_filename, "rb");
    if (!bios_file) {
        printf("Error: BIOS file could not be opened.\n");
        exit(1);  // Exit if the BIOS file can't be opened
    }

    // Read BIOS into a buffer
    uint8_t bios[BIOS_SIZE];
    size_t bytes_read = fread(bios, 1, BIOS_SIZE, bios_file);
    if (bytes_read != BIOS_SIZE) {
        printf("Error: BIOS file does not have the expected size.\n");
        fclose(bios_file);
        exit(1);
    }

    for (uint16_t i = 0; i < BIOS_SIZE; i++) {
        bus_write(i, bios[i]);
    }

    printf("BIOS loaded successfully.\n");

    fclose(bios_file);
}

void execute_bios(cpu_context *cpu) {
    while (cpu->regi.pc < BIOS_SIZE) {
        execute_instruction(cpu);
    }
}
