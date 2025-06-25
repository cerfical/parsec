module;

#include <istream>
#include <ostream>

export module parsec:Compiler;

import :CodeGen;

namespace parsec {

    /**
     * @brief Compiles a grammar spec into a parser recognizing the grammar language.
     */
    export class Compiler {
    public:

        Compiler() = default;

        Compiler(const Compiler&) = delete;
        Compiler& operator=(const Compiler&) = delete;

        Compiler(Compiler&&) noexcept = default;
        Compiler& operator=(Compiler&&) noexcept = default;

        ~Compiler() = default;


        /** @{ */
        /**
         * @brief Set an input stream to use as the source of a template for the generated code.
         */
        void setOutputTemplateSource(std::istream* tmpl) {
            codegen_.setOutputTemplateSource(tmpl);
        }


        /**
         * @brief Set an output stream to receive the generated code.
         */
        void setOutputSink(std::ostream* output) {
            codegen_.setOutputSink(output);
        }


        /**
         * @brief Set an input stream containing the grammar to compile.
         */
        void setInputSource(std::istream* input) {
            input_ = input;
        }


        /**
         * @brief Start the compilation process.
         */
        void compile();
        /** @} */


    private:
        std::istream* input_ = {};
        CodeGen codegen_;
    };

}
