%token ident
%token number
%token lparen "("
%token rparen ")"
%token times "*"
%token slash "/"
%token plus "+"
%token minus "-"
%token eql "="
%token neq "#"
%token lss "<"
%token leq "<="
%token gtr ">"
%token geq ">="
%token callsym "call"
%token beginsym "begin"
%token semicolon ";"
%token endsym "end"
%token ifsym "if"
%token whilesym "while"
%token becomes ":="
%token thensym "then"
%token dosym "do"
%token constsym "const"
%token comma ","
%token varsym "var"
%token procsym "procedure"
%token period "."
%token oddsym "odd"

program = block "." ;

block
    = ["const" ident "=" num {"," ident "=" num} ";"]
        ["var" ident {"," ident} ";"]
        {"procedure" ident ";" block ";"} statement ;

statement
    = ident ":=" expression
    | "call" ident
    | "begin" statement {";" statement } "end"
    | "if" condition "then" statement
    | "while" condition "do" statement
    ;

condition
    = "odd" expression
    | expression ("="|"#"|"<"|"<="|">"|">=") expression
    ;

expression = ["+"|"-"] term {("+"|"-") term} ;

term = factor {("*"|"/") factor} ;

factor
    = ident
    | number
    | "(" expression ")"
    ;
