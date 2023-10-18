#include "fg/NodePrinter.hpp"
#include "fg/nodes.hpp"

namespace parsec::fg {
	void NodePrinter::visitNode(const RuleRef& n) {
		*m_out << n.ruleName();
	}

	void NodePrinter::visitNode(const AlternNode& n) {
		*m_out << '(' << *n.left() << " | " << *n.right() << ')';
	}

	void NodePrinter::visitNode(const ConcatNode& n) {
		*m_out << '(' << *n.left() << ' ' << *n.right() << ')';
	}
}