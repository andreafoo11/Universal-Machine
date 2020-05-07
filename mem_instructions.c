/*
 * mem_instructions.c
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
#include <stack.h>
#include <uarray.h>
#include "mem_instructions.h"

/* 
 * map_segment
 * Input: pointer to main memory Sequence, pointer to registers UArray, 
 *        index of registers B and C, pointer to unmapped ids Stack
 * Returns: none 
 * Purpose: creates a new segment in main memory Sequence and with num words
 *          equal to value in regC. all words are initialized to 0, and new
 *          segment id is placed in regB
 * Effects: checked runtime error if trying to map segment 0    
 */
void map_segment(Seq_T main_memory, UArray_T registers, int regB, int regC, 
                 Stack_T unmapped_ids)
 {
     uint32_t segment_id = 0;
     
     if (Stack_empty(unmapped_ids)) {
         /* if there are no available segment ids, create new id */
         segment_id = Seq_length(main_memory);
     }
     else {
         /* if there are available segment ids on stack, pop */
         segment_id = (uint32_t)(uintptr_t) Stack_pop(unmapped_ids);
     }
     assert(segment_id != 0);
     unsigned *num_words = UArray_at(registers, regC);
     
     /* allocate memory for new segment */
     UArray_T new_segment = UArray_new(*num_words, sizeof(uint32_t));
     uint32_t length = (uint32_t) Seq_length(main_memory);
     
     /* add new segment to sequence in correct index */
     if (segment_id < length) {
         Seq_put(main_memory, segment_id, (void *) new_segment);
     } else {
         Seq_addhi(main_memory, (void *) new_segment);
     }
     
     /* set all indices of new segment to 0 */
     int uarray_length = UArray_length(new_segment);
     for (int i = 0; i < uarray_length; i++) {
        uint32_t *curr = UArray_at(new_segment, i);
        *curr = 0;
     }
     
     /* place new segment id in register B */
     unsigned *new_id = UArray_at(registers, regB);
     *new_id = segment_id;

 }

/* 
 * unmap_segment
 * Input: pointer to main memory Sequence, pointer to registers UArray, 
 *        index of register C, pointer to unmapped ids Stack
 * Returns: none 
 * Purpose: unmaps segment with segment id held in regC and adds segment id 
 *          to unmapped ids Stack
 * Effects: checked runtime error if trying to unmap segment 0
 */
void unmap_segment(Seq_T main_memory, UArray_T registers, int regC, 
                    Stack_T unmapped_ids)
{
    /* get correct segment */
    uint32_t *curr_id = UArray_at(registers, regC);
    assert(*curr_id != 0);
    UArray_T curr_segment = Seq_get(main_memory, *curr_id);
    
    /* free segment */
    if (curr_segment != NULL) {
        UArray_free(&curr_segment);
    }
    Seq_put(main_memory, *curr_id, NULL);
    
    /* push newly available segment id onto id stack */
    Stack_push(unmapped_ids, (void *) (uintptr_t) *curr_id);
}

/* 
 * segmented_load
 * Input: pointer to main memory Sequence, pointer to registers UArray, 
 *        index of registers A, B, and C
 * Returns: none 
 * Purpose: loads value of word at $m[$r[B]][$r[C]] in regA
 * Effects: checked runtime error if segment to load from is out of bounds
 */
void segmented_load(Seq_T main_memory, UArray_T registers, int regA, int regB, 
                    int regC)
{
    /* get correct segment */
    uint32_t *segment_id = UArray_at(registers, regB);
    uint32_t length = (uint32_t) Seq_length(main_memory);
    assert(*segment_id < length);
    UArray_T curr_segment = Seq_get(main_memory, *segment_id);
    
    /* get correct word in segment */
    uint32_t *word_index = UArray_at(registers, regC);
    uint32_t *new_value = UArray_at(curr_segment, *word_index);
    
    /* update register A to hold value from segment */
    uint32_t *old_value = UArray_at(registers, regA);
    *old_value = *new_value;
}

/* 
 * segmented_store
 * Input: pointer to main memory Sequence, pointer to registers UArray, 
 *        index of registers A, B, and C
 * Returns: none 
 * Purpose: stores value regC in word at $m[$r[A]][$r[B]] 
 * Effects: checked runtime error if segment to store into is out of bounds
 */
void segmented_store(Seq_T main_memory, UArray_T registers, int regA, int regB,
                     int regC)
{
    /* get correct segment */
    uint32_t length = (uint32_t) Seq_length(main_memory);
    uint32_t *segment_id = UArray_at(registers, regA);
    assert(*segment_id < length);
    UArray_T curr_segment = Seq_get(main_memory, *segment_id);
    
    /* get correct word in segment */
    uint32_t *word_index = UArray_at(registers, regB);
    uint32_t *old_value = UArray_at(curr_segment, *word_index);
    
    /* update word in segment to hold value in register C */
    uint32_t *new_value = UArray_at(registers, regC);
    *old_value = *new_value;
}

/* 
 * load_program
 * Input: pointer to main memory Sequence, pointer to registers UArray, 
 *        index of registers B and C
 * Returns: none 
 * Purpose: duplicates segment $m[$r[B]] and replaces segment 0, 
 *          then resets program counter to point to $m[0][$r[C]]
 */
void load_program(Seq_T main_memory, UArray_T registers, int regB, int regC, 
                  uint32_t *program_counter)
{
    uint32_t *segment_id = UArray_at(registers, regB);
    
    /* if segment to load is segment 0, just update program counter */
    if (*segment_id == 0) {
        uint32_t *regC_val = UArray_at(registers, regC);
        *program_counter = *regC_val;
        return;
    }
    
    /* duplicate segment to load */
    UArray_T curr_segment = Seq_get(main_memory, *segment_id);
    UArray_T duplicate = UArray_copy(curr_segment, UArray_length(curr_segment));
    
    /* free old segment 0 */
    UArray_T segment_zero = Seq_get(main_memory, 0);
    UArray_free(&segment_zero);
    
    /* replace segment zero with duplicate and update program counter */
    Seq_put(main_memory, 0, (void *) duplicate);
    uint32_t *regC_val = UArray_at(registers, regC);
    *program_counter = *regC_val;
}
