#pragma once
#include "MaterialFile.h"
class BGEM : public MaterialFile {

public:
	static const unsigned int SIGNATURE = 0x4D454742u;
	void load(std::string fileName);
	void save(std::string fileName);
	BGEM();
	~BGEM();

protected:
	void read(std::fstream &file);
	void write(std::fstream &file);


private:
	std::string baseTexture = "";
	std::string grayscaleTexture = "";
	std::string envmapTexture = "";
	std::string normalTexture = "";
	std::string envmapMaskTexture = "";



	bool bloodEnabled = false;
	bool effectLightingEnabled = false;
	bool falloffEnabled = false;
	bool falloffColorEnabled = false;
	bool grayscaleToPaletteAlpha = false;
	bool softEnabled = false;
	unsigned int baseColor = 0xFFFFFFFFu;
	float baseColorScale = 1.0f;
	float falloffStartAngle = 1.0f;
	float falloffStopAngle = 1.0f;
	float falloffStartOpacity = 0.0f;
	float falloffStopOpacity = 0.0f;
	float lightingInfluence = 1.0f;
	unsigned char envmapMinLOD = 0;
	float softDepth = 100.0f;



};

