<TeXmacs|1.0.3.4>

<style|source>

<\body>
  <active*|<\src-title>
    <src-package|tmdoc-traversal|1.0>

    <\src-purpose>
      This package contains macros which allow for the automatic generation
      or extraction of documentation from small files. For instance, there
      are tags which indicate how to traverse the documentation.
    </src-purpose>

    <src-copyright|2001--2004|Joris van der Hoeven>

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
      Decorations for title-bars with gnus.
    </src-comment>
  </active*>

  <assign|tmdoc-lgnu|<macro|<active*|<postscript|local:$TEXMACS_PATH/misc/images/tm_gnu1.ps|/4|/4||||>>>>

  <assign|tmdoc-rgnu|<macro|<active*|<postscript|local:$TEXMACS_PATH/misc/images/tm_gnu2.ps|/4|/4||||>>>>

  <assign|tmdoc-gnu-title|<macro|title|<tabular*|<tformat|<twith|table-width|0.99par>|<cwith|1|1|2|2|cell-hpart|1>|<cwith|1|1|1|-1|cell-valign|c>|<cwith|1|1|1|1|cell-vcorrect|n>|<cwith|1|1|3|3|cell-vcorrect|n>|<cwith|1|1|2|2|cell-hyphen|c>|<cwith|1|1|1|1|cell-lsep|0fn>|<cwith|1|1|3|3|cell-rsep|0fn>|<table|<row|<cell|<tmdoc-lgnu>>|<\cell>
    <\with|par-mode|center>
      <arg|title>
    </with>
  </cell>|<cell|<tmdoc-rgnu>>>>>>>>

  <\active*>
    <\src-comment>
      Title bars.
    </src-comment>
  </active*>

  <assign|tmdoc-title|<macro|x|<style-with|src-compact|none|<with|font-series|bold|font-size|1.19|color|dark
  grey|<block*|<tformat|<twith|table-width|1par>|<cwith|1|-1|1|1|cell-lborder|0.5ln>|<cwith|1|-1|1|1|cell-rborder|0.5ln>|<cwith|1|-1|1|1|cell-bborder|0.5ln>|<cwith|1|-1|1|1|cell-tborder|0.5ln>|<cwith|1|1|1|1|cell-rsep|0fn>|<cwith|1|1|1|1|cell-lsep|0fn>|<cwith|1|1|1|-1|cell-background|pastel
  yellow>|<table|<row|<cell|<tmdoc-gnu-title|<with|color|black|<arg|x>>>>>>>>><vspace|1fn>>>>

  <assign|tmdoc-title*|<macro|x|y|<style-with|src-compact|none|<with|font-series|bold|font-size|1.19|color|dark
  grey|<block*|<tformat|<twith|table-width|1par>|<cwith|1|-1|1|1|cell-lborder|0.5ln>|<cwith|1|-1|1|1|cell-rborder|0.5ln>|<cwith|1|-1|1|1|cell-bborder|0.5ln>|<cwith|1|-1|1|1|cell-tborder|0.5ln>|<cwith|1|1|1|1|cell-rsep|0fn>|<cwith|1|1|1|1|cell-lsep|0fn>|<cwith|2|2|1|-1|cell-background|pastel
  orange>|<cwith|1|1|1|-1|cell-background|pastel
  yellow>|<cwith|2|2|1|1|cell-hyphen|c>|<table|<row|<cell|<tmdoc-gnu-title|<with|color|black|<arg|x>>>>>|<row|<\cell>
    <\with|par-mode|center>
      <with|color|black|font-series|medium|font-size|0.84|[<arg|y>]>
    </with>
  </cell>>>>>><vspace|1fn>>>>

  <assign|tmdoc-title**|<macro|x|y|z|<style-with|src-compact|none|<with|font-series|bold|font-size|1.19|color|dark
  grey|<block*|<tformat|<twith|table-width|1par>|<cwith|1|-1|1|1|cell-lborder|0.5ln>|<cwith|1|-1|1|1|cell-rborder|0.5ln>|<cwith|1|-1|1|1|cell-bborder|0.5ln>|<cwith|1|-1|1|1|cell-tborder|0.5ln>|<cwith|2|2|1|1|cell-rsep|0fn>|<cwith|2|2|1|1|cell-lsep|0fn>|<cwith|1|1|1|-1|cell-background|pastel
  orange>|<cwith|3|3|1|-1|cell-background|pastel
  orange>|<cwith|2|2|1|-1|cell-background|pastel
  yellow>|<cwith|1|1|1|1|cell-hyphen|c>|<cwith|3|3|1|1|cell-hyphen|c>|<table|<row|<\cell>
    <\with|par-mode|center>
      <with|color|black|font-series|medium|font-size|0.84|[<arg|x>]>
    </with>
  </cell>>|<row|<cell|<tmdoc-gnu-title|<with|color|black|<arg|y>>>>>|<row|<\cell>
    <\with|par-mode|center>
      <with|color|black|font-series|medium|font-size|0.84|[<arg|z>]>
    </with>
  </cell>>>>>><vspace|1fn>>>>

  <\active*>
    <\src-comment>
      Tags for displaying hyperlinks to all available translations of
      documentation.

      The flags were found in /usr/share/pixmaps/gkb.
    </src-comment>
  </active*>

  <assign|tmdoc-flag|<macro|flag| <with|color|black|<block|<tformat|<cwith|1|1|1|1|cell-lsep|0fn>|<cwith|1|1|1|1|cell-rsep|0fn>|<cwith|1|1|1|1|cell-bsep|0fn>|<cwith|1|1|1|1|cell-tsep|0fn>|<cwith|1|1|1|1|cell-vcorrect|n>|<table|<row|<cell|<arg|flag>>>>>>>
  >>

  <assign|tmdoc-translation|<macro|name|suffix|<if|<unequal|<find-file|<merge|<arg|name>|<merge|<arg|suffix>|.tm>>>|false>|<hlink|<compound|tmdoc-flag|<postscript|<merge|http://www.texmacs.org/Images/flag|<merge|<arg|suffix>|.png>>|/2|/2||||>>|<merge|<arg|name>|<merge|<arg|suffix>|.tm>>>>>>

  <assign|tmdoc-translations|<\macro|name>
    <\surround||<vspace|0.5fn>>
      <\with|par-mode|center>
        <tmdoc-translation|<arg|name>|.de><tmdoc-translation|<arg|name>|.en><tmdoc-translation|<arg|name>|.es><tmdoc-translation|<arg|name>|.fr><tmdoc-translation|<arg|name>|.it><tmdoc-translation|<arg|name>|.pt>
      </with>
    </surround>
  </macro>>

  <\active*>
    <\src-comment>
      The footers and copyright information.
    </src-comment>
  </active*>

  <assign|tmdoc-bar|<macro|content|<with|color|dark
  grey|<block|<tformat|<cwith|1|1|1|1|cell-background|broken
  white>|<twith|table-width|1par>|<cwith|1|1|1|1|cell-hyphen|t>|<cwith|1|1|1|1|cell-lborder|0.5ln>|<cwith|1|1|1|1|cell-rborder|0.5ln>|<cwith|1|1|1|1|cell-bborder|0.5ln>|<cwith|1|1|1|1|cell-tborder|0.5ln>|<table|<row|<cell|<with|color|black|<arg|content>>>>>>>>>>

  <assign|tmdoc-copyright-extra|<macro|x|, <arg|x>>>

  <assign|tmdoc-copyright|<xmacro|x|<vspace*|1fn><tmdoc-bar|<copyright><with|font-size|0.84|
  \ <arg|x|0> <localize|by> <arg|x|1><map-args|tmdoc-copyright-extra|concat|x|2>>>>>

  <assign|tmdoc-license|<macro|x|<with|color|grey|font-size|0.59|<with|language|english|<arg|x>>>>>

  <\active*>
    <\src-comment>
      Tags for the traversal of documentation.
    </src-comment>
  </active*>

  <assign|traverse|<macro|x|<itemize|<arg|x>>>>

  <assign|branch|<macro|x|y|<item><hlink|<arg|x>|<arg|y>>>>

  <assign|extra-branch|<macro|x|y|<item><hlink|<with|color|brown|<arg|x>>|<arg|y>>>>

  <assign|continue|<macro|x|y|<item><hlink|<with|color|brown|<arg|x>>|<arg|y>>>>

  <assign|tmdoc-include|<\macro|what>
    <extern|tmdoc-include|<find-file|.|$TEXMACS_DOC_PATH|http://www.gnu.org/software/texmacs-doc|<arg|what>>>
  </macro>>

  \;
</body>

<\initial>
  <\collection>
    <associate|language|english>
    <associate|page-bot|30mm>
    <associate|page-even|30mm>
    <associate|page-odd|30mm>
    <associate|page-reduce-bot|15mm>
    <associate|page-reduce-left|25mm>
    <associate|page-reduce-right|25mm>
    <associate|page-reduce-top|15mm>
    <associate|page-right|30mm>
    <associate|page-top|30mm>
    <associate|page-type|a4>
    <associate|par-width|150mm>
    <associate|preamble|true>
    <associate|sfactor|4>
  </collection>
</initial>