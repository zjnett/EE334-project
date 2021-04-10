#include "project.h"
#include "btb.h"

// Global PC current and PC next

// Wrapper function to trace through program.
// Assumes trace has been opened for mode read successfully.
void trace_program(FILE *trace, BTB_entry btb[MAX_BTB_SIZE]) {
    uint32_t PC_current = 0;
    uint32_t PC_next = 0;

    // Initialize tracker variables
    int hit = 0, miss = 0, right = 0, wrong = 0, wrong_addr = 0, collision = 0, taken = 0;

    initial_read(trace, &PC_current, &PC_next);
    
    // loop until at end of trace
    while (!feof(trace)) {
        // Stage 1 (IF)
        // is PC_current in BTB?
        if (btb[BTB_INDEX(PC_current)].target != 0) {
            // PC current is in BTB
            hit++;
            // Stage 2
            // right prediction?
            // check if prediction is taken or not
            if (btb[BTB_INDEX(PC_current)].pred == TAKEN) {
                if (btb[BTB_INDEX(PC_current)].target == PC_next) {
                    // right
                    right++;
                    taken++;
                    if (PC_next != PC_current + 4) {
                        wrong_addr++;
                        taken++;
                    }
                } else {
                    // wrong
                    wrong++;
                }
            } else if (btb[BTB_INDEX(PC_current)].pred == NOT_TAKEN) {
                if (PC_next == PC_current + 4) {
                    right++;
                } else {
                    wrong++;
                    taken++;
                }
            }
        } else {
            // PC current is not in BTB
            // Stage 2
            // is instruction a taken branch?
            if (PC_next != PC_current + 4) {
                // branch was taken
                miss++;
                taken++;
                // add current PC and next PC to BTB
                add_btb_entry(btb, PC_current, PC_next);
            } else {
                // normal program execution
            }
        }
        move_to_next_PC(trace, &PC_current, &PC_next);
    }

}

void add_btb_entry(BTB_entry btb[], uint32_t PC_current, uint32_t PC_next) {
    btb[BTB_INDEX(PC_current)].pc = PC_current;
    btb[BTB_INDEX(PC_current)].target = PC_next;
    btb[BTB_INDEX(PC_current)].pred = 0b00; // default
}

void move_to_next_PC(FILE *trace, uint32_t *current, uint32_t *next) {
    *current = *next;
    read_address(trace, next);
}

void initial_read(FILE *trace, uint32_t *current, uint32_t *next) {
    fscanf(trace, "%x", current);
    fscanf(trace, "%x", next);
}

void read_address(FILE *trace, uint32_t *address) {
    fscanf(trace, "%x", address);
}

void read_next(FILE *trace, uint32_t *next) {
    fscanf(trace, "%x", next);
}

int calculate_btb_index(uint32_t address) {
    return (address & 0xFFC) >> 2;
}

void print_btb(BTB_entry btb[]) {
    printf("entry:\tPC:\ttarget:\tPred.:\n");
    for (int i = 0; i < MAX_BTB_SIZE; i++) {
        if (btb[i].target != 0) {
            printf("%d\t%x\t%x\t%x\n", i, btb[i].pc, btb[i].target, btb[i].pred);
        }
    }
}