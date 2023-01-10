
#ifndef _READ_SUBFILE_H_
#define _READ_SUBFILE_H_
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SubHandle(FILE *fid);
int getCurRecord(double *stime, double *etime, char *msg);
#endif // _READ_SUBFILE_H_
