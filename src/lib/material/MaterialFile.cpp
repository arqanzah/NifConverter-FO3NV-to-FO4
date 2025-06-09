#include "MaterialFile.h"



MaterialFile::MaterialFile(unsigned int signature) {
	this->signature = signature;
	alphaBlendMode = AlphaBlendModeType::Unknown;
}


MaterialFile::~MaterialFile() {
}

void MaterialFile::read(std::fstream &file) {
	
	
		file.read(reinterpret_cast<char *>(&signature), sizeof(signature));
		//std::cout << "Signature: " << signature << std::endl;

		file.read(reinterpret_cast<char *>(&version), sizeof(version));
		//std::cout << "Version: " << version << std::endl;

		unsigned int tileFlags;
		file.read(reinterpret_cast<char *>(&tileFlags), sizeof(tileFlags));
		tileU = (tileFlags & 2) != 0;
		//std::cout << "tileU: " << tileU << std::endl;
		tileV = (tileFlags & 1) != 0;
		//std::cout << "tileV: " << tileV << std::endl;

		file.read(reinterpret_cast<char *>(&UOffset), sizeof(UOffset));
		//std::cout << "UOffset: " << UOffset << std::endl;

		file.read(reinterpret_cast<char *>(&VOffset), sizeof(VOffset));
		//std::cout << "VOffset: " << VOffset << std::endl;

		file.read(reinterpret_cast<char *>(&UScale), sizeof(UScale));
		//std::cout << "UScale: " << UScale << std::endl;

		file.read(reinterpret_cast<char *>(&VScale), sizeof(VScale));
		//std::cout << "VScale: " << VScale << std::endl;

		file.read(reinterpret_cast<char *>(&alpha), sizeof(alpha));
		//std::cout << "alpha: " << alpha << std::endl;

		unsigned char alphaBlendMode0;
		file.read(reinterpret_cast<char *>(&alphaBlendMode0), sizeof(alphaBlendMode0));
		unsigned int alphaBlendMode1;
		file.read(reinterpret_cast<char *>(&alphaBlendMode1), sizeof(alphaBlendMode1));
		unsigned int alphaBlendMode2;
		file.read(reinterpret_cast<char *>(&alphaBlendMode2), sizeof(alphaBlendMode2));

		alphaBlendMode = convertAlphaBlendMode(alphaBlendMode0, alphaBlendMode1, alphaBlendMode2);
		//std::cout << "alphaBlendMode: " << alphaBlendMode << std::endl;

		file.read(reinterpret_cast<char *>(&alphaTestRef), sizeof(alphaTestRef));
		//std::cout << "alphaTestRef: " << alphaTestRef << std::endl;

		file.read(reinterpret_cast<char *>(&alphaTest), sizeof(alphaTest));
		//std::cout << "alphaTest: " << alphaTest << std::endl;



		file.read(reinterpret_cast<char *>(&zBufferWrite), sizeof(zBufferWrite));
		//std::cout << "zBufferWrite: " << zBufferWrite << std::endl;

		file.read(reinterpret_cast<char *>(&zBufferTest), sizeof(zBufferTest));
		//std::cout << "zBufferTest: " << zBufferTest << std::endl;

		file.read(reinterpret_cast<char *>(&screenSpaceReflections), sizeof(screenSpaceReflections));
		//std::cout << "screenSpaceReflections: " << screenSpaceReflections << std::endl;

		file.read(reinterpret_cast<char *>(&wetnessControlScreenSpaceReflections), sizeof(wetnessControlScreenSpaceReflections));
		//std::cout << "wetnessControlScreenSpaceReflections: " << wetnessControlScreenSpaceReflections << std::endl;


		file.read(reinterpret_cast<char *>(&decal), sizeof(decal));
		file.read(reinterpret_cast<char *>(&twoSided), sizeof(twoSided));
		file.read(reinterpret_cast<char *>(&decalNoFade), sizeof(decalNoFade));
		file.read(reinterpret_cast<char *>(&nonOccluder), sizeof(nonOccluder));
		file.read(reinterpret_cast<char *>(&refraction), sizeof(refraction));
		file.read(reinterpret_cast<char *>(&refractionFalloff), sizeof(refractionFalloff));
		file.read(reinterpret_cast<char *>(&refractionPower), sizeof(refractionPower));


		file.read(reinterpret_cast<char *>(&environmentMapping), sizeof(environmentMapping));
		file.read(reinterpret_cast<char *>(&environmentMappingMaskScale), sizeof(environmentMappingMaskScale));


		file.read(reinterpret_cast<char *>(&grayscaleToPaletteColor), sizeof(grayscaleToPaletteColor));
		//std::cout << "grayscaleToPaletteColor: " << grayscaleToPaletteColor << std::endl;


}



