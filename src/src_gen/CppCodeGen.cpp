#include "src_gen/CppCodeGen.hpp"

#include "dfa/Automaton.hpp"

#include "elr/Automaton.hpp"

#include "util/cpp_util.hpp"
#include "util/string_util.hpp"

#include <format>

namespace parsec::src_gen {
    namespace {
        class CppLexerGen {
        public:

            CppLexerGen(const SymbolGrammar& tokens, const ConfigStore& configs, std::ostream& out)
                : tokens_(tokens), configs_(configs), out_(out) {
                dfa_ = dfa::Automaton(tokens);
            }

            void run() {
                tokenKindsEnum();
                out_ << '\n';
                out_ << '\n';

                tokenClass();
                out_ << '\n';
                out_ << '\n';

                lexerClass();
            }

        private:
            void tokenKindsEnum() {
                out_ << cpp_util::makeEnum("TokenKinds", tokens_.symbols());
            }


            void tokenClass() {
                out_ << R"-(class Token {
public:

    Token() = default;

    Token(const std::string& text, TokenKinds kind, const SourceLoc& loc)
        : text_(text), loc_(loc), kind_(kind) {}


    const std::string& text() const {
        return text_;
    }

    const SourceLoc& loc() const {
        return loc_;
    }

    TokenKinds kind() const {
        return kind_;
    }


    template <TokenKinds K>
    bool is() const {
        return kind() == tok;
    }


private:
    std::string text_;
    SourceLoc loc_;
    TokenKinds kind_ = {};
};

std::ostream& operator<<(std::ostream& out, const Token& tok) {
    return out << "(" << tok.kind() << ": \"" << tok.text() << "\")";
}
)-";
            }


            void lexerClass() {
                out_ << "class Lexer {" << '\n';

                lexerClassPublic();
                lexerClassPrivate();

                out_ << "};" << '\n';
            }


