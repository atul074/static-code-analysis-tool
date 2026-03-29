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
#include "../rules/HSCAP.1.4/MemberInitializationCheck.h"
#include "../rules/HSCAP.0.2/SelfAssignmentCheck.h"
#include "../rules/HSCAN.1.2/VirtualInheritanceMismatchCheck.h"
#include "../rules/HSCAN.3.4/MemberFunctionPointerComparisonCheck.h"
#include "../rules/HSCAR.8.1/FunctionTemplateSpecializationCheck.h"


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
    rules.push_back(std::make_unique<MemberInitializationCheck>());
    rules.push_back(std::make_unique<SelfAssignmentCheck>());
    rules.push_back(std::make_unique<VirtualInheritanceMismatchCheck>());
    rules.push_back(std::make_unique<MemberFunctionPointerComparisonCheck>());
    rules.push_back(std::make_unique<FunctionTemplateSpecializationCheck>());
}

void Analyzer::setupMatchers() {
    for (auto &rule : rules) {
        rule->registerMatchers(finder);
    }
}

clang::ast_matchers::MatchFinder& Analyzer::getFinder() {
    return finder;
}