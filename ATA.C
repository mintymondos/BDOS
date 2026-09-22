#include "DET.h"
#include "ATA.h"
#include "MAIN.h"
#include "FDC.h"
#include <stdint.h>

uint16_t rootbuf[256 * 32];


driveloco_t driver;
driveloco_t drivea;
driveloco_t *current_drive = &driver;

uint8_t floopyreadmulti(uint32_t lba, uint8_t count, uint16_t *buffer) {
    uint8_t i = 0;
    while (i < count) {
        uint8_t status = floopyrlba(lba + i, buffer + (i * 256));
        if (status != 0) {
            return 1;
        }
        i++;
    }
    return 0;
}
uint8_t floopywritemulti(uint32_t lba, uint8_t count, uint16_t *buffer) {
    uint8_t i = 0;
    while (i < count) {
        uint8_t status = floopywlba(lba + i, buffer + (i * 256));
        if (status != 0) {
            return 1;
        }
        i++;
    }
    return 0;
}


void readfile(driveloco_t *drive, uint16_t start_cluster, uint16_t *buffer, uint16_t buffer_capacity_words);
uint8_t readsectata(uint32_t lba, uint8_t count, uint16_t *buffer);
uint8_t writesectata(uint32_t lba, uint8_t count, uint16_t *buffer);


void initdrives(void) {
    driver.readsector = readsectata;
    driver.writesector = writesectata;
    drivea.readsector = floopyreadmulti;
    drivea.writesector = floopywritemulti;
}


uint8_t readsectata(uint32_t lba, uint8_t count, uint16_t *buffer) {
    uint8_t status;
        __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1f6\n\t"
        "mov al, bl\n\t"
        "or al, 0xe0\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F2\n\t"
        "mov al, cl\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F3\n\t"
        "mov al, byte ptr [esi]\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F4\n\t"
        "mov al, byte ptr [esi+1]\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F5\n\t"
        "mov al, byte ptr [esi+2]\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F7\n\t"
        "mov al, 0x20\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        : "b" ((uint8_t)((lba >> 24) & 0x0F)), "c" (count), "S" (&lba)
        : "ax", "dx"
    );

    __asm__ __volatile__ (
    ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F7\n\t"
        "poll_read_%=:\n\t"
        "in al, dx\n\t"
        "test al, 0x80\n\t"
        "jnz poll_read_%=\n\t"
        "test al, 0x01\n\t"
        "jnz read_err_%=\n\t"
        "test al, 0x08\n\t"
        "jz poll_read_%=\n\t"
        "jmp read_ok_%=\n\t"
        "read_err_%=:\n\t"
        "mov al, 1\n\t"
        "jmp read_done_%=\n\t"
        "read_ok_%=:\n\t"
        "mov al, 0\n\t"
        "read_done_%=:\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (status)
        :
        : "dx"
    );
    if (status != 0) {
        return 1;
    }
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F0\n\t"
        "mov ch, 0\n\t"
        "shl cx, 8\n\t"
        "read_loop_%=:\n\t"
        "in ax, dx\n\t"
        "stosw\n\t"
        "loop read_loop_%=\n\t"
        ".att_syntax prefix\n\t"
        :
        : "D" (buffer), "c" (count)
        : "ax", "dx", "memory"
    );
    return 0;
}

uint8_t writesectata(uint32_t lba, uint8_t count, uint16_t *buffer) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1f6\n\t"
        "mov al, bl\n\t"
        "or al, 0xe0\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F2\n\t"
        "mov al, cl\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F3\n\t"
        "mov al, byte ptr [esi]\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F4\n\t"
        "mov al, byte ptr [esi+1]\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F5\n\t"
        "mov al, byte ptr [esi+2]\n\t"
        "out dx, al\n\t"
        "mov dx, 0x1F7\n\t"
        "mov al, 0x30\n\t"
        "out dx, al\n\t"
        ".att_syntax prefix\n\t"
        :
        : "b" ((uint8_t)((lba >> 24) & 0x0F)), "c" (count), "S" (&lba)
        : "ax", "dx"
    );


    uint8_t status;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F7\n\t"
        "poll_write_%=:\n\t"
        "in al, dx\n\t"
        "test al, 0x80\n\t"
        "jnz poll_write_%=\n\t"
        "test al, 0x01\n\t"
        "jnz write_err_%=\n\t"
        "test al, 0x08\n\t"
        "jz poll_write_%=\n\t"
        "jmp write_ok_%=\n\t"
        "write_err_%=:\n\t"
        "mov al, 1\n\t"
        "jmp write_done_%=\n\t"
        "write_ok_%=:\n\t"
        "mov al, 0\n\t"
        "write_done_%=:\n\t"
        ".att_syntax prefix\n\t"
        : "=a" (status)
        :
        : "dx"
    );
    if (status != 0) {
        return 1;
    }

    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov dx, 0x1F0\n\t"
        "mov ch, 0\n\t"
        "shl cx, 8\n\t"
        "write_loop_%=:\n\t"
        "lodsw\n\t"
        "out dx, ax\n\t"
        "loop write_loop_%=\n\t"
        ".att_syntax prefix\n\t"
        :
        : "S" (buffer), "c" (count)
        : "ax", "dx", "memory"
    );
    return 0;
}


