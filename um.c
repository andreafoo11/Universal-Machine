/*
 * um.c
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW6 um
 * Due: April 13, 2020
 * Purpose: Implmentation file for the UM to call necessary functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>
#include <uarray.h>
#include <seq.h>
#include <stack.h>
#include <sys/stat.h>
#include <um-dis.h>
#include "mem_instructions.h"
#include "arith_instructions.h"
#include "io_instructions.h"
#include "um.h"


/* CONSTANTS */
const int NUM_REGISTERS = 8;
const int WORD_SIZE = 4;

/* ENUM FOR OP CODES */
typedef enum Um_opcode {
        CMOV = 0, SEG_LOAD, SEG_STORE, ADD, MULT, DIV,
        NAND, HALT, MAP, UNMAP, OUTPUT, INPUT, LOAD_PROGRAM, LOAD_VALUE
} Um_opcode;

/* FUNCTION PROTOTYPES */
static void set_program(char *filename, Seq_T main_memory);
static void run_program(Seq_T main_memory, UArray_T registers, 
                        Stack_T unmapped_ids, uint32_t program_counter);
static void perform_instruction(Seq_T main_memory, UArray_T registers, 
                                Stack_T unmapped_ids, uint32_t curr_word, 
                                uint32_t *program_counter_p);
static void halt(Seq_T main_memory, UArray_T registers, Stack_T unmapped_ids);

/* 
 * run_machine
 * Input: filename containing instruction words
 * Returns: none 
 * Purpose: initializes data structures to store main memory, registers, 
            segment ids, then calls functions to run program on um
 */
void run_machine(char *filename)
{
    /* intialize data structures */
    uint32_t program_counter = 0;
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    
    /* set all registers to hold 0 */
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = UArray_at(registers, i);
        *curr_reg = 0;
    }
    set_program(filename, main_memory);
    run_program(main_memory, registers, unmapped_ids, program_counter);
}

/* 
 * set_program
 * Input: filename containing instruction words, pointer to main_memory Seq
 * Returns: none 
 * Purpose: reads in and stores instruction words into segment 0
 */
static void set_program(char *filename, Seq_T main_memory)
{
    /* calculate number of instruction words in file */
    struct stat buf;          
    stat(filename, &buf);
    int num_bytes = buf.st_size;
    int num_words = num_bytes / 4;
    
    FILE *fp;
    fp = fopen(filename, "r");
    
    /* create segment 0 */
    UArray_T segment_zero = UArray_new(num_words, WORD_SIZE);
    Seq_addhi(main_memory, segment_zero);
    
    /* read in instruction words by byte and store in segment 0 */
    for (int i = 0; i < num_words; i++) {
        uint32_t *curr_word = (uint32_t *) UArray_at(segment_zero, i);
        unsigned lsb = 24;
        for (int j = 0; j < 4; j++) {
            uint32_t curr_byte = fgetc(fp);
            *curr_word = Bitpack_newu(*curr_word, 8, lsb, curr_byte);
            lsb -= 8;
        }
    }
    fclose(fp);
    
}

/* 

 * run_program
 * Input: pointer to main memory sequence, pointer to registers uarray, 
          pointer to ids stack, program counter
 * Returns: none 
 * Purpose: performs each instructions stored in segment 0
 */
 static void run_program(Seq_T main_memory, UArray_T registers, 
                         Stack_T unmapped_ids, uint32_t program_counter)
 {
    UArray_T segment_zero = Seq_get(main_memory, 0);
    
    /* only continue looping while in bounds of segment 0 */
    while(program_counter < (uint32_t) UArray_length(segment_zero)) {
        /* get current word and perform instruction */
        uint32_t *curr_word = (uint32_t *) UArray_at(segment_zero, 
                                                     program_counter);
        perform_instruction(main_memory, registers, unmapped_ids, 
                            *curr_word, &program_counter);
        segment_zero = Seq_get(main_memory, 0);
    }
 }

/* perform_instruction
 * Input: pointer to main memory sequence, pointer to registers uarray, 
          pointer to ids stack, current instruction word,
          pointer to program counter
 * Returns: none 
 * Purpose: performs a given instruction 
 * Effects: checked runtime error if register indices are out of bounds
            alters program counter by reference
 */
static void perform_instruction(Seq_T main_memory, UArray_T registers, 
                                Stack_T unmapped_ids, uint32_t curr_word, 
                                uint32_t *program_counter_p)
{
    /* unpack instruction word */
    Um_opcode opcode = Bitpack_getu(curr_word, 4, 28);
    unsigned regA = Bitpack_getu(curr_word, 3, 6);
    unsigned regB = Bitpack_getu(curr_word, 3, 3);
    unsigned regC = Bitpack_getu(curr_word, 3, 0);
    unsigned value = Bitpack_getu(curr_word, 25, 0);

    assert(regA < 8 && regB < 8 && regC < 8);
    
    /* call corresponding function based on opcode */
    switch(opcode) {
        case CMOV :
            conditional_move(registers, regA, regB, regC);
            break;
        case SEG_LOAD :
            segmented_load(main_memory, registers, regA, regB, regC);
            break;
        case SEG_STORE :
            segmented_store(main_memory, registers, regA, regB, regC);
            break;
        case ADD:
            add(registers, regA, regB, regC);
            break;
        case MULT :
            multiply(registers, regA, regB, regC);
            break;
        case DIV :
            divide(registers, regA, regB, regC);
            break;
        case NAND :
            bitwise_NAND(registers, regA, regB, regC);
            break;
        case HALT :
            halt(main_memory, registers, unmapped_ids);
            break;
        case MAP:
            map_segment(main_memory, registers, regB, regC, unmapped_ids);
            break;
        case UNMAP :
            unmap_segment(main_memory, registers, regC, unmapped_ids);
            break;
        case OUTPUT :
            output(registers, regC);
            break;
        case INPUT :
            read_input(registers, regC);
            break;
        case LOAD_PROGRAM:
            load_program(main_memory, registers, regB, regC, 
                         program_counter_p);
            break;
        case LOAD_VALUE :
            regA = Bitpack_getu(curr_word, 3, 25);
            load_value(registers, regA, value);
            break;
        default :
            break;
    }
    /* only increment program counter if new program was not loaded */
    if (opcode != LOAD_PROGRAM) {
        (*program_counter_p)++;
    }    
        
}


/* 
 * halt
 * Input: pointer to main memory Sequence, pointer to registers UArray, 
 *        pointer to unmapped ids Stack
 * Returns: none 
 * Purpose: frees memory associated with machine, then halts and exits
 * Effects: checked runtime error if any of the pointers to the data
            structures are NULL
 */
static void halt(Seq_T main_memory, UArray_T registers, Stack_T unmapped_ids)
{    
    assert(main_memory != NULL);
    int length = Seq_length(main_memory);
    /* free each segment in main memory */
    for (int i = 0; i < length; i++) {
        UArray_T curr_seg = Seq_get(main_memory, i);
        if (curr_seg != NULL){
            UArray_free(&curr_seg);
        }
    }
    
    /* free main memory, registers, stack */    
    Seq_free(&main_memory);
    assert(registers != NULL);
    UArray_free(&registers);
    assert(unmapped_ids != NULL);
    Stack_free(&unmapped_ids);
    
    exit(EXIT_SUCCESS);
}