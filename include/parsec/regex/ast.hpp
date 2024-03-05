#ifndef PARSEC_REGEX_AST_ROOT_HEADER
#define PARSEC_REGEX_AST_ROOT_HEADER

#include "ast/NodeVisitor.hpp"
#include "ast/NodeTraverser.hpp"

#include "ast/CharAtom.hpp"
#include "ast/NilExpr.hpp"

#include "ast/PlusExpr.hpp"
#include "ast/StarExpr.hpp"
#include "ast/OptionalExpr.hpp"

#include "ast/ConcatExpr.hpp"
#include "ast/AlternExpr.hpp"

/**
 * @brief Implements abstract syntax tree nodes for regular expressions.
*/
namespace parsec::regex::ast
{ }

#endif