#include "core/Compiler.hpp"

#include "core/NameConflictError.hpp"
#include "core/ParseError.hpp"
#include "core/RegularExpr.hpp"
#include "core/SourceLoc.hpp"
#include "core/Symbol.hpp"
#include "core/SymbolGrammar.hpp"

#include "pars/Parser.hpp"
#include "pars/Token.hpp"

#include "pars/ast/AlternRule.hpp"
#include "pars/ast/ConcatRule.hpp"
#include "pars/ast/EmptyNode.hpp"
#include "pars/ast/EmptyRule.hpp"
#include "pars/ast/InlineToken.hpp"
#include "pars/ast/ListNode.hpp"
#include "pars/ast/NamedRule.hpp"
#include "pars/ast/NamedToken.hpp"
#include "pars/ast/Node.hpp"
#include "pars/ast/NodeVisitor.hpp"
#include "pars/ast/OptionalRule.hpp"
#include "pars/ast/PlusRule.hpp"
#include "pars/ast/StarRule.hpp"
#include "pars/ast/SymbolAtom.hpp"

#include "regex/make.hpp"
#include "regex/nodes/ExprNode.hpp"

#include "util/string_util.hpp"

#include <cstddef>
#include <format>
#include <string>
#include <unordered_map>
#include <utility>

namespace parsec {
    using namespace pars;

    namespace {
        constexpr auto EofTokenName = "Eof";
        constexpr auto WsTokenName = "Ws";
        constexpr auto UnnamedTokenPrefix = "Unnamed";


        Symbol makeName(const Token& name) {
            return string_util::toPascalCase(name.text());
        }


        class AstTraverser : private NodeVisitor {
        public:

            void traverse(const Node& ast) {
                ast.accept(*this);
            }

        private:
            void visit(const NamedRule& n) override {
                n.rule()->accept(*this);
            }

            void visit(const ConcatRule& n) override {
                n.left()->accept(*this);
                n.right()->accept(*this);
            }

            void visit(const AlternRule& n) override {
                n.left()->accept(*this);
                n.right()->accept(*this);
            }

            void visit(const OptionalRule& n) override {
                n.inner()->accept(*this);
            }

            void visit(const PlusRule& n) override {
                n.inner()->accept(*this);
            }

            void visit(const StarRule& n) override {
                n.inner()->accept(*this);
            }

            void visit(const ListNode& n) override {
                n.head()->accept(*this);
                n.tail()->accept(*this);
            }

            void visit(const SymbolAtom& n) override {}
            void visit(const InlineToken& n) override {}
            void visit(const EmptyRule& n) override {}
            void visit(const NamedToken& n) override {}
            void visit(const EmptyNode& n) override {}
        };


        class PatternNameCache {
        public:

            void insertEntry(const std::string& pattern, const Symbol& name) {
                patternsToNames_[pattern] = name;
            }

            const Symbol* lookupName(const std::string& pattern) const {
                const auto patternToNameIt = patternsToNames_.find(pattern);
                if(patternToNameIt != patternsToNames_.end()) {
                    return &patternToNameIt->second;
                }
                return nullptr;
            }

        private:
            std::unordered_map<std::string, Symbol> patternsToNames_;
        };


        class NameTable {
        public:

            void insertEntry(const Symbol& name, const Token& tok) {
                namesToTokens_[name] = tok;
            }

            const Token* lookupToken(const Symbol& name) const {
                const auto nameToTokenIt = namesToTokens_.find(name);
                if(nameToTokenIt != namesToTokens_.end()) {
                    return &nameToTokenIt->second;
                }
                return nullptr;
            }

            bool contains(const Symbol& name) const {
                return namesToTokens_.contains(name) || name == EofTokenName;
            }

        private:
            std::unordered_map<Symbol, Token> namesToTokens_;
        };


        void collectNamesAndPatterns(const Node& ast, NameTable& names, PatternNameCache& patterns) {
            class Impl : public AstTraverser {
            public:

                Impl(NameTable& names, PatternNameCache& patterns)
                    : names_(&names), patterns_(&patterns) {}

            private:
                void visit(const NamedToken& n) override {
                    insertName(n.name());
                    patterns_->insertEntry(n.pattern().text(), makeName(n.name()));
                }

                void visit(const NamedRule& n) override {
                    insertName(n.name());
                    // no need to visit the rule itself
                }

                void insertName(const Token& name) {
                    const auto unifiedName = makeName(name);
                    if(!unifiedName) {
                        throw ParseError::emptyName(name.loc());
                    }

                    if(isReservedName(unifiedName)) {
                        throw ParseError::reservedNameRedefine(name.loc());
                    }

                    if(names_->contains(unifiedName)) {
                        throw ParseError::nameRedefine(name.loc());
                    }
                    names_->insertEntry(unifiedName, name);
                }

                static bool isReservedName(const Symbol& name) {
                    return name.text().starts_with(UnnamedTokenPrefix) || name == EofTokenName;
                }

                PatternNameCache* patterns_ = {};
                NameTable* names_ = {};
            } impl(names, patterns);
            impl.traverse(ast);
        }


        void checkForUndefinedNames(const Node& ast, const NameTable& names) {
            class Impl : public AstTraverser {
            public:

                Impl(const NameTable& names)
                    : names_(&names) {}

            private:
                void visit(const SymbolAtom& n) override {
                    if(!names_->contains(makeName(n.value()))) {
                        throw ParseError::undefinedName(n.value().loc());
                    }
                }

                const NameTable* names_ = {};
            } impl(names);
            impl.traverse(ast);
        }


