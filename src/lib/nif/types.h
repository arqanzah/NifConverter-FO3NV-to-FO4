#pragma once

/*
* types.h
*
*  Created on: Feb 24, 2020
*      Author: User
*/

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>

namespace nif {

	enum NiEndian : unsigned char {
		ENDIAN_BIG,
		ENDIAN_LITTLE
	};


	struct Vector2 {
		float u;
		float v;

		Vector2() {
			u = v = 0.0f;
		}
		Vector2(float U, float V) {
			u = U;
			v = V;
		}
		void zero() {
			u = v = 0.0f;
		}
	};

	struct Vector3 {

		float x;
		float y;
		float z;

		Vector3() {
			x = y = z = 0.0f;
		}

		Vector3(float X, float Y, float Z) {
			x = X;
			y = Y;
			z = Z;
		}

		void zero() {
			x = y = z = 0.0f;
		}

		void normalize() {
			float n = std::sqrt(x*x + y*y + z*z);
			if (n == 0) { n = 1.0f; }
			x /= n;
			y /= n;
			z /= n;
		}

		bool isZero() {
			if (x == 0.0f && y == 0.0f && z == 0.0f) { return true; }
			return false;
		}

		Vector3 cross(const Vector3& o) const {
			Vector3 ret;
			ret.x = y*o.z - z*o.y;
			ret.y = z*o.x - x*o.z;
			ret.z = x*o.y - y*o.x;
			return ret;
		}

		float dot(const Vector3& o) const {
			return x*o.x + y*o.y + z*o.z;
		}

		Vector3& operator -= (const Vector3& o) {
			x -= o.x;
			y -= o.y;
			z -= o.z;
			return (*this);
		}

		Vector3 operator - (const Vector3& o) const {
			Vector3 tmp = (*this);
			tmp -= o;
			return tmp;
		}

		Vector3& operator += (const Vector3& o) {
			x += o.x;
			y += o.y;
			z += o.z;
			return (*this);
		}

		Vector3 operator + (const Vector3& o) const {
			Vector3 tmp = (*this);
			tmp += o;
			return tmp;
		}

		Vector3& operator *= (float f) {
			x *= f;
			y *= f;
			z *= f;
			return(*this);
		}

		Vector3 operator * (float f) const {
			Vector3 tmp = (*this);
			tmp *= f;
			return tmp;
		}

		Vector3& operator /= (float f) {
			x /= f;
			y /= f;
			z /= f;
			return (*this);
		}

		Vector3 operator / (float f) const {
			Vector3 tmp = (*this);
			tmp /= f;
			return tmp;
		}

	};



	struct Vector4 {
		float x;
		float y;
		float z;
		float w;

		Vector4() {
			x = y = z = w = 0.0f;
		}
		Vector4(const float& x, const float& y, const float& z, const float& w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}


		bool operator == (const Vector4& other) { return (x == other.x && y == other.y && z == other.z && w == other.w); }
		bool operator != (const Vector4& other) { return !(*this == other); }

	};

	class Matrix3 {

		Vector3 rows[3] = {
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f)
		};
	public:

		Vector3& operator[] (int index) {
			return rows[index];
		}

		const Vector3& operator[] (int index) const {
			return rows[index];
		}

		Matrix3 operator-(const Matrix3& other) const {
			Matrix3 res(*this);
			res -= other;
			return res;
		}

		Matrix3& operator-=(const Matrix3& other) {
			rows[0] -= other[0];
			rows[1] -= other[1];
			rows[2] -= other[2];
			return(*this);
		}

		Matrix3 operator+(const Matrix3& other) const {
			Matrix3 res(*this);
			res += other;
			return res;
		}

