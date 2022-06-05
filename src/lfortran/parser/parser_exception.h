#ifndef LCOMPILERS_PARSER_PARSER_EXCEPTION_H
#define LCOMPILERS_PARSER_PARSER_EXCEPTION_H

#include <libasr/exception.h>

namespace LCompilers {

namespace parser_local {

    // Local exceptions that are used to terminate the parser.
    // It is not propagated outside.
    // This file is included in parser.tab.cc (via semantics.h)
    // And in parser.cpp. Nowhere else.

    class TokenizerError
    {
    public:
        diag::Diagnostic d;
    public:
        TokenizerError(const std::string &msg, const Location &loc)
            : d{diag::Diagnostic(msg, diag::Level::Error, diag::Stage::Tokenizer, {
                diag::Label("", {loc})
            })}
        { }

        TokenizerError(const diag::Diagnostic &d) : d{d} { }
    };

    class ParserError
    {
    public:
        LCompilers::diag::Diagnostic d;
    public:
        ParserError(const std::string &msg, const LCompilers::Location &loc)
            : d{diag::Diagnostic(msg, diag::Level::Error, diag::Stage::Parser, {
                    diag::Label("", {loc})
                })}
        { }

        ParserError(const std::string &msg)
            : d{diag::Diagnostic(msg, diag::Level::Error, diag::Stage::Parser)}
        { }
    };

}
}


#endif
