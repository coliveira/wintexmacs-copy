
/******************************************************************************
* MODULE     : printer.cpp
* DESCRIPTION: Renderer for printing post-script graphics
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "printer.hpp"
#include "Metafont/tex_files.hpp"
#include "Freetype/tt_file.hpp"
#include "file.hpp"
#include "image_files.hpp"
#include "analyze.hpp"
#include "iterator.hpp"
#include "merge_sort.hpp"
#include "scheme.hpp"

string PS_CLIP_PUSH ("gsave");
string PS_CLIP_POP ("grestore");
string PS_CLIP ("cl");
string PS_LINE ("ln");
string PS_FILL ("fl");
string PS_ARC ("ac");
string PS_FILL_ARC ("fac");
string PS_STROKE ("st");
string PS_POL_START ("sp");
string PS_POL_NEXT ("np");
string PS_POL_END ("ep");
string PS1 ("u");
string PS2 ("z");

/******************************************************************************
* constructors and destructors
******************************************************************************/

printer_rep::printer_rep (
  url ps_file_name2, int dpi2, int nr_pages2,
  string page_type2, bool landscape2, double paper_w2, double paper_h2):
    ps_file_name (ps_file_name2), dpi (dpi2),
    nr_pages (nr_pages2), page_type (page_type2),
    landscape (landscape2), paper_w (paper_w2), paper_h (paper_h2),
    linelen (0), fg (-1), bg (-1), ncols (0),
    lw (-1), nwidths (0), cfn (""), nfonts (0),
    xpos (0), ypos (0), tex_flag (false),
    defs ("?"), tex_chars ("?"), tex_width ("?"),
    tex_fonts ("?"), tex_font_chars (array<int>(0))    
{
  type_1    = get_font_type () > 0;

  string tex_pro, special_pro, color_pro, texps_pro;
  load_string ("$TEXMACS_PATH/misc/convert/tex.pro", tex_pro, true);
  load_string ("$TEXMACS_PATH/misc/convert/special.pro", special_pro, true);
  load_string ("$TEXMACS_PATH/misc/convert/color.pro", color_pro, true);
  load_string ("$TEXMACS_PATH/misc/convert/texps.pro", texps_pro, true);
  
  prologue   << "%!PS-Adobe-2.0\n"
	     << "%%Creator: TeXmacs-" TEXMACS_VERSION "\n"
	     << "%%Title: " << as_string (ps_file_name) << "\n"
	     << "%%Pages: " << as_string (nr_pages) << "\n"
	     << "%%PageOrder: Ascend\n";
  if (page_type != "user")
    prologue << "%%DocumentPaperSizes: " << page_type << "\n";
  if (landscape)
    prologue << "%%BoundingBox: 0 0 "
	     << as_string ((int) (28.36*paper_h+ 0.5)) << " "
	     << as_string ((int) (28.36*paper_w+ 0.5)) << "\n"
	     << "%%Orientation: Landscape\n";
  else
    prologue << "%%BoundingBox: 0 0 "
	     << as_string ((int) (28.36*paper_w+ 0.5)) << " "
	     << as_string ((int) (28.36*paper_h+ 0.5)) << "\n";
  prologue   << "%%EndComments\n\n"
	     << tex_pro << "\n"
	     << special_pro << "\n"
	     << texps_pro << "\n"
	     << "TeXDict begin\n"
	     << as_string ((int) (1864680.0*paper_w+ 0.5)) << " "
	     << as_string ((int) (1864680.0*paper_h+ 0.5)) << " 1000 "
	     << as_string (dpi) << " " << as_string (dpi)
	     << " (TeXmacs) @start\n";

  define (PS_CLIP, string ("/pt4 X /pt3 X /pt2 X /pt1 X\n") *
	  string ("newpath pt1 pt2 moveto pt3 pt2 lineto ") *
	  string ("pt3 pt4 lineto pt1 pt4 lineto pt1 pt2 lineto clip"));
  define (PS_LINE, string ("/pt4 X /pt3 X /pt2 X /pt1 X\n") *
	  string ("newpath pt1 pt2 moveto pt3 pt4 lineto stroke"));
  define (PS_FILL, string ("/pt4 X /pt3 X /pt2 X /pt1 X\n") *
	  string ("newpath pt1 pt2 moveto pt3 pt2 lineto ") *
	  string ("pt3 pt4 lineto pt1 pt4 lineto pt1 pt2 eofill stroke"));
  define (PS_ARC, string ("/a2 X /a1 X /r2 X /r1 X /pt2 X /pt1 X\n") *
	  string ("newpath pt1 pt2 r1 r2 a1 a2 ellipse stroke"));
  define (PS_FILL_ARC, string ("/a2 X /a1 X /r2 X /r1 X /pt2 X /pt1 X\n") *
	  string ("newpath pt1 pt2 r1 r2 a1 a2 ellipse eofill stroke"));
  define (PS_STROKE, string ("stroke"));
  define (PS_POL_START, string ("/pt2 X /pt1 X\n") *
	  string ("newpath pt1 pt2 moveto"));
  define (PS_POL_NEXT, string ("/pt2 X /pt1 X\n") *
	  string ("pt1 pt2 lineto"));
  define (PS_POL_END, string ("closepath eofill"));
  define (PS1, string ("gsave"));
  define (PS2, string ("1 -1 scale show grestore"));

  cur_page= 0;
  next_page ();
}

