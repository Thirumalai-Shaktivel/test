"""
Generate C++ AST node definitions from an ASDL description.
"""

import sys
import os
import asdl


class ASDLVisitor(asdl.VisitorBase):

    def __init__(self, stream, data):
        super(ASDLVisitor, self).__init__()
        self.stream = stream
        self.data = data

    def visitModule(self, mod, *args):
        for df in mod.dfns:
            self.visit(df, *args)

    def visitSum(self, sum, *args):
        for tp in sum.types:
            self.visit(tp, *args)

    def visitType(self, tp, *args):
        self.visit(tp.value, *args)

    def visitProduct(self, prod, *args):
        for field in prod.fields:
            self.visit(field, *args)

    def visitConstructor(self, cons, *args):
        for field in cons.fields:
            self.visit(field, *args)

    def visitField(self, field, *args):
        pass

    def emit(self, line, level=0):
        indent = "    "*level
        self.stream.write(indent + line + "\n")


def is_simple_sum(sum):
    """
    Returns true if `sum` is a simple sum.

    Example of a simple sum:

        boolop = And | Or

    Example of not a simple sum:

        type
            = Integer(int kind)
            | Real(int kind)

    """
    assert isinstance(sum, asdl.Sum)
    for constructor in sum.types:
        if constructor.fields:
            return False
    return True

def attr_to_args(attrs):
    args = []
    for attr in attrs:
        kw = ""
        if attr.type == "int":
            if attr.name in ["lineno", "col_offset"]:
                kw = "=1"
            else:
                kw = "=0"
        elif attr.type in ["string", "identifier"]:
            kw = '=None'
        elif attr.seq:
            kw = "=[]"
        else:
            kw = "=None"
        args.append(attr.name + kw)
    return ", ".join(args)

simple_sums = []
sums = []
products = []
subs = {}

def convert_type(asdl_type, seq, opt, mod_name):
    if asdl_type in simple_sums:
        type_ = asdl_type + "Type"
        assert not seq
    elif asdl_type == "string":
        type_ = "char*"
        assert not seq
    elif asdl_type == "identifier":
        type_ = "char*"
        if seq:
            # List of strings is **
            type_ = type_ + "*"
    elif asdl_type == "bool":
        type_ = "bool"
        assert not seq
    elif asdl_type == "node":
        type_ = "%s_t*" % mod_name
        if seq:
            type_ = type_ + "*"
    elif asdl_type == "symbol_table":
        type_ = "SymbolTable*"
    elif asdl_type == "int":
        type_ = "int64_t"
        assert not seq
    else:
        type_ = asdl_type + "_t"
        if asdl_type in products:
            # Product type
            # Not a pointer by default
            if seq or opt:
                # Sequence or an optional argument must be a pointer
                type_ = type_ + "*"
        else:
            # Sum type
            # Sum type is polymorphic, must be a pointer
            type_ = type_ + "*"
            if seq:
                # Sequence of polymorphic types must be a double pointer
                type_ = type_ + "*"
    return type_

class CollectVisitor(ASDLVisitor):

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitSum(self, sum, base):
        if not is_simple_sum(sum):
            sums.append(base);