		Matrix3& operator+=(const Matrix3& other) {
			rows[0] += other[0];
			rows[1] += other[1];
			rows[2] += other[2];
			return (*this);
		}

	};

	class Matrix4 {

		float mtx[16];

	public:
		Matrix4() {
			std::memset(mtx, 0, sizeof(float) * 16);
			mtx[0] = mtx[5] = mtx[10] = mtx[15] = 1.0f;
		}

		void getRow(int row, Vector3& ref) {
			ref.x = mtx[row * 4 + 0];
			ref.y = mtx[row * 4 + 1];
			ref.z = mtx[row * 4 + 2];
		}

		void setRow(int row, Vector3& ref) {
			mtx[row * 4 + 0] = ref.x;
			mtx[row * 4 + 1] = ref.y;
			mtx[row * 4 + 2] = ref.z;
		}
	};


	class NiStream {

	private:
		std::iostream* stream = nullptr;
		unsigned int blockSize = 0;

	public:
		NiStream(std::iostream* stream) {
			this->stream = stream;
		};
		~NiStream() {}

		NiStream& getLine(std::string& version_str) {
			std::getline(*stream, version_str);
			return *this;
		}

		NiStream& writeLine(const char* ptr, std::streamsize count) {
			write(ptr, count);
			write("\n", 1);
			return *this;
		}

		NiStream& write(const char* str, std::streamsize size) {
			stream->write(str, size);
			blockSize += size;
			return *this;
		}

		NiStream& read(char* ptr, std::streamsize size) {
			stream->read(ptr, size);
			return *this;
		}

		template<typename T>
		NiStream& operator >> (T& t) {
			read((char*)&t, sizeof(T));
			return *this;
		}

		template<typename T>
		NiStream& operator<<(const T& t) {
			write((const char*)&t, sizeof(T));
			return *this;
		}

		void resetBlockSize() {
			blockSize = 0;
		}

		const unsigned int& getBlockSize() const {
			return blockSize;
		}

		std::streampos tellp() {
			return stream->tellp();
		}

		void seekg(std::streampos pos) {
			stream->seekg(pos);
		}

		bool eof() const {
			return stream->eof();
		}

	};


	template<typename T>
	class NiStringInterface {

	protected:
		std::string str;
		NiStringInterface() {
			static_assert(std::is_same<unsigned char, T>::value || std::is_same<unsigned short, T>::value || std::is_same<unsigned int, T>::value, "only unsigned [char, short and int] allowed");
		};
	public:
		NiStringInterface(const std::string& str) : str(str) {
			NiStringInterface();
		};
		virtual ~NiStringInterface() {};
		virtual void set(const std::string& str) {
			this->str = str;
		}

		virtual const std::string& get() const {
			return str;
		}

		size_t length() {
			return str.length();
		}

		virtual void read(NiStream& stream) {
			T size;
			stream >> size;

			if (size > 0) {
				std::unique_ptr<char> s(new char[size + 1]);
				stream.read(s.get(), size);
				s.get()[size] = 0;

				str = s.get();
			}
		}

		virtual void write(NiStream& stream) {
			stream.write(str.c_str(), str.length());
		}

	};


	template<typename T>
	class NiString : public NiStringInterface<T> {

	public:
		NiString() : NiStringInterface<T>() {};
		NiString(const std::string& str) : NiStringInterface<T>(str) {};
		virtual ~NiString() {};
		virtual void write(NiStream& stream) override {
			T size = T(this->str.length());
			stream << size;
			NiStringInterface<T>::write(stream);
		}
	};

	template<typename T>
	class NiStringNull : public NiStringInterface<T> {

	public:
		NiStringNull() : NiStringInterface<T>() {};
		NiStringNull(const std::string& str) : NiStringInterface<T>(str) {};
		virtual ~NiStringNull() {};
		virtual void write(NiStream& stream) override {
			T size = T(this->str.length()) + 1;
			stream << size;
			NiStringInterface<T>::write(stream);
			unsigned char nullTermination = 0;
			stream << nullTermination;
		}
	};


	struct Triangle {
		unsigned short p1;
		unsigned short p2;
		unsigned short p3;

		Triangle() {
			p1 = p2 = p3 = 0;
		}

		Triangle(unsigned short p1, unsigned short p2, unsigned short p3) {
			this->p1 = p1;
			this->p2 = p2;
			this->p3 = p3;
		}

	};


	struct HavokFilter {
		unsigned char layer = 1;
		unsigned char flagsAndPartNumber = 0;
		unsigned short group = 0;
	};

	struct CInfoProperty {
		unsigned int Data = 0;
		unsigned int Size = 0;
		unsigned int CapacityAndFlags = 0x80000000;
	};

	enum BroadPhaseType : unsigned char {
		BROAD_PHASE_INVALID,
		BROAD_PHASE_INTITY,
		BROAD_PHASE_PHANTOM,
		BROAD_PHASE_BORDER
	};

	enum hkResponseType : unsigned char {
		RESPONSE_INVALID,
		RESPONSE_SIMPLE_CONTACT,
		RESPONSE_REPORTING,
		RESPONSE_NONE
	};

	enum hkMotionSystem : unsigned char {
		MS_INVALID,
		MS_DYNAMIC,
		MS_SPHERE_INERTIA,
		MS_SPHERE_STABILIZED,
		MS_BOX_INERTIA,
		MS_BOX_STABILIZED,
		MS_KEYFRAMED,
		MS_FIXED,
		MS_THIN_BOX,
		MS_CHARACTER
	};

	enum hkDeactivatorType : unsigned char {
		DEACTIVATOR_INVALID,
		DEACTIVATOR_NEVER,
		DEACTIVATOR_SPATIAL
	};

	enum hkSolverDeactivation : unsigned char {
		SOLVER_DEACTIVATION_INVALID,
		SOLVER_DEACTIVATION_OFF,
		SOLVER_DEACTIVATION_LOW,
		SOLVER_DEACTIVATION_MEDIUM,
		SOLVER_DEACTIVATION_HIGH,
		SOLVER_DEACTIVATION_MAX
	};

	enum hkQualityType : unsigned char {
		QUALITY_INVALID,
		QUALITY_FIXED,
		QUALITY_KEYFRAMED,
		QUALITY_DEBRIS,
		QUALITY_MOVING,
		QUALITY_CRITICAL,
		QUALITY_BULLET,
		QUALITY_USER,
		QUALITY_CHARACTER,
		QUALITY_KEYFRAMED_REPORT,
	};

	enum ShaderType : unsigned int {
		SHADER_TALL_GRASS = 0,
		SHADER_DEFAULT = 1,
		SHADER_SKY = 10,
		SHADER_SKIN = 14,
		SHADER_WATER = 17,
		SHADER_LIGHTING30 = 29,
		SHADER_TILE = 32,
		SHADER_NO_LIGHTING = 33
	};



	struct TriangleData {
		Triangle triangle;
		unsigned short weldingInfo = 0;
	};

	struct VertexData {
		Vector3 vertex;
		float bitangentX = 0.0f;
		Vector2 uv;
		unsigned char normal[3];
		unsigned char bitangentY = 0;
		unsigned char tangent[3];
		unsigned char bitangentZ = 0;

		unsigned char vertexColors[4];

		float boneWeights[4];
		unsigned char boneIndices[4];

		float eyeData = 0.0f;


	};

	struct SkinTransform {

		Matrix3 rotation;
		Vector3 translation;
		float scale = 1.0f;
	};












	enum VertexAttribute : unsigned char {
		VA_POSITION = 0x0,
		VA_TEXCOORD0 = 0x1,
		VA_TEXCOORD1 = 0x2,
		VA_NORMAL = 0x3,
		VA_BINORMAL = 0x4,
		VA_COLOR = 0x5,
		VA_SKINNING = 0x6,
		VA_LANDDATA = 0x7,
		VA_EYEDATA = 0x8,
		VA_COUNT = 9
	};

	enum VertexFlags : unsigned short {
		VF_VERTEX = 1 << VA_POSITION,
		VF_UV = 1 << VA_TEXCOORD0,
		VF_UV_2 = 1 << VA_TEXCOORD1,
		VF_NORMAL = 1 << VA_NORMAL,
		VF_TANGENT = 1 << VA_BINORMAL,
		VF_COLORS = 1 << VA_COLOR,
		VF_SKINNED = 1 << VA_SKINNING,
		VF_LANDDATA = 1 << VA_LANDDATA,
		VF_EYEDATA = 1 << VA_EYEDATA,
		VF_FULLPREC = 0x400
	};

	enum Masks : uint64_t {
		DESC_MASK_VERT = 0xFFFFFFFFFFFFFFF0,
		DESC_MASK_UVS = 0xFFFFFFFFFFFFFF0F,
		DESC_MASK_NBT = 0xFFFFFFFFFFFFF0FF,
		DESC_MASK_SKCOL = 0xFFFFFFFFFFFF0FFF,
		DESC_MASK_DATA = 0xFFFFFFFFFFF0FFFF,
		DESC_MASK_OFFSET = 0xFFFFFF0000000000,
		DESC_MASK_FLAGS = ~(DESC_MASK_OFFSET)
	};


	class VertexDesc {
	private:
		uint64_t desc = 0;

	public:

		void clear() {
			desc = 0;
		}

		void set(const uint64_t desc) {
			this->desc = desc;
		}

		const uint64_t& get() const {
			return desc;
		}

		void setFlag(VertexFlags flag) {
			desc |= ((uint64_t)flag << 44);
		}

		void removeFlag(VertexFlags flag) {
			desc &= ~((uint64_t)flag << 44);
		}

		bool hasFlag(VertexFlags flag) {
			return ((desc >> 44) & flag) != 0;
		}

		VertexFlags getFlags() {
			return VertexFlags((desc & Masks::DESC_MASK_OFFSET) >> 44);
		}

		void clearAttributeOffsets() {
			desc &= Masks::DESC_MASK_OFFSET;
		}

		void setAttributeOffset(VertexAttribute attr, unsigned int offset) {
			if (attr != VertexAttribute::VA_POSITION) {
				desc = ((uint64_t)offset << (4 * (unsigned char)attr + 2)) | (desc & ~(15 << (4 * (unsigned char)attr + 4)));
			}
		}

		void setSize(unsigned int size) {
			desc &= Masks::DESC_MASK_VERT;
			desc |= (uint64_t)size >> 2;
		}

		void setFlags(VertexFlags flags) {
			desc |= ((uint64_t)flags << 44) | (desc & Masks::DESC_MASK_FLAGS);
		}

		void setDynamic() {
			desc &= Masks::DESC_MASK_UVS;
			desc |= 0x40;
		}


		bool isFullPrecision() { return this->hasFlag(VertexFlags::VF_FULLPREC); }
		void setFullPrecision(const bool set) {
			if (set) {
				this->setFlag(VertexFlags::VF_FULLPREC);
			} else {
				this->removeFlag(VertexFlags::VF_FULLPREC);
			}
		}

		bool hasVertices() { return this->hasFlag(VertexFlags::VF_VERTEX); }


		bool hasUVs() { return this->hasFlag(VertexFlags::VF_UV); }
		bool hasUVs2() { return this->hasFlag(VertexFlags::VF_UV_2); }
		void setUvs(const bool set) {
			if (set) {
				this->setFlag(VertexFlags::VF_UV);
			} else {
				this->removeFlag(VertexFlags::VF_UV);
			}
		}

		bool hasNormals() { return this->hasFlag(VertexFlags::VF_NORMAL); }
		void setNormals(const bool set) {
			if (set) {
				this->setFlag(VertexFlags::VF_NORMAL);
			} else {
				this->removeFlag(VertexFlags::VF_NORMAL);
			}
		}

		bool hasTangents() { return this->hasFlag(VertexFlags::VF_TANGENT); }
		void setTangents(const bool set) {
			if (set) {
				this->setFlag(VertexFlags::VF_TANGENT);
			} else {
				this->removeFlag(VertexFlags::VF_TANGENT);
			}
		}

		bool hasVertexColors() { return this->hasFlag(VertexFlags::VF_COLORS); }
		void setVertexColors(const bool set) {
			if (set) {
				this->setFlag(VertexFlags::VF_COLORS);
			} else
				this->removeFlag(VertexFlags::VF_COLORS);
		}

		bool isSkinned() { return this->hasFlag(VertexFlags::VF_SKINNED); }
		void setSkinned(const bool set) {
			if (set) {
				setFlag(VertexFlags::VF_SKINNED);
			} else {
				removeFlag(VertexFlags::VF_SKINNED);
			}
		}

		bool hasEyeData() { return this->hasFlag(VertexFlags::VF_EYEDATA); }




	};


}