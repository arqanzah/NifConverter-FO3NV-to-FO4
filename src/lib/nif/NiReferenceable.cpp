#include "NiReferenceable.h"

namespace nif {

	NiReferenceable::NiReferenceable() {}

	NiReferenceable::~NiReferenceable() {}

	int32_t NiReferenceable::getIndex() {
		return index;
	}

	void NiReferenceable::setIndex(const int32_t index) {
		this->index = index;
	}

	std::vector<NiReferenceInterface*>& NiReferenceable::getReferencedBy() {
		return this->referencedBy;
	}

	std::vector<NiReferenceInterface*>& NiReferenceable::getReferences() {
		return this->references;
	}

}