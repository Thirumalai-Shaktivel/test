%token id
%token lparen "("
%token rparen ")"
%token plus "+"
%token star "*"

%%

E : T Ep ;

Ep
    : "+" T Ep
    | %empty
    ;

T : F Tp ;

Tp
    : "*" F Tp
    | %empty
    ;

F
    : "(" E ")"
    | id
    ;
