<TeXmacs|1.0.3.7>

<style|tmdoc>

<\body>
  <tmdoc-title|Style-sheet language>

  The style-sheet language refers to the set of document constructs used for
  writing style files. This includes logical and physical document markup and
  a number <def-index|style-sheet markup> primitives which should never be
  used in documents. The style-sheet markup primitives can be separated in
  two main categories: the style-sheet fundamentals, described in this
  section, and the functional operator primitives which are used to perform
  computations in style-sheets.

  This section documents the fundamental building blocks of the style-sheet
  language, operators which bear a special relation to typeset boxes, and a
  few assorted operators which should normally only be used in style-sheets.

  <\traverse>
    <branch|Environment|prim-env.en.tm>

    <branch|Macros|prim-macro.en.tm>

    <branch|Flow control|prim-control.en.tm>

    <branch|Box operators|prim-box.en.tm>

    <branch|Miscellaneous style-sheet operators|prim-style-misc.en.tm>
  </traverse>

  <tmdoc-copyright|2004|David Allouche>

  <tmdoc-license|Permission is granted to copy, distribute and/or modify this
  document under the terms of the GNU Free Documentation License, Version 1.1
  or any later version published by the Free Software Foundation; with no
  Invariant Sections, with no Front-Cover Texts, and with no Back-Cover
  Texts. A copy of the license is included in the section entitled "GNU Free
  Documentation License".>
</body>

<\initial>
  <\collection>
    <associate|page-bot|30mm>
    <associate|page-even|30mm>
    <associate|page-odd|30mm>
    <associate|page-reduce-bot|15mm>
    <associate|page-reduce-left|25mm>
    <associate|page-reduce-right|25mm>
    <associate|page-reduce-top|15mm>
    <associate|page-right|30mm>
    <associate|page-top|30mm>
    <associate|par-width|150mm>
  </collection>
</initial>