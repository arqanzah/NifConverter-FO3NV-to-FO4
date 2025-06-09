#include "objects.h"

namespace nif {

	NiObjectNet::NiObjectNet() {
		extraDataList = std::make_unique<NiReferenceArray>(this);
		controllerRef = std::make_unique<NiReference>(this);
	}

	void NiObjectNet::read(NiStream& stream) {

		stream >> nameRef;

		extraDataList->read(stream);

		controllerRef->read(stream);

	}

	void NiObjectNet::write(NiStream& stream) {

		stream << nameRef;

		extraDataList->write(stream);

		controllerRef->write(stream);

	}


	NiAvObject::NiAvObject() {
		properties = std::make_unique<NiReferenceArray>(this);
		collisionRef = std::make_unique<NiReference>(this);
	}

	void NiAvObject::read(NiStream& stream) {

		NiObjectNet::read(stream);

		stream >> flags;

		stream >> translation;

		stream >> rotation;

		stream >> scale;

		properties->read(stream);

		collisionRef->read(stream);

	}

	void NiAvObject::write(NiStream& stream) {

		NiObjectNet::write(stream);

		stream << flags;

		stream << translation;

		stream << rotation;

		stream << scale;

		//properties->write(stream);

		collisionRef->write(stream);

	}
















	NiNode::NiNode() {
		children = std::make_unique<NiReferenceArray>(this);
		effects = std::make_unique<NiReferenceArray>(this);
	}

	void NiNode::read(NiStream& stream) {

		NiAvObject::read(stream);

		children->read(stream);

		effects->read(stream);

	}

	void NiNode::write(NiStream& stream) {

		NiAvObject::write(stream);

		children->write(stream);

		//effects->write(stream);

	}


	BSMultiBoundNode::BSMultiBoundNode() {
		multiBoundRef = std::make_unique<NiReference>(this);
	}

	void BSMultiBoundNode::read(NiStream& stream) {

		NiNode::read(stream);
		multiBoundRef->read(stream);
	}

	void BSMultiBoundNode::write(NiStream& stream) {

		NiNode::write(stream);
		multiBoundRef->write(stream);
		stream << cullingMode;
	}

	void NiBillboardNode::read(NiStream& stream) {

		NiNode::read(stream);
		stream >> billboardMode;
	}

	void NiBillboardNode::write(NiStream& stream) {

		NiNode::write(stream);
		stream << billboardMode;
	}



	void BSOrderedNode::read(NiStream& stream) {

		NiNode::read(stream);
		stream >> alphaSortBound;
		stream >> staticBound;
	}

	void BSOrderedNode::write(NiStream& stream) {

		NiNode::write(stream);
		stream << alphaSortBound;
		stream << staticBound;
	}



	void BSValueNode::read(NiStream& stream) {

		NiNode::read(stream);
		stream >> value;
		stream >> valueNodeFlags;
	}

	void BSValueNode::write(NiStream& stream) {

		NiNode::write(stream);
		stream << value;
		stream << valueNodeFlags;
	}

	void BSRangeNode::read(NiStream& stream) {

		NiNode::read(stream);
		stream >> min;
		stream >> max;
		stream >> current;
	}

	void BSRangeNode::write(NiStream& stream) {

		NiNode::write(stream);
		stream << min;
		stream << max;
		stream << current;
	}


















	void NiExtraData::read(NiStream& stream) {
		stream >> nameRef;
	}

	void NiExtraData::write(NiStream& stream) {
		stream << nameRef;
	}

	void NiIntegerExtraData::read(NiStream& stream) {
		NiExtraData::read(stream);
		stream >> integerData;
	}

	void NiIntegerExtraData::write(NiStream& stream) {
		NiExtraData::write(stream);
		stream << integerData;
	}

	void NiStringExtraData::read(NiStream& stream) {
		NiExtraData::read(stream);
		stream >> stringData;
	}

	void NiStringExtraData::write(NiStream& stream) {
		NiExtraData::write(stream);
		stream << stringData;
	}











	bhkNiCollisionObject::bhkNiCollisionObject() {
		targetRef = std::make_unique<NiReference>(this);
		bodyRef = std::make_unique<NiReference>(this);
	}

	void bhkNiCollisionObject::read(NiStream& stream) {
		targetRef->read(stream);
		stream >> flags;
		bodyRef->read(stream);
	}

	void bhkNiCollisionObject::write(NiStream& stream) {
		targetRef->write(stream);
		stream << flags;
		bodyRef->write(stream);
	}


	bhkWorldObject::bhkWorldObject() {
		shapeRef = std::make_unique<NiReference>(this);
	}

	void bhkWorldObject::read(NiStream& stream) {
		shapeRef->read(stream);
		stream >> filter;
		stream >> bhkWorldObject_unknown1;
		stream >> broadPhaseType;
		stream >> bhkWorldObject_unknown2;
		stream >> cInfoProperty;
	}

	void bhkWorldObject::write(NiStream& stream) {
		shapeRef->write(stream);
		stream << filter;
		stream << bhkWorldObject_unknown1;
		stream << broadPhaseType;
		stream << bhkWorldObject_unknown2;
		stream << cInfoProperty;
	}


	bhkRigidBody::bhkRigidBody() {
		constraints = std::make_unique<NiReferenceArray>(this);
	}

	void bhkRigidBody::read(NiStream& stream) {
		bhkWorldObject::read(stream);

		stream >> collisionResponse;
		stream >> bhkRigidBody_unknown1;
		stream >> processContactCallbackDelay;
		stream >> bhkRigidBody_unknown2;
		stream >> filterCopy;
		stream >> bhkRigidBody_unknown3;
		stream >> collisionResponse2;
		stream >> bhkRigidBody_unknown4;
		stream >> processContactCallbackDelay2;
		stream >> bhkRigidBody_unknown5;
		stream >> translation;
		stream >> rotation;
		stream >> linearVelocity;
		stream >> angularVelocity;
		stream >> inertiaTensor;
		stream >> center;
		stream >> mass;
		stream >> linearDamping;
		stream >> angularDamping;
		stream >> friction;
		stream >> restitution;
		stream >> maxLinearVelocity;
		stream >> maxAngularVelocity;
		stream >> penetrationDepth;
		stream >> motionSystem;
		stream >> deactivatorType;
		stream >> solverDeactivation;
		stream >> qualityType;
		stream >> bhkRigidBody_unknown6;
		//stream >> numConstraints;
		//constraints.resize(numConstraints);
		//for (unsigned int i = 0; i < numConstraints; i++) {
		//	stream >> constraints[i];
		//}
		constraints->read(stream);
		stream >> bodyFlags;
	}

	void bhkRigidBody::write(NiStream& stream) {
		bhkWorldObject::write(stream);

		stream << collisionResponse;
		stream << bhkRigidBody_unknown1;
		stream << processContactCallbackDelay;
		stream << bhkRigidBody_unknown2;
		stream << filterCopy;
		stream << bhkRigidBody_unknown3;
		stream << collisionResponse2;
		stream << bhkRigidBody_unknown4;
		stream << processContactCallbackDelay2;
		stream << bhkRigidBody_unknown5;
		stream << translation;
		stream << rotation;
		stream << linearVelocity;
		stream << angularVelocity;
		stream << inertiaTensor;
		stream << center;
		stream << mass;
		stream << linearDamping;
		stream << angularDamping;

		stream << timeFactor; // f4

		stream << friction;

		stream << rollingFrictionMult;

		stream << restitution;
		stream << maxLinearVelocity;
		stream << maxAngularVelocity;
		//stream << penetrationDepth;
		stream << motionSystem;

		stream << enableDeactivation;

		//stream << deactivatorType;
		stream << solverDeactivation;
		stream << qualityType;

		stream << penetrationDepth; //moved

		stream << gravityFactor;

		stream << bhkRigidBody_unknown6;

		stream << bhkRigidBody_unknown7;

		//stream << numConstraints;
		//for (unsigned int i = 0; i < numConstraints; i++) {
		//	stream << constraints[i];
		//}
		constraints->write(stream);

		//stream << bodyFlags; not in f4

		stream << bhkRigidBody_unknown8;

	}




	void bhkSimpleShapePhantom::read(NiStream& stream) {

		bhkPhantom::read(stream);

		stream >> bhkSimpleShapePhantom_unknown1;
		stream >> transform;
	}

	void bhkSimpleShapePhantom::write(NiStream& stream) {

		bhkPhantom::write(stream);

		stream << bhkSimpleShapePhantom_unknown1;
		stream << transform;
	}





	bhkMoppBvTreeShape::bhkMoppBvTreeShape() {
		shapeRef = std::make_unique<NiReference>(this);
	}

	void bhkMoppBvTreeShape::read(NiStream& stream) {

		shapeRef->read(stream);
		stream >> bhkMoppBvTreeShape_unknown1;
		stream >> shapeScale;
		stream >> moppDataSize;
		stream >> origin;
		stream >> scale;

		moppData.resize(moppDataSize);
		for (unsigned int i = 0; i < moppDataSize; i++) {
			stream >> moppData[i];
		}

	}

	void bhkMoppBvTreeShape::write(NiStream& stream) {

		shapeRef->write(stream);
		stream << bhkMoppBvTreeShape_unknown1;
		stream << shapeScale;
		stream << moppDataSize;
		stream << origin;
		stream << scale;

		stream << buildType; //fo4

		for (unsigned int i = 0; i < moppDataSize; i++) {
			stream << moppData[i];
		}

	}



	void bhkSphereRepShape::read(NiStream& stream) {

		stream >> material;
		stream >> radius;
	}

	void bhkSphereRepShape::write(NiStream& stream) {

		stream << material;
		stream << radius;
	}


	void bhkBoxShape::read(NiStream& stream) {

		bhkConvexShape::read(stream);

		stream >> bhkBoxShape_unknown1;
		stream >> dimensions;
		stream >> bhkBoxShape_unknown2;
	}

	void bhkBoxShape::write(NiStream& stream) {

		bhkConvexShape::write(stream);

		stream << bhkBoxShape_unknown1;
		stream << dimensions;
		stream << bhkBoxShape_unknown2;
	}


	void bhkCapsuleShape::read(NiStream& stream) {

		bhkConvexShape::read(stream);

		stream >> bhkCapsuleShape_unknown1;
		stream >> point1;
		stream >> radius1;
		stream >> point2;
		stream >> radius2;
	}

	void bhkCapsuleShape::write(NiStream& stream) {

		bhkConvexShape::write(stream);

		stream << bhkCapsuleShape_unknown1;
		stream << point1;
		stream << radius1;
		stream << point2;
		stream << radius2;
	}



	bhkTransformShape::bhkTransformShape() {
		shapeRef = std::make_unique<NiReference>(this);
	}

	void bhkTransformShape::read(NiStream& stream) {

		shapeRef->read(stream);

		bhkConvexShape::read(stream);

		stream >> bhkTransformShape_unknown1;

		stream >> transform;

	}

