#include "CodeGen.hpp"

#include "bnf/SymbolGrammar.hpp"

#include <inja/inja.hpp>
#include <iterator>

import parsec.fsm;
import parsec.text;

namespace parsec {
    namespace {
        class GenerateJsonLexStates : private fsm::DfaStateGen::StateSink {
        public:

            inja::json run(const bnf::SymbolGrammar* tokens) {
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
                    {          "id",                  id },
                    { "transitions", inja::json::array() },
                });
            }

            void addStateTransition(int state, int target, const bnf::Symbol& label) override {
                states_[state]["transitions"].push_back({
                    {  "label", text::escape(label.text()) },
                    { "target",                     target }
                });
            }

            void setStateMatch(int state, const bnf::Symbol& match) override {
                states_[state]["match"] = match.text();
            }

            inja::json states_;
        };


        class GenerateJsonParseStates : private fsm::ElrStateGen::StateSink {
        public:

            inja::json run(const bnf::SymbolGrammar* rules) {
                states_ = inja::json::array();

                fsm::ElrStateGen()
                    .setStateSink(this)
                    .setInputGrammar(rules)
                    .generate();

                return std::move(states_);
            }

        private:
            void addState(int id) override {
                states_.push_back({
                    {                "id",                  id },
                    { "token_transitions", inja::json::array() },
                    {  "rule_transitions", inja::json::array() },
                    {         "backlinks", inja::json::array() },
                });
            }

            void addStateTokenTransition(int state, int target, const bnf::Symbol& label) override {
                states_[state]["token_transitions"].push_back({
                    {  "label", text::escape(label.text()) },
                    { "target",                     target }
                });
            }

            void addStateRuleTransition(int state, int target, const bnf::Symbol& label) override {
                states_[state]["rule_transitions"].push_back({
                    {  "label", text::escape(label.text()) },
                    { "target",                     target }
                });
            }

            void addStateBacklink(int state, int backlink) override {
                states_[state]["backlinks"].push_back(backlink);
            }

            void setActiveBacklink(int state, int backlink) override {
                states_[state]["active_backlink"] = backlink;
            }

            void setStateMatch(int state, const bnf::Symbol& match) override {
                states_[state]["match"] = match.text();
            }

            inja::json states_;
        };


        inja::json generateJsonSymbols(const bnf::SymbolGrammar* grammar) {
            auto json = inja::json::array();
            if(grammar) {
                for(const auto& s : grammar->symbols()) {
                    json.push_back(s.text());
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
            {      "token_names",          generateJsonSymbols(tokens_) },
            {       "lex_states",  GenerateJsonLexStates().run(tokens_) },
            { "parse_rule_names",           generateJsonSymbols(rules_) },
            {     "parse_states", GenerateJsonParseStates().run(rules_) }
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
