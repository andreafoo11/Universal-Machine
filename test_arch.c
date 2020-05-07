/*
 * test_arch.c
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW6 um
 * Due: April 13, 2020
 * Purpose: Architecture tests for UM memory implementations
 */

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <uarray.h>
#include <stack.h>
#include "mem_instructions.h"

/* CONSTANTS */
const int NUM_REGISTERS = 8;
const int WORD_SIZE = 4;

/* 
 * test_mapping
 * Input: none
 * Returns: none 
 * Purpose: tests the map segment function by initializing sequence for main 
 *          memory, stack of unmapped ids, and Uarray for registers. 
 *          Create segment zero and then map another 99 segments, then printing
 *          out the segment ids to make sure that the segments are properly
 *          mapped in sequence in main memory
 */
void test_mapping()
{
    /* Initializing main memory, unmapped ids, and registers */
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, i);
        *curr_reg = 0;
    }
    
    /* create segment zero and add to main memory */
    UArray_T seg_zero = UArray_new(2, sizeof(uint32_t));
    
    /* loop to map new 99 new segments after segment zero */
    Seq_addhi(main_memory, seg_zero);
    for (int i = 1; i < 100; i++){
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = i;
        map_segment(main_memory, registers, 1, 2, unmapped_ids);
        curr_reg = (uint32_t *) UArray_at(registers, 1);
        fprintf(stderr, "segment_id %d\n", *curr_reg);
    }
}


/* 
 * test_unmapping1
 * Input: none
 * Returns: none 
 * Purpose: tests the unmap segment function by initializing sequence for main 
 *          memory, stack of unmapped ids, and Uarray for registers. 
 *          Create segment zero. Map and then immediately unmap segment, which
 *          leads to segment id 1 to be constantly reused so segment id should
 *          be 1 for when looping 1 to 100.
 */
void test_unmapping1()
{
    /* Initializing main memory, unmapped ids, and registers */
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, i);
        *curr_reg = 0;
    }
    
    /* create segment zero and add to main memory */
    UArray_T seg_zero = UArray_new(2, sizeof(uint32_t));
    Seq_addhi(main_memory, seg_zero);
    
    /* loop to map and unmap 99 segments after segment zero */
    for (int i = 1; i < 100; i++){
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = i;
        map_segment(main_memory, registers, 1, 2, unmapped_ids);
        unmap_segment(main_memory, registers, 1, unmapped_ids);
        curr_reg = (uint32_t *) UArray_at(registers, 1);
        fprintf(stderr, "segment_id: %d\n ", *curr_reg);
    }
    
}


/* 
 * test_unmapping2
 * Input: none
 * Returns: none 
 * Purpose: tests the unmap segment function by initializing sequence for main 
 *          memory, stack of unmapped ids, and Uarray for registers. 
 *          Create segment zero. Map 99 segments and then unmap them. different
 *          from previous test because the unmap segments should have ids 1 to 
 *          99. 
 */
void test_unmapping2()
{
    /* Initializing main memory, unmapped ids, and registers */
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, i);
        *curr_reg = 0;
    }
    
    /* create segment zero and add to main memory */
    UArray_T seg_zero = UArray_new(2, sizeof(uint32_t));
    Seq_addhi(main_memory, seg_zero);
    
    /* loop to map 99 segments after segment zero */
    for (int i = 1; i < 100; i++){
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = i;
        map_segment(main_memory, registers, 1, 2, unmapped_ids);
        curr_reg = (uint32_t *) UArray_at(registers, 1);
    }
    
    /* loop to unmap the previously mapped segments */
    for (int i = 1; i < 100; i++){
        fprintf(stderr, "unmapped segment id: %d\n ", i);
        unmap_segment(main_memory, registers, 1, unmapped_ids);
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = i;
    }
}

/* 
 * test_segmented_load
 * Input: none
 * Returns: none 
 * Purpose: tests the segment load function by storing a value in a segment
 *          and then calling segmented load to make sure it correctly loads 
 *          value from that segment
 */
void test_segmented_load() 
{
    /* Initializing main memory, unmapped ids, and registers */
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, i);
        *curr_reg = 0;
    }
    /* create segment zero and add to main memory */
    UArray_T seg_zero = UArray_new(2, sizeof(uint32_t));
    Seq_addhi(main_memory, seg_zero);
    
    /* loop to map 99 segments after segment zero, store value 6 at index 0 of 
     * every segment. Load the values from the segment store in register 5.
     */
    for (int i = 1; i < 100; i++){
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = 1;
        map_segment(main_memory, registers, 1, 2, unmapped_ids);
        
        uint32_t *seg_id = (uint32_t *) UArray_at(registers, 1);
        
        UArray_T curr_seg = Seq_get(main_memory, *seg_id);
        uint32_t *curr_val = UArray_at(curr_seg, 0);
        *curr_val = 6;
        segmented_load(main_memory, registers, 5, 1, 0);
        
        uint32_t *curr_reg5 = (uint32_t *) UArray_at(registers, 5);
        fprintf(stderr, "i: %d\n value in register 5: %d\n", i, *curr_reg5);
    }
}