uint16_t findir(uint8_t *dirbuf, uint16_t entry_count, char *filename, uint32_t *out_file_size) {
    uint16_t start_cluster = 0;
    uint8_t match = 1;
    for (int i = 0; i < entry_count; i++) {
        uint16_t offset = i * 32;
        if (dirbuf[offset] == 0x00) {
            break;
        }
        else if (dirbuf[offset] == 0xE5) {
            continue;
        }
        match = 1;
        for (int j = 0; j < 11; j++) {
            if (dirbuf[offset + j] != filename[j]) {
                match = 0;
                break;
            }
        }
        if (match == 1) {
            start_cluster = dirbuf[offset + 26] | (dirbuf[offset + 27] << 8);
            *out_file_size = dirbuf[offset + 28] | (dirbuf[offset + 29] << 8) | (dirbuf[offset + 30] << 16) | ((uint32_t)dirbuf[offset + 31] << 24);
            break;
        }
    }
    return start_cluster;
}


// fat chud filesystem
uint16_t fat16(driveloco_t *drive, char *filename, uint32_t *out_file_size) {
    uint16_t bootsect[256];
    uint8_t err = drive->readsector(0, 1, bootsect);
    if (err != 0) {
        nodrive();
    }
    uint8_t *bs = (uint8_t *)bootsect;
    uint16_t bytes_per_sector = bs[0x0B] | (bs[0x0C] << 8);
    uint8_t sectors_per_cluster = bs[0x0D];
    uint16_t reserved_sectors = bs[0x0E] | (bs[0x0F] << 8);
    uint8_t num_fats = bs[0x10];
    uint16_t root_entry_count = bs[0x11] | (bs[0x12] << 8);
    uint16_t sectors_per_fat = bs[0x16] | (bs[0x17] << 8);
    uint16_t total_sectors_16 = bs[0x13] | (bs[0x14] << 8);

    uint16_t fat_start = reserved_sectors;
    uint16_t fat2_start = fat_start + sectors_per_fat;

    uint16_t root_dir_start = fat_start + (num_fats * sectors_per_fat);
    uint16_t root_dir_sectors = (root_entry_count * 32) / bytes_per_sector;
    uint16_t data_start = root_dir_start + root_dir_sectors;
    drive->data_start = data_start;
    drive->sectors_per_cluster = sectors_per_cluster;
    drive->fat_start = fat_start;
    drive->bytes_per_sector = bytes_per_sector;
    drive->root_dir_start = root_dir_start;
    drive->root_dir_sectors = root_dir_sectors;
    drive->total_clusters = (total_sectors_16 - data_start) / sectors_per_cluster;
    if (drive->current_dir_cluster == 0) {
        drive->current_dir_entries = root_entry_count;
    }
    uint32_t file_size = 0;
    uint16_t start_cluster = 0;
    if (drive->current_dir_cluster == 0) {
        drive->readsector(root_dir_start, root_dir_sectors, rootbuf);
        start_cluster = findir((uint8_t *)rootbuf, root_entry_count, filename, &file_size);
    }
    else {
        readfile(drive, drive->current_dir_cluster, rootbuf, 256 * 32);
        start_cluster = findir((uint8_t *)rootbuf, drive->current_dir_entries, filename, &file_size);
    }
    *out_file_size = file_size;
    return start_cluster;
}

void readcluster(driveloco_t *drive, uint16_t cluster, uint16_t *buffer) {
    uint16_t lba = drive->data_start + ((cluster - 2) * drive->sectors_per_cluster);
    drive->readsector(lba, drive->sectors_per_cluster, buffer);
}

uint16_t nextcluster(driveloco_t *drive, uint16_t cluster) {
    uint16_t fat_offset = cluster * 2;
    uint16_t fat_sector = drive->fat_start + (fat_offset / drive->bytes_per_sector);
    uint16_t offset_in_sector = fat_offset % drive->bytes_per_sector;
    uint16_t fatbuf[256];
    drive->readsector(fat_sector, 1, fatbuf);
    uint8_t *fb = (uint8_t *)fatbuf;
    uint16_t next = fb[offset_in_sector] | (fb[offset_in_sector + 1] << 8);
    return next;
}

