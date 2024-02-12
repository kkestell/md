# `md`

Quickly converts a simplified dialect of Markdown to HTML.

## Features

* Minimal dependencies
* Reasonably fast (~2 million lines per second on an i7-10700K)
* No runtime memory allocation
* UTF-8 support
* Simplified Markdown dialect
  * Headers
  * Paragraphs
  * Inline formatting: bold, italic, code
  * Hyperlinks
  * Ordered and unordered lists
  * Multi-line blockquotes
  * Code blocks
  * Tables
  * Horizontal rules
  * Embedded HTML

## Usage

```
md [-i input_file] [-o output_file]
```

If `-i` is not specified, `md` reads from `stdin`. If `-o` is not specified, `md` writes to `stdout`.

## Example

### Markdown

~~~
# Header 1

## Header 2

### Header 3

#### Header 4

##### Header 5

###### Header 6

Paragraphs are *separated by a _blank_ line*.
Single `newlines are treated as line breaks`. Separate ![https://www.google.com/](block elements) with a blank line.

* This is a list item
* This is another list item

### Tables

| Name | Price |
|------|-------|
| A    | 1     |
| B    | 2     |
| C    | 3     |

## UTF-8

∮ E⋅da = Q,  n → ∞, ∑ f(i) = ∏ g(i), ∀x∈ℝ: ⌈x⌉ = −⌊−x⌋, α ∧ ¬β = ¬(¬α ∨ β),

ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ, ⊥ < a ≠ b ≡ c ≤ d ≪ ⊤ ⇒ (A ⇔ B),

2H₂ + O₂ ⇌ 2H₂O, R = 4.7 kΩ, ⌀ 200 mm

((V⍳V)=⍳⍴V)/V←,V ←⍳→⍴∆∇⊃‾⍎⍕⌈

### Multibyte Characters

中英文均可混在⼀起 한국어조차도 가능합니다.

> “The programmer, like the poet, works only slightly removed from pure thought-stuff. He builds his castles in the air, from air, creating by exertion of the imagination. Few media of creation are so flexible, so easy to polish and rework, so readily capable of realizing grand conceptual structures.”
> ― Frederick P. Brooks

1. This is a numbered list item
2. This is another numbered list item

```
def hello_world():
    print("Hello World!")
```

<table border="1">
    <tr>
        <td>Hello</td>
        <td>World</td>
    </tr>
</table>
~~~

### HTML

```
<h1>Header 1</h1>
<h2>Header 2</h2>
<h3>Header 3</h3>
<h4>Header 4</h4>
<h5>Header 5</h5>
<h6>Header 6</h6>
<p>Paragraphs are <b>separated by a <i>blank</i> line</b>.<br>Single <code>newlines are treated as line breaks</code>. Separate <a href="https://www.google.com/">block elements</a> with a blank line.</p>
<ul>
<li>This is a list item</li>
<li>This is another list item</li>
</ul>
<h3>Tables</h3>
<table>
<tr><td>Name</td><td>Price</td></tr>
<tr><td>A</td><td>1</td></tr>
<tr><td>B</td><td>2</td></tr>
<tr><td>C</td><td>3</td></tr>
</table>
<h2>UTF-8</h2>
<p>∮ E⋅da = Q,  n → ∞, ∑ f(i) = ∏ g(i), ∀x∈ℝ: ⌈x⌉ = −⌊−x⌋, α ∧ ¬β = ¬(¬α ∨ β),</p>
<p>ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ, ⊥ < a ≠ b ≡ c ≤ d ≪ ⊤ ⇒ (A ⇔ B),</p>
<p>2H₂ + O₂ ⇌ 2H₂O, R = 4.7 kΩ, ⌀ 200 mm</p>
<p>((V⍳V)=⍳⍴V)/V←,V ←⍳→⍴∆∇⊃‾⍎⍕⌈</p>
<h3>Multibyte Characters</h3>
<p>中英文均可混在⼀起 한국어조차도 가능합니다.</p>
<blockquote>“The programmer, like the poet, works only slightly removed from pure thought-stuff. He builds his castles in the air, from air, creating by exertion of the imagination. Few media of creation are so flexible, so easy to polish and rework, so readily capable of realizing grand conceptual structures.”<br>― Frederick P. Brooks</blockquote>
<ol>
<li>This is a numbered list item</li>
<li>This is another numbered list item</li>
</ol>
<pre><code>def hello_world():
    print("Hello World!")
</code></pre>
<table border="1">
    <tr>
        <td>Hello</td>
        <td>World</td>
    </tr>
</table>
```