printer_rep::~printer_rep () {
  next_page ();
  body << "\n%%Trailer\n"
       << "end\n"
       << "userdict /end-hook known{end-hook} if\n"
       << "%%EOF\n";

  generate_tex_fonts ();
  prologue << "end\n"
           << "%%EndProlog\n\n"
	   << "%%BeginSetup\n"
	   << "%%Feature: *Resolution " << as_string (dpi) << "dpi\n"
	   << "TeXDict begin\n";
  if (page_type != "user") {
    prologue << "%%BeginPaperSize: " << page_type << "\n";
    prologue << page_type << "\n";
    prologue << "%%EndPaperSize\n";
  }
  if (landscape)
    prologue << "@landscape\n";
  prologue << "%%EndSetup\n";

  string ps_text= prologue * "\n" * body;
  save_string (ps_file_name, ps_text);
}

bool
printer_rep::is_printer () {
  return true;
}

void
printer_rep::next_page () {
  if (cur_page > 0) print ("eop\n");
  if (cur_page >= nr_pages) return;
  cur_page++;
  body << "\n%%Page: " << as_string (cur_page) << " "
       << as_string (cur_page) << "\n"
       << as_string (cur_page) << " "
       << as_string (cur_page-1) << " bop\n";

  set_clipping (0, (int) (-(dpi*PIXEL*paper_h)/2.54),
		(int) ((dpi*PIXEL*paper_w)/2.54), 0);

  fg  = -1;
  bg  = -1;
  lw  = -1;
  cfn = "";
  xpos= 0;
  ypos= 0;
}

/******************************************************************************
* subroutines for printing
******************************************************************************/

void
printer_rep::define (string s, string defn) {
  if (defs->contains (s)) return;
  defs (defn)= s;
  prologue << "/" << s << " {" << defn << "} N\n";
}

void
printer_rep::sep () {
  if ((N(body) > 0) &&
      (body [N(body)-1] != ')') &&
      (body [N(body)-1] != '\n')) {
    body << " ";
    linelen++;
    tex_flag= false;
  }
}

void
printer_rep::cr () {
  body << "\n";
  linelen= 0;
  tex_flag= false;
}

void
printer_rep::print (string s) {
  if (N(s)==0) return;
  if ((linelen>0) && (linelen+N(s)>79)) {
    body << "\n";
    linelen= 0;
    tex_flag= false;
  }
  else if (s[0]!='(') sep ();
  if (tex_flag && (s[0]=='(')) {
    body->resize (N(body)-2);
    linelen -= 2;
    s= s (1,N(s));
  }
  body << s;
  linelen += N(s);
  tex_flag= false;
}

void
printer_rep::print (SI x, SI y) {
  x += ox; y += oy;
  if (x>=0) x= x/PIXEL; else x= (x-PIXEL+1)/PIXEL;
  if (y>=0) y= y/PIXEL; else y= (y-PIXEL+1)/PIXEL;
  print (as_string (x-dpi));
  print (as_string (-y-dpi));
}