void readfile(driveloco_t *drive, uint16_t start_cluster, uint16_t *buffer, uint16_t buffer_capacity_words) {
    uint16_t current = start_cluster;
    uint16_t buf_offset = 0;
    while (current < 0xFFF8) {
        readcluster(drive, current, buffer + buf_offset);
        buf_offset = buf_offset + (drive->sectors_per_cluster * 256);
        current = nextcluster(drive, current);
        if (buf_offset >= buffer_capacity_words) {
            break;
        }
    }
}

uint16_t findfreecluster(driveloco_t *drive) {
    for (uint16_t c = 2; c < drive->total_clusters + 2; c++) {
        uint16_t entry = nextcluster(drive, c);
        if (entry == 0x0000) {
            return c;
        }
    }
    return 0;
}
void setfatentry(driveloco_t *drive,uint16_t cluster, uint16_t value) {
    uint16_t fat_offset = cluster * 2;
    uint16_t fat_sector = drive->fat_start + (fat_offset / drive->bytes_per_sector);
    uint16_t offset_in_sector = fat_offset % drive->bytes_per_sector;
    uint16_t fatbuf[256];
    drive->readsector(fat_sector, 1, fatbuf);
    uint8_t *fb = (uint8_t *)fatbuf;
    fb[offset_in_sector] = value & 0xFF;
    fb[offset_in_sector + 1] = (value >> 8) & 0xFF;
    drive->writesector(fat_sector, 1, fatbuf);
}
void writedirentry(driveloco_t *drive, char *filename, uint16_t start_cluster, uint32_t file_size, uint8_t attr) {
    uint16_t entry_count;
    if (drive->current_dir_cluster == 0) {
        drive->readsector(drive->root_dir_start, drive->root_dir_sectors, rootbuf);
        entry_count = drive->current_dir_entries;
    }
    else {
        readfile(drive, drive->current_dir_cluster, rootbuf, 256 * 32);
        entry_count = drive->current_dir_entries;
    }

    uint8_t *rb = (uint8_t *)rootbuf;
    uint16_t offset = 0;
    uint16_t found_slot = 0xFFFF;
    for (int i = 0; i < entry_count; i++) {
        offset = i * 32;
        if (rb[offset] == 0x00 || rb[offset] == 0xE5) {
            found_slot = offset;
            break;
        }
    }
    if (found_slot == 0xFFFF) {
        printst("DISK WRITE ERROR\n", 4);
        printst("Directory is completely full!", 15);
        return;
    }
    for (int j = 0; j < 11; j++) {
        rb[found_slot + j] = filename[j];
    }
    rb[found_slot + 11] = attr;
    rb[found_slot + 26] = start_cluster & 0xFF;
    rb[found_slot + 27] = (start_cluster >> 8) & 0xFF;
    rb[found_slot + 28] = file_size & 0xFF;
    rb[found_slot + 29] = (file_size >> 8) & 0xFF;
    rb[found_slot + 30] = (file_size >> 16) & 0xFF;
    rb[found_slot + 31] = (file_size >> 24) & 0xFF;

    if (drive->current_dir_cluster == 0) {
        drive->writesector(drive->root_dir_start, drive->root_dir_sectors, rootbuf);
    }
    else {
        uint16_t current = drive->current_dir_cluster;
        uint16_t buf_offset = 0;
        while (current < 0xFFF8) {
            uint16_t lba = drive->data_start + ((current - 2) * drive->sectors_per_cluster);
            drive->writesector(lba, drive->sectors_per_cluster, rootbuf + buf_offset);
            buf_offset = buf_offset + (drive->sectors_per_cluster * 256);
            current = nextcluster(drive, current);
        }
    }
}

