/*
 * arith_instructions.c
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW6 um
 * Due: April 13, 2020
 * Purpose: Implemntation file for the arithmetic and logical operation 
 *          instructions 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>
#include <uarray.h>
#include "arith_instructions.h"


/* 
 * add
 * Input: a pointer to the uarray registers and 3 integers 
 *        that specify the indices of the registers
 * Returns: none
 * Purpose: add contents of register B and register C and 
 *          store in register A
 */
void add(UArray_T registers, int regA, int regB, int regC)
{
    /* Gets pointers to registers at given indices */
    uint32_t *regA_val = UArray_at(registers, regA);
    uint32_t *regB_val = UArray_at(registers, regB);
    uint32_t *regC_val = UArray_at(registers, regC);
    
    /* Stores result of add computation to a register */
    *regA_val = (*regB_val + *regC_val); 
}


/* 
 * multiply
 * Input: a pointer to the uarray registers and 3 integers 
 *        that specify the indices of the registers
 * Returns: none
 * Purpose: multiply contents of register B and register C 
 *          and store in register A
 */
void multiply(UArray_T registers, int regA, int regB, int regC)
{
    /* Gest pointers to registers at given indices */
    uint32_t *regA_val = UArray_at(registers, regA);
    uint32_t *regB_val = UArray_at(registers, regB);
    uint32_t *regC_val = UArray_at(registers, regC);
    
    /* Stores result of multiply computation to a register */
    *regA_val = ((*regB_val) * (*regC_val)); 
}


/* 
 * divide
 * Input: a pointer to the uarray registers and 3 integers that 
 *        specify the indices of the registers
 * Returns: none
 * Purpose: divide contents of register B and register C and 
 *         store in register A
 */
void divide(UArray_T registers, int regA, int regB, int regC)
{
    /* Gest pointers to registers at given indices */
    uint32_t *regA_val = UArray_at(registers, regA);
    uint32_t *regB_val = UArray_at(registers, regB);
    uint32_t *regC_val = UArray_at(registers, regC);
    
    /* Stores result of divide computation to a register */
    *regA_val = ((*regB_val) / (*regC_val));
  
}


/* 
 * load_value
 * Input: a pointer to the uarray registers and 3 integers 
 *        that specify the indices of the registers
 * Returns: none
 * Purpose: Stores given value at the index of the 
 *          register given
 */
void load_value(UArray_T registers, int regA, int value)
{
    /* Gest pointers to register at given index */
    uint32_t *curr_val = UArray_at(registers, regA);
    /* Stores a value to a register */
    *curr_val = value;
}

/* 
 * conditional_move
 * Input: a pointer to the uarray registers and 3 integers 
 *        that specify the indices of the registers
 * Returns: none
 * Purpose: If value in register C is not 0, then store contents of
 *          register B in register A
 */
void conditional_move(UArray_T registers, int regA, int regB, int regC)
{
    /* Gest pointers to register at given indices */
    uint32_t *regA_val = UArray_at(registers, regA);
    uint32_t *regB_val = UArray_at(registers, regB);
    uint32_t *regC_val = UArray_at(registers, regC);
    
    /* Stores value of one register to another if regC is not zero */
    if (*regC_val != 0) {
        *regA_val = *regB_val;
    }
    
}


/* 
 * bitwise_NAND
 * Input: a pointer to the uarray registers and 3 integers 
 *        that specify the indices of the registers
 * Returns: none
 * Purpose: Bitwise NAND register B and C and store result in register A
 */
void bitwise_NAND(UArray_T registers, int regA, int regB, int regC)
{
    /* Gest pointers to register at given indices */
    uint32_t *regA_val = UArray_at(registers, regA);
    uint32_t *regB_val = UArray_at(registers, regB);
    uint32_t *regC_val = UArray_at(registers, regC);
  
    /* Stores result of bitwise NAND logical operation to a register */
    uint32_t and_val = ((uint32_t) *regB_val) & ((uint32_t) *regC_val);
    *regA_val = (uint32_t) ~and_val;
}
