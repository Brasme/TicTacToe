#include "utils/getopt.h"


#include <string.h>
#include <stdio.h>

extern "C" {
    /* Source: Linux source code : https://codebrowser.dev/glibc/glibc/posix/getopt.c.html
    *  Modifications: Bent Gramdal - Using a structure instead of globals form internal attributes
    */
    // Getopt for GNU.
    // Copyright (C) 1987-2022 Free Software Foundation, Inc.
    // This file is part of the GNU C Library and is also part of gnulib.
    // Patches to this file should be submitted to both projects.
    // The GNU C Library is free software; you can redistribute it and/or
    // modify it under the terms of the GNU Lesser General Public
    // License as published by the Free Software Foundation; either
    // version 2.1 of the License, or (at your option) any later version.
    // The GNU C Library is distributed in the hope that it will be useful,
    // but WITHOUT ANY WARRANTY; without even the implied warranty of
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    // Lesser General Public License for more details.
    // You should have received a copy of the GNU Lesser General Public
    // License along with the GNU C Library; if not, see
    // <https://www.gnu.org/licenses/>.  */

    int getopt_init(struct getopt_t* opt, int argc, char* argv[], const char* ostr) {
        if (!opt || argc <= 0 || !argv)
            return -1;
        opt->argc = argc;
        opt->argv = argv;
        opt->opterr = 1;
        opt->optind = 1;
        opt->optopt = 0;
        opt->optreset = 0;
        opt->optarg = NULL;
        opt->ostr = ostr;
        opt->BADCH = (int)'?';
        opt->BADARG = (int)':';
        opt->EMSG = &opt->EMSGCHAR;
        opt->EMSGCHAR = '\0';
        return 0;
    }

    int getopt_parse(struct getopt_t* opt)
    {
        if (!opt)
            return -1;
        static char* place = opt->EMSG;   /* option letter processing */
        const char* oli;                  /* option letter list index */

        if (opt->optreset || !*place) {              /* update scanning pointer */
            opt->optreset = 0;
            if (opt->optind >= opt->argc || *(place = opt->argv[opt->optind]) != '-') {
                place = opt->EMSG;
                return (-1);
            }
            if (place[1] && *++place == '-') {      /* found "--" */
                ++(opt->optind);
                place = opt->EMSG;
                return (-1);
            }
        }                                       /* option letter okay? */
        if ((opt->optopt = (int)*place++) == (int)':' ||
            !(oli = strchr(opt->ostr, opt->optopt))) { /* if the user didn't specify '-' as an option, assume it means -1. */
            if (opt->optopt == (int)'-')
                return (-1);
            if (!*place)
                ++(opt->optind);
            if (opt->opterr && *opt->ostr != ':')
                (void)printf("illegal option -- %c\n", opt->optopt);
            return (opt->BADCH);
        }
        if (*++oli != ':') {                    /* don't need argument */
            opt->optarg = NULL;
            if (!*place)
                ++(opt->optind);
        }
        else {                                  /* need an argument */
            if (*place)                     /* no white space */
                opt->optarg = place;
            else if (opt->argc <= ++(opt->optind)) {   /* no arg */
                place = opt->EMSG;
                if (*(opt->ostr) == ':')
                    return (opt->BADARG);
                if (opt->opterr)
                    (void)printf("option requires an argument -- %c\n", opt->optopt);
                return (opt->BADCH);
            }
            else /* white space */
                opt->optarg = opt->argv[opt->optind];
            place = opt->EMSG;
            ++(opt->optind);
        }
        return (opt->optopt); /* dump back option letter */
    }

    int parse_args_getopt_example_c(int argc, char* argv[])
    {
        struct getopt_t opt;
        // put ':' at the starting of the string so compiler can distinguish between '?' and ':'
        if (getopt_init(&opt, argc, argv, ":if:lrx") != 0)
            return -1; // Failed to init

        int option;
        while ((option = getopt_parse(&opt)) != -1) { //get option from the getopt() method
            switch (option) {
                //For option i, r, l, print that these are options
            case 'i':
            case 'l':
            case 'r':
                printf("Given Option: %c\n", option);
                break;
            case 'f': //here f is used for some file name
                printf("Given File: %s\n", opt.optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?': //used for some unknown options
                printf("unknown option: %c\n", opt.optopt);
                break;
            }
        }
        for (; opt.optind < argc; opt.optind++) { //when some extra arguments are passed
            printf("Given extra arguments: %s\n", argv[opt.optind]);
        }
        return 0;
    }

    int parse_args_getopt_example_cpp(int argc, char* argv[])
    {
        utils::GetOpt opt(argc, argv, ":if:lrx");

        int option;
        while ((option = opt.parse()) != -1) { //get option from the getopt() method
            switch (option) {
                //For option i, r, l, print that these are options
            case 'i':
            case 'l':
            case 'r':
                printf("Given Option: %c\n", option);
                break;
            case 'f': //here f is used for some file name
                printf("Given File: %s\n", opt.optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?': //used for some unknown options
                printf("unknown option: %c\n", opt.optopt);
                break;
            }
        }
        for (; opt.optind < argc; opt.optind++) { //when some extra arguments are passed
            printf("Given extra arguments: %s\n", argv[opt.optind]);
        }
        return 0;
    }

}
