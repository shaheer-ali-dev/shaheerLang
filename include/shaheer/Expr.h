#pragma once
#include <memory>
#include <string>
#include <vector>
#include "shaheer/Token.h"
#include "shaheer/Value.h"

class Literal;
class Variable;
class Unary;
class Binary;
class Grouping;
class Call;
struct Assign;  



class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual Value visitBinary(Binary* expr) = 0;
    virtual Value visitGrouping(Grouping* expr) = 0;
    virtual Value visitLiteral(Literal* expr) = 0;
    virtual Value visitUnary(Unary* expr) = 0;
    virtual Value visitVariable(Variable* expr) = 0;
    virtual Value visitCall(Call* expr) = 0;
    virtual Value visitAssignExpr(Assign*) = 0;   // ✅ REQUIRED

};

class Expr {
public:
    virtual ~Expr() = default;
    virtual Value accept(ExprVisitor* visitor) = 0;
};

class Literal : public Expr {
public:
    Value value;

    Literal(const Value& v) : value(v) {}

    Value accept(ExprVisitor* visitor) override {
        return visitor->visitLiteral(this);
    }
};

class Grouping : public Expr {
public:
    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expr) : expression(std::move(expr)) {}

    Value accept(ExprVisitor* visitor) override {
        return visitor->visitGrouping(this);
    }
};

class Unary : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;

    Unary(const Token& o, std::unique_ptr<Expr> r) : op(o), right(std::move(r)) {}

    Value accept(ExprVisitor* visitor) override {
        return visitor->visitUnary(this);
    }
};

class Binary : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> l, const Token& o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}

    Value accept(ExprVisitor* visitor) override {
        return visitor->visitBinary(this);
    }
};

class Variable : public Expr {
public:
    std::string name;

    Variable(const std::string& n) : name(n) {}

    Value accept(ExprVisitor* visitor) override {
        return visitor->visitVariable(this);
    }
};

struct Call : Expr {
    std::string callee;
    std::vector<std::unique_ptr<Expr>> arguments;

    Call(const std::string& callee,
         std::vector<std::unique_ptr<Expr>> args)
        : callee(callee), arguments(std::move(args)) {}

    Value accept(ExprVisitor* visitor) override {
        return visitor->visitCall(this);
    }
};
struct Assign : Expr {
    std::string name;
    std::unique_ptr<Expr> value;

    Assign(const std::string& name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}

    Value accept(ExprVisitor* visitor) override {
        return visitor->visitAssignExpr(this);
    }
};
