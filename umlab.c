/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 * Edited by: Isabelle Lai (ilai01) and Andrea Foo (afoo01)
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
    Um_instruction word = 0;
    word = Bitpack_newu(word, 4, 28, op);
    word = Bitpack_newu(word, 3, 0, rc);
    word = Bitpack_newu(word, 3, 3, rb);
    word = Bitpack_newu(word, 3, 6, ra);
    return word;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
    Um_instruction word = 0;
    word = Bitpack_newu(word, 4, 28, LV);
    word = Bitpack_newu(word, 3, 25, ra);
    word = Bitpack_newu(word, 25, 0, val);
    return word;
}

/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b, 
                                      Um_register c)
{
    return three_register(MUL, a, b, c);
}

static inline Um_instruction divide(Um_register a, Um_register b, Um_register c)
{
    return three_register(DIV, a, b, c);
}

static inline Um_instruction conditional_move(Um_register a, Um_register b, 
                                              Um_register c)
{
    return three_register(CMOV, a, b, c);
}

static inline Um_instruction bitwise_NAND(Um_register a, Um_register b, 
                                          Um_register c)
{
    return three_register(NAND, a, b, c);
}

static inline Um_instruction input(Um_register c)
{
    return three_register(IN, 0, 0, c);
}

static inline Um_instruction output(Um_register c)
{
    return three_register(OUT, 0, 0, c);
}

static inline Um_instruction map_segment(Um_register b, Um_register c)
{
    return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_segment(Um_register c)
{
    return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction segmented_load(Um_register a, Um_register b, 
                                            Um_register c )
{
    return three_register(SLOAD, a, b, c);
}

static inline Um_instruction segmented_store(Um_register a, Um_register b, 
                                             Um_register c )
{
    return three_register(SSTORE, a, b, c);
}

static inline Um_instruction load_program(Um_register b, Um_register c)
{
    return three_register(LOADP, 0, b, c);
}

/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}

/* Unit tests for the UM */

/*
 * build halt test 
 * Expected Effects: halts
 */
void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

/*
 * build halt verbose test 
 * Expected Effects: halts before outputting anything
 */
void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

/*
 * build add test 
 * Expected Effects: outputs '6'
 */
void build_add_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3)); 
        append(stream, halt());
}

/*
 * build multiply test
 * Expected Effects: outputs 'n'
 */
void build_multiply_test(Seq_T stream)
{
        append(stream, loadval(r2, 11));
        append(stream, loadval(r3, 10));
        append(stream, multiply(r1, r2, r3));
        append(stream, output(r1)); 
        append(stream, halt());
}

/*
 * build multiply test edge1
 * Expected Effects: outputs nothing (null)
 */
void build_multiply_test_edge1(Seq_T stream)
{
        append(stream, loadval(r2, 0));
        append(stream, loadval(r3, 2));
        append(stream, multiply(r1, r2, r3));
        append(stream, output(r1)); 
        append(stream, halt());
}

/*
 * build divide test
 * Expected Effects: outputs 'd'
 */
void build_divide_test(Seq_T stream)
{
        append(stream, loadval(r2, 400));
        append(stream, loadval(r3, 4));
        append(stream, divide(r1, r2, r3)); 
        append(stream, output(r1)); 
        append(stream, halt());
}

/*
 * build divide test edge2
 * Expected Effects: outputs nothing (null)
 */
void build_divide_test_edge2(Seq_T stream)
{
        append(stream, loadval(r2, 48));
        append(stream, loadval(r3, 0));
        append(stream, divide(r1, r3, r2)); 
        append(stream, output(r1)); 
        append(stream, halt());
}

/*
 * build output test
 * Expected Effects: outputs '6'
 */
void build_output_test(Seq_T stream)
{
        append(stream, loadval(r1, 54));
        append(stream, output(r1));  
        append(stream, halt());
}

/*
 * build cmov test
 * Expected Effects: outputs 't'
 */
void build_cmov_test(Seq_T stream)
{
    append(stream, loadval(r3, 1));
    append(stream, loadval(r2, 116));
    append(stream, loadval(r1, 102));
    append(stream, conditional_move(r1, r2, r3));
    append(stream, output(r1)); 
    append(stream, halt());
}

