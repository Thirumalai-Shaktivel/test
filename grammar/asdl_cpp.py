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
products = []

def convert_type(asdl_type):
    if asdl_type in simple_sums:
        type_ = asdl_type + "Type"
    elif asdl_type == "string":
        type_ = "char*"
    elif asdl_type == "identifier":
        type_ = "char*"
    elif asdl_type == "constant":
        type_ = "bool"
    elif asdl_type == "object":
        # FIXME: this should use some other type that we actually need
        type_ = "int /* object */"
    elif asdl_type == "int":
        type_ = "int"
    else:
        type_ = asdl_type + "_t"
        if not asdl_type in products:
            # Sum type is polymorphic, must be a pointer
            type_ = type_ + "*"
    return type_

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
        super(ASTNodeVisitor1, self).visitModule(mod)

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitProduct(self, product, name):
        self.emit("struct %s_t // Product" % name)
        self.emit("{");
        for f in product.fields:
            type_ = convert_type(f.type)
            if f.seq:
                type_ = type_ + "*"
            self.emit("%s m_%s;" % (type_, f.name), 1)
        self.emit("};");


class ASTNodeVisitor(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Sums declarations")
        self.emit("")
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
        self.emit(    "%s_t base;" % base, 2);
        args = ["Allocator &al"]
        lines = []
        for f in cons.fields:
            type_ = convert_type(f.type)
            if f.seq:
                type_ = type_ + "*"
            self.emit("%s m_%s;" % (type_, f.name), 2)
            args.append("%s a_%s" % (type_, f.name))
            lines.append("n->m_%s = a_%s;" % (f.name, f.name))
        self.emit("};", 1)
        self.emit("static inline %s_t* make_%s_t(%s) {" % (base, cons.name,
            ", ".join(args)), 1)
        self.emit("%s_t *n;" % cons.name, 2)
        self.emit("n = al.make_new<%s_t>();" % cons.name, 2)
        self.emit("n->base.type = %sType::%s;" % (base, cons.name), 2)
        for line in lines:
            self.emit(line, 2)
        self.emit("return (%s_t*)n;" % base, 2)
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
            self.emit("    switch (x.type) {")
            for type_ in sum.types:
                self.emit("        case %sType::%s: { v.visit_%s((const %s_t &)x);"
                    " return; }" % (base, type_.name, type_.name, type_.name))
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
        self.emit("    Derived& self() { return LFortran::down_cast<Derived&>(*this); }")
        self.emit("public:")
        super(ASTVisitorVisitor2, self).visitModule(mod)
        self.emit("};")

    def visitType(self, tp):
        self.visit(tp.value, tp.name)

    def visitSum(self, sum, base):
        if not is_simple_sum(sum):
            self.emit("void visit_%s(const %s_t &b) { visit_%s_t(b, self()); }"\
                    % (base, base, base), 1)
            for type_ in sum.types:
                self.emit("void visit_%s(const %s_t &x) { }" \
                        % (type_.name, type_.name), 2)


class ASTWalkVisitorVisitor(ASDLVisitor):

    def visitModule(self, mod):
        self.emit("/" + "*"*78 + "/")
        self.emit("// Walk Visitor base class")
        self.emit("")
        self.emit("template <class Derived>")
        self.emit("class BaseWalkVisitor : public BaseVisitor<Derived>")
        self.emit("{")
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
        have_body = False
        for field in fields:
            self.visitField(field)
        self.emit("}", 1)

    def visitField(self, field):
        if (field.type not in asdl.builtin_types and
            field.type not in self.data.simple_types):
            level = 2
            if field.type in products:
                template = "this->visit_%s(x.m_%s);" % (field.type, field.name)
            else:
                template = "this->visit_%s(*x.m_%s);" % (field.type, field.name)
            if field.seq:
                template = "// self.visit_sequence(node.%s)" % field.name
            elif field.opt:
                self.emit("//if node.%s:" % field.name, 2)
                level = 3
            self.emit(template, level)



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


HEAD = r"""#ifndef LFORTRAN_AST_H
#define LFORTRAN_AST_H

// Generated by grammar/asdl_cpp.py

#include <lfortran/parser/alloc.h>
#include <lfortran/casts.h>


namespace LFortran::AST {


"""

FOOT = r"""} // namespace LFortran::AST

#endif // LFORTRAN_AST_H
"""

visitors = [ASTNodeVisitor0, ASTNodeVisitor1, ASTNodeVisitor,
        ASTVisitorVisitor1, ASTVisitorVisitor2, ASTWalkVisitorVisitor]


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
    fp = open(out_file, "w")
    try:
        fp.write(HEAD)
        for visitor in visitors:
            visitor(fp, data).visit(mod)
            fp.write("\n\n")
        fp.write(FOOT)
    finally:
        fp.close()


if __name__ == "__main__":
    sys.exit(main(sys.argv))
