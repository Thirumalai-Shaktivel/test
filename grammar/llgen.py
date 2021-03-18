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
        i = 0
        while True:
            i += 1
            if i > 3: break
            self.line = self.file.readline()
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

p = Parser()
ast = p.parse_file(sys.argv[1])
print(ast)
