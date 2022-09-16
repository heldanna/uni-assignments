// Group 14
// isabella bierdzinski
// Guan-Yu Lin
// Emily Mendez
// Heldanna Solomon

#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult,char *Zero)
{
  switch ((int)ALUControl) {
  case 0: //adding
    *ALUresult = A + B;
    break;

  case 1: //subtracting
    *ALUresult = A - B;
    break;

  case 2:
    if ((signed)A < (signed)B)
      *ALUresult = 1;
    else
      *ALUresult = 0;
    break;

  case 3:
    if (A < B)
      *ALUresult = 1;
    else
      *ALUresult = 0;
    break;

  case 4: //and
    *ALUresult = A & B;
    break;

  case 5: //or
    *ALUresult = A | B;
    break;

  case 6: //bit shifting
    *ALUresult = B << 16;
    break;

  case 7:
    *ALUresult = ~A;
    break;
  }

  if ((*ALUresult)==0)
    *Zero = 1;
  else
    *Zero = 0;

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
  // If word is not aligned
  if (PC % 4 != 0){
    return 1;
  }

  *instruction = Mem[PC >> 2];
  return 0;

}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
  *op = (instruction & 0b11111100000000000000000000000000) >> 26; // shift right >> after mask &
  *r1 = (instruction & 0b11111000000000000000000000) >> 21;
  *r2 = (instruction & 0b111110000000000000000) >> 16;
  *r3 = (instruction & 0b1111100000000000) >> 11;
  *funct = instruction & 0b00000000000000000000000000111111;
  *offset = instruction & 0b00000000000000000111111111111111;
  *jsec = instruction & 0b00000011111111111111111111111111;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls) {
  switch (op) {

    // r-format
    case 0:
      controls->RegDst = 1;
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 7;
      controls->MemWrite = 0;
      controls->ALUSrc = 0;
      controls->RegWrite = 1;
      break;

    // addi
    case 8:
      controls->RegDst = 0;
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 0;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // load word
    case 35:
      controls->RegDst = 0;
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 1;
      controls->MemtoReg = 1;
      controls->ALUOp = 0;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // store word
    case 43:
      controls->RegDst = 0;//2
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;//2
      controls->ALUOp = 0;
      controls->MemWrite = 1;
      controls->ALUSrc = 1;
      controls->RegWrite = 0;
      break;

    // lower upper intermediate
    case 15:
      controls->RegDst = 0;
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 6;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // branch on equal
    case 4:
      controls->RegDst = 0;//2
      controls->Jump = 0;
      controls->Branch = 1;
      controls->MemRead = 0;
      controls->MemtoReg = 0;//2
      controls->ALUOp = 1;
      controls->MemWrite = 0;
      controls->ALUSrc = 0;
      controls->RegWrite = 0;
      break;

    // set less than intermediate
    case 10:
      controls->RegDst = 0; //1
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 2;
      controls->MemWrite = 0;
      controls->ALUSrc = 1; //0
      controls->RegWrite = 1;
      break;

    // set less than intermediate unsigned
    case 11:
      controls->RegDst = 0;
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 3;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // jump
    case 2:
      controls->RegDst = 0; //2
      controls->Jump = 1;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0; //2
      controls->ALUOp = 0;
      controls->MemWrite = 0;
      controls->ALUSrc = 0; //2
      controls->RegWrite = 0;
      break;

    default:
      return 1;
  }
  return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2) {
  *data1 = Reg[r1];
  *data2 = Reg[r2];
  //Writing read values to data1 and data2
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value) {
  if ((offset >> 15) == 1)
    *extended_value = offset | 0xffff0000;
  else
    *extended_value = offset & 0x0000ffff;
  //sign extended offset value assigned to extended value
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
  if (ALUOp == 0x7)
  {
    switch (funct) {
      case 0x20:
        ALUOp = 0x0;
        break;
      case 0x24:
        ALUOp = 0x4;
        break;
      case 0x25:
        ALUOp = 0x5;
        break;
      case 0x2a:
        ALUOp = 0x2;
        break;
      case 0x2b:
        ALUOp = 0x3;
        break;
      case 0x22:
        ALUOp = 0x1;
        break;
      default:
        return 1;
    }

  }
  else
  {
    switch(ALUOp)
      {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
          break;
        default:
          return 1; // halt
      }
  }

  if (ALUSrc == 1)
  {
    data2 = extended_value;
  }
  ALU(data1, data2, ALUOp, ALUresult, Zero);
  return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead,unsigned *memdata, unsigned *Mem)
{
  if (MemRead == 1) //if reading, read content from mem regarding ALUresult to memdata.
  {
    if ((ALUresult % 4) == 0)
    {
      *memdata = Mem[ALUresult >> 2];
    }
    else
      return 1; // invalid


 }
  if (MemWrite == 1) //if writing, write content from mem regarding  ALUresult to memdata.
    {
      if ((ALUresult % 4) == 0)
      {
        data2 = Mem[ALUresult >> 2];
      }
      else
        return 1; //halt


  }
  return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata,unsigned ALUresult, char RegWrite, char RegDst,char MemtoReg, unsigned *Reg)
{
  if (RegWrite == 1 && MemtoReg == 1)
  {
    if (RegDst == 0) {
      Reg[r2] = memdata; //write data to reg adressed by r2
    }
    else {
      Reg[r3] = memdata; //write data to reg adressed by r3
    }
  }
  if (RegWrite == 1 && MemtoReg == 0) {
    if (RegDst == 0) {
      Reg[r2] = ALUresult; //write data to reg adressed by r2
    }
    else {
      Reg[r3] = ALUresult; //write data to reg adressed by r3
    }
  }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump,
               char Zero, unsigned *PC) {
  *PC += 4;
  if (Zero == 1 && Branch == 1)
    *PC += extended_value << 2; //extd value shift left 2 added to *PC
  if (Jump == 1) //if jumping, shift jsec ;
  {
    *PC = (jsec << 2) | (*PC & 0xf0000000);
  }
}
