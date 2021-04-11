#ifndef BTB_H
#define BTB_H

#include "project.h"

#define MAX_BTB_SIZE 1024
#define BTB_INDEX(x) (x & 0xFFC) >> 2

#define TAKEN 1
#define NOT_TAKEN 0

typedef struct _BTB_entry {
    uint32_t pc;
    uint32_t target;
    uint8_t pred;
} BTB_entry;

// Function prototypes
void trace_program(FILE *trace, BTB_entry btb[MAX_BTB_SIZE]);
void add_btb_entry(BTB_entry btb[], uint32_t PC_current, uint32_t PC_next);
void move_to_next_PC(FILE *trace, uint32_t *current, uint32_t *next);
void initial_read(FILE *trace, uint32_t *current, uint32_t *next);
void read_address(FILE *trace, uint32_t *address);
void read_next(FILE *trace, uint32_t *next);
int calculate_btb_index(uint32_t address);
void print_btb(BTB_entry btb[]);
void change_prediction_bits(int taken, BTB_entry *entry);
void prediction_A(int taken, BTB_entry *entry);
void print_binary(int bin_num);

#endif