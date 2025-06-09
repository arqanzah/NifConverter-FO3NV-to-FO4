#pragma once

#include <iostream>

#include "types.h"
#include "NiReferenceInterface.h"
#include "NiReferenceable.h"

namespace nif {

	class NiReference : public NiReferenceInterface {

		NiReferenceable* owner = nullptr;
		NiReferenceable* prop = nullptr;

	public:
		virtual ~NiReference();

		NiReference(NiReferenceable* owner);

		virtual NiReferenceable* getOwner() override;

		virtual NiReferenceable* get() override;

		virtual void set(NiReferenceable* property) override;

		virtual void clear() override;

		void read(NiStream& stream);

		void write(NiStream& stream);

	};


}