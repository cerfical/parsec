#include "CompileError.hpp"

#include <format>
#include <string>
#include <string_view>

namespace parsec {

    CompileError CompileError::syntaxError(const scan::SourceLoc& loc, const std::string& msg) {
        return { loc, msg };
    }

    CompileError CompileError::nameRedefine(const scan::SourceLoc& nameLoc) {
        return { nameLoc, "name redefinition" };
    }

    CompileError CompileError::reservedNameRedefine(const scan::SourceLoc& nameLoc) {
        return { nameLoc, "trying to redefine a reserved name" };
    }

    CompileError CompileError::emptyName(const scan::SourceLoc& nameLoc) {
        return { nameLoc, "name must contain at least one letter" };
    }

    CompileError CompileError::undefinedName(const scan::SourceLoc& nameRefLoc) {
        return { nameRefLoc, "use of undefined name" };
    }


    CompileError CompileError::patternConflict(const scan::SourceLoc& pat1Loc, std::string_view pat2Name) {
        return { pat1Loc, std::format("pattern is in conflict with pattern \"{}\"", pat2Name) };
    }

    CompileError CompileError::ruleConflict(const scan::SourceLoc& rule1Loc, std::string_view rule2Name) {
        return { rule1Loc, std::format("rule is in conflict with rule \"{}\"", rule2Name) };
    }

}
