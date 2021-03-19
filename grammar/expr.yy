%token ident
%token number
%token lparen "("
%token rparen ")"
%token times "*"
%token slash "/"
%token plus "+"
%token minus "-"

%%

expression
    : "+" term_plus
    | "-" term_plus
    | term_plus
    ;

term_plus : term term_plus_rest ;

term_plus_rest
    : "+" term_plus
    | "-" term_plus
    | %empty
    ;

term : factor term_rest ;

term_rest
    : "*" term
    | "/" term
    | %empty
    ;

factor
    : ident
    | number
    | "(" expression ")"
    ;
