#include "Mem.h"

Mem::Mem() {
    memory = JSON::Object{};
}

void Mem::serialize(JSON::Value &to) {
    to["memory"] = this->memory;
}

void Mem::deserialize(JSON::Value &from) {
    this->memory = from["memory"];
}

void Mem::del(const char *key) {
    memory[key] = JSON::null;
}

void Mem::setBool(const char *key, bool value) {
    memory[key] = value;
}

bool Mem::getBool(const char *key, bool defaultValue) {
    auto value = memory[key];
    if (!value.is(JSON::JSON_BOOL)) {
        return defaultValue;
    }
    return value.as<bool>();
}