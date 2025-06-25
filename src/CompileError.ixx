module;

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

export module parsec:CompileError;

import parsec.scan;

namespace parsec {

    /**
     * @brief Describes a compilation error.
     */
    export class CompileError : public std::runtime_error {
    public:

        /**
         * @brief Construct an error description for a syntax error.
         *
         * @param loc The location of the syntax error.
         * @param msg The error description.
         */
        static CompileError syntaxError(const scan::SourceLoc& loc, const std::string& msg) {
            return { loc, msg };
        }


        /**
         * @brief Construct a description for a *name redefinition*.
         *
         * @param nameLoc The location of one of the multiply-defined names.
         */
        static CompileError nameRedefine(const scan::SourceLoc& nameLoc) {
            return { nameLoc, "name redefinition" };
        }


        /**
         * @brief Construct a description for a *reserved name redefinition*.
         *
         * @param nameLoc The location of the name definition.
         */
        static CompileError reservedNameRedefine(const scan::SourceLoc& nameLoc) {
            return { nameLoc, "trying to redefine a reserved name" };
        }


        /**
         * @brief Construct a description for an *empty name definition*.
         *
         * @param nameLoc The location of the name definition.
         */
        static CompileError emptyName(const scan::SourceLoc& nameLoc) {
            return { nameLoc, "name must contain at least one letter" };
        }


        /**
         * @brief Construct a description for a *reference to an undefined name*.
         *
         * @param nameRefLoc The location of the name reference.
         */
        static CompileError undefinedName(const scan::SourceLoc& nameRefLoc) {
            return { nameRefLoc, "use of undefined name" };
        }


        /**
         * @brief Construct a description for *conflicting patterns*.
         *
         * @param pat1Loc The location of the first pattern.
         * @param pat2Name The name of the second pattern.
         */
        static CompileError patternConflict(const scan::SourceLoc& pat1Loc, std::string_view pat2Name) {
            return { pat1Loc, std::format("pattern is in conflict with pattern \"{}\"", pat2Name) };
        }


        /**
         * @brief Construct a description for *conflicting rules*.
         *
         * @param rule1Loc The location of the first rule.
         * @param rule2Name The name of the second rule.
         */
        static CompileError ruleConflict(const scan::SourceLoc& rule1Loc, std::string_view rule2Name) {
            return { rule1Loc, std::format("rule is in conflict with rule \"{}\"", rule2Name) };
        }


        /**
         * @brief Construct an error with a description and its location.
         *
         * @param loc The error location.
         * @param msg The error description.
         */
        CompileError(const scan::SourceLoc& loc, const std::string& msg)
            : std::runtime_error(msg)
            , loc_(loc) {}


        /**
         * @brief The location where the error occurred.
         */
        const scan::SourceLoc& loc() const noexcept {
            return loc_;
        }


    private:
        scan::SourceLoc loc_;
    };

}
