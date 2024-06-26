#pragma once

#include "Error.hpp"
#include "SourceLoc.hpp"

#include <string>
#include <string_view>

namespace parsec {

    /**
     * @brief Describes a syntax error in the input grammar.
     */
    class ParseError : public Error {
    public:

        /**
         * @brief Construct a description for a *name redefinition*.
         *
         * @param nameLoc The location of one of the multiply-defined names.
         */
        static ParseError nameRedefine(const SourceLoc& nameLoc);


        /**
         * @brief Construct a description for a *reserved name redefinition*.
         *
         * @param nameLoc The location of the name definition.
         */
        static ParseError reservedNameRedefine(const SourceLoc& nameLoc);


        /**
         * @brief Construct a description for an *empty name definition*.
         *
         * @param nameLoc The location of the name definition.
         */
        static ParseError emptyName(const SourceLoc& nameLoc);


        /**
         * @brief Construct a description for a *reference to an undefined name*.
         *
         * @param nameRefLoc The location of the name reference.
         */
        static ParseError undefinedName(const SourceLoc& nameRefLoc);


        /**
         * @brief Construct a description for *conflicting patterns*.
         *
         * @param pat1Loc The location of the first pattern.
         * @param pat2Name The name of the second pattern.
         */
        static ParseError patternConflict(const SourceLoc& pat1Loc, std::string_view pat2Name);


        /**
         * @brief Construct a description for *conflicting rules*.
         *
         * @param rule1Loc The location of the first rule.
         * @param rule2Name The name of the second rule.
         */
        static ParseError ruleConflict(const SourceLoc& rule1Loc, std::string_view rule2Name);


        /**
         * @brief Construct a description for an *unexpected end of file*.
         *
         * @param eofLoc The location of the encountered EOF.
         */
        static ParseError unexpectedEof(const SourceLoc& eofLoc);


        /**
         * @brief Construct a description for a *use of an illegal character*.
         *
         * @param chLoc The location of the character.
         * @param ch The value of the character.
         */
        static ParseError invalidChar(const SourceLoc& chLoc, char ch);


        /**
         * @brief Construct a description for an *empty hexadecimal escape sequence*.
         *
         * @param seqLoc The location of the escape sequence.
         */
        static ParseError emptyHexEscapeSeq(const SourceLoc& seqLoc);


        /**
         * @brief Construct a description for an *out-of-order character range*.
         *
         * @param rngLoc The location of the character range.
         */
        static ParseError outOfOrderCharRange(const SourceLoc& rngLoc);


        /**
         * @brief Construct a description for a *character found at an invalid position* situation.
         *
         * @param chLoc The location of the character.
         * @param ch The value of the character.
         */
        static ParseError misplacedChar(const SourceLoc& chLoc, char ch);


        /**
         * @brief Construct a description for a *token found at an invalid position* situation.
         *
         * @param tokLoc The location of the token.
         * @param tokVal The value of the token.
         */
        static ParseError misplacedToken(const SourceLoc& tokLoc, std::string_view tokVal);


        /**
         * @brief Construct a description for a *mismatched token*.
         *
         * @param foundLoc The location of the token found.
         * @param expectVal The value of the expected token.
         */
        static ParseError unmatchedToken(const SourceLoc& foundLoc, std::string_view expectVal);


        /**
         * @brief Construct a generic error containing only the problem location.
         *
         * @param loc The location of the problem.
         */
        explicit ParseError(const SourceLoc& loc)
            : ParseError(loc, "syntax error") {}


        /**
         * @brief Construct an error describing the problem and its location.
         *
         * @param loc The location of the problem.
         * @param msg The description of the problem.
         */
        ParseError(const SourceLoc& loc, const std::string& msg)
            : Error(msg), loc_(loc) {}


        /**
         * @brief The location of the text that was the cause of the problem.
         */
        const SourceLoc& loc() const noexcept {
            return loc_;
        }


    private:
        SourceLoc loc_;
    };

}