        SymbolGrammar compileTokenGrammar(const Node& ast, NameTable& names, PatternNameCache& patterns) {
            class Impl : private AstTraverser {
            public:

                Impl(NameTable& names, PatternNameCache& patterns)
                    : names_(&names), patterns_(&patterns) {}

                SymbolGrammar operator()(const Node& ast) {
                    traverse(ast);
                    return std::move(tokens_);
                }

            private:
                void visit(const NamedToken& n) override {
                    defineToken(makeName(n.name()), n.pattern());
                }

                void visit(const InlineToken& n) override {
                    const auto* const name = patterns_->lookupName(n.pattern().text());
                    if(!name) {
                        const auto fakeName = std::format("{}{}", UnnamedTokenPrefix, nextUnnamedTokenId_++);
                        patterns_->insertEntry(n.pattern().text(), fakeName);
                        names_->insertEntry(fakeName, n.pattern());

                        defineToken(fakeName, n.pattern());
                    } else if(!tokens_.contains(*name)) {
                        defineToken(*name, n.pattern());
                    }
                }

                void defineToken(const Symbol& name, const Token& pattern) {
                    RegularExpr regex;
                    try {
                        regex = RegularExpr(pattern.text());
                    } catch(const ParseError& err) {
                        const auto& patLoc = pattern.loc();
                        const auto& errLoc = err.loc();

                        // adjust the error location to take into account the pattern's token
                        static constexpr int LeftPatternDelim = 1;
                        const auto newLoc = SourceLoc{
                            .offset = patLoc.offset + errLoc.offset + LeftPatternDelim,
                            .colCount = errLoc.colCount,
                            .line = patLoc.line,
                        };
                        throw ParseError(newLoc, err.what());
                    }
                    tokens_.define(name, regex);
                }

                SymbolGrammar tokens_;
                std::size_t nextUnnamedTokenId_ = 0;

                NameTable* names_ = {};
                PatternNameCache* patterns_ = {};
            } impl(names, patterns);
            return impl(ast);
        }


        SymbolGrammar compileRuleGrammar(const Node& ast, const NameTable& names, const PatternNameCache& patterns) {
            class Impl : private AstTraverser {
            public:

                Impl(const NameTable& names, const PatternNameCache& patterns)
                    : names_(&names), patterns_(&patterns) {}

                SymbolGrammar operator()(const Node& ast) {
                    traverse(ast);
                    return std::move(rules_);
                }

            private:
                void visit(const NamedRule& n) override {
                    traverse(*n.rule());

                    const auto unifedName = makeName(n.name());
                    rules_.define(unifedName, rule_);

                    // make the first symbol encountered the start symbol
                    if(!rules_.root()) {
                        rules_.setRoot(unifedName);
                    }
                }

                void visit(const InlineToken& n) override {
                    rule_ = regex::atom(*patterns_->lookupName(n.pattern().text()));
                }

                void visit(const EmptyRule& /*n*/) override {
                    rule_ = regex::empty();
                }

                void visit(const SymbolAtom& n) override {
                    rule_ = regex::atom(makeName(n.value()));
                }

                void visit(const ConcatRule& n) override {
                    const regex::NodePtr left = (traverse(*n.left()), rule_);
                    rule_ = (traverse(*n.right()), regex::concat(left, rule_));
                }

                void visit(const AlternRule& n) override {
                    const regex::NodePtr left = (traverse(*n.left()), rule_);
                    rule_ = (traverse(*n.right()), regex::altern(left, rule_));
                }

                void visit(const OptionalRule& n) override {
                    traverse(*n.inner());
                    rule_ = regex::optional(rule_);
                }

                void visit(const PlusRule& n) override {
                    traverse(*n.inner());
                    rule_ = regex::plusClosure(rule_);
                }

                void visit(const StarRule& n) override {
                    traverse(*n.inner());
                    rule_ = regex::starClosure(rule_);
                }


                SymbolGrammar rules_;
                regex::NodePtr rule_;

                const NameTable* names_ = {};
                const PatternNameCache* patterns_ = {};
            } impl(names, patterns);
            return impl(ast);
        }
    }


    void Compiler::compile() {
        if(!input_) {
            return;
        }

        PatternNameCache patterns;
        NameTable names;

        const auto ast = Parser::parseFrom(*input_);
        collectNamesAndPatterns(*ast, names, patterns);
        checkForUndefinedNames(*ast, names);

        auto tokens = compileTokenGrammar(*ast, names, patterns);
        tokens.define(EofTokenName, {});
        tokens.define(WsTokenName, {});

        const auto rules = compileRuleGrammar(*ast, names, patterns);

        codegen_.setRuleGrammar(&rules);
        codegen_.setTokenGrammar(&tokens);

        try {
            codegen_.generate();
        } catch(const NameConflictError& err) {
            const auto* const srcTok1 = names.lookupToken(err.name1().text());
            const auto* const srcTok2 = names.lookupToken(err.name2().text());

            if(tokens.contains(err.name1())) {
                throw ParseError::patternConflict(srcTok1->loc(), srcTok2->text());
            }
            throw ParseError::ruleConflict(srcTok1->loc(), srcTok2->text());
        }
    }

}
