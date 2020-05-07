/*
 * main.c
 * Andrea Foo (afoo01) and Isabelle Lai (ilai01)
 * Comp40, HW6 um
 * Due: April 13, 2020
 * Purpose: Implmentation driver for the main UM 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>
#include "um.h"


/* main to run um program */
int main(int argc, char *argv[])
{   
    /* Check to make sure correct number of arguments */ 
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }
    /* Calls run machine function from um.c */
    run_machine(argv[1]);
    return EXIT_SUCCESS; 
}