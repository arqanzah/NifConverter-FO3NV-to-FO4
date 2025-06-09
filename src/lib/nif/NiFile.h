#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include "NiObjectFactory.h"

namespace nif {

	class NiFile {

	public:
		std::string sourceFileName;

		std::string version_str;
		int version = 0;
		NiEndian endian = NiEndian::ENDIAN_LITTLE;
		unsigned int userVersion = 0;
		unsigned int userVersion2 = 0;

		NiStringNull<unsigned char> exportInfoAuthor;
		NiStringNull<unsigned char> exportInfoProcessScript;
		NiStringNull<unsigned char> exportInfoExportScript;


		std::vector<NiString<unsigned int>> strings;
		unsigned int numGroups = 0;
		std::vector<unsigned int> groups;

		std::vector<std::unique_ptr<NiObject>> blocks;

		NiStringNull<unsigned char> maxFilePath; //<fo4>

		std::vector<NiReferenceable*> blocksToDelete;


	public:
		NiFile();
		virtual ~NiFile();

		const std::vector<std::unique_ptr<NiObject>>& getBlocks();

		void add(NiObject* object);
		void insert(uint32_t index, NiObject* object);
		void replace(uint32_t index, NiObject* object);
		void remove(uint32_t index);



		NiFile(const std::string& fileName, std::unordered_map<std::string, std::string>& names);


		void load(const std::string& fileName, std::unordered_map<std::string, std::string>& names);
		void save(std::string fileName);

		NiNode* getRootNode();

		//blocks marked for deletion, will be deleted on save
		void markForDeletion(NiReferenceable* referenceable) {
			blocksToDelete.push_back(referenceable);
		}

		const int addString(const std::string& str) {
			strings.push_back(NiString<unsigned int>(str));
			return strings.size() - 1;
		}

		void changeString(int index, const std::string& str) {

			if (index < strings.size()) {
				strings[index] = str;
			}

		}



	};


}


