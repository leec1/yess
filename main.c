#include <stdio.h>
#include <limits.h>
#include "types.h"
#include "tools.h"
#include "memory.h"
#include "dump.h"

void initialize();

int main(int argc, char * argv[]) {
    if (argc < 2) {
        printf("Please provide a filename.\n");
        return 0;
    }

    if (!openFile(argv[1])) {
        printf("Failed to open file.\nUsage: yess <filename>.yo\n");
        return 0;
    }
    
    initialize();
    
    if (!load()) {//printf("\n");
        dumpMemory();
        return 0;
    }
    
    clockCount = 0;
    stop = FALSE;
    while(!stop){
        stop = writebackStage();
        memoryStage();
        executeStage();
        decodeStage();
        fetchStage();
        clockCount++;
    }

    //dumpProgramRegisters();
    //dumpProcessorRegisters();
    //dumpMemory(); 

}

void initialize() {
    clearMemory();
    clearRegisters();
    clearFregister();
    clearDregister();
    clearEregister();
    clearMregister();
    clearWregister();
}
