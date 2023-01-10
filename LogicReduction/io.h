#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>

#define IO_H_NO_ERROR   0
#define ERROR_ARGC 0x8001
#define ERROR_FILE 0x8002
struct ARG_LIST{
    FILE *fin;
    FILE *fout;
    int  format;
    int  m;
    int  n;
    unsigned char NotPos;
    char *FormatS;
    char *EquS;
    char *NotS;
    char *AndS;
    char *OrS;
    char *EndS;
};
typedef struct ARG_LIST ARGLIST;
typedef struct ARG_LIST *pARGLIST;

int option(int argc, char *argv[], pARGLIST *parg);
void CloseAll(pARGLIST parg);

FILE *OpenExt( char *fname, char *ext, char *rw);
int fgetline(FILE *fid, char s[], int N);
const char* getfield(char* line, int num, const char *s);
int toTokenArray(char *instr, char *tok[], char *delimiters);

#endif // IO_H_INCLUDED