/* 
 * test_segmented_load2
 * Input: none
 * Returns: none 
 * Purpose: tests the map segment function by initializing sequence for main 
 *          memory, stack of unmapped ids, and Uarray for registers. 
 *          Create segment zero and then in a loop maps a segments,
 *          unmaps that segment, and attempts to load a value from that
 *          unmapped segment. 
 * Effects: Should cause program to abort
 */
void test_segmented_load2() 
{
    /* Initializing main memory, unmapped ids, and registers */
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, i);
        *curr_reg = 0;
    }
    
    /* create segment zero and add to main memory */
    UArray_T seg_zero = UArray_new(2, sizeof(uint32_t));
    Seq_addhi(main_memory, seg_zero);
    
    /* loop to map 99 new segments, unmap those segments, 
       and attempts to load value from unmapped segment.
       Should cause program to abort */
    for (int i = 1; i < 100; i++){
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = 1;
        map_segment(main_memory, registers, 1, 2, unmapped_ids);
        unmap_segment(main_memory, registers, 1, unmapped_ids);

        uint32_t *seg_id = (uint32_t *) UArray_at(registers, 1);
        fprintf(stderr, "segment id: %d\n", *seg_id);
        
        UArray_T curr_seg = Seq_get(main_memory, *seg_id);
        uint32_t *curr_val = UArray_at(curr_seg, 0);
        *curr_val = 6;
        segmented_load(main_memory, registers, 5, 1, 0);
        
        uint32_t *curr_reg5 = (uint32_t *) UArray_at(registers, 5);
        fprintf(stderr, "i: %d\n value in register 5: %d\n", i, *curr_reg5);
    }
}

/* 
 * test_segmented_load3
 * Input: none
 * Returns: none 
 * Purpose: tests the map segment function by initializing sequence for main 
 *          memory, stack of unmapped ids, and Uarray for registers. 
 *          Create segment zero and then map another 99 segments,
 *          loads a values into each segment, unmaps segment, then maps a 
 *          new segment. 
 */
void test_segmented_load3() 
{
    /* Initializing main memory, unmapped ids, and registers */
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, i);
        *curr_reg = 0;
    }
    
    /* create segment zero and add to main memory */
    UArray_T seg_zero = UArray_new(2, sizeof(uint32_t));
    Seq_addhi(main_memory, seg_zero);
    
    /* loop to map 99 new segments, load a value into each segment, 
       then unmap the segment and map a new segment */
    for (int i = 1; i < 100; i++){
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = 1;
        map_segment(main_memory, registers, 1, 2, unmapped_ids);

        uint32_t *seg_id = (uint32_t *) UArray_at(registers, 1);
        fprintf(stderr, "segment id: %d\n", *seg_id);

        UArray_T curr_seg = Seq_get(main_memory, *seg_id);
        uint32_t *curr_val = UArray_at(curr_seg, 0);
        *curr_val = 1000000;
        segmented_load(main_memory, registers, 5, 1, 0);

        uint32_t *curr_reg5 = (uint32_t *) UArray_at(registers, 5);
        fprintf(stderr, "value in register 5: %d\n", *curr_reg5);
        unmap_segment(main_memory, registers, 1, unmapped_ids);
        map_segment(main_memory, registers, 1, 2, unmapped_ids);
    }
}

/* 
 * test_segmented_store
 * Input: none
 * Returns: none 
 * Purpose: tests the map segment function by initializing sequence for main 
 *          memory, stack of unmapped ids, and Uarray for registers. 
 *          Create segment zero and then map another 99 segments, stores and 
 *          loads the values into each segment, then prints out value to 
 *          ensure it matches the expected value
 */
void test_segmented_store() 
{
    /* Initializing main memory, unmapped ids, and registers */
    Seq_T main_memory = Seq_new(100);
    Stack_T unmapped_ids = Stack_new();
    UArray_T registers = UArray_new(NUM_REGISTERS, WORD_SIZE);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, i);
        *curr_reg = 0;
    }
    
    /* create segment zero and add to main memory */
    UArray_T seg_zero = UArray_new(2, sizeof(uint32_t));
    Seq_addhi(main_memory, seg_zero);
    
    /* loop to map 99 new segments, store and load value of i from segments */
    for (int i = 1; i < 100; i++){
        uint32_t *curr_reg = (uint32_t *) UArray_at(registers, 2);
        *curr_reg = 1;
        map_segment(main_memory, registers, 1, 2, unmapped_ids);
        
        uint32_t *curr_reg5 = (uint32_t *) UArray_at(registers, 5);
        *curr_reg5 = i; 
        segmented_store(main_memory, registers, 1, 0, 5);
        segmented_load(main_memory, registers, 5, 1, 0);
        
        fprintf(stderr, "expected value in register 5: %d\n", i);
        fprintf(stderr, "value in register 5: %d\n", *curr_reg5);
    }
}


int main() 
{
    /* NOTE: test_segmented_load2 should cause UM to fail
    test_mapping();
    test_unmapping1();
    test_unmapping2();
    test_segmented_load();
    test_segmented_load2(); 
    test_segmented_load3();
    test_segmented_store();
    */
    return 0;
}
