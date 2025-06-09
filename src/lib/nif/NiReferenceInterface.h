#pragma once

namespace nif {

	class NiReferenceable;

	class NiReferenceInterface {

	public:
		virtual ~NiReferenceInterface() {};

		NiReferenceInterface() {};

		virtual NiReferenceable* getOwner() = 0;

		virtual NiReferenceable* get() = 0;

		virtual void set(NiReferenceable* property) = 0;

		virtual void clear() = 0;

	};

}