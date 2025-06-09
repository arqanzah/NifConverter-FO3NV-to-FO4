#pragma once

#include <vector>

#include "NiReferenceInterface.h"

namespace nif {

	class NiReferenceable {

		int32_t index = 0xFFFFFFFF;
		std::vector<NiReferenceInterface*> referencedBy;
		std::vector<NiReferenceInterface*> references;

	public:
		NiReferenceable();
		virtual ~NiReferenceable();

		int32_t getIndex();
		void setIndex(const int32_t index);

		std::vector<NiReferenceInterface*>& getReferencedBy();
		std::vector<NiReferenceInterface*>& getReferences();
	};


}