	void bhkTransformShape::write(NiStream& stream) {

		shapeRef->write(stream);

		bhkConvexShape::write(stream);

		stream << bhkTransformShape_unknown1;

		stream << transform;

	}


	bhkListShape::bhkListShape() {
		subShapeRefs = std::make_unique<NiReferenceArray>(this);
	}

	void bhkListShape::read(NiStream& stream) {

		subShapeRefs->read(stream);

		stream >> material;

		stream >> childShapeProperty;
		stream >> childFilterProperty;

		stream >> num_bhkListShape_unknown1;

		bhkListShape_unknown1.resize(num_bhkListShape_unknown1);
		for (unsigned int i = 0; i < num_bhkListShape_unknown1; i++) {
			stream >> bhkListShape_unknown1[i];
		}



	}

	void bhkListShape::write(NiStream& stream) {

		subShapeRefs->write(stream);

		stream << material;

		stream << childShapeProperty;
		stream << childFilterProperty;

		stream << num_bhkListShape_unknown1;

		for (unsigned int i = 0; i < num_bhkListShape_unknown1; i++) {
			stream << bhkListShape_unknown1[i];
		}

	}


	bhkConvexListShape::bhkConvexListShape() {
		shapeRefs = std::make_unique<NiReferenceArray>(this);
	}
	void bhkConvexListShape::read(NiStream& stream) {

		shapeRefs->read(stream);

		stream >> material;
		stream >> radius;
		stream >> bhkConvexListShape_unknown1;
		stream >> bhkConvexListShape_unknown2;
		stream >> childShapeProp;
		stream >> bhkConvexListShape_unknown3;
		stream >> bhkConvexListShape_unknown4;
	}

	void bhkConvexListShape::write(NiStream& stream) {

		shapeRefs->write(stream);

		stream << material;
		stream << radius;
		stream << bhkConvexListShape_unknown1;
		stream << bhkConvexListShape_unknown2;
		stream << childShapeProp;
		stream << bhkConvexListShape_unknown3;
		stream << bhkConvexListShape_unknown4;
	}

	void bhkConvexVerticesShape::read(NiStream& stream) {

		bhkConvexShape::read(stream);

		stream >> verticesProperty;
		stream >> normalsProperty;

		stream >> numVerts;
		vertices.resize(numVerts);
		for (unsigned int i = 0; i < numVerts; i++) {
			stream >> vertices[i];
		}

		stream >> numNormals;
		normals.resize(numNormals);
		for (unsigned int i = 0; i < numNormals; i++) {
			stream >> normals[i];
		}

	}

	void bhkConvexVerticesShape::write(NiStream& stream) {

		bhkConvexShape::write(stream);

		stream << verticesProperty;
		stream << normalsProperty;

		stream << numVerts;
		for (unsigned int i = 0; i < numVerts; i++) {
			stream << vertices[i];
		}


		stream << numNormals;
		for (unsigned int i = 0; i < numNormals; i++) {
			stream << normals[i];
		}

	}


	NiGeometry::NiGeometry() {
		skinRef = std::make_unique<NiReference>(this);
	}

	NiGeometry3::NiGeometry3() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void NiGeometry3::read(NiStream& stream) {

		NiAvObject::read(stream);

		dataRef->read(stream);

		skinRef->read(stream);
		stream >> numMaterials;
		materialNameRefs.resize(numMaterials);
		materialExtraData.resize(numMaterials);
		for (unsigned int i = 0; i < numMaterials; i++) {
			stream >> materialNameRefs[i];
			stream >> materialExtraData[i];
		}
		stream >> activeMaterial;
		stream >> materialNeedsUpdate;
	}

	void NiGeometry3::write(NiStream& stream) {
		/*
		NiAvObject::write(stream);

		dataRef->write(stream);
		skinRef->write(stream);
		stream << numMaterials;

		for (unsigned int i = 0; i < numMaterials; i++) {
		stream << materialNameRefs[i];
		stream << materialExtraData[i];
		}
		stream << activeMaterial;
		stream << materialNeedsUpdate;

		stream << shaderPropertyRef;
		stream << alphaPropertyRef;
		*/
	}



	void BSSegmentedTriShape::read(NiStream& stream) {

		NiTriShape::read(stream);

		stream >> numSegments;

		segments.resize(numSegments);
		for (auto& segment : segments) {
			stream >> segment.flags;
			stream >> segment.index;
			stream >> segment.numTrisInSegnent;
		}


	}

	// #not-tested!!!
	void BSSegmentedTriShape::write(NiStream& stream) {
		/*
		NiTriShape::write(stream);

		stream << numSegments;

		for (auto& segment : segments) {
		stream << segment.flags;
		stream << segment.index;
		stream << segment.numTrisInSegnent;
		}

		*/
	}




	void NiAlphaProperty::read(NiStream& stream) {

		NiProperty::read(stream);

		stream >> flags;
		stream >> threshold;

	}

	void NiAlphaProperty::write(NiStream& stream) {

		NiProperty::write(stream);

		stream << flags;
		stream << threshold;

	}


	void NiStencilProperty::read(NiStream& stream) {

		NiProperty::read(stream);

		stream >> flags;
		stream >> stenscilRef;
		stream >> stenscilMask;
	}

	//#not-tested!!!
	void NiStencilProperty::write(NiStream& stream) {

		NiProperty::write(stream);

		stream << flags;
		stream << stenscilRef;
		stream << stenscilMask;

	}


	void NiMaterialProperty::read(NiStream& stream) {

		NiProperty::read(stream);

		stream >> specularColor;
		stream >> emissiveColor;
		stream >> glossiness;
		stream >> alpha;
		stream >> emissiveMult;


	}

	void NiMaterialProperty::write(NiStream& stream) {

		NiProperty::write(stream);

		stream << specularColor;
		stream << emissiveColor;
		stream << glossiness;
		stream << alpha;
		stream << emissiveMult;
	}


	void BSShaderTextureSet::read(NiStream& stream) {

		stream >> numTextures;
		textures.resize(numTextures);

		for (unsigned int i = 0; i < numTextures; i++) {
			//stream >> textures[i];
			textures[i].read(stream);
		}

	}

	void BSShaderTextureSet::write(NiStream& stream) {

		stream << numTextures;

		for (unsigned int i = 0; i < numTextures; i++) {
			//stream << textures[i];
			textures[i].write(stream);
		}

	}


	NiSourceTexture::NiSourceTexture() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void NiSourceTexture::read(NiStream& stream) {

		NiObjectNet::read(stream);

		stream >> useExternal;
		stream >> fileName;
		dataRef->read(stream);
		stream >> pixelLayout;
		stream >> mipMapFormat;
		stream >> alphaFormat;
		stream >> isStatic;
		stream >> directRender;
		stream >> persistentRenderData;

	}
	//#not-tested!!!
	void NiSourceTexture::write(NiStream& stream) {
		/*
		NiObjectNet::write(stream);

		stream << useExternal;
		stream << fileName;
		dataRef->write(stream);
		stream << pixelLayout;
		stream << mipMapFormat;
		stream << alphaFormat;
		stream << isStatic;
		stream << directRender;
		stream << persistentRenderData;
		*/
	}


	NiTexturingProperty::NiTexturingProperty() {
		baseTex = std::make_unique<TextureDescription>(this);
		darkTex = std::make_unique<TextureDescription>(this);
		detailTex = std::make_unique<TextureDescription>(this);
		glossTex = std::make_unique<TextureDescription>(this);
		glowTex = std::make_unique<TextureDescription>(this);
		bumpTex = std::make_unique<TextureDescription>(this);
		normalTex = std::make_unique<TextureDescription>(this);
		parallaxTex = std::make_unique<TextureDescription>(this);
		decalTex0 = std::make_unique<TextureDescription>(this);
		decalTex1 = std::make_unique<TextureDescription>(this);
		decalTex2 = std::make_unique<TextureDescription>(this);
		decalTex3 = std::make_unique<TextureDescription>(this);
	}

	void NiTexturingProperty::read(NiStream& stream) {

		NiProperty::read(stream);

		stream >> flags;
		stream >> textureCount;

		if (textureCount > 0) {
			stream >> hasBaseTex;
			if (hasBaseTex) {
				baseTex->read(stream);
			}
		}

		if (textureCount > 1) {
			stream >> hasDarkTex;
			if (hasDarkTex) {
				darkTex->read(stream);
			}
		}

		if (textureCount > 2) {
			stream >> hasDetailTex;
			if (hasDetailTex) {
				detailTex->read(stream);
			}
		}

		if (textureCount > 3) {
			stream >> hasGlossTex;
			if (hasGlossTex) {
				glossTex->read(stream);
			}
		}

		if (textureCount > 4) {
			stream >> hasGlowTex;
			if (hasGlowTex) {
				glowTex->read(stream);
			}
		}

		if (textureCount > 5) {
			stream >> hasBumpTex;
			if (hasBumpTex) {
				bumpTex->read(stream);
				stream >> lumaScale;
				stream >> lumaOffset;
				stream >> bumpMatrix;
			}
		}

		if (textureCount > 6) {
			stream >> hasNormalTex;
			if (hasNormalTex) {
				normalTex->read(stream);
			}
		}

		if (textureCount > 7) {
			stream >> hasParallaxTex;
			if (hasParallaxTex) {
				parallaxTex->read(stream);
				stream >> parallaxFloat;
			}
		}

		if (textureCount > 8) {
			stream >> hasDecalTex0;
			if (hasDecalTex0) {
				decalTex0->read(stream);
			}
		}

		if (textureCount > 9) {
			stream >> hasDecalTex1;
			if (hasDecalTex1) {
				decalTex1->read(stream);
			}
		}

		if (textureCount > 10) {
			stream >> hasDecalTex2;
			if (hasDecalTex2) {
				decalTex2->read(stream);
			}
		}

		if (textureCount > 11) {
			stream >> hasDecalTex3;
			if (hasDecalTex3) {
				decalTex3->read(stream);
			}
		}

		stream >> numShaderTex;
		for (unsigned int i = 0; i < numShaderTex; i++) {

			std::unique_ptr<ShaderTexDesc> sText = std::make_unique<ShaderTexDesc>(this);
			sText->read(stream);
			shaderTex.push_back(std::move(sText));

		}

	}

	void NiTexturingProperty::write(NiStream& stream) {}








	void BSShaderProperty::read(NiStream& stream) {

		NiShaderProperty::read(stream);

		stream >> flags;
		stream >> shaderType;
		stream >> shaderFlags1;
		stream >> shaderFlags2;
		stream >> environmentMapScale;
	}

	void BSShaderProperty::write(NiStream& stream) {

		NiShaderProperty::write(stream);

		stream << shaderFlags1;
		stream << shaderFlags2;
		stream << uvOffset;
		stream << uvScale;

	}


	void BSShaderLightingProperty::read(NiStream& stream) {

		BSShaderProperty::read(stream);

		stream >> textureClampMode;
	}