void
printer_rep::move_to (SI x, SI y) {
  x += ox; y += oy;
  if (x>=0) x= x/PIXEL; else x= (x-PIXEL+1)/PIXEL;
  if (y>=0) y= y/PIXEL; else y= (y-PIXEL+1)/PIXEL;
  if (tex_flag && (xpos==x) && (ypos==y)) return;
  if (tex_flag && (ypos==y)) {
    body->resize (N(body)-1);
    linelen -= 1;
    tex_flag= false;

    int diff= x-xpos;
    if ((diff>=-4) && (diff<=4)) print (string ((char) ('p'+diff)));
    else {
      print (as_string (diff));
      print ("b");
    }
    xpos= x;
    return;
  }
  xpos= x; ypos= y;
  print (as_string (x-dpi));
  print (as_string (-y-dpi));
  print ("a");
}

void
printer_rep::select_color (color c) {
  int r, g, b;
  get_rgb_color (c, r, g, b);
  r= 10000+ ((r*1000)/255);
  g= 10000+ ((g*1000)/255);
  b= 10000+ ((b*1000)/255);
  string rr= as_string (r); rr= rr(1,2) * "." * rr(2,5);
  string gg= as_string (g); gg= gg(1,2) * "." * gg(2,5);
  string bb= as_string (b); bb= bb(1,2) * "." * bb(2,5);
  string s = rr * " " * gg * " " * bb * " setrgbcolor";
  if (!defs->contains (s)) {
    define ("C" * as_string (ncols), s);
    ncols++;
  }
  print (defs[s]);
}

void
printer_rep::select_line_width (SI w) {
  w= w/PIXEL; if (w<1) w=1;
  string s = as_string (w) * " setlinewidth";
  if (!defs->contains (s)) {
    define ("W" * as_string (nwidths), s);
    nwidths++;
  }
  print (defs[s]);
}

/******************************************************************************
* subroutines for fonts
******************************************************************************/

static string
prepare_text (string s) {
  int i;
  string r;
  for (i=0; i<N(s); i++) {
    int c= ((unsigned char) s[i]);
    if ((s[i]=='(') || (s[i]==')') || (s[i]=='\\'))
      r << '\\' << s[i];
    else if ((c <= 32) || (c >= 128)) {
      r << '\\';
      r << ('0' + (c >> 6));
      r << ('0' + ((c >> 3) & 7));
      r << ('0' + (c & 7));
    }
    else r << s[i];
  }
  return r;
}

void
printer_rep::select_tex_font (string name) {
  if (cfn==name) return;
  cfn= name;
  print (tex_fonts [name]);
}

/******************************************************************************
* make tex characters and fonts
******************************************************************************/

static const char* hex_string= "0123456789ABCDEF";

void
printer_rep::make_tex_char (string name, unsigned char c, glyph gl) {
  // cout << "Make char " << (int) c << " of " << name << "\n";
  string char_name (name * "-" * as_string ((int) c));
  if (tex_chars->contains (char_name)) return;
  if (!tex_fonts->contains (name)) {
    tex_fonts (name)= "F" * as_string (nfonts);
    tex_font_chars (name)= array<int> (0);
    nfonts++;
  }
  tex_font_chars (name) << ((int) c);

  string hex_code;
  int i, j, count=0, cur= 0;
  for (j=0; j < gl->height; j++)
    for (i=0; i < ((gl->width+7) & (-8)); i++) {
      cur= cur << 1;
      if ((i<gl->width) && (gl->get_x(i,j)>0)) cur++;
      count++;
      if (count==4) {
	hex_code << hex_string[cur];
	cur  = 0;
	count= 0;
      }
    }

  int d1= gl->width;
  int d2= gl->height;
  int d3= 130+ gl->xoff;
  int d4= 126+ gl->yoff;
  int d5= gl->lwidth;
  if ((d1<256) && (d2<256) && (d3<256) && (d4<256) && (d5<256)) {
    hex_code << as_hexadecimal (d1, 2) << as_hexadecimal (d2, 2)
	     << as_hexadecimal (d3, 2) << as_hexadecimal (d4, 2)
	     << as_hexadecimal (d5, 2);
    hex_code= "<" * hex_code * ">";
  }
  else {
    hex_code= "[<" * hex_code * ">";
    hex_code << as_string (d1) << " " << as_string (d2) << " "
	     << as_string (d3) << " " << as_string (d4) << " "
	     << as_string (d5) << " ";
  }

  tex_chars (char_name)= hex_code;
  tex_width (char_name)= as_string (d5);
}

