#include "Compiler.hpp"
#include "CompileError.hpp"

#include "bnf/RegularExpr.hpp"
#include "bnf/Symbol.hpp"
#include "bnf/SymbolGrammar.hpp"

#include "fsm/NameConflictError.hpp"
#include "scan/SourceLoc.hpp"

#include "pars/Parser.hpp"
#include "pars/Token.hpp"

#include "pars/ast/AlternRuleNode.hpp"
#include "pars/ast/ConcatRuleNode.hpp"
#include "pars/ast/EmptyNode.hpp"
#include "pars/ast/EmptyRuleNode.hpp"
#include "pars/ast/InlineTokenNode.hpp"
#include "pars/ast/ListNode.hpp"
#include "pars/ast/NamedRuleNode.hpp"
#include "pars/ast/NamedTokenNode.hpp"
#include "pars/ast/Node.hpp"
#include "pars/ast/NodeVisitor.hpp"
#include "pars/ast/OptionalRuleNode.hpp"
#include "pars/ast/PlusRuleNode.hpp"
#include "pars/ast/StarRuleNode.hpp"
#include "pars/ast/SymbolRuleNode.hpp"

#include "regex/ParseError.hpp"
#include "regex/ast/ExprNode.hpp"
#include "regex/make.hpp"

#include "text/text.hpp"

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


        bnf::Symbol makeName(const Token& name) {
            return text::toPascalCase(name.text());
        }


        class AstTraverser : private NodeVisitor {
        public:

            void traverse(const Node& ast) {
                ast.accept(*this);
            }

        private:
            void visit(const NamedRuleNode& n) override {
                n.rule()->accept(*this);
            }

            void visit(const ConcatRuleNode& n) override {
                n.left()->accept(*this);
                n.right()->accept(*this);
            }

            void visit(const AlternRuleNode& n) override {
                n.left()->accept(*this);
                n.right()->accept(*this);
            }

            void visit(const OptionalRuleNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const PlusRuleNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const StarRuleNode& n) override {
                n.inner()->accept(*this);
            }

            void visit(const ListNode& n) override {
                n.head()->accept(*this);
                n.tail()->accept(*this);
            }

            void visit(const SymbolRuleNode& n) override {}
            void visit(const InlineTokenNode& n) override {}
            void visit(const EmptyRuleNode& n) override {}
            void visit(const NamedTokenNode& n) override {}
            void visit(const EmptyNode& n) override {}
        };


        class PatternNameCache {
        public:

            void insertEntry(const std::string& pattern, const bnf::Symbol& name) {
                patternsToNames_[pattern] = name;
            }

            const bnf::Symbol* lookupName(const std::string& pattern) const {
                const auto patternToNameIt = patternsToNames_.find(pattern);
                if(patternToNameIt != patternsToNames_.end()) {
                    return &patternToNameIt->second;
                }
                return nullptr;
            }

        private:
            std::unordered_map<std::string, bnf::Symbol> patternsToNames_;
        };


        class NameTable {
        public:

            void insertEntry(const bnf::Symbol& name, const Token& tok) {
                namesToTokens_[name] = tok;
            }

            const Token* lookupToken(const bnf::Symbol& name) const {
                const auto nameToTokenIt = namesToTokens_.find(name);
                if(nameToTokenIt != namesToTokens_.end()) {
                    return &nameToTokenIt->second;
                }
                return nullptr;
            }

            bool contains(const bnf::Symbol& name) const {
                return namesToTokens_.contains(name) || name == EofTokenName;
            }

        private:
            std::unordered_map<bnf::Symbol, Token> namesToTokens_;
        };


        void collectNamesAndPatterns(const Node& ast, NameTable& names, PatternNameCache& patterns) {
            class Impl : public AstTraverser {
            public:

                Impl(NameTable& names, PatternNameCache& patterns)
                    : names_(&names), patterns_(&patterns) {}

            private:
                void visit(const NamedTokenNode& n) override {
                    insertName(n.name());
                    patterns_->insertEntry(n.pattern().text(), makeName(n.name()));
                }

                void visit(const NamedRuleNode& n) override {
                    insertName(n.name());
                    // no need to visit the rule itself
                }

                void insertName(const Token& name) {
                    const auto unifiedName = makeName(name);
                    if(!unifiedName) {
                        throw CompileError::emptyName(name.loc());
                    }

                    if(isReservedName(unifiedName)) {
                        throw CompileError::reservedNameRedefine(name.loc());
                    }

                    if(names_->contains(unifiedName)) {
                        throw CompileError::nameRedefine(name.loc());
                    }
                    names_->insertEntry(unifiedName, name);
                }

                static bool isReservedName(const bnf::Symbol& name) {
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
                void visit(const SymbolRuleNode& n) override {
                    if(!names_->contains(makeName(n.value()))) {
                        throw CompileError::undefinedName(n.value().loc());
                    }
                }

                const NameTable* names_ = {};
            } impl(names);
            impl.traverse(ast);
        }


        bnf::SymbolGrammar compileTokenGrammar(const Node& ast, NameTable& names, PatternNameCache& patterns) {
            class Impl : private AstTraverser {
            public:

                Impl(NameTable& names, PatternNameCache& patterns)
                    : names_(&names), patterns_(&patterns) {}

                bnf::SymbolGrammar operator()(const Node& ast) {
                    traverse(ast);
                    return std::move(tokens_);
                }

            private:
                void visit(const NamedTokenNode& n) override {
                    defineToken(makeName(n.name()), n.pattern());
                }

                void visit(const InlineTokenNode& n) override {
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

                void defineToken(const bnf::Symbol& name, const Token& pattern) {
                    bnf::RegularExpr regex;
                    try {
                        regex = bnf::RegularExpr(pattern.text());
                    } catch(const regex::ParseError& err) {
                        const auto& patLoc = pattern.loc();
                        const auto& errLoc = err.loc();

                        // adjust the error location to take into account the pattern's token
                        static constexpr int LeftPatternDelim = 1;
                        const auto newLoc = scan::SourceLoc{
                            .offset = patLoc.offset + errLoc.offset + LeftPatternDelim,
                            .colCount = errLoc.colCount,
                            .line = patLoc.line,
                        };
                        throw CompileError::syntaxError(newLoc, err.what());
                    }
                    tokens_.define(name, regex);
                }

                bnf::SymbolGrammar tokens_;
                std::size_t nextUnnamedTokenId_ = 0;

                NameTable* names_ = {};
                PatternNameCache* patterns_ = {};
            } impl(names, patterns);
            return impl(ast);
        }


        bnf::SymbolGrammar compileRuleGrammar(const Node& ast, const NameTable& names, const PatternNameCache& patterns) {
            class Impl : private AstTraverser {
            public:

                Impl(const NameTable& names, const PatternNameCache& patterns)
                    : names_(&names), patterns_(&patterns) {}

                bnf::SymbolGrammar operator()(const Node& ast) {
                    traverse(ast);
                    return std::move(rules_);
                }

            private:
                void visit(const NamedRuleNode& n) override {
                    traverse(*n.rule());

                    const auto unifedName = makeName(n.name());
                    rules_.define(unifedName, rule_);

                    // make the first symbol encountered the start symbol
                    if(!rules_.root()) {
                        rules_.setRoot(unifedName);
                    }
                }

                void visit(const InlineTokenNode& n) override {
                    rule_ = regex::atom(patterns_->lookupName(n.pattern().text())->text());
                }

                void visit(const EmptyRuleNode& /*n*/) override {
                    rule_ = regex::empty();
                }

                void visit(const SymbolRuleNode& n) override {
                    rule_ = regex::atom(makeName(n.value()).text());
                }

                void visit(const ConcatRuleNode& n) override {
                    const regex::NodePtr left = (traverse(*n.left()), rule_);
                    rule_ = (traverse(*n.right()), regex::concat(left, rule_));
                }

                void visit(const AlternRuleNode& n) override {
                    const regex::NodePtr left = (traverse(*n.left()), rule_);
                    rule_ = (traverse(*n.right()), regex::altern(left, rule_));
                }

                void visit(const OptionalRuleNode& n) override {
                    traverse(*n.inner());
                    rule_ = regex::optional(rule_);
                }

                void visit(const PlusRuleNode& n) override {
                    traverse(*n.inner());
                    rule_ = regex::plusClosure(rule_);
                }

                void visit(const StarRuleNode& n) override {
                    traverse(*n.inner());
                    rule_ = regex::starClosure(rule_);
                }


                bnf::SymbolGrammar rules_;
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

        NodePtr ast;
        try {
            ast = Parser::parseFrom(*input_);
        } catch(const ParseError& err) {
            throw CompileError::syntaxError(err.loc(), err.what());
        }

        collectNamesAndPatterns(*ast, names, patterns);
        checkForUndefinedNames(*ast, names);

        auto tokens = compileTokenGrammar(*ast, names, patterns);
        tokens.define(EofTokenName);
        tokens.define(WsTokenName);

        const auto rules = compileRuleGrammar(*ast, names, patterns);

        codegen_.setRuleGrammar(&rules);
        codegen_.setTokenGrammar(&tokens);

        try {
            codegen_.generate();
        } catch(const fsm::NameConflictError& err) {
            const auto* const srcTok1 = names.lookupToken(err.name1().text());
            const auto* const srcTok2 = names.lookupToken(err.name2().text());

            if(tokens.contains(err.name1())) {
                throw CompileError::patternConflict(srcTok1->loc(), srcTok2->text());
            }
            throw CompileError::ruleConflict(srcTok1->loc(), srcTok2->text());
        }
    }

}