	void BSShaderLightingProperty::write(NiStream& stream) {

		BSShaderProperty::write(stream);

		stream << textureClampMode;
	}



	BSShaderPPLightingProperty::BSShaderPPLightingProperty() {
		textureSetRef = std::make_unique<NiReference>(this);
	}

	void BSShaderPPLightingProperty::read(NiStream& stream) {

		BSShaderLightingProperty::read(stream);

		textureSetRef->read(stream);
		stream >> refractionStrength;
		stream >> refractionFirePeriod;
		stream >> parallaxMaxPasses;
		stream >> parallaxScale;
	}

	void BSShaderPPLightingProperty::write(NiStream& stream) {

		BSShaderLightingProperty::write(stream);

		textureSetRef->write(stream);
		stream << refractionStrength;
		stream << refractionFirePeriod;
		stream << parallaxMaxPasses;
		stream << parallaxScale;
	}


	void BSShaderNoLightingProperty::read(NiStream& stream) {

		BSShaderLightingProperty::read(stream);

		baseTexture.read(stream);

		stream >> falloffStartAngle;
		stream >> falloffStopAngle;
		stream >> falloffStartOpacity;
		stream >> falloffStopOpacity;

	}

	void BSShaderNoLightingProperty::write(NiStream& stream) {

		BSShaderLightingProperty::write(stream);

		baseTexture.write(stream);

		stream << falloffStartAngle;
		stream << falloffStopAngle;
		stream << falloffStartOpacity;
		stream << falloffStopOpacity;

	}


	void TallGrassShaderProperty::read(NiStream& stream) {

		BSShaderProperty::read(stream);

		fileName.read(stream);
	}

	//#not-tested!!!
	void TallGrassShaderProperty::write(NiStream& stream) {

		BSShaderProperty::write(stream);

		fileName.write(stream);
	}


	void SkyShaderProperty::read(NiStream& stream) {

		BSShaderLightingProperty::read(stream);

		fileName.read(stream);
		stream >> skyObjectType;

	}

	//#not-tested!!!
	void SkyShaderProperty::write(NiStream& stream) {

		BSShaderLightingProperty::write(stream);

		fileName.write(stream);
		stream << skyObjectType;
	}


	void TileShaderProperty::read(NiStream& stream) {

		BSShaderLightingProperty::read(stream);

		fileName.read(stream);

	}

	//#not-tested!!!
	void TileShaderProperty::write(NiStream& stream) {

		BSShaderLightingProperty::write(stream);

		fileName.write(stream);
	}



	NiGeometryData::NiGeometryData() {
		additionalDataRef = std::make_unique<NiReference>(this);
	}

	void NiGeometryData::read(NiStream& stream) {

		stream >> groupID;
		stream >> numVertices;
		stream >> keepFlags;
		stream >> compressFlags;
		stream >> hasVertices;

		if (hasVertices) {
			vertices.resize(numVertices);
			for (unsigned int i = 0; i < numVertices; i++) {
				stream >> vertices[i];
			}
		}

		stream >> bsVectorFlags;

		stream >> hasNormals;

		if (hasNormals) {

			normals.resize(numVertices);
			for (unsigned int i = 0; i < numVertices; i++) {
				stream >> normals[i];
			}

			unsigned short hasTangents = bsVectorFlags & 0xF000; //if last 4 bits (F) has any value, means has tangents

			if (hasTangents) {

				tangents.resize(numVertices);
				for (unsigned int i = 0; i < numVertices; i++) {
					stream >> tangents[i];
				}

				bitangents.resize(numVertices);
				for (unsigned int i = 0; i < numVertices; i++) {
					stream >> bitangents[i];
				}
			}

		}

		stream >> center;
		stream >> radius;

		stream >> hasVertexColors;
		if (hasVertexColors) {
			vertexColors.resize(numVertices);
			for (unsigned int i = 0; i < numVertices; i++) {
				stream >> vertexColors[i];
			}
		}

		unsigned char numUvSets = bsVectorFlags & 0x1;


		if (numUvSets > 0) {
			uvSets.resize(numUvSets);
			for (unsigned int i = 0; i < numUvSets; i++) {
				uvSets[i].resize(numVertices);
				for (unsigned int j = 0; j < numVertices; j++) {
					stream >> uvSets[i][j];
				}
			}
		}

		stream >> consistencyFlags;
		additionalDataRef->read(stream);

	}

	void NiGeometryData::write(NiStream& stream) {


		stream << groupID;
		stream << numVertices;
		stream << keepFlags;
		stream << compressFlags;
		stream << hasVertices;

		if (hasVertices) {
			for (unsigned int i = 0; i < numVertices; i++) {
				stream << vertices[i];
			}
		}

		stream << bsVectorFlags;

		stream << materialCRC;

		stream << hasNormals;

		if (hasNormals) {

			for (unsigned int i = 0; i < numVertices; i++) {
				stream << normals[i];
			}

			unsigned short hasTangents = bsVectorFlags & 0xF000; //if last 4 bits (F) has any value, means has tangents

			if (hasTangents) {

				for (unsigned int i = 0; i < numVertices; i++) {
					stream << tangents[i];
				}

				for (unsigned int i = 0; i < numVertices; i++) {
					stream << bitangents[i];
				}
			}

		}

		stream << center;
		stream << radius;

		stream << hasVertexColors;
		if (hasVertexColors) {

			for (unsigned int i = 0; i < numVertices; i++) {
				stream << vertexColors[i];
			}
		}

		unsigned char numTextureSets = bsVectorFlags & 0x1;


		if (numTextureSets > 0) {
			for (unsigned int i = 0; i < numTextureSets; i++) {
				for (unsigned int j = 0; j < numVertices; j++) {
					stream << uvSets[i][j];
				}
			}
		}

		stream << consistencyFlags;
		additionalDataRef->write(stream);

	}

	void NiTriBasedGeometryData::read(NiStream& stream) {

		NiGeometryData::read(stream);

		stream >> numTriangles;

	}

	void NiTriBasedGeometryData::write(NiStream& stream) {

		NiGeometryData::write(stream);

		stream << numTriangles;

	}


	void NiTriShapeData::read(NiStream& stream) {

		NiTriBasedGeometryData::read(stream);

		stream >> numTrianglePoints;
		stream >> hasTriangles;

		if (hasTriangles) {
			triangles.resize(numTriangles);
			for (int i = 0; i < numTriangles; i++) {
				stream >> triangles[i];
			}
		}
		stream >> numMatchGroups;

		matchGroups.resize(numMatchGroups);
		MatchGroup group;
		for (int i = 0; i < numMatchGroups; i++) {
			stream >> group.numVertices;
			group.vertexIndices.resize(group.numVertices);
			for (int j = 0; j < group.numVertices; j++) {
				stream >> group.vertexIndices[j];
			}
			matchGroups[i] = group;
		}



	}

	// #not-tested!!!
	void NiTriShapeData::write(NiStream& stream) {

		NiTriBasedGeometryData::write(stream);

		stream << numTrianglePoints;
		stream << hasTriangles;

		if (hasTriangles) {
			for (int i = 0; i < numTriangles; i++) {
				stream << triangles[i];
			}
		}

		stream << numMatchGroups;
		for (int i = 0; i < numMatchGroups; i++) {
			stream << matchGroups[i].numVertices;
			for (int j = 0; j < matchGroups[i].numVertices; j++) {
				stream << matchGroups[i].vertexIndices[j];
			}
		}
	}


	std::unique_ptr<std::vector<Triangle>> NiTriShapeData::getTriangles() const {

		std::unique_ptr<std::vector<Triangle>> _triangles = std::make_unique<std::vector<Triangle>>();

		for (auto triangle : triangles) {
			_triangles->push_back(triangle);
		}

		return _triangles;
	}




	void NiTriStripsData::read(NiStream& stream) {

		NiTriBasedGeometryData::read(stream);

		stream >> numStrips;

		stripLengths.resize(numStrips);
		for (unsigned int i = 0; i < numStrips; i++) {
			stream >> stripLengths[i];
		}

		stream >> hasPoints;
		if (hasPoints) {
			points.resize(numStrips);
			for (unsigned int i = 0; i < numStrips; i++) {
				points[i].resize(stripLengths[i]);
				for (unsigned int j = 0; j < stripLengths[i]; j++) {
					stream >> points[i][j];
				}
			}
		}

	}

	void NiTriStripsData::write(NiStream& stream) {

		NiTriBasedGeometryData::write(stream);

		stream << numStrips;

		for (unsigned int i = 0; i < numStrips; i++) {
			stream << stripLengths[i];
		}

		stream << hasPoints;
		if (hasPoints) {
			for (unsigned int i = 0; i < numStrips; i++) {
				for (unsigned int j = 0; j < stripLengths[i]; j++) {
					stream << points[i][j];
				}
			}
		}

	}

	std::unique_ptr<std::vector<Triangle>> NiTriStripsData::getTriangles() const {

		std::unique_ptr<std::vector<Triangle>> triangles = std::make_unique<std::vector<Triangle>>();

		Triangle triangle;

		for (int i = 0; i < numStrips; i++) {
			for (int j = 0; j < stripLengths[i] - 2; j++) {
				if (j & 1) {
					triangle.p1 = points[i][j];
					triangle.p2 = points[i][j + 2];
					triangle.p3 = points[i][j + 1];
				} else {
					triangle.p1 = points[i][j];
					triangle.p2 = points[i][j + 1];
					triangle.p3 = points[i][j + 2];
				}

				if (triangle.p1 == triangle.p2 || triangle.p2 == triangle.p3 || triangle.p3 == triangle.p1) {
					continue;
				}

				triangles->push_back(triangle);
			}
		}



		return triangles;
	}


	bhkPackedNiTriStripsShape::bhkPackedNiTriStripsShape() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void bhkPackedNiTriStripsShape::read(NiStream& stream) {

		stream >> userData;
		stream >> bhkPackedNiTriStripsShape_unknown1;
		stream >> radius;
		stream >> bhkPackedNiTriStripsShape_unknown2;
		stream >> scale;
		stream >> radius2;
		stream >> scale2;
		dataRef->read(stream);
	}

	void bhkPackedNiTriStripsShape::write(NiStream& stream) {

		stream << userData;
		stream << bhkPackedNiTriStripsShape_unknown1;
		stream << radius;
		stream << bhkPackedNiTriStripsShape_unknown2;
		stream << scale;
		stream << radius2;
		stream << scale2;
		dataRef->write(stream);
	}


	void bhkPackedNiTriStripsData::read(NiStream& stream) {

		stream >> numTriangles;

		triangleData.resize(numTriangles);
		for (unsigned int i = 0; i < numTriangles; i++) {
			stream >> triangleData[i];
		}

		stream >> numVertices;
		stream >> hkPackedNiTriStripsData_unknown1;

		vertices.resize(numVertices);
		for (unsigned int i = 0; i < numVertices; i++) {
			stream >> vertices[i];
		}

		stream >> numSubShapes;

		subShapes.resize(numSubShapes);
		for (unsigned int i = 0; i < numSubShapes; i++) {
			stream >> subShapes[i];
		}

	}