static string
find_ps_font_name (string name, string s) {
  int i, n= N(s);
  for (i=0; i<n; i++) {
    if (test (s, i, "/FontName /")) {
      i += 11;
      int start= i;
      while (i<n && s[i] != ' ') i++;
      return s (start, i);
    }
    while (i<n && s[i] != '\12' && s[i] != '\15') i++;
  }
  return name;
}

void
printer_rep::generate_tex_fonts () {
  hashset<string> done;
  iterator<string> it= iterate (tex_fonts);
  while (it->busy ()) {
    string fn_name= it->next ();
    array<int> a= tex_font_chars [fn_name];
    merge_sort (a);

    int i, d, l;
    string name = tex_fonts [fn_name], ttf;
    int    pos  = search_forwards (".", fn_name);
    string root = (pos==-1? fn_name: fn_name (0, pos));
#ifndef OS_WIN32 // we need pfbtopfa
    if ((pos!=-1) && ends (fn_name, "tt")) {
      int pos2= search_backwards (":", fn_name);
      root= fn_name (0, pos2);
      url u= tt_font_find (root);
      if (suffix (u) == "pfb") {
	url v= url_temp (".pfa");
	system ("pfb2pfa", u, v);
	(void) load_string (v, ttf, true);
	remove (v);
      }
    }
#endif

    if (ttf != "") {
      string ttf_name= find_ps_font_name (root, ttf);
      if (!done->contains (root)) {
	prologue << "%%BeginFont: " << root << "\n";
	prologue << ttf;
	prologue << "\n%%EndFont\n";
	done->insert (root);
      }

      array<string> cum;
      cum << "{}" * as_string (N(a));
      for (i=0; i<N(a); i++) {
	string w= tex_width [fn_name * "-" * as_string (a[i])];
	d= (i==0? a[0]: (a[i]-a[i-1]-1));
	if (d>0) cum << as_string (d) * "[";
	cum << w * " ";
      }
      d= 255-a[i-1];
      if (d>0) cum << as_string (d) * "[";

      int szpos = pos-1;
      while ((szpos>0) && is_numeric (fn_name[szpos-1])) szpos--;
      double sz = as_double (fn_name (szpos, pos));
      double dpi= as_double (fn_name (pos+1, N(fn_name)-2));
      string mag= as_string (83.022 * (sz/10.0) * (dpi/600.0));

      string fdef;
      for (i=N(cum)-1; i>=0; i--) fdef << cum[i];
      fdef= "/" * name * " " * fdef * " " * mag * " /" * ttf_name * " rf";
      for (i=0, l=0; i<N(fdef); i++, l++)
	if ((l<70) || (fdef[i]!=' ')) prologue << fdef[i];
	else { prologue << '\n'; l=-1; }
      prologue << "\n";
    }
    else {
      prologue << "/" << tex_fonts [fn_name]
	       << " " << as_string (N(a))
	       << " " << as_string (a[N(a)-1]+1) << " df\n";
      for (i=0; i<N(a); i++) {
	int end;
	string hex_code= tex_chars [fn_name * "-" * as_string (a[i])];
	for (end=1; end < N(hex_code); end++)
	  if (hex_code[end-1]=='>') break;
	string after= hex_code (end, N(hex_code));
	if ((i>0) && (a[i]==(a[i-1]+1))) after << "I";
	else after << as_string (a[i]) << " D";
	if (i==(N(a)-1)) after << " E";
	hex_code= hex_code (0, end);
      
	int j, l, n= N(hex_code);
	for (j=0; j<n; j+=79) {
	  if (n < (j+79)) prologue << hex_code (j, n);
	  else prologue << hex_code (j, j+79) << "\n";
	}
	l= 79-(n%79);
	if (l<N(after)) prologue << "\n";
	prologue << after << "\n";
      }
    }
  }
}

/******************************************************************************
* Clipping
******************************************************************************/

