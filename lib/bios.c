#include <bus.h>

void load_bios(const char *bios_path) {
    FILE *bios_file = fopen(bios_path, "rb");
    if (bios_file == NULL) {
        fprintf(stderr, "Error: Could not load BIOS file\n");
        return;
    }

    // Read BIOS data into the memory space reserved for the BIOS (0x0000–0x00FF)
    uint8_t bios_data[BIOS_SIZE];
    fread(bios_data, 1, BIOS_SIZE, bios_file);
    fclose(bios_file);

    // Load BIOS into the bus memory (0x0000–0x00FF)
    for (int i = 0; i < BIOS_SIZE; ++i) {
        bus_write(i, bios_data[i]);
    }
}
