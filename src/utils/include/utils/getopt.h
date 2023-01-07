#ifndef UTILS_GETOPT_H_
#define UTILS_GETOPT_H_

#include "utils/lib.h"

#include "utils/getopt.h"

#include <stdint.h>
#include <string>
#include <ostream>

#ifdef __cplusplus
extern "C" {
#endif

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

    struct getopt_t {
        int   argc;
        char** argv;

        int opterr;  /* printf of errors*/
        int optind;  /* index into parent argv vector */
        int optopt;  /* character checked for validity */
        int optreset; /* reset getopt */
        char* optarg; /* argument associated with option */
        const char* ostr;
        int BADCH;
        int BADARG;
        char* EMSG;
        char EMSGCHAR;
    };

    UTILS_API int getopt_init(struct getopt_t* opt, int argc, char* argv[], const char* ostr);
    UTILS_API int getopt_parse(struct getopt_t* opt);

#ifdef __cplusplus
} extern "C"
	
namespace utils {
    struct UTILS_API GetOpt : ::getopt_t {
        GetOpt(int argc, char* argv[], const char* ostr) { getopt_init(this, argc, argv, ostr); }
        int parse() { return getopt_parse(this); }
    };
}
#endif

#endif
