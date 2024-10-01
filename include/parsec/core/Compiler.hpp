#pragma once

#include "CodeGen.hpp"

#include <istream>
#include <ostream>

namespace parsec {

    class Compiler {
    public:

        Compiler() noexcept = default;

        Compiler(const Compiler&) = delete;
        Compiler& operator=(const Compiler&) = delete;

        Compiler(Compiler&&) noexcept = default;
        Compiler& operator=(Compiler&&) noexcept = default;

        ~Compiler() noexcept = default;


        void setOutputTemplateSource(std::istream* tmpl) noexcept {
            codegen_.setOutputTemplateSource(tmpl);
        }

        void setOutputSink(std::ostream* output) noexcept {
            codegen_.setOutputSink(output);
        }

        void setInputSource(std::istream* input) noexcept {
            input_ = input;
        }


        void compile();


    private:
        std::istream* input_ = {};
        CodeGen codegen_;
    };

}
