#include <cart.h>

static cart_context cart_ctx;

static const char *ROM_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

// static const char *LIC_CODE[0xA5] = {
//     [0x00] = "None",
//     [0x01] = "Nintendo R&D1",
//     [0x08] = "Capcom",
//     [0x13] = "Electronic Arts",
//     [0x18] = "Hudson Soft",
//     [0x19] = "b-ai",
//     [0x20] = "kss",
//     [0x22] = "pow",
//     [0x24] = "PCM Complete",
//     [0x25] = "san-x",
//     [0x28] = "Kemco Japan",
//     [0x29] = "seta",
//     [0x30] = "Viacom",
//     [0x31] = "Nintendo",
//     [0x32] = "Bandai",
//     [0x33] = "Ocean/Acclaim",
//     [0x34] = "Konami",
//     [0x35] = "Hector",
//     [0x37] = "Taito",
//     [0x38] = "Hudson",
//     [0x39] = "Banpresto",
//     [0x41] = "Ubi Soft",
//     [0x42] = "Atlus",
//     [0x44] = "Malibu",
//     [0x46] = "angel",
//     [0x47] = "Bullet-Proof",
//     [0x49] = "irem",
//     [0x50] = "Absolute",
//     [0x51] = "Acclaim",
//     [0x52] = "Activision",
//     [0x53] = "American sammy",
//     [0x54] = "Konami",
//     [0x55] = "Hi tech entertainment",
//     [0x56] = "LJN",
//     [0x57] = "Matchbox",
//     [0x58] = "Mattel",
//     [0x59] = "Milton Bradley",
//     [0x60] = "Titus",
//     [0x61] = "Virgin",
//     [0x64] = "LucasArts",
//     [0x67] = "Ocean",
//     [0x69] = "Electronic Arts",
//     [0x70] = "Infogrames",
//     [0x71] = "Interplay",
//     [0x72] = "Broderbund",
//     [0x73] = "sculptured",
//     [0x75] = "sci",
//     [0x78] = "THQ",
//     [0x79] = "Accolade",
//     [0x80] = "misawa",
//     [0x83] = "lozc",
//     [0x86] = "Tokuma Shoten Intermedia",
//     [0x87] = "Tsukuda Original",
//     [0x91] = "Chunsoft",
//     [0x92] = "Video system",
//     [0x93] = "Ocean/Acclaim",
//     [0x95] = "Varie",
//     [0x96] = "Yonezawa/s’pal",
//     [0x97] = "Kaneko",
//     [0x99] = "Pack in soft",
//     [0xA4] = "Konami (Yu-Gi-Oh!)"
// };

// static const char *cart_lic_name() {
//     if (cart_ctx.header->new_lic_code <= 0xA4) {
//         return LIC_CODE[cart_ctx.header->lic_code];
//     }

//     return "UNKNOWN";
// }

static const char *cart_type_name() {
    if (cart_ctx.header->type <= 0x22) {
        return ROM_TYPES[cart_ctx.header->type];
    }

    return "UNKNOWN";
}

static void cart_init_ctx()
{
        // Initialize cartridge context
    cart_ctx.header = (rom_header *)(cart_ctx.rom_data + 0x100);
    cart_ctx.header->title[15] = 0;
    cart_ctx.rom_bank = 1;  // Default ROM bank
    cart_ctx.ram_bank = 0;  // Default RAM bank
    cart_ctx.ram_enabled = false;
}

bool cart_load(char *cart) {
    snprintf(cart_ctx.filename, sizeof(cart_ctx.filename), "%s", cart);

    FILE *fp = fopen(cart, "rb");
    if (!fp) {
        printf("Failed to open: %s\n", cart);
        return false;
    }

    printf("Opened: %s\n", cart_ctx.filename);

    // Load ROM data
    fseek(fp, 0, SEEK_END);
    cart_ctx.rom_size = ftell(fp);
    rewind(fp);
    cart_ctx.rom_data = malloc(cart_ctx.rom_size);
    fread(cart_ctx.rom_data, cart_ctx.rom_size, 1, fp);
    fclose(fp);

    cart_init_ctx();

    // Allocate RAM if necessary
    uint8_t ram_size_lookup[] = {0, 2, 8, 32, 128, 64};
    cart_ctx.ram_size = (cart_ctx.header->ram_size < sizeof(ram_size_lookup) ? ram_size_lookup[cart_ctx.header->ram_size] : 0) * RAM_BANK_SIZE;
    if (cart_ctx.ram_size > 0) {
        cart_ctx.ram_data = malloc(cart_ctx.ram_size);
        memset(cart_ctx.ram_data, 0, cart_ctx.ram_size);
    }

    
    printf("Cartridge Loaded:\n");
    printf("\t Title    : %s\n", cart_ctx.header->title);
    printf("\t Type     : %2.2X (%s)\n", cart_ctx.header->type, cart_type_name());
    printf("\t ROM Size : %d KB\n", 32 << cart_ctx.header->rom_size);
    printf("\t RAM Size : %d KB\n", cart_ctx.ram_size / 1024);

    return true;
}


uint8_t cart_read(uint16_t address) {
    if (address <= 0x3FFF) { // Fixed ROM bank (Bank 0)
        return cart_ctx.rom_data[address];
    } else if (address >= 0x4000 && address <= 0x7FFF) { // Switchable ROM bank
        uint32_t bank_offset = cart_ctx.rom_bank * ROM_BANK_SIZE;
        return cart_ctx.rom_data[bank_offset + (address - 0x4000)];
    } else if (address >= 0xA000 && address <= 0xBFFF) { // Cartridge RAM
        if (cart_ctx.ram_enabled && cart_ctx.ram_data) {
            uint32_t ram_offset = cart_ctx.ram_bank * RAM_BANK_SIZE;
            return cart_ctx.ram_data[ram_offset + (address - 0xA000)];
        }
        return 0xFF;  // Open bus behavior when RAM is disabled
    }
    return 0xFF;  // Invalid address
}

void cart_write(uint16_t address, uint8_t value) {
    if (address <= 0x1FFF) { // Enable/disable external RAM
        cart_ctx.ram_enabled = (value & 0x0A) == 0x0A;
    } else if (address >= 0x2000 && address <= 0x3FFF) { // ROM bank switching
        cart_ctx.rom_bank = value & 0x1F;  // MBC1 example: 5-bit ROM bank number
        if (cart_ctx.rom_bank == 0) cart_ctx.rom_bank = 1;  // Bank 0 is not selectable
    } else if (address >= 0x4000 && address <= 0x5FFF) {
        // RAM bank switching or upper ROM bank bits
        cart_ctx.ram_bank = value & 0x03;  // MBC1 example: 2-bit RAM bank number
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        // Write to cartridge RAM
        if (cart_ctx.ram_enabled && cart_ctx.ram_data) {
            uint32_t ram_offset = cart_ctx.ram_bank * RAM_BANK_SIZE;
            cart_ctx.ram_data[ram_offset + (address - 0xA000)] = value;
        }
    }
}