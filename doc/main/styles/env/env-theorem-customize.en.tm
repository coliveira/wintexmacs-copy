<TeXmacs|1.0.4>

<style|tmdoc>

<\body>
  <tmdoc-title|Customization of the theorem-like environments>

  The following customizable macros are used for the rendering of textual
  environments:

  <\explain|<explain-macro|render-theorem|name|body>>
    This macro is used for displaying a theorem-like environments. The first
    argument <src-arg|name> specifies the name of the theorem, like ``Theorem
    1.2'' and the second argument <src-arg|body> contains the body of the
    theorem. This environment is used for environments defined by
    <markup|new-theorem>.
  </explain>

  <\explain|<explain-macro|render-remark|name|body>>
    Similar to <markup|render-theorem>, but for remark-like environments.
  </explain>

  <\explain|<explain-macro|render-exercise|name|body>>
    Similar to <markup|render-theorem>, but for exercise-like environments.
  </explain>

  <\explain|<explain-macro|render-proof|name|body>>
    Similar to <markup|render-theorem>, but for proofs. This environment is
    mainly used for customizing the name of a proof, like in ``End of the
    proof of theorem 1.2''.
  </explain>

  Notice that you may also use these macros if you want an environment which
  is rendered in a similar way as a theorem, but with another name (like
  ``Corollary of Theorem 7'').

  The following tags can be used for further customization of the rendering:

  <\explain|<explain-macro|theorem-name|name>>
    This macro controls the appearance of the names of theorem-like <em|and>
    remark-like environments. Most styles use bold face or small capitals.
  </explain>

  <\explain|<explain-macro|exercise-name|name>>
    Similar to <markup|theorem-name>, but for exercises.
  </explain>

  <\explain|<explain-macro|theorem-sep>>
    The separator between the name of a theorem-like or remark-like
    environment and its main body. By default, this is a period followed by a
    space.
  </explain>

  <\explain|<explain-macro|exercise-sep>>
    Similar to <markup|theorem-sep>, but for exercises.
  </explain>

  Each standard environment <markup|<em|x>> also comes with a customizable
  macro <markup|<em|x>-text> which renders the localized name of the
  environment. For instance, <inactive*|<with|language|dutch|<theorem-text>>>
  yields ``<with|language|dutch|<theorem-text>>''.

  <tmdoc-copyright|1998--2002|Joris van der Hoeven>

  <tmdoc-license|Permission is granted to copy, distribute and/or modify this
  document under the terms of the GNU Free Documentation License, Version 1.1
  or any later version published by the Free Software Foundation; with no
  Invariant Sections, with no Front-Cover Texts, and with no Back-Cover
  Texts. A copy of the license is included in the section entitled "GNU Free
  Documentation License".>
</body>

<\initial>
  <\collection>
    <associate|language|english>
  </collection>
</initial>