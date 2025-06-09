#pragma once

#include "objects.h"

namespace nif {

	class NiObjectFactoryTemplate {

	public:
		virtual ~NiObjectFactoryTemplate() {}
		virtual std::unique_ptr<NiObject> create() = 0;
		virtual std::unique_ptr<NiObject> create(NiStream& stream) = 0;
	};


	template<class T>
	class NiObjectFactoryTemplateImp : public NiObjectFactoryTemplate {

	public:
		virtual ~NiObjectFactoryTemplateImp() {}
		virtual std::unique_ptr<NiObject> create() override {
			return std::move(std::make_unique<T>());
		};

		virtual std::unique_ptr<NiObject> create(NiStream& stream) override {
			auto _template = std::make_unique<T>();
			_template->read(stream);
			return std::move(_template);
		};
	};


	class NiObjectFactory {

		std::unordered_map<std::string, std::shared_ptr<NiObjectFactoryTemplate>> templates;

	public:
		virtual ~NiObjectFactory() {};

		static NiObjectFactory& getInstance() {
			static NiObjectFactory instance;
			return instance;
		}

		template<class T> void registerTemplate() {
			templates.emplace(T::GET_BLOCK_NAME(), std::make_shared<NiObjectFactoryTemplateImp<T>>());
		}

		std::shared_ptr<NiObjectFactoryTemplate> getTemplate(const std::string& name) {
			auto it = templates.find(name);
			if (it != templates.end()) {
				return it->second;
			}
			return nullptr;
		}

