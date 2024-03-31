#ifndef PARSEC_PARS_AST_HEADER
#define PARSEC_PARS_AST_HEADER

#include "ast/Node.hpp"
#include "ast/NodeVisitor.hpp"

#include "ast/EmptyNode.hpp"
#include "ast/ListNode.hpp"

#include "ast/InlineToken.hpp"
#include "ast/NamedToken.hpp"

#include "ast/EmptyRule.hpp"
#include "ast/SymbolRule.hpp"
#include "ast/NamedRule.hpp"

#include "ast/ConcatRule.hpp"
#include "ast/AlternRule.hpp"

#include "ast/OptionalRule.hpp"
#include "ast/PlusRule.hpp"
#include "ast/StarRule.hpp"

/**
 * @brief Defines all necessary node types to represent the AST (abstract syntax tree) of the parsed input.
*/
namespace parsec::pars::ast {}

#endif