#pragma once

#include "../../framework/Rule.h"
#include "../../core/Diagnostic.h"

class ExceptionPointerCheck : public Rule {
public:
    std::string name() const override {
        return "HSCAS.1.1";
    }

    void registerMatchers(clang::ast_matchers::MatchFinder &finder) override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};