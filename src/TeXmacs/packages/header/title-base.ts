<TeXmacs|1.0.4>

<style|<tuple|source|std-pattern>>

<\body>
  <active*|<\src-title>
    <src-package-dtd|title-base|1.0|header-title|1.0>

    <\src-purpose>
      Common macros for title information
    </src-purpose>

    <src-copyright|1998--2004|Joris van der Hoeven>

    <\src-license>
      This <TeXmacs> style package falls under the <hlink|GNU general public
      license|$TEXMACS_PATH/LICENSE> and comes WITHOUT ANY WARRANTY
      WHATSOEVER. If you do not have a copy of the license, then write to the
      Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
      02111-1307, USA.
    </src-license>
  </src-title>>

  <\active*>
    <\src-comment>
      Special texts.
    </src-comment>
  </active*>

  <assign|by-text|<macro|<localize|by>>>

  <assign|email-text|<macro|<localize|Email>>>

  <assign|homepage-text|<macro|<localize|Web>>>

  <assign|abstract-text|<macro|<localize|Abstract>>>

  <assign|keywords-text|<macro|<localize|Keywords>>>

  <assign|AMS-class-text|<macro|<localize|A.M.S. subject classification>>>

  <\active*>
    <\src-comment>
      Frequently used block environments.
    </src-comment>
  </active*>

  <assign|doc-title-block|<macro|body|<tabular*|<tformat|<twith|table-width|1par>|<cwith|1|1|1|1|cell-lsep|0spc>|<cwith|1|1|1|1|cell-rsep|0spc>|<cwith|1|1|1|1|cell-bsep|0spc>|<cwith|1|1|1|1|cell-tsep|0spc>|<cwith|1|1|1|1|cell-hyphen|t>|<table|<row|<\cell>
    <\with|par-mode|center>
      <arg|body>
    </with>
  </cell>>>>>>>

  <assign|doc-author-block|<macro|body|<tabular*|<tformat|<cwith|1|1|1|1|cell-lsep|0spc>|<cwith|1|1|1|1|cell-rsep|0spc>|<cwith|1|1|1|1|cell-bsep|0spc>|<cwith|1|1|1|1|cell-tsep|0spc>|<cwith|1|1|1|1|cell-hyphen|t>|<cwith|1|1|1|1|cell-hmode|min>|<cwith|1|1|1|1|cell-width|1par>|<table|<row|<\cell>
    <\with|par-mode|center>
      <arg|body>
    </with>
  </cell>>>>>>>

  <\active*>
    <\src-comment>
      Rendering the title.
    </src-comment>
  </active*>

  <assign|doc-title|<macro|body|<surround|<vspace*|0.5fn>|<vspace|0.5fn>|<doc-title-block|<with|math-font-series|bold|font-series|bold|<huge|<arg|body>>>>>>>

  <assign|doc-subtitle|<macro|body|<surround|<vspace*|0.25fn>|<vspace|0.5fn>|<doc-title-block|<with|math-font-series|bold|font-series|bold|<larger|<arg|body>>>>>>>

  <assign|doc-running-title|<macro|body|<if|<unequal|<arg|body>|<uninit>>|<header-title|<arg|body>>>>>

  <assign|doc-author-by|<macro|body|<with|font-shape|small-caps|<by-text>
  <arg|body>>>>

  <assign|doc-author-name|<macro|body|<vspace*|1fn><arg|body><vspace|1fn>>>

  <assign|doc-author-info|<\macro|body>
    <\surround|<vspace*|1fn>|<vspace|1fn>>
      <arg|body>
    </surround>
  </macro>>

  <assign|doc-running-author|<macro|body|<if|<unequal|<arg|body>|<uninit>>|<header-author|<arg|body>>>>>

  <assign|doc-date|<macro|body|<style-with|src-compact|none|<vspace*|0.5fn><doc-title-block|<with|font-shape|italic|<arg|body>>><vspace|0.5fn>>>>

  <\active*>
    <\src-comment>
      Rendering the abstract.
    </src-comment>
  </active*>

  <assign|doc-abstract|<\macro|body>
    <\padded-normal|2fn|1fn>
      <\with|par-left|15mm|par-right|15mm>
        <\small>
          <sectional-centered-bold|<abstract-text>><vspace|0.5fn>

          <arg|body>
        </small>
      </with>
    </padded-normal>
  </macro>>

  <assign|doc-abstract*|<\macro|body|note>
    <\quasi>
      <\doc-abstract>
        <surround||<vspace|0.5fn>|<unquote|<quote-arg|body>>>

        <\with|par-par-sep|0.25fn>
          <unquote*|<arg|note>>
        </with>
      </doc-abstract>
    </quasi>
  </macro>>

  <assign|doc-keywords|<xmacro|args|<style-with|src-compact|none|<no-indent><theorem-name|<keywords-text><localize|:>
  ><concat-tuple|<quote-arg|args>|, >>>>

  <assign|doc-AMS-class|<xmacro|args|<style-with|src-compact|none|<no-indent><theorem-name|<AMS-class-text><localize|:>
  ><concat-tuple|<quote-arg|args>|, >>>>

  \;

  <assign|abstract|<\macro|body>
    <style-with|src-compact|none|<with|abstract-note|<look-up|<quasi|<doc-data-abstract|<unquote*|<quote-value|the-doc-data>>>>|0>|<style-with|src-compact|none|<compound|<style-with|src-compact|none|<if|<equal|<get-arity|<quote-value|abstract-note>>|0>|doc-abstract|doc-abstract*>>|<arg|body>|<quote-value|abstract-note>>>>>
  </macro>>

  <\active*>
    <\src-comment>
      Rendering footnotes.
    </src-comment>
  </active*>

  <new-counter|doc-note>

  <assign|the-doc-note|<macro|<number|<value|doc-note-nr>|fnsymbol>>>

  <assign|doc-author-note-next|<macro|<inc-doc-note><the-doc-note>>>

  \;

  <assign|doc-footnote-ref|<macro|body|<style-with|src-compact|none|<if|<quasi|<unequal|<get-arity|<unquote|<quote-arg|body>>>|0>>|<rsup|<doc-author-note-next>>>>>>

  <assign|doc-footnote-sub|<macro|x|; <arg|x>>>

  <assign|doc-footnote|<macro|body|<style-with|src-compact|none|<if|<unequal|<get-arity|<quote-arg|body>>|0>|<quasi|<style-with|src-compact|none|<render-footnote|<unquote|<doc-author-note-next>>|<arg|body|0><map-args|doc-footnote-sub|concat|body|1>>>>>>>>

  \;

  <assign|doc-author-note|<macro|body|<quasi|<doc-footnote|<unquote|<quote-arg|body>>>>>>

  <\active*>
    <\src-comment>
      Document titles.
    </src-comment>
  </active*>

  <assign|doc-data-one-author|<\xmacro|data>
    <\quasi>
      <unquote*|<select|<quote-arg|data>|doc-title>>

      <unquote*|<select|<quote-arg|data>|doc-subtitle>>

      <unquote*|<select|<quote-arg|data>|doc-author-data>>

      <unquote*|<select|<quote-arg|data>|doc-date>>

      <unquote*|<select|<quote-arg|data>|inactive>>
    </quasi>
  </xmacro>>

  <assign|doc-data-several-authors|<\xmacro|data>
    <\quasi>
      <unquote*|<select|<quote-arg|data>|doc-title>>

      <unquote*|<select|<quote-arg|data>|doc-subtitle>>

      <doc-authors-data|<unquote*|<select|<quote-arg|data>|doc-author-data>>>

      <unquote*|<select|<quote-arg|data>|doc-date>>

      <unquote*|<select|<quote-arg|data>|inactive>>
    </quasi>
  </xmacro>>

  <assign|doc-data-hidden|<xmacro|data|<quasi|<style-with|src-compact|none|<doc-authors-data-bis|<unquote*|<select|<quote-arg|data>|doc-author-data>>><style-with|src-compact|none|<doc-running-title|<unquote*|<select|<quote-arg|data>|doc-title|0>>>><style-with|src-compact|none|<doc-running-title|<unquote*|<select|<quote-arg|data>|doc-running-title|0>>>><doc-running-author|<style-with|src-compact|none|<author-from-authors|<unquote*|<select|<quote-arg|data>|doc-author-data|author-name|0>>>>><style-with|src-compact|none|<doc-running-author|<unquote*|<select|<quote-arg|data>|doc-running-author|0>>>>>>>>

  <assign|doc-data-abstract|<\xmacro|data>
    <\quasi>
      <unquote*|<select|<quote-arg|data>|doc-keywords>>

      <unquote*|<select|<quote-arg|data>|doc-AMS-class>>
    </quasi>
  </xmacro>>

  <assign|doc-data|<\xmacro|data>
    <style-with|src-compact|none|<\surround|<assign|the-doc-data|<quote-arg|data>>|<with|doc-note-nr|0|<quasi|<doc-data-hidden|<unquote*|<quote-arg|data>>>>>>
      <\doc-title-block>
        <with|doc-note-nr|0|<\quasi>
          <style-with|src-compact|none|<compound|<unquote|<style-with|src-compact|none|<if|<lesseq|<length|<select|<quote-arg|data>|doc-author-data>>|1>|<value|doc-data-one-author>|<value|doc-data-several-authors>>>>|<unquote*|<quote-arg|data>>>>
        </quasi>>
      </doc-title-block>
    </surround>>
  </xmacro>>

  <\active*>
    <\src-comment>
      Rendering author infomation.
    </src-comment>
  </active*>

  <assign|author-name|<macro|x|<name|<arg|x>>>>

  <assign|author-from-authors*|<macro|x|, <arg|x>>>

  <assign|author-from-authors|<xmacro|x|<style-with|src-compact|none|<if|<unequal|<get-arity|<quote-arg|x>>|0>|<arg|x|0><map-args|author-from-authors*|concat|x|1>>>>>

  <assign|author-address|<\macro|x>
    <surround|<vspace*|0.5fn>|<vspace|0.5fn>|<arg|x>>
  </macro>>

  <assign|author-email|<macro|x|<style-with|src-compact|none|<with|font-shape|italic|<email-text><localize|:>
  ><with|font-family|tt|<arg|x>>>>>

  <assign|author-homepage|<macro|x|<style-with|src-compact|none|<with|font-shape|italic|<homepage-text><localize|:>
  ><with|font-family|tt|<arg|x>>>>>

  <assign|author-note|<\macro|x>
    <arg|x>
  </macro>>

  <\active*>
    <\src-comment>
      Documents with one author.
    </src-comment>
  </active*>

  <assign|doc-author-data-note|<xmacro|data|<\quasi>
    <unquote*|<select|<quote-arg|data>|author-note|document|<pat-any>>>
  </quasi>>>

  <assign|doc-author-produce|<\macro|data>
    <\style-with|src-compact|none>
      <\quasi>
        <doc-author-name|<style-with|src-compact|none|<doc-author-by|<unquote*|<select|<quote-arg|data>|author-name>>><style-with|src-compact|none|<doc-footnote-ref|<unquote|<quasi|<doc-author-data-note|<unquote*|<quote-arg|data>>>>>>>>>

        <unquote*|<select|<quote-arg|data>|author-address>>

        <unquote*|<select|<quote-arg|data>|author-email>>

        <unquote*|<select|<quote-arg|data>|author-homepage>>
      </quasi>
    </style-with>
  </macro>>

  <assign|doc-author-data|<\xmacro|data>
    <\style-with|src-compact|none>
      <\quasi>
        <\surround|<vspace*|1fn>|<vspace|1.5fn>>
          <\with|par-par-sep|0fn>
            <doc-author-produce|<unquote|<quote-arg|data>>>
          </with>
        </surround>
      </quasi>
    </style-with>
  </xmacro>>

  <assign|doc-author-data-bis|<macro|body|<quasi|<style-with|src-compact|none|<doc-author-note|<unquote|<quasi|<doc-author-data-note|<unquote*|<quote-arg|body>>>>>>>>>>

  <\active*>
    <\src-comment>
      Documents with several authors.
    </src-comment>
  </active*>

  <assign|doc-authors-data*|<macro|x|<style-with|src-compact|none|<line-break><tabular*|<tformat|<cwith|1|1|1|1|cell-width|0.8par>|<cwith|1|1|1|1|cell-hmode|min>|<cwith|1|1|1|1|cell-hyphen|t>|<twith|table-valign|T>|<cwith|1|1|1|1|cell-lsep|1fn>|<cwith|1|1|1|1|cell-rsep|1fn>|<cwith|1|1|1|1|cell-bsep|0.75fn>|<cwith|1|1|1|1|cell-tsep|0.75fn>|<table|<row|<\cell>
    <with|par-mode|center|par-line-sep|0fn|font-size|0.92|<style-with|src-compact|none|<arg|x>>>
  </cell>>>>>>>>

  <assign|doc-authors-data|<xmacro|x|<style-with|src-compact|none|<with|par-mode|center|doc-author-by|<macro|body|<with|font-shape|small-caps|<arg|body>>>|<map-args|doc-authors-data*|concat|x>>>>>

  <assign|doc-authors-data-bis|<xmacro|x|<style-with|src-compact|none|<quasi|<space|0spc><unquote*|<map|doc-author-data-bis|<quote-arg|x>>>>>>>

  \;
</body>

<\initial>
  <\collection>
    <associate|preamble|true>
  </collection>
</initial>