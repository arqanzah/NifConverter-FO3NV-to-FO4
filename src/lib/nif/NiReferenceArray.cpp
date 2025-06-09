#include "NiReferenceArray.h"

namespace nif {

	NiReferenceArray::NiReferenceArray(NiReferenceable* owner) : owner(owner) {}

	NiReferenceArray::~NiReferenceArray() {}

	NiReferenceable* NiReferenceArray::getOwner() {
		return owner;
	}


	const std::vector<NiReference*> NiReferenceArray::get() {

		std::vector<NiReference*> refs;

		for (auto& ref : references) {
			refs.push_back(ref.get());
		}

		return refs;
	}


	void NiReferenceArray::add(NiReferenceable* referenceable) {

		std::unique_ptr<NiReference> ref = std::make_unique<NiReference>(this->owner);
		ref->set(referenceable);
		references.push_back(std::move(ref));

	}


	void NiReferenceArray::read(NiStream& stream) {

		int size = 0;
		stream >> size;

		for (int i = 0; i < size; i++) {
			std::unique_ptr<NiReference> ref = std::make_unique<NiReference>(this->owner);
			ref->read(stream);
			references.push_back(std::move(ref));

		}

	};

	void NiReferenceArray::write(NiStream& stream, bool removeEmptry) {


		std::vector<NiReference*> refs;

		if (removeEmptry) {

			for (auto& ref : references) {
				if (ref->get()) {
					refs.push_back(ref.get());
				}
			}

		} else {

			for (auto& ref : references) {
				refs.push_back(ref.get());
			}

		}


		/*
		references.erase(
		std::remove_if(
		references.begin(),
		references.end(),
		[](std::unique_ptr<NiReference> const &ref) { return !ref->get(); }
		),
		references.end()
		);
		*/

		int size = refs.size();
		stream << size;

		for (auto ref : refs) {
			ref->write(stream);
		}


	};



	NiReferenceArrayShort::NiReferenceArrayShort(NiReferenceable* owner) : NiReferenceArray(owner) {}

	NiReferenceArrayShort::~NiReferenceArrayShort() {}

	void NiReferenceArrayShort::read(NiStream& stream) {

		short size = 0;
		stream >> size;

		for (short i = 0; i < size; i++) {
			std::unique_ptr<NiReference> ref = std::make_unique<NiReference>(this->owner);
			ref->read(stream);
			references.push_back(std::move(ref));

		}

	}

	void NiReferenceArrayShort::write(NiStream& stream, bool removeEmptry) {

		std::vector<NiReference*> refs;

		if (removeEmptry) {
			for (auto& ref : references) {
				if (ref->get()) {
					refs.push_back(ref.get());
				}
			}
		} else {
			for (auto& ref : references) {
				refs.push_back(ref.get());
			}
		}


		short size = (short)refs.size();
		stream << size;

		for (auto ref : refs) {
			ref->write(stream);
		}

	}

}