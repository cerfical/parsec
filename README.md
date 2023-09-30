# parsec - Parser Compiler

Simple command line tool for generating lexical/syntax analysers from corresponding grammar descriptions.

Currently, only the generation of lexical analyzers is supported.

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

## Lexer specification

### Basics

The *name=pattern* pairs enclosed in a `tokens` block define tokens for the generated lexer:

```
tokens {
    open-paren = "\(";
    close-paren = "\)";
    letter = "[a-zA-Z]";
    ws = "[\r\n ]+";
}
```

The special token `ws` resets lexical analysis process and can be used to skip whitespace characters from the input before parsing the token.

### Regular expressions

Token patterns support a subset of the most common regular expressions operations, including:
 - Alternation, concatenation, grouping: `(a|b)c`
 - Repetition operators: `a*b+c?`
 - Character sets: `[abc]`
 - Character ranges: `[A-Za-z]`
 - Empty regexes: `[]` or `()`

Special characters used by the regex syntax can be escaped with a backslash (`\`) to take the symbol literally.

For convenience, additional escape sequences are available, such as `\n` or `\t`.
Also, an arbitrary character literal may be specified using its hexadecimal code, e.g. `\xff`.

### Conflict resolution

Tokens with the same name are merged together with `|` (alternation).

If multiple patterns are matched, the pattern mentioned earlier in the specification takes precedence.
