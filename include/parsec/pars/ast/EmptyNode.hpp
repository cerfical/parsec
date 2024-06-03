#pragma once

#include "Node.hpp"

namespace parsec::pars {

	/**
	 * @brief Empty node.
	*/
	class EmptyNode : public Node {
	public:

		void accept(NodeVisitor& visitor) const override;

	};

}
