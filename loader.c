#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "tools.h"
#include "memory.h"
#include "loader.h"

static FILE *file;
int lineNum, addr, prevAddr, numBytes, prevBytes;

int testLine(char *line);
long int getAddress(char *line);
void storeData(char *line);

/* openFile
 *      Attempts to open the #EXT file with the specified filename.
 * Params:   char *filename - the name of the file to open
 * Returns:  bool - true if the file was succesfully opened
 * Modifies: *file - a static reference to the file opened
 */
bool openFile(char *filename) {
    char *ext = &(filename[(strlen(filename)-strlen(EXT))]);
    if (strcmp(ext, EXT))
        return FALSE;
    return (file = fopen(filename, "r")) != NULL;
}

/* load
 *      Reads the opened file, checks it for errors and stores it if none
 *      are found. If no file is opened, returns false.
 * Params:   none
 * Returns:  bool - true if the file was read and stored succesfully
 * Modifies: lineNum, memory in memory.c
 *
 */
bool load() {
    if (file == NULL)
        return FALSE;
    char line[128];
    lineNum = 1;
    while (fgets(line, 128, file) != NULL) {
        int result = testLine(line);
        if (result >= 0)
            // we want to count good lines and comments
            lineNum++;
        if (result == 0)
            // comment line, just ignore and move on
            continue;
        if (result < 0) {
            // found an error, stop parsing
            printf("Error on line %d\n%s\n", lineNum, line);
            return FALSE;
        } else
            // good line, store it
            storeData(line);
    }
    // because we aren't barbarians
    fclose(file);
    return TRUE;
}

/* testLine
 *      Tests a single line in an input file for .yo format correctness.
 *      proper format is:
 *       0xADDR: INSTRUCTION |
 *      1234567890123456789012
 * Params:   char *line - a pointer to the line to test
 * Returns   int - status code of test (-1 bad line, 0 comment, 1 good line)
 * Modifies: addr, numBytes, prevAddr, prevBytes
 */
int testLine(char *line) {
    int i;

    // this loop tests for commentness
    for (i=0; i<22; i++) {
        if (line[i] != ' ')
            break;
        if (i == 21)
            return LINE_COMMENT;
    }
    
    // this tests for a lot of things, see above.
    if ((line[0] != ' ' && line[0] != 1) || line[1] != ' ' || line[2] != '0' ||
         line[3] != 'x' || line[7] != ':'|| line[8] != ' ' || line[21]!= ' ' ||
         line[22]!= '|') {
        return LINE_ERROR;
    }

    int numBits = strcspn(&line[9], " ");\
    // cant have a nibble instruction
    if ((numBits & 1) != 0)
        return LINE_ERROR;

    addr = getAddress(line);
    // cant go backwards in addressing
    if (addr < prevAddr)
        return LINE_ERROR;
    
    numBytes = numBits >> 1;
    // need enough room for instructions
    if (lineNum > 1 && (prevAddr + prevBytes > addr))
        return LINE_ERROR;

    // line is GOOD
    prevBytes = numBytes;
    prevAddr = addr;
    return LINE_GOOD;
}

/* getAddress
 *      Converts the address form the specified line to a long.
 * Params:   char *line - a pointer to the line we want the address from
 * Returns   l int - the address retrieved from the line, assumes b_16
 * Modifies: none
 */
long int getAddress(char *line) {
    return strtol(&line[2], NULL, 16);
}

/* storeData
 *      From the specified line, get the data portion and stores it in memory.
 * Params:   char *line - a pointer to the line we want the data from
 * Returns:  bool - true if the read/store was successful
 * Modifies: none
 */
void storeData(char *line) {
    bool memError;
    char byte[2];
    int i;
    for (i=0; i<numBytes*2; i+=2) {
        strncpy(byte, &line[i+9], 2);
        putByte(addr+(i/2), (unsigned char)strtol(byte,NULL,16), &memError);
    }
}

