/*


    Copyright (C) 2016 spuggy0919@gmail.com


    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included
     in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io/io.h"

#define MAXTABLESIZE  1024  // Input Limit to 10 bits
pARGLIST parg;
#define SYMLEN 11
char outSym[]="ABCDEFGHIJKLMNOPRSTUVWXYZ";
char inSym[]="pqrstuvwxy";
char lineBuf[512];
int bitloc=0;
unsigned long  trueTable[MAXTABLESIZE]; // for 2^10 Input long 32bits 2 bit for 1 output bits
// Output Limit by 32 bits /2 = 16 outputs
int firstTerm;

int bin2Dec(char * str)
{
    int m=strlen(str);
    int i,ret=0;
    if ((1<<m)>MAXTABLESIZE) return  -1;
    for(i=0;i<m;i++) {
        ret=ret*2+(str[i]-'0');
    }
    return ret;
}
unsigned long true2Dec(char * str)
{
    int n=strlen(str);
    int i;
    unsigned long ret=0;
    unsigned char c=0;
    for(i=0;i<n;i++) {
            switch(toupper(str[i])){
                case '0': c=0; break;
                case '1': c=1; break;
                case 'X': c=3; break;
            }
            ret=(ret<<2) | c;
    }
    return ret;
}
int readTrueTable(int *ins, int *outs)
{
    int i,n,idx,toknum,lno=0;
    char *tok[5];
    *ins=0; *outs=0;
    for (i=0;i<MAXTABLESIZE;i++) trueTable[i]=0xffffffff;

    do{
        n=fgetline(parg->fin,lineBuf,1024);lno++;
        if (lineBuf[0]=='%') {
                fprintf(parg->fout,"%s",lineBuf);
                continue;
        }else{
            fprintf(parg->fout,"%%%s",lineBuf);
        }

        if (n==0) break;
        toknum = toTokenArray(lineBuf,tok, " \n");
        if (toknum!=2) {
            printf("Input Format Error!\n");
            return -1;
        }
        if (strlen(tok[0])>*ins) *ins=strlen(tok[0]);
        if (strlen(tok[1])>*outs) *outs=strlen(tok[1]);
        idx = bin2Dec(tok[0]);
        if (idx <0 ) return -1;
        if (*outs>sizeof(long)*4) return -2;
        trueTable[idx] = true2Dec(tok[1]);
    }while(n>0);
    return lno;
}
// idx is Input Mask to check output truetable
static int oneCnt=0,dontCareCnt=0;
int checkOnes(int m, int outbit){
    int i;
    unsigned long outmask=0b11<<(outbit<<1);
    unsigned long outone=0b01<<(outbit<<1);
    unsigned long outdncare=0b11<<(outbit<<1);
    oneCnt =0 ;
    dontCareCnt =0 ;
     for(i=0;i<(1<<m);i++){
        if ((trueTable[i]&outmask) == outone ){
            oneCnt++;
        }
        if ((trueTable[i]&outmask) == outdncare ){
            dontCareCnt++;
        }
    }
    return oneCnt;
}
int checkCoverage(int m, unsigned long inmask, unsigned long inval, int outbit){
    int i,one=0;
    if (oneCnt==0) return 0;
    unsigned long outmask=0b11<<(outbit<<1);
    unsigned long outone=0b01<<(outbit<<1);
    unsigned long outdoncare=0b11<<(outbit<<1);
    for(i=0;i<(1<<m);i++){
         if ((i&inmask)==inval) { // match selected
            if ((trueTable[i]&outmask) == 0 ) {// false item
                return 0;
            }
            if ((trueTable[i]&outmask) == outone ) {// true item
                one++;
            }
         }
    }
    for(i=0;i<(1<<m);i++){ // clear one to don't care
         if ((i&inmask)==inval) { // match selected
           if ((trueTable[i]&outmask) == outone ) {// true item
                trueTable[i] = (trueTable[i]&(~outmask))|outdoncare;
           }
         }
    }
    oneCnt -=one;
    return one;
}
void combinationUtil(int data[], int start, int end,
                     int index, int r);

// The main function that prints all combinations of size r
// in arr[] of size n. This function mainly uses combinationUtil()
void reduceCombination(int n, int r)
{
    // A temporary array to store all combination one by one
    int data[r];

    // Print all combination using temprary array 'data[]'
    combinationUtil(data, 0, n-1, 0, r);
}

void outputTerm( int data[], int r)
{
  int dat,j;
    if (firstTerm==0) {
            fprintf(parg->fout,"%s",parg->OrS);
    }
    for ( j=0; j<r; j++){
        if (j==0&& parg->format!=0&&r!=1) fprintf(parg->fout,"(");
        if (r!=1&&j!=0) fprintf(parg->fout,"%s",parg->AndS);
        dat=data[r-1-j];
        if (dat>0){
            dat--;
            fprintf(parg->fout,"%c",inSym[parg->m-1-dat]);
            firstTerm=0;
        }else{
            dat=-dat-1;
            if (parg->NotPos==0)
                fprintf(parg->fout,"%s%c",parg->NotS,inSym[parg->m-1-dat]);
            else if (parg->NotPos==1)
                fprintf(parg->fout,"%c%s",inSym[parg->m-1-dat],parg->NotS);
            else  if (parg->NotPos==2)
                fprintf(parg->fout,"(%s %c)",parg->NotS,inSym[parg->m-1-dat]); // VHDL

            firstTerm=0;
        //    val |=0x00<<data[j];
        }
        if (j==r-1&& parg->format!=0&&r!=1) fprintf(parg->fout,")");
    }

            return;
}
/* modified from source
   http://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
   data[] ---> Temporary array to store current combination
   start & end ---> Staring and Ending indexes in arr[]
   index  ---> Current index in data[]
   r ---> Size of a combination to be printed */
