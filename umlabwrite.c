/*
 * umlabwrite.c
 * Isabelle Lai (ilai01) Andrea Foo (afoo01)
 * Comp40, HW6
 * Summary: builds um binary files for unit testing of um program
 * Due: April 13, 2020
 */
 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

/* Functions to build tests */
extern void build_halt_test(Seq_T instructions);
extern void build_verbose_halt_test(Seq_T instructions);
extern void build_output_test(Seq_T instructions);
extern void build_add_test(Seq_T instructions);
extern void build_multiply_test(Seq_T instructions);
extern void build_multiply_test_edge1(Seq_T instructions);
extern void build_divide_test(Seq_T instructions);
extern void build_divide_test_edge2(Seq_T instructions);
extern void build_cmov_test(Seq_T instructions);
extern void build_cmov_test_edge1(Seq_T instructions);
extern void build_NAND_test(Seq_T instructions);
extern void build_input_test(Seq_T instructions);
extern void build_input_test2(Seq_T instructions);
extern void build_map_segment_test(Seq_T instructions);
extern void build_unmap_segment_test(Seq_T instructions);
extern void build_segmented_store_test(Seq_T instructions);
extern void build_segmented_load_test(Seq_T instructions);
extern void build_load_program_test(Seq_T stream);

/* The array `tests` contains all unit tests for the lab. */
static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "", build_halt_test },
        { "halt-verbose", NULL, "", build_verbose_halt_test },
        { "output", NULL, "6", build_output_test },
        { "add", NULL, "6", build_add_test },
        { "multiply", NULL, "n", build_multiply_test },
        { "multiply-edge1", NULL, "", build_multiply_test_edge1 },
        { "divide", NULL, "d", build_divide_test },
        { "divide-edge2", NULL, "", build_divide_test_edge2 },
        { "cmov", NULL, "t", build_cmov_test },
        { "cmov-edge1", NULL, "f", build_cmov_test_edge1 },
        { "bitwise-nand", NULL, "b", build_NAND_test },
        { "input", "i", "i", build_input_test },
        { "input-2", "0", "b", build_input_test2 },
        { "map-segment", NULL, "", build_map_segment_test },
        { "unmap-segment", NULL, "", build_unmap_segment_test },
        { "segmented-store", NULL, "", build_segmented_store_test },
        { "segmented-load", NULL, "a", build_segmented_load_test },
        { "load-program", NULL, "a", build_load_program_test }
};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
