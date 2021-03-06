
/******************************************************************************
* MODULE     : tm_configure.gen.in or tm_configure.gen.h
* DESCRIPTION: Defines system dependent macros (using autoconf)
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef TM_CONFIGURE_H
#define TM_CONFIGURE_H
#include "config.h"

@CONFIG_STD_SETENV@

#define TEXMACS_VERSION "@DEVEL_VERSION@"
#define TEXMACS_SOURCES "@tmorig@"
#define OS_@CONFIG_OS@
#define GUILE_@CONFIG_GUILE_SERIAL@
#define WORD_LENGTH @CONFIG_WORD_LENGTH@
#define WORD_LENGTH_INC @CONFIG_WORD_LENGTH_INC@
#define WORD_MASK @CONFIG_WORD_MASK@
#define MAX_FAST @CONFIG_MAX_FAST@

#define TEXMACS_TGZ "@tmtgz@"
#define TEXMACS_RPM "@tmrpm@"
#define TEXMACS_STGZ "@tmstgz@"
#define TEXMACS_SRPM "@tmsrpm@"
#define TEXMACS_WIN "@tmwin@"

#endif // defined TM_CONFIGURE_H
