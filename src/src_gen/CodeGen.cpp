#include "src_gen/CodeGen.hpp"

namespace parsec::src_gen {
	void CodeGen::run(const fg::SymbolGrammar& tokens, const fg::SymbolGrammar& syntax, const ConfigStore& configs) {
		if(m_out) {
			const auto utils = onPreambleGen(configs);
			const auto lexer = onLexerGen(tokens, configs);
			const auto parser = onParserGen(syntax, configs);

			*m_out << utils + (utils.empty() ? "" : "\n\n")
				+ lexer + (lexer.empty() ? "" : "\n\n")
				+ parser;
		}
	}
}