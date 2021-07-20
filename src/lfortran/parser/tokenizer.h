#ifndef LFORTRAN_SRC_PARSER_TOKENIZER_H
#define LFORTRAN_SRC_PARSER_TOKENIZER_H

#include <lfortran/exception.h>
#include <lfortran/parser/parser_stype.h>

namespace LFortran
{

class Tokenizer
{
public:
    unsigned char *start;
    unsigned char *cur;
    unsigned char *tok;

    int last_token=-1;

    std::vector<uint64_t> label_do_stack = {0};
    bool next_continue_is_enddo=false;

public:
    // Set the string to tokenize. The caller must ensure `str` will stay valid
    // as long as `lex` is being called.
    void set_string(const std::string &str);

    // Get next token. Token ID is returned as function result, the semantic
    // value is put into `yylval`.
    int lex(Allocator &al, YYSTYPE &yylval, Location &loc);

    // Return the current token as std::string
    std::string token() const
    {
        return std::string((char *)tok, cur - tok);
    }

    // Return the current token as YYSTYPE::Str
    void token(Str &s) const
    {
        s.p = (char*) tok;
        s.n = cur-tok;
    }

    // Return the current token as YYSTYPE::Str, strips first and last character
    void token_str(Str &s) const
    {
        s.p = (char*) tok + 1;
        s.n = cur-tok-2;
    }

    // Return the current token's location
    void token_loc(Location &loc)
    {
        loc.first = tok-start;
        loc.last = cur-start;
    }
};

bool lex_int(const unsigned char *s, const unsigned char *e, uint64_t &u,
    Str &suffix);

} // namespace LFortran

#endif
