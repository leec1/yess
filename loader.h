#ifndef LOADER_H
#define LOADER_H

#define EXT ".yo"

#define LINE_COMMENT 0
#define LINE_ERROR -1
#define LINE_GOOD 1

bool openFile(char *filename);
bool load();
#endif
