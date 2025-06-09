#pragma once

#include <iostream>
#include <string>
#include <fstream>


enum AlphaBlendModeType
{
	Unknown = 0,
	None,
	Standard,
	Additive,
	Multiplicative,
};

enum MaskWriteFlags
{
	ALBEDO = 1,
	NORMAL = 2,
	SPECULAR = 4,
	AMBIENT_OCCLUSION = 8,
	EMISSIVE = 16,
	GLOSS = 32
};

struct Color
{
public:
	  float R;
	  float G;
	  float B;

	Color(float r, float g, float b)
	{
		R = r;
		G = g;
		B = b;
	}

	unsigned int toUInt32()
	{
		unsigned int value = 0;
		value |= (unsigned char)(R * 255);
		value <<= 8;
		value |= (unsigned char)(G * 255);
		value <<= 8;
		value |= (unsigned char)(B * 255);
		return value;
	}

	static Color fromUInt32(unsigned int value)
	{
		const float multiplier = 1.0f / 255;

		auto b = (unsigned char)(value & 0xFF);
		value >>= 8;
		auto g = (unsigned char)(value & 0xFF);
		value >>= 8;
		auto r = (unsigned char)(value & 0xFF);
		return Color(r * multiplier, g * multiplier, b * multiplier);
	}


	static Color read(std::fstream &stream)
	{
		float r;
		stream.read(reinterpret_cast<char *>(&r), sizeof(r));
		float g;
		stream.read(reinterpret_cast<char *>(&g), sizeof(g));
		float b;
		stream.read(reinterpret_cast<char *>(&b), sizeof(b));

		return Color(r, g, b);
	}

	void write(std::fstream &stream)
	{
		stream.write(reinterpret_cast<char *>(&R), sizeof(R));
		stream.write(reinterpret_cast<char *>(&G), sizeof(G));
		stream.write(reinterpret_cast<char *>(&B), sizeof(B));
	}
};

class MaterialFile
{

public:
	MaterialFile(unsigned int signature);
	virtual ~MaterialFile();

	virtual void load(std::string fileName) {};
	virtual void save(std::string fileName) {};

	unsigned char& getAphaTestRef() { return alphaTestRef; }
	const unsigned char& getAphaTestRef() const { return alphaTestRef; }

	bool& getAlphaTest() { return alphaTest; }
	const bool& getAlphaTest() const { return alphaTest; }

	bool& getTwoSided() { return twoSided; }
	const bool& getTwoSided() const { return twoSided; }

	bool& getDecald() { return decal; }
	const bool& getDecald() const { return decal; }

	


private:

	unsigned int signature = 0xffffffff;
	unsigned int version = 2;
	bool tileU = true;
	bool tileV = true;
	float UOffset = 0.0f;
	float VOffset = 0.0f;
	float UScale = 1.0f;
	float VScale = 1.0f;

	float alpha = 1.0f;
	AlphaBlendModeType alphaBlendMode;
	unsigned char alphaTestRef = 128;
	bool alphaTest = false;

	bool zBufferWrite = true;
	bool zBufferTest = true;

	bool screenSpaceReflections = false;
	bool wetnessControlScreenSpaceReflections = false;

	bool decal = false;
	bool twoSided = false;
	bool decalNoFade = false;
	bool nonOccluder = false;
	bool refraction = false;
	bool refractionFalloff = false;
	float refractionPower = 0.0f;

	bool environmentMapping = false;
	float environmentMappingMaskScale = 1.0f;

	bool grayscaleToPaletteColor = false;













	static  AlphaBlendModeType convertAlphaBlendMode(unsigned char &a, unsigned int &b, unsigned int &c)
	{
		if (a == 0 && b == 6 && c == 7)
		{
			return AlphaBlendModeType::Unknown;
		}
		else if (a == 0 && b == 0 && c == 0)
		{
			return AlphaBlendModeType::None;
		}
		else if (a == 1 && b == 6 && c == 7)
		{
			return AlphaBlendModeType::Standard;
		}
		else if (a == 1 && b == 6 && c == 0)
		{
			return AlphaBlendModeType::Additive;
		}
		else if (a == 1 && b == 4 && c == 1)
		{
			return AlphaBlendModeType::Multiplicative;
		}

		return AlphaBlendModeType::Unknown;

	}

	static void convertAlphaBlendMode(const AlphaBlendModeType &type, unsigned char &a, unsigned int &b, unsigned int &c)
	{
		if (type == AlphaBlendModeType::Unknown)
		{
			a = 0;
			b = 6;
			c = 7;
		}
		else if (type == AlphaBlendModeType::None)
		{
			a = 0;
			b = 0;
			c = 0;
		}
		else if (type == AlphaBlendModeType::Standard)
		{
			a = 1;
			b = 6;
			c = 7;
		}
		else if (type == AlphaBlendModeType::Additive)
		{
			a = 1;
			b = 6;
			c = 0;
		}
		else if (type == AlphaBlendModeType::Multiplicative)
		{
			a = 1;
			b = 4;
			c = 1;
		}

	}


protected:

	void read(std::fstream &file);
	void write(std::fstream &file);

	static std::string readString(std::fstream &stream)
	{
		std::string str;

		unsigned int length;
		stream.read(reinterpret_cast<char *>(&length), sizeof(length));

		str.resize(length);
		stream.read(&str[0], length);

		int index = str.find_last_of('\0');
		if (index >= 0)
			str = str.erase(index, 1);


		return str;
	}

	static void writeString(std::fstream &stream, const std::string &str)
	{
		//unsigned int length = str.length() + 1;
		size_t len = str.length() + 1;
		stream.write(reinterpret_cast<char *>(&len), sizeof(size_t)); //stream.write(&len, sizeof(size_t));
		stream.write(str.c_str() + '\0', len);
	}


};