            void lexerClassPublic() {
                out_ << R"(public:

    Lexer() = default;

    explicit Lexer(std::istream& input)
        : input_(&input) {}


    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    Lexer(Lexer&&) = default;
    Lexer& operator=(Lexer&&) = default;


    const Token& peek() {
        if(!token_) {
            token_ = nextToken();
        }
        return *token_;
    }

    Token lex() {
        if(!token_) {
            token_ = nextToken();
        }

        Token tok = std::move(token_.value());
        token_.reset();
        return tok;
    }


    bool isEof() const {
        return isInputEnd();
    }

    SourceLoc loc() const {
        const auto colCount = inputPos_ - tokenStart_;
        const auto startCol = tokenStart_ - inputLinePos_;
        
        return SourceLoc(startCol, colCount, inputLineNo_, inputLinePos_);
    }


    bool skipIf(TokenKinds tok) {
        if(peek().kind() == tok) {
            skip();
            return true;
        }
        return false;
    }

    bool skipIf(std::string_view tok) {
        if(peek().text() == tok) {
            skip();
            return true;
        }
        return false;
    }

    void skip() {
        lex();
    }


)";
            }


            void lexerClassPrivate() {
                out_ << R"(private:
    consteval static unsigned char uchar(char ch) noexcept {
        return static_cast<unsigned char>(ch);
    }

    Token nextToken() {
        const auto kind = parseToken();
        return Token(tokenText_, kind, loc());
    }

)";

                lexerClassParseTokenFunc();
                out_ << '\n';

                out_ << R"(    int getChar() {
        if(isInputEnd()) {
            return std::char_traits<char>::eof();
        }

        const auto ch = input_->get();
        if(ch == uchar('\n')) {
            inputLinePos_ = inputPos_;
            inputLineNo_++;
        }
        inputPos_++;
        return ch;
    }

    int peekChar() {
        if(isInputEnd()) {
            return std::char_traits<char>::eof();
        }
        return input_->peek();
    }

    bool isInputEnd() const {
        if(!input_) {
            return true;
        }

        if(input_->peek() == std::char_traits<char>::eof()) {
            input_->clear(input_->rdstate() ^ std::ios::eofbit);
            return true;
        }
        return false;
    }

    [[noreturn]]
    void error() {
        throw ParseError(isInputEnd() ? "unexpected end of file" : "malformed token", loc());
    }

    std::istream* input_ = {};
    int inputPos_ = 0;
    int inputLineNo_ = 0;
    int inputLinePos_ = 0;

    std::optional<Token> token_;
    std::string tokenText_;
    int tokenStart_ = {};
)";
            }


            void lexerClassParseTokenFunc() {
                out_ << "\t" << "TokenKinds parseToken() {" << '\n';
                out_ << "\t\t" << "TokenKinds kind = {};" << '\n';
                out_ << "\t" << "reset:" << '\n';

                if(const auto& eof = eofToken()) {
                    out_ << "\t\t" << "if(isInputEnd()) {" << '\n';
                    out_ << "\t\t\t" << std::format("kind = TokenKinds::{};", eof.value()) << '\n';
                    out_ << "\t\t\t" << "goto accept;" << '\n';
                    out_ << "\t\t" << "}" << '\n';
                }
                out_ << "\t\t" << "tokenStart_ = inputPos_;" << '\n';
                out_ << "\t\t" << "tokenText_.clear();" << '\n';

                lexStates();

                out_ << "\t" << "accept:" << '\n';
                if(const auto& ws = wsToken()) {
                    out_ << "\t\t" << std::format("if(kind == TokenKinds::{}) {{", ws.value()) << '\n';
                    out_ << "\t\t\t" << "goto reset;" << '\n';
                    out_ << "\t\t" << "}" << '\n';
                }
                out_ << "\t\t" << "return kind;" << '\n';
                out_ << "\t" << "}" << '\n';
            }


            void lexStates() {
                if(dfa_) {
                    out_ << "\t\t" << "goto start;" << '\n';
                    out_ << '\n';

                    for(const auto& state : dfa_.states()) {
                        lexState(state);
                        out_ << '\n';
                    }
                } else {
                    out_ << "\t\t" << "error();" << '\n';
                    out_ << '\n';
                }
            }


            void lexState(const dfa::State& state) {
                out_ << "\t" << std::format("state{}:", state.id()) << '\n';
                out_ << "\t\t" << "tokenText_ += getChar();" << '\n';

                if(state.isStartState()) {
                    out_ << "\t" << "start:" << '\n';
                }

                lexTransitions(state.transitions());

                if(state.isMatchState()) {
                    out_ << "\t\t" << std::format("kind = TokenKinds::{};", state.matchedRule().value()) << '\n';
                    out_ << "\t\t" << "goto accept;" << '\n';
                } else {
                    out_ << "\t\t" << "error();" << '\n';
                }
            }


            void lexTransitions(const std::vector<dfa::StateTrans>& transitions) {
                if(!transitions.empty()) {
                    out_ << "\t\t" << "switch(peekChar()) {" << '\n';
                    for(const auto& trans : transitions) {
                        out_ << "\t\t\t" << std::format("case uchar('{}'): goto state{};", string_util::escape(trans.label.value()), trans.target) << '\n';
                    }
                    out_ << "\t\t" << "}" << '\n';
                }
            }


            const Symbol& eofToken() const {
                return tokens_.resolve(configs_.eofTokenName()).head();
            }


            const Symbol& wsToken() const {
                return tokens_.resolve(configs_.wsTokenName()).head();
            }


            const SymbolGrammar& tokens_;
            const ConfigStore& configs_;

            std::ostream& out_;
            dfa::Automaton dfa_;
        };


        class CppParserGen {
        public:

            CppParserGen(const SymbolGrammar& syntax, std::ostream& out)
                : syntax_(syntax), out_(out) {
                elr_ = elr::Automaton(syntax);
            }

            void run() {
                parseRulesEnum();
                out_ << '\n';
                out_ << '\n';

                parserClass();
            }

        private:
            void parseRulesEnum() {
                out_ << cpp_util::makeEnum("ParseRules", syntax_.symbols());
            }


            void parserClass() {
                out_ << R"(class Parser {
public:

    Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    Parser(Parser&&) = default;
    Parser& operator=(Parser&&) = default;


    void parse(std::string_view str) {
        auto in = std::istringstream(std::string(str));
        parse(in);
    }

    void parse(std::istream& input) {
        lexer_ = Lexer(input);
        startParse();
    }


)";

                parserClassProtected();
                parserClassPrivate();

                out_ << "};" << '\n';
            }


            void parserClassProtected() {
                out_ << "protected:" << '\n';
                for(const auto& symbol : syntax_.symbols()) {
                    out_ << "\t" << std::format("virtual void on{}(std::span<const Token> tokens) {{}}", symbol.value()) << '\n';
                }
                out_ << '\n';
            }


            void parserClassPrivate() {
                out_ << R"(private:
    using ParseHook = void (Parser::*)(std::span<const Token>);
    using StateFunc = void (Parser::*)();


    [[noreturn]] void error() {
        throw ParseError("unexpected token", lexer_.loc());
    }

    void shiftState(StateFunc state) {
        parsedTokens_.push_back(lexer_.lex());
        (this->*state)();
        reduceTokenCount_++;
    }

    void gotoState(StateFunc state) {
        (this->*state)();
    }

    void startReduce(ParseRules rule, ParseHook hook, int backLink) noexcept {
        reduceRule_ = rule;
        reduceHook_ = hook;
        reduceBackLink_ = backLink;
    }

    bool reduce(const int backLinks[]) {
        reduceBackLink_ = backLinks[reduceBackLink_];
        if(reduceBackLink_ == -1) {
            (this->*reduceHook_)(std::span(parsedTokens_.end() - reduceTokenCount_, reduceTokenCount_));
            parsedTokens_.resize(parsedTokens_.size() - reduceTokenCount_);
            reduceTokenCount_ = 0;
            return true;
        }
        return false;
    }

)";

                if(elr_) {
                    out_ << "\t" << "void startParse() {" << '\n';
                    out_ << "\t\t" << std::format("state{}();", elr_.startState().id()) << '\n';
                    out_ << "\t" << "}" << '\n';
                    out_ << '\n';
                    out_ << '\n';

                    for(const auto& state : elr_.states()) {
                        parserState(state);
                        out_ << '\n';
                    }
                } else {
                    out_ << "\t" << "void startParse() {}" << '\n';
                    out_ << '\n';
                }

                out_ << R"(
    ParseHook reduceHook_ = {};
    std::size_t reduceTokenCount_ = 0;
    ParseRules reduceRule_ = {};
    int reduceBackLink_ = -1;

    std::vector<Token> parsedTokens_;
    Lexer lexer_;
)";
            }


            void parserState(const elr::State& state) {
                out_ << "\t" << std::format("void state{}() {{", state.id()) << '\n';

                parserStateBackLinks(state);
                parserStateShifts(state);
                parserStateGotos(state);

                out_ << "\t" << "}" << '\n';
            }


            void parserStateShifts(const elr::State& state) {
                out_ << "\t\t" << "switch(lexer_.peek().kind()) {" << '\n';
                for(const auto& trans : state.shifts()) {
                    out_ << "\t\t\t" << std::format("case TokenKinds::{}: shiftState(&Parser::state{}); break;", trans.label.value(), trans.target) << '\n';
                }

                out_ << "\t\t\t" << "default: " << (state.isReduceState() ? std::format("startReduce(ParseRules::{0}, &Parser::on{0}, {1});", state.reduction().reduceRule.value(), state.reduction().backLink) : "error();")
                     << " break;" << '\n';

                out_ << "\t\t" << "}" << '\n';
            }


            void parserStateGotos(const elr::State& state) {
                out_ << "\t\t" << "while(reduce(backLinks)) switch(reduceRule_) {" << '\n';

                for(const auto& trans : state.gotos()) {
                    out_ << "\t\t\t" << std::format("case ParseRules::{}: gotoState(&Parser::state{}); break;", trans.label.value(), trans.target) << '\n';
                }

                out_ << "\t\t\t" << "default: break;" << '\n';
                out_ << "\t\t" << "}" << '\n';
            }


            void parserStateBackLinks(const elr::State& state) {
                out_ << "\t\t" << "static const int backLinks[] = {";
                for(bool first = true; const auto& backLink : state.backLinks()) {
                    if(!first) {
                        out_ << ',';
                    } else {
                        first = false;
                    }
                    out_ << ' ' << backLink;
                }
                out_ << " };" << '\n';
            }


            const SymbolGrammar& syntax_;

            std::ostream& out_;
            elr::Automaton elr_;
        };
    }


    void CppCodeGen::onLexerGen(const SymbolGrammar& tokens, const ConfigStore& configs) {
        CppLexerGen(tokens, configs, output()).run();

        output() << '\n';
        output() << '\n';
    }


    void CppCodeGen::onParserGen(const SymbolGrammar& syntax, const ConfigStore& /* configs */) {
        CppParserGen(syntax, output()).run();
    }


    void CppCodeGen::onPreambleGen(const ConfigStore& /* configs */) {
        output() << R"(class SourceLoc {
public:

    SourceLoc() = default;

    SourceLoc(int startCol, int colCount, int lineNo, int linePos)
        : startCol_(startCol), colCount_(colCount), lineNo_(lineNo), linePos_(linePos) {}


    int linePos() const {
        return linePos_;
    }

    int lineNo() const {
        return lineNo_;
    }

    int colCount() const {
        return colCount_;
    }

    int startCol() const {
        return startCol_;
    }


    int endCol() const {
        return startCol() + colCount();
    }

    int pos() const {
        return linePos() + startCol();
    }


private:
    int startCol_ = {};
    int colCount_ = {};
    int lineNo_ = {};
    int linePos_ = {};
};

std::ostream& operator<<(std::ostream& out, const SourceLoc& loc) {
    out << '(' << loc.lineNo() + 1 << ": " << loc.startCol() + 1;
    if(loc.colCount()) {
        out << '-' << (loc.endCol() - 1) + 1;
    }
    out << ')';
    return out;
}


class ParseError : public std::runtime_error {
public:
        
    ParseError(const std::string& msg, const SourceLoc& loc)
        : runtime_error(msg), loc_(loc) {}

    const SourceLoc& loc() const {
        return loc_;
    }

private:
    SourceLoc loc_;
};
)";

        output() << '\n';
        output() << '\n';
    }
}
