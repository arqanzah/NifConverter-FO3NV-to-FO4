#include "NiFile.h"

namespace nif {

	NiFile::NiFile() {}

	NiFile::~NiFile() {}


	NiFile::NiFile(const std::string& fileName, std::unordered_map<std::string, std::string>& names) : NiFile() {
		load(fileName, names);
	}


	NiNode* NiFile::getRootNode() {

		for (size_t i = 0; i < blocks.size(); i++) {
			if (auto niNode = dynamic_cast<NiNode*>(blocks[i].get())) {
				return niNode;
			}
		}

		return nullptr;
	}


	void NiFile::load(const std::string& fileName, std::unordered_map<std::string, std::string>& names) {

		sourceFileName = fileName;

		std::fstream fStream(fileName.c_str(), std::ios::in | std::ios::binary);

		if (fStream.is_open()) {

			unsigned int numBlocks = 0;
			unsigned short numBlockTypes = 0;
			std::vector<NiString<unsigned int>> blockTypes;
			std::vector<unsigned short> blockTypeIndices;
			std::vector<unsigned int> blockSizes;
			unsigned int numStrings = 0;
			unsigned int maxStringLength = 0;

			NiStream stream(&fStream);

			stream.getLine(version_str);
			stream >> version;
			stream >> endian;
			stream >> userVersion;
			stream >> numBlocks;
			stream >> userVersion2;
			exportInfoAuthor.read(stream);
			exportInfoProcessScript.read(stream);
			exportInfoExportScript.read(stream);


			stream >> numBlockTypes;
			blockTypes.resize(numBlockTypes);
			for (unsigned int i = 0; i < numBlockTypes; i++) {
				blockTypes[i].read(stream);
			}

			blockTypeIndices.resize(numBlocks);
			for (unsigned int i = 0; i < numBlocks; i++) {
				stream >> blockTypeIndices[i];
			}

			blockSizes.resize(numBlocks);
			for (unsigned int i = 0; i < numBlocks; i++) {
				stream >> blockSizes[i];
			}

			stream >> numStrings;
			stream >> maxStringLength;
			strings.resize(numStrings);
			for (unsigned int i = 0; i < numStrings; i++) {
				strings[i].read(stream);
			}

			stream >> numGroups;
			groups.resize(numGroups);
			for (unsigned int i = 0; i < numGroups; i++) {
				stream >> groups[i];
			}

			NiObjectFactory& factory = NiObjectFactory::getInstance();

			blocks.resize(numBlocks);
			for (unsigned int i = 0; i < numBlocks; i++) {

				std::string name = blockTypes[blockTypeIndices[i]].get();
				auto factory_template = factory.getTemplate(name);

				if (factory_template) {

					//if (name == "NiParticleSystem") {
					//	std::streampos pos = stream.tellp();
					//	printf("Reading: %d\n", (int)pos); //
					//}

					std::vector<char> buff;
					buff.resize(blockSizes[i]);
					stream.read(buff.data(), blockSizes[i]);
					std::stringstream ss(std::stringstream::binary | std::stringstream::in | std::stringstream::out);
					ss.write(buff.data(), blockSizes[i]);

					NiStream _stream(&ss);

					blocks[i] = factory_template->create(_stream);
				} else {
					blocks[i] = std::make_unique<NiUnknow>(stream, name, blockSizes[i]);
					//printf("Warning! Unknown block: %s\n", name.c_str());

					if (names.find(name) == names.end()) {
						names.insert(std::make_pair(name, fileName));
					}
				}

				blocks[i].get()->setIndex(i);

			}

			//read references
			for (auto& object : blocks) {
				for (auto ref : object.get()->getReferences()) {

					NiReferenceable* refrenceable = ref->get();
					if (refrenceable) {
						ref->set(blocks[refrenceable->getIndex()].get());
						delete refrenceable;
					}

				}
			}

			fStream.close();
		} else {
			printf("Can not open file %s\n", fileName.c_str());
		}

	}

