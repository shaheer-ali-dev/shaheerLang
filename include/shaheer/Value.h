#pragma once
#include <string>

enum class ValueType { INT, DOUBLE, STRING };

class Value {
public:
    ValueType type;
    int intValue;
    double doubleValue;
    std::string stringValue;

    // Default constructor
    Value() : type(ValueType::INT), intValue(0) {}

    Value(int v) { type = ValueType::INT; intValue = v; }
    Value(double v) { type = ValueType::DOUBLE; doubleValue = v; }
    Value(const std::string& v) { type = ValueType::STRING; stringValue = v; }

    double toDouble() const;
    int toInt() const;
    std::string toString() const;
};
