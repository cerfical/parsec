#include "fsm/AutomatonFactory.hpp"

namespace parsec::fsm {
	AutomatonFactory& AutomatonFactory::get() {
		static AutomatonFactory factory;
		return factory;
	}
}