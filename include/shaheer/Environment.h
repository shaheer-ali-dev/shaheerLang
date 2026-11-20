#pragma once
#include "shaheer/Value.h"
#include <unordered_map>
#include <string>

class Environment {
public:
    void define(const std::string& name, Value value);
    void assign(const std::string& name, const Value& value);
    Value get(const std::string& name);

private:
    std::unordered_map<std::string, Value> values;
};
