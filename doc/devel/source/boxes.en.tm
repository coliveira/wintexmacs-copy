<TeXmacs|1.0.1.11>

<style|tmdoc>

<\body>
  <expand|tmdoc-title|The boxes produced by the typesetter>

  <section|Introduction>

  The <apply|TeXmacs> typesetter essentially translates a document
  represented by a tree into a graphical box, which can either be displayed
  on the screen or on a printer. Contrary to a system like <apply|LaTeX>, the
  graphical box actually contains much more information than is necessary for
  a graphical rendering. Roughly speaking, this information can be subdivided
  into the following categories:

  <\itemize>
    <item>Logical and physical bounding boxes.

    <item>A method for graphical rendering.

    <item>Miscellaneous typesetting information.

    <item>Keeping track of the source subtree which led to box.

    <item>Computing the positions of cursors and selections.

    <item>Event handlers for dynamic content.
  </itemize>

  The logical bounding box is used by the typesetter to position the box with
  respect to other boxes. A certain amount of other information, such as the
  slant of the box, is also stored for the typesetter. The physical bounding
  box encloses the graphical representation of the box. This knowledge is
  needed when partially redrawing a box in an efficient way.

  In order to position the cursor or when making a selection, it is necessary
  to have a correspondence between logical positions in the source tree and
  physical positions in the typesetted boxes. More precisely, boxes and their
  subboxes are logically organized as a tree. Boxes provide routines to
  translate between paths in the box tree and the source tree and to find the
  path which is associated to a graphical point.

  <section|The correspondence between a box and its source>

  <subsection|Discussion of the problems being encountered>

  In order to implement the correspondence between paths in the source tree
  and the box tree, one has to face several simultaneous difficulties:

  <\enumerate>
    <item>Due to line breaking, footnotes and macro expansions, the
    correspondence may be non straightforward.

    <item>The correspondence has to be reasonably time and space efficient.

    <item>Some boxes, such header and footers, or certain results of macro
    expansions, may not be ``accessible''. Although one should be able to
    find a reasonable cursor position when clicking on them, the contents of
    this box can not be edited directly.

    <item>The correspondence has to be reasonably complete (see the next
    section).
  </enumerate>

  The first difficulty forces us to store a path in the source tree along
  with any box. In order to save storage, this path is stored in a reversed
  manner, so that common heads can be shared. This common head sharing is
  also necessary to quickly change the source locations when modifying the
  source tree, for instance by inserting a new paragraph.

  In order to cope with the third difficulty, the inverse path may start with
  a negative number, which indicates that the box can not directly be edited
  (we also say that the box is a decoration). In this case, the tail of the
  inverse path corresponds to a location in the source tree, where the cursor
  should be positioned when clicking on the box. The negative number
  influences the way in which this is done.

  <subsection|The three kinds of paths>

  More precisely, we have to deal with three kinds of paths:

  <\description>
    <expand|item*|Tree paths.>These paths correspond to paths in the source
    tree. Actually, the path minus its last item points to a subtree of the
    source tree. The last item gives a position in this subtree: if the
    subtree is a leaf, i.e. a string, it is a position in this string.
    Otherwise a zero indicates a position before the subtree and a one a
    position after the subtree.

    <expand|item*|Inverse paths.>These are just reverted tree paths (with
    shared tails), with an optional negative head. A negative head indicates
    that the tree path is not accessible, i.e. the corresponding subtree does
    not correspond to editable content. If the negative value is
    <with|mode|math|-2>, <with|mode|math|-3> or <with|mode|math|-4>, then a
    zero or one has to be put behind the tree path, depending on the value
    and the cursor position.

    <expand|item*|Box paths.>These paths correspond to logical paths in the
    box tree. Again, the path minus its last item points to a subbox of the
    main box, and the last item gives a position in this subtree: if the
    subbox corresponds to a text box it is a position in this text. Otherwise
    a zero indicates a position before the subbox and a one a position after
    it. In the case of side boxes, a two and a three may also indicate the
    position after the left script resp. before the right script.
  </description>

  <subsection|The conversion routines>

  In order to implement the conversion between the three kinds of paths,
  every box comes with a reference inverse path <verbatim|ip> in the source
  tree. Composite boxes also come with a left and a right inverse path
  <verbatim|lip> resp. <verbatim|rip>, which correspond to the left-most and
  right-most accessible paths in its subboxes (if there are such subboxes).

  The routine:

  <\verbatim>
    \ \ \ \ virtual path box_rep::find_tree_path (path bp)
  </verbatim>

  transforms a box path into a tree path. This routine (which only uses
  <verbatim|ip>) is fast and has a linear time complexity as a function of
  the lengths of the paths. The routine:\ 

  <\verbatim>
    \ \ \ \ virtual path box_rep::find_box_path (path p)
  </verbatim>

  does the inverse conversion. Unfortunately, in the worst case, it may be
  necessary to search for the matching tree path in all subboxes.
  Nevertheless, in the best case, a dichotomic algorithm (which uses
  <verbatim|lip> and <verbatim|rip>), finds the right branch how to descend
  in a logarithmic time. This algorithm also has a quadratic time complexity
  as a function of the lengths of the paths, because we frequently need to
  revert paths.

  <section|The cursor and selections>

  In order to fulfill the requirement of being a ``structured editor'',
  <apply|TeXmacs> needs to provide a (reasonably) complete correspondence
  between logical tree paths and physical cursor positions. This yields an
  additional difficulty in the case of ``environment changes'', such as a
  change in font or color. Indeed, when you are on the border of such a
  change, it is not clear <with|font shape|italic|a priori> which environment
  you are in.

  In <apply|TeXmacs>, the cursor position therefore contains an
  <with|mode|math|x> and a <with|mode|math|y> coordinate, as well as an
  additional infinitesimal <with|mode|math|x>-coordinate, called
  <with|mode|math|\<delta\>>. A change in environment is then represented by
  a box with an infinitesimal width. Although the
  <with|mode|math|\<delta\>>-position of the cursor is always zero when you
  select using the mouse, it may be non zero when moving around using the
  cursor keys. The linear time routine:\ 

  <\verbatim>
    \ \ \ \ virtual path box_rep::find_box_path (SI x, SI y, SI delta)
  </verbatim>

  as a function of the length of the path searches the box path which
  corresponds to a cursor position. Inversely, the routine:\ 

  <\verbatim>
    \ \ \ \ virtual cursor box_rep::find_cursor (box bp)
  </verbatim>

  yields a graphical representation for the cursor at a certain box path. The
  cursor is given by its <with|mode|math|x>, <with|mode|math|y> and
  <with|mode|math|\<delta\>> coordinates and a line segment relative to this
  origin, given by its extremities <with|mode|math|(x<rsub|1>,y<rsub|1>)> and
  <with|mode|math|(x<rsub|2>,y<rsub|2>)>.

  In a similar way, the routine:\ 

  <\verbatim>
    \ \ \ \ virtual selection box_rep::find_selection (box lbp, box rbp)
  </verbatim>

  computes the selection between two given box paths. This selection
  comprises two delimiting tree paths and a graphical representation in the
  form of a list of rectangles.

  <apply|tmdoc-copyright|1998--2002|Joris van der Hoeven>

  <expand|tmdoc-license|Permission is granted to copy, distribute and/or
  modify this document under the terms of the GNU Free Documentation License,
  Version 1.1 or any later version published by the Free Software Foundation;
  with no Invariant Sections, with no Front-Cover Texts, and with no
  Back-Cover Texts. A copy of the license is included in the section entitled
  "GNU Free Documentation License".>
