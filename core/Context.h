#pragma once

#include <unordered_set>
#include <string>

struct Context {
    std::unordered_set<std::string> enabledRules;
    std::unordered_set<std::string> disabledRules;
    bool enableAll = false;
};