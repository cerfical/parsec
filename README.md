# parsec - Parser Compiler

Simple command line tool for generating lexical/syntax analysers from corresponding grammar descriptions.



## Usage

Run `parsec --help` for usage information:

```console
> parsec --help
Usage:
  parsec <input-file> [<output-file>]
  parsec [options]

Options:
  --version                 print version information
  --help                    produce help message

  -o [ --output-file ] arg  output file
  -i [ --input-file ] arg   input file
```



## Syntax

To produce source code for a parser, the syntax of a language, accepted by the parser, must be described using a **grammar**.
In short, a grammar is a set of **symbols** with specifications for each symbol on how that symbol is formed.

Symbols are just names, which in turn are case-insensitive and consist of words separated by `-` or `_` and containing only alphanumeric characters.
`-` and `_` are just delimiters and don't contribute to the name.
Thus, the names `add-op`, `AddOp`, `ADD_OP` are all refer to the same symbol.
The first word of any name must begin with a letter.

`root` is the **root** or **start symbol** of a grammar.



### Tokens Specification

Lexical analysis on some text is performed by breaking the text into a sequence of named **tokens**, or, more formally, **terminal symbols**.  
Partitioning of the input into these tokens is defined with **string patterns**.

The *name = pattern* pairs enclosed in a `tokens` block list and name such patterns.
Patterns are either **raw string literals**, enclosed in single quotes like this: `'class'`; or, for more elaborate scenarios, **regular expressions**: `"[a-z]*"`.

An example of this syntax would be:

```
tokens {
	ws = "[ \f\n\r\t\v]+";
	
	number = "[1-9][0-9]*";
	ident = "[A-Za-z_][A-Za-z_0-9]*";

	open-paren = '(';
	close-paren = ')';

	add-op = '+';
	sub-op = '-';
	
	mul-op = '*';
	div-op = '/';
}
```

A few pattern names are treated specially to provide useful functionality, such as the end-of-file detection or automatic handling of whitespace characters.
Such tokens would be `eof` and `ws` respectively:

 - when `ws` token is encountered, the analysis process is reset and token parse starts over,
 - when the end-of-file is reached, `eof` token is automatically spawned.



### Regular Expressions

Regex patterns support a subset of the most common regular expressions operations, including:

 - concatenation: `abc`,
 - alternation: `a|b|c`,
 - grouping: `(a)`,
 - Kleene plus: `a+`,
 - Kleene star: `a*`,
 - optional expression: `a?`,
 - character sets: `[abc]`,
 - character ranges: `[A-Za-z]`,
 - empty expressions: `[]` or `()`.

Metacharacters used by the regex syntax itself can be escaped with a backslash (`\`) to take the character literally.

For convenience, some standard `C` escape sequences are also available, such as `\n`, `\t`, or `\xff`.



### Rules

Similar syntax is used to formalize the syntactic structure of a language.

The only differences are:
 - slightly limited set of available operators: only `|`, `*`, `()`, `+` and `?` are supported,
 - use of symbols instead of character atoms,
 - absence of quotes.

```
rules {
  root = expr;

  expr = ( expr ( '+' | '-' ) )? term;
  term = ( term ( '*' | '/' ) )? factor;

  factor = ident | number;
}
```

As an additional convinience, tokens alternatively can be referenced by their defining pattern instead of the name.
If a token with the pattern doesn't exist, it will be created automatically with a generated name in the form `UnnamedN_`, where `N` is the relative numeric identifier for the generated token.



### Conflict Resolution

Tokens and rules with the same name are merged together with `|` (alternation).

If some patterns/rules conflict the pattern/rule mentioned earlier takes precedence.



## Generator Output

The generated lexer is represented in the form of a class under the name `Lexer`, which takes as the input characters from standard `std::istream` streams and outputs recognized tokens as objects of a `Token` class.
To differentiate lexer-emitted tokens, a `TokenKinds` enumeration is provided, populated with token names taken from the grammar.

Similarly, a `Parser` class drives syntax analysis.
The parser houses virtual methods of the form `void on<RuleName>()` for each rule symbol defined.
They are called by the parser when some part of the input matches a rule with the corresponding name.

All produced source code is placed inside a single header file.
First include the `<parsec/deps.hpp>` header, which lists the generated source code dependencies, and then include the output file to make use of it.
Furthermore, linkage to the `parsec-lib` library target may be required.
