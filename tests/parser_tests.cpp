#include <iostream>
#include <stdexcept>
#include <string>
#include "shaheer/Lexer.h"
#include "shaheer/Parser.h"

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond "\n"; ++failures; } } while (0)

static std::vector<std::unique_ptr<Stmt>> parse(const std::string& src) {
    Lexer lexer(src);
    Parser parser(lexer.scanTokens());
    return parser.parse();
}

// Parse errors are also echoed to stderr by the parser; that noise is expected here.
static bool throwsOnParse(const std::string& src) {
    try { parse(src); } catch (const std::exception&) { return true; }
    return false;
}

static void testStatementKinds() {
    auto s = parse("define x = 1; x = 2; logOnConsole(x);");
    CHECK(s.size() == 3);
    CHECK(dynamic_cast<VarDecl*>(s[0].get()) != nullptr);
    CHECK(dynamic_cast<ExprStmt*>(s[1].get()) != nullptr);
    CHECK(dynamic_cast<PrintStmt*>(s[2].get()) != nullptr);
}

static void testDeclarationWithoutInitializer() {
    auto s = parse("define x;");
    CHECK(s.size() == 1);
    auto* d = dynamic_cast<VarDecl*>(s[0].get());
    CHECK(d != nullptr && d->name == "x" && d->initializer == nullptr);
}

static void testPrecedence() {
    // 1 + 2 * 3  ->  Binary(1, +, Binary(2, *, 3))
    auto s = parse("1 + 2 * 3;");
    auto* es = dynamic_cast<ExprStmt*>(s[0].get());
    CHECK(es != nullptr);
    auto* top = dynamic_cast<Binary*>(es->expression.get());
    CHECK(top != nullptr && top->op.type == TokenType::PLUS);
    auto* rhs = dynamic_cast<Binary*>(top->right.get());
    CHECK(rhs != nullptr && rhs->op.type == TokenType::STAR);
}

static void testGroupingAndAssignment() {
    auto s = parse("(1 + 2) * 3; y = 4;");
    auto* top = dynamic_cast<Binary*>(dynamic_cast<ExprStmt*>(s[0].get())->expression.get());
    CHECK(top != nullptr && top->op.type == TokenType::STAR);
    CHECK(dynamic_cast<Grouping*>(top->left.get()) != nullptr);
    CHECK(dynamic_cast<Assign*>(dynamic_cast<ExprStmt*>(s[1].get())->expression.get()) != nullptr);
}

static void testErrors() {
    CHECK(throwsOnParse("define x = 1"));        // missing ';'
    CHECK(throwsOnParse("logOnConsole 1;"));     // missing '('
    CHECK(throwsOnParse("1 = 2;"));              // invalid assignment target
    CHECK(throwsOnParse("(1 + 2;"));             // missing ')'
}

int main() {
    testStatementKinds();
    testDeclarationWithoutInitializer();
    testPrecedence();
    testGroupingAndAssignment();
    testErrors();
    if (failures) { std::cerr << failures << " parser check(s) failed\n"; return 1; }
    std::cout << "parser_tests: all passed\n";
    return 0;
}
