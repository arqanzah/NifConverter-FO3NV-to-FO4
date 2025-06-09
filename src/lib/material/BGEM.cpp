#include "BGEM.h"



BGEM::BGEM() : MaterialFile(SIGNATURE) {
}


BGEM::~BGEM()
{
}


void BGEM::load(std::string fileName) {
	std::fstream file(fileName.c_str(), std::ios::in | std::ios::binary);
	if (file.is_open()) {
		read(file);
		file.close();
	}
}

void BGEM::save(std::string fileName) {
	std::fstream file(fileName.c_str(), std::ios::out | std::ios::binary);
	if (file.is_open()) {
		write(file);
		file.flush();
		file.close();
	}
}


void BGEM::read(std::fstream &file) {

	MaterialFile::read(file);

	baseTexture = readString(file);
	//std::cout << "baseTexture: " << baseTexture << std::endl;

	grayscaleTexture = readString(file);
	//std::cout << "grayscaleTexture: " << grayscaleTexture << std::endl;

	envmapTexture = readString(file);
	//std::cout << "envmapTexture: " << envmapTexture << std::endl;

	normalTexture = readString(file);
	//std::cout << "normalTexture: " << normalTexture << std::endl;

	envmapMaskTexture = readString(file);
	//std::cout << "envmapMaskTexture: " << envmapMaskTexture << std::endl;

	file.read(reinterpret_cast<char *>(&bloodEnabled), sizeof(bloodEnabled));
	//std::cout << "bloodEnabled: " << bloodEnabled << std::endl;

	file.read(reinterpret_cast<char *>(&effectLightingEnabled), sizeof(effectLightingEnabled));
	//std::cout << "effectLightingEnabled: " << effectLightingEnabled << std::endl;

	file.read(reinterpret_cast<char *>(&falloffEnabled), sizeof(falloffEnabled));
	//std::cout << "falloffEnabled: " << falloffEnabled << std::endl;

	file.read(reinterpret_cast<char *>(&falloffColorEnabled), sizeof(falloffColorEnabled));
	//std::cout << "falloffColorEnabled: " << falloffColorEnabled << std::endl;

	file.read(reinterpret_cast<char *>(&grayscaleToPaletteAlpha), sizeof(grayscaleToPaletteAlpha));
	//std::cout << "grayscaleToPaletteAlpha: " << grayscaleToPaletteAlpha << std::endl;

	file.read(reinterpret_cast<char *>(&softEnabled), sizeof(softEnabled));
	//std::cout << "softEnabled: " << softEnabled << std::endl;

	baseColor = Color::read(file).toUInt32();
	//std::cout << "baseColor: " << baseColor << std::endl;

	file.read(reinterpret_cast<char *>(&baseColorScale), sizeof(baseColorScale));
	//std::cout << "baseColorScale: " << baseColorScale << std::endl;

	file.read(reinterpret_cast<char *>(&falloffStartAngle), sizeof(falloffStartAngle));
	//std::cout << "falloffStartAngle: " << falloffStartAngle << std::endl;

	file.read(reinterpret_cast<char *>(&falloffStopAngle), sizeof(falloffStopAngle));
	//std::cout << "falloffStopAngle: " << falloffStopAngle << std::endl;

	file.read(reinterpret_cast<char *>(&falloffStartOpacity), sizeof(falloffStartOpacity));
	//std::cout << "falloffStartOpacity: " << falloffStartOpacity << std::endl;

	file.read(reinterpret_cast<char *>(&falloffStopOpacity), sizeof(falloffStopOpacity));
	//std::cout << "falloffStopOpacity: " << falloffStopOpacity << std::endl;

	file.read(reinterpret_cast<char *>(&lightingInfluence), sizeof(lightingInfluence));
	//std::cout << "lightingInfluence: " << lightingInfluence << std::endl;

	file.read(reinterpret_cast<char *>(&envmapMinLOD), sizeof(envmapMinLOD));
	//std::cout << "envmapMinLOD: " << envmapMinLOD << std::endl;

	file.read(reinterpret_cast<char *>(&softDepth), sizeof(softDepth));
	//std::cout << "softDepth: " << softDepth << std::endl;

}



void BGEM::write(std::fstream &file) {

	MaterialFile::write(file);

	writeString(file, baseTexture);
	
	writeString(file, grayscaleTexture);
	
	writeString(file, envmapTexture);
	
	writeString(file, normalTexture);
	
	writeString(file, envmapMaskTexture);
	
	file.write(reinterpret_cast<char *>(&bloodEnabled), sizeof(bloodEnabled));
	
	file.write(reinterpret_cast<char *>(&effectLightingEnabled), sizeof(effectLightingEnabled));
	
	file.write(reinterpret_cast<char *>(&falloffEnabled), sizeof(falloffEnabled));
	
	file.write(reinterpret_cast<char *>(&falloffColorEnabled), sizeof(falloffColorEnabled));
	
	file.write(reinterpret_cast<char *>(&grayscaleToPaletteAlpha), sizeof(grayscaleToPaletteAlpha));
	
	file.write(reinterpret_cast<char *>(&softEnabled), sizeof(softEnabled));

	Color::fromUInt32(baseColor).write(file);
	
	file.write(reinterpret_cast<char *>(&baseColorScale), sizeof(baseColorScale));
	
	file.write(reinterpret_cast<char *>(&falloffStartAngle), sizeof(falloffStartAngle));
	
	file.write(reinterpret_cast<char *>(&falloffStopAngle), sizeof(falloffStopAngle));
	
	file.write(reinterpret_cast<char *>(&falloffStartOpacity), sizeof(falloffStartOpacity));
	
	file.write(reinterpret_cast<char *>(&falloffStopOpacity), sizeof(falloffStopOpacity));
	
	file.write(reinterpret_cast<char *>(&lightingInfluence), sizeof(lightingInfluence));
	
	file.write(reinterpret_cast<char *>(&envmapMinLOD), sizeof(envmapMinLOD));
	
	file.write(reinterpret_cast<char *>(&softDepth), sizeof(softDepth));
	

}