void writefile(driveloco_t *drive, char *filename, uint16_t *data, uint32_t data_size) {
    uint16_t total_clusters = drive->total_clusters;
    uint16_t first_cluster = findfreecluster(drive);
    if (first_cluster == 0) {
        printst("DISK WRITE ERROR\n", 4);
        printst("Disk does not have any space on it left, please delete some files to free up storage using the \"DEL\" command.", 15);
        return;

    }
    uint16_t current_cluster = first_cluster;
    uint16_t previous_cluster = 0;
    uint32_t bytes_written = 0;
    uint16_t bytes_per_cluster = drive->sectors_per_cluster * drive->bytes_per_sector;
    while (bytes_written < data_size) {
        uint16_t lba = drive->data_start + ((current_cluster - 2) * drive->sectors_per_cluster);
        drive->writesector(lba, drive->sectors_per_cluster, data + (bytes_written / 2));
        if (previous_cluster != 0) {
            setfatentry(drive, previous_cluster, current_cluster);
        }
        bytes_written = bytes_written + bytes_per_cluster;
        previous_cluster = current_cluster;
        if (bytes_written < data_size) {
            current_cluster = findfreecluster(drive);
            if (current_cluster == 0) {
                printst("DISK WRITE ERROR:", 4);
                printst("Disk ran out of space whilst writing file, delete some files to free up space using the \"DEL\" command.\n", 15);
                setfatentry(drive, previous_cluster, 0xFFFF);
                return;
            }
        }
    }
    setfatentry(drive, previous_cluster, 0xFFFF);
    writedirentry(drive, filename, first_cluster, data_size, 0x20);
}


uint8_t chdir(driveloco_t *drive, char *dirname) {
    if (dirname[0] == '.' && dirname[1] == '.' && dirname[2] == ' ') {
        if (drive->dir_stack_depth == 0) {
            drive->current_dir_cluster = 0;
            return 0;
        }
        drive->dir_stack_depth = drive->dir_stack_depth - 1;
        drive->current_dir_cluster = drive->dir_stack[drive->dir_stack_depth];
        if (drive->current_dir_cluster != 0) {
            uint16_t cluster_count = 0;
            uint16_t walk = drive->current_dir_cluster;
            while (walk < 0xFFF8) {
                cluster_count = cluster_count + 1;
                walk = nextcluster(drive, walk);
            }
            uint16_t total_bytes = cluster_count * drive->sectors_per_cluster * drive->bytes_per_sector;
            drive->current_dir_entries = total_bytes / 32;
        }
        return 0;
    }
    uint32_t fakesize = 0;
    uint16_t target_cluster = fat16(drive, dirname, &fakesize);
    if (target_cluster == 0) {
        printst("ERROR", 4);
        printst("Target is not a directory or directory could not be found: \"", 15);
        printst(dirname, 13);
        printst("\"", 15);
        return 1;
    }

    if (drive->dir_stack_depth < 16) {
        drive->dir_stack[drive->dir_stack_depth] = drive->current_dir_cluster;
        drive->dir_stack_depth = drive->dir_stack_depth + 1;
    }

    uint16_t cluster_count = 0;
    uint16_t walk = target_cluster;
    while (walk < 0xFFF8) {
        cluster_count = cluster_count + 1;
        walk = nextcluster(drive, walk);
    }
    uint16_t total_bytes = cluster_count * drive->sectors_per_cluster * drive->bytes_per_sector;
    drive->current_dir_entries = total_bytes / 32;
    drive->current_dir_cluster = target_cluster;
    return 0;
}

uint8_t makedir(driveloco_t *drive, char *dirname) {
    uint16_t new_cluster = findfreecluster(drive);
    if (new_cluster == 0) {
        printst("DISK WRITE ERROR\n", 4);
        printst("Disk does not have any space on it left, please delete some files to free up storage using the \"DEL\" command.\n", 15);
        return 1;
    }
    
    uint16_t dirbuf[256 * 8];
    uint8_t *db = (uint8_t *)dirbuf;
    for (int i = 0; i < (256 * 8 * 2); i++) {
        db[i] = 0x00;
    }
    db[0] = '.';
    for (int i = 1; i < 11; i++) {
        db[i] = ' ';
    }
    db[11] = 0x10;
    db[26] = new_cluster & 0xFF;
    db[27] = (new_cluster >> 8) & 0xFF;
    db[32] = '.';
    db[33] = '.';
    for (int i = 34; i < 42; i++) {
        db[i] = ' ';
    }
    db[43] = 0x10;
    db[58] = drive->current_dir_cluster & 0xFF;
    db[59] = (drive->current_dir_cluster >> 8) & 0xFF;
    uint16_t lba = drive->data_start + ((new_cluster - 2) * drive->sectors_per_cluster);
    drive->writesector(lba, drive->sectors_per_cluster, dirbuf);
    setfatentry(drive, new_cluster, 0xFFFF);
    writedirentry(drive, dirname, new_cluster, 0, 0x10);
    return 0;
}

void initata() {
    uint8_t res = atadetectionfuckyou();

    if (res == 1) {
        nodrive();
    }
    else {
        printst("ATA driver loaded successfully\n", 14);
    }
}
