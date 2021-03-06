
/******************************************************************************
* MODULE     : mac_utilities.mm
* DESCRIPTION: Utilities for Mac OS X
* COPYRIGHT  : (C) 2007  Massimiliano Gubinelli
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include <Cocoa/Cocoa.h>

void mac_fix_paths()
{
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  /* add appropriate TEXMACS_PATH to the current environment */
#if 0
  setenv("TEXMACS_PATH",
         [[[[NSBundle mainBundle] resourcePath] 
            stringByAppendingPathComponent:@"share/TeXmacs"] 
            cStringUsingEncoding:NSUTF8StringEncoding],
         1);
#endif
  /* add TeX directory */
  /* FIXME: make this user-defined */
  setenv("PATH",
         [[[NSString stringWithCString:getenv("PATH")] 
            stringByAppendingString:@":/usr/texbin"]
            cStringUsingEncoding:NSUTF8StringEncoding],
         1); 
  setenv("GUILE_LOAD_PATH","/opt/local/share/guile/1.8",1);
  system("printenv");
  [pool release];  
}
