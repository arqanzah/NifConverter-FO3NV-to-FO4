#include "NiReference.h"

namespace nif {

	NiReference::~NiReference() {}

	NiReference::NiReference(NiReferenceable* owner) : owner(owner) {
		this->owner->getReferences().push_back(this);
	}


	NiReferenceable* NiReference::getOwner() {
		return owner;
	}

	NiReferenceable* NiReference::get() {
		return prop;
	}



	void NiReference::set(NiReferenceable* property) {

		if (property) {

			if (this->prop) {

				auto it = std::find(this->prop->getReferencedBy().begin(), this->prop->getReferencedBy().end(), this);
				if (it != this->prop->getReferencedBy().end()) {
					this->prop->getReferencedBy().erase(it);
				}

			}

			this->prop = property;
			this->prop->getReferencedBy().push_back(this);

		}
	}

	void NiReference::clear() {

		if (this->prop) {

			auto it = std::find(this->prop->getReferencedBy().begin(), this->prop->getReferencedBy().end(), this);
			if (it != this->prop->getReferencedBy().end()) {
				this->prop->getReferencedBy().erase(it);
			}

			prop = nullptr;

		}

	}

	void NiReference::read(NiStream& stream) {

		int32_t index = -1;

		stream >> index;

		if (index > -1) {
			prop = new NiReferenceable();
			prop->setIndex(index);
		}

	};

	void NiReference::write(NiStream& stream) {

		int32_t index = -1;

		if (prop) {
			index = prop->getIndex();
		}

		stream << index;

	};

}

