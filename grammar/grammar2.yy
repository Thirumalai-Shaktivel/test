%token a
%token b
%token d
%token g
%token h

%%

S
    : A C B
    | C b b
    | B a
    ;

A
    : d a
    | B C
    ;

B
    : g
    | %empty
    ;

C
    : h
    | %empty
    ;