class ASTNodeVisitor0(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Forward declarations")
        self.emit("")
        super(ASTNodeVisitor0, self).visitModule(mod)

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitSum(self, sum, base):
        if is_simple_sum(sum):
            simple_sums.append(base)
            self.emit("enum %sType // Simple Sum" % base)
            self.emit("{ // Types");
            s = [cons.name for cons in sum.types]
            self.emit(    ", ".join(s), 1)
            self.emit("};");
        else:
            self.emit("struct %s_t; // Sum" % base)

    def visitProduct(self, product, name):
        products.append(name)
        self.emit("struct %s_t; // Product" % name)


class ASTNodeVisitor1(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Products declarations")
        self.emit("")
        self.mod = mod
        super(ASTNodeVisitor1, self).visitModule(mod)

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitProduct(self, product, name):
        self.emit("struct %s_t // Product" % name)
        self.emit("{");
        self.emit(    "Location loc;", 1);
        for f in product.fields:
            type_ = convert_type(f.type, f.seq, f.opt, self.mod.name.lower())
            if f.seq:
                seq = " size_t n_%s; // Sequence" % f.name
            else:
                seq = ""
            self.emit("%s m_%s;%s" % (type_, f.name, seq), 1)
        self.emit("};");


class ASTNodeVisitor(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Sums declarations")
        self.emit("")
        self.mod = mod
        super(ASTNodeVisitor, self).visitModule(mod)

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitSum(self, sum, base):
        if not is_simple_sum(sum):
            self.emit("enum %sType // Types" % base)
            self.emit("{");
            s = [cons.name for cons in sum.types]
            self.emit(    ", ".join(s), 1)
            self.emit("};");
            self.emit("")
            self.emit("struct %s_t // Sum" % base)
            self.emit("{")
            mod = subs["mod"]
            self.emit(    "const static %sType class_type = %sType::%s;" \
                    % (mod, mod, base), 1)
            self.emit(    "%(mod)s_t base;" % subs, 1)
            self.emit(    "%sType type;" % base, 1)
            self.emit("};")
            self.emit("")
            for cons in sum.types:
                self.visit(cons, base, sum.attributes)
            self.emit("")
            self.emit("")

    def visitConstructor(self, cons, base, extra_attributes):
        self.emit("struct %s_t // Constructor" % cons.name, 1)
        self.emit("{", 1);
        self.emit(    "const static %sType class_type = %sType::%s;" \
                % (base, base, cons.name), 2)
        self.emit(    "typedef %s_t parent_type;" % base, 2)
        self.emit(    "%s_t base;" % base, 2);
        args = ["Allocator &al", "const Location &a_loc"]
        lines = []
        for f in cons.fields:
            type_ = convert_type(f.type, f.seq, f.opt, self.mod.name.lower())
            if f.seq:
                seq = " size_t n_%s; // Sequence" % f.name
            else:
                seq = ""
            self.emit("%s m_%s;%s" % (type_, f.name, seq), 2)
            args.append("%s a_%s" % (type_, f.name))
            lines.append("n->m_%s = a_%s;" % (f.name, f.name))
            if f.seq:
                args.append("size_t n_%s" % (f.name))
                lines.append("n->n_%s = n_%s;" % (f.name, f.name))
        self.emit("};", 1)
        self.emit("static inline %s_t* make_%s_t(%s) {" % (subs["mod"],
            cons.name, ", ".join(args)), 1)
        self.emit(    "%s_t *n;" % cons.name, 2)
        self.emit(    "n = al.make_new<%s_t>();" % cons.name, 2)
        self.emit(    "n->base.type = %sType::%s;" % (base, cons.name), 2)
        self.emit(    "n->base.base.type = %sType::%s;" % (subs["mod"],
            base), 2)
        self.emit(    "n->base.base.loc = a_loc;", 2)
        for line in lines:
            self.emit(line, 2)
        self.emit(    "return (%(mod)s_t*)n;" % subs, 2)
        self.emit("}", 1)
        self.emit("")

class ASTVisitorVisitor1(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Visitor functions")
        self.emit("")
        super(ASTVisitorVisitor1, self).visitModule(mod)

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitSum(self, sum, base):
        if not is_simple_sum(sum):
            self.emit("template <class Visitor>")
            self.emit("static void visit_%s_t(const %s_t &x, Visitor &v) {" \
                    % (base, base))
            self.emit(    "LFORTRAN_ASSERT(x.base.type == %sType::%s)" \
                    % (subs["mod"], base), 1)
            self.emit(    "switch (x.type) {", 1)
            for type_ in sum.types:
                self.emit("        case %sType::%s: { v.visit_%s((const %s_t &)x);"
                    " return; }" % (base, type_.name, type_.name, type_.name))
            self.emit("    }")
            self.emit("}")
            self.emit("")

class ASTVisitorVisitor1b(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("template <class Visitor>")
        self.emit("static void visit_%(mod)s_t(const %(mod)s_t &x, Visitor &v) {" % subs)
        self.emit("    switch (x.type) {")
        for type_ in sums:
            self.emit("        case %sType::%s: { v.visit_%s((const %s_t &)x);"
                " return; }" % (subs["mod"], type_, type_, type_))
        self.emit("    }")
        self.emit("}")
        self.emit("")

class ASTVisitorVisitor2(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Visitor base class")
        self.emit("")
        self.emit("template <class Derived>")
        self.emit("class BaseVisitor")
        self.emit("{")
        self.emit("private:")
        self.emit("    Derived& self() { return static_cast<Derived&>(*this); }")
        self.emit("public:")
        self.emit(    "void visit_%(mod)s(const %(mod)s_t &b) { visit_%(mod)s_t(b, self()); }" % subs, 1)
        super(ASTVisitorVisitor2, self).visitModule(mod)
        self.emit("};")

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitSum(self, sum, base):
        if not is_simple_sum(sum):
            self.emit("void visit_%s(const %s_t &b) { visit_%s_t(b, self()); }"\
                    % (base, base, base), 1)
            for type_ in sum.types:
                self.emit("""void visit_%s(const %s_t & /* x */) { throw LFortran::LFortranException("visit_%s() not implemented"); }""" \
                        % (type_.name, type_.name, type_.name), 2)


class ASTWalkVisitorVisitor(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Walk Visitor base class")
        self.emit("")
        self.emit("template <class Derived>")
        self.emit("class BaseWalkVisitor : public BaseVisitor<Derived>")
        self.emit("{")
        self.emit("private:")
        self.emit("    Derived& self() { return static_cast<Derived&>(*this); }")
        self.emit("public:")
        super(ASTWalkVisitorVisitor, self).visitModule(mod)
        self.emit("};")

    def visitType(self, tp):
        if not (isinstance(tp.value, asdl.Sum) and
                is_simple_sum(tp.value)):
            super(ASTWalkVisitorVisitor, self).visitType(tp, tp.name)

    def visitProduct(self, prod, name):
        self.make_visitor(name, prod.fields)

    def visitConstructor(self, cons, _):
        self.make_visitor(cons.name, cons.fields)

    def make_visitor(self, name, fields):
        self.emit("void visit_%s(const %s_t &x) {" % (name, name), 1)
        self.used = False
        have_body = False
        for field in fields:
            self.visitField(field)
        if not self.used:
            # Note: a better solution would be to change `&x` to `& /* x */`
            # above, but we would need to change emit to return a string.
            self.emit("if ((bool&)x) { } // Suppress unused warning", 2)
        self.emit("}", 1)

    def visitField(self, field):
        if (field.type not in asdl.builtin_types and
            field.type not in self.data.simple_types):
            level = 2
            if field.type in products:
                if field.opt:
                    template = "self().visit_%s(*x.m_%s);" % (field.type, field.name)
                else:
                    template = "self().visit_%s(x.m_%s);" % (field.type, field.name)
            else:
                template = "self().visit_%s(*x.m_%s);" % (field.type, field.name)
            self.used = True
            if field.seq:
                self.emit("for (size_t i=0; i<x.n_%s; i++) {" % field.name, level)
                if field.type in products:
                    self.emit("    self().visit_%s(x.m_%s[i]);" % (field.type, field.name), level)
                else:
                    self.emit("    self().visit_%s(*x.m_%s[i]);" % (field.type, field.name), level)
                self.emit("}", level)
                return
            elif field.opt:
                self.emit("if (x.m_%s)" % field.name, 2)
                level = 3
            self.emit(template, level)


class PickleVisitorVisitor(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Pickle Visitor base class")
        self.emit("")
        self.emit("template <class Derived>")
        self.emit("class PickleBaseVisitor : public BaseVisitor<Derived>")
        self.emit("{")
        self.emit("private:")
        self.emit(  "Derived& self() { return static_cast<Derived&>(*this); }", 1)
        self.emit("public:")
        self.emit(  "std::string s, indtd;", 1)
        self.emit(  "bool use_colors;", 1)
        self.emit(  "bool indent;", 1)
        self.emit(  "int indent_level = 0, indent_spaces = 3;", 1)
        self.emit("public:")
        self.emit(  "PickleBaseVisitor() : use_colors(false), indent(false) { s.reserve(100000); }", 1)
        self.emit(  "void inc_indent() {", 1)
        self.emit(      "indent_level++;", 2)
        self.emit(      "indtd = std::string(indent_level*indent_spaces, ' ');",2)
        self.emit(  "}",1)
        self.emit(  "void dec_indent() {", 1)
        self.emit(      "indent_level--;", 2)
        self.emit(      "indtd = std::string(indent_level*indent_spaces, ' ');",2)
        self.emit(  "}",1)
        self.mod = mod
        super(PickleVisitorVisitor, self).visitModule(mod)
        self.emit("};")

    def visitType(self, tp):
        super(PickleVisitorVisitor, self).visitType(tp, tp.name)

    def visitSum(self, sum, *args):
        assert isinstance(sum, asdl.Sum)
        if is_simple_sum(sum):
            name = args[0] + "Type"
            self.make_simple_sum_visitor(name, sum.types)
        else:
            for tp in sum.types:
                self.visit(tp, *args)

    def visitProduct(self, prod, name):
        self.make_visitor(name, prod.fields, False)

    def visitConstructor(self, cons, _):
        self.make_visitor(cons.name, cons.fields, True)

    def make_visitor(self, name, fields, cons):
        self.emit("void visit_%s(const %s_t &x) {" % (name, name), 1)
        self.emit(      'if(indent) {',2)
        self.emit(          's.append("\\n"+indtd);', 3)
        self.emit(          'inc_indent();',3)
        self.emit(      '}', 2)
        self.emit(      's.append("(");', 2)
        subs = {
            "Assignment": "=",
            "Associate": "=>",
        }
        if name in subs:
            name = subs[name]
        if cons:
            self.emit(    'if (use_colors) {', 2)
            self.emit(        's.append(color(style::bold));', 3)
            self.emit(        's.append(color(fg::magenta));', 3)
            self.emit(    '}', 2)
            self.emit(    's.append("%s");' % name, 2)
            self.emit(    'if (use_colors) {', 2)
            self.emit(        's.append(color(fg::reset));', 3)
            self.emit(        's.append(color(style::reset));', 3)
            self.emit(    '}', 2)
            if len(fields) > 0:
                self.emit(    's.append(" ");', 2)
        self.used = False
        for n, field in enumerate(fields):
            self.visitField(field, cons)
            if n < len(fields) - 1:
                self.emit(    's.append(" ");', 2)
        self.emit(    's.append(")");', 2)
        if not self.used:
            # Note: a better solution would be to change `&x` to `& /* x */`
            # above, but we would need to change emit to return a string.
            self.emit("if ((bool&)x) { } // Suppress unused warning", 2)
        self.emit("}", 1)

    def make_simple_sum_visitor(self, name, types):
        self.emit("void visit_%s(const %s &x) {" % (name, name), 1)
        self.emit(    'if (use_colors) {', 2)
        self.emit(        's.append(color(style::bold));', 3)
        self.emit(        's.append(color(fg::green));', 3)
        self.emit(    '}', 2)
        self.emit(    'switch (x) {', 2)
        for tp in types:
            self.emit(    'case (%s::%s) : {' % (name, tp.name), 3)
            self.emit(      'if(indent) {',4)
            self.emit(          'dec_indent();',5)
            self.emit(          's.append("\\n"+indtd);', 5)
            self.emit(      '}', 4)
            self.emit(      's.append("%s");' % (tp.name), 4)
            self.emit(     ' break; }',3)
        self.emit(    '}', 2)
        self.emit(    'if (use_colors) {', 2)
        self.emit(        's.append(color(fg::reset));', 3)
        self.emit(        's.append(color(style::reset));', 3)
        self.emit(    '}', 2)
        self.emit("}", 1)

    def visitField(self, field, cons):
        if (field.type not in asdl.builtin_types and
            field.type not in self.data.simple_types):
            self.used = True
            level = 2
            if field.type in products:
                if field.opt:
                    template = "self().visit_%s(*x.m_%s);" % (field.type, field.name)
                else:
                    template = "self().visit_%s(x.m_%s);" % (field.type, field.name)
            else:
                template = "self().visit_%s(*x.m_%s);" % (field.type, field.name)
            if field.seq:
                self.emit('s.append("[");', level)
                self.emit("for (size_t i=0; i<x.n_%s; i++) {" % field.name, level)
                if field.type in sums:
                    self.emit("self().visit_%s(*x.m_%s[i]);" % (field.type, field.name), level+1)
                else:
                    self.emit("self().visit_%s(x.m_%s[i]);" % (field.type, field.name), level+1)
                self.emit(    'if (i < x.n_%s-1) s.append(" ");' % (field.name), level+1)
                self.emit("}", level)
                self.emit('s.append("]");', level)
            elif field.opt:
                self.emit("if (x.m_%s) {" % field.name, 2)
                self.emit(template, 3)
                self.emit("} else {", 2)
                self.emit(    's.append("()");', 3)
                self.emit("}", 2)
            else:
                self.emit(template, level)
        else:
            if field.type == "identifier":
                if field.seq:
                    assert not field.opt
                    level = 2
                    self.emit('s.append("[");', level)
                    self.emit("for (size_t i=0; i<x.n_%s; i++) {" % field.name, level)
                    self.emit("s.append(x.m_%s[i]);" % (field.name), level+1)
                    self.emit(    'if (i < x.n_%s-1) s.append(" ");' % (field.name), level+1)
                    self.emit("}", level)
                    self.emit('s.append("]");', level)
                else:
                    if field.opt:
                        self.emit("if (x.m_%s) {" % field.name, 2)
                        self.emit(    's.append(x.m_%s);' % field.name, 3)
                        self.emit("} else {", 2)
                        self.emit(    's.append("()");', 3)
                        self.emit("}", 2)
                    else:
                        self.emit('if(indent) s.append("\\n"+indtd);', 2)
                        self.emit('s.append(x.m_%s);' % field.name, 2)
            elif field.type == "node":
                assert not field.opt
                assert field.seq
                level = 2
                self.emit('if(indent) s.append("\\n"+indtd);', level)
                self.emit('s.append("[");', level)
                self.emit("for (size_t i=0; i<x.n_%s; i++) {" % field.name, level)
                mod_name = self.mod.name.lower()
                self.emit("self().visit_%s(*x.m_%s[i]);" % (mod_name, field.name), level+1)
                self.emit(    'if (i < x.n_%s-1) s.append(" ");' % (field.name), level+1)
                self.emit("}", level)
                self.emit('s.append("]");', level)
            elif field.type == "symbol_table":
                assert not field.opt
                assert not field.seq
                if field.name == "parent_symtab":
                    level = 2
                    self.emit('if(indent) s.append("\\n"+indtd);', level)
                    self.emit('s.append(x.m_%s->get_counter());' % field.name, level)
                else:
                    level = 2
                    self.emit(      'if(indent) {',level)
                    self.emit(          's.append("\\n"+indtd);', level+1)
                    self.emit(          'inc_indent();',level+1)
                    self.emit(      '}', level)
                    self.emit(      's.append("(");', level)
                    self.emit('if (use_colors) {', level)
                    self.emit(    's.append(color(fg::yellow));', level+1)
                    self.emit('}', level)
                    self.emit('s.append("SymbolTable");', level)
                    self.emit('if (use_colors) {', level)
                    self.emit(    's.append(color(fg::reset));', level+1)
                    self.emit('}', level)
                    self.emit('s.append(" ");', level)
                    self.emit(      'if(indent) s.append("\\n"+indtd);', level)
                    self.emit(      's.append(x.m_%s->get_counter());' % field.name, level)
                    self.emit(      's.append(" ");', level)
                    self.emit(      'if(indent) {',level)
                    self.emit(          's.append("\\n"+indtd);', level+1)
                    self.emit(          'inc_indent();',level+1)
                    self.emit(      '}', level)
                    self.emit(      's.append("{");', level)
                    self.emit('{', level)
                    self.emit('    size_t i = 0;', level)
                    self.emit('    for (auto &a : x.m_%s->scope) {' % field.name, level)
                    self.emit(      'if(indent) {',level)
                    self.emit(          's.append("\\n"+indtd);', level+1)
                    self.emit(          'inc_indent();',level+1)
                    self.emit(      '}', level)
                    self.emit('      s.append(a.first + ": ");', level)
                    self.emit('        this->visit_symbol(*a.second);', level)
                    self.emit('        if (i < x.m_%s->scope.size()-1) s.append(", ");' % field.name, level)
                    self.emit('        i++;', level)
                    self.emit('    }', level)
                    self.emit('}', level)
                    self.emit(      'if(indent){',level)
                    self.emit(          'dec_indent();', level+1)
                    self.emit(          's.append("\\n"+indtd);', level+1)
                    self.emit(      '}', level)
                    self.emit(      's.append("})");', level)
            elif field.type == "string" and not field.seq:
                if field.opt:
                    self.emit("if (x.m_%s) {" % field.name, 2)
                    self.emit(    's.append("\\"" + std::string(x.m_%s) + "\\"");' % field.name, 3)
                    self.emit("} else {", 2)
                    self.emit(    's.append("()");', 3)
                    self.emit("}", 2)
                else:
                    self.emit('s.append("\\"" + std::string(x.m_%s) + "\\"");' % field.name, 2)
            elif field.type == "int" and not field.seq:
                if field.opt:
                    self.emit("if (x.m_%s) {" % field.name, 2)
                    self.emit(    's.append(std::to_string(x.m_%s));' % field.name, 3)
                    self.emit("} else {", 2)
                    self.emit(    's.append("()");', 3)
                    self.emit("}", 2)
                else:
                    self.emit('s.append(std::to_string(x.m_%s));' % field.name, 2)
            elif field.type == "bool" and not field.seq and not field.opt:
                self.emit("if (x.m_%s) {" % field.name, 2)
                self.emit(    's.append(".true.");', 3)
                self.emit("} else {", 2)
                self.emit(    's.append(".false.");', 3)
                self.emit("}", 2)
            elif field.type in self.data.simple_types:
                if field.opt:
                    self.emit('s.append("Unimplementedopt");', 2)
                else:
                    self.emit('visit_%sType(x.m_%s);' \
                            % (field.type, field.name), 2)
            else:
                self.emit('s.append("Unimplemented' + field.type + '");', 2)



class SerializationVisitorVisitor(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Serialization Visitor base class")
        self.emit("")
        self.emit("template <class Derived>")
        self.emit("class SerializationBaseVisitor : public BaseVisitor<Derived>")
        self.emit("{")
        self.emit("private:")
        self.emit(  "Derived& self() { return static_cast<Derived&>(*this); }", 1)
        self.emit("public:")
        self.mod = mod
        super(SerializationVisitorVisitor, self).visitModule(mod)
        self.emit("};")

    def visitType(self, tp):
        super(SerializationVisitorVisitor, self).visitType(tp, tp.name)

    def visitSum(self, sum, *args):
        assert isinstance(sum, asdl.Sum)
        if is_simple_sum(sum):
            name = args[0] + "Type"
            self.make_simple_sum_visitor(name, sum.types)
        else:
            for tp in sum.types:
                self.visit(tp, *args)

    def visitProduct(self, prod, name):
        self.make_visitor(name, prod.fields, False)

    def visitConstructor(self, cons, _):
        self.make_visitor(cons.name, cons.fields, True)

    def make_visitor(self, name, fields, cons):
        self.emit("void visit_%s(const %s_t &x) {" % (name, name), 1)
        if cons:
            self.emit(    'self().write_int8(x.base.type);', 2)
        self.used = False
        for n, field in enumerate(fields):
            self.visitField(field, cons)
        if not self.used:
            # Note: a better solution would be to change `&x` to `& /* x */`
            # above, but we would need to change emit to return a string.
            self.emit("if ((bool&)x) { } // Suppress unused warning", 2)
        self.emit("}", 1)

    def make_simple_sum_visitor(self, name, types):
        self.emit("void visit_%s(const %s &x) {" % (name, name), 1)
        self.emit(    'self().write_int8(x);', 2)
        self.emit("}", 1)

    def visitField(self, field, cons):
        if (field.type not in asdl.builtin_types and
            field.type not in self.data.simple_types):
            self.used = True
            level = 2
            if field.type in products:
                if field.opt:
                    template = "self().visit_%s(*x.m_%s);" % (field.type, field.name)
                else:
                    template = "self().visit_%s(x.m_%s);" % (field.type, field.name)
            else:
                template = "self().visit_%s(*x.m_%s);" % (field.type, field.name)
            if field.seq:
                self.emit('self().write_int64(x.n_%s);' % field.name, level)
                self.emit("for (size_t i=0; i<x.n_%s; i++) {" % field.name, level)
                if field.type in sums:
                    self.emit("self().visit_%s(*x.m_%s[i]);" % (field.type, field.name), level+1)
                else:
                    self.emit("self().visit_%s(x.m_%s[i]);" % (field.type, field.name), level+1)
                self.emit("}", level)
            elif field.opt:
                self.emit("if (x.m_%s) {" % field.name, 2)
                self.emit(    'self().write_bool(true);', 3)
                self.emit(template, 3)
                self.emit("} else {", 2)
                self.emit(    'self().write_bool(false);', 3)
                self.emit("}", 2)
            else:
                self.emit(template, level)
        else:
            if field.type == "identifier":
                if field.seq:
                    assert not field.opt
                    level = 2
                    self.emit('self().write_int64(x.n_%s);' % field.name, level)
                    self.emit("for (size_t i=0; i<x.n_%s; i++) {" % field.name, level)
                    self.emit("self().write_string(x.m_%s[i]);" % (field.name), level+1)
                    self.emit("}", level)
                else:
                    if field.opt:
                        self.emit("if (x.m_%s) {" % field.name, 2)
                        self.emit(    'self().write_bool(true);', 3)
                        self.emit(    'self().write_string(x.m_%s);' % field.name, 3)
                        self.emit("} else {", 2)
                        self.emit(    'self().write_bool(false);', 3)
                        self.emit("}", 2)
                    else:
                        self.emit('self().write_string(x.m_%s);' % field.name, 2)
            elif field.type == "node":
                assert not field.opt
                assert field.seq
                level = 2
                self.emit('self().write_int64(x.n_%s);' % field.name, level)
                self.emit("for (size_t i=0; i<x.n_%s; i++) {" % field.name, level)
                mod_name = self.mod.name.lower()
                self.emit("self().write_int8(x.m_%s[i]->type);" % \
                        field.name, level+1)
                self.emit("self().visit_%s(*x.m_%s[i]);" % (mod_name, field.name), level+1)
                self.emit("}", level)
            elif field.type == "symbol_table":
                assert not field.opt
                assert not field.seq
                if field.name == "parent_symtab":
                    level = 2
                    self.emit('self().write_int64(x.m_%s->get_counter());' % field.name, level)
                else:
                    level = 2
                    self.emit('self().write_int64(x.m_%s->get_counter());' % field.name, level)
                    self.emit('self().write_int64(x.m_%s->scope.size());' % field.name, level)
                    self.emit('for (auto &a : x.m_%s->scope) {' % field.name, level)
                    self.emit('    self().write_string(a.first);', level)
                    self.emit('    this->visit_symbol(*a.second);', level)
                    self.emit('}', level)
            elif field.type == "string" and not field.seq:
                if field.opt:
                    self.emit("if (x.m_%s) {" % field.name, 2)
                    self.emit(    'self().write_bool(true);', 3)
                    self.emit(    'self().write_string(x.m_%s);' % field.name, 3)
                    self.emit("} else {", 2)
                    self.emit(    'self().write_bool(false);', 3)
                    self.emit("}", 2)
                else:
                    self.emit('self().write_string(x.m_%s);' % field.name, 2)
            elif field.type == "int" and not field.seq:
                if field.opt:
                    self.emit("if (x.m_%s) {" % field.name, 2)
                    self.emit(    'self().write_bool(true);', 3)
                    self.emit(    'self().write_int64(x.m_%s);' % field.name, 3)
                    self.emit("} else {", 2)
                    self.emit(    'self().write_bool(false);', 3)
                    self.emit("}", 2)
                else:
                    self.emit('self().write_int64(x.m_%s);' % field.name, 2)
            elif field.type == "bool" and not field.seq and not field.opt:
                self.emit("if (x.m_%s) {" % field.name, 2)
                self.emit(    'self().write_bool(true);', 3)
                self.emit("} else {", 2)
                self.emit(    'self().write_bool(false);', 3)
                self.emit("}", 2)
            elif field.type in self.data.simple_types:
                if field.opt:
                    raise Exception("Unimplemented opt for field type: " + field.type);
                else:
                    self.emit('visit_%sType(x.m_%s);' \
                            % (field.type, field.name), 2)
            else:
                raise Exception("Unimplemented field type: " + field.type);

class DeserializationVisitorVisitor(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Deserialization Visitor base class")
        self.emit("")
        self.emit("template <class Derived>")
        self.emit("class DeserializationBaseVisitor : public BaseVisitor<Derived>")
        self.emit("{")
        self.emit("private:")
        self.emit(  "Derived& self() { return static_cast<Derived&>(*this); }", 1)
        self.emit("public:")
        self.emit(  "Allocator &al;", 1)
        self.emit(  r"DeserializationBaseVisitor(Allocator &al) : al{al} {}", 1)
        self.emit_deserialize_node();
        self.mod = mod
        super(DeserializationVisitorVisitor, self).visitModule(mod)
        self.emit("};")

    def visitType(self, tp):
        super(DeserializationVisitorVisitor, self).visitType(tp, tp.name)

    def visitSum(self, sum, *args):
        assert isinstance(sum, asdl.Sum)
        if is_simple_sum(sum):
            self.emit("%sType deserialize_%s() {" % (args[0], args[0]), 1)
            self.emit(  'uint8_t t = self().read_int8();', 2)
            self.emit(  '%sType ty = static_cast<%sType>(t);' % (args[0], args[0]), 2)
            self.emit(  'return ty;', 2)
            self.emit("}", 1)
        else:
            for tp in sum.types:
                self.visit(tp, *args)
            self.emit("%s_t* deserialize_%s() {" % (subs["mod"], args[0]), 1)
            self.emit(  'uint8_t t = self().read_int8();', 2)
            self.emit(  '%s::%sType ty = static_cast<%s::%sType>(t);' % (subs["mod"].upper(), args[0],
                subs["mod"].upper(), args[0]), 2)
            self.emit(  'switch (ty) {', 2)
            for tp in sum.types:
                self.emit(    'case (%s::%sType::%s) : return self().deserialize_%s();' \
                    % (subs["mod"].upper(), args[0], tp.name, tp.name), 3)
            self.emit(    'default : throw LFortranException("Unknown type in deserialize_%s()");' % args[0], 3)
            self.emit(  '}', 2)
            self.emit(  'throw LFortranException("Switch statement above was not exhaustive.");', 2)

            self.emit("}", 1)

    def emit_deserialize_node(self):
        name = "node"
        self.emit("%s_t* deserialize_%s() {" % (subs["mod"], name), 1)
        self.emit(  'uint8_t t = self().read_int8();', 2)
        self.emit(  '%s::%sType ty = static_cast<%s::%sType>(t);' % (subs["mod"].upper(), subs["mod"],
            subs["mod"].upper(), subs["mod"]), 2)
        self.emit(  'switch (ty) {', 2)
        for tp in sums:
            self.emit(    'case (%s::%sType::%s) : return self().deserialize_%s();' \
                % (subs["mod"].upper(), subs["mod"], tp, tp), 3)
        self.emit(    'default : throw LFortranException("Unknown type in deserialize_%s()");' % name, 3)
        self.emit(  '}', 2)
        self.emit(  'throw LFortranException("Switch statement above was not exhaustive.");', 2)
        self.emit(  '}', 1)

    def visitProduct(self, prod, name):
        self.emit("%s_t deserialize_%s() {" % (name, name), 1)
        self.emit(  '%s_t x;' % (name), 2)
        for field in prod.fields:
            if field.seq:
                assert not field.opt
                assert field.type not in asdl.builtin_types
                assert field.type not in simple_sums
                self.emit('{', 2)
                self.emit('uint64_t n = self().read_int64();', 3)
                if field.type in products:
                    self.emit("Vec<%s_t> v;" % (field.type), 3)
                else:
                    self.emit("Vec<%s_t*> v;" % (field.type), 3)
                self.emit("v.reserve(al, n);", 3)
                self.emit("for (uint64_t i=0; i<n; i++) {", 3)
                if field.type in products:
                    self.emit("v.push_back(al, self().deserialize_%s());" \
                         % (field.type), 4)
                else:
                    self.emit("v.push_back(al, down_cast<%s_t>(self().deserialize_%s()));" % (field.type, field.type), 4)
                self.emit('}', 3)
                self.emit('x.m_%s = v.p;' % (field.name), 3)
                self.emit('x.n_%s = v.n;' % (field.name), 3)
                self.emit('}', 2)
            else:
                self.emit('{', 2)
                if field.opt:
                    self.emit("bool present=self().read_bool();", 3)
                if field.type in asdl.builtin_types:
                    if field.type == "identifier":
                        rhs = "self().read_cstring()"
                    elif field.type == "string":
                        rhs = "self().read_cstring()"
                    else:
                        print(field.type)
                        assert False
                elif field.type in simple_sums:
                    rhs = "deserialize_%s()" % (field.type)
                else:
                    assert field.type not in products
                    rhs = "down_cast<%s_t>(deserialize_%s())" % (field.type,
                        field.type)
                if field.opt:
                    self.emit('if (present) {', 3)
                self.emit('x.m_%s = %s;' % (field.name, rhs), 4)
                if field.opt:
                    self.emit('} else {', 3)
                    self.emit(  'x.m_%s = nullptr;' % (field.name), 4)
                    self.emit('}', 3)
                self.emit('}', 2)
        self.emit(  'return x;', 2)
        self.emit("}", 1)

    def visitConstructor(self, cons, _):
        name = cons.name
        self.emit("%s_t* deserialize_%s() {" % (subs["mod"], name), 1)
        lines = []
        args = ["al", "loc"]
        for f in cons.fields:
            #type_ = convert_type(f.type, f.seq, f.opt, self.mod.name.lower())
            if f.seq:
                seq = "size_t n_%s; // Sequence" % f.name
                self.emit("%s" % seq, 2)
            else:
                seq = ""
            if f.seq:
                assert f.type not in self.data.simple_types
                if f.type not in asdl.builtin_types:
                    lines.append("n_%s = self().read_int64();" % (f.name))
                    if f.type in products:
                        lines.append("Vec<%s_t> v_%s;" % (f.type, f.name))
                    else:
                        lines.append("Vec<%s_t*> v_%s;" % (f.type, f.name))
                    lines.append("v_%s.reserve(al, n_%s);" % (f.name, f.name))
                    lines.append("for (size_t i=0; i<n_%s; i++) {" % (f.name))
                    if f.type in products:
                        lines.append("    v_%s.push_back(al, self().deserialize_%s());" % (f.name, f.type))
                    else:
                        lines.append("    v_%s.push_back(al, %s::down_cast<%s::%s_t>(self().deserialize_%s()));" % (f.name,
                            subs["mod"].upper(), subs["mod"].upper(), f.type, f.type))
                    lines.append("}")
                else:
                    if f.type == "node":
                        lines.append("n_%s = self().read_int64();" % (f.name))
                        lines.append("Vec<%s_t*> v_%s;" % (subs["mod"], f.name))
                        lines.append("v_%s.reserve(al, n_%s);" % (f.name, f.name))
                        lines.append("for (size_t i=0; i<n_%s; i++) {" % (f.name))
                        lines.append("    v_%s.push_back(al, self().deserialize_node());" % (f.name))
                        lines.append("}")
                    elif f.type == "identifier":
                        lines.append("n_%s = self().read_int64();" % (f.name))
                        lines.append("Vec<char*> v_%s;" % (f.name))
                        lines.append("v_%s.reserve(al, n_%s);" % (f.name, f.name))
                        lines.append("for (size_t i=0; i<n_%s; i++) {" % (f.name))
                        lines.append("    v_%s.push_back(al, self().read_cstring());" % (f.name))
                        lines.append("}")
                    else:
                        print(f.type)
                        assert False
                args.append("v_%s.p" % (f.name))
                args.append("v_%s.n" % (f.name))
            else:
                # if builtin or simple types, handle appropriately
                if f.type in asdl.builtin_types:
                    if f.type == "identifier":
                        lines.append("char *m_%s;" % (f.name))
                        if f.opt:
                            lines.append("bool m_%s_present = self().read_bool();" \
                                % f.name)
                            lines.append("if (m_%s_present) {" % f.name)
                        lines.append("m_%s = self().read_cstring();" % (f.name))
                        if f.opt:
                            lines.append("} else {")
                            lines.append("m_%s = nullptr;" % (f.name))
                            lines.append("}")
                        args.append("m_%s" % (f.name))
                    elif f.type == "string":
                        lines.append("char *m_%s;" % (f.name))
                        if f.opt:
                            lines.append("bool m_%s_present = self().read_bool();" \
                                % f.name)
                            lines.append("if (m_%s_present) {" % f.name)
                        lines.append("m_%s = self().read_cstring();" % (f.name))
                        if f.opt:
                            lines.append("} else {")
                            lines.append("m_%s = nullptr;" % (f.name))
                            lines.append("}")
                        args.append("m_%s" % (f.name))
                    elif f.type == "int":
                        assert not f.opt
                        lines.append("int64_t m_%s = self().read_int64();" % (f.name))
                        args.append("m_%s" % (f.name))
                    elif f.type == "bool":
                        assert not f.opt
                        lines.append("bool m_%s = self().read_bool();" % (f.name))
                        args.append("m_%s" % (f.name))
                    elif f.type == "symbol_table":
                        assert not f.opt
                        # TODO: read the symbol table:
                        lines.append("SymbolTable *m_%s=nullptr;" % (f.name))
                        lines.append('throw LFortranException("SymbolTable not implemented");')
                        args.append("m_%s" % (f.name))
                    else:
                        print(f.type)
                        assert False
                else:
                    if f.type in products:
                        assert not f.opt
                        lines.append("%s::%s_t m_%s = self().deserialize_%s();" % (subs["mod"].upper(), f.type, f.name, f.type))
                    else:
                        if f.type in simple_sums:
                            assert not f.opt
                            lines.append("%s::%sType m_%s = self().deserialize_%s();" % (subs["mod"].upper(),
                                f.type, f.name, f.type))
                        else:
                            lines.append("%s::%s_t *m_%s;" % (subs["mod"].upper(),
                                f.type, f.name))
                            if f.opt:
                                lines.append("if (self().read_bool()) {")
                                lines.append("m_%s = %s::down_cast<%s::%s_t>(self().deserialize_%s());" % (
                                    f.name, subs["mod"].upper(), subs["mod"].upper(), f.type, f.type))
                                lines.append("} else {")
                                lines.append("m_%s = nullptr;" % f.name)
                                lines.append("}")
                            else:
                                lines.append("m_%s = %s::down_cast<%s::%s_t>(self().deserialize_%s());" % (
                                    f.name, subs["mod"].upper(), subs["mod"].upper(), f.type, f.type))
                    args.append("m_%s" % (f.name))
        for line in lines:
            self.emit(line, 2)

        self.emit(    'Location loc;', 2)
        self.emit(    'return %s::make_%s_t(%s);' % (subs["mod"].upper(), name, ", ".join(args)), 2)
        self.emit("}", 1)


class ASDLData(object):

    def __init__(self, tree):
        simple_types = set()
        prod_simple = set()
        field_masks = {}
        required_masks = {}
        optional_masks = {}
        cons_attributes = {}
        def add_masks(fields, node):
            required_mask = 0
            optional_mask = 0
            for i, field in enumerate(fields):
                flag = 1 << i
                if field not in field_masks:
                    field_masks[field] = flag
                else:
                    assert field_masks[field] == flag
                if field.opt:
                    optional_mask |= flag
                else:
                    required_mask |= flag
            required_masks[node] = required_mask
            optional_masks[node] = optional_mask
        for tp in tree.dfns:
            if isinstance(tp.value, asdl.Sum):
                sum = tp.value
                if is_simple_sum(sum):
                    simple_types.add(tp.name)
                else:
                    attrs = [field for field in sum.attributes]
                    for cons in sum.types:
                        add_masks(attrs + cons.fields, cons)
                        cons_attributes[cons] = attrs
            else:
                prod = tp.value
                prod_simple.add(tp.name)
                add_masks(prod.fields, prod)
        prod_simple.update(simple_types)
        self.cons_attributes = cons_attributes
        self.simple_types = simple_types
        self.prod_simple = prod_simple
        self.field_masks = field_masks
        self.required_masks = required_masks
        self.optional_masks = optional_masks


HEAD = r"""#ifndef LFORTRAN_%(MOD)s_H
#define LFORTRAN_%(MOD)s_H

// Generated by grammar/asdl_cpp.py

#include <lfortran/parser/alloc.h>
#include <lfortran/parser/location.h>
#include <lfortran/colors.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/semantics/asr_scopes.h>


namespace LFortran::%(MOD)s {

enum %(mod)sType
{
    %(types)s
};

struct %(mod)s_t
{
    %(mod)sType type;
    Location loc;
};


template <class T, class U>
inline bool is_a(const U &x)
{
    return T::class_type == x.type;
}

// Cast one level down

template <class T, class U>
static inline T* down_cast(const U *f)
{
    LFORTRAN_ASSERT(f != nullptr);
    LFORTRAN_ASSERT(is_a<T>(*f));
    return (T*)f;
}

// Cast two levels down

template <class T>
static inline T* down_cast2(const %(mod)s_t *f)
{
    typedef typename T::parent_type ptype;
    ptype *t = down_cast<ptype>(f);
    return down_cast<T>(t);
}


"""

FOOT = r"""} // namespace LFortran::%(MOD)s

#endif // LFORTRAN_%(MOD)s_H
"""

visitors = [ASTNodeVisitor0, ASTNodeVisitor1, ASTNodeVisitor,
        ASTVisitorVisitor1, ASTVisitorVisitor1b, ASTVisitorVisitor2,
        ASTWalkVisitorVisitor, PickleVisitorVisitor,
        SerializationVisitorVisitor, DeserializationVisitorVisitor]


def main(argv):
    if len(argv) == 3:
        def_file, out_file = argv[1:]
    elif len(argv) == 1:
        print("Assuming default values of AST.asdl and ast.h")
        here = os.path.dirname(__file__)
        def_file = os.path.join(here, "AST.asdl")
        out_file = os.path.join(here, "..", "src", "lfortran", "ast.h")
    else:
        print("invalid arguments")
        return 2
    mod = asdl.parse(def_file)
    data = ASDLData(mod)
    CollectVisitor(None, data).visit(mod)
    types_ = ", ".join(sums)
    global subs
    subs = {
        "MOD": mod.name.upper(),
        "mod": mod.name.lower(),
        "types": types_,
    }
    fp = open(out_file, "w")
    try:
        fp.write(HEAD % subs)
        for visitor in visitors:
            visitor(fp, data).visit(mod)
            fp.write("\n\n")
        fp.write(FOOT % subs)
    finally:
        fp.close()


if __name__ == "__main__":
    sys.exit(main(sys.argv))
