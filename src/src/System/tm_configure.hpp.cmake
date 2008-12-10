
/******************************************************************************
* MODULE     : tm_configure.gen.in or tm_configure.gen.h
* DESCRIPTION: Defines system dependent macros (using autoconf)
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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
