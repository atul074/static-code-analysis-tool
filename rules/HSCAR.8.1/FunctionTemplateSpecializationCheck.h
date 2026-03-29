#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

class FunctionTemplateSpecializationCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAR.8.1";
    }

    void registerMatchers(clang::ast_matchers::MatchFinder &finder) override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};