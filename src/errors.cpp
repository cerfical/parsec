#include "errors.hpp"

#include "utils/char_utils.hpp"
#include "utils/string_utils.hpp"

#include <format>

namespace parsec {
	namespace {
		class ErrorCategory : public std::error_category {
		public:

			static const ErrorCategory* get() {
				static ErrorCategory category;
				return &category;
			}


			std::string message(int code) const override {
				const auto codeEnum = static_cast<ErrorCodes>(code);
				switch(codeEnum) {
					case ErrorCodes::Success: return "success";
					case ErrorCodes::UnexpectedEof: return "unexpected end of file";
					case ErrorCodes::InvalidChar: return "invalid character '{}'";
					case ErrorCodes::UnexpectedChar: return "unexpected '{}'";
					case ErrorCodes::OutOfOrderCharRange: return "character range is out of order";
					case ErrorCodes::InvalidHexChar: return "expected at least one hexadecimal digit";
					case ErrorCodes::UnexpectedToken: return "unexpected \"{}\"";
					case ErrorCodes::TokenMismatch: return "expected \"{}\", but got \"{}\"";
					case ErrorCodes::UnmatchedParenthesis: return "unmatched parenthesis";
				}
				return "unknown error";
			}

			const char* name() const noexcept override {
				return "parsec";
			}

		};
	}


	Error::Error(ErrorCodes code)
		: system_error(static_cast<int>(code), *ErrorCategory::get()) {}


	const char* InvalidCharError::what() const {
		if(m_msg.empty()) {
			m_msg = std::vformat(code().message(), std::make_format_args(char_utils::escape(m_ch)));
		}
		return m_msg.c_str();
	}


	const char* UnexpectedCharError::what() const {
		if(m_msg.empty()) {
			m_msg = std::vformat(code().message(), std::make_format_args(char_utils::escape(m_ch)));
		}
		return m_msg.c_str();
	}


	const char* UnexpectedTokenError::what() const {
		if(m_msg.empty()) {
			m_msg = std::vformat(code().message(), std::make_format_args(string_utils::escape(m_tok)));
		}
		return m_msg.c_str();
	}


	const char* TokenMismatchError::what() const {
		if(m_msg.empty()) {
			m_msg = std::vformat(code().message(), std::make_format_args(
				string_utils::escape(m_expect),
				string_utils::escape(m_got)
			));
		}
		return m_msg.c_str();
	}
}