void
printer_rep::set_clipping (SI x1, SI y1, SI x2, SI y2, bool restore) {
  outer_round (x1, y1, x2, y2);
  renderer_rep::set_clipping (x1, y1, x2, y2);
  if (restore) {
    print (PS_CLIP_POP);
    cfn= "";
  }
  else {
    print (PS_CLIP_PUSH);
    print (x1, y1);
    print (x2, y2);
    print (PS_CLIP);
  }
}
  
/******************************************************************************
* graphical routines
******************************************************************************/

color
printer_rep::get_color () {
  return fg;
}

color
printer_rep::get_background () {
  return bg;
}

void
printer_rep::set_color (color c) {
  if (fg==c) return;
  fg= c;
  select_color (c);
}

void
printer_rep::set_background (color c) {
  if (bg==c) return;
  bg= c;
}

void
printer_rep::draw (int ch, font_glyphs fn, SI x, SI y) {
  glyph gl= fn->get(ch);
  if (is_nil (gl)) return;
  string name= fn->res_name;
  unsigned char c= ch;
  if (ch >= 256) {
    name= name * "-" * as_string (ch / 256);
    c= (unsigned char) (ch & 255);
  }
  make_tex_char (name, c, gl);
  select_tex_font (name);
  move_to (x, y);
  print ("(" * prepare_text (string ((char) c)) * ")p");
  tex_flag= true;
  xpos += gl->lwidth;
}

void
printer_rep::set_line_style (SI w, int type, bool round) {
  (void) type;
  (void) round;
  if (lw == w) return;
  lw= w;
  select_line_width (w);
}

void
printer_rep::line (SI x1, SI y1, SI x2, SI y2) {
  print (x1, y1);
  print (x2, y2);
  print (PS_LINE);
}

void
printer_rep::lines (array<SI> x, array<SI> y) {
  int i, n= N(x);
  if ((N(y) != n) || (n<1)) return;
  print (x[0], y[0]);
  print (PS_POL_START);
  for (i=1; i<n; i++) {
    print (x[i], y[i]);
    print (PS_POL_NEXT);
  }
  print (PS_STROKE);
}

void
printer_rep::clear (SI x1, SI y1, SI x2, SI y2) {
  select_color (bg);
  print (x1, y1);
  print (x2, y2);
  print (PS_FILL);
  select_color (fg);
}

void
printer_rep::fill (SI x1, SI y1, SI x2, SI y2) {
  if ((x1<x2) && (y1<y2)) {
    print (x1, y1);
    print (x2, y2);
    print (PS_FILL);
  }
}

void
printer_rep::arc (SI x1, SI y1, SI x2, SI y2, int alpha, int delta) {
  print ((x1+x2)/2, (y1+y2)/2);
  print (as_string ((x2-x1)/(2*PIXEL)));
  print (as_string ((y1-y2)/(2*PIXEL)));
  print (as_string (((double) alpha)/64));
  print (as_string (((double) (alpha+delta))/64));
  print (PS_ARC);
}

void
printer_rep::fill_arc (SI x1, SI y1, SI x2, SI y2, int alpha, int delta) {
  print ((x1+x2)/2, (y1+y2)/2);
  print (as_string ((x2-x1)/(2*PIXEL)));
  print (as_string ((y1-y2)/(2*PIXEL)));
  print (as_string (((double) alpha)/64));
  print (as_string (((double) (alpha+delta))/64));
  print (PS_FILL_ARC);
}

void
printer_rep::polygon (array<SI> x, array<SI> y, bool convex) {
  int i, n= N(x);
  if ((N(y) != n) || (n<1)) return;
  print (x[0], y[0]);
  print (PS_POL_START);
  for (i=1; i<n; i++) {
    print (x[i], y[i]);
    print (PS_POL_NEXT);
  }
  print (PS_POL_END);
}

void
printer_rep::xpm (url file_name, SI x, SI y) {
  (void) file_name; (void) x; (void) y;
  fatal_error ("Not yet implemented", "printer_rep::xpm");
}

/*
string
incorporate_postscript (string s) {
  int i;
  string r;
  for (i=0; i<N(s); )
    if (s[i] == '%') {
      for (; (i<N(s)) && (s[i]!='\n'); i++);
      if (i<N(s)) i++;
    }
    else {
      for (; (i<N(s)) && (s[i]!='\n'); ) r << s[i++];
      if (i<N(s)) { r << s[i++]; }
    }
  return r;
}
*/

void
printer_rep::image (
  url u, SI w, SI h, SI x, SI y,
  double cx1, double cy1, double cx2, double cy2)
{
  int bx1, by1, bx2, by2;
  ps_bounding_box (u, bx1, by1, bx2, by2);
  int x1= bx1 + (int) (cx1 * (bx2 - bx1) + 0.5);
  int y1= by1 + (int) (cy1 * (by2 - by1) + 0.5);
  int x2= bx1 + (int) (cx2 * (bx2 - bx1) + 0.5);
  int y2= by1 + (int) (cy2 * (by2 - by1) + 0.5);

  double sc_x= (72.0/dpi) * ((double) (w/PIXEL)) / ((double) (x2-x1));
  double sc_y= (72.0/dpi) * ((double) (h/PIXEL)) / ((double) (y2-y1));
  cr ();
  cr ();

  print (x, y);
  print ("a");
  print ("currentpoint");
  print ("currentpoint");
  print ("translate");
  print (as_string (sc_x));
  print (as_string (sc_y));
  print ("scale");
  print ("neg");
  print ("exch");
  print ("neg");
  print ("exch");
  print ("translate");
  print (x, y);
  print ("a");
  cr ();
  /* Black Black 248 3155 a currentpoint currentpoint translate
     0.37114 0.37114 scale neg exch neg exch translate 248 3155 a */

  print ("@beginspecial");
  print (as_string (x1));
  print ("@llx");
  print (as_string (y1));
  print ("@lly");
  print (as_string (x2));
  print ("@urx");
  print (as_string (y2));
  print ("@ury");
  print (as_string (10*(x2-x1)));
  print ("@rwi");
  print ("@clip");
  print ("@setspecial");
  cr ();
  /* @beginspecial 0 @llx 0 @lly 613.291260 @urx 613.291260 @ury 6110 @rwi
     @clip @setspecial */
  
  string ps_image= ps_load (u);
  string imtext= is_ramdisc (u)? "inline image": as_string (u);
  body << "%%BeginDocument: " << imtext  << "\n";
  body << ps_image; // incorporate_postscript (ps_image);
  body << "%%EndDocument";
  cr ();

  print ("@endspecial");
  print (x, y);
  print ("a");
  print ("currentpoint");
  print ("currentpoint");
  print ("translate");
  print (as_string (1/sc_x));
  print (as_string (1/sc_y));
  print ("scale");
  print ("neg");
  print ("exch");
  print ("neg");
  print ("exch");
  print ("translate");
  print (x, y);
  print ("a");
  cr ();
  cr ();
  
  /* @endspecial 248 3155 a currentpoint currentpoint translate
     1 0.37114 div 1 0.37114 div scale neg exch neg exch translate
     248 3155 a 660 3073 a ... */

  (void) w; (void) h;
}

void
printer_rep::fetch (SI x1, SI y1, SI x2, SI y2, renderer ren, SI x, SI y) {
  (void) x1; (void) y1; (void) x2; (void) y2;
  (void) ren; (void) x; (void) y;
}

void
printer_rep::new_shadow (renderer& ren) {
  (void) ren;
}

void
printer_rep::delete_shadow (renderer& ren) {
  (void) ren;
}

void
printer_rep::get_shadow (renderer ren, SI x1, SI y1, SI x2, SI y2) {
  (void) ren; (void) x1; (void) y1; (void) x2; (void) y2;
}

void
printer_rep::put_shadow (renderer ren, SI x1, SI y1, SI x2, SI y2) {
  (void) ren; (void) x1; (void) y1; (void) x2; (void) y2;
}

void
printer_rep::apply_shadow (SI x1, SI y1, SI x2, SI y2) {
  (void) x1; (void) y1; (void) x2; (void) y2;
}

/******************************************************************************
* user interface
******************************************************************************/

renderer
printer (url ps_file_name, int dpi, int nr_pages,
	 string page_type, bool landscape, double paper_w, double paper_h)
{
  return tm_new<printer_rep> (ps_file_name, dpi, nr_pages,
			  page_type, landscape, paper_w, paper_h);
}