	protected:
		NiObjectFactory() {



			registerTemplate<NiExtraData>();
			registerTemplate<NiIntegerExtraData>();
			registerTemplate<NiStringExtraData>();
			registerTemplate<BSXFlags>();


			registerTemplate<NiNode>();
			registerTemplate<BSFadeNode>();
			registerTemplate<BSMultiBoundNode>();
			registerTemplate<NiBillboardNode>();
			registerTemplate<BSOrderedNode>();


			registerTemplate<BSValueNode>();
			registerTemplate<BSDamageStage>();
			registerTemplate<BSBlastNode>();
			registerTemplate<BSDebrisNode>();




			registerTemplate<bhkNiCollisionObject>();
			registerTemplate<bhkCollisionObject>();
			registerTemplate<bhkPCollisionObject>();
			registerTemplate<bhkSPCollisionObject>();




			registerTemplate<bhkRigidBody>();
			registerTemplate<bhkRigidBodyT>();
			registerTemplate<bhkSimpleShapePhantom>();


			registerTemplate<bhkBoxShape>();
			registerTemplate<bhkMoppBvTreeShape>();

			registerTemplate<bhkSphereShape>();
			registerTemplate<bhkCapsuleShape>();

			registerTemplate<bhkTransformShape>();
			registerTemplate<bhkConvexTransformShape>();
			registerTemplate<bhkListShape>();
			registerTemplate<bhkConvexListShape>();
			registerTemplate<bhkConvexVerticesShape>();

			registerTemplate<NiAlphaProperty>();

			registerTemplate<NiStencilProperty>();

			registerTemplate<NiMaterialProperty>();

			registerTemplate<NiTexturingProperty>();
			registerTemplate<NiSourceTexture>();

			registerTemplate<BSShaderTextureSet>();

			registerTemplate<BSShaderPPLightingProperty>();
			registerTemplate<Lighting30ShaderProperty>();
			registerTemplate<BSShaderNoLightingProperty>();
			registerTemplate<TallGrassShaderProperty>();
			registerTemplate<WaterShaderProperty>();
			registerTemplate<SkyShaderProperty>();

			registerTemplate<TileShaderProperty>();

			registerTemplate<BSWaterShaderProperty>();
			registerTemplate<BSSkyShaderProperty>();





			registerTemplate<NiTriShape>();
			registerTemplate<NiTriShapeData>();

			registerTemplate<BSSegmentedTriShape>();

			registerTemplate<NiTriStrips>();
			registerTemplate<NiTriStripsData>();


			registerTemplate<bhkNiTriStripsShape>();
			registerTemplate<bhkPackedNiTriStripsShape>();
			registerTemplate<bhkPackedNiTriStripsData>();


			registerTemplate<BSMasterParticleSystem>();
			registerTemplate<NiParticleSystem>();
			registerTemplate<BSStripParticleSystem>();
			registerTemplate<NiPSysData>();
			registerTemplate<BSStripPSysData>();



			registerTemplate<NiAlphaController>();

			registerTemplate<BSEffectShaderPropertyFloatController>();
			registerTemplate<BSEffectShaderPropertyColorController>();

			registerTemplate<NiMaterialColorController>();

			registerTemplate<BSLightingShaderPropertyFloatController>();
			registerTemplate<BSLightingShaderPropertyColorController>();

			registerTemplate<NiTextureTransformController>();
			registerTemplate<BSMaterialEmittanceMultController>();
			registerTemplate<BSRefractionStrengthController>();





			registerTemplate<NiAmbientLight>();
			registerTemplate<NiPointLight>();
			registerTemplate<NiSpotLight>();



			registerTemplate<NiSequence>();
			registerTemplate<NiControllerSequence>();




			registerTemplate<NiSkinInstance>();
			registerTemplate<BSDismemberSkinInstance>();
			registerTemplate<NiSkinData>();
			registerTemplate<NiSkinPartition>();






			//---------------------------------------------- new ----------------------------------------
			registerTemplate<NiBoolInterpolator>();
			registerTemplate<NiTransformInterpolator>();
			registerTemplate<NiFloatInterpolator>();
			registerTemplate<NiPoint3Interpolator>();
			registerTemplate<NiBlendBoolInterpolator>();
			registerTemplate<NiBlendFloatInterpolator>();
			registerTemplate<NiBlendPoint3Interpolator>();
			registerTemplate<NiBoolTimelineInterpolator>();
			registerTemplate<NiPathInterpolator>();




			registerTemplate<NiVisController>();
			registerTemplate<NiTransformController>();
			registerTemplate<NiControllerManager>();
			registerTemplate<NiMultiTargetTransformController>();
			registerTemplate<NiPSysEmitterCtlr>();
			registerTemplate<NiPSysUpdateCtlr>();
			registerTemplate<bhkBlendController>();
			registerTemplate<NiFloatExtraDataController>();
			registerTemplate<NiGeomMorpherController>();
			registerTemplate<NiBSBoneLODController>();
			registerTemplate<NiPSysEmitterInitialRadiusCtlr>();
			registerTemplate<NiPSysEmitterDeclinationVarCtlr>();
			registerTemplate<NiPSysEmitterSpeedCtlr>();
			registerTemplate<NiPSysModifierActiveCtlr>();
			registerTemplate<NiLightColorController>();
			registerTemplate<BSPSysMultiTargetEmitterCtlr>();
			registerTemplate<NiLightDimmerController>();
			registerTemplate<NiPSysGravityStrengthCtlr>();
			registerTemplate<NiPSysEmitterLifeSpanCtlr>();






			registerTemplate<NiPSysRotationModifier>();
			registerTemplate<BSPSysSimpleColorModifier>();
			registerTemplate<NiPSysAgeDeathModifier>();
			registerTemplate<NiPSysSpawnModifier>();
			registerTemplate<NiPSysGrowFadeModifier>();
			registerTemplate<NiPSysGravityModifier>();
			registerTemplate<NiPSysPositionModifier>();
			registerTemplate<NiPSysBoundUpdateModifier>();
			registerTemplate<NiPSysColliderManager>();
			registerTemplate<BSParentVelocityModifier>();
			registerTemplate<NiPSysDragModifier>();
			registerTemplate<BSWindModifier>();
			registerTemplate<BSPSysStripUpdateModifier>();
			registerTemplate<NiPSysBombModifier>();





			registerTemplate<NiPSysMeshEmitter>();
			registerTemplate<NiPSysBoxEmitter>();
			registerTemplate<NiPSysCylinderEmitter>();
			registerTemplate<NiPSysSphereEmitter>();



			registerTemplate<NiDefaultAVObjectPalette>();
			registerTemplate<BSBound>();
			registerTemplate<NiFloatExtraData>();
			registerTemplate<BSWArray>();
			registerTemplate<BSDecalPlacementVectorExtraData>();
			registerTemplate<bhkBlendCollisionObject>();
			registerTemplate<NiCamera>();
			registerTemplate<BSMultiBound>();
			registerTemplate<BSMultiBoundSphere>();
			registerTemplate<NiPSysPlanarCollider>();
			registerTemplate<BSMultiBoundAABB>();
			registerTemplate<bhkLiquidAction>();
			registerTemplate<bhkOrientHingedBodyAction>();
			registerTemplate<NiFogProperty>();

			registerTemplate<bhkRagdollConstraint>();
			registerTemplate<bhkPrismaticConstraint>();
			registerTemplate<bhkHingeConstraint>();
			registerTemplate<bhkLimitedHingeConstraint>();
			registerTemplate<bhkBallAndSocketConstraint>();
			registerTemplate<bhkStiffSpringConstraint>();
			registerTemplate<bhkMalleableConstraint>();
			registerTemplate<bhkBreakableConstraint>();




		};

	};

}