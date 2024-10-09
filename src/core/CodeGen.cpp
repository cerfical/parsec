#include "core/CodeGen.hpp"
#include "core/SymbolGrammar.hpp"

#include "fsm/DfaStateGen.hpp"
#include "fsm/ElrAutomaton.hpp"
#include "util/strings.hpp"

#include <inja/inja.hpp>
#include <iterator>

namespace parsec {
    namespace {
        class GenerateJsonLexStates : private fsm::DfaStateGen::StateSink {
        public:

            inja::json run(const SymbolGrammar* tokens) {
                states_ = inja::json::array();

                fsm::DfaStateGen()
                    .setStateSink(this)
                    .setInputGrammar(tokens)
                    .generate();

                return std::move(states_);
            }

        private:
            void addState(int id) override {
                states_.push_back({
                    {             "id",                  id },
                    { "is_start_state",             id == 0 },
                    { "is_match_state",               false },
                    {    "transitions", inja::json::array() },
                    {          "match",                  "" }
                });
            }

            void addStateTransition(int state, int target, const Symbol& label) override {
                states_[state]["transitions"].push_back({
                    {  "label", strings::escape(label.text()) },
                    { "target",                        target }
                });
            }

            void setStateMatch(int state, const Symbol& match) override {
                states_[state]["is_match_state"] = true;
                states_[state]["match"] = match.text();
            }

            inja::json states_;
        };


        inja::json makeTransitions(std::span<const fsm::StateTrans> transitions) {
            auto json = inja::json::array();
            for(const auto& t : transitions) {
                json.push_back({
                    {  "label", strings::escape(t.label.text()) },
                    { "target",                        t.target }
                });
            }
            return json;
        }

        inja::json generateJsonSymbols(const SymbolGrammar* grammar) {
            auto json = inja::json::array();
            if(grammar) {
                for(const auto& s : grammar->symbols()) {
                    json.push_back(s.text());
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
                        {          "match",             s.match().text() },
                        {       "backlink",                 s.backlink() }
                    });
                }
            }
            return json;
        }
    }


    void CodeGen::generate() {
        if(!output_) {
            return;
        }

        const inja::json vars = {
            {      "token_names",         generateJsonSymbols(tokens_) },
            {       "lex_states", GenerateJsonLexStates().run(tokens_) },
            { "parse_rule_names",          generateJsonSymbols(rules_) },
            {     "parse_states",              makeParseStates(rules_) }
        };

        if(tmpl_) {
            const auto tmplStr = std::string(
                std::istreambuf_iterator<char>(*tmpl_),
                std::istreambuf_iterator<char>()
            );

            inja::render_to(*output_, tmplStr, vars);
        } else {
            *output_ << vars.dump(4);
        }
    }
}