	void bhkPackedNiTriStripsData::write(NiStream& stream) {

		stream << numTriangles;

		for (unsigned int i = 0; i < numTriangles; i++) {
			stream << triangleData[i];
		}

		stream << numVertices;
		stream << hkPackedNiTriStripsData_unknown1;

		for (unsigned int i = 0; i < numVertices; i++) {
			stream << vertices[i];
		}

		stream << numSubShapes;

		for (unsigned int i = 0; i < numSubShapes; i++) {
			stream << subShapes[i];
		}

	}


	bhkNiTriStripsShape::bhkNiTriStripsShape() {
		partRefs = std::make_unique<NiReferenceArray>(this);
	}
	//#not-tested!!!
	void bhkNiTriStripsShape::read(NiStream& stream) {

		stream >> material;
		stream >> radius;
		stream >> bhkNiTriStripsShape_unknown1;
		stream >> growBy;
		stream >> scale;

		partRefs->read(stream);

		stream >> numFilters;
		filters.resize(numFilters);
		for (unsigned int i = 0; i < numFilters; i++) {
			stream >> filters[i];
		}
	}

	void bhkNiTriStripsShape::write(NiStream& stream) {

		stream << material;
		stream << radius;
		stream << bhkNiTriStripsShape_unknown1;
		stream << growBy;
		stream << scale;

		partRefs->write(stream);

		stream << numFilters;
		for (unsigned int i = 0; i < numFilters; i++) {
			stream << filters[i];
		}

	}






	NiGeometry4::NiGeometry4() {
		shaderPropertyRef = std::make_unique<NiReference>(this);
		alphaPropertyRef = std::make_unique<NiReference>(this);
	}


	//not tested
	void NiGeometry4::read(NiStream& stream) {}

	void NiGeometry4::write(NiStream& stream) {

		NiAvObject::write(stream);

		stream << boundingSphereCenter;

		stream << boundingSphereRadius;

		skinRef->write(stream);

		shaderPropertyRef->write(stream);

		alphaPropertyRef->write(stream);

		stream << vertexDesc.get();

	}



	BSTriShape::BSTriShape() {
		vertexDesc.setFlag(VertexFlags::VF_VERTEX);
		vertexDesc.setFlag(VertexFlags::VF_UV);
		vertexDesc.setFlag(VertexFlags::VF_NORMAL);
		vertexDesc.setFlag(VertexFlags::VF_TANGENT);
		//vertexDesc.vertexDesc.SetFlag(VertexFlags::VF_SKINNED);
	}

	//not tested
	void BSTriShape::read(NiStream& stream) { }

	void BSTriShape::write(NiStream& stream) {

		NiGeometry4::write(stream);

		stream << numTriangles;

		stream << numVertices;

		stream << dataSize;

		if (dataSize > 0) {

			half_float::half half;

			for (int i = 0; i < numVertices; i++) {

				auto& vertex = vertexData[i];

				if (vertexDesc.hasVertices()) {

					if (vertexDesc.isFullPrecision()) {

						stream << vertex.vertex;
						stream << vertex.bitangentX;

					} else {

						half = vertex.vertex.x;
						stream.write((char*)&half, 2);
						half = vertex.vertex.y;
						stream.write((char*)&half, 2);
						half = vertex.vertex.z;
						stream.write((char*)&half, 2);
						half = vertex.bitangentX;
						stream.write((char*)&half, 2);

					}
				}

				if (vertexDesc.hasUVs()) {

					half = vertex.uv.u;
					stream.write((char*)&half, 2);
					half = vertex.uv.v;
					stream.write((char*)&half, 2);

				}

				if (vertexDesc.hasNormals()) {

					for (int j = 0; j < 3; j++) {
						stream << vertex.normal[j];
					}

					stream << vertex.bitangentY;

					if (vertexDesc.hasTangents()) {

						for (int j = 0; j < 3; j++) {
							stream << vertex.tangent[j];
						}

						stream << vertex.bitangentZ;

					}
				}

				if (vertexDesc.hasVertexColors()) {

					for (int j = 0; j < 4; j++) {
						stream << vertex.vertexColors[j];
					}

				}


				if (vertexDesc.isSkinned()) {

					for (int j = 0; j < 4; j++) {

						half = vertex.boneWeights[j];
						stream.write((char*)&half, 2);

					}

					for (int j = 0; j < 4; j++) {
						stream << vertex.boneIndices[j];
					}

				}

				if (vertexDesc.hasEyeData()) {
					stream << vertex.eyeData;
				}

			}
		}

		if (dataSize > 0) {
			for (unsigned int i = 0; i < numTriangles; i++) {
				stream << triangles[i];
			}

		}



	}



	//not tested
	void BSSubIndexTriShape::read(NiStream& stream) {}

	void BSSubIndexTriShape::write(NiStream& stream) {

		BSTriShape::write(stream);

		if (dataSize > 0) {
			stream << numPrimitives;
			stream << numSegments;
			stream << totalSegments;

			for (auto &segment : segments) {
				stream << segment.startIndex;
				stream << segment.numPrimitives;
				stream << segment.parentArrayIndex;
				stream << segment.numSubSegments;

				for (auto &subSegment : segment.subSegments) {
					stream << subSegment.startIndex;
					stream << subSegment.numPrimitives;
					stream << subSegment.arrayIndex;
					stream << subSegment.unused;
				}
			}

			if (numSegments < totalSegments) {
				stream << segmentData.numSegments;
				stream << segmentData.totalSegments;

				for (auto &arrayIndex : segmentData.arrayIndices)
					stream << arrayIndex;

				for (auto &perSegmentData : segmentData.arrayPerSegmentData) {
					stream << perSegmentData.userIndex;
					stream << perSegmentData.boneID;
					stream << perSegmentData.numCutOffsets;

					for (auto &cutOffset : perSegmentData.cutOffsets)
						stream << cutOffset;
				}
				segmentData.ssfFile.write(stream);
			}
		}


	}



	BSLightingShaderProperty::BSLightingShaderProperty() {
		//shaderFlags1 = 0x80400203;
		//shaderFlags2 = 0x00000081;
		textureSetRef = std::make_unique<NiReference>(this);
	};

	void BSLightingShaderProperty::read(NiStream& stream) {}

	void BSLightingShaderProperty::write(NiStream& stream) {



		stream << shaderType;

		BSShaderProperty::write(stream);

		textureSetRef->write(stream);

		stream << emissiveColor;

		stream << emissiveMultiple;

		stream << wetMaterialNameRef;

		stream << textureClampMode;

		stream << alpha;

		stream << refractionStrength;

		stream << smoothness;

		stream << specularColor;

		stream << specularStrength;

		stream << subsurfaceRolloff;

		stream << rimlightPower;

		stream << backlightPower;

		stream << grayscaleToPaletteScale;

		stream << fresnelPower;

		stream << wetnessSpecScale;

		stream << wetnessSpecPower;

		stream << wetnessMinVar;

		stream << wetnessEnvmapScale;

		stream << wetnessFresnelPower;

		stream << wetnessMetalness;

		switch (shaderType) {
		case 1:
			stream << environmentMapScale;
			stream << unkEnvmapShort;
			break;
		case 5:
			stream << skinTintColor;
			stream << unknownSkinTintInt;
			break;
		case 6:
			stream << hairTintColor;
			break;
		case 7:
			stream << maxPasses;
			stream << scale;
			break;
		case 11:
			stream << parallaxInnerLayerThickness;
			stream << parallaxRefractionScale;
			stream << parallaxInnerLayerTextureScale;
			stream << parallaxEnvmapStrength;
			break;
		case 14:
			stream << sparkleParameters;
			break;
		case 16:
			stream << eyeCubemapScale;
			stream << eyeLeftReflectionCenter;
			stream << eyeRightReflectionCenter;
			break;
		}

	}

	void BSEffectShaderProperty::read(NiStream& stream) {}

	void BSEffectShaderProperty::write(NiStream& stream) {

		BSShaderProperty::write(stream);

		sourceTexture.write(stream);

		stream << textureClampMode;

		stream << lightingInfluence;

		stream << envMapMinLOD;

		stream << unknownByte;

		stream << falloffStartAngle;

		stream << falloffStopAngle;

		stream << falloffStartOpacity;

		stream << falloffStopOpacity;

		stream << emissiveColor;

		stream << emissiveMultiple;

		stream << softFalloffDepth;

		greyscaleTexture.write(stream);

		envMapTexture.write(stream);

		normalTexture.write(stream);

		envMaskTexture.write(stream);

		stream << envMapScale;

	}

	void BSWaterShaderProperty::read(NiStream& stream) {}

	void BSWaterShaderProperty::write(NiStream& stream) {

		BSShaderProperty::write(stream);

		stream << waterFlags;

	}

	void BSSkyShaderProperty::read(NiStream& stream) {}

	void BSSkyShaderProperty::write(NiStream& stream) {

		BSShaderProperty::write(stream);

		baseTexture.write(stream);
		stream << skyFlags;

	}



	BSMasterParticleSystem::BSMasterParticleSystem() {
		particleSysRefs = std::make_unique<NiReferenceArray>(this);
	}

	void BSMasterParticleSystem::read(NiStream& stream) {

		NiNode::read(stream);

		stream >> maxEmitterObjects;

		particleSysRefs->read(stream);
	}


	void BSMasterParticleSystem::write(NiStream& stream) {

		NiNode::write(stream);

		stream << maxEmitterObjects;

		particleSysRefs->write(stream);
	}


	NiParticleSystem::NiParticleSystem() {
		vertexDesc.setFlag(VertexFlags::VF_UV);
		vertexDesc.setFlag(VertexFlags::VF_FULLPREC);
		modifiers = std::make_unique<NiReferenceArray>(this);
	}

	void NiParticleSystem::read(NiStream& stream) {

		NiGeometry3::read(stream);

		stream >> worldSpace;

		modifiers->read(stream);

	}


	void NiParticleSystem::write(NiStream& stream) {

		NiGeometry4::write(stream);

		stream << farBegin;
		stream << farEnd;
		stream << nearBegin;
		stream << nearEnd;

		dataRef->write(stream);

		stream << worldSpace;

		modifiers->write(stream);

	}




	NiPSysData::NiPSysData() {
		additionalDataRef = std::make_unique<NiReference>(this);
	}

	void NiPSysData::read(NiStream& stream) {

		stream >> groupID;
		stream >> numVertices;
		stream >> keepFlags;
		stream >> compressFlags;
		stream >> hasVertices;

		stream >> bsVectorFlags;

		stream >> hasNormals;

		stream >> center;
		stream >> radius;

		stream >> hasVertexColors;

		stream >> consistencyFlags;
		additionalDataRef->read(stream);

		stream >> hasRadii;
		stream >> numActive;
		stream >> hasSizes;
		stream >> hasRotations;
		stream >> hasRotationAngles;
		stream >> hasRotationAxes;
		stream >> hasTextureIndices;

		unsigned char numOffsets = 0;
		stream >> numOffsets;
		numSubtexOffsets = numOffsets;

		subtexOffsets.resize(numSubtexOffsets);
		for (unsigned int i = 0; i < numSubtexOffsets; i++) {
			stream >> subtexOffsets[i];
		}

		stream >> hasRotationSpeeds;

	}


	void NiPSysData::write(NiStream& stream) {

		stream << groupID;
		stream << numVertices;
		stream << keepFlags;
		stream << compressFlags;
		stream << hasVertices;

		stream << bsVectorFlags;

		stream << materialCRC;

		stream << hasNormals;

		stream << center;
		stream << radius;

		stream << hasVertexColors;

		stream << consistencyFlags;
		additionalDataRef->write(stream);

		stream << hasRadii;
		stream << numActive;
		stream << hasSizes;
		stream << hasRotations;
		stream << hasRotationAngles;
		stream << hasRotationAxes;
		stream << hasTextureIndices;

		stream << numSubtexOffsets;


		for (unsigned int i = 0; i < numSubtexOffsets; i++) {
			stream << subtexOffsets[i];
		}

		stream << aspectRatio;
		stream << aspectFlags;
		stream << speedToAspectAspect2;
		stream << speedToAspectSpeed1;
		stream << speedToAspectSpeed2;

		stream << hasRotationSpeeds;

	}

	void BSStripPSysData::read(NiStream& stream) {

		NiPSysData::read(stream);

		stream >> maxPointCount;
		stream >> startCapSize;
		stream >> endCapSize;
		stream >> doZPrepass;
	}

	void BSStripPSysData::write(NiStream& stream) {

		NiPSysData::write(stream);

		stream << maxPointCount;
		stream << startCapSize;
		stream << endCapSize;
		stream << doZPrepass;
	}




	NiTimeController::NiTimeController() {
		nextControllerRef = std::make_unique<NiReference>(this);
		targetRef = std::make_unique<NiReference>(this);
	}

	void NiTimeController::read(NiStream& stream) {

		nextControllerRef->read(stream);
		stream >> flags;
		stream >> frequency;
		stream >> phase;
		stream >> startTime;
		stream >> stopTime;
		targetRef->read(stream);
	}


	void NiTimeController::write(NiStream& stream) {

		nextControllerRef->write(stream);
		stream << flags;
		stream << frequency;
		stream << phase;
		stream << startTime;
		stream << stopTime;
		targetRef->write(stream);
	}



	NiInterpolatorController::NiInterpolatorController() {
		interpolatorRef = std::make_unique<NiReference>(this);
	}

	void NiInterpolatorController::read(NiStream& stream) {

		NiTimeController::read(stream);
		interpolatorRef->read(stream);

	}


	void NiInterpolatorController::write(NiStream& stream) {

		NiTimeController::write(stream);
		interpolatorRef->write(stream);

	}


	void FloatController::read(NiStream& stream) {

		NiInterpolatorController::read(stream);

		stream >> typeOfControlledVariable;
	}


	void FloatController::write(NiStream& stream) {

		NiInterpolatorController::write(stream);

		stream << typeOfControlledVariable;
	}

	void ColorController::read(NiStream& stream) {

		NiInterpolatorController::read(stream);

		stream >> typeOfControlledColor;
	}


	void ColorController::write(NiStream& stream) {

		NiInterpolatorController::write(stream);

		stream << typeOfControlledColor;
	}


	void NiMaterialColorController::read(NiStream& stream) {

		NiInterpolatorController::read(stream);

		stream >> targetColor;
	}


	void NiMaterialColorController::write(NiStream& stream) {

		NiInterpolatorController::write(stream);

		stream << targetColor;
	}


	void NiTextureTransformController::read(NiStream& stream) {

		NiInterpolatorController::read(stream);

		stream >> shaderMap;
		stream >> textureSlot;
		stream >> operation;
	}


	void NiTextureTransformController::write(NiStream& stream) {

		NiInterpolatorController::write(stream);

		stream << shaderMap;
		stream << textureSlot;
		stream << operation;
	}




	void NiDynamicEffect::read(NiStream& stream) {

		NiAvObject::read(stream);

		stream >> switchState;

		stream >> numAffectedNodes;

		affectedNodes.resize(numAffectedNodes);
		for (unsigned int i = 0; i < numAffectedNodes; i++) {
			stream >> affectedNodes[i];
		}

	}

	void NiDynamicEffect::write(NiStream& stream) {

		NiAvObject::write(stream);

	}


	void NiLight::read(NiStream& stream) {

		NiDynamicEffect::read(stream);

		stream >> dimmer;
		stream >> ambientColor;
		stream >> diffuseColor;
		stream >> specularColor;

	}

	void NiLight::write(NiStream& stream) {

		NiDynamicEffect::write(stream);

		stream << dimmer;
		stream << ambientColor;
		stream << diffuseColor;
		stream << specularColor;

	}



	void NiPointLight::read(NiStream& stream) {

		NiLight::read(stream);

		stream >> constantAttenuation;
		stream >> linearAttenuation;
		stream >> quadraticAttenuation;

	}

	void NiPointLight::write(NiStream& stream) {

		NiLight::write(stream);

		stream << constantAttenuation;
		stream << linearAttenuation;
		stream << quadraticAttenuation;

	}


	void NiSpotLight::read(NiStream& stream) {

		NiPointLight::read(stream);

		stream >> cutoffAngle;
		stream >> unkFloat;
		stream >> exponent;
	}

	void NiSpotLight::write(NiStream& stream) {

		NiPointLight::write(stream);

		stream << cutoffAngle;
		stream << unkFloat;
		stream << exponent;
	}




	void NiSequence::read(NiStream& stream) {

		stream >> nameRef;

		stream >> numControlledBlocks;

		stream >> arrayGrowBy;

		for (unsigned int i = 0; i < numControlledBlocks; i++) {

			std::unique_ptr<ControllerBlock> block = std::make_unique<ControllerBlock>(this);

			block->interpolatorRef->read(stream);
			block->controllerRef->read(stream);
			stream >> block->priority;
			stream >> block->nodeName;
			stream >> block->propType;
			stream >> block->controllerType;
			stream >> block->controllerID;
			stream >> block->interpolatorID;

			controlledBlocks.push_back(std::move(block));

		}
	}

	void NiSequence::write(NiStream& stream) {

		stream << nameRef;

		stream << numControlledBlocks;

		stream << arrayGrowBy;

		for (unsigned int i = 0; i < numControlledBlocks; i++) {

			controlledBlocks[i]->interpolatorRef->write(stream);
			controlledBlocks[i]->controllerRef->write(stream);
			stream << controlledBlocks[i]->priority;
			stream << controlledBlocks[i]->nodeName;
			stream << controlledBlocks[i]->propType;
			stream << controlledBlocks[i]->controllerType;
			stream << controlledBlocks[i]->controllerID;
			stream << controlledBlocks[i]->interpolatorID;

		}

	}



	NiControllerSequence::NiControllerSequence() {
		textKeyRef = std::make_unique<NiReference>(this);
		managerRef = std::make_unique<NiReference>(this);
		animNoteRefs = std::make_unique<NiReferenceArrayShort>(this);
	}

	void NiControllerSequence::read(NiStream& stream) {

		NiSequence::read(stream);

		stream >> weight;
		textKeyRef->read(stream);
		stream >> cycleType;
		stream >> frequency;
		stream >> startTime;
		stream >> stopTime;
		managerRef->read(stream);
		stream >> accumRootName;

		animNoteRefs->read(stream);

	}

	void NiControllerSequence::write(NiStream& stream) {

		NiSequence::write(stream);

		stream << weight;
		textKeyRef->write(stream);
		stream << cycleType;
		stream << frequency;
		stream << startTime;
		stream << stopTime;
		managerRef->write(stream);
		stream << accumRootName;

		animNoteRefs->write(stream);
	}






	NiBoneContainer::NiBoneContainer() {
		boneRefs = std::make_unique<NiReferenceArray>(this);
	}

	void NiBoneContainer::read(NiStream& stream) {
		boneRefs->read(stream);
	}
	void NiBoneContainer::write(NiStream& stream) {
		boneRefs->write(stream);
	}



	NiSkinInstance::NiSkinInstance() {
		dataRef = std::make_unique<NiReference>(this);
		skinPartitionRef = std::make_unique<NiReference>(this);
		skeletonRoot = std::make_unique<NiReference>(this);
	}

	void NiSkinInstance::read(NiStream& stream) {

		dataRef->read(stream);
		skinPartitionRef->read(stream);
		skeletonRoot->read(stream);

		NiBoneContainer::read(stream);

	}

	void NiSkinInstance::write(NiStream& stream) {}



	void BSDismemberSkinInstance::read(NiStream& stream) {

		NiSkinInstance::read(stream);

		stream >> numPartitions;

		partitions.resize(numPartitions);
		for (unsigned int i = 0; i < numPartitions; i++) {
			stream >> partitions[i];
		}
	}

	void BSDismemberSkinInstance::write(NiStream& stream) {}





	void NiSkinData::read(NiStream& stream) {

		stream >> skinTransform.rotation;
		stream >> skinTransform.translation;
		stream >> skinTransform.scale;
		stream >> numBones;
		stream >> hasVertexWeights;

		if (hasVertexWeights > 1)
			hasVertexWeights = 1;

		bones.resize(numBones);
		for (unsigned int i = 0; i < numBones; i++) {
			BoneData boneData;
			stream >> boneData.boneTransform.rotation;
			stream >> boneData.boneTransform.translation;
			stream >> boneData.boneTransform.scale;
			stream >> boneData.center;
			stream >> boneData.radius;
			stream >> boneData.numVertices;

			if (hasVertexWeights) {
				boneData.vertexWeights.resize(boneData.numVertices);

				for (int j = 0; j < boneData.numVertices; j++) {
					stream >> boneData.vertexWeights[j].index;
					stream >> boneData.vertexWeights[j].weight;
				}
			} else
				boneData.numVertices = 0;

			bones[i] = std::move(boneData);
		}


	}

	void NiSkinData::write(NiStream& stream) {}







