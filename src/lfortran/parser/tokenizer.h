#ifndef LFORTRAN_SRC_PARSER_TOKENIZER_H
#define LFORTRAN_SRC_PARSER_TOKENIZER_H

#include <lfortran/exception.h>
#include <lfortran/parser/parser_stype.h>

namespace LFortran
{

class Tokenizer
{
public:
    unsigned char *cur;
    unsigned char *tok;
    unsigned char *cur_line;
    unsigned int line_num;

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
    // De-escapes the string
    void token_str(Allocator &al, Str &s) const
    {
        char quote = *tok;
        s.n = cur-tok-2;
        //s.p = (char*) tok + 1;
        s.p = al.allocate<char>(s.n);
        size_t i = 0, j = 0;
        while (j < s.n) {
            s.p[i] = *(tok+j+1);
            if (*(tok+j+1) == quote && *(tok+j+2) == quote) j++;
            i++;
            j++;
        }
    }

    // Return the current token's location
    void token_loc(Location &loc)
    {
        loc.first_line = line_num;
        loc.last_line = line_num;
        loc.first_column = tok-cur_line+1;
        loc.last_column = cur-cur_line;
    }
};

bool lex_int(const unsigned char *s, const unsigned char *e, uint64_t &u,
    Str &suffix);

} // namespace LFortran

#endif
