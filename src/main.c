/** @FileInfo main.c
 *
 * Main command implementation
 *
 * Copyright ©2024 Rémi COHEN SCALI / EFREI
 *
 * MIT License
 * -----------
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the “Software”), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include "libfcm.h"
#include "libstnrgen.h"

#define DONT_USE_FLAG

#define SHORT_OPTION_STRING ":hv"
#define EXIT_FAILURE 1

#ifdef DONT_USE_FLAG
/* Flag ptr */
# define VERBOSE_FLAG NULL
# define HELP_FLAG NULL
# define TEST_FLAG NULL
/* Returned value */
# define VERBOSE_VALUE 'v'
# define HELP_VALUE 'h'
# define TEST_VALUE 't'
#else
/* Flag ptr */
# define VERBOSE_FLAG &verbose_flag
# define HELP_FLAG &help_flag
# define TEST_FLAG &test_flag
// Value set in flag
# define VERBOSE_VALUE TRUE
# define HELP_VALUE TRUE
# define TEST_VALUE TRUE
#endif

#ifdef TRUE
# undef TRUE
#endif
#define TRUE (10 == 10)
#ifdef FALSE
# undef FALSE
#endif
#define FALSE (10 == 11)

/**
 * print_usage
 *
 * Function for displaying usage string for quick help
 *
 * @param cmd       the command string
 */
void
print_usage(char* cmd)
{
    fprintf(stdout, "%s: usage: textfilestatz [-h|--help],[-v|--verbose]\n", cmd);
}

/**
 * print_help
 *
 * Function for displaying basic help file
 *
 * @param cmd       the command string
 */
void
print_help(char* blank_cmd)
{
    fprintf(stderr, "%s                                                                       \n", blank_cmd);
    fprintf(stderr, "%s  Short description:                                                   \n", blank_cmd);
    fprintf(stderr, "%s     This command allows to compute statistics on text files in term of\n", blank_cmd);
    fprintf(stderr, "%s     characters and words (optionally including their probabilities).  \n", blank_cmd);
    fprintf(stderr, "%s     In order to use it you can use several options.                   \n", blank_cmd);
    fprintf(stderr, "%s                                                                       \n", blank_cmd);
    fprintf(stderr, "%s  Options:                                                             \n", blank_cmd);
    fprintf(stderr, "%s     --verbose|-v          increase command verbosity.                 \n", blank_cmd);
    fprintf(stderr, "%s     --help|-h             display this short help.                    \n", blank_cmd);
    fprintf(stderr, "%s     --input|-i <textfile> provide a text file to process. By default  \n", blank_cmd);
    fprintf(stderr, "%s                           if no file is provided, the standard input  \n", blank_cmd);
    fprintf(stderr, "%s                           is used.                                    \n", blank_cmd);
    fprintf(stderr, "%s     --count|-c <char>     include this char to process.               \n", blank_cmd);
    fprintf(stderr, "%s     --word|-w <word>      include this word to process.               \n", blank_cmd);
    fprintf(stderr, "%s     --proba|-p            also compute probabilities of occurence for \n", blank_cmd);
    fprintf(stderr, "%s                           each cjaracter and word.                    \n", blank_cmd);
    fprintf(stderr, "%s     --report|-r <file>    generate report in the provided file. By    \n", blank_cmd);
    fprintf(stderr, "%s                           default if no report file is provided, the  \n", blank_cmd);
    fprintf(stderr, "%s                           standard output is used.                    \n", blank_cmd);
    fprintf(stderr, "%s                                                                       \n", blank_cmd);
    fprintf(stderr, "Copyright ⓒ2024 Rémi COHEN SCALI / EFREI                               %s\n", blank_cmd);
}

/**
 * main
 *
 * Main command entry point function
 *
 * @param argc      number of option arguments on command line
 * @param argv      array of option arguments on command line
 *
 * @return 0 if success, none 0 otherwise
 */
int
main(int argc, char *argv[])
{
    /*
     * Flags and variable for handling command line options
     */
    char c = -1;
    int verbose_flag = FALSE;
    int help_flag = FALSE;

    /*
     * Command to display for info/error
     */
    char* cmd = strrchr(argv[0], '\\') ? strdup(strrchr(argv[0], '\\') + 1) : argv[0];
    char* tmp;
    if ((tmp = strrchr(cmd, '.')) != NULL)
        *tmp = 0;

    /*
     * Blanked command used for indenting outputs
     */
    char* blank_cmd = strdup(cmd);
    for (tmp = blank_cmd; tmp < blank_cmd + strlen(cmd); tmp++)
        *tmp = ' ';

    /*
     * Long options defined for getopt_long processing
     */
    struct option long_options[] = {
            /* Verbose (-v|--verbose) */
            {"verbose",  no_argument,  VERBOSE_FLAG, VERBOSE_VALUE},
            /* Help (-h|--help) */
            {"help",     no_argument,  HELP_FLAG, HELP_VALUE},
            /* Null value for terminating table */
            {0, 0, 0, 0}
    };

    /*
     * Main loop for options processing
     */
    do
    {
#ifdef DONT_USE_FLAG
        /* Call getopt_long for arguments parsing */
        if ((c = getopt_long(argc, argv, SHORT_OPTION_STRING, long_options, NULL)) != -1)
            /* Then according to option found, do whatever is needed */
            switch(c) {
                case 'v':
                    verbose_flag = TRUE;
                    break;
                case 'h':
                    help_flag = TRUE;
                    break;
                default:
                    printf("%s: error: option '%c' doesn't exists !\n", optopt);
                    break;
            }
#else /* !DONT_USE_FLAG */
        c = getopt_long(argc, argv, SHORT_OPTION_STRING, long_options, NULL);
        if (c == '?')
            printf("%s: error: option '%c' doesn't exist\n", optopt);
#endif /* DONT_USE_FLAG */
    }
    while (c != -1); /* Until getopt_long do not find any other option argument */

    /* If help requested or no option provided */
    if (help_flag || (!help_flag && !verbose_flag))
    {
        /* Display an error message */
        if (!help_flag)
            fprintf(stderr, "%s: error: no options provided !\n", cmd);
        /* Print the usage short message */
        print_usage(cmd);
        /* Then display the help */
        if (help_flag)
            print_help(blank_cmd);
        /* Exit: if help was requested, success exit */
        exit(help_flag ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    /* If verbose was requsted, notify user */
    if (verbose_flag)
        fprintf(stderr, "%s: info: verbose mode requested\n", cmd);

    /* If an error was detected by getopt_long */
    if (c == '?')
    {
        /* Display an error message and exit failure */
        fprintf(stderr, "%s: error: unknown option '%c'\n", cmd, optopt);
        exit(EXIT_FAILURE);
    }

    /* Traitement des arguments restants (si nécessaire) */
    for (int index = optind; index < argc; index++)
    {
        if (verbose_flag)
            fprintf(stdout, "%s: info: tail argument : '%s'\n", cmd, argv[index]);
        else
            fprintf(stdout, "tail argument : '%s'\n", argv[index]);
    }

    return 0;
}
