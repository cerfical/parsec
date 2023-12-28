#ifndef PARSEC_PARS_AST_ROOT_HEADER
#define PARSEC_PARS_AST_ROOT_HEADER

#include "ast/Node.hpp"
#include "ast/NodeVisitor.hpp"
#include "ast/NilNode.hpp"
#include "ast/NodeList.hpp"

#include "ast/InlinePattern.hpp"
#include "ast/NamedPattern.hpp"

#include "ast/NilRule.hpp"
#include "ast/NameRule.hpp"
#include "ast/NamedRule.hpp"

#include "ast/ConcatRule.hpp"
#include "ast/AlternRule.hpp"

#include "ast/OptionalRule.hpp"
#include "ast/PlusRule.hpp"
#include "ast/StarRule.hpp"

/**
 * @brief Defines different node types for the AST (abstract syntax tree) that will be constructed during parsing.
*/
namespace parsec::pars::ast
{ }

#endif