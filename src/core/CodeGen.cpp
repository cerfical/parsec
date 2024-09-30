#include "core/CodeGen.hpp"

#include "dfa/Automaton.hpp"
#include "dfa/State.hpp"

#include "elr/Automaton.hpp"
#include "elr/State.hpp"

#include "util/string_util.hpp"

#include <inja/inja.hpp>

#include <iterator>

namespace parsec {
    namespace {
        inja::json jsonifySymbol(const Symbol& symbol) {
            return symbol.value();
        }


        inja::json jsonifyStateTransitions(std::span<const dfa::StateTrans> transitions) {
            auto json = inja::json::array();
            for(const auto& t : transitions) {
                json.push_back({
                    {  "label", string_util::escape(t.label.value()) },
                    { "target",                             t.target }
                });
            }
            return json;
        }

        inja::json jsonifyLexStates(const SymbolGrammar* tokens) {
            auto json = inja::json::array();
            if(tokens) {
                const auto dfa = dfa::Automaton(*tokens);
                for(const auto& s : dfa.states()) {
                    json.push_back({
                        {          "id",                                   s.id() },
                        { "transitions", jsonifyStateTransitions(s.transitions()) },
                        {       "match",                  s.matchedRule().value() }
                    });
                }
            }
            return json;
        }


        inja::json jsonifyNumbers(std::span<const int> numbers) {
            auto json = inja::json::array();
            for(const auto& n : numbers) {
                json.push_back(n);
            }
            return json;
        }

        inja::json jsonifyStateTransitions(std::span<const elr::StateTrans> transitions) {
            auto json = inja::json::array();
            for(const auto& t : transitions) {
                json.push_back({
                    {  "label", t.label.value() },
                    { "target",        t.target }
                });
            }
            return json;
        }

        inja::json jsonifyParseStates(const SymbolGrammar* rules) {
            auto json = inja::json::array();
            if(rules) {
                const auto elr = elr::Automaton(*rules);
                for(const auto& s : elr.states()) {
                    json.push_back({
                        {        "id",                              s.id() },
                        {    "shifts", jsonifyStateTransitions(s.shifts()) },
                        {     "gotos",  jsonifyStateTransitions(s.gotos()) },
                        { "backlinks",       jsonifyNumbers(s.backLinks()) },
                        {     "match",    s.reduction().reduceRule.value() },
                        {  "backlink",              s.reduction().backLink }
                    });
                }
            }
            return json;
        }


        inja::json jsonifyGrammarSymbols(const SymbolGrammar* grammar) {
            auto json = inja::json::array();
            if(grammar) {
                for(const auto& s : grammar->symbols()) {
                    json.push_back(s.value());
                }
            }
            return json;
        }
    }


    void CodeGen::generate() {
        if(!tmpl_ || !output_) {
            return;
        }

        const inja::json vars = {
            {  "token_names", jsonifyGrammarSymbols(tokens_) },
            {   "lex_states",      jsonifyLexStates(tokens_) },
            {   "rule_names",  jsonifyGrammarSymbols(rules_) },
            { "parse_states",     jsonifyParseStates(rules_) }
        };

        const auto tmplStr = std::string(
            std::istreambuf_iterator<char>(*tmpl_),
            std::istreambuf_iterator<char>()
        );

        inja::render_to(*output_, tmplStr, vars);
    }
}
