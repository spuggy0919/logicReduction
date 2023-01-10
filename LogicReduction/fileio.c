
#include "io.h"
char *cmdDescription[]={
    "Minimize uitlity Ver 1.0 2015 CopyRight(c)\n",
    "author:spuggy0919@gmail.com\n",
    "Usage:minimize in.txt out.txt [Format]\n",
    "  in.txt input true table\n",
    " out.txt output logical equation\n",
    " Format:0 default\n",
    "        1 C style\n",
    "        2 Abel style\n",
    "        3 Verilog style\n",
    "        4 PALASM style\n",
  //  "        5 VHDL style\n",
    0
};
void setFormatStr(pARGLIST parg)
{
    switch(parg->format){
    default:
    case 0:
        parg->FormatS="%default Format";
        parg->NotS="'"; parg->NotPos = (unsigned char)1; // after
        parg->AndS="";
        parg->OrS="+";
        parg->EquS="=";
        parg->EndS="";
        break;
    case 1: // C
        parg->FormatS="%C Style Format";
        parg->NotS="!"; parg->NotPos = 0; // before
        parg->AndS="&&";
        parg->OrS="||";
        parg->EquS="=";
        parg->EndS=";";
        break;
    case 2: // !p&q&r) # Abel
        parg->FormatS="%ABEL Style Format";
        parg->NotS="!"; parg->NotPos = 0;
        parg->AndS="&";
        parg->OrS="#";
        parg->EquS="=";
        parg->EndS="";
        break;
    case 3: // !p&q&r) |  Verilog
        parg->FormatS="%Verilog Style Format";
        parg->NotS="!"; parg->NotPos = 0;
        parg->AndS="&";
        parg->OrS="|";
        parg->EquS="=";
        parg->EndS=";";
        break;
    case 4: // /p*q*r) +  PALASM
        parg->FormatS="%PALASM Style Format";
        parg->NotS="/"; parg->NotPos = 0;
        parg->AndS="*";
        parg->OrS="+";
        parg->EquS="=";
        parg->EndS="";
        break;
 #if 1
    case 5: // (NOT p)AND q AND r) OR  VHDL
        parg->FormatS="%VHDL Style Format";
        parg->NotS="NOT "; parg->NotPos = 2;
        parg->AndS=" AND ";
        parg->OrS=" OR ";
        parg->EquS="=";
        parg->EndS="";
        break;
#endif
    }
}
int option(int argc, char *argv[], pARGLIST *parg)
{
	pARGLIST p;
	char **pdes;

    if (argc<2){
        for(pdes=cmdDescription; *pdes; pdes++){
            printf( (const char *)*pdes);
        }
   //     system("pause");
        return ERROR_ARGC;
    }
    p=(pARGLIST)malloc(sizeof(ARGLIST));
    if (argc>=2){
        p->fin=OpenExt(argv[1],"txt","r");
    }
    p->fout=NULL;
    if (argc>=3){
        p->fout=OpenExt(argv[2],"txt","w");
    }
    p->format=0;
    if (argc>=4){
        p->format=atoi(argv[3]);
    }
        setFormatStr(p);


    if (!p->fin) return ERROR_FILE;
    if (!p->fout) return ERROR_FILE;
    *parg=p;
    return IO_H_NO_ERROR;
}

const char* getfield(char* line, int num, const char *s)
{
	const char* tok;
	for (tok = strtok(line, s);
			tok && *tok;
			tok = strtok(NULL, s))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}


FILE *OpenExt( char *fname, char *ext, char *rw)
{
    FILE *fid;
    char *cloc;
    int loc=0;
    char *fstr;
    // check extension exist ?
    cloc = strchr(fname, '.');
    if (cloc>0){ // with extension
        loc = (int)(cloc - fname);
        fstr=(char *)malloc(strlen(fname)+5);
        strcpy(fstr,fname);
        if (strlen(ext)!=0)
            strcpy(&fstr[loc+1],ext);
    }else{
        fstr=(char *)malloc(strlen(fname)+2+strlen(ext));
        strcpy(fstr,fname);
        strcat(fstr,".");
        strcat(fstr,ext);
    }
    fid=fopen(fstr,rw);
    if (fid) {
         //   printf("File Open Successful(%s)!\n",fstr);
    }else {
            printf("File Open Fail(%s)!\07\n",fstr);
    }
    free(fstr);
    return fid;
}
void CloseAll(pARGLIST parg)
{
	if (parg->fout!=NULL) fclose(parg->fout);
	if (parg->fin!=NULL)fclose(parg->fin);
    free(parg);
};

int fgetline(FILE *fid, char s[], int N) {
    int c = 0, i;

    for (i=0; i<N-1 && (c=fgetc(fid)) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

int toTokenArray(char *instr, char *tok[], char *delimiters)
{
    int n=(int)strlen(instr);
    int m=(int)strlen(delimiters);
    int i,j,k=0;
    char c;
    tok[k]=instr; k++;
    for(i=0;i<n;i++){
        c=instr[i];
        for(j=0;j<m;j++) {
            if (c==delimiters[j]) {
                instr[i]=0;
                tok[k]=&instr[i+1]; k++;
                break;
            }
        }
    }
    return k-1;
}

