#include "resolver.h"

#include "lox.h"

namespace lox {

void Resolver::Declare(Token name) {
    if (std::empty(scopes)) {
        return;
    }
    scopes.back().insert({name.Lexeme, false});
}

void Resolver::Define(Token name) {
    if (std::empty(scopes)) {
        return;
    }
    scopes.back().insert_or_assign(name.Lexeme, true);
}

void Resolver::Resolve(std::vector<std::shared_ptr<Statement>>& statements) {
    for (auto& s : statements) {
        Resolve(*s);
    }
}

void Resolver::Resolve(std::vector<std::unique_ptr<Statement>>& statements) {
    for (auto& s : statements) {
        Resolve(*s);
    }
}

void Resolver::BeginScope() { scopes.push_back(std::map<std::string, bool>()); }

void Resolver::EndScope() { scopes.pop_back(); }

void Resolver::Resolve(Expression& expression) { expression.Accept(*this); }

void Resolver::Resolve(Statement& statement) { statement.Accept(*this); }

void Resolver::Visit(Literal&) {}

void Resolver::Visit(BinaryExpr& b) {
    Resolve(*b.Left);
    Resolve(*b.Right);
}

void Resolver::Visit(UnaryExpr& u) { Resolve(*u.Expr); }

void Resolver::Visit(Grouping& g) { Resolve(*g.Expr); }

void Resolver::ResolveLocal(Expression* expr, Token Name) {
    for (int i = std::size(scopes) - 1; i >= 0; --i) {
        if (scopes[i].find(Name.Lexeme) != scopes[i].end()) {
            // We found the symbol, tell the resolver where the symbol is located.
            interpreter_.Resolve(expr, std::size(scopes) - 1 - i);
            return;
        }
    }
}

void Resolver::Visit(Variable& v) {
    if (!std::empty(scopes)) {
        // Check if the varaible is accessed inside its own initializer.
        auto val = scopes.back().find(v.Name.Lexeme);
        if (val != scopes.back().end() &&
            !val->second) {  // if the var exists, and has not been init.
            lox::Error(v.Name.Line,
                       "can't read the local variable in its own intializer");
            return;
        }
    }

    ResolveLocal(&v, v.Name);
}

void Resolver::Visit(Assignment& a) {
    Resolve(*a.Expr);
    ResolveLocal(&a, a.Name);
}

void Resolver::Visit(Logical& l) {
    Resolve(*l.Left);
    Resolve(*l.Right);
}

void Resolver::Visit(Call& c) {
    Resolve(*c.Callee);
    for (auto& a : c.Arguments) {
        Resolve(*a);
    }
}

void Resolver::Visit(PrintStatement& p) { Resolve(*p.Expr); }

void Resolver::Visit(ExpressionStatement& e) { Resolve(*e.Expr); }

void Resolver::Visit(VariableDeclaration& vdecl) {
    Declare(vdecl.Name);
    if (vdecl.Initializer != nullptr) {
        Resolve(*vdecl.Initializer);
    }
    Define(vdecl.Name);
}

void Resolver::Visit(Block& blk) {
    BeginScope();
    Resolve(blk.Statements);
    EndScope();
}

void Resolver::Visit(IfStatement& i) {
    Resolve(*i.Condition);
    Resolve(*i.ThenBranch);
    if(i.ElseBranch != nullptr)
    {
        Resolve(*i.ElseBranch);
    }
}

void Resolver::Visit(While& w) {
    Resolve(*w.Condition);
    Resolve(*w.Body);
}

void Resolver::ResolveFunction(FunctionDeclaration& f) {
    BeginScope();
    for (auto& param : f.Params) {
        Declare(param);
        Define(param);
    }

    Resolve(f.Body);
    EndScope();
}

void Resolver::Visit(FunctionDeclaration& s) {
    Declare(s.Name);
    Define(s.Name);

    ResolveFunction(s);
}

void Resolver::Visit(ReturnStatement& r) {
    if (r.Value != nullptr) {
        Resolve(*r.Value);
    }
}

}  // namespace lox
