#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "shaheer/Lexer.h"
#include "shaheer/Parser.h"
#include "shaheer/Interpreter.h"

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond "\n"; ++failures; } } while (0)

// Runs a program and returns everything it printed with logOnConsole.
static std::string run(const std::string& src) {
    std::ostringstream captured;
    std::streambuf* old = std::cout.rdbuf(captured.rdbuf());
    try {
        Lexer lexer(src);
        Parser parser(lexer.scanTokens());
        auto statements = parser.parse();
        Interpreter interpreter;
        interpreter.interpret(statements);
    } catch (...) {
        std::cout.rdbuf(old);
        throw;
    }
    std::cout.rdbuf(old);
    return captured.str();
}

static void testArithmetic() {
    CHECK(run("logOnConsole(1 + 2);")       == "3.000000\n");
    CHECK(run("logOnConsole(2 * 3 + 1);")   == "7.000000\n");
    CHECK(run("logOnConsole((1 + 2) * 4);") == "12.000000\n");
    CHECK(run("logOnConsole(10 - 4);")      == "6.000000\n");
    CHECK(run("logOnConsole(10 / 4);")      == "2.500000\n");
    CHECK(run("logOnConsole(10 % 3);")      == "1\n");
    CHECK(run("logOnConsole(-5);")          == "-5.000000\n");
}

static void testComparison() {
    CHECK(run("logOnConsole(1 < 2);")  == "1\n");
    CHECK(run("logOnConsole(2 <= 1);") == "0\n");
    CHECK(run("logOnConsole(3 > 2);")  == "1\n");
    CHECK(run("logOnConsole(2 >= 3);") == "0\n");
    CHECK(run("logOnConsole(2 == 2);") == "1\n");
    CHECK(run("logOnConsole(2 != 2);") == "0\n");
}

static void testVariables() {
    CHECK(run("define x = 4; logOnConsole(x * 2);") == "8.000000\n");
    CHECK(run("define x = 1; x = 5; logOnConsole(x);") == "5.000000\n");
    CHECK(run("define s = \"SHAHEER\"; logOnConsole(s);").find("SHAHEER") != std::string::npos);
}

static void testRuntimeErrors() {
    bool threw = false;
    try { run("logOnConsole(missing);"); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);

    threw = false;
    try { run("missing = 1;"); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);

    threw = false;
    try { run("nope(1);"); } catch (const std::exception&) { threw = true; }
    CHECK(threw);
}

int main() {
    testArithmetic();
    testComparison();
    testVariables();
    testRuntimeErrors();
    if (failures) { std::cerr << failures << " interpreter check(s) failed\n"; return 1; }
    std::cout << "interpreter_tests: all passed\n";
    return 0;
}
