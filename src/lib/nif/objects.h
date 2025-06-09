#pragma once

#include "types.h"
#include "../half/half.hpp"
#include "NiReferenceable.h"
#include "NiReference.h"
#include "NiReferenceArray.h"

namespace nif {

	class NiObject : public NiReferenceable {

	public:
		virtual ~NiObject() {}

		virtual const std::string& getBlockName() const = 0;

		virtual void read(NiStream& stream) = 0;
		virtual void write(NiStream& stream) = 0;


	};


	class NiUnknow : public NiObject {

	private:
		std::string blockName;
		std::vector<char> data;
		unsigned int size = 0;

	private:
		NiUnknow() {};
		NiUnknow(const std::string& blockName, const unsigned int size) : blockName(blockName), size(size) {
			data.resize(size);
		};

	public:
		virtual ~NiUnknow() {}

		NiUnknow(NiStream& stream, const std::string& blockName, const unsigned int size) : NiUnknow(blockName, size) {
			read(stream);
		};

		const std::string& getBlockName() const override {
			return blockName;
		}

		const std::vector<char>& getData() const {
			return data;
		}

		const unsigned int& getSize() const {
			return size;
		}

		virtual void read(NiStream& stream) override {
			stream.read(&data[0], size);
		}

		virtual void write(NiStream& stream) override {
			stream.write(&data[0], size);
		}

	};


	class NiObjectNet : public NiObject {

	public:
		int nameRef = 0xFFFFFFFF;
		std::unique_ptr<NiReferenceArray> extraDataList;
		std::unique_ptr<NiReference> controllerRef;