	void NiSkinPartition::read(NiStream& stream) {

		stream >> numPartitions;
		partitions.resize(numPartitions);

		for (unsigned int p = 0; p < numPartitions; p++) {

			PartitionBlock partition;
			stream >> partition.numVertices;
			stream >> partition.numTriangles;
			stream >> partition.numBones;
			stream >> partition.numStrips;
			stream >> partition.numWeightsPerVertex;

			partition.bones.resize(partition.numBones);
			for (int i = 0; i < partition.numBones; i++)
				stream >> partition.bones[i];

			stream >> partition.hasVertexMap;
			if (partition.hasVertexMap) {
				partition.vertexMap.resize(partition.numVertices);
				for (int i = 0; i < partition.numVertices; i++)
					stream >> partition.vertexMap[i];
			}

			stream >> partition.hasVertexWeights;
			if (partition.hasVertexWeights) {
				partition.vertexWeights.resize(partition.numVertices);
				for (int i = 0; i < partition.numVertices; i++)
					stream >> partition.vertexWeights[i];
			}

			partition.stripLengths.resize(partition.numStrips);
			for (int i = 0; i < partition.numStrips; i++)
				stream >> partition.stripLengths[i];

			stream >> partition.hasFaces;
			if (partition.hasFaces) {
				partition.strips.resize(partition.numStrips);
				for (int i = 0; i < partition.numStrips; i++) {
					partition.strips[i].resize(partition.stripLengths[i]);
					for (int j = 0; j < partition.stripLengths[i]; j++)
						stream >> partition.strips[i][j];
				}
			}

			if (partition.numStrips == 0 && partition.hasFaces) {
				partition.triangles.resize(partition.numTriangles);
				for (int i = 0; i < partition.numTriangles; i++)
					stream >> partition.triangles[i];
			}

			stream >> partition.hasBoneIndices;
			if (partition.hasBoneIndices) {
				partition.boneIndices.resize(partition.numVertices);
				for (int i = 0; i < partition.numVertices; i++)
					stream >> partition.boneIndices[i];
			}

			partitions[p] = partition;
		}


	}

	void NiSkinPartition::write(NiStream& stream) {}







	BSSkinInstance::BSSkinInstance() {
		skeletonRoot = std::make_unique<NiReference>(this);
		dataRef = std::make_unique<NiReference>(this);
	}

	void BSSkinInstance::read(NiStream& stream) {}

	void BSSkinInstance::write(NiStream& stream) {

		skeletonRoot->write(stream);
		dataRef->write(stream);

		NiBoneContainer::write(stream);

		stream << num_BSSkinInstance_Unknown;

		for (unsigned int i = 0; i < num_BSSkinInstance_Unknown; i++) {
			stream << BSSkinInstance_Unknown[i];
		}

	}

	void BSSkinBoneData::read(NiStream& stream) {}

	void BSSkinBoneData::write(NiStream& stream) {

		stream << numBones;

		for (unsigned int i = 0; i < numBones; i++) {
			stream << boneData[i].center;
			stream << boneData[i].radius;
			stream << boneData[i].boneTransform.rotation;
			stream << boneData[i].boneTransform.translation;
			stream << boneData[i].boneTransform.scale;
		}

	}


















	//---------------------------------------------- new ----------------------------------------


	// interpolators

	NiBoolInterpolator::NiBoolInterpolator() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void NiBoolInterpolator::read(NiStream& stream) {
		stream >> value;
		dataRef->read(stream);
	}

	void NiBoolInterpolator::write(NiStream& stream) {
		stream << value;
		dataRef->write(stream);
	}



	NiTransformInterpolator::NiTransformInterpolator() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void NiTransformInterpolator::read(NiStream& stream) {

		stream >> translation;
		stream >> rotation;
		stream >> scale;
		dataRef->read(stream);

	}

	void NiTransformInterpolator::write(NiStream& stream) {

		stream << translation;
		stream << rotation;
		stream << scale;
		dataRef->write(stream);

	}



	NiFloatInterpolator::NiFloatInterpolator() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void NiFloatInterpolator::read(NiStream& stream) {

		stream >> value;
		dataRef->read(stream);

	}

	void NiFloatInterpolator::write(NiStream& stream) {

		stream << value;
		dataRef->write(stream);

	}



	NiPoint3Interpolator::NiPoint3Interpolator() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void NiPoint3Interpolator::read(NiStream& stream) {

		stream >> value;
		dataRef->read(stream);

	}

	void NiPoint3Interpolator::write(NiStream& stream) {

		stream << value;
		dataRef->write(stream);

	}




	void NiBlendInterpolator::read(NiStream& stream) {

		stream >> flags;
		stream >> arraySize;
		stream >> weightThreshold;

	}

	void NiBlendInterpolator::write(NiStream& stream) {

		stream << flags;
		stream << arraySize;
		stream << weightThreshold;

	}



	void NiBlendBoolInterpolator::read(NiStream& stream) {

		NiBlendInterpolator::read(stream);

		stream >> value;

	}

	void NiBlendBoolInterpolator::write(NiStream& stream) {

		NiBlendInterpolator::write(stream);

		stream << value;

	}



	void NiBlendFloatInterpolator::read(NiStream& stream) {

		NiBlendInterpolator::read(stream);

		stream >> value;

	}

	void NiBlendFloatInterpolator::write(NiStream& stream) {

		NiBlendInterpolator::write(stream);

		stream << value;

	}



	void NiBlendPoint3Interpolator::read(NiStream& stream) {

		NiBlendInterpolator::read(stream);

		stream >> value;
	}

	void NiBlendPoint3Interpolator::write(NiStream& stream) {

		NiBlendInterpolator::write(stream);

		stream << value;

	}



	NiPathInterpolator::NiPathInterpolator() {
		pathDataRef = std::make_unique<NiReference>(this);
		percentDataRef = std::make_unique<NiReference>(this);
	}

	void NiPathInterpolator::read(NiStream& stream) {

		stream >> flags;
		stream >> bankDDir;
		stream >> maxBankAngle;
		stream >> smoothing;
		stream >> followAxis;

		pathDataRef->read(stream);
		percentDataRef->read(stream);

	}

	void NiPathInterpolator::write(NiStream& stream) {

		stream << flags;
		stream << bankDDir;
		stream << maxBankAngle;
		stream << smoothing;
		stream << followAxis;

		pathDataRef->write(stream);
		percentDataRef->write(stream);

	}









	// contollers

	NiControllerManager::NiControllerManager() {
		controllerSequenceRefs = std::make_unique<NiReferenceArray>(this);
		objectPalette = std::make_unique<NiReference>(this);
	}

	void NiControllerManager::read(NiStream& stream) {

		NiTimeController::read(stream);

		stream >> cumulative;
		controllerSequenceRefs->read(stream);
		objectPalette->read(stream);

	}

	void NiControllerManager::write(NiStream& stream) {

		NiTimeController::write(stream);

		stream << cumulative;
		controllerSequenceRefs->write(stream);
		objectPalette->write(stream);

	}


	NiMultiTargetTransformController::NiMultiTargetTransformController() {
		extraTargetRefs = std::make_unique<NiReferenceArrayShort>(this);
	}

	void NiMultiTargetTransformController::read(NiStream& stream) {

		NiTimeController::read(stream);

		extraTargetRefs->read(stream);

	}

	void NiMultiTargetTransformController::write(NiStream& stream) {

		NiTimeController::write(stream);

		extraTargetRefs->write(stream);

	}





	void bhkBlendController::read(NiStream& stream) {

		NiTimeController::read(stream);

		stream >> keys;

	}

	void bhkBlendController::write(NiStream& stream) {

		NiTimeController::write(stream);

		stream << keys;

	}




	void NiFloatExtraDataController::read(NiStream& stream) {

		NiInterpolatorController::read(stream);

		stream >> extraDataNameRef;

	}

	void NiFloatExtraDataController::write(NiStream& stream) {

		NiInterpolatorController::write(stream);

		stream << extraDataNameRef;

	}



	NiGeomMorpherController::NiGeomMorpherController() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void NiGeomMorpherController::read(NiStream& stream) {

		NiTimeController::read(stream);

		stream >> morpherFlags;
		dataRef->read(stream);
		stream >> alwaysUpdate;

		uint32_t sz = 0;
		stream >> sz;

		for (uint32_t i = 0; i < sz; i++) {
			std::unique_ptr<InterplatorWeight> obj = std::make_unique<InterplatorWeight>(this);
			obj->read(stream);
			weights.push_back(std::move(obj));
		}

	}

	void NiGeomMorpherController::write(NiStream& stream) {

		NiTimeController::write(stream);

		stream << morpherFlags;
		dataRef->write(stream);
		stream << alwaysUpdate;

		uint32_t sz = weights.size();
		stream << sz;

		for (auto& ref : weights) {
			ref->write(stream);
		}

	}




	void NiBSBoneLODController::read(NiStream& stream) {

		NiTimeController::read(stream);

		stream >> lod;
		stream >> numLods;

		uint32_t sz = 0;
		stream >> sz;

		for (uint32_t i = 0; i < sz; i++) {
			std::unique_ptr<NodeGroup> obj = std::make_unique<NodeGroup>(this);
			obj->read(stream);
			nodeGroups.push_back(std::move(obj));
		}

	}

	void NiBSBoneLODController::write(NiStream& stream) {

		NiTimeController::write(stream);

		stream << lod;
		stream << numLods;

		uint32_t sz = nodeGroups.size();
		stream << sz;

		for (auto& ref : nodeGroups) {
			ref->write(stream);
		}

	}





	void NiPSysModifierCtlr::read(NiStream& stream) {

		NiInterpolatorController::read(stream);

		stream >> modifierNameRef;

	}

	void NiPSysModifierCtlr::write(NiStream& stream) {

		NiInterpolatorController::write(stream);

		stream << modifierNameRef;

	}




	void NiLightColorController::read(NiStream& stream) {

		NiInterpolatorController::read(stream);

		stream >> targetColor;

	}

	void NiLightColorController::write(NiStream& stream) {

		NiInterpolatorController::write(stream);

		stream << targetColor;

	}



	NiPSysEmitterCtlr::NiPSysEmitterCtlr() {
		visibilityInterpolatorRef = std::make_unique<NiReference>(this);
	}

	void NiPSysEmitterCtlr::read(NiStream& stream) {

		NiPSysModifierCtlr::read(stream);

		visibilityInterpolatorRef->read(stream);

	}

	void NiPSysEmitterCtlr::write(NiStream& stream) {

		NiPSysModifierCtlr::write(stream);

		visibilityInterpolatorRef->write(stream);

	}



	BSPSysMultiTargetEmitterCtlr::BSPSysMultiTargetEmitterCtlr() {
		masterPartcleSystemRef = std::make_unique<NiReference>(this);
	}

	void BSPSysMultiTargetEmitterCtlr::read(NiStream& stream) {

		NiPSysEmitterCtlr::read(stream);

		stream >> maxEmitters;
		masterPartcleSystemRef->read(stream);

	}

	void BSPSysMultiTargetEmitterCtlr::write(NiStream& stream) {

		NiPSysEmitterCtlr::write(stream);

		stream << maxEmitters;
		masterPartcleSystemRef->write(stream);

	}




















	// modifier 



	NiPSysModifier::NiPSysModifier() {
		targetRef = std::make_unique<NiReference>(this);
	}

