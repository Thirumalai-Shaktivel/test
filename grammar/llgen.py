#!/usr/bin/env python3

from collections import namedtuple
import re
from struct import pack
import sys

TokenDefinition = namedtuple("TokenDefinition", ["name", "string"])
Rule = namedtuple("Rule", ["name", "alternatives"])
RuleAlt = namedtuple("RuleAlt", ["items"])
RHSString = namedtuple("RHSString", ["string"])
RHSName = namedtuple("RHSName", ["name"])
RHSEmpty = namedtuple("RHSEmpty", [])

ASRRule = namedtuple("ASRRule", ["name", "alternatives"])
ASRAlt = namedtuple("ASRAlt", ["items"])
ASRRuleRef = namedtuple("ASRRuleRef", ["name"])
ASRTokenRef = namedtuple("ASRTokenRef", ["name"])
ASREmpty = namedtuple("ASREmpty", [])


class Parser:

    def parse_file(self, filename):
        self.file = open(filename)
        token_defs = self.parse_token_definitions()

        self.line = self.file.readline()
        while not self.line.startswith(r"%%"):
            self.line = self.file.readline()

        rules = self.parse_rules()
        return token_defs, rules

    def parse_token_definitions(self):
        token_defs = []
        self.line = self.file.readline()
        while self.line.startswith(r"%token"):
            l = self.line.split()
            assert l[0] == r"%token";
            name = l[1]
            if len(l) >= 3:
                s = l[2]
                assert s[0] == '"'
                assert s[-1] == '"'
                string = s[1:-1]
            else:
                string = None
            token_defs.append(TokenDefinition(name, string))
            self.line = self.file.readline()
        return token_defs

    def parse_rules(self):
        rules = []
        while True:
            self.line = self.file.readline()
            if self.line == "":
                break
            while len(self.line.split()) == 0:
                self.line = self.file.readline()
            rules.append(self.parse_rule());
        return rules

    def parse_rule(self):
        t = []
        l = self.line.split()
        while ";" not in l:
            t.extend(l)
            self.line = self.file.readline()
            l = self.line.split()
        t.extend(l)
        name = t[0]
        assert t[1] == ":"
        assert t[-1] == ";"
        t = t[2:-1]
        rhs = []
        var = []
        for x in t:
            if x == "|":
                rhs.append(RuleAlt(var))
                var = []
            elif x == r"%empty":
                var.append(RHSEmpty())
            elif x[0] == '"':
                assert x[-1] == '"'
                var.append(RHSString(x[1:-1]))
            else:
                var.append(RHSName(x))
        rhs.append(RuleAlt(var))
        return Rule(name, rhs)

def ast_to_asr(ast):
    token_defs, ast_rules = ast

    tokens = []
    name2token = {}
    string2token = {}
    idx = 0

    for t in token_defs:
        assert t.name not in string2token
        name2token[t.name] = idx
        tokens.append(t.name)
        if t.string:
            assert t.string not in string2token
            string2token[t.string] = idx
        idx += 1

    rules = {}

    for rule in ast_rules:
        assert rule.name not in name2token
        assert rule.name not in rules
        rules[rule.name] = True

    for rule in ast_rules:
        alts = []
        for alt in rule.alternatives:
            tmp = []
            for item in alt.items:
                if isinstance(item, RHSName):
                    if item.name in rules:
                        tmp.append(ASRRuleRef(item.name))
                    elif item.name in tokens:
                        tmp.append(ASRTokenRef(item.name))
                    else:
                        assert False
                elif isinstance(item, RHSString):
                    assert item.string in string2token
                    tmp.append(ASRTokenRef(tokens[string2token[item.string]]))
                elif isinstance(item, RHSEmpty):
                    tmp.append(ASREmpty())
                else:
                    assert False
            alts.append(ASRAlt(tmp))
        rules[rule.name] = ASRRule(rule.name, alts)
    return tokens, name2token, rules

def print_alt(l):
    s = []
    for a in l:
        if isinstance(a, ASRRuleRef):
            s.append(a.name)
        elif isinstance(a, ASRTokenRef):
            s.append("TK_%s" % a.name)
        elif isinstance(a, ASREmpty):
            s.append("ε")
        else:
            assert False
    return " ".join(s)


def print_asr(asr):
    tokens, name2token, rules = asr
    for t in tokens:
        print("TOKEN: TK_%s = %d" % (t, name2token[t]))
    print()
    for r in rules:
        rule = rules[r]
        print("RULE:", rule.name)
        for alt in rule.alternatives:
            print("    | ", print_alt(alt.items))
        print()

def get_first_token(rules, t):
    if isinstance(t, ASRTokenRef):
        return t
    elif isinstance(t, ASRRuleRef):
        rule = rules[t.name]
        # TODO: go over alternatives and collect first tokens
        return "TODO_ALTS";
    else:
        assert False


def asr2c(asr):
    tokens, name2token, rules = asr
    s = ""
    s += "typedef enum {%s} Symbol;\n" % (", ".join(tokens))
    s += """\

Symbol sym;
void nextsym(void);
void error(const char msg[]);

int accept(Symbol s) {
    if (sym == s) {
        nextsym();
        return 1;
    }
    return 0;
}

int expect(Symbol s) {
    if (accept(s))
        return 1;
    error("expect: unexpected symbol");
    return 0;
}

"""
    for r in rules:
        rule = rules[r]
        s += "void %s() {\n" % rule.name
        if len(rule.alternatives) == 1:
            for item in rule.alternatives[0].items:
                s += "    "
                if isinstance(item, ASRRuleRef):
                    s += "%s();\n" % item.name
                elif isinstance(item, ASRTokenRef):
                    s += "expect(%s);\n" % item.name
                else:
                    assert False
        else:
            s += "    ";
            for alt in rule.alternatives:
                if isinstance(alt.items[0], ASREmpty):
                    s += "{\n";
                    s += "        // Empty\n";
                    s += "    }\n";
                    break
                if isinstance(alt.items[0], ASRTokenRef):
                    #first = get_first_token(rules, alt.items[0])
                    first = alt.items[0]
                    s += "if (accept(%s)) {\n" % first;
                    n0 = 1
                elif isinstance(alt.items[0], ASRRuleRef):
                    if alt is rule.alternatives[-1]:
                        s += "{\n"
                        n0 = 0
                    else:
                        raise Exception("Multiple expressions not implemented")
                else:
                    assert False
                for item in alt.items[n0:]:
                    s += "        "
                    if isinstance(item, ASRRuleRef):
                        s += "%s();\n" % item.name
                    elif isinstance(item, ASRTokenRef):
                        s += "expect(%s);\n" % item.name
                    else:
                        assert False
                if isinstance(alt.items[0], ASRRuleRef):
                    s += "    }";
                else:
                    s += "    } else ";
            if isinstance(rule.alternatives[-1].items[0], ASREmpty):
                pass
            elif isinstance(rule.alternatives[-1].items[0], ASRRuleRef):
                pass
            else:
                s += "{\n";
                s += '        error("%s: syntax error");\n' % rule.name;
                s += '        nextsym();\n';
                s += "    }\n";

        s += "}\n\n"
    return s

p = Parser()
ast = p.parse_file(sys.argv[1])
asr = ast_to_asr(ast)
#print_asr(asr)
c = asr2c(asr)
print(c)