	void NiFile::save(std::string fileName) {

		std::fstream fStream(fileName.c_str(), std::ios::out | std::ios::binary);

		if (fStream.is_open()) {

			// "delete" marked for deletion blocks before saving





			std::vector<NiString<unsigned int>> blockTypes;
			std::vector<unsigned short> blockTypeIndices;
			blockTypeIndices.resize(blocks.size());
			std::vector<unsigned int> blockSizes;
			blockSizes.resize(blocks.size());
			unsigned int numStrings = 0;
			unsigned int maxStringLength = 0;

			NiStream stream(&fStream);

			stream.writeLine(version_str.c_str(), version_str.size());

			stream << version;
			stream << endian;
			stream << userVersion;
			stream << (unsigned int)blocks.size();//stream << numBlocks;
			stream << userVersion2;
			exportInfoAuthor.write(stream);
			exportInfoProcessScript.write(stream);
			exportInfoExportScript.write(stream);

			maxFilePath.write(stream);

			for (unsigned int i = 0; i < blocks.size(); i++) {

				std::vector<NiString<unsigned int>>::iterator it = std::find_if(blockTypes.begin(), blockTypes.end(), [&](const NiString<unsigned int>& blockType) {
					return blockType.get() == blocks[i]->getBlockName();
				});

				if (it == blockTypes.end()) {
					blockTypes.push_back(NiString<unsigned int>(blocks[i]->getBlockName()));
					blockTypeIndices[i] = blockTypes.size() - 1;
				} else {
					blockTypeIndices[i] = std::distance(blockTypes.begin(), it);
				}

				blockSizes[i] = 0;
			}

			stream << (unsigned short)blockTypes.size();
			for (unsigned int i = 0; i < blockTypes.size(); i++) {
				blockTypes[i].write(stream);

			}

			for (unsigned int i = 0; i < blockTypeIndices.size(); i++) {
				stream << blockTypeIndices[i];
			}

			std::streampos blockSizesPos = stream.tellp(); // save block sizes position
			for (unsigned int i = 0; i < blockSizes.size(); i++) {
				stream << blockSizes[i];
			}


			numStrings = strings.size();
			maxStringLength = 0;
			for (auto &s : strings) {
				if (maxStringLength < s.length()) {
					maxStringLength = s.length();
				}
			}


			stream << numStrings;
			stream << maxStringLength;
			for (unsigned int i = 0; i < numStrings; i++) {
				strings[i].write(stream);
			}

			stream << numGroups;
			for (unsigned int i = 0; i < numGroups; i++) {
				stream << groups[i];
			}

			blockSizes.clear(); // reset block sizes
			blockSizes.resize(blocks.size());
			stream.resetBlockSize();

			for (unsigned int i = 0; i < blocks.size(); i++) {
				blocks[i]->write(stream);
				blockSizes[i] = stream.getBlockSize(); // get new block sizes
				stream.resetBlockSize();
			}


			unsigned int endFile = 1;
			stream << endFile;
			endFile = 0;
			stream << endFile;

			// update block sizes
			if (blockSizesPos != std::streampos()) {
				stream.seekg(blockSizesPos);
				for (unsigned int i = 0; i < blockSizes.size(); i++) {
					stream << blockSizes[i];
				}
			}

			fStream.close();
		} else {
			printf("Can not open file %s\n", fileName.c_str());
		}

	}










































	const std::vector<std::unique_ptr<NiObject>>& NiFile::getBlocks() {
		return blocks;
	}

	void NiFile::add(NiObject* object) {

		object->setIndex(blocks.size());
		blocks.push_back(std::unique_ptr<NiObject>(object));

	}

	void NiFile::insert(uint32_t index, NiObject* object) {

		blocks.insert(blocks.begin() + index, std::unique_ptr<NiObject>(object));
	
		for (size_t i = index; i < blocks.size(); i++) {
			blocks[i]->setIndex(i);
		}
	
	}

	void NiFile::replace(uint32_t index, NiObject* object) {

		if (index < blocks.size()) {

			NiObject* old = blocks.at(index).get();

			for (auto ref : old->getReferences()) {
				ref->clear();
			}

			std::vector<NiReferenceInterface*> temp;

			for (auto ref : old->getReferencedBy()) {
				temp.push_back(ref);
			}

			for (auto ref : temp) {
				ref->set(object);
			}


			object->setIndex(index);
			blocks[index] = std::move(std::unique_ptr<NiObject>(object));

		}

	}

	void NiFile::remove(uint32_t index) {

		if (index < blocks.size()) {

			NiObject* old = blocks.at(index).get();

			for (auto ref : old->getReferences()) {
				ref->clear();
			}

			std::vector<NiReferenceInterface*> temp;

			for (auto ref : old->getReferencedBy()) {
				temp.push_back(ref);
			}

			for (auto ref : temp) {
				ref->clear();
			}

			blocks.erase(blocks.begin() + index);

			for (size_t i = index; i < blocks.size(); i++) {
				blocks[i]->setIndex(i);
			}

		}


	}

}



