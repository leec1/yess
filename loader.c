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
//char * getData(char *line);

/* openFile
 *      Attempts to open the #EXT file with the specified filename.
 * Params:   char *filename - the name of the file to open
 * Returns:  bool - true if the file was succesfully opened
 * Modifies: *file - a static reference to the file opened
 */
bool openFile(char *filename) {
    char *ext = &(filename[(strlen(filename)-strlen(EXT))]);
    if (strcmp(ext, EXT)) return FALSE;
    return (file = fopen(filename, "r")) != NULL;
}

bool load() {
    if (file == NULL) return FALSE;
    char line[128];                 // storage for line data
    lineNum = 1;
    while (fgets(line, 128, file) != NULL) {
        //fscanf(file, "%[^\n]", line); //handy dandy "read until \n"
        //if (fgetc(file) == EOF) break;
        //printf("%s\n", line);
        int result = testLine(line);
        if (result >= 0) lineNum++; // count good lines and comments
        if (result == 0) continue;  // dont bother with comment lines 
        if (result < 0) {
            printf("Error on line %d\n%s\n\n", lineNum, line);
            return FALSE;           // found an error, stop parsing
        } else {
            storeData(line);     // good line, store it
        }
    }
    fclose(file);                   // because we aren't barbarians
    return TRUE;
}
/* load
 *      Reads the opened file, checks it for errors and stores it if none
 *      are found. If no file is opened, returns false.
 * Params:   none
 * Returns:  bool - true if the file was read and stored succesfully
 * Modifies: lineNum, memory in memory.c
 *
bool load() {
    if (file == NULL) return FALSE;
    char line[128];                 // storage for line data
    lineNum = 1;
    do {
        fscanf(file, "%[^\n]", line); //handy dandy "read until \n"
        printf("%s\n", line);
        int result = testLine(line);
        if (result >= 0) lineNum++; // count good lines and comments
        if (result == 0) continue;  // dont bother with comment lines 
        if (result < 0) {
            printf("Error on line %d\n%s\n\n", lineNum, line);
            return FALSE;           // found an error, stop parsing
        } else {
            //getData(line);
            storeData(line);     // good line, store it
        }
    } while (fgetc(file) != EOF);   // tests AND takes care of trailing \n
    fclose(file);                   // because we aren't barbarians
    return TRUE;
}*/

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
    for (i=0; i<22; i++) { // this loop tests for commentness
        if (line[i] != ' ') break;
        if (i == 21) return LINE_COMMENT;
    }

    if ((line[0] != ' ' && line[0] != 1) || line[1] != ' ' || line[2] != '0' ||
         line[3] != 'x' || line[7] != ':'|| line[8] != ' ' || line[21]!= ' ' ||
         line[22]!= '|') { // this tests for a lot of things. see above.
        return LINE_ERROR;
    }

    int numBits = strcspn(&line[9], " "); // cant have a nibble instruction
    if ((numBits & 1) != 0) return LINE_ERROR; // remember, X%2 == X&1

    addr = getAddress(line); // cant go backwards in addressing (?)
    if (addr < prevAddr) return LINE_ERROR;
    
    numBytes = numBits >> 1; // need enough room in memory for instructions
    if (lineNum > 1 && (prevAddr + prevBytes > addr)) return LINE_ERROR;

    prevBytes = numBytes;
    prevAddr = addr;
    return LINE_GOOD; // good line
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

/*long long int getData(char *line) {
    char data[numBytes];
    long long int rData = 0;
    //data[numBytes-1] = '\0';
    strncpy(data, &line[9], numBytes*2);
    return strtoll(data, NULL, 16);
    //printf("0x%llx  ::  %s\n",rData, line);
    //printf("%d: bytes=%d\n", lineNum, numBytes);
    //if (numBytes > 0) printf("line data: %s\n", data);
}*/

/*char* getData(char *line) {
    char data[numBytes];
    char byte[2];
    int i;
    for (i=0; i<numBytes*2; i+=2) {
        strncpy(byte, &line[i+9], 2);
        data[i] = strtol(byte, NULL, 16);
    }
    printf("%s -> %d\n", line[9], data);
    return data;
}*/
