#include "shaheer/Parser.h"
#include <iostream>
#include <stdexcept>




// Parse function with debug output
std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    
    std::vector<std::unique_ptr<Stmt>> statements;
    
    while (!isAtEnd()) {
        
        try {
            auto stmt = statement();
            if (stmt) {
                statements.push_back(std::move(stmt));
            }
        } catch (const std::exception& e) {
            std::cerr << "[PARSE_ERROR] Exception: " << e.what() << std::endl;
            throw;
        }
    }
    
    return statements;
}

std::unique_ptr<Stmt> Parser::statement() {
    
    if (isAtEnd()) {
        return nullptr;
    }
    
    if (match({TokenType::VAR})) {
        return varDeclaration();
    }
    
    if (match({TokenType::LOGONCONSOLE})) {
        return printStatement();
    }
    
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    
    consume(TokenType::IDENTIFIER, "Expected variable name after 'var'");
    Token name = previous();

    std::unique_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    
    return std::make_unique<VarDecl>(name.lexeme, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::printStatement() {
    
    consume(TokenType::LEFT_PAREN, "Expected '(' after logOnConsole");
    
    auto value = expression();
    
    consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");
    
    consume(TokenType::SEMICOLON, "Expected ';' after logOnConsole");
    
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    
    if (isAtEnd()) {
        return nullptr;
    }
    
    auto expr = expression();
    
    consume(TokenType::SEMICOLON, "Expected ';' after expression");
    
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression() {
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment() {
    
    auto expr = logicalOr();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = assignment();

        if (auto* var = dynamic_cast<Variable*>(expr.get())) {
            return std::make_unique<Assign>(
                var->name,
                std::move(value)
            );
        }

        throw std::runtime_error("Invalid assignment target");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::logicalOr() {
    auto expr = logicalAnd();
    return expr;
}

std::unique_ptr<Expr> Parser::logicalAnd() {
    auto expr = equality();
    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    
    auto expr = addition();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        auto right = addition();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::addition() {
    
    auto expr = multiplication();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        auto right = multiplication();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::multiplication() {
    
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR, TokenType::PERCENT})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    
    if (match({TokenType::BANG_EQUAL, TokenType::MINUS})) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    
    if (match({TokenType::TRUE})) {
        return std::make_unique<Literal>(Value(true));
    }

    if (match({TokenType::FALSE})) {
        return std::make_unique<Literal>(Value(false));
    }

    if (match({TokenType::NUMBER})) {
        Token prev = previous();
        return std::make_unique<Literal>(Value(std::stod(prev.lexeme)));
    }

    if (match({TokenType::STRING_LIT})) {
        Token prev = previous();
        return std::make_unique<Literal>(Value(prev.lexeme));
    }

    if (match({TokenType::IDENTIFIER})) {
        Token prev = previous();
        return std::make_unique<Variable>(prev.lexeme);
    }

    if (match({TokenType::LEFT_PAREN})) {
        
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw std::runtime_error("Unexpected token in primary expression");
}

Token Parser::peek() const {
    if (current >= (int)tokens.size()) {
        static Token eofToken(TokenType::END_OF_FILE, "", 0);
        return eofToken;
    }
    return tokens[current];
}

Token Parser::previous() const {
    if (current == 0) {
        return tokens[0];
    }
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) {
        current++;
    }
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) {
        return false;
    }
    return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

void Parser::consume(TokenType type, const std::string& message) {
    // std::cerr << "\n[CONSUME] Looking for type " << (int)type << " at position " << current << std::endl;
    
    if (check(type)) {
        // std::cerr << "[CONSUME] ✓ Found expected token, advancing" << std::endl;
        advance();
        return;
    }
    
    if (!isAtEnd()) {
        std::cerr << "[CONSUME ERROR] Got type: " << (int)tokens[current].type 
                  << " | lexeme: '" << tokens[current].lexeme << "'" << std::endl;
    }
    
    throw std::runtime_error(message);
}

bool Parser::isAtEnd() const {
    return current >= (int)tokens.size() || peek().type == TokenType::END_OF_FILE;
}