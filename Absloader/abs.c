#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(){
  FILE *inp,*op;
  char line[70];
  inp=fopen("input.txt","r");
  op=fopen("output.txt","w");
  char type[10],length[10],obcode[10];
  int locctr,startaddr;
  while(!(feof(inp))){
    fscanf(inp,"%s",type);
    if(strcmp(type,"T")==0){
      fscanf(inp,"%x",&startaddr);
      locctr=--startaddr;
      fscanf(inp,"%s",length);
      while(getc(inp)!='\n'){
        fscanf(inp,"%s",obcode);
        fprintf(op, "%x\t%s\n",++locctr,obcode);
      }
    }
}
  fclose(inp);
  fclose(op);
}