	public:
		NiObjectNet();
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class NiAvObject : public NiObjectNet {

	public:
		unsigned int flags = 0;
		Vector3 translation;
		Matrix3 rotation;
		float scale = 1.0f;
		std::unique_ptr<NiReferenceArray> properties;
		std::unique_ptr<NiReference> collisionRef;


	public:
		NiAvObject();
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



















	class NiNode : public NiAvObject {

	public:
		std::unique_ptr<NiReferenceArray> children;
		std::unique_ptr<NiReferenceArray> effects;

	public:
		NiNode();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSFadeNode : public NiNode {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSFadeNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class BSMultiBoundNode : public NiNode {

	public:
		std::unique_ptr<NiReference> multiBoundRef;
		unsigned int cullingMode = 0;

	public:
		BSMultiBoundNode();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSMultiBoundNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class NiBillboardNode : public NiNode {

		unsigned short billboardMode = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiBillboardNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class BSOrderedNode : public NiNode {
	private:
		Vector4 alphaSortBound;
		unsigned char staticBound = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSOrderedNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSValueNode : public NiNode {
	private:
		int value = 0;
		unsigned char valueNodeFlags = 0x0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSValueNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSRangeNode : public NiNode {
	private:
		unsigned char min = 0;
		unsigned char max = 0;
		unsigned char current = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSRangeNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSBlastNode : public BSRangeNode {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSBlastNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class BSDamageStage : public BSBlastNode {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSDamageStage"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};

	class BSDebrisNode : public BSRangeNode {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSDebrisNode"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};























	class NiExtraData : public NiObject {
	private:
		int nameRef = 0xFFFFFFFF;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiExtraData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiIntegerExtraData : public NiExtraData {
	private:
		unsigned int integerData = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiIntegerExtraData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiStringExtraData : public NiExtraData {
	private:
		int stringData = 0xFFFFFFFF;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiStringExtraData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class BSXFlags : public NiIntegerExtraData {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSXFlags"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};














	class bhkNiCollisionObject : public NiObject {

	public:
		std::unique_ptr<NiReference> targetRef;
		unsigned short flags = 1;
		std::unique_ptr<NiReference> bodyRef;

	public:
		bhkNiCollisionObject();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkNiCollisionObject"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class bhkCollisionObject : public bhkNiCollisionObject {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkCollisionObject"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};

	class bhkPCollisionObject : public bhkNiCollisionObject {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkPCollisionObject"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};

	class bhkSPCollisionObject : public bhkPCollisionObject {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkSPCollisionObject"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};




	class bhkWorldObject : public NiObject {

	public:
		std::unique_ptr<NiReference> shapeRef;
		HavokFilter filter;
		unsigned char bhkWorldObject_unknown1[4] = { 0 };
		BroadPhaseType broadPhaseType = BroadPhaseType::BROAD_PHASE_INVALID;
		unsigned char bhkWorldObject_unknown2[3] = { 0 };
		CInfoProperty cInfoProperty;

	public:
		bhkWorldObject();
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class bhkRigidBody : public bhkWorldObject {

	public:
		unsigned char collisionResponse = hkResponseType::RESPONSE_SIMPLE_CONTACT;
		unsigned char bhkRigidBody_unknown1 = 0;
		unsigned short processContactCallbackDelay = 0xFFFF;
		unsigned char bhkRigidBody_unknown2[4] = { 0 };
		HavokFilter filterCopy;
		unsigned char bhkRigidBody_unknown3[4] = { 0 };
		unsigned char collisionResponse2 = hkResponseType::RESPONSE_SIMPLE_CONTACT;
		unsigned char bhkRigidBody_unknown4 = 0;
		unsigned short processContactCallbackDelay2 = 0xFFFF;
		unsigned char bhkRigidBody_unknown5[4] = { 0 };
		Vector4 translation;
		Vector4 rotation;
		Vector4 linearVelocity;
		Vector4 angularVelocity;
		float inertiaTensor[12] = { 0.0 };
		Vector4 center;
		float mass = 0.0f;
		float linearDamping = 0.0f;
		float angularDamping = 0.0f;
		float friction = 0.0f;
		float restitution = 0.0f;
		float maxLinearVelocity = 0.0f;
		float maxAngularVelocity = 0.0f;
		float penetrationDepth = 0.0f;
		unsigned char motionSystem = hkMotionSystem::MS_DYNAMIC;
		unsigned char deactivatorType = hkDeactivatorType::DEACTIVATOR_NEVER;
		unsigned char solverDeactivation = hkSolverDeactivation::SOLVER_DEACTIVATION_OFF;
		unsigned char qualityType = hkQualityType::QUALITY_FIXED;
		unsigned char bhkRigidBody_unknown6[12] = { 0 };
		//unsigned int numConstraints = 0;
		//std::vector<int> constraints;
		std::unique_ptr<NiReferenceArray> constraints;
		unsigned int bodyFlags = 0;


		float timeFactor = 1.0f;
		float rollingFrictionMult = 1.0f;
		unsigned char enableDeactivation = 0;
		float gravityFactor = 1.0f;
		unsigned int bhkRigidBody_unknown7 = 0;

		unsigned short bhkRigidBody_unknown8 = 0;




	public:
		bhkRigidBody();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkRigidBody"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class bhkRigidBodyT : public bhkRigidBody {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkRigidBodyT"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};

	class bhkPhantom : public bhkWorldObject {};

	class bhkShapePhantom : public bhkPhantom {};

	class bhkSimpleShapePhantom : public bhkShapePhantom {
	private:
		unsigned char bhkSimpleShapePhantom_unknown1[8] = { 0 };
		Matrix4 transform;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkSimpleShapePhantom"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};













	class bhkShape : public NiObject {
	};


	class bhkMoppBvTreeShape : public bhkShape {

	public:
		std::unique_ptr<NiReference> shapeRef;
		unsigned char bhkMoppBvTreeShape_unknown1[12] = { 0 };
		float shapeScale = 0.0f;
		unsigned int moppDataSize = 0;
		Vector3 origin;
		float scale = 0.0f;
		std::vector<unsigned char> moppData;

		unsigned char buildType = 2;


	public:
		bhkMoppBvTreeShape();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkMoppBvTreeShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class bhkSphereRepShape : public bhkShape {
	public:
		unsigned int material = 0;
		float radius = 0.0f;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class bhkConvexShape : public bhkSphereRepShape {};


	class bhkBoxShape : public bhkConvexShape {

	public:
		unsigned char bhkBoxShape_unknown1[8] = { 0 };
		Vector3 dimensions;
		float bhkBoxShape_unknown2 = 0.0f;
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkBoxShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class bhkSphereShape : public bhkConvexShape {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkSphereShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class bhkCapsuleShape : public bhkConvexShape {
	public:
		unsigned char bhkCapsuleShape_unknown1[8] = { 0 };
		Vector3 point1;
		float radius1 = 0.0f;
		Vector3 point2;
		float radius2 = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkCapsuleShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class bhkTransformShape : public bhkConvexShape {
	public:
		std::unique_ptr<NiReference> shapeRef;
		unsigned char bhkTransformShape_unknown1[8] = { 0 };
		Matrix4 transform;

	public:
		bhkTransformShape();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkTransformShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class bhkConvexTransformShape : public bhkTransformShape {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkConvexTransformShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class bhkListShape : public bhkShape {
	public:
		std::unique_ptr<NiReferenceArray> subShapeRefs;
		unsigned int material = 0;
		CInfoProperty childShapeProperty;
		CInfoProperty childFilterProperty;
		unsigned int num_bhkListShape_unknown1 = 0;
		std::vector<unsigned int> bhkListShape_unknown1;

	public:
		bhkListShape();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkListShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class bhkConvexListShape : public bhkShape {
	public:
		std::unique_ptr<NiReferenceArray> shapeRefs;
		unsigned int material = 0;
		float radius = 0.0f;
		unsigned char bhkConvexListShape_unknown1[4] = { 0 };
		unsigned char bhkConvexListShape_unknown2[4] = { 0 };
		CInfoProperty childShapeProp;
		unsigned char bhkConvexListShape_unknown3 = 0;
		unsigned char bhkConvexListShape_unknown4[4] = { 0 };

	public:
		bhkConvexListShape();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkConvexListShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class bhkConvexVerticesShape : public bhkConvexShape {

	public:

		CInfoProperty verticesProperty;
		CInfoProperty normalsProperty;

		unsigned int numVerts = 0;
		std::vector<Vector4> vertices;
		unsigned int numNormals = 0;
		std::vector<Vector4> normals;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkConvexVerticesShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class bhkPackedNiTriStripsShape : public bhkShape {

	public:
		unsigned int userData = 0;
		unsigned int bhkPackedNiTriStripsShape_unknown1 = 0;
		float radius = 0.0f;
		unsigned int bhkPackedNiTriStripsShape_unknown2 = 0;
		Vector4 scale;
		float radius2 = 0.0f;
		Vector4 scale2;
		std::unique_ptr<NiReference> dataRef;

	public:
		bhkPackedNiTriStripsShape();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkPackedNiTriStripsShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};





	class bhkPackedNiTriStripsData : public bhkShape {

		struct SubShape {
			HavokFilter filter;
			unsigned int numVertices = 0;
			unsigned int material = 0;
		};

	public:
		unsigned int numTriangles = 0;
		std::vector<TriangleData> triangleData;
		unsigned int numVertices = 0;
		unsigned char hkPackedNiTriStripsData_unknown1 = 0;
		std::vector<Vector3> vertices;
		unsigned short numSubShapes = 0;
		std::vector<SubShape> subShapes;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "hkPackedNiTriStripsData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class bhkNiTriStripsShape : public bhkShape {
	public:

		unsigned int material = 0;
		float radius = 0.1f;
		unsigned int bhkNiTriStripsShape_unknown1[5] = { 0 };
		unsigned int growBy = 1;
		Vector4 scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		std::unique_ptr<NiReferenceArray> partRefs;

		unsigned int numFilters = 0;
		std::vector<HavokFilter> filters;

	public:
		bhkNiTriStripsShape();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkNiTriStripsShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};





	// ------------------------------------------------------------ <property> -----------------------------------------------------------------------



	class NiProperty : public NiObjectNet {};


	class NiAlphaProperty : public NiProperty {

	public:
		unsigned short flags = 4844;
		unsigned char threshold = 128;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiAlphaProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiStencilProperty : public NiProperty {

	private:
		unsigned short flags = 0;
		unsigned int stenscilRef;
		unsigned int stenscilMask;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiStencilProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};

	class NiMaterialProperty : public NiProperty {

	public:
		Vector3 specularColor;
		Vector3 emissiveColor;
		float glossiness = 1.0f;
		float alpha = 1.0f;
		float emissiveMult = 1.0f;
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiMaterialProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSShaderTextureSet : public NiObject {

	public:
		unsigned int numTextures = 10;
		std::vector<NiString<unsigned int>> textures = std::vector<NiString<unsigned int>>(10);


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSShaderTextureSet"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class NiSourceTexture : public NiObjectNet {
	private:
		unsigned char useExternal = 0;
		int fileName = 0xFFFFFFFF;
		std::unique_ptr<NiReference> dataRef;
		unsigned int pixelLayout = 0;
		unsigned int mipMapFormat = 0;
		unsigned int alphaFormat = 0;
		unsigned char isStatic = 0;
		unsigned char directRender = 0;
		unsigned char persistentRenderData = 0;

	public:
		NiSourceTexture();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiSourceTexture"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class NiTexturingProperty : public NiProperty {

		struct TextureTransform {
			Vector2 translation;
			Vector2 tiling;
			float wRotation = 0.0f;
			unsigned int transformType = 0;
			Vector2 offset;
		};

		class TextureDescription {
		public:
			std::unique_ptr<NiReference> sourceRef;
			unsigned short flags = 0;
			bool hasTexTransform = false;
			TextureTransform transform;

		public:
			TextureDescription(NiReferenceable* owner) {
				sourceRef = std::make_unique<NiReference>(owner);
			}
			void read(NiStream& stream) {
				sourceRef->read(stream);
				stream >> flags;
				stream >> hasTexTransform;

				if (hasTexTransform)
					stream >> transform;
			}

			void write(NiStream& stream) {
				sourceRef->write(stream);
				stream << flags;
				stream << hasTexTransform;

				if (hasTexTransform)
					stream << transform;
			}

		};

		class ShaderTexDesc {
		public:
			bool isUsed = false;
			std::unique_ptr<TextureDescription> data;
			unsigned int mapIndex = 0;

		public:
			ShaderTexDesc(NiReferenceable* owner) {
				data = std::make_unique<TextureDescription>(owner);
			}

			void read(NiStream& stream) {
				stream >> isUsed;

				if (isUsed) {
					data->read(stream);
					stream >> mapIndex;
				}
			}

			void write(NiStream& stream) {
				stream << isUsed;

				if (isUsed) {
					data->write(stream);
					stream << mapIndex;
				}
			}
		};

	public:
		NiTexturingProperty();
		unsigned short flags = 0;
		unsigned int textureCount = 0;
		unsigned char hasBaseTex = 0;
		std::unique_ptr<TextureDescription> baseTex;
		unsigned char hasDarkTex = 0;
		std::unique_ptr<TextureDescription> darkTex;
		unsigned char hasDetailTex = 0;
		std::unique_ptr<TextureDescription> detailTex;
		unsigned char hasGlossTex = 0;
		std::unique_ptr<TextureDescription> glossTex;
		unsigned char hasGlowTex = 0;
		std::unique_ptr<TextureDescription> glowTex;
		unsigned char hasBumpTex = 0;
		std::unique_ptr<TextureDescription> bumpTex;
		float lumaScale = 1.0f;
		float lumaOffset = 0.0f;
		Vector4 bumpMatrix;
		unsigned char hasNormalTex = 0;
		std::unique_ptr<TextureDescription> normalTex;
		unsigned char hasParallaxTex = 0;
		std::unique_ptr<TextureDescription> parallaxTex;
		float parallaxFloat = 0.0f;
		unsigned char hasDecalTex0 = 0;
		std::unique_ptr<TextureDescription> decalTex0;
		unsigned char hasDecalTex1 = 0;
		std::unique_ptr<TextureDescription> decalTex1;
		unsigned char hasDecalTex2 = 0;
		std::unique_ptr<TextureDescription> decalTex2;
		unsigned char hasDecalTex3 = 0;
		std::unique_ptr<TextureDescription> decalTex3;
		unsigned int numShaderTex = 0;
		std::vector<std::unique_ptr<ShaderTexDesc>> shaderTex;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTexturingProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};








	// ------------------------------------------------------------ </property> -----------------------------------------------------------------------





	// ------------------------------------------------------------ <shader> -----------------------------------------------------------------------


	class NiShaderProperty : public NiProperty {};

	class BSShaderProperty : public NiShaderProperty {

	public:
		unsigned short flags = 1;
		ShaderType shaderType = ShaderType::SHADER_DEFAULT;
		unsigned int shaderFlags1 = 0x80400201;
		unsigned int shaderFlags2 = 0x00000021;
		float environmentMapScale = 1.0f;

		Vector2 uvOffset;
		Vector2 uvScale = Vector2(1.0f, 1.0f);

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class BSShaderLightingProperty : public BSShaderProperty {

	public:
		unsigned int textureClampMode = 3;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class BSShaderPPLightingProperty : public BSShaderLightingProperty {

	public:
		std::unique_ptr<NiReference> textureSetRef;
		float refractionStrength = 0.0f;
		int refractionFirePeriod = 0;
		float parallaxMaxPasses = 4.0f;
		float parallaxScale = 1.0f;

	public:
		BSShaderPPLightingProperty();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSShaderPPLightingProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class Lighting30ShaderProperty : public BSShaderPPLightingProperty {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "Lighting30ShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};




	class BSShaderNoLightingProperty : public BSShaderLightingProperty {

	public:
		NiString<unsigned int> baseTexture;
		float falloffStartAngle = 1.0f;
		float falloffStopAngle = 0.0f;
		float falloffStartOpacity = 1.0f;
		float falloffStopOpacity = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSShaderNoLightingProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class TallGrassShaderProperty : public BSShaderProperty {

	public:
		NiString<unsigned int> fileName;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "TallGrassShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class WaterShaderProperty : public BSShaderProperty {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "WaterShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};

	class SkyShaderProperty : public BSShaderLightingProperty {
	public:
		NiString<unsigned int> fileName;
		unsigned int skyObjectType = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "SkyShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class TileShaderProperty : public BSShaderLightingProperty {
	public:
		NiString<unsigned int> fileName;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "TileShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	// ------------------------------------------------------------ </shader> -----------------------------------------------------------------------






	// ------------------------------------------------------------ <geometry> -----------------------------------------------------------------------



	class NiGeometry : public NiAvObject {
	public:
		std::unique_ptr<NiReference> skinRef;
	public:
		NiGeometry();
	};

	class NiGeometry3 : public virtual NiGeometry {

	public:
		std::unique_ptr<NiReference> dataRef;

		unsigned int numMaterials = 0;
		std::vector<int>  materialNameRefs;
		std::vector<int>  materialExtraData;

		int activeMaterial = 0;
		unsigned char materialNeedsUpdate = 0;


	public:
		NiGeometry3();
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class NiTriBasedGeom : public NiGeometry3 {};


	class NiTriShape : public NiTriBasedGeom {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTriShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};

	class BSSegmentedTriShape : public NiTriShape {

		struct SegmentData {
			unsigned char flags = 0;
			unsigned int index = 0;
			unsigned int numTrisInSegnent = 0;

		};

	public:

		unsigned int numSegments;
		std::vector<SegmentData> segments;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSSegmentedTriShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class NiTriStrips : public NiTriBasedGeom {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTriStrips"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};








	class NiGeometryData : public NiObject {

	public:
		int groupID = 0;
		unsigned short numVertices = 0;
		unsigned char keepFlags = 0;
		unsigned char compressFlags = 0;
		unsigned char hasVertices = 0;
		std::vector<Vector3> vertices;
		unsigned short bsVectorFlags = 0;
		unsigned char hasNormals = 0;
		std::vector<Vector3> normals;
		std::vector<Vector3> tangents;
		std::vector<Vector3> bitangents;
		Vector3 center;
		float radius = 0.0f;
		unsigned char hasVertexColors = 0;
		std::vector<Vector4> vertexColors;
		std::vector<std::vector<Vector2>> uvSets;
		unsigned short consistencyFlags = 0;
		std::unique_ptr<NiReference> additionalDataRef;

		unsigned int materialCRC = 0;


	public:
		NiGeometryData();

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

		virtual std::unique_ptr<std::vector<Triangle>> getTriangles() const {
			return nullptr;
		}

	};

	class NiTriBasedGeometryData : public NiGeometryData {

	public:
		unsigned short numTriangles = 0;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class NiTriShapeData : public NiTriBasedGeometryData {

		struct MatchGroup {
			unsigned short numVertices = 0;
			std::vector<unsigned short> vertexIndices;
		};

	public:
		unsigned int numTrianglePoints = 0;
		unsigned char hasTriangles = 0;
		std::vector<Triangle> triangles;

		unsigned short numMatchGroups = 0;
		std::vector<MatchGroup> matchGroups;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTriShapeData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

		virtual std::unique_ptr<std::vector<Triangle>> getTriangles() const override;
	};

	class NiTriStripsData : public NiTriBasedGeometryData {

	public:
		unsigned short numStrips = 0;
		std::vector<unsigned short> stripLengths;
		unsigned char hasPoints = 0;
		std::vector<std::vector<unsigned short>> points;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTriStripsData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

		virtual std::unique_ptr<std::vector<Triangle>> getTriangles() const override;

	};



	// ------------------------------------------------------------ </geometry> -----------------------------------------------------------------------




	class NiGeometry4 : public virtual NiGeometry {

	public:
		Vector3 boundingSphereCenter;
		float boundingSphereRadius = 0.0f;

		std::unique_ptr<NiReference> shaderPropertyRef;
		std::unique_ptr<NiReference> alphaPropertyRef;

		VertexDesc vertexDesc;

	public:
		NiGeometry4();
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class BSTriShape : public NiGeometry4 {


	public:
		unsigned int dataSize = 0;
		unsigned short numVertices = 0;
		std::vector<VertexData> vertexData;

		unsigned int numTriangles = 0;
		std::vector<Triangle> triangles;

	public:
		BSTriShape();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSTriShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class BSDynamicTriShape : public BSTriShape {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSDynamicTriShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class BSSubIndexTriShape : public BSTriShape {

	public:
		struct SubSegment {
			unsigned int startIndex = 0;
			unsigned int numPrimitives = 0;
			unsigned int arrayIndex = 0;
			unsigned int unused = 0;
		};

		struct Segment {
			unsigned int startIndex = 0;
			unsigned int numPrimitives = 0;
			unsigned int parentArrayIndex = 0xFFFFFFFF;
			unsigned int numSubSegments = 0;
			std::vector<SubSegment> subSegments;
		};

		struct PerSegmentData {
			unsigned int userIndex = 0;
			unsigned int boneID = 0xFFFFFFFF;
			unsigned int numCutOffsets = 0;
			std::vector<float> cutOffsets;
		};

		struct SegmentData {
			unsigned int numSegments = 0;
			unsigned int totalSegments = 0;
			std::vector<unsigned int> arrayIndices;
			std::vector<PerSegmentData> arrayPerSegmentData;
			NiString<unsigned short> ssfFile;
		};

	public:
		unsigned int numPrimitives = 0;
		unsigned int numSegments = 0;
		unsigned int totalSegments = 0;
		std::vector<Segment> segments;
		SegmentData segmentData;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSSubIndexTriShape"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;


	};



	class BSLightingShaderProperty : public BSShaderProperty {

	public:
		unsigned int shaderType = 0;

		std::unique_ptr<NiReference> textureSetRef;
		Vector3 emissiveColor;
		float emissiveMultiple = 1.0f;
		int wetMaterialNameRef = 0xFFFFFFFF;
		unsigned int textureClampMode = 3;
		float alpha = 1.0f;
		float refractionStrength = 0.0f;
		float smoothness = 1.0f;
		Vector3 specularColor = Vector3(1.0f, 1.0f, 1.0f);
		float specularStrength = 1.0f;
		float subsurfaceRolloff = 0.0f;
		float rimlightPower = std::numeric_limits<float>::max();
		float backlightPower = 0.0f;
		float grayscaleToPaletteScale = 1.0f;
		float fresnelPower = 5.0f;
		float wetnessSpecScale = 1.0f;
		float wetnessSpecPower = 1.0f;
		float wetnessMinVar = 0.1f;
		float wetnessEnvmapScale = 1.0f;
		float wetnessFresnelPower = 1.0f;
		float wetnessMetalness = 0.0f;

		float environmentMapScale = 1.0f;
		unsigned short unkEnvmapShort = 0;
		Vector3 skinTintColor = Vector3(1.0f, 1.0f, 1.0f);
		unsigned int unknownSkinTintInt = 0;
		Vector3 hairTintColor = Vector3(1.0f, 1.0f, 1.0f);
		float maxPasses = 1.0f;
		float scale = 1.0f;
		float parallaxInnerLayerThickness = 0.0f;
		float parallaxRefractionScale = 1.0f;
		Vector2 parallaxInnerLayerTextureScale = Vector2(1.0f, 1.0f);
		float parallaxEnvmapStrength = 1.0f;
		Vector4 sparkleParameters;
		float eyeCubemapScale = 1.0f;
		Vector3 eyeLeftReflectionCenter;
		Vector3 eyeRightReflectionCenter;

	public:
		BSLightingShaderProperty();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSLightingShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class BSEffectShaderProperty : public BSShaderProperty {

	public:
		NiString<unsigned int> sourceTexture;
		unsigned char textureClampMode = 3;
		unsigned char lightingInfluence = 0;
		unsigned char envMapMinLOD = 0;
		unsigned char unknownByte = 0;
		float falloffStartAngle = 1.0f;
		float falloffStopAngle = 1.0f;
		float falloffStartOpacity = 0.0f;
		float falloffStopOpacity = 0.0f;
		Vector4 emissiveColor;
		float emissiveMultiple = 0.0f;
		float softFalloffDepth = 0.0f;
		NiString<unsigned int> greyscaleTexture;
		NiString<unsigned int> envMapTexture;
		NiString<unsigned int> normalTexture;
		NiString<unsigned int> envMaskTexture;
		float envMapScale = 1.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSEffectShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSWaterShaderProperty : public BSShaderProperty {
	private:
		unsigned int waterFlags = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSWaterShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class BSSkyShaderProperty : public BSShaderProperty {
	public:
		NiString<unsigned int> baseTexture;
		unsigned int skyFlags = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSSkyShaderProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};







	class BSMasterParticleSystem : public NiNode {
	private:
		unsigned short maxEmitterObjects = 0;
		std::unique_ptr<NiReferenceArray> particleSysRefs;

	public:
		BSMasterParticleSystem();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSMasterParticleSystem"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};







	class NiParticleSystem : public NiGeometry3, public NiGeometry4 {

	public:
		unsigned char worldSpace = 0;
		std::unique_ptr<NiReferenceArray> modifiers;

		unsigned short farBegin = 0;
		unsigned short farEnd = 0;
		unsigned short nearBegin = 0;
		unsigned short nearEnd = 0;

	public:
		NiParticleSystem();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiParticleSystem"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSStripParticleSystem : public NiParticleSystem {
	public:
		BSStripParticleSystem() : NiParticleSystem() {}
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSStripParticleSystem"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};





	class NiPSysData : public NiObject {
	private:
		int groupID = 0;
		unsigned short numVertices = 0;
		unsigned char keepFlags = 0;
		unsigned char compressFlags = 0;
		unsigned char hasVertices = 0;
		std::vector<Vector3> vertices;
		unsigned short bsVectorFlags = 0;
		unsigned char hasNormals = 0;
		std::vector<Vector3> normals;
		std::vector<Vector3> tangents;
		std::vector<Vector3> bitangents;
		Vector3 center;
		float radius = 0.0f;
		unsigned char hasVertexColors = 0;
		std::vector<Vector4> vertexColors;
		std::vector<std::vector<Vector2>> uvSets;
		unsigned short consistencyFlags = 0;
		std::unique_ptr<NiReference> additionalDataRef;

		unsigned int materialCRC = 0;

		bool hasRadii = false;
		unsigned short numActive = 0;
		bool hasSizes = false;
		bool hasRotations = false;
		bool hasRotationAngles = false;
		bool hasRotationAxes = false;
		bool hasTextureIndices = false;

		unsigned int numSubtexOffsets = 0;
		std::vector<Vector4> subtexOffsets;

		float aspectRatio;
		unsigned short aspectFlags;
		float speedToAspectAspect2;
		float speedToAspectSpeed1;
		float speedToAspectSpeed2;
		bool hasRotationSpeeds = false;

	public:
		NiPSysData();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSStripPSysData : public NiPSysData {
	private:
		unsigned short maxPointCount = 0;
		unsigned int startCapSize = 0;
		unsigned int endCapSize = 0;
		unsigned char doZPrepass = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSStripPSysData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class NiTimeController : public NiObject {

	public:
		std::unique_ptr<NiReference> nextControllerRef;
		unsigned short flags = 0;
		float frequency = 1.0f;
		float phase = 0.0f;
		float startTime = 0.0f;
		float stopTime = 0.0f;
		std::unique_ptr<NiReference> targetRef;

	public:
		NiTimeController();

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};

	class NiInterpolatorController : public NiTimeController {

	public:
		std::unique_ptr<NiReference> interpolatorRef;

	public:
		NiInterpolatorController();

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};

	class NiAlphaController : public NiInterpolatorController {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiAlphaController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};





	class FloatController : public NiInterpolatorController {
	public:
		unsigned int typeOfControlledVariable = 0;
	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class ColorController : public NiInterpolatorController {
	public:
		unsigned int typeOfControlledColor = 0;
	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};






	class BSEffectShaderPropertyFloatController : public FloatController {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSEffectShaderPropertyFloatController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};

	class BSEffectShaderPropertyColorController : public ColorController {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSEffectShaderPropertyColorController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};



	class NiMaterialColorController : public NiInterpolatorController {

	public:
		unsigned short targetColor = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiMaterialColorController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSLightingShaderPropertyFloatController : public FloatController {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSLightingShaderPropertyFloatController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};

	class BSLightingShaderPropertyColorController : public ColorController {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSLightingShaderPropertyColorController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class NiTextureTransformController : public NiInterpolatorController {

	public:
		enum TransformType : unsigned int {
			TT_TRANSLATE_U,
			TT_TRANSLATE_V,
			TT_ROTATE,
			TT_SCALE_U,
			TT_SCALE_V
		};

	public:
		unsigned char shaderMap = 0;
		unsigned int textureSlot = 0;
		TransformType operation = TT_TRANSLATE_U;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTextureTransformController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class BSMaterialEmittanceMultController : public NiInterpolatorController {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSMaterialEmittanceMultController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};



	class BSRefractionStrengthController : public NiInterpolatorController {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSRefractionStrengthController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};











	class NiDynamicEffect : public NiAvObject {
	public:
		unsigned char switchState = 0;
		unsigned int numAffectedNodes = 0;
		std::vector<int> affectedNodes;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiLight : public NiDynamicEffect {
	private:
		float dimmer = 0.0f;
		Vector3 ambientColor;
		Vector3 diffuseColor;
		Vector3 specularColor;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class NiAmbientLight : public NiLight {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiAmbientLight"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class NiPointLight : public NiLight {
	private:
		float constantAttenuation = 0.0f;
		float linearAttenuation = 0.0f;
		float quadraticAttenuation = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPointLight"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class NiSpotLight : public NiPointLight {
	private:
		float cutoffAngle = 0.0f;
		float unkFloat = 0.0f;
		float exponent = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiSpotLight"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class NiSequence : public NiObject {

	public:
		class ControllerBlock {

		public:
			std::unique_ptr<NiReference> interpolatorRef;
			std::unique_ptr<NiReference> controllerRef;
			unsigned char priority;
			int nodeName = 0xFFFFFFFF;
			int propType = 0xFFFFFFFF;
			int controllerType = 0xFFFFFFFF;
			int controllerID = 0xFFFFFFFF;
			int interpolatorID = 0xFFFFFFFF;

		public:
			ControllerBlock(NiReferenceable* referenceable) {
				interpolatorRef = std::make_unique<NiReference>(referenceable);
				controllerRef = std::make_unique<NiReference>(referenceable);
			}

		};

	public:
		int nameRef = 0xFFFFFFFF;
		unsigned int numControlledBlocks = 0;
		unsigned int arrayGrowBy = 0;
		std::vector<std::unique_ptr<ControllerBlock>> controlledBlocks;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiSequence"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};

	class NiControllerSequence : public NiSequence {
	public:
		float weight = 1.0f;
		std::unique_ptr<NiReference> textKeyRef;
		unsigned int cycleType = 0;
		float frequency = 0.0f;
		float startTime = 0.0f;
		float stopTime = 0.0f;
		std::unique_ptr<NiReference> managerRef;
		int accumRootName = 0xFFFFFFFF;

		std::unique_ptr<NiReferenceArrayShort> animNoteRefs;


	public:
		NiControllerSequence();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiControllerSequence"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};










	class NiBoneContainer : public NiObject {
	public:
		std::unique_ptr<NiReferenceArray> boneRefs;

	public:
		NiBoneContainer();
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};

	class NiSkinInstance : public NiBoneContainer {
	public:
		std::unique_ptr<NiReference> dataRef;
		std::unique_ptr<NiReference> skinPartitionRef;
		std::unique_ptr<NiReference> skeletonRoot;

	public:
		NiSkinInstance();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiSkinInstance"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class BSDismemberSkinInstance : public NiSkinInstance {
	public:
		struct PartitionInfo {
			unsigned short flags = 0;
			unsigned short partId;
		};

	public:
		unsigned int numPartitions = 0;
		std::vector<PartitionInfo> partitions;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSDismemberSkinInstance"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class NiSkinData : public NiObject {
	public:

		struct VertexWeight {
			unsigned short index;
			float weight;
		};

		struct BoneData {
			SkinTransform boneTransform;
			Vector3 center;
			float radius = 0.0f;
			unsigned short numVertices = 0;
			std::vector<VertexWeight> vertexWeights;
		};

		SkinTransform skinTransform;
		unsigned int numBones = 0;
		unsigned char hasVertexWeights = 1;
		std::vector<BoneData> bones;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiSkinData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiSkinPartition : public NiObject {
	public:

		struct BoneIndices {
			unsigned char i1 = 0;
			unsigned char i2 = 0;
			unsigned char i3 = 0;
			unsigned char i4 = 0;
		};

		struct VertexWeight {
			float w1 = 0.0f;
			float w2 = 0.0f;
			float w3 = 0.0f;
			float w4 = 0.0f;
		};

		struct PartitionBlock {
			unsigned short numVertices = 0;
			unsigned short numTriangles = 0;
			unsigned short numBones = 0;
			unsigned short numStrips = 0;
			unsigned short numWeightsPerVertex = 0;
			std::vector<unsigned short> bones;
			unsigned char hasVertexMap = 0;
			std::vector<unsigned short> vertexMap;
			unsigned char hasVertexWeights = 0;
			std::vector<VertexWeight> vertexWeights;
			std::vector<unsigned short> stripLengths;
			unsigned char hasFaces = 0;
			std::vector<std::vector<unsigned short>> strips;
			std::vector<Triangle> triangles;
			unsigned char hasBoneIndices = 0;
			std::vector<BoneIndices> boneIndices;
		};

		unsigned int numPartitions = 0;
		std::vector<PartitionBlock> partitions;


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiSkinPartition"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};














	class BSSkinInstance : public NiBoneContainer {
	public:

		std::unique_ptr<NiReference> skeletonRoot;
		std::unique_ptr<NiReference> dataRef;

		unsigned int num_BSSkinInstance_Unknown = 0;
		std::vector<Vector3> BSSkinInstance_Unknown;

	public:
		BSSkinInstance();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSSkin::Instance"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class BSSkinBoneData : public NiObject {
	public:
		unsigned int numBones = 0;

		struct BoneData {
			Vector3 center;
			float radius = 0.0f;
			SkinTransform boneTransform;
		};

		std::vector<BoneData> boneData;


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSSkin::BoneData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};









	//---------------------------------------------- new ----------------------------------------


	// has no references
	class NiBoolData {}; 
	class NiTransformData {};
	class NiTextKeyExtraData {};
	class NiPosData {};
	class NiFloatData {};
	class NiMorphData {};
	class NiAdditionalGeometryData {};
	class BSFurnitureMarker {}; //f3/f4 not compatible



	// interpolators

	class NiInterpolator : public NiObject {};
	class NiKeyBasedInterpolator : public NiInterpolator {};


	class NiBoolInterpolator : public NiKeyBasedInterpolator {

		unsigned char value = 0;
		std::unique_ptr<NiReference> dataRef;

	public:
		NiBoolInterpolator();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiBoolInterpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class NiTransformInterpolator : public NiKeyBasedInterpolator {

		Vector3 translation;
		Vector4 rotation;
		float scale = 1.0f;
		std::unique_ptr<NiReference> dataRef;

	public:
		NiTransformInterpolator();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTransformInterpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiFloatInterpolator : public NiKeyBasedInterpolator {

		float value = 0;
		std::unique_ptr<NiReference> dataRef;

	public:
		NiFloatInterpolator();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiFloatInterpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};




	class NiPoint3Interpolator : public NiKeyBasedInterpolator {

		Vector3 value;
		std::unique_ptr<NiReference> dataRef;

	public:
		NiPoint3Interpolator();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPoint3Interpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiBlendInterpolator : public NiInterpolator {

		unsigned char flags = 0;
		unsigned char arraySize = 0;
		float weightThreshold = 0.0f;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiBlendBoolInterpolator : public NiBlendInterpolator {

		unsigned char value = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiBlendBoolInterpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiBlendFloatInterpolator : public NiBlendInterpolator {

		float value = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiBlendFloatInterpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiBlendPoint3Interpolator : public NiBlendInterpolator {

		Vector3 value;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiBlendPoint3Interpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiBoolTimelineInterpolator : public NiBoolInterpolator {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiBoolTimelineInterpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};



	class NiPathInterpolator : public NiKeyBasedInterpolator {

		unsigned short flags = 0;
		uint32_t bankDDir = 0;
		float maxBankAngle = 0.0f;
		float smoothing = 0.0f;
		unsigned short followAxis = 0;
		std::unique_ptr<NiReference> pathDataRef;
		std::unique_ptr<NiReference> percentDataRef;

	public:
		NiPathInterpolator();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPathInterpolator"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};

















	// contollers

	class NiVisController : public NiInterpolatorController {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiVisController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class NiTransformController : public NiInterpolatorController {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiTransformController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};



	class NiControllerManager : public NiTimeController {

	public:
		bool cumulative = false;
		std::unique_ptr<NiReferenceArray> controllerSequenceRefs;
		std::unique_ptr<NiReference> objectPalette;

	public:
		NiControllerManager();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiControllerManager"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class NiMultiTargetTransformController : public NiTimeController {

	public:
		std::unique_ptr<NiReferenceArrayShort> extraTargetRefs;

	public:
		NiMultiTargetTransformController();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiMultiTargetTransformController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;


	};







	class NiPSysUpdateCtlr : public NiTimeController {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysUpdateCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};


	class bhkBlendController : public NiTimeController {
	public:
		uint32_t keys = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkBlendController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiFloatExtraDataController : public NiInterpolatorController {

		int extraDataNameRef = 0xFFFFFFFF;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiFloatExtraDataController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiGeomMorpherController : public NiTimeController {

		class InterplatorWeight {
		
			std::unique_ptr<NiReference> interpolatorRef;
			float weight = 0.0f;

		public:
			InterplatorWeight(NiReferenceable* referenceable) {
				interpolatorRef = std::make_unique<NiReference>(referenceable);
			}

			void read(NiStream& stream) {
				interpolatorRef->read(stream);
				stream >> weight;
			}
		
			void write(NiStream& stream) {
				interpolatorRef->write(stream);
				stream << weight;
			}

		};



	public:
		unsigned short morpherFlags = 0;
		std::unique_ptr<NiReference> dataRef;
		unsigned char alwaysUpdate = 0;
		std::vector<std::unique_ptr<InterplatorWeight>> weights;
	

	public:
		NiGeomMorpherController();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiGeomMorpherController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};




	class NiBSBoneLODController : public NiTimeController {

		class NodeGroup {

			std::unique_ptr<NiReferenceArray> nodeRefs;

		public:
			NodeGroup(NiReferenceable* referenceable) {
				nodeRefs = std::make_unique<NiReferenceArray>(referenceable);
			}

			void read(NiStream& stream) {
				nodeRefs->read(stream);
			}

			void write(NiStream& stream) {
				nodeRefs->write(stream);
			}

		};


	public:
		uint32_t lod = 0;
		uint32_t numLods = 0;
		std::vector<std::unique_ptr<NodeGroup>> nodeGroups;


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiBSBoneLODController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};




	class NiPSysModifierCtlr : public NiInterpolatorController {

		int modifierNameRef = 0xFFFFFFFF;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiPSysEmitterInitialRadiusCtlr : public NiPSysModifierCtlr {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysEmitterInitialRadiusCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};



	class NiPSysEmitterDeclinationVarCtlr : public NiPSysModifierCtlr {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysEmitterDeclinationVarCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};



	class NiPSysEmitterSpeedCtlr : public NiPSysModifierCtlr {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysEmitterSpeedCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};


	class NiPSysModifierActiveCtlr : public NiPSysModifierCtlr {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysModifierActiveCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};



	class NiLightColorController : public NiInterpolatorController {

		unsigned short targetColor = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiLightColorController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiPSysEmitterCtlr : public NiPSysModifierCtlr {

		std::unique_ptr<NiReference> visibilityInterpolatorRef;


	public:
		NiPSysEmitterCtlr();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysEmitterCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class BSPSysMultiTargetEmitterCtlr : public NiPSysEmitterCtlr {

		unsigned short maxEmitters = 0;
		std::unique_ptr<NiReference> masterPartcleSystemRef;

	public:
		BSPSysMultiTargetEmitterCtlr();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSPSysMultiTargetEmitterCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiLightDimmerController : public NiInterpolatorController {
	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiLightDimmerController"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }
	};




	class NiPSysGravityStrengthCtlr : public NiPSysModifierCtlr {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysGravityStrengthCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};



	class NiPSysEmitterLifeSpanCtlr : public NiPSysModifierCtlr {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysEmitterLifeSpanCtlr"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};










	// modifier 

	class NiPSysModifier : public NiObject {

	public:
		int nameRef = 0xFFFFFFFF;
		uint32_t order;
		std::unique_ptr<NiReference> targetRef;
		bool active = false;

	public:
		NiPSysModifier();

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class NiPSysRotationModifier : public NiPSysModifier {

	public:
		float speed = 0.0f;
		float speedVariation = 0.0f;
		float angle = 0.0f;
		float angleVariation = 0.0f;
		bool randomRotationSpeedSign = false;
		bool randomAxis = false;
		Vector3 axis;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysRotationModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class BSPSysSimpleColorModifier : public NiPSysModifier {

	public:
		float fadeInPercent = 0.0f;
		float fadeOutPercent = 0.0f;
		float color1EndPercent = 0.0f;
		float color1StartPercent = 0.0f;
		float color2EndPercent = 0.0f;
		float color2StartPercent = 0.0f;
		Vector4 color1;
		Vector4 color2;
		Vector4 color3;


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSPSysSimpleColorModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiPSysAgeDeathModifier : public NiPSysModifier {

	public:
		bool spawnOnDeath = false;
		std::unique_ptr<NiReference> spawnModifierRef;

	public:
		NiPSysAgeDeathModifier();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysAgeDeathModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiPSysSpawnModifier : public NiPSysModifier {

	public:
		unsigned short numSpawnGenerations = 0;
		float percentageSpawned = 0.0f;
		unsigned short minNumToSpawn = 0;
		unsigned short maxNumToSpawn = 0;
		float spawnSpeedVariation = 0.0f;
		float spawnDirVariation = 0.0f;
		float lifeSpan = 0.0f;
		float lifeSpanVariation = 0.0f;


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysSpawnModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiPSysGrowFadeModifier : public NiPSysModifier {

	public:
		float glowTime = 0.0f;
		unsigned short glowGeneration = 0;
		float fadeTime = 0.0f;
		unsigned short fadeGeneration = 0;
		float baseScale = 0.0f;


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysGrowFadeModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiPSysGravityModifier : public NiPSysModifier {

	public:
		std::unique_ptr<NiReference> gravityObjectRef;
		Vector3 gravirtyAxis;
		float decay = 0.0f;
		float strength = 0.0f;
		uint32_t forceType = 2;
		float turbulence = 0.0f;
		float turbulenceScale = 0.0f;
		bool worldAligned = false;


	public:
		NiPSysGravityModifier();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysGravityModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiPSysPositionModifier : public NiPSysModifier {

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysPositionModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

	};



	class NiPSysBoundUpdateModifier : public NiPSysModifier {

	public:
		unsigned short updateSkip = 0;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysBoundUpdateModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiPSysColliderManager : public NiPSysModifier {

	public:
		std::unique_ptr<NiReference> colliderRef;

	public:
		NiPSysColliderManager();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysColliderManager"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class BSParentVelocityModifier : public NiPSysModifier {

		float dampling = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSParentVelocityModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};




	class NiPSysDragModifier : public NiPSysModifier {

	public:
		std::unique_ptr<NiReference> dragObjectRef;
		Vector3 dragAxis;
		float percentage = 0.0f;
		float range = 0.0f;
		float rangeFalloff = 0.0f;

	public:
		NiPSysDragModifier();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysDragModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};




	class BSWindModifier : public NiPSysModifier {

		float strength = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSWindModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class BSPSysStripUpdateModifier : public NiPSysModifier {

		float updateDeltaTime = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSPSysStripUpdateModifier"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};




	class NiPSysBombModifier : public NiPSysModifier {

		std::unique_ptr<NiReference> bombObject;
		Vector3 bombAxis;
		float decay = 0.0f;
		float decayV = 0.0f;
		uint32_t decayType = 0;
		uint32_t symmetryType = 0;

		public:
			NiPSysBombModifier();

			static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysBombModifier"; return blockName; }
			virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

			virtual void read(NiStream& stream) override;
			virtual void write(NiStream& stream) override;

	};












	// emitter 


	class NiPSysEmitter : public NiPSysModifier {

	public:
		float speed = 0.0f;
		float speedVariation = 0.0f;
		float declination = 0.0f;
		float declinationVariation = 0.0f;
		float angle = 0.0f;
		float angleVariation = 0.0f;
		Vector4 initialColor;
		float radius = 0.0f;
		float radiusVariation = 0.0f;
		float lifeSpan = 0.0f;
		float lifeSpanVariation = 0.0f;

	public:
		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiPSysMeshEmitter : public NiPSysEmitter {

	public:
		std::unique_ptr<NiReferenceArray> emitterMeshRefs;
		uint32_t initialVelocityType = 0;
		uint32_t emissionType = 0;
		Vector3 emissionAxis;

	public:
		NiPSysMeshEmitter();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysMeshEmitter"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class NiPSysBoxEmitter : public NiPSysEmitter {

	public:
		std::unique_ptr<NiReference> emitterObjectRef;
		float width = 0.0f;
		float height = 0.0f;
		float depth = 0.0f;

	public:
		NiPSysBoxEmitter();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysBoxEmitter"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class NiPSysCylinderEmitter : public NiPSysEmitter {

	public:
		std::unique_ptr<NiReference> emitterObjectRef;
		float radius = 0.0f;
		float height = 0.0f;

	public:
		NiPSysCylinderEmitter();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysCylinderEmitter"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};




	class NiPSysSphereEmitter : public NiPSysEmitter {

	public:
		std::unique_ptr<NiReference> emitterObjectRef;
		float radius = 0.0f;

	public:
		NiPSysSphereEmitter();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysSphereEmitter"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};























	//other


	class NiDefaultAVObjectPalette : public NiObject {


		class Object {
			NiString<unsigned int> name;
			std::unique_ptr<NiReference> aVObjectRef;

		public:
			Object(NiReferenceable* referenceable) {
				aVObjectRef = std::make_unique<NiReference>(referenceable);
			}

			void read(NiStream& stream) {
				name.read(stream);
				aVObjectRef->read(stream);
			}

			void write(NiStream& stream) {
				name.write(stream);
				aVObjectRef->write(stream);
			}

		};


	public:
		std::unique_ptr<NiReference> sceneRef;
		std::vector<std::unique_ptr<Object>> objectRefs;

	public:
		NiDefaultAVObjectPalette();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiDefaultAVObjectPalette"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;


	};



	class BSBound : public NiExtraData {
	private:
		Vector3 center;
		Vector3 dimensions;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSBound"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiFloatExtraData : public NiExtraData {
	private:
		float data = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiFloatExtraData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class BSWArray : public NiExtraData {
	private:
		std::vector<uint32_t> items;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSWArray"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class BSDecalPlacementVectorExtraData : public NiFloatExtraData {

		class VectorBlock {

			std::vector<Vector3> points;
			std::vector<Vector3> normals;

		public:
			void read(NiStream& stream) {

				unsigned short sz = 0;
				stream >> sz;

				points.resize(sz);
				for (uint32_t i = 0; i < sz; i++) {
					stream >> points[i];
				}

				normals.resize(sz);
				for (uint32_t i = 0; i < sz; i++) {
					stream >> normals[i];
				}

			}

			void write(NiStream& stream) {

				unsigned short sz = points.size();
				stream << sz;

				for (uint32_t i = 0; i < sz; i++) {
					stream << points[i];
				}

				for (uint32_t i = 0; i < sz; i++) {
					stream << normals[i];
				}

			}
		};

	private:
		std::vector<VectorBlock> blocks;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSDecalPlacementVectorExtraData"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class bhkBlendCollisionObject : public bhkCollisionObject {

	public:
		float heirGain = 0.0f;
		float velGain = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkBlendCollisionObject"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class NiCamera : public NiAvObject {

	public:
		unsigned short cameraFlags = 0;
		float frustrumLeft = 0.0f;
		float frustrumRight = 0.0f;
		float frustrumTop = 0.0f;
		float frustrumBottom = 0.0f;
		float frustrumNear = 0.0f;
		float frustrumFar = 0.0f;
		bool useOrthoProjection = false;
		float viewPortLeft = 0.0f;
		float viewPortRight = 0.0f;
		float viewPortTop = 0.0f;
		float viewPortBottom = 0.0f;
		float lodAdjust = 0.0f;
		std::unique_ptr<NiReference> sceneRef;
		uint32_t numScreenPolygons = 0;
		uint32_t numScreenTextures = 0;


	public:
		NiCamera();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiCamera"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class BSMultiBound : public NiObject {

	public:
		std::unique_ptr<NiReference> dataRef;

	public:
		BSMultiBound();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSMultiBound"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;


	};



	class BSMultiBoundSphere : public NiObject {

		Vector3 center;
		float radius = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSMultiBoundSphere"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;


	};




	class NiPSysCollider : public NiObject {

		float bounce = 0.0f;
		bool spawnOnCollide = false;
		bool dieOnCollide = false;
		std::unique_ptr<NiReference> spawnModifierRef;
		std::unique_ptr<NiReference> parentRef;
		std::unique_ptr<NiReference> nextColliderRef;
		std::unique_ptr<NiReference> colliderObjectRef;

	public:
		NiPSysCollider();

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};

	class NiPSysPlanarCollider : public NiPSysCollider {

		float width = 0.0f;
		float hight = 0.0f;
		Vector3 xAxis;
		Vector3 yAxis;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiPSysPlanarCollider"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class BSMultiBoundData : public NiObject {};

	class BSMultiBoundAABB : public BSMultiBoundData {

	public:
		Vector3 position;
		Vector3 extent;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "BSMultiBoundAABB"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class bhkLiquidAction : public NiObject {

		std::unique_ptr<NiReference> rigidBodyRef; //?
		unsigned char bhkLiquidAction_unknown1[8] = { 0 };
		float initiakStickForce = 0.0f;
		float stickStrength = 0.0f;
		float neighborDistance = 0.0f;
		float neighborStrength = 0.0f;

	public:
		bhkLiquidAction();
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkLiquidAction"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};




	class bhkOrientHingedBodyAction : public NiObject {

		std::unique_ptr<NiReference> entity;
		unsigned char bhkOrientHingedBodyAction_unknown1[8] = { 0 };
		unsigned char bhkOrientHingedBodyAction_unknown2[8] = { 0 };
		Vector4 hingeAxisLs;
		Vector4 forwardLs;
		float strength = 0.0f;
		float dampling = 0.0f;
		unsigned char bhkOrientHingedBodyAction_unknown3[8] = { 0 };




	public:
		bhkOrientHingedBodyAction();

		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkOrientHingedBodyAction"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};



	class NiFogProperty : public NiProperty {

		unsigned short flags = 0;
		float depth = 0.0f;
		Vector3 color;


	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "NiFogProperty"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


























	// constrain 




	enum MotorType : unsigned char {
		MOTOR_NONE,
		MOTOR_POSITION,
		MOTOR_VELOCITY,
		MOTOR_SPRING
	};



	class MotorDetails {

	public:

		float minForce = 0.0f;
		float maxForce = 0.0f;
		bool enabled = false;

	protected:
		virtual void _read(NiStream& stream) = 0;
		virtual void _write(NiStream& stream) = 0;

	public:
		virtual void read(NiStream& stream) final  {
			stream >> minForce;
			stream >> maxForce;
			_read(stream);
			stream >> enabled;
		}
	
		virtual void write(NiStream& stream) final {
			stream << minForce;
			stream << maxForce;
			_write(stream);
			stream << enabled;
		}
	};

	class PositionMotorDetails : public MotorDetails {

	public:
		float tau = 0.0f;
		float damping = 0.0f;
		float proportionalRecoveryVelocity = 0.0f;
		float constantRecoveryVelocity = 0.0f;

	protected:
		virtual void _read(NiStream& stream) override {
			stream >> tau;
			stream >> damping;
			stream >> proportionalRecoveryVelocity;
			stream >> constantRecoveryVelocity;
		}
	
		virtual void _write(NiStream& stream) override {
			stream << tau;
			stream << damping;
			stream << proportionalRecoveryVelocity;
			stream << constantRecoveryVelocity;
		}

	};


	class VelocityMotorDetails : public MotorDetails {

	public:
		float tau = 0.0f;
		float targetVelocity = 0.0f;
		bool useVelocityTarget = false;

	protected:
		virtual void _read(NiStream& stream) override {
			stream >> tau;
			stream >> targetVelocity;
			stream >> useVelocityTarget;
		}

		virtual void _write(NiStream& stream) override {
			stream << tau;
			stream << targetVelocity;
			stream << useVelocityTarget;
		}

	};


	class SpringDamperMotorDetails : public MotorDetails {

	public:
		float springConstant = 0.0f;
		float springDamping = 0.0f;

	protected:
		virtual void _read(NiStream& stream) override {
			stream >> springConstant;
			stream >> springDamping;
		}

		virtual void _write(NiStream& stream) override {
			stream << springConstant;
			stream << springDamping;
		}

	};



	class Motor {

		MotorType motortype = MotorType::MOTOR_NONE;
		std::unique_ptr<MotorDetails> motorDetails;

	public:

		void read(NiStream& stream)  {
		
			stream >> motortype;

			switch (motortype) {
			case MotorType::MOTOR_POSITION:
				motorDetails = std::make_unique<PositionMotorDetails>();
				break;

			case MotorType::MOTOR_VELOCITY:
				motorDetails = std::make_unique<VelocityMotorDetails>();
				break;
		
			case MotorType::MOTOR_SPRING:
				motorDetails = std::make_unique<SpringDamperMotorDetails>();
				break;
			}

			if (motorDetails) {
				motorDetails->read(stream);
			}

		}

		void write(NiStream& stream)  {
		
			stream << motortype;

			if (motorDetails) {
				motorDetails->write(stream);
			}
		
		}

	};









	enum hkConstraintType : uint32_t {
		BallAndSocket = 0,
		Hinge = 1,
		LimitedHinge = 2,
		Prismatic = 6,
		Ragdoll = 7,
		StiffSpring = 8,
		Malleable = 13
	};



	class bhkConstraint : public NiObject {

	public:
		std::unique_ptr<NiReferenceArray> enitityRefs;
		uint32_t priority = 0;


	public:
		bhkConstraint();

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;
	};


	class bhkRagdollConstraint : public bhkConstraint {

		Vector4 twistA;
		Vector4 planeA;
		Vector4 motorA;
		Vector4 pivotA;
		Vector4 twistB;
		Vector4 planeB;
		Vector4 motorB;
		Vector4 pivotB;
		float coneMaxAngle = 0.0f;
		float planeMinAngle = 0.0f;
		float planeMaxAngle = 0.0f;
		float twistMinAngle = 0.0f;
		float twistMaxAngle = 0.0f;
		float maxFriction = 0.0f;
		Motor motor;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkRagdollConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class bhkPrismaticConstraint : public bhkConstraint {

		Vector4 slidingA;
		Vector4 rotationA;
		Vector4 planeA;
		Vector4 pivotA;
		Vector4 slidingB;
		Vector4 rotationB;
		Vector4 planeB;
		Vector4 pivotB;
		float minDistance = 0.0f;
		float maxDistance = 0.0f;
		float friction = 0.0f;
		Motor motor;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkPrismaticConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class bhkHingeConstraint : public bhkConstraint {

		Vector4 axisA;
		Vector4 axisInA1;
		Vector4 axisInA2;
		Vector4 pivotA;
		Vector4 axisB;
		Vector4 axisInB1;
		Vector4 axisInB2;
		Vector4 pivotB;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkHingeConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class bhkLimitedHingeConstraint : public bhkHingeConstraint {

		float minAngle = 0.0f;
		float maxAngle = 0.0f;
		float maxFriction = 0.0f;
		Motor motor;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkLimitedHingeConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class bhkBallAndSocketConstraint : public bhkConstraint {

		Vector4 pivotA;
		Vector4 pivotB;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkBallAndSocketConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


	class bhkStiffSpringConstraint : public bhkBallAndSocketConstraint {

		float length = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkStiffSpringConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};





	class bhkMalleableConstraint : public bhkConstraint {

		hkConstraintType type;
		std::unique_ptr<bhkConstraint> constraint;
		float strength = 0.0f;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkMalleableConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};



	class bhkBreakableConstraint : public bhkMalleableConstraint {

		bool removeWhenBroken = false;

	public:
		static const std::string& GET_BLOCK_NAME() { const static std::string blockName = "bhkBreakableConstraint"; return blockName; }
		virtual const std::string& getBlockName() const override { return GET_BLOCK_NAME(); }

		virtual void read(NiStream& stream) override;
		virtual void write(NiStream& stream) override;

	};


}