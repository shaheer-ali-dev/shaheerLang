#include "shaheer/Value.h"

double Value::toDouble() const {
    if (type == ValueType::INT) return static_cast<double>(intValue);
    if (type == ValueType::DOUBLE) return doubleValue;
    return 0.0;
}

int Value::toInt() const {
    if (type == ValueType::INT) return intValue;
    if (type == ValueType::DOUBLE) return static_cast<int>(doubleValue);
    return 0;
}

std::string Value::toString() const {
    if (type == ValueType::INT) return std::to_string(intValue);
    if (type == ValueType::DOUBLE) return std::to_string(doubleValue);
    return stringValue;
}
