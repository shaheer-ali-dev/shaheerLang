#include <iostream>
#include <string>
#include <vector>
#include "shaheer/Lexer.h"

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond "\n"; ++failures; } } while (0)

static std::vector<Token> lex(const std::string& src) {
    Lexer lexer(src);
    return lexer.scanTokens();
}

static void testDeclaration() {
    auto t = lex("define x = 12.5;");
    CHECK(t.size() == 6);
    CHECK(t[0].type == TokenType::VAR);
    CHECK(t[1].type == TokenType::IDENTIFIER && t[1].lexeme == "x");
    CHECK(t[2].type == TokenType::EQUAL);
    CHECK(t[3].type == TokenType::NUMBER && t[3].lexeme == "12.5");
    CHECK(t[4].type == TokenType::SEMICOLON);
    CHECK(t[5].type == TokenType::END_OF_FILE);
}

static void testKeywords() {
    auto t = lex("logOnConsole if else while true false");
    CHECK(t[0].type == TokenType::LOGONCONSOLE);
    CHECK(t[1].type == TokenType::IF);
    CHECK(t[2].type == TokenType::ELSE);
    CHECK(t[3].type == TokenType::WHILE);
    CHECK(t[4].type == TokenType::TRUE);
    CHECK(t[5].type == TokenType::FALSE);
}

static void testOperators() {
    auto t = lex("+ - * / % == != < <= > >= ( ) { } , . ;");
    TokenType expected[] = {
        TokenType::PLUS, TokenType::MINUS, TokenType::STAR, TokenType::SLASH,
        TokenType::PERCENT, TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL,
        TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER,
        TokenType::GREATER_EQUAL, TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN,
        TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE, TokenType::COMMA,
        TokenType::DOT, TokenType::SEMICOLON, TokenType::END_OF_FILE};
    CHECK(t.size() == sizeof(expected) / sizeof(expected[0]));
    for (size_t i = 0; i < t.size() && i < sizeof(expected) / sizeof(expected[0]); ++i)
        CHECK(t[i].type == expected[i]);
}

static void testStringAndLines() {
    auto t = lex("define s = \"hi\";\nlogOnConsole(s);");
    CHECK(t[3].type == TokenType::STRING_LIT);
    CHECK(t[3].lexeme.find("hi") != std::string::npos);
    CHECK(t[0].line == 1);
    CHECK(t[5].line == 2);   // logOnConsole is on line 2
}

int main() {
    testDeclaration();
    testKeywords();
    testOperators();
    testStringAndLines();
    if (failures) { std::cerr << failures << " lexer check(s) failed\n"; return 1; }
    std::cout << "lexer_tests: all passed\n";
    return 0;
}
