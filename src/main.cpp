#include <iostream>
#include <fstream>
#include <sstream>
#include "shaheer/Lexer.h"
#include "shaheer/Parser.h"
#include "shaheer/Interpreter.h"

int main(int argc, char** argv) {
    // std::cout << "🚀 NetLang starting...\n";

    // Usage: shaheerLang [source-file]   (defaults to examples/hello.net)
    const std::string path = (argc > 1) ? argv[1] : "examples/hello.net";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << path << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
if (!code.empty() && code.back() == '\n') {
        code.pop_back();
    }
    // std::cout << "📄 Source code loaded:\n";
    // std::cout << "----------------------\n";
    // std::cout << code << "\n";
    // std::cout << "----------------------\n\n";

    // std::cout << "🔍 Lexical analysis started...\n";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.scanTokens();
    // std::cout << "✅ Tokens generated: " << tokens.size() << "\n";

    // std::cout << "🧠 Parsing started...\n";
// std::cout << "Last tokens before parsing:\n";


    Parser parser(tokens);
    auto statements = parser.parse();
    // std::cout << "✅ Parsing completed\n";

    // std::cout << "⚙️ Interpretation started...\n";
    Interpreter interpreter;
    interpreter.interpret(statements);
    // std::cout << "✅ Execution finished\n";

    // std::cout << "🏁 NetLang exited successfully\n";
    return 0;
}
