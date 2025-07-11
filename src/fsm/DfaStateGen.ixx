export module parsec.fsm:DfaStateGen;

import parsec.bnf;

namespace parsec::fsm {

    /**
     * @brief Generates states for a DFA automaton.
     */
    export class DfaStateGen {
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
             * @brief Create a transition between two states.
             */
            virtual void addStateTransition(int state, int target, const bnf::Symbol& label) = 0;


            /**
             * @brief Set a match for a state to report if there is no transition to take.
             */
            virtual void setStateMatch(int state, const bnf::Symbol& match) = 0;

        protected:
            ~StateSink() = default;
        };


        DfaStateGen() = default;

        DfaStateGen(const DfaStateGen&) = delete;
        DfaStateGen& operator=(const DfaStateGen&) = delete;

        DfaStateGen(DfaStateGen&&) noexcept = default;
        DfaStateGen& operator=(DfaStateGen&&) noexcept = default;

        ~DfaStateGen() = default;


        /** @{ */
        /**
         * @brief Set an input grammar that defines a language for an automaton to recognize.
         */
        DfaStateGen& setInputGrammar(const bnf::SymbolGrammar* grammar) {
            grammar_ = grammar;
            return *this;
        }


        /**
         * @brief Set a sink to store the generated states.
         */
        DfaStateGen& setStateSink(StateSink* sink) {
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