</body>

<\initial>
  <\collection>
    <associate|paragraph width|150mm>
    <associate|odd page margin|30mm>
    <associate|shrinking factor|4>
    <associate|page right margin|30mm>
    <associate|page top margin|30mm>
    <associate|reduction page right margin|25mm>
    <associate|page type|a4>
    <associate|reduction page bottom margin|15mm>
    <associate|even page margin|30mm>
    <associate|reduction page left margin|25mm>
    <associate|page bottom margin|30mm>
    <associate|reduction page top margin|15mm>
    <associate|language|english>
  </collection>
</initial>

<\references>
  <\collection>
    <associate|idx-1|<tuple|<uninit>|?>>
    <associate|toc-1|<tuple|1|?>>
    <associate|toc-2|<tuple|2|?>>
    <associate|idx-2|<tuple|<uninit>|?>>
    <associate|toc-3|<tuple|2.1|?>>
    <associate|toc-4|<tuple|2.2|?>>
    <associate|toc-5|<tuple|2.3|?>>
    <associate|toc-6|<tuple|3|?>>
    <associate|toc-7|<tuple|4.|?>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|toc>
      <vspace*|1fn><with|font series|<quote|bold>|math font
      series|<quote|bold>|1<space|2spc>Introduction><value|toc-dots><pageref|toc-1><vspace|0.5fn>

      <vspace*|1fn><with|font series|<quote|bold>|math font
      series|<quote|bold>|2<space|2spc>The correspondence between a box and
      its source><value|toc-dots><pageref|toc-2><vspace|0.5fn>

      2.1<space|2spc>Discussion of the problems being
      encountered<value|toc-dots><pageref|toc-3>

      2.2<space|2spc>The three kinds of paths<value|toc-dots><pageref|toc-4>

      2.3<space|2spc>The conversion routines<value|toc-dots><pageref|toc-5>

      <vspace*|1fn><with|font series|<quote|bold>|math font
      series|<quote|bold>|3<space|2spc>The cursor and
      selections><value|toc-dots><pageref|toc-6><vspace|0.5fn>
    </associate>
  </collection>
</auxiliary>
