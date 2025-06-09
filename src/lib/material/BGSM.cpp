#include "BGSM.h"



BGSM::BGSM() : MaterialFile(SIGNATURE) {}


BGSM::~BGSM() {
}


void BGSM::load(std::string fileName) {
	std::fstream file(fileName.c_str(), std::ios::in | std::ios::binary);
	if (file.is_open()) {
		read(file);
		file.close();
	}
}

void BGSM::save(std::string fileName) {
	std::fstream file(fileName.c_str(), std::ios::out | std::ios::binary);
	if (file.is_open()) {
		write(file);
		file.flush();
		file.close();
	}
}


void BGSM::read(std::fstream &file) {

		MaterialFile::read(file);

		diffuseTexture = readString(file);
		//std::cout << "diffuseTexture: " << diffuseTexture << std::endl;

		normalTexture = readString(file);
		//std::cout << "normalTexture: " << normalTexture << std::endl;

		smoothSpecTexture = readString(file);
		//std::cout << "smoothSpecTexture: " << smoothSpecTexture << std::endl;

		greyscaleTexture = readString(file);
		//std::cout << "greyscaleTexture: " << greyscaleTexture << std::endl;


		envmapTexture = readString(file);
		//std::cout << "envmapTexture: " << envmapTexture << std::endl;

		glowTexture = readString(file);
		//std::cout << "glowTexture: " << glowTexture << std::endl;

		innerLayerTexture = readString(file);
		//std::cout << "innerLayerTexture: " << innerLayerTexture << std::endl;

		wrinklesTexture = readString(file);
		//std::cout << "wrinklesTexture: " << wrinklesTexture << std::endl;

		displacementTexture = readString(file);
		//std::cout << "displacementTexture: " << displacementTexture << std::endl;

		file.read(reinterpret_cast<char *>(&enableEditorAlphaRef), sizeof(enableEditorAlphaRef));
		//std::cout << "enableEditorAlphaRef: " << enableEditorAlphaRef << std::endl;


		file.read(reinterpret_cast<char *>(&rimLighting), sizeof(rimLighting));
		//std::cout << "RimLighting: " << rimLighting << std::endl;

		file.read(reinterpret_cast<char *>(&rimPower), sizeof(rimPower));
		//std::cout << "RimPower: " << rimPower << std::endl;

		file.read(reinterpret_cast<char *>(&backLightPower), sizeof(backLightPower));
		//std::cout << "BackLightPower: " << backLightPower << std::endl;

		file.read(reinterpret_cast<char *>(&subsurfaceLighting), sizeof(subsurfaceLighting));
		//std::cout << "SubsurfaceLighting: " << subsurfaceLighting << std::endl;

		file.read(reinterpret_cast<char *>(&subsurfaceLightingRolloff), sizeof(subsurfaceLightingRolloff));
		//std::cout << "SubsurfaceLightingRolloff: " << subsurfaceLightingRolloff << std::endl;

		file.read(reinterpret_cast<char *>(&specularEnabled), sizeof(specularEnabled));
		//std::cout << "SpecularEnabled: " << specularEnabled << std::endl;

		specularColor = Color::read(file).toUInt32();
		//std::cout << "specularColor: " << specularColor << std::endl;

		file.read(reinterpret_cast<char *>(&specularMult), sizeof(specularMult));
		//std::cout << "specularMult: " << specularMult << std::endl;

		file.read(reinterpret_cast<char *>(&smoothness), sizeof(smoothness));
		//std::cout << "smoothness: " << smoothness << std::endl;

		file.read(reinterpret_cast<char *>(&fresnelPower), sizeof(fresnelPower));
		//std::cout << "fresnelPower: " << fresnelPower << std::endl;

		file.read(reinterpret_cast<char *>(&wetnessControlSpecScale), sizeof(wetnessControlSpecScale));
		//std::cout << "wetnessControlSpecScale: " << wetnessControlSpecScale << std::endl;

		file.read(reinterpret_cast<char *>(&wetnessControlSpecPowerScale), sizeof(wetnessControlSpecPowerScale));
		//std::cout << "wetnessControlSpecPowerScale: " << wetnessControlSpecPowerScale << std::endl;

		file.read(reinterpret_cast<char *>(&wetnessControlSpecMinvar), sizeof(wetnessControlSpecMinvar));
		//std::cout << "wetnessControlSpecMinvar: " << wetnessControlSpecMinvar << std::endl;

		file.read(reinterpret_cast<char *>(&wetnessControlEnvMapScale), sizeof(wetnessControlEnvMapScale));
		//std::cout << "wetnessControlEnvMapScale: " << wetnessControlEnvMapScale << std::endl;

		file.read(reinterpret_cast<char *>(&wetnessControlFresnelPower), sizeof(wetnessControlFresnelPower));
		//std::cout << "wetnessControlFresnelPower: " << wetnessControlFresnelPower << std::endl;

		file.read(reinterpret_cast<char *>(&wetnessControlMetalness), sizeof(wetnessControlMetalness));
		//std::cout << "wetnessControlMetalness: " << wetnessControlMetalness << std::endl;

		rootMaterialPath = readString(file);
		//std::cout << "rootMaterialPath: " << rootMaterialPath << std::endl;

		file.read(reinterpret_cast<char *>(&anisoLighting), sizeof(anisoLighting));
		//std::cout << "anisoLighting: " << anisoLighting << std::endl;

		file.read(reinterpret_cast<char *>(&emitEnabled), sizeof(emitEnabled));
		//std::cout << "emitEnabled: " << emitEnabled << std::endl;

		if (emitEnabled) {
			emittanceColor = Color::read(file).toUInt32();
			//std::cout << "emittanceColor: " << emittanceColor << std::endl;
		}

		file.read(reinterpret_cast<char *>(&emittanceMult), sizeof(emittanceMult));
		//std::cout << "emittanceMult: " << emittanceMult << std::endl;




		file.read(reinterpret_cast<char *>(&modelSpaceNormals), sizeof(modelSpaceNormals));
		//std::cout << "modelSpaceNormals: " << modelSpaceNormals << std::endl;

		file.read(reinterpret_cast<char *>(&externalEmittance), sizeof(externalEmittance));
		//std::cout << "externalEmittance: " << externalEmittance << std::endl;

		file.read(reinterpret_cast<char *>(&backLighting), sizeof(backLighting));
		//std::cout << "backLighting: " << backLighting << std::endl;

		file.read(reinterpret_cast<char *>(&receiveShadows), sizeof(receiveShadows));
		//std::cout << "receiveShadows: " << receiveShadows << std::endl;

		file.read(reinterpret_cast<char *>(&hideSecret), sizeof(hideSecret));
		//std::cout << "hideSecret: " << hideSecret << std::endl;

		file.read(reinterpret_cast<char *>(&castShadows), sizeof(castShadows));
		//std::cout << "castShadows: " << castShadows << std::endl;

		file.read(reinterpret_cast<char *>(&dissolveFade), sizeof(dissolveFade));
		//std::cout << "dissolveFade: " << dissolveFade << std::endl;

		file.read(reinterpret_cast<char *>(&assumeShadowmask), sizeof(assumeShadowmask));
		//std::cout << "assumeShadowmask: " << assumeShadowmask << std::endl;

		file.read(reinterpret_cast<char *>(&glowmap), sizeof(glowmap));
		//std::cout << "glowmap: " << glowmap << std::endl;

		file.read(reinterpret_cast<char *>(&environmentMappingWindow), sizeof(environmentMappingWindow));
		//std::cout << "environmentMappingWindow: " << environmentMappingWindow << std::endl;

		file.read(reinterpret_cast<char *>(&environmentMappingEye), sizeof(environmentMappingEye));
		//std::cout << "environmentMappingEye: " << environmentMappingEye << std::endl;

		file.read(reinterpret_cast<char *>(&hair), sizeof(hair));
		//std::cout << "hair: " << hair << std::endl;

		hairTintColor = Color::read(file).toUInt32();
		//std::cout << "hairTintColor: " << hairTintColor << std::endl;

		file.read(reinterpret_cast<char *>(&tree), sizeof(tree));
		//std::cout << "tree: " << tree << std::endl;

		file.read(reinterpret_cast<char *>(&facegen), sizeof(facegen));
		//std::cout << "facegen: " << facegen << std::endl;

		file.read(reinterpret_cast<char *>(&skinTint), sizeof(skinTint));
		//std::cout << "skinTint: " << skinTint << std::endl;

		file.read(reinterpret_cast<char *>(&tessellate), sizeof(tessellate));
		//std::cout << "tessellate: " << tessellate << std::endl;

		file.read(reinterpret_cast<char *>(&displacementTextureBias), sizeof(displacementTextureBias));
		//std::cout << "displacementTextureBias: " << displacementTextureBias << std::endl;

		file.read(reinterpret_cast<char *>(&displacementTextureScale), sizeof(displacementTextureScale));
		//std::cout << "displacementTextureScale: " << displacementTextureScale << std::endl;

		file.read(reinterpret_cast<char *>(&tessellationPnScale), sizeof(tessellationPnScale));
		//std::cout << "tessellationPnScale: " << tessellationPnScale << std::endl;

		file.read(reinterpret_cast<char *>(&tessellationBaseFactor), sizeof(tessellationBaseFactor));
		//std::cout << "tessellationBaseFactor: " << tessellationBaseFactor << std::endl;

		file.read(reinterpret_cast<char *>(&tessellationFadeDistance), sizeof(tessellationFadeDistance));
		//std::cout << "tessellationFadeDistance: " << tessellationFadeDistance << std::endl;

		file.read(reinterpret_cast<char *>(&grayscaleToPaletteScale), sizeof(grayscaleToPaletteScale));
		//std::cout << "grayscaleToPaletteScale: " << grayscaleToPaletteScale << std::endl;

		file.read(reinterpret_cast<char *>(&skewSpecularAlpha), sizeof(skewSpecularAlpha));
		//std::cout << "skewSpecularAlpha: " << skewSpecularAlpha << std::endl;
	




}