	void NiPSysModifier::read(NiStream& stream) {

		stream >> nameRef;
		stream >> order;
		targetRef->read(stream);
		stream >> active;

	}

	void NiPSysModifier::write(NiStream& stream) {

		stream << nameRef;
		stream << order;
		targetRef->write(stream);
		stream << active;

	}


	void NiPSysRotationModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> speed;
		stream >> speedVariation;
		stream >> angle;
		stream >> angleVariation;
		stream >> randomRotationSpeedSign;
		stream >> randomAxis;
		stream >> axis;

	}

	void NiPSysRotationModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << speed;
		stream << speedVariation;
		stream << angle;
		stream << angleVariation;
		stream << randomRotationSpeedSign;
		stream << randomAxis;
		stream << axis;;

	}



	void BSPSysSimpleColorModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> fadeInPercent;
		stream >> fadeOutPercent;
		stream >> color1EndPercent;
		stream >> color1StartPercent;
		stream >> color2EndPercent;
		stream >> color2StartPercent;
		stream >> color1;
		stream >> color2;
		stream >> color3;

	}

	void BSPSysSimpleColorModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << fadeInPercent;
		stream << fadeOutPercent;
		stream << color1EndPercent;
		stream << color1StartPercent;
		stream << color2EndPercent;
		stream << color2StartPercent;
		stream << color1;
		stream << color2;
		stream << color3;

	}



	NiPSysAgeDeathModifier::NiPSysAgeDeathModifier() {
		spawnModifierRef = std::make_unique<NiReference>(this);
	}

	void NiPSysAgeDeathModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> spawnOnDeath;
		spawnModifierRef->read(stream);

	}

	void NiPSysAgeDeathModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << spawnOnDeath;
		spawnModifierRef->write(stream);

	}



	void NiPSysSpawnModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> numSpawnGenerations;
		stream >> percentageSpawned;
		stream >> minNumToSpawn;
		stream >> maxNumToSpawn;
		stream >> spawnSpeedVariation;
		stream >> spawnDirVariation;
		stream >> lifeSpan;
		stream >> lifeSpanVariation;

	}

	void NiPSysSpawnModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << numSpawnGenerations;
		stream << percentageSpawned;
		stream << minNumToSpawn;
		stream << maxNumToSpawn;
		stream << spawnSpeedVariation;
		stream << spawnDirVariation;
		stream << lifeSpan;
		stream << lifeSpanVariation;

	}



	void NiPSysGrowFadeModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> glowTime;
		stream >> glowGeneration;
		stream >> fadeTime;
		stream >> fadeGeneration;
		stream >> baseScale;

	}

	void NiPSysGrowFadeModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << glowTime;
		stream << glowGeneration;
		stream << fadeTime;
		stream << fadeGeneration;
		stream << baseScale;

	}



	NiPSysGravityModifier::NiPSysGravityModifier() {
		gravityObjectRef = std::make_unique<NiReference>(this);
	}

	void NiPSysGravityModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		gravityObjectRef->read(stream);
		stream >> gravirtyAxis;
		stream >> decay;
		stream >> strength;
		stream >> forceType;
		stream >> turbulence;
		stream >> turbulenceScale;
		stream >> worldAligned;

	}

	void NiPSysGravityModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		gravityObjectRef->write(stream);
		stream << gravirtyAxis;
		stream << decay;
		stream << strength;
		stream << forceType;
		stream << turbulence;
		stream << turbulenceScale;
		stream << worldAligned;

	}




	void NiPSysBoundUpdateModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> updateSkip;

	}

	void NiPSysBoundUpdateModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << updateSkip;

	}



	NiPSysColliderManager::NiPSysColliderManager() {
		colliderRef = std::make_unique<NiReference>(this);
	}

	void NiPSysColliderManager::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		colliderRef->read(stream);

	}

	void NiPSysColliderManager::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		colliderRef->write(stream);

	}



	void BSParentVelocityModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> dampling;

	}

	void BSParentVelocityModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << dampling;

	}




	NiPSysDragModifier::NiPSysDragModifier() {
		dragObjectRef = std::make_unique<NiReference>(this);
	}

	void NiPSysDragModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		dragObjectRef->read(stream);
		stream >> dragAxis;
		stream >> percentage;
		stream >> range;
		stream >> rangeFalloff;

	}

	void NiPSysDragModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		dragObjectRef->write(stream);
		stream << dragAxis;
		stream << percentage;
		stream << range;
		stream << rangeFalloff;

	}




	void BSWindModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> strength;

	}

	void BSWindModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << strength;

	}



	void BSPSysStripUpdateModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> updateDeltaTime;

	}

	void BSPSysStripUpdateModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << updateDeltaTime;

	}



	NiPSysBombModifier::NiPSysBombModifier() {
		bombObject = std::make_unique<NiReference>(this);
	}

	void NiPSysBombModifier::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		bombObject->read(stream);
		stream >> bombAxis;
		stream >> decay;
		stream >> decayV;
		stream >> decayType;
		stream >> symmetryType;

	}

	void NiPSysBombModifier::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		bombObject->write(stream);
		stream << bombAxis;
		stream << decay;
		stream << decayV;
		stream << decayType;
		stream << symmetryType;

	}






















	// emitter 

	void NiPSysEmitter::read(NiStream& stream) {

		NiPSysModifier::read(stream);

		stream >> speed;
		stream >> speedVariation;
		stream >> declination;
		stream >> declinationVariation;
		stream >> angle;
		stream >> angleVariation;
		stream >> initialColor;
		stream >> radius;
		stream >> radiusVariation;
		stream >> lifeSpan;
		stream >> lifeSpanVariation;

	}

	void NiPSysEmitter::write(NiStream& stream) {

		NiPSysModifier::write(stream);

		stream << speed;
		stream << speedVariation;
		stream << declination;
		stream << declinationVariation;
		stream << angle;
		stream << angleVariation;
		stream << initialColor;
		stream << radius;
		stream << radiusVariation;
		stream << lifeSpan;
		stream << lifeSpanVariation;

	}


	NiPSysMeshEmitter::NiPSysMeshEmitter() {
		emitterMeshRefs = std::make_unique<NiReferenceArray>(this);
	}

	void NiPSysMeshEmitter::read(NiStream& stream) {

		NiPSysEmitter::read(stream);

		emitterMeshRefs->read(stream);
		stream >> initialVelocityType;
		stream >> emissionType;
		stream >> emissionAxis;

	}

	void NiPSysMeshEmitter::write(NiStream& stream) {

		NiPSysEmitter::write(stream);

		emitterMeshRefs->write(stream);
		stream << initialVelocityType;
		stream << emissionType;
		stream << emissionAxis;

	}



	NiPSysBoxEmitter::NiPSysBoxEmitter() {
		emitterObjectRef = std::make_unique<NiReference>(this);
	}

	void NiPSysBoxEmitter::read(NiStream& stream) {

		NiPSysEmitter::read(stream);

		emitterObjectRef->read(stream);
		stream >> width;
		stream >> height;
		stream >> depth;

	}

	void NiPSysBoxEmitter::write(NiStream& stream) {

		NiPSysEmitter::write(stream);

		emitterObjectRef->write(stream);
		stream << width;
		stream << height;
		stream << depth;

	}




	NiPSysCylinderEmitter::NiPSysCylinderEmitter() {
		emitterObjectRef = std::make_unique<NiReference>(this);
	}

	void NiPSysCylinderEmitter::read(NiStream& stream) {

		NiPSysEmitter::read(stream);

		emitterObjectRef->read(stream);
		stream >> radius;
		stream >> height;

	}

	void NiPSysCylinderEmitter::write(NiStream& stream) {

		NiPSysEmitter::write(stream);

		emitterObjectRef->write(stream);
		stream << radius;
		stream << height;

	}



	NiPSysSphereEmitter::NiPSysSphereEmitter() {
		emitterObjectRef = std::make_unique<NiReference>(this);
	}

	void NiPSysSphereEmitter::read(NiStream& stream) {

		NiPSysEmitter::read(stream);

		emitterObjectRef->read(stream);
		stream >> radius;

	}

	void NiPSysSphereEmitter::write(NiStream& stream) {

		NiPSysEmitter::write(stream);

		emitterObjectRef->write(stream);
		stream << radius;

	}






























	//other



	NiDefaultAVObjectPalette::NiDefaultAVObjectPalette() {
		sceneRef = std::make_unique<NiReference>(this);
	}

	void NiDefaultAVObjectPalette::read(NiStream& stream) {

		sceneRef->read(stream);

		uint32_t sz = 0;
		stream >> sz;

		for (uint32_t i = 0; i < sz; i++) {
			std::unique_ptr<Object> obj = std::make_unique<Object>(this);
			obj->read(stream);
			objectRefs.push_back(std::move(obj));
		}

	}

	void NiDefaultAVObjectPalette::write(NiStream& stream) {

		sceneRef->write(stream);

		uint32_t sz = objectRefs.size();
		stream << sz;

		for (auto& ref : objectRefs) {
			ref->write(stream);
		}

	}




	void BSBound::read(NiStream& stream) {

		NiExtraData::read(stream);

		stream >> center;
		stream >> dimensions;
	}

	void BSBound::write(NiStream& stream) {

		NiExtraData::write(stream);

		stream << center;
		stream << dimensions;

	}



	void NiFloatExtraData::read(NiStream& stream) {

		NiExtraData::read(stream);

		stream >> data;

	}

	void NiFloatExtraData::write(NiStream& stream) {

		NiExtraData::write(stream);

		stream << data;

	}




	void BSWArray::read(NiStream& stream) {

		NiExtraData::read(stream);

		uint32_t sz = 0;
		stream >> sz;

		items.resize(sz);
		for (uint32_t i = 0; i < sz; i++) {
			stream >> items[i];
		}

	}

	void BSWArray::write(NiStream& stream) {

		NiExtraData::write(stream);

		uint32_t sz = items.size();
		stream << sz;

		for (uint32_t i = 0; i < sz; i++) {
			stream << items[i];
		}

	}


	void BSDecalPlacementVectorExtraData::read(NiStream& stream) {

		NiFloatExtraData::read(stream);

		unsigned short sz = 0;
		stream >> sz;

		blocks.resize(sz);
		for (uint32_t i = 0; i < sz; i++) {
			blocks[i].read(stream);
		}

	}

	void BSDecalPlacementVectorExtraData::write(NiStream& stream) {

		NiFloatExtraData::write(stream);

		unsigned short sz = blocks.size();
		stream << sz;

		for (uint32_t i = 0; i < sz; i++) {
			blocks[i].write(stream);
		}

	}



	void bhkBlendCollisionObject::read(NiStream& stream) {

		bhkCollisionObject::read(stream);

		stream >> heirGain;
		stream >> velGain;

	}

	void bhkBlendCollisionObject::write(NiStream& stream) {

		bhkCollisionObject::write(stream);

		stream << heirGain;
		stream << velGain;

	}






	NiCamera::NiCamera() {
		sceneRef = std::make_unique<NiReference>(this);
	}

	void NiCamera::read(NiStream& stream) {

		NiAvObject::read(stream);

		stream >> cameraFlags;
		stream >> frustrumLeft;
		stream >> frustrumRight;
		stream >> frustrumTop;
		stream >> frustrumBottom;
		stream >> frustrumNear;
		stream >> frustrumFar;
		stream >> useOrthoProjection;
		stream >> viewPortLeft;
		stream >> viewPortRight;
		stream >> viewPortTop;
		stream >> viewPortBottom;
		stream >> lodAdjust;
		sceneRef->read(stream);
		stream >> numScreenPolygons;
		stream >> numScreenTextures;

	}

	void NiCamera::write(NiStream& stream) {

		NiAvObject::write(stream);

		stream << cameraFlags;
		stream << frustrumLeft;
		stream << frustrumRight;
		stream << frustrumTop;
		stream << frustrumBottom;
		stream << frustrumNear;
		stream << frustrumFar;
		stream << useOrthoProjection;
		stream << viewPortLeft;
		stream << viewPortRight;
		stream << viewPortTop;
		stream << viewPortBottom;
		stream << lodAdjust;
		sceneRef->write(stream);
		stream << numScreenPolygons;
		stream << numScreenTextures;

	}


	BSMultiBound::BSMultiBound() {
		dataRef = std::make_unique<NiReference>(this);
	}

	void BSMultiBound::read(NiStream& stream) {

		dataRef->read(stream);

	}

	void BSMultiBound::write(NiStream& stream) {

		dataRef->write(stream);

	}



	void BSMultiBoundSphere::read(NiStream& stream) {

		stream >> center;
		stream >> radius;

	}

	void BSMultiBoundSphere::write(NiStream& stream) {

		stream << center;
		stream << radius;

	}


	NiPSysCollider::NiPSysCollider() {
		spawnModifierRef = std::make_unique<NiReference>(this);
		parentRef = std::make_unique<NiReference>(this);
		nextColliderRef = std::make_unique<NiReference>(this);
		colliderObjectRef = std::make_unique<NiReference>(this);
	}

	void NiPSysCollider::read(NiStream& stream) {

		stream >> bounce;
		stream >> spawnOnCollide;
		stream >> dieOnCollide;
		spawnModifierRef->read(stream);
		parentRef->read(stream);
		nextColliderRef->read(stream);
		colliderObjectRef->read(stream);

	}

	void NiPSysCollider::write(NiStream& stream) {

		stream << bounce;
		stream << spawnOnCollide;
		stream << dieOnCollide;
		spawnModifierRef->write(stream);
		parentRef->write(stream);
		nextColliderRef->write(stream);
		colliderObjectRef->write(stream);

	}



	void NiPSysPlanarCollider::read(NiStream& stream) {

		NiPSysCollider::read(stream);

		stream >> width;
		stream >> hight;
		stream >> xAxis;
		stream >> yAxis;

	}

	void NiPSysPlanarCollider::write(NiStream& stream) {

		NiPSysCollider::write(stream);

		stream << width;
		stream << hight;
		stream << xAxis;
		stream << yAxis;

	}



	void BSMultiBoundAABB::read(NiStream& stream) {

		stream >> position;
		stream >> extent;

	}

	void BSMultiBoundAABB::write(NiStream& stream) {

		stream << position;
		stream << extent;

	}



	bhkLiquidAction::bhkLiquidAction() {
		rigidBodyRef = std::make_unique<NiReference>(this);
	}

	void bhkLiquidAction::read(NiStream& stream) {

		rigidBodyRef->read(stream);
		stream >> bhkLiquidAction_unknown1;
		stream >> initiakStickForce;
		stream >> stickStrength;
		stream >> neighborDistance;
		stream >> neighborStrength;

	}

	void bhkLiquidAction::write(NiStream& stream) {

		rigidBodyRef->write(stream);
		stream << bhkLiquidAction_unknown1;
		stream << initiakStickForce;
		stream << stickStrength;
		stream << neighborDistance;
		stream << neighborStrength;

	}




	bhkOrientHingedBodyAction::bhkOrientHingedBodyAction() {
		entity = std::make_unique<NiReference>(this);
	}

	void bhkOrientHingedBodyAction::read(NiStream& stream) {

		entity->read(stream);
		stream >> bhkOrientHingedBodyAction_unknown1;
		stream >> bhkOrientHingedBodyAction_unknown2;
		stream >> hingeAxisLs;
		stream >> forwardLs;
		stream >> strength;
		stream >> dampling;
		stream >> bhkOrientHingedBodyAction_unknown3;

	}

	void bhkOrientHingedBodyAction::write(NiStream& stream) {

		entity->write(stream);
		stream << bhkOrientHingedBodyAction_unknown1;
		stream << bhkOrientHingedBodyAction_unknown2;
		stream << hingeAxisLs;
		stream << forwardLs;
		stream << strength;
		stream << dampling;
		stream << bhkOrientHingedBodyAction_unknown3;

	}




	void NiFogProperty::read(NiStream& stream) {

		NiProperty::read(stream);
		stream >> flags;
		stream >> depth;
		stream >> color;

	}

	void NiFogProperty::write(NiStream& stream) {

		NiProperty::write(stream);

		stream << flags;
		stream << depth;
		stream << color;

	}







































	// constrain 




	bhkConstraint::bhkConstraint() {
		enitityRefs = std::make_unique<NiReferenceArray>(this);
	}

	void bhkConstraint::read(NiStream& stream) {

		enitityRefs->read(stream);
		stream >> priority;

	}

	void bhkConstraint::write(NiStream& stream) {

		enitityRefs->write(stream, false);
		stream << priority;


	}




	void bhkRagdollConstraint::read(NiStream& stream) {

		bhkConstraint::read(stream);

		stream >> twistA;
		stream >> planeA;
		stream >> motorA;
		stream >> pivotA;
		stream >> twistB;
		stream >> planeB;
		stream >> motorB;
		stream >> pivotB;
		stream >> coneMaxAngle;
		stream >> planeMinAngle;
		stream >> planeMaxAngle;
		stream >> twistMinAngle;
		stream >> twistMaxAngle;
		stream >> maxFriction;
		motor.read(stream);

	}

	void bhkRagdollConstraint::write(NiStream& stream) {

		bhkConstraint::write(stream);

		stream << twistA;
		stream << planeA;
		stream << motorA;
		stream << pivotA;
		stream << twistB;
		stream << planeB;
		stream << motorB;
		stream << pivotB;
		stream << coneMaxAngle;
		stream << planeMinAngle;
		stream << planeMaxAngle;
		stream << twistMinAngle;
		stream << twistMaxAngle;
		stream << maxFriction;
		motor.write(stream);

	}



	void bhkPrismaticConstraint::read(NiStream& stream) {

		bhkConstraint::read(stream);

		stream >> slidingA;
		stream >> rotationA;
		stream >> planeA;
		stream >> pivotA;
		stream >> slidingB;
		stream >> rotationB;
		stream >> planeB;
		stream >> pivotB;
		stream >> minDistance;
		stream >> maxDistance;
		stream >> friction;
		motor.read(stream);

	}

	void bhkPrismaticConstraint::write(NiStream& stream) {

		bhkConstraint::write(stream);

		stream << slidingA;
		stream << rotationA;
		stream << planeA;
		stream << pivotA;
		stream << slidingB;
		stream << rotationB;
		stream << planeB;
		stream << pivotB;
		stream << minDistance;
		stream << maxDistance;
		stream << friction;
		motor.write(stream);

	}



	void bhkHingeConstraint::read(NiStream& stream) {

		bhkConstraint::read(stream);

		stream >> axisA;
		stream >> axisInA1;
		stream >> axisInA2;
		stream >> pivotA;
		stream >> axisB;
		stream >> axisInB1;
		stream >> axisInB2;
		stream >> pivotB;

	}

	void bhkHingeConstraint::write(NiStream& stream) {

		bhkConstraint::write(stream);

		stream << axisA;
		stream << axisInA1;
		stream << axisInA2;
		stream << pivotA;
		stream << axisB;
		stream << axisInB1;
		stream << axisInB2;
		stream << pivotB;

	}




	void bhkLimitedHingeConstraint::read(NiStream& stream) {

		bhkHingeConstraint::read(stream);

		stream >> minAngle;
		stream >> maxAngle;
		stream >> maxFriction;
		motor.read(stream);

	}

	void bhkLimitedHingeConstraint::write(NiStream& stream) {

		bhkHingeConstraint::write(stream);

		stream << minAngle;
		stream << maxAngle;
		stream << maxFriction;
		motor.write(stream);

	}





	void bhkBallAndSocketConstraint::read(NiStream& stream) {

		bhkConstraint::read(stream);

		stream >> pivotA;
		stream >> pivotB;

	}

	void bhkBallAndSocketConstraint::write(NiStream& stream) {

		bhkConstraint::write(stream);

		stream << pivotA;
		stream << pivotB;

	}





	void bhkStiffSpringConstraint::read(NiStream& stream) {

		bhkBallAndSocketConstraint::read(stream);

		stream >> length;

	}

	void bhkStiffSpringConstraint::write(NiStream& stream) {

		bhkBallAndSocketConstraint::write(stream);

		stream << length;

	}









	void bhkMalleableConstraint::read(NiStream& stream) {

		bhkConstraint::read(stream);

		stream >> type;

		switch (type) {

		case hkConstraintType::Ragdoll:
			constraint = std::make_unique<bhkRagdollConstraint>();
			break;

		case hkConstraintType::Prismatic:
			constraint = std::make_unique<bhkPrismaticConstraint>();
			break;

		case hkConstraintType::Hinge:
			constraint = std::make_unique<bhkHingeConstraint>();
			break;

		case hkConstraintType::LimitedHinge:
			constraint = std::make_unique<bhkLimitedHingeConstraint>();
			break;

		case hkConstraintType::BallAndSocket:
			constraint = std::make_unique<bhkBallAndSocketConstraint>();
			break;

		case hkConstraintType::StiffSpring:
			constraint = std::make_unique<bhkStiffSpringConstraint>();
			break;

		}

		if (constraint) {
			constraint->read(stream);
		}

		stream >> strength;

	}

	void bhkMalleableConstraint::write(NiStream& stream) {

		bhkConstraint::write(stream);

		stream << type;

		if (constraint) {
			constraint->write(stream);
		}

		stream << strength;

	}





	void bhkBreakableConstraint::read(NiStream& stream) {

		bhkMalleableConstraint::read(stream);

		stream >> removeWhenBroken;


	}

	void bhkBreakableConstraint::write(NiStream& stream) {

		bhkMalleableConstraint::write(stream);

		stream << removeWhenBroken;

	}

}