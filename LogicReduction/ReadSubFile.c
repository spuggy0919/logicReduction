
#include "ReadSubFile.h"


FILE *SubFile=NULL;
char linebuf[1024];
/* file SubTitle readInput */
void SubHandle(FILE *fid)
{
    if (fid) SubFile=fid;
}

double convert_sec(char *instr)
{
    int i,j;
    double tms=0.0;
    char *pch,*s=instr;
    char *delim = ".";
    char tstr[20];
    strcpy(tstr,instr); // for avoid line parser NULL char
    for (i=0; s[i]; s[i]=='.' ? i++: *s++);
    pch = strtok(tstr,delim);
    for(j=0;j<i;j++){
       // printf ("%s\n",pch);
        tms = tms*60 + (double) atoi(pch);
        pch = strtok (NULL, delim);
    }
    tms = tms + ((double) atoi(pch))/100.0;

    return tms;
}

int getCurRecord(double *stime, double *etime, char *msg)
{
    int n=0;
    char *pch,*next;
    char *delim = " \n";

    if (!SubFile) return 0;
    n=fgetline(SubFile,linebuf,1000);
    if (n==0) {
            fclose(SubFile);
            return 0;
    }

    pch = strtok(linebuf,delim);
    next=&pch[strlen(pch)+1];
    *stime = convert_sec(pch);
    pch = strtok(next,delim);
    next=&pch[strlen(pch)+1];
   *etime = convert_sec(pch);
    pch = strtok(next,delim);
    strcpy(msg,pch);
    printf("   Read: (%8.2f,%8.2f,%s \n",(float)*stime,(float)*etime,msg);
    return n;


}