void BGSM::write(std::fstream &file) {

	MaterialFile::write(file);

	writeString(file, diffuseTexture);
	writeString(file, normalTexture);
	writeString(file, smoothSpecTexture);
	writeString(file, greyscaleTexture);
	writeString(file, envmapTexture);
	writeString(file, glowTexture);
	writeString(file, innerLayerTexture);
	writeString(file, wrinklesTexture);
	writeString(file, displacementTexture);

	file.write(reinterpret_cast<char *>(&enableEditorAlphaRef), sizeof(enableEditorAlphaRef));


	file.write(reinterpret_cast<char *>(&rimLighting), sizeof(rimLighting));
	
	file.write(reinterpret_cast<char *>(&rimPower), sizeof(rimPower));
	
	file.write(reinterpret_cast<char *>(&backLightPower), sizeof(backLightPower));
	
	file.write(reinterpret_cast<char *>(&subsurfaceLighting), sizeof(subsurfaceLighting));
	
	file.write(reinterpret_cast<char *>(&subsurfaceLightingRolloff), sizeof(subsurfaceLightingRolloff));
	
	file.write(reinterpret_cast<char *>(&specularEnabled), sizeof(specularEnabled));
	

	Color::fromUInt32(specularColor).write(file);

	file.write(reinterpret_cast<char *>(&specularMult), sizeof(specularMult));

	file.write(reinterpret_cast<char *>(&smoothness), sizeof(smoothness));
	
	file.write(reinterpret_cast<char *>(&fresnelPower), sizeof(fresnelPower));
	
	file.write(reinterpret_cast<char *>(&wetnessControlSpecScale), sizeof(wetnessControlSpecScale));
	
	file.write(reinterpret_cast<char *>(&wetnessControlSpecPowerScale), sizeof(wetnessControlSpecPowerScale));
	
	file.write(reinterpret_cast<char *>(&wetnessControlSpecMinvar), sizeof(wetnessControlSpecMinvar));
	
	file.write(reinterpret_cast<char *>(&wetnessControlEnvMapScale), sizeof(wetnessControlEnvMapScale));
	
	file.write(reinterpret_cast<char *>(&wetnessControlFresnelPower), sizeof(wetnessControlFresnelPower));
	
	file.write(reinterpret_cast<char *>(&wetnessControlMetalness), sizeof(wetnessControlMetalness));
	
	writeString(file, rootMaterialPath);

	file.write(reinterpret_cast<char *>(&anisoLighting), sizeof(anisoLighting));

	file.write(reinterpret_cast<char *>(&emitEnabled), sizeof(emitEnabled));

	if (emitEnabled) {
		Color::fromUInt32(emittanceColor).write(file);
	}

	file.write(reinterpret_cast<char *>(&emittanceMult), sizeof(emittanceMult));


	file.write(reinterpret_cast<char *>(&modelSpaceNormals), sizeof(modelSpaceNormals));
	
	file.write(reinterpret_cast<char *>(&externalEmittance), sizeof(externalEmittance));
	
	file.write(reinterpret_cast<char *>(&backLighting), sizeof(backLighting));
	
	file.write(reinterpret_cast<char *>(&receiveShadows), sizeof(receiveShadows));
	
	file.write(reinterpret_cast<char *>(&hideSecret), sizeof(hideSecret));
	
	file.write(reinterpret_cast<char *>(&castShadows), sizeof(castShadows));
	
	file.write(reinterpret_cast<char *>(&dissolveFade), sizeof(dissolveFade));
	
	file.write(reinterpret_cast<char *>(&assumeShadowmask), sizeof(assumeShadowmask));
	
	file.write(reinterpret_cast<char *>(&glowmap), sizeof(glowmap));
	
	file.write(reinterpret_cast<char *>(&environmentMappingWindow), sizeof(environmentMappingWindow));
	
	file.write(reinterpret_cast<char *>(&environmentMappingEye), sizeof(environmentMappingEye));
	
	file.write(reinterpret_cast<char *>(&hair), sizeof(hair));
	
	Color::fromUInt32(hairTintColor).write(file);

	file.write(reinterpret_cast<char *>(&tree), sizeof(tree));
	
	file.write(reinterpret_cast<char *>(&facegen), sizeof(facegen));
	
	file.write(reinterpret_cast<char *>(&skinTint), sizeof(skinTint));
	
	file.write(reinterpret_cast<char *>(&tessellate), sizeof(tessellate));
	
	file.write(reinterpret_cast<char *>(&displacementTextureBias), sizeof(displacementTextureBias));
	
	file.write(reinterpret_cast<char *>(&displacementTextureScale), sizeof(displacementTextureScale));
	
	file.write(reinterpret_cast<char *>(&tessellationPnScale), sizeof(tessellationPnScale));
	
	file.write(reinterpret_cast<char *>(&tessellationBaseFactor), sizeof(tessellationBaseFactor));
	
	file.write(reinterpret_cast<char *>(&tessellationFadeDistance), sizeof(tessellationFadeDistance));
	
	file.write(reinterpret_cast<char *>(&grayscaleToPaletteScale), sizeof(grayscaleToPaletteScale));
	
	file.write(reinterpret_cast<char *>(&skewSpecularAlpha), sizeof(skewSpecularAlpha));
	
}
