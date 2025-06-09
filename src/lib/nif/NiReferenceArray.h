#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "types.h"
#include "NiReference.h"

namespace nif {

	class NiReferenceArray {

	protected:
		NiReferenceable* owner = nullptr;
		std::vector<std::unique_ptr<NiReference>> references;

	public:
		NiReferenceArray(NiReferenceable* owner);
		virtual ~NiReferenceArray();

		virtual NiReferenceable* getOwner();

		void add(NiReferenceable* referenceable);

		virtual void read(NiStream& stream);

		virtual void write(NiStream& stream, bool removeEmptry = true);

		const std::vector<NiReference*> get();


	};

	class NiReferenceArrayShort : public NiReferenceArray {

	public:
		NiReferenceArrayShort(NiReferenceable* owner);
		virtual ~NiReferenceArrayShort();

		virtual void read(NiStream& stream) override;

		virtual void write(NiStream& stream, bool removeEmptry = true) override;

	};

}



