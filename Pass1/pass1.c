//ERROR: LOCCTR NOT INCREMENTED FOR LABELS IN OUTPUT FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void pass1(char label[10],char opcode[10],char operand[10],char mneumonic[10],char code[10]);

int main(){
  char label[10],opcode[10],operand[10];
  //For reading from input file
  char mneumonic[10],code[10];
  //For reading from optab
  pass1(label,opcode,operand,mneumonic,code);
}

void pass1(char label[10],char opcode[10],char operand[10],char mneumonic[10],char code[10]){
  int locctr,start,length;
  FILE *inp,*op,*sym,*out;

  inp=fopen("input.txt","r");
  op=fopen("optab.txt","r");
  sym=fopen("symtab.txt","w");
  out=fopen("output.txt","w+");

  fscanf(inp,"%s\t%s\t%s",label,opcode,operand); //Reading first line of input file
  if(strcmp(opcode,"START")==0){
    start=atoi(operand);
    locctr=start;
  }
  else{
    locctr=0;
  }

  fprintf(out,"\t%s\t%s\t%s\n",label,opcode,operand );  //Write first line to ouput file(First line does not require an address)
  fscanf(inp,"%s\t%s\t%s",label,opcode,operand); //Read next line

  while(strcmp(opcode,"END")!=0){
    fprintf(out,"%d\t%s\t%s\t%s\n",locctr,label,opcode,operand );

    if(strcmp(label,"**")!=0){      //** implies no label
      fprintf(sym,"%s\t%d",label,locctr); //If symbol exists put in symtab
    }
    fscanf(op,"%s\t%s",mneumonic,code);

    while(strcmp(mneumonic,"END")!=0){
      if(strcmp(mneumonic,opcode)==0){
        locctr+=3;    //Increment locctr if the mneumonic exists in the optab
        break;
      }
      fscanf(op,"%s\t%s",mneumonic,code);
    }
    //things left to handle: WORD,RESW,RESB,BYTE
    if(strcmp(opcode,"RESB")==0){
      locctr+=atoi(operand);
    }
    else if(strcmp(opcode,"RESW")==0){
      locctr+=(atoi(operand)*3);
    }
    else if(strcmp(opcode,"WORD")==0){
      locctr+=3;
    }
    if(strcmp(opcode,"BYTE")==0){
      ++locctr;
    }
    fscanf(inp,"%s\t%s\t%s",label,opcode,operand);
  }
  fprintf(out,"%d\t%s\t%s\t%s",locctr,label,opcode,operand );

  length= locctr-start;
  fprintf(out, "\nLength= %d\n",length );  //Write the length of the program into output file

  fclose(inp);
  fclose(out);
  fclose(sym);
  fclose(op);
}