/*
 * build cmov test edge1
 * Expected Effects: outputs 'f'
 */
void build_cmov_test_edge1(Seq_T stream)
{
    append(stream, loadval(r3, 0));
    append(stream, loadval(r2, 116));
    append(stream, loadval(r1, 102));
    append(stream, conditional_move(r1, r2, r3));
    append(stream, output(r1)); 
    append(stream, halt());
}

/*
 * build NAND test
 * Expected Effects: outputs 'b'
 */
void build_NAND_test(Seq_T stream)
{
    append(stream, loadval(r3, 0));
    append(stream, loadval(r2, 0));
    append(stream, bitwise_NAND(r1, r2, r3)); 
    append(stream, loadval(r4, 98));
    append(stream, loadval(r5, 97));
    append(stream, conditional_move(r5, r4, r1));
    append(stream, output(r5)); 
    append(stream, halt());
}

/*
 * build input test
 * Expected Effects: waits for user to input a character
 *                   outputs whatever character was inputted
 */
void build_input_test(Seq_T stream)
{
    append(stream, input(r1));
    append(stream, output(r1)); 
    append(stream, halt());
}

/*
 * build input test2
 * Expected Effects: waits for user to input a character
 *                   outputs the character corresponding to the ASCII value of 
 *                   the character that was inputted + 50
 */
void build_input_test2(Seq_T stream)
{
    append(stream, input(r3));
    append(stream, loadval(r2, 50));
    append(stream, add(r1, r2, r3));
    append(stream, output(r1)); 
    append(stream, halt());
}

/*
 * build map segment test
 * Expected Effects: maps a segment in main memory
 */
void build_map_segment_test(Seq_T stream)
{
    append(stream, loadval(r2, 50));
    append(stream, map_segment(r1, r2));
    append(stream, halt());
}

/*
 * build unmap segment test
 * Expected Effects: maps then unmaps a segment in main memory
 */
void build_unmap_segment_test(Seq_T stream)
{
    append(stream, loadval(r2, 50));
    append(stream, map_segment(r1, r2));
    append(stream, unmap_segment(r1));
    append(stream, halt());
}

/*
 * build segmented store test
 * Expected Effects: maps two segments and stores value 20 in one index of 
 *                   one of the segments
 */
void build_segmented_store_test(Seq_T stream)
{
    append(stream, loadval(r2, 50));
    append(stream, map_segment(r1, r2));
    append(stream, loadval(r2, 30));
    append(stream, map_segment(r4, r2));
    append(stream, loadval(r3, 20));
    append(stream, segmented_store(r1, r3, r3));
    append(stream, halt());
}

/*
 * build segmented load test
 * Expected Effects: outputs 'a'
 */
void build_segmented_load_test(Seq_T stream)
{
    append(stream, loadval(r2, 50));
    append(stream, map_segment(r1, r2));
    append(stream, loadval(r2, 30));
    append(stream, map_segment(r4, r2));
    append(stream, loadval(r4, 97));
    append(stream, segmented_store(r1, r3, r4));
    append(stream, segmented_load(r5, r1, r3)); 
    append(stream, output(r5)); 
    append(stream, halt());
}

/*
 * build load program test
 * Expected Effects: outputs 'a'
 */
void build_load_program_test(Seq_T stream)
{
    append(stream, loadval(r2, 3));
    append(stream, map_segment(r1, r2));
    append(stream, loadval(r4, 16));
    append(stream, segmented_load(r6, r0, r4)); 
    append(stream, loadval(r7, 0));
    append(stream, segmented_store(r1, r7, r6)); 
    
    append(stream, loadval(r4, 17));
    append(stream, segmented_load(r6, r0, r4)); 
    append(stream, loadval(r7, 1));
    append(stream, segmented_store(r1, r7, r6)); 
    
    append(stream, loadval(r4, 18));
    append(stream, segmented_load(r6, r0, r4)); 
    append(stream, loadval(r7, 2));
    append(stream, segmented_store(r1, r7, r6)); 
    
    append(stream, loadval(r7, 0));
    append(stream, load_program(r1, r7));   
                                            
    append(stream, loadval(r4, 97));
    append(stream, output(r4)); 
    append(stream, halt());
}




