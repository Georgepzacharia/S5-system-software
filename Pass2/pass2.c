#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char t1[20],t2[20],t3[20],t4[20],address[20],label[20],opcode[20],operand[20];//t1-t4l: Array for temporary storage
char length[10],size[10],a[10],ad[10],start[10];
int s=-1,o=-1,i,j; //s-symtab index,o-optab index
FILE *fp1,*fp2,*fp3,*fp4,*fp5;
//FP1-inter,FP2-symtab,FP3-optab,FP4-length,FP5-objectcode
struct sym{
    char label[10];
    char addr[10];
}ST[30];

struct op{
    char mneumonic[10];
    char opcode[10];
}OT[30];

void read_opt(){  //Put values in optab to struct array
  do{
  o++;
  fscanf(fp3,"%s%s",OT[o].mneumonic,OT[o].opcode);
  }
  while(getc(fp3)!=EOF);
}

void read_sym(){  //Put values in symtab to struct array
  do{
  s++;
  fscanf(fp2,"%s%s",ST[s].label,ST[s].addr);
  }
  while(getc(fp2)!=EOF);
}

void read_line(){//Fn to read a single line of intermediate file and categorize them into types of instructions

  strcpy(t1,"");strcpy(t2,"");strcpy(t3,"");strcpy(t4,"");
  fscanf(fp1,"%s",t1);
  if(getc(fp1)!='\n'){ //Only one instruction(no label,no operand) in the line(just the address)
    fscanf(fp1,"%s",t2);
  }
  if(getc(fp1)=='\n'){ //Only two instruction(no label) in the line(END or address and mneumonic)
      if(strcmp(t1,"END")==0){
        strcpy(opcode,t1);strcpy(operand,t2);
        }
        else{
          strcpy(address,t1);strcpy(opcode,t2);
        }
      }
    else{
      fscanf(fp1,"%s",t3);
      if(getc(fp1)=='\n'){ //Three instuctions in the line eg. 1000 STA 13
        strcpy(address,t1);strcpy(opcode,t2);strcpy(operand,t3);
      }
      else{
        fscanf(fp1,"%s",t4); //All four instructions present in the line
        strcpy(address,t1);strcpy(label,t2);strcpy(opcode,t3);strcpy(operand,t4);
      }
  }
}

void main(){
  fp1=fopen("inter.txt","r");
  fp2=fopen("symtab.txt","r");
  fp3=fopen("optab.txt","r");
  fp4=fopen("length.txt","r");
  fp5=fopen("objcode.txt","w");

  fscanf(fp4,"%s%s",length,size);   //Length of program and length of the object code
  read_opt();
  read_sym();
  fscanf(fp1,"%s%s%s",label,opcode,operand);
  strcpy(start,operand);
  fprintf(fp5, "H^%s^%s^%s\n",label,operand,length);  //H^prg name^Starting address^length of obj code
  fprintf(fp5,"T^%s^%s",operand,size);//T^Starting address^Length of object code
  read_line();  //After Header and text record is initialized call read_line

  //Handle resw,resb(Do nothing),BYTE,WORD
  while(strcmp(opcode,"END")!=0){

    if(strcmp(opcode,"BYTE")==0){
      sprintf(ad,"%x",operand[2]);  //BYTE C'Z' operand[2]=Z
      fprintf(fp5, "^%s",ad);
    }
    else if(strcmp(opcode,"WORD")==0){
      fprintf(fp5, "^%s",operand);
    }
    else if((strcmp(opcode,"RESB")==0) || strcmp(opcode,"RESW")==0){}
    else{
      //Find the index of structs for current operand
      j=0; //OPTAB
      while(strcmp(opcode,OT[j].mneumonic)!=0){j++;}
      i=0; //SYMTAB
      while(strcmp(operand,ST[i].label)!=0){i++;}
      fprintf(fp5, "^%s%s",OT[j].opcode,ST[i].addr);
    }
    read_line();  //Read subsequent lines
  }
  fprintf(fp5, "\nE^%s",start);
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
  fclose(fp5);
}
