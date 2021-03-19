#include "pl0.h"

unsigned char *cur;
unsigned char *mar;
unsigned char *ctxmar;

void nextsym(void)
{
    for (;;) {
        /*
        Re2c has an excellent documentation at:

        https://re2c.org/manual/manual_c.html

        The first paragraph there explains the basics:

        * If multiple rules match, the longest match takes precedence
        * If multiple rules match the same string, the earlier rule takes
          precedence
        * Default rule `*` should always be defined, it has the lowest priority
          regardless of its place and matches any code unit

        See the manual for more details.
        */
        /*!re2c
            re2c:define:YYCURSOR = cur;
            re2c:define:YYMARKER = mar;
            re2c:define:YYCTXMARKER = ctxmar;
            re2c:yyfill:enable = 0;
            re2c:define:YYCTYPE = "unsigned char";

            end = "\x00";
            whitespace = [ \t\v\r]+;
            newline = "\n";
            digit = [0-9];
            char =  [a-zA-Z_];
            name = char (char | digit)*;
            integer = digit+;

            * { error("Token not recognized"); }
            end { error("End of file reached"); }
            whitespace { continue; }

            // Keywords
            'call' { sym = callsym; return; }
            'begin' { sym = beginsym; return; }
            'end' { sym = endsym; return; }
            'if' { sym = ifsym; return; }
            'while' { sym = whilesym; return; }
            'then' { sym = thensym; return; }
            'do' { sym = dosym; return; }
            'const' { sym = constsym; return; }
            'var' { sym = varsym; return; }
            'procedure' { sym = procsym; return; }
            'odd' { sym = oddsym; return; }

            newline { continue; }

            // Single character symbols
            "(" { sym = lparen; return; }
            ")" { sym = rparen; return; }
            "*" { sym = times; return; }
            "/" { sym = slash; return; }
            "+" { sym = plus; return; }
            "-" { sym = minus; return; }

            ";" { sym = semicolon; return; }
            ":=" { sym = becomes; return; }
            "," { sym = comma; return; }
            "." { sym = period; return; }

            // Relational operators
            "=" { sym = eql; return; }
            "#" { sym = neq; return; }
            "<"  { sym = lss; return; }
            "<=" { sym = leq; return; }
            ">"  { sym = gtr; return; }
            ">=" { sym = geq; return; }

            integer { sym = number; return; }

            name { sym = ident; return; }
        */
    }
}
