#pragma once
#include <vector>
#include <memory>
#include "shaheer/Expr.h"
#include "shaheer/Stmt.h"
#include "shaheer/Environment.h"

class Interpreter : public ExprVisitor, public StmtVisitor {
private:
    Environment env;

public:
    Interpreter() {}

    // ExprVisitor methods
     Value evaluate(std::unique_ptr<Expr>& expr); 
    Value visitLiteral(Literal* expr) override;
    Value visitVariable(Variable* expr) override;
    Value visitUnary(Unary* expr) override;
    Value visitBinary(Binary* expr) override;
    Value visitGrouping(Grouping* expr) override;
    Value visitCall(Call* expr) override;
    Value visitAssignExpr(Assign*) override;

    // StmtVisitor methods
    void visitVarDecl(VarDecl*) override;
    void visitExprStmt(ExprStmt* stmt) override;
    void visitPrintStmt(PrintStmt* stmt) override;

    void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
};