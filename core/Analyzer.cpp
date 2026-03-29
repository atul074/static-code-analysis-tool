#include "Analyzer.h"

#include "../rules/HSCAA.1.2/ReturnValueUsedCheck.h"
#include "../rules/HSCAA.2.1/UnusedVariableCheck.h"
#include "../rules/HSCAA.2.2/UnusedParameterCheck.h"
#include "../rules/HSCAA.2.3/UnusedTypeCheck.h"
#include "../rules/HSCAA.2.4/UnusedFunctionCheck.h"
#include "../rules/HSCAG.4.2/NameHidingCheck.h"
#include "../rules/HSCAG.8.3/DanglingPointerAssignmentCheck.h"
#include "../rules/HSCAG.8.4/RefQualifierCheck.h"
#include "../rules/HSCAP.1.1/DynamicTypeUsageCheck.h"
#include "../rules/HSCAP.1.2/BaseInitializationCheck.h"
#include "../rules/HSCAP.1.3/ExplicitConstructorCheck.h"
#include "../rules/HSCAN.3.1/VirtualSpecifierCheck.h"
#include "../rules/HSCAN.3.2/VirtualFunctionDefaultArgsCheck.h"
#include "../rules/HSCAS.1.1/ExceptionPointerCheck.h"
#include "../rules/HSCAS.4.1/ExceptionNoexceptCheck.h"
#include "../rules/HSCAI.2.1/VirtualBaseCastCheck.h"
#include "../rules/HSCAI.2.2/CStyleAndFunctionalCastCheck.h"
#include "../rules/HSCAI.2.3/ConstCastAwayQualifierRule.h"

Analyzer::Analyzer() {}

void Analyzer::registerRules() {

    rules.push_back(std::make_unique<ReturnValueUsedCheck>());
    rules.push_back(std::make_unique<UnusedVariableCheck>());
    rules.push_back(std::make_unique<UnusedParameterCheck>());
    rules.push_back(std::make_unique<UnusedTypeCheck>());
    rules.push_back(std::make_unique<UnusedFunctionCheck>());
    rules.push_back(std::make_unique<NameHidingCheck>());
    rules.push_back(std::make_unique<DanglingPointerAssignmentCheck>());
    rules.push_back(std::make_unique<RefQualifierCheck>());
    rules.push_back(std::make_unique<DynamicTypeUsageCheck>());
    rules.push_back(std::make_unique<BaseInitializationCheck>());
    rules.push_back(std::make_unique<ExplicitConstructorCheck>());
    rules.push_back(std::make_unique<VirtualSpecifierCheck>());
    rules.push_back(std::make_unique<VirtualFunctionDefaultArgsCheck>());
    rules.push_back(std::make_unique<ExceptionPointerCheck>());
    rules.push_back(std::make_unique<ExceptionNoexceptCheck>());
    rules.push_back(std::make_unique<VirtualBaseCastCheck>());
    rules.push_back(std::make_unique<CStyleAndFunctionalCastCheck>());
    rules.push_back(std::make_unique<ConstCastAwayQualifierRule>());
}

void Analyzer::setupMatchers() {
    for (auto &rule : rules) {
        rule->registerMatchers(finder);
    }
}

clang::ast_matchers::MatchFinder& Analyzer::getFinder() {
    return finder;
}