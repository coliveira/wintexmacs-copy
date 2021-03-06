<TeXmacs|1.0.6.9>

<style|source>

<\body>
  <active*|<\src-title>
    <src-package|gui-widget|1.0>

    <\src-purpose>
      Global appearance and low-level markup for widgets
    </src-purpose>

    <src-copyright|2007|Joris van der Hoeven>

    <\src-license>
      This software falls under the <hlink|GNU general public
      license, version 3 or later|$TEXMACS_PATH/LICENSE>.
      It comes WITHOUT ANY WARRANTY WHATSOEVER.
      You should have received a copy of the license which the software.
      If not, see <hlink|http://www.gnu.org/licenses/gpl-3.0.html|http://www.gnu.org/licenses/gpl-3.0.html>.
    </src-license>
  </src-title>>

  <\active*>
    <\src-comment>
      Global layout for widgets
    </src-comment>
  </active*>

  <assign|bg-color|light grey>

  <assign|page-medium|automatic>

  <assign|scroll-bars|false>

  <assign|font-family|ss>

  <assign|par-sep|0.2em>

  <assign|par-par-sep|0em>

  <assign|par-line-sep|0em>

  <assign|page-screen-left|0.5em>

  <assign|page-screen-right|0.5em>

  <assign|page-screen-top|0.5em>

  <assign|page-screen-bot|0.5em>

  <\active*>
    <\src-comment>
      Low-level macros for widgets
    </src-comment>
  </active*>

  <assign|disable-writability|<value|identity>>

  <assign|enable-writability|<value|identity>>

  <drd-props|disable-writability|arity|1|border|no|accessible|all|disable-writability|all>

  <drd-props|enable-writability|arity|1|border|no|accessible|all|enable-writability|all>

  <drd-props|widget-window|arity|<tuple|repeat|1|1>|border|no>

  \;

  <assign|widget-prefix|>

  <assign|widget|<macro|name|body|<style-with|src-compact|none|<with|widget-prefix|<merge|<value|widget-prefix>|<arg|name>|->|<wide-normal|<arg|body>>>>>>

  <assign|widget-id|<macro|name|<id|<merge|<value|widget-prefix>|<arg|name>>>>>

  <assign|widget-value|<macro|name|<extern|widget-ref|<arg|name>|<value|widget-prefix>>>>

  <assign|widget-cmd|<macro|cmd|<merge|(widget-delay (widget-with
  "|<value|widget-prefix>|" |<arg|cmd>|))>>>

  \;
</body>

<\initial>
  <\collection>
    <associate|language|english>
    <associate|preamble|true>
    <associate|sfactor|5>
  </collection>
</initial>

<\references>
  <\collection>
    <associate||<tuple|<error|argument body>|?>>
  </collection>
</references>