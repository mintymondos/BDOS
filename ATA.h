#include <stdint.h>
void initata();
void initdrives(void);
typedef struct {
    uint16_t data_start;
    uint8_t sectors_per_cluster;
    uint16_t fat_start;
    uint16_t bytes_per_sector;
    uint16_t root_dir_start;
    uint16_t root_dir_sectors;
    uint16_t total_clusters;
    uint16_t current_dir_cluster;
    uint16_t current_dir_entries;
    uint16_t dir_stack[16];
    uint8_t dir_stack_depth;
    uint8_t (*readsector)(uint32_t lba, uint8_t count, uint16_t *buffer);
    uint8_t (*writesector)(uint32_t lba, uint8_t count, uint16_t *buffer);
} driveloco_t;
void writefile(driveloco_t *drive, char *filename, uint16_t *data, uint32_t data_size);
void readfile(driveloco_t *drive, uint16_t start_cluster, uint16_t *buffer, uint16_t buffer_capacity_words);

uint16_t fat16(driveloco_t *drive, char *filename, uint32_t *out_file_size);
uint8_t chdir(driveloco_t *drive, char *dirname);
uint8_t makedir(driveloco_t *drive, char *dirname);
extern driveloco_t driver;
extern driveloco_t drivea;
extern driveloco_t *current_drive;