void combinationUtil( int data[], int start, int end,
                     int index, int r)
{
 int i,j;   // Current combination is ready to be printed, print it
 int mask=0;
 int val=0;
 int flag;
 int dat;
    if (index == r)
    {
        for ( j=0; j<r; j++){
//            printf("%d ", data[j]);
            dat=data[j];
            if (dat>0){
                dat--;
                mask|=0b1<<(dat);
                val |=0b1<<(dat);
            }else{
                dat=-dat-1;
                mask|=0b1<<(dat);
            //    val |=0x00<<data[j];
            }
         //   printf("%x,mask=%x,val=%x ", dat,mask,val);
        }
        flag=checkCoverage(parg->m,mask,val,bitloc);
        if (flag>0) {
            outputTerm(data,r);
        }
        return;
    }

    // replace index with all possible elements. The condition
    // "end-i+1 >= r-index" makes sure that including one element
    // at index will make a combination with remaining elements
    // at remaining positions
    for ( i=start; i<=end && end-i+1 >= r-index; i++)
    {
        data[index] = i+1;
        combinationUtil( data, i+1, end, index+1, r);
        data[index] = -i-1;
        combinationUtil( data, i+1, end, index+1, r);
    }
}




int main(int argc, char *argv[])
{

    int i,j,r;

    if (option(argc,argv,&parg)!=IO_H_NO_ERROR) return -1;
    fprintf(parg->fout,"%%Minimize ver 1.0 spuggy0919@gmail.com\n");
    r=readTrueTable(&(parg->m),&(parg->n));
    if (r<0 ) {
            if (r==-1) printf("input parameters should be less than 10 bits\n");
            if (r==-2) printf("output parameters is less than (unsigned long size /2bits)\n");
            return -1;
    }
    fprintf(parg->fout,"\n%% Input Symbols:");
    for(i=0;i<parg->m;i++) fprintf(parg->fout,"%1d",parg->m-1-i); fprintf(parg->fout,"\n");
    fprintf(parg->fout,"%%              :");
    for(i=0;i<parg->m;i++) fprintf(parg->fout,"%c",inSym[i]);
    fprintf(parg->fout,"\n");
    fprintf(parg->fout,"%%Output Symbols:");
    for(i=0;i<parg->n;i++) fprintf(parg->fout,"%1d",(i)%10); fprintf(parg->fout,"\n");
    fprintf(parg->fout,"%%              :");
    for(i=0;i<parg->n;i++) fprintf(parg->fout,"%c",outSym[i]);
    fprintf(parg->fout,"\n%s\n",parg->FormatS);
    for(i=0;i<parg->n;i++) {
        bitloc=parg->n-1-i;
        checkOnes(parg->m,bitloc);
    //    printf("one=%d\n",oneCnt);
        fprintf(parg->fout,"%c%s",outSym[parg->n-1-bitloc],parg->EquS);
        if ((oneCnt+dontCareCnt)== (1<<parg->m)) {
            fprintf(parg->fout,"1\n");
            continue;
        }else if (oneCnt==0){
            fprintf(parg->fout,"0\n");
            continue;
        }else{
            firstTerm=1;
            for(j=1;j<=parg->m;j++) {
                reduceCombination(parg->m, j);
            }
        //   printf("\n");
            fprintf(parg->fout,"%s\n",parg->EndS);
        }
    }
    CloseAll(parg);
    return 1;
}
