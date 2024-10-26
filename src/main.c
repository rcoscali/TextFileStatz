/** @FileInfo main.c
 *
 * Main command implementation
 *
 * Copyright 2024 Rémi COHEN SCALI / EFREI
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

void
print_usage(char* cmd)
{
    fprintf(stdout, "%s: usage: textfilestatz [-h|--help],[-v|--verbose]\n", cmd);
}

int main(int argc, char *argv[]) {
    char c = -1;
    int verbose_flag = FALSE;
    int help_flag = FALSE;

    char *cmd = strrchr(argv[0], '\\') ? strdup(strrchr(argv[0], '\\') + 1) : argv[0];

    struct option long_options[] = {
            {"verbose",  no_argument,  VERBOSE_FLAG, VERBOSE_VALUE},
            {"help",     no_argument,  HELP_FLAG, HELP_VALUE},
            {0, 0, 0, 0}
    };

    do
    {
#ifdef DONT_USE_FLAG
        if ((c = getopt_long(argc, argv, SHORT_OPTION_STRING, long_options, NULL)) != -1)
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
#else
        c = getopt_long(argc, argv, SHORT_OPTION_STRING, long_options, NULL);
        if (c == '?')
            printf("Argument non-option : '%c' '%c'\n", optopt, c);
#endif
    }
    while (c != -1);

    if (help_flag || (!help_flag && !verbose_flag))
        print_usage(cmd);
    if (verbose_flag)
        fprintf(stderr, "Mode verbeux activé\n");

    if (c == '?')
        fprintf(stderr, "Option inconnue : %c\n", optopt);

    /* Traitement des arguments restants (si nécessaire) */
    for (int index = optind; index < argc; index++) {
        fprintf(stdout, "Argument non-option : %s\n", argv[index]);
    }

    return 0;
}
