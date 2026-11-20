#include "shaheer/Environment.h"
#include <stdexcept>

void Environment::define(const std::string& name, Value value) {
    values[name] = value;
}

void Environment::assign(const std::string& name, const Value& value) {
    if (values.find(name) != values.end()) {
        values[name] = value;
        return;
    }
    throw std::runtime_error("Undefined variable '" + name + "'");
}

Value Environment::get(const std::string& name) {
    if (values.find(name) != values.end())
        return values[name];
    throw std::runtime_error("Undefined variable: " + name);
}
