#pragma once
#include "MaterialFile.h"
class BGSM : public MaterialFile {


private:



	std::string diffuseTexture = "";
	std::string normalTexture = "";
	std::string smoothSpecTexture = "";
	std::string  greyscaleTexture = "";

	std::string envmapTexture = "";
	std::string glowTexture = "";
	std::string innerLayerTexture = "";
	std::string wrinklesTexture = "";
	std::string displacementTexture = "";

	bool enableEditorAlphaRef = false;

	bool rimLighting = false;
	float rimPower = 2.0f;
	float backLightPower = 0.0f;
	bool subsurfaceLighting = false;
	float subsurfaceLightingRolloff = 0.3f;

	bool specularEnabled = false;
	unsigned int specularColor = 0xFFFFFFFFu;

	float specularMult = 1.0f;
	float smoothness = 1.0f;

	float fresnelPower = 5.0f;
	float wetnessControlSpecScale = -1.0f;
	float wetnessControlSpecPowerScale = -1.0f;
	float wetnessControlSpecMinvar = -1.0f;

	float wetnessControlEnvMapScale = -1.0f;
	float wetnessControlFresnelPower = -1.0f;
	float wetnessControlMetalness = -1.0f;

	std::string rootMaterialPath = "";
	bool anisoLighting = false;

	bool emitEnabled = false;
	unsigned int emittanceColor = 0xFFFFFFFFu;
	float emittanceMult = 1.0f;


	bool modelSpaceNormals = false;
	bool externalEmittance = false;
	bool backLighting = false;
	bool receiveShadows = false;
	bool hideSecret = false;
	bool castShadows = false;
	bool dissolveFade = false;
	bool assumeShadowmask = false;
	bool glowmap = false;
	bool environmentMappingWindow = false;
	bool environmentMappingEye = false;
	bool hair = false;
	unsigned int hairTintColor = 0x808080u;
	bool tree = false;
	bool facegen = false;
	bool skinTint = false;
	bool tessellate = false;
	float displacementTextureBias = -0.5f;
	float displacementTextureScale = 10.0f;
	float tessellationPnScale = 1.0f;
	float tessellationBaseFactor = 1.0f;
	float tessellationFadeDistance = 0.0f;
	float grayscaleToPaletteScale = 1.0f;
	bool skewSpecularAlpha = false;




public:
	static const unsigned int SIGNATURE = 0x4D534742u;

	BGSM();
	virtual ~BGSM();
	void load(std::string fileName);
	void save(std::string fileName);

	
	

	/*
	auto getDiffuseTexture()       -> std::string& { return diffuseTexture; }
	auto getDiffuseTexture() const -> const std::string& { return diffuseTexture; }
	*/

	std::string& getDiffuseTexture() { return diffuseTexture; }
	const std::string& getDiffuseTexture() const { return diffuseTexture; }

	std::string& getNormalTexture() { return normalTexture; }
	const std::string& getNormalTexture() const { return normalTexture; }

	std::string& getSmoothSpecTexture() { return smoothSpecTexture; }
	const std::string& getSmoothSpecTexture() const { return smoothSpecTexture; }

	std::string& getGreyscaleTexture() { return greyscaleTexture; }
	const std::string& getGreyscaleTexture() const { return greyscaleTexture; }
	
	std::string& getEnvmapTexture() { return envmapTexture; }
	const std::string& getEnvmapTexture() const { return envmapTexture; }

	std::string& getGlowTexture() { return glowTexture; }
	const std::string& getGlowTexture() const { return glowTexture; }

	std::string& getInnerLayerTexture() { return innerLayerTexture; }
	const std::string& getInnerLayerTexture() const { return innerLayerTexture; }

	std::string& getWrinklesTexture() { return wrinklesTexture; }
	const std::string& getWrinklesTexture() const { return wrinklesTexture; }

	std::string& getDisplacementTexture() { return displacementTexture; }
	const std::string& getDisplacementTexture() const { return displacementTexture; }
	
	//std::string  = "";
	//std::string  = "";
	//std::string  = "";
	//std::string  = "";
	//std::string  = "";

protected:
	void read(std::fstream &file);
	void write(std::fstream &file);



};

