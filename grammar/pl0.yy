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

%%

program : block "." ;

block : const_opt var_opt procedure_star statement ;

procedure_star
    : "procedure" ident ";" block ";" procedure_star
    | %empty
    ;

var_opt
    : "var" ident_plus ";"
    | %empty
    ;

const_opt
    : "const" const_decl_plus ";"
    | %empty
    ;

const_decl_plus
    : const_decl "," const_decl_plus
    | const_decl
    ;

const_decl : ident "=" number ;

ident_plus
    : ident "," ident_plus
    | ident
    ;

statement_plus
    : statement ";" statement_plus
    | statement
    ;

statement
    : ident ":=" expression
    | "call" ident
    | "begin" statement_plus "end"
    | "if" condition "then" statement
    | "while" condition "do" statement
    ;

condition
    : "odd" expression
    | expression "=" expression
    | expression "#" expression
    | expression "<" expression
    | expression "<=" expression
    | expression ">" expression
    | expression ">=" expression
    ;

expression
    : term_plus
    | "+" term_plus
    | "-" term_plus
    ;

term_plus
    : term "+" term_plus
    | term "-" term_plus
    | term
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
