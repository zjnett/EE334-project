#include "project.h"
#include "btb.h"

// Expected usage:
// ./a.out <trace file name>
int main(int argc, char *argv[]) {
    char filename[MAX_STRING_SIZE] = "sample_trace";
    if (argc == 2) {
        strcpy(filename, argv[1]);
    }

    // Declare BTB and initialize to 0
    BTB_entry branch_target_buffer[MAX_BTB_SIZE] = { 0 };
    memset(branch_target_buffer, 0, sizeof(BTB_entry) * MAX_BTB_SIZE);

    // Attempt to open trace file provided by CLI or default trace
    FILE *trace = fopen(filename, "r");
    if (trace != NULL) {
        trace_program(trace, branch_target_buffer);
        fclose(trace);
    }

    print_btb(branch_target_buffer);

    return 0;
}