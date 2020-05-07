/*
 * mem_instructions.h
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW6 um
 * Due: April 13, 2020
 * Purpose: Interface file for the memory instructions implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <bitpack.h>
#include <seq.h>
#include <uarray.h>

#ifndef MEM_INSTRUCTIONS_INCLUDED
#define MEM_INSTRUCTIONS_INCLUDED

/* Creates a new segment in main memory */
void map_segment(Seq_T main_memory, UArray_T registers, int regB, int regC, 
                 Stack_T unmapped_ids);

/* unmaps segment at segment id held in regC */
void unmap_segment(Seq_T main_memory, UArray_T registers, int regC, 
                   Stack_T unmapped_ids);


/* loads value of word at $m[$r[B]][$r[C]] in regA*/
void segmented_load(Seq_T main_memory, UArray_T registers, int regA, int regB, 
                    int regC);

/* Stores value regA in word at $m[$r[B]][$r[C]] in regA */
void segmented_store(Seq_T main_memory, UArray_T registers, int regA, int regB, 
                     int regC);

/* duplicates segment $m[$r[B]] and replaces segment 0, then resets program 
 * counter to point to $m[0][$r[C]] */
void load_program(Seq_T main_memory, UArray_T registers, int regB, int regC, 
                  uint32_t *program_counter);

#endif
