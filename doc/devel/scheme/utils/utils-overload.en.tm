<TeXmacs|1.0.6.10>

<style|tmdoc>

<\body>
  <tmdoc-title|Function definition and contextual overloading>

  Conventional programming languages often provide mechanism to overload
  certain functions depending on the types of the arguments. <TeXmacs>
  provides additional context-based overloading mechanisms, which require the
  use of the <scm-macro|tm-define> construct for function declarations (and
  <scm-macro|tm-define-macro> for macro declarations). Furthermore, one may
  use <scm-macro|tm-define> for associating additional properties to a
  function, such as documentation or default values for the arguments.

  <\explain>
    <explain-scm-macro|tm-define|<scm-arg|head>|<scm-args|options>|<scm-args|body>><explain-synopsis|<TeXmacs>
    function definition>

    <explain-scm-macro|tm-define-macro|<scm-arg|head>|<scm-args|options>|<scm-args|body>><explain-synopsis|<TeXmacs>
    macro definition>
  <|explain>
    <TeXmacs> function and macro declarations are similar to usual
    declarations based on <verbatim|define> and <verbatim|define-macro>,
    except for the additional list of <scm-arg|options> and the fact that all
    functions and macros defined using <scm-macro|tm-define> and
    <scm-macro|tm-define-macro> are public. Each option is of the form
    <verbatim|(:<scm-arg|kind> <scm-args|arguments>)> and the <scm-arg|body>
    starts at the first element of the list following <scm-arg|head> which is
    not of this form.
  </explain>

  <paragraph|Contextual overloading>

  We will first describe the various options for overloading. If several
  definitions are given for the same function (or macro) <verbatim|foo> and
  several definitions satisfy the corresponding overloading conditions, when
  applying <verbatim|foo> to some arguments, then the <em|most particular>
  definition will prevail. For any of the overloading options, we will
  therefore have to specify what we mean my ``most particular''. When trying
  to find out the most particular set of options, we first sort on mode, next
  on context and finally on function arguments. Notice that sorting on
  function arguments is not yet fully implemented, so we will not discuss
  this yet.

  <\explain>
    <verbatim|(:mode <scm-arg|mode>)><explain-synopsis|mode-based
    overloading>
  <|explain>
    This option specifies that the definition is only valid when we are in a
    given <scm-arg|mode>. New modes are defined using
    <scm-macro|texmacs-modes> and modes can inherit from other modes. A mode
    <math|m<rsub|1>> is understood to be more particular than another mode
    <math|m<rsub|2>> if <math|m<rsub|1>> inherits from <math|m<rsub|2>>.
  </explain>

  <\explain>
    <verbatim|(:context <scm-arg|pred?>)><explain-synopsis|cursor path based
    overloading>
  <|explain>
    Let <math|t<rsub|1>> be the innermost tree to which the cursor is
    attached and let <math|t<rsub|2>,\<ldots\>,t<rsub|n>> denote the
    successive parents of <math|t<rsub|1>>, ending with the document
    <math|t<rsub|n>> itself. The context option is satisfied if and only if
    one of the trees <math|t<rsub|1>,\<ldots\>,t<rsub|n>> satisfies the
    preducate <scm-arg|pred?>. In the case when two predicates
    <math|P<rsub|1>> and<nbsp><math|P<rsub|2>> compete, then the most
    particular one is the one which is satisfied by a <math|t<rsub|i>> with
    the lowest value of <math|i>. An example will be given below for the
    option <verbatim|:inside>, which is a special case of
    <verbatim|:context>.
  </explain>

  <\explain>
    <verbatim|(:inside <scm-arg|label>)><explain-synopsis|cursor path based
    overloading>
  <|explain>
    This option is a special case of the <verbatim|:context> option, for the
    predicate <verbatim|(lambda (t) (tree-in? '<scm-arg|label>))>. As an
    example, let us consider the following definitions:

    <\scheme-fragment>
      (tm-define (special)

      \ \ (:inside 'frac)

      \ \ (with-innermost t 'frac

      \ \ \ \ (tree-set! t `(frac ,(tree-ref t 1) ,(tree-ref t 0)))))

      \;

      (tm-define (special)

      \ \ (:inside 'rsub)

      \ \ (with-innermost t 'rsub

      \ \ \ \ (tree-set! t `(rsup ,(tree-ref t 0)))))
    </scheme-fragment>

    Assuming that your cursor is inside a fraction inside a subscript,
    calling <verbatim|special> will swap the numerator and the denominator.
    On the other hand, if your cursor is inside a subscript inside a
    fraction, then calling <verbatim|special> will change the subscript into
    a superscript.
  </explain>

  <\explain>
    <verbatim|(:match <scm-arg|pattern>)><explain-synopsis|argument based
    overloading>
  <|explain>
    This option specifies that one necessary condition for the declaration to
    be valid valid is that the arguments match the specified pattern
    according to the primitive <scm-fun|match?>. We have not yet implemented
    a function to test whether a pattern is a restriction of another pattern,
    so ambiguous overloads cannot be resolved.
  </explain>

  <\explain>
    <verbatim|(:require <scm-arg|cond>)><explain-synopsis|argument based
    overloading>
  <|explain>
    This option specifies that one necessary condition for the declaration to
    be valid is that the condition <scm-arg|cond> is met. This condition may
    involve the arguments of the function. Again, ambiguous overloads cannot
    be resolved.
  </explain>

  <\explain>
    <verbatim|(:case <scm-arg|label-1> ...
    <scm-arg|label-n>)><explain-synopsis|argument based dispatching>
  <|explain>
    This is a very special case of the <verbatim|:match> option, where we
    require the first argument to be a compound hybrid tree whose root label
    is amongst <scm-arg|label-1> until <scm-arg|label-n>. Besides a
    simplified syntax, the implementation of <verbatim|:case> is done using a
    dispatch via a hash table. When appropriate, you should therefore
    priviledge <verbatim|:case> over the general form of <verbatim|:match>. A
    typical situation when <verbatim|:case> is useful is when writing a
    converter <verbatim|tm-\<gtr\>foo> of <TeXmacs> trees into your own
    foormat: specific converters for given tags can be added using

    <\scheme-fragment>
      (tm-define (tm-\<gtr\>foo t)

      \ \ (:case frac)

      \ \ <with|prog-font-shape|italic|tm-to-foo-converter-for-frac>)
    </scheme-fragment>
  </explain>

  <paragraph|Other options for function and macro declarations>

  Besides the contextual overloading options, the <scm-macro|tm-define> and
  <scm-macro|tm-define-macro> primitives admit several other options for
  attaching additional information to the function or macro. We will now
  describe these options and explain how the additional information attached
  to functions can be exploited.

  <\explain>
    <verbatim|(:synopsis <scm-arg|short-help>)><explain-synopsis|short
    description>
  <|explain>
    This option gives a short discription of the function or macro, in the
    form of a string <scm-arg|short-help>. As a convention, <value|scheme>
    expressions may be encoded inside this string by using the
    <verbatim|@>-prefix. For instance:

    <\scheme-fragment>
      (tm-define (list-square l)

      \ \ (:synopsis "Appends the list @l to itself")

      \ \ (append l l))
    </scheme-fragment>

    The synopsis of a function is used for instance in order to provide a
    short help string for the function. In the future, we might also use it
    for help balloons describing menu items.
  </explain>

  <\explain>
    <verbatim|(:argument <scm-arg|var> <scm-arg|description>)><explain-synopsis|argument
    description>
  <|explain>
    This option gives a short <scm-arg|description> of one of the arguments
    <scm-arg|var> to the function or macro. Such a description is used for
    instance for the prompts, when calling the function interactively.
  </explain>

  <\explain>
    <verbatim|(:returns <scm-arg|description>)><explain-synopsis|return value
    description>
  <|explain>
    This option gives a short <scm-arg|description> of the return value of
    the function or macro.
  </explain>

  <\explain>
    <verbatim|(:default <scm-arg|var> <scm-arg|val>)><explain-synopsis|default
    value for argument>
  <|explain>
    This option specifies a default value <scm-arg|val> for the argument
    <scm-arg|var>. The default value will typically be proposed when calling
    the function interactively.
  </explain>

  <\explain>
    <verbatim|(:proposals <scm-arg|var> <scm-arg|val-list>)><explain-synopsis|proposed
    values for argument>
  <|explain>
    This option is a variant of <verbatim|:default>, when a list of possible
    values is proposed instead of merely one default value.
  </explain>

  <\explain>
    <verbatim|(:interactive <scm-arg|yes?>)><explain-synopsis|relies on
    interactive user input?>
  <|explain>
    This option specifies that the function relies on interactive user input,
    whenever <scm-arg|yes?> is <verbatim|#t>. Interactive functions are
    postfixed by three dots when they are used in menu items.
  </explain>

  <\explain>
    <verbatim|(:check-mark <scm-arg|type>
    <scm-arg|pred?>)><explain-synopsis|use checkmark in menus item?>
  <|explain>
    This option indicates that a checkmark might have to be displayed before
    a menu item which invokes the function. More precisely, a predicate
    <scm-arg|pred?> is used in order to determine whether the checkmark has
    to be displayed; it is passed exactly the same arguments as the function
    itself. Several types of checkmarks are available: <verbatim|"v"> for
    <math|\<checked\>>, <verbatim|"o"> for <math|\<circ\>> and <verbatim|"*">
    for <math|\<bullet\>>. The first type is used for toggles and the last
    ones for the current value among different possibilities (the
    <math|<group|\<circ\>>> is used for implicitly determined values and
    <math|\<bullet\>> for values which are explicitly selected by the user).
  </explain>

  <\explain>
    <verbatim|(:secure <scm-arg|yes?>)><explain-synopsis|secure function or
    macro?>
  <|explain>
    This option specifies whether a given function or macro is secure. Secure
    functions are useful in combination with stylesheets. Indeed, allowing
    documents to call arbitrary <value|scheme> programs would be insecure
    when opening a foreign document found somewhere on the web. Nevertheless,
    when writing plug-ins, it can be useful to use arbitrary scheme functions
    in order to evaluate <TeXmacs> macros. This can be done by declaring the
    functions that you want to use to be secure.
  </explain>

  <tmdoc-copyright|2007|Joris van der Hoeven>

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