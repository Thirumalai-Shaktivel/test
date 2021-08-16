Example
```
./llgen.py pl0.yy
./pl0 example3.pl0
```

The classic expression grammar is left recursive:

    expr
        = expr "+" term
        | term
    term
        = term "*" factor
        | factor
    factor
        = number
        | "(" expr ")"

So it can only be used with LR parsers. The "+" operator can be either left or
right associative, the LR parser can be generated for both options.

To use it in LL parses, we have to rewrite it to be right recursive (EBNF)

    expr
        = term ("+" term)*
    term
        = factor ("*" factor)*
    factor
        = number
        | "(" expr ")"

Which can be equivalently written without the grouping operator `(...)*`
as (BNF):

    expr
        = term expr2
    expr2                      // equivalent to: ("+" term)*
        = "+" term expr2
        | %empty
    term
        = factor term2
    term2                      // equivalent to: ("*" factor)*
        = "*" factor term2
        | %empty
    factor
        = number
        | "(" expr ")"

Again the LL parser can be generated for both left and right associative "+".

Another way to write the LL grammar is (BNF):

    expr
        = term "+" expr
        | term
    term
        = factor "*" term
        | factor
    factor
        = number
        | "(" expr ")"

Let's just show how the code would look like for the first part: expr and expr2.

The grammar rule:

    expr = term ("+" term)*;

is equivalent to:

    expr = term expr2;

    expr2
        = "+" term expr2
        | %empty
        ;

or

    expr
        = term "+" expr
        | term

If `+` is left associative (such as + or -), then the generator must generate
the following code for the first rule:

    ast expr() {
        ast t = term();
        while (token == "+") {
            accept("+");
            t = ACTION(t, term());
        }
        return t
    }

And the following code for the second rule:

    ast expr() {
        ast t;
        return expr2(term());
    }

    // expr2 is right recursive, we pass all arguments to the left of it
    // as an argument, that way it can call an action on it when it encounters
    // an operator
    ast expr2(term t) {
        if (token == "+") {
            accept("+");
            return expr2(ACTION(t, term()));
        } else {
            return t;
        }
    }

and the following code for the third rule:

    ? The only way to do it seems like for the second rule, which seems
      complicated for the generator to do (it would have to unwrap the `expr`
      rule into `term` and the rest, effectively rewriting to the second rule).

Both parse 1+2+3 as (1+2)+3.

If `+` is right associative (such as `**`), then the generator must generate
the following code for the first rule:

    ast expr() {
        std::vector<ast> stack;
        stack.push_back(term());
        while (token == "+") {
            accept("+");
            stack.push_back(term())
        }
        term t = stack.pop();
        while (stack.size() > 0);
            t = ACTION(stack.pop(), t);
        }
        return t;
    }

And the following code for the second rule:

    ast expr() {
        ast t;
        return expr2(term());
    }

    ast expr2(term t) {
        if (token == "+") {
            accept("+");
            return ACTION(t, expr2(term()));
        } else {
            return t;
        }
    }

and for the third rule

    ast expr() {
        ast t = term();
        if (token == "+") {
            accept("+");
            return ACTION(t, expr2());
        } else {
            return t;
        }
    }

All three parse 1+2+3 as 1+(2+3).

Since most operators are left associative, we can make a convention that the
first rule will create left associative code, and the third rule creates right
associative code. That way the generated code does not need to pass anything as
arguments. The second rule can be used for both left and right associative
operators as easily. There, if a rule like `expr = term expr2` only has one
branch, it will collect all the items and pass them as an argument to the last
rule (`expr2`) and the result of `expr` is whatever `expr2` returns. No action.

Actions would be written as:

    expr
        = term (              // Always generates left associative code
            ( "+"             { $$ = PLUS($1, $3)  }
            | "-"             { $$ = MINUS($1, $3) }
            ) term)*
    term
        = power (
            ("*"              { $$ = MUL($1, $3) }
            |"/"              { $$ = DIV($1, $3) }
            ) power)*
    power                     // Always generates right associative code:
        = factor "**" power   { $$ = POW($1, $3) }
        | factor              { $$ = $1 }
    factor
        = number              { $$ = $1 }
        | "(" expr ")"        { $$ = $2 }

Or:

    # SET +,-,*,/ as left associative
    # SET ** as right associative

    expr
        = term expr2           // No action -- returns the result of expr2
    expr2
        = "+" term expr2       { $$ = PLUS($0, $2) }
        = "-" term expr2       { $$ = MINUS($0, $2) }
        | %empty               { $$ = $0 }
    term
        = power term2          // No action
    term2
        = "*" power term2      { $$ = MUL($0, $2) }
        | "/" power term2      { $$ = DIV($0, $2) }
        | %empty               { $$ = $0 }
    power
        = factor power2        // No action
    power2
        = "**" factor power2   { $$ = POW($0, $2) }
        | %empty               { $$ = $0 }
    factor
        = number               { $$ = $1 }
        | "(" expr ")"         { $$ = $2 }