void MaterialFile::write(std::fstream &file) {
	
	file.write(reinterpret_cast<char *>(&signature), sizeof(signature));
	file.write(reinterpret_cast<char *>(&version), sizeof(version));
	
	unsigned int tileFlags = 0;
	if (tileU) tileFlags += 2;
	if (tileV) tileFlags += 1;
	file.write(reinterpret_cast<char *>(&tileFlags), sizeof(tileFlags));
	

	file.write(reinterpret_cast<char *>(&UOffset), sizeof(UOffset));
	file.write(reinterpret_cast<char *>(&VOffset), sizeof(VOffset));
	file.write(reinterpret_cast<char *>(&UScale), sizeof(UScale));
	file.write(reinterpret_cast<char *>(&VScale), sizeof(VScale));

	file.write(reinterpret_cast<char *>(&alpha), sizeof(alpha));

	unsigned char alphaBlendMode0 = 0;
	unsigned int alphaBlendMode1 = 0;
	unsigned int alphaBlendMode2 = 0;
	convertAlphaBlendMode(alphaBlendMode, alphaBlendMode0, alphaBlendMode1, alphaBlendMode2);
	file.write(reinterpret_cast<char *>(&alphaBlendMode0), sizeof(alphaBlendMode0));
	file.write(reinterpret_cast<char *>(&alphaBlendMode1), sizeof(alphaBlendMode1));
	file.write(reinterpret_cast<char *>(&alphaBlendMode2), sizeof(alphaBlendMode2));

	file.write(reinterpret_cast<char *>(&alphaTestRef), sizeof(alphaTestRef));
	file.write(reinterpret_cast<char *>(&alphaTest), sizeof(alphaTest));


	file.write(reinterpret_cast<char *>(&zBufferWrite), sizeof(zBufferWrite));
	file.write(reinterpret_cast<char *>(&zBufferTest), sizeof(zBufferTest));
	file.write(reinterpret_cast<char *>(&screenSpaceReflections), sizeof(screenSpaceReflections));
	file.write(reinterpret_cast<char *>(&wetnessControlScreenSpaceReflections), sizeof(wetnessControlScreenSpaceReflections));


	file.write(reinterpret_cast<char *>(&decal), sizeof(decal));
	file.write(reinterpret_cast<char *>(&twoSided), sizeof(twoSided));
	file.write(reinterpret_cast<char *>(&decalNoFade), sizeof(decalNoFade));
	file.write(reinterpret_cast<char *>(&nonOccluder), sizeof(nonOccluder));
	file.write(reinterpret_cast<char *>(&refraction), sizeof(refraction));
	file.write(reinterpret_cast<char *>(&refractionFalloff), sizeof(refractionFalloff));
	file.write(reinterpret_cast<char *>(&refractionPower), sizeof(refractionPower));


	file.write(reinterpret_cast<char *>(&environmentMapping), sizeof(environmentMapping));
	file.write(reinterpret_cast<char *>(&environmentMappingMaskScale), sizeof(environmentMappingMaskScale));


	file.write(reinterpret_cast<char *>(&grayscaleToPaletteColor), sizeof(grayscaleToPaletteColor));

}