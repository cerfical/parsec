#include "core/CodeGen.hpp"

#include "fsm/DfaAutomaton.hpp"
#include "fsm/ElrAutomaton.hpp"

#include <inja/inja.hpp>

#include <iterator>

namespace parsec {
    namespace {
        inja::json makeTransitions(std::span<const fsm::StateTrans> transitions) {
            auto json = inja::json::array();
            for(const auto& t : transitions) {
                json.push_back({
                    {  "label", t.label.value() },
                    { "target",        t.target }
                });
            }
            return json;
        }

        inja::json makeGrammarSymbols(const SymbolGrammar* grammar) {
            auto json = inja::json::array();
            if(grammar) {
                for(const auto& s : grammar->symbols()) {
                    json.push_back(s.value());
                }
            }
            return json;
        }


        inja::json makeLexStates(const SymbolGrammar* tokens) {
            auto json = inja::json::array();
            if(tokens) {
                const auto dfa = fsm::DfaAutomaton(*tokens);
                for(const auto& s : dfa.states()) {
                    json.push_back({
                        {             "id",                           s.id() },
                        { "is_start_state",                 s.isStartState() },
                        { "is_match_state",                 s.isMatchState() },
                        {    "transitions", makeTransitions(s.transitions()) },
                        {          "match",                s.match().value() }
                    });
                }
            }
            return json;
        }


        inja::json makeBacklinks(std::span<const int> backlinks) {
            auto json = inja::json::array();
            for(const auto& backlink : backlinks) {
                json.push_back(backlink);
            }
            return json;
        }

        inja::json makeParseStates(const SymbolGrammar* rules) {
            auto json = inja::json::array();
            if(rules) {
                const auto elr = fsm::ElrAutomaton(*rules);
                for(const auto& s : elr.states()) {
                    json.push_back({
                        {             "id",                       s.id() },
                        { "is_start_state",             s.isStartState() },
                        { "is_match_state",             s.isMatchState() },
                        {         "shifts",  makeTransitions(s.shifts()) },
                        {          "gotos",   makeTransitions(s.gotos()) },
                        {      "backlinks", makeBacklinks(s.backlinks()) },
                        {          "match",            s.match().value() },
                        {       "backlink",                 s.backlink() }
                    });
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
            {      "token_names", makeGrammarSymbols(tokens_) },
            {       "lex_states",      makeLexStates(tokens_) },
            { "parse_rule_names",  makeGrammarSymbols(rules_) },
            {     "parse_states",     makeParseStates(rules_) }
        };

        const auto tmplStr = std::string(
            std::istreambuf_iterator<char>(*tmpl_),
            std::istreambuf_iterator<char>()
        );

        inja::render_to(*output_, tmplStr, vars);
    }
}
