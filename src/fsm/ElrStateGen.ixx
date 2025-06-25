module;

#include "bnf/SymbolGrammar.hpp"

export module parsec.fsm:ElrStateGen;

namespace parsec::fsm {

    /**
     * @brief Generates states for an ELR automaton.
     */
    export class ElrStateGen {
    public:

        /**
         * @brief Stores generated states in arbitrary format.
         */
        class StateSink {
        public:

            /**
             * @brief Create a new state with a unique identifier.
             */
            virtual void addState(int id) = 0;


            /**
             * @brief Create a token-labeled transition between two states.
             */
            virtual void addStateTokenTransition(int state, int target, const bnf::Symbol& label) = 0;


            /**
             * @brief Create a rule-labeled transition between two states.
             */
            virtual void addStateRuleTransition(int state, int target, const bnf::Symbol& label) = 0;


            /**
             * @brief Add a backlink to a state.
             */
            virtual void addStateBacklink(int state, int backlink) = 0;


            /**
             * @brief Set a backlink for a state to use on a match.
             */
            virtual void setActiveBacklink(int state, int backlink) = 0;


            /**
             * @brief Set a match for a state to report if there is no transition to take.
             */
            virtual void setStateMatch(int state, const bnf::Symbol& match) = 0;

        protected:
            ~StateSink() = default;
        };


        ElrStateGen() = default;

        ElrStateGen(const ElrStateGen&) = delete;
        ElrStateGen& operator=(const ElrStateGen&) = delete;

        ElrStateGen(ElrStateGen&&) noexcept = default;
        ElrStateGen& operator=(ElrStateGen&&) noexcept = default;

        ~ElrStateGen() = default;


        /** @{ */
        /**
         * @brief Set an input grammar that defines a language for an automaton to recognize.
         */
        ElrStateGen& setInputGrammar(const bnf::SymbolGrammar* grammar) {
            grammar_ = grammar;
            return *this;
        }


        /**
         * @brief Set a sink to store the generated states.
         */
        ElrStateGen& setStateSink(StateSink* sink) {
            sink_ = sink;
            return *this;
        }


        /**
         * @brief Start the generation process.
         */
        void generate();
        /** @} */


    private:
        const bnf::SymbolGrammar* grammar_ = {};
        StateSink* sink_ = {};
    };

}
