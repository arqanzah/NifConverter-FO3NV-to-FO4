#pragma once

#include <experimental/filesystem>

#include "NiFile.h"
#include <chrono>

namespace nif {

	template<typename T>
	void convert_controller(NiFile& file, NiReferenceable* controllerRef, NiReferenceable* targetRef) {

		static_assert(std::is_same<BSLightingShaderProperty, T>::value || std::is_same<BSEffectShaderProperty, T>::value, "only unsigned [BSEffectShaderProperty and BSLightingShaderProperty] allowed");

		if (auto controller = dynamic_cast<NiInterpolatorController*>(controllerRef)) {


			if (controller->nextControllerRef->get()) {
				convert_controller<T>(file, controller->nextControllerRef->get(), targetRef);
			}

			NiInterpolatorController* newController = nullptr;

			if (dynamic_cast<NiAlphaController*>(controller)) {

				if (std::is_same<BSLightingShaderProperty, T>::value) {
					newController = new BSLightingShaderPropertyFloatController();
				} else {
					newController = new BSEffectShaderPropertyFloatController();
				}
				static_cast<FloatController*>(newController)->typeOfControlledVariable = 5;

			} else if (auto textureTransformController = dynamic_cast<NiTextureTransformController*>(controller)) {

				if (std::is_same<BSLightingShaderProperty, T>::value) {
					newController = new BSLightingShaderPropertyFloatController();
				} else {
					newController = new BSEffectShaderPropertyFloatController();
				}

				if (textureTransformController->operation == NiTextureTransformController::TransformType::TT_TRANSLATE_U) {
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 6;
				} else if (textureTransformController->operation == NiTextureTransformController::TransformType::TT_TRANSLATE_V) {
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 8;
				} else if (textureTransformController->operation == NiTextureTransformController::TransformType::TT_SCALE_U) {
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 7;
				} else if (textureTransformController->operation == NiTextureTransformController::TransformType::TT_SCALE_V) {
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 9;
				} else if (textureTransformController->operation == NiTextureTransformController::TransformType::TT_ROTATE) {
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 6; // no rotate option in FloatController
				}

			} else if (dynamic_cast<NiMaterialColorController*>(controller)) {

				if (std::is_same<BSLightingShaderProperty, T>::value) {
					newController = new BSLightingShaderPropertyColorController();
				} else {
					newController = new BSEffectShaderPropertyColorController();
				}

				static_cast<ColorController*>(newController)->typeOfControlledColor = 0;

			} else if (dynamic_cast<BSMaterialEmittanceMultController*>(controller)) {

				if (std::is_same<BSLightingShaderProperty, T>::value) {
					newController = new BSLightingShaderPropertyFloatController();
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 4;
				} else {
					newController = new BSEffectShaderPropertyFloatController();
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 0;
				}

			} else if (dynamic_cast<BSRefractionStrengthController*>(controller)) {

				if (std::is_same<BSLightingShaderProperty, T>::value) {
					newController = new BSLightingShaderPropertyFloatController();
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 0;
				} else {
					newController = new BSEffectShaderPropertyFloatController();
					static_cast<FloatController*>(newController)->typeOfControlledVariable = 0;  //BSEffectShaderPropertyFloatController does not have refraction so set to 0
				}

			}

			if (newController) {

				newController->nextControllerRef->set(controller->nextControllerRef->get());
				newController->flags = controller->flags;
				newController->frequency = controller->frequency;
				newController->phase = controller->phase;
				newController->startTime = controller->startTime;
				newController->stopTime = controller->stopTime;
				newController->targetRef->set(targetRef);
				newController->interpolatorRef->set(controller->interpolatorRef->get());

				file.replace(controllerRef->getIndex(), newController);
				//file.blocks[controllerRef] = std::unique_ptr<NiObject>(newController);
			}

		}

	}



	void convert_shape_properties(NiFile& file, NiGeometry3* shape, NiGeometry4* bsTriShape) {

		NiAlphaProperty* alpha = nullptr;
		NiMaterialProperty* material = nullptr;
		NiStencilProperty* stencil = nullptr;
		NiTexturingProperty* texturing = nullptr;
		//NiSourceTexture* sourceTexture = nullptr;


		int shaderIndex = 0xFFFFFFFF;
		NiShaderProperty* shader = nullptr;


		for (auto& ref : shape->properties->get()) {

			if (ref->get()) {

				int index = ref->get()->getIndex();


				//if (file.blocks[prop]->getBlockName() == NiAlphaProperty::GET_BLOCK_NAME()) {}
				if (auto _alpha = dynamic_cast<NiAlphaProperty*>(file.blocks[index].get())) {

					alpha = _alpha;
					bsTriShape->alphaPropertyRef->set(ref->get());
					alpha->threshold = 0;

					alpha->flags &= ~(1 << 0); //disable bleding, otherwise not showing in FO4, for example if flags is 4845, sets to 4844


				} else if (auto _material = dynamic_cast<NiMaterialProperty*>(file.blocks[index].get())) {
					material = _material;
				} else if (auto _texturing = dynamic_cast<NiTexturingProperty*>(file.blocks[index].get())) {
					texturing = _texturing;
				} else if (auto _stencil = dynamic_cast<NiStencilProperty*>(file.blocks[index].get())) {
					stencil = _stencil;
					//} else if (auto _sourceTexture = dynamic_cast<NiSourceTexture*>(file.blocks[index].get())) {
					//sourceTexture = _sourceTexture;
				} else if (auto _shader = dynamic_cast<NiShaderProperty*>(file.blocks[index].get())) {
					shader = _shader;
					shaderIndex = index;
				}

			}

		}

		if (shader) {
			if (auto shaderPPLightingProperty = dynamic_cast<BSShaderPPLightingProperty*>(shader)) {

				//std::unique_ptr<BSLightingShaderProperty> bsLightingShaderProperty = std::make_unique<BSLightingShaderProperty>();
				BSLightingShaderProperty* bsLightingShaderProperty = new BSLightingShaderProperty();

				bsLightingShaderProperty->textureSetRef->set(shaderPPLightingProperty->textureSetRef->get());

				if (shaderPPLightingProperty->shaderFlags1 & (1 << 1)) {
					bsLightingShaderProperty->shaderFlags1 |= (1 << 1); //skinned
				}


				if (shaderPPLightingProperty->shaderFlags2 & (1 << 1)) { //lod terrain

					bsLightingShaderProperty->shaderFlags1 = 2151682048;
					bsLightingShaderProperty->shaderFlags2 = 3;
					bsLightingShaderProperty->shaderType = 18;
					bsLightingShaderProperty->textureClampMode = 0;
				}

				if (shaderPPLightingProperty->shaderFlags2 & (1 << 2)) {  //lod object

					bsLightingShaderProperty->shaderFlags1 = 2151677953;
					bsLightingShaderProperty->shaderFlags2 = 5;
					bsLightingShaderProperty->shaderType = 0;
					bsLightingShaderProperty->textureClampMode = 0;
				}



				if (material) {
					bsLightingShaderProperty->specularColor = material->specularColor;
					bsLightingShaderProperty->emissiveColor = material->emissiveColor;
					bsLightingShaderProperty->smoothness = material->glossiness;
					bsLightingShaderProperty->alpha = material->alpha;
					bsLightingShaderProperty->emissiveMultiple = material->emissiveMult;

					if (material->controllerRef->get()) {
						convert_controller<BSLightingShaderProperty>(file, material->controllerRef->get(), shader);
						bsLightingShaderProperty->controllerRef->set(material->controllerRef->get());
					}

				}

				if (texturing) {
					if (texturing->controllerRef->get()) {
						convert_controller<BSLightingShaderProperty>(file, texturing->controllerRef->get(), shader);
						bsLightingShaderProperty->controllerRef->set(texturing->controllerRef->get());
					}
				}

				if (stencil) {
					bsLightingShaderProperty->shaderFlags2 |= (1 << 4); //set double sided
				}


				if (shaderPPLightingProperty->controllerRef->get()) {
					convert_controller<BSLightingShaderProperty>(file, shaderPPLightingProperty->controllerRef->get(), shader);
					bsLightingShaderProperty->controllerRef->set(shaderPPLightingProperty->controllerRef->get());
				}


				file.replace(shaderIndex, bsLightingShaderProperty);
				//file.blocks[shaderIndex] = std::move(bsLightingShaderProperty);

				bsTriShape->shaderPropertyRef->set(bsLightingShaderProperty);


			} else if (auto shaderNoLightingProperty = dynamic_cast<BSShaderNoLightingProperty*>(shader)) {

				//std::unique_ptr<BSEffectShaderProperty> bsEffectShaderProperty = std::make_unique<BSEffectShaderProperty>();
				BSEffectShaderProperty* bsEffectShaderProperty = new BSEffectShaderProperty();

				bsEffectShaderProperty->textureClampMode = shaderNoLightingProperty->textureClampMode;
				bsEffectShaderProperty->falloffStartAngle = shaderNoLightingProperty->falloffStartAngle;
				bsEffectShaderProperty->falloffStartOpacity = shaderNoLightingProperty->falloffStartOpacity;
				bsEffectShaderProperty->falloffStopAngle = shaderNoLightingProperty->falloffStopAngle;
				bsEffectShaderProperty->falloffStopOpacity = shaderNoLightingProperty->falloffStopOpacity;

				if (material) {
					bsEffectShaderProperty->emissiveColor.x = material->emissiveColor.x;
					bsEffectShaderProperty->emissiveColor.y = material->emissiveColor.y;
					bsEffectShaderProperty->emissiveColor.z = material->emissiveColor.z;
					bsEffectShaderProperty->emissiveMultiple = material->emissiveMult;

					if (material->controllerRef->get()) {
						convert_controller<BSEffectShaderProperty>(file, material->controllerRef->get(), shader);
						bsEffectShaderProperty->controllerRef->set(material->controllerRef->get());
					}

				}

				if (texturing) {
					if (texturing->controllerRef->get()) {
						convert_controller<BSEffectShaderProperty>(file, texturing->controllerRef->get(), shader);
						bsEffectShaderProperty->controllerRef->set(texturing->controllerRef->get());
					}
				}


				if (stencil) {
					bsEffectShaderProperty->shaderFlags2 |= (1 << 4); //set double sided
				}



				if (shaderNoLightingProperty->controllerRef->get()) {
					convert_controller<BSEffectShaderProperty>(file, shaderNoLightingProperty->controllerRef->get(), shader);
					bsEffectShaderProperty->controllerRef->set(shaderNoLightingProperty->controllerRef->get());
				}


				file.replace(shaderIndex, bsEffectShaderProperty);
				//file.blocks[shaderIndex] = std::move(bsEffectShaderProperty);
				bsTriShape->shaderPropertyRef->set(bsEffectShaderProperty);

			} else if (auto tallGrassShaderProperty = dynamic_cast<TallGrassShaderProperty*>(shader)) {

				//std::unique_ptr<BSLightingShaderProperty> bsLightingShaderProperty = std::make_unique<BSLightingShaderProperty>();
				BSLightingShaderProperty* bsLightingShaderProperty = new BSLightingShaderProperty();

				BSShaderTextureSet* bsShaderTextureSet = new BSShaderTextureSet();
				bsShaderTextureSet->textures[0].set(tallGrassShaderProperty->fileName.get());
				file.add(bsShaderTextureSet);
				bsLightingShaderProperty->textureSetRef->set(bsShaderTextureSet);

				if (stencil) {
					bsLightingShaderProperty->shaderFlags2 |= (1 << 4); //set double sided
				}

				file.replace(shaderIndex, bsLightingShaderProperty);
				//file.blocks[shaderIndex] = std::move(bsLightingShaderProperty);
				bsTriShape->shaderPropertyRef->set(bsLightingShaderProperty);


			} else if (auto tileShaderProperty = dynamic_cast<TileShaderProperty*>(shader)) {

				//std::unique_ptr<BSLightingShaderProperty> bsLightingShaderProperty = std::make_unique<BSLightingShaderProperty>();
				BSLightingShaderProperty* bsLightingShaderProperty = new BSLightingShaderProperty();

				BSShaderTextureSet* bsShaderTextureSet = new BSShaderTextureSet();
				bsShaderTextureSet->textures[0].set(tileShaderProperty->fileName.get());
				file.add(bsShaderTextureSet);
				bsLightingShaderProperty->textureSetRef->set(bsShaderTextureSet);

				if (stencil) {
					bsLightingShaderProperty->shaderFlags2 |= (1 << 4); //set double sided
				}

				file.replace(shaderIndex, bsLightingShaderProperty);
				//file.blocks[shaderIndex] = std::move(bsLightingShaderProperty);
				bsTriShape->shaderPropertyRef->set(bsLightingShaderProperty);


			} else if (dynamic_cast<WaterShaderProperty*>(shader)) {

				//std::unique_ptr<BSWaterShaderProperty> bsWaterShaderProperty = std::make_unique<BSWaterShaderProperty>();
				BSWaterShaderProperty* bsWaterShaderProperty = new BSWaterShaderProperty();

				file.replace(shaderIndex, bsWaterShaderProperty);
				//file.blocks[shaderIndex] = std::move(bsWaterShaderProperty);
				bsTriShape->shaderPropertyRef->set(bsWaterShaderProperty);


			} else if (auto skyShaderProperty = dynamic_cast<SkyShaderProperty*>(shader)) {

				//std::unique_ptr<BSSkyShaderProperty> bsSkyShaderProperty = std::make_unique<BSSkyShaderProperty>();
				BSSkyShaderProperty* bsSkyShaderProperty = new BSSkyShaderProperty();

				bsSkyShaderProperty->baseTexture.set(skyShaderProperty->fileName.get());

				file.replace(shaderIndex, bsSkyShaderProperty);
				//file.blocks[shaderIndex] = std::move(bsSkyShaderProperty);
				bsTriShape->shaderPropertyRef->set(bsSkyShaderProperty);


			}

		}

	}



	void convert_shapes(NiFile& file) {

		for (size_t blocIdx = 0; blocIdx < file.blocks.size(); blocIdx++) {

			if (auto shape = dynamic_cast<NiGeometry3*>(file.blocks[blocIdx].get())) {

				if (auto particleSystem = dynamic_cast<NiParticleSystem*>(shape)) {

					convert_shape_properties(file, particleSystem, particleSystem);

				} else {

					//if (auto data = dynamic_cast<NiGeometryData*>(file.blocks[shape->dataRef->get()->getIndex()].get())) {
					if (auto data = dynamic_cast<NiGeometryData*>(shape->dataRef->get())) {

						std::vector<Vector3>& vertices = data->vertices;

						std::vector<Vector3>& normals = data->normals;

						std::vector<Vector4>& vertexColors = data->vertexColors;

						std::vector<Vector2>* uvSets = nullptr;

						if (!data->uvSets.empty()) {
							uvSets = &data->uvSets[0];
						}

						std::unique_ptr<std::vector<Triangle>> triangles = data->getTriangles();

						/**/

						BSTriShape* bsTriShape = nullptr;
						auto bsSegmentShape = dynamic_cast<BSSegmentedTriShape*>(shape);

						if (bsSegmentShape) {
							bsTriShape = new BSSubIndexTriShape();
						} else {
							bsTriShape = new BSTriShape();
						}



						if (shape->nameRef != -1) {
							bsTriShape->nameRef = shape->nameRef;
						} else {

							long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
							std::string txt = bsTriShape->getBlockName() + ":" + std::to_string(ms);
							bsTriShape->nameRef = file.addString(txt);

						}

						bsTriShape->controllerRef->set(shape->controllerRef->get());
						bsTriShape->skinRef->set(shape->skinRef->get());
						bsTriShape->collisionRef->set(shape->collisionRef->get());

						for (auto extraData : shape->extraDataList->get()) {
							if (extraData->get()) {
								bsTriShape->extraDataList->add(extraData->get());
							}
						}

						bsTriShape->translation = shape->translation;
						bsTriShape->rotation = shape->rotation;
						bsTriShape->scale = shape->scale;



						convert_shape_properties(file, shape, bsTriShape);


						//----------------------------------------------

						unsigned short maxVerticesIndex = std::numeric_limits<unsigned short>().max(); // @suppress("Ambiguous problem")
						size_t verticesCount = vertices.size();
						if (verticesCount > maxVerticesIndex) {
							bsTriShape->numVertices = 0;
						} else {
							bsTriShape->numVertices = (unsigned short)(verticesCount);
						}

						unsigned int maxTrianglesIndex = std::numeric_limits<unsigned int>().max(); // @suppress("Ambiguous problem")
						size_t trianglesCount = triangles->size();
						if (trianglesCount > maxTrianglesIndex || bsTriShape->numVertices == 0) {
							bsTriShape->numTriangles = 0;
						} else {
							bsTriShape->numTriangles = (unsigned int)(trianglesCount);
						}

						if (uvSets && uvSets->size() != bsTriShape->numVertices) {
							bsTriShape->vertexDesc.setUvs(false);
						}


						bsTriShape->vertexData.resize(bsTriShape->numVertices);


						for (int i = 0; i < bsTriShape->numVertices; i++) {

							auto& vertex = bsTriShape->vertexData[i];
							vertex.vertex = vertices[i];

							if (uvSets && uvSets->size() == bsTriShape->numVertices) {
								vertex.uv = (*uvSets)[i];
							}

							vertex.bitangentX = 0.0f;
							vertex.bitangentY = 0;
							vertex.bitangentZ = 0;
							vertex.normal[0] = vertex.normal[1] = vertex.normal[2] = 0;
							std::memset(vertex.vertexColors, 255, 4);
							std::memset(vertex.boneWeights, 0, sizeof(float) * 4);
							std::memset(vertex.boneIndices, 0, 4);
							vertex.eyeData = 0.0f;

						}

						bsTriShape->triangles.resize(bsTriShape->numTriangles);
						for (unsigned int i = 0; i < bsTriShape->numTriangles; i++) {
							bsTriShape->triangles[i] = (*triangles)[i];
						}



						if (normals.size() == bsTriShape->numVertices) {

							bsTriShape->vertexDesc.setNormals(true);
							for (int i = 0; i < bsTriShape->numVertices; i++) {
								bsTriShape->vertexData[i].normal[0] = (unsigned char)round((((normals[i].x + 1.0f) / 2.0f) * 255.0f));
								bsTriShape->vertexData[i].normal[1] = (unsigned char)round((((normals[i].y + 1.0f) / 2.0f) * 255.0f));
								bsTriShape->vertexData[i].normal[2] = (unsigned char)round((((normals[i].z + 1.0f) / 2.0f) * 255.0f));
							}



							bsTriShape->vertexDesc.setTangents(true);

							//------------------------ calc tangents ----------------------

							if (bsTriShape->vertexDesc.hasNormals() && bsTriShape->vertexDesc.hasUVs()) {

								std::vector<Vector3> rawNormals;
								rawNormals.resize(bsTriShape->numVertices);
								for (int i = 0; i < bsTriShape->numVertices; i++) {
									float q1 = (((float)bsTriShape->vertexData[i].normal[0]) / 255.0f) * 2.0f - 1.0f;
									float q2 = (((float)bsTriShape->vertexData[i].normal[1]) / 255.0f) * 2.0f - 1.0f;
									float q3 = (((float)bsTriShape->vertexData[i].normal[2]) / 255.0f) * 2.0f - 1.0f;

									float x = q1;
									float y = q2;
									float z = q3;

									rawNormals[i].x = x;
									rawNormals[i].z = z;
									rawNormals[i].y = y;

								}


								std::vector<Vector3> tan1;
								std::vector<Vector3> tan2;
								tan1.resize(bsTriShape->numVertices);
								tan2.resize(bsTriShape->numVertices);

								for (std::size_t i = 0; i < bsTriShape->triangles.size(); i++) {
									int i1 = bsTriShape->triangles[i].p1;
									int i2 = bsTriShape->triangles[i].p2;
									int i3 = bsTriShape->triangles[i].p3;

									if (i1 >= bsTriShape->numVertices || i2 >= bsTriShape->numVertices || i3 >= bsTriShape->numVertices)
										continue;

									Vector3 v1 = bsTriShape->vertexData[i1].vertex;
									Vector3 v2 = bsTriShape->vertexData[i2].vertex;
									Vector3 v3 = bsTriShape->vertexData[i3].vertex;

									Vector2 w1 = bsTriShape->vertexData[i1].uv;
									Vector2 w2 = bsTriShape->vertexData[i2].uv;
									Vector2 w3 = bsTriShape->vertexData[i3].uv;

									float x1 = v2.x - v1.x;
									float x2 = v3.x - v1.x;
									float y1 = v2.y - v1.y;
									float y2 = v3.y - v1.y;
									float z1 = v2.z - v1.z;
									float z2 = v3.z - v1.z;

									float s1 = w2.u - w1.u;
									float s2 = w3.u - w1.u;
									float t1 = w2.v - w1.v;
									float t2 = w3.v - w1.v;

									float r = (s1 * t2 - s2 * t1);
									r = (r >= 0.0f ? +1.0f : -1.0f);

									Vector3 sdir = Vector3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
									Vector3 tdir = Vector3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

									sdir.normalize();
									tdir.normalize();

									tan1[i1] += tdir;
									tan1[i2] += tdir;
									tan1[i3] += tdir;

									tan2[i1] += sdir;
									tan2[i2] += sdir;
									tan2[i3] += sdir;
								}


								std::vector<Vector3> rawTangents;
								std::vector<Vector3> rawBitangents;

								rawTangents.resize(bsTriShape->numVertices);
								rawBitangents.resize(bsTriShape->numVertices);


								for (int i = 0; i < bsTriShape->numVertices; i++) {
									rawTangents[i] = tan1[i];
									rawBitangents[i] = tan2[i];

									if (rawTangents[i].isZero() || rawBitangents[i].isZero()) {
										rawTangents[i].x = rawNormals[i].y;
										rawTangents[i].y = rawNormals[i].z;
										rawTangents[i].z = rawNormals[i].x;
										rawBitangents[i] = rawNormals[i].cross(rawTangents[i]);
									} else {
										rawTangents[i].normalize();
										rawTangents[i] = (rawTangents[i] - rawNormals[i] * rawNormals[i].dot(rawTangents[i]));
										rawTangents[i].normalize();

										rawBitangents[i].normalize();

										rawBitangents[i] = (rawBitangents[i] - rawNormals[i] * rawNormals[i].dot(rawBitangents[i]));
										rawBitangents[i] = (rawBitangents[i] - rawTangents[i] * rawTangents[i].dot(rawBitangents[i]));

										rawBitangents[i].normalize();
									}

									bsTriShape->vertexData[i].tangent[0] = (unsigned char)round((((rawTangents[i].x + 1.0f) / 2.0f) * 255.0f));
									bsTriShape->vertexData[i].tangent[1] = (unsigned char)round((((rawTangents[i].y + 1.0f) / 2.0f) * 255.0f));
									bsTriShape->vertexData[i].tangent[2] = (unsigned char)round((((rawTangents[i].z + 1.0f) / 2.0f) * 255.0f));

									bsTriShape->vertexData[i].bitangentX = rawBitangents[i].x;
									bsTriShape->vertexData[i].bitangentY = (unsigned char)round((((rawBitangents[i].y + 1.0f) / 2.0f) * 255.0f));
									bsTriShape->vertexData[i].bitangentZ = (unsigned char)round((((rawBitangents[i].z + 1.0f) / 2.0f) * 255.0f));
								}

							}





							//------------------------ calc tangents ----------------------


						} else {
							bsTriShape->vertexDesc.setNormals(false);
							bsTriShape->vertexDesc.setTangents(false);
						}

						//----------------------------------------------



						//bsTriShape->flags = 14;
						bsTriShape->flags = shape->flags;

						//if (shape->skinRef == int(0xFFFFFFFF)) {
						if (shape->skinRef->get() == nullptr) {
							bsTriShape->boundingSphereCenter = data->center;
							bsTriShape->boundingSphereRadius = data->radius;
						}


						if (bsSegmentShape) {

							//process segments

						}




						if (bsTriShape->numVertices > 0) {

							bool removeColors = true;
							Vector4 blank(1.0f, 1.0f, 1.0f, 1.0f);
							for (auto& color : vertexColors) {
								if (color != blank) {
									removeColors = false;
									break;
								}
							}

							if (!removeColors && vertexColors.size() > 0) {

								if (!bsTriShape->vertexDesc.hasVertexColors()) {
									for (auto &v : bsTriShape->vertexData) {
										v.vertexColors[0] = 255;
										v.vertexColors[1] = 255;
										v.vertexColors[2] = 255;
										v.vertexColors[3] = 255;
									}
								}
								bsTriShape->vertexDesc.setVertexColors(true);

								for (int i = 0; i < bsTriShape->numVertices; i++) {
									auto& vertex = bsTriShape->vertexData[i];

									float f = std::max(0.0f, std::min(1.0f, vertexColors[i].x));
									vertex.vertexColors[0] = (unsigned char)std::floor(f == 1.0f ? 255 : f * 256.0);

									f = std::max(0.0f, std::min(1.0f, vertexColors[i].y));
									vertex.vertexColors[1] = (unsigned char)std::floor(f == 1.0f ? 255 : f * 256.0);

									f = std::max(0.0f, std::min(1.0f, vertexColors[i].z));
									vertex.vertexColors[2] = (unsigned char)std::floor(f == 1.0f ? 255 : f * 256.0);

									f = std::max(0.0f, std::min(1.0f, vertexColors[i].w));
									vertex.vertexColors[3] = (unsigned char)std::floor(f == 1.0f ? 255 : f * 256.0);
								}
							}



						}

						// ============================== skin ===============================================


						if (bsTriShape->numVertices > 0 && shape->skinRef->get()) {

							bsTriShape->vertexDesc.setSkinned(true);

							//if (auto skinInstance = dynamic_cast<NiSkinInstance*>(file.blocks[shape->skinRef->get()->getIndex()].get())) {
							if (auto skinInstance = dynamic_cast<NiSkinInstance*>(shape->skinRef->get())) {


								BSSkinInstance* bsSkinInstance = new BSSkinInstance();

								bsSkinInstance->skeletonRoot->set(skinInstance->skeletonRoot->get());

								for (auto ref : skinInstance->boneRefs->get()) {
									if (ref->get()) {
										bsSkinInstance->boneRefs->add(ref->get());
									}
								}


								if (skinInstance->dataRef->get()) {

									auto skinData = dynamic_cast<NiSkinData*>(skinInstance->dataRef->get());

									if (skinData) {
										//std::unique_ptr<BSSkinBoneData> bsSkinBoneData = std::make_unique<BSSkinBoneData>();
										BSSkinBoneData* bsSkinBoneData = new BSSkinBoneData();

										bsSkinBoneData->numBones = skinData->numBones;
										bsSkinBoneData->boneData.resize(skinData->numBones);

										for (unsigned int i = 0; i < skinData->numBones; i++) {

											bsSkinBoneData->boneData[i].center = skinData->bones[i].center;
											bsSkinBoneData->boneData[i].radius = skinData->bones[i].radius;
											bsSkinBoneData->boneData[i].boneTransform = skinData->bones[i].boneTransform;

										}

										//bsTriShape->translation = bsTriShape->translation + skinData->skinTransform.translation;
										//bsTriShape->rotation = bsTriShape->rotation + skinData->skinTransform.rotation;



										bsSkinInstance->dataRef->set(bsSkinBoneData);

										file.replace(skinInstance->dataRef->get()->getIndex(), bsSkinBoneData);
									}


								}

								if (skinInstance->skinPartitionRef->get()) {

									auto skinPartition = dynamic_cast<NiSkinPartition*>(skinInstance->skinPartitionRef->get());

									for (unsigned int i = 0; i < skinPartition->numPartitions; i++) {

										auto& partBlock = skinPartition->partitions[i];

										for (unsigned short j = 0; j < partBlock.numVertices; j++) {

											const unsigned short map = partBlock.vertexMap[j];

											if (bsTriShape->vertexData.size() > map) {


												if (partBlock.hasVertexWeights) {
													bsTriShape->vertexData[map].boneWeights[0] = partBlock.vertexWeights[j].w1;
													bsTriShape->vertexData[map].boneWeights[1] = partBlock.vertexWeights[j].w2;
													bsTriShape->vertexData[map].boneWeights[2] = partBlock.vertexWeights[j].w3;
													bsTriShape->vertexData[map].boneWeights[3] = partBlock.vertexWeights[j].w4;
												}

												if (partBlock.hasBoneIndices) {
													auto& boneIndices = partBlock.boneIndices[j];
													bsTriShape->vertexData[map].boneIndices[0] = partBlock.bones[boneIndices.i1];
													bsTriShape->vertexData[map].boneIndices[1] = partBlock.bones[boneIndices.i2];
													bsTriShape->vertexData[map].boneIndices[2] = partBlock.bones[boneIndices.i3];
													bsTriShape->vertexData[map].boneIndices[3] = partBlock.bones[boneIndices.i4];
												}
											}
										}
									}

									file.markForDeletion(skinInstance->skinPartitionRef->get());
								}


								file.replace(skinInstance->getIndex(), bsSkinInstance);

							}

						} else {
							bsTriShape->vertexDesc.setSkinned(false);
						}


						// ============================== /skin ===============================================


						//------------------------ segements ----------------------
						if (bsSegmentShape) {

							BSSubIndexTriShape* bsSubIndexTriShape = dynamic_cast<BSSubIndexTriShape*>(bsTriShape);

							bsSubIndexTriShape->numPrimitives = bsSubIndexTriShape->numTriangles;
							bsSubIndexTriShape->numSegments = bsSegmentShape->numSegments;
							bsSubIndexTriShape->totalSegments = bsSegmentShape->numSegments;

							for (auto& segement : bsSegmentShape->segments) {

								BSSubIndexTriShape::Segment triShapeSegment;
								triShapeSegment.startIndex = segement.index;
								triShapeSegment.numPrimitives = segement.numTrisInSegnent;
								bsSubIndexTriShape->segments.push_back(triShapeSegment);

							}


						}


						//------------------------ calc data ----------------------


						unsigned int vertexSize = 0;
						bsTriShape->dataSize = 0;

						VertexFlags originalFlags = bsTriShape->vertexDesc.getFlags();
						bsTriShape->vertexDesc.clearAttributeOffsets();

						unsigned int attributeSizes[VertexAttribute::VA_COUNT] = {};

						if (bsTriShape->vertexDesc.hasVertices()) {
							if (bsTriShape->vertexDesc.isFullPrecision())
								attributeSizes[VertexAttribute::VA_POSITION] = 4;
							else
								attributeSizes[VertexAttribute::VA_POSITION] = 2;
						}

						if (bsTriShape->vertexDesc.hasUVs())
							attributeSizes[VertexAttribute::VA_TEXCOORD0] = 1;

						if (bsTriShape->vertexDesc.hasUVs2())
							attributeSizes[VertexAttribute::VA_TEXCOORD1] = 1;

						if (bsTriShape->vertexDesc.hasNormals()) {
							attributeSizes[VertexAttribute::VA_NORMAL] = 1;

							if (bsTriShape->vertexDesc.hasTangents())
								attributeSizes[VertexAttribute::VA_BINORMAL] = 1;
						}

						if (bsTriShape->vertexDesc.hasVertexColors())
							attributeSizes[VertexAttribute::VA_COLOR] = 1;

						if (bsTriShape->vertexDesc.isSkinned())
							attributeSizes[VertexAttribute::VA_SKINNING] = 3;

						if (bsTriShape->vertexDesc.hasEyeData())
							attributeSizes[VertexAttribute::VA_EYEDATA] = 1;

						for (int va = 0; va < VertexAttribute::VA_COUNT; va++) {
							if (attributeSizes[va] != 0) {
								bsTriShape->vertexDesc.setAttributeOffset(VertexAttribute(va), vertexSize);
								vertexSize += attributeSizes[va] * 4;
							}
						}

						bsTriShape->vertexDesc.setSize(vertexSize);
						bsTriShape->vertexDesc.setFlags(originalFlags);



						if (dynamic_cast<BSDynamicTriShape*>(bsTriShape))
							bsTriShape->vertexDesc.setDynamic();

						bsTriShape->dataSize = vertexSize * bsTriShape->numVertices + 6 * bsTriShape->numTriangles;


						//------------------------ calc data ----------------------

						file.markForDeletion(shape->dataRef->get());

						file.replace(blocIdx, bsTriShape);
						//auto swap = std::unique_ptr<NiObject>(bsTriShape);
						//file.blocks[blocIdx].swap(swap);


					}


				}


			}
		}

	}




	void convert_collision_shape(NiFile& file, bhkShape* shape, int shapeRef, std::vector<int> &alreadyScaledShapes) {

		const float scale = 0.1f;

		bool isShapeAlreadyScaled = std::find(alreadyScaledShapes.begin(), alreadyScaledShapes.end(), shapeRef) != alreadyScaledShapes.end();

		if (auto moppBvTreeShape = dynamic_cast<bhkMoppBvTreeShape*>(shape)) {

			int childShapeRef = moppBvTreeShape->shapeRef->get()->getIndex();
			auto childShape = dynamic_cast<bhkShape*>(file.blocks[childShapeRef].get());
			convert_collision_shape(file, childShape, childShapeRef, alreadyScaledShapes);

		} else if (auto packedNiTriStripsShape = dynamic_cast<bhkPackedNiTriStripsShape*>(shape)) {


			int packedNiTriStripsDataRef = packedNiTriStripsShape->dataRef->get()->getIndex();
			auto packedNiTriStripsData = dynamic_cast<bhkPackedNiTriStripsData*>(file.blocks[packedNiTriStripsDataRef].get());

			std::vector<TriangleData> triData = packedNiTriStripsData->triangleData;
			std::vector<Vector3> vertData = packedNiTriStripsData->vertices;



			NiTriStripsData* triStripsData = new NiTriStripsData();
			triStripsData->numVertices = packedNiTriStripsData->vertices.size();


			triStripsData->hasVertices = 1;
			for (size_t i = 0; i < vertData.size(); i++) {
				Vector3 v3;
				v3.x = vertData.at(i).x * 7;
				v3.y = vertData.at(i).y * 7;
				v3.z = vertData.at(i).z * 7;
				triStripsData->vertices.push_back(v3);
			}


			//-------------------------------------------------------copy triangles--------------------------------------------------------------

			for (size_t i = 0; i < triData.size(); i++) {
				triStripsData->stripLengths.push_back(3);
				std::vector<unsigned short> points;

				points.push_back(triData.at(i).triangle.p1);
				points.push_back(triData.at(i).triangle.p2);
				points.push_back(triData.at(i).triangle.p3);

				triStripsData->points.push_back(points);
			}

			triStripsData->hasPoints = 1;
			triStripsData->numStrips = triStripsData->points.size();
			triStripsData->numTriangles = triStripsData->points.size();

			//-------------------------------------------------------copy triangles--------------------------------------------------------------


			bhkNiTriStripsShape* triStripsShape = new bhkNiTriStripsShape();
			HavokFilter f;


			// -------------- <temporary fix for reversed triangle faces of collision object> -------------------------
			// second NiTriStripsData is created, reverses faces of triangles and added to shape parts
			NiTriStripsData* collisionFixTriStripsData = new NiTriStripsData();

			collisionFixTriStripsData->groupID = triStripsData->groupID;
			collisionFixTriStripsData->numVertices = triStripsData->numVertices;
			collisionFixTriStripsData->keepFlags = triStripsData->keepFlags;
			collisionFixTriStripsData->compressFlags = triStripsData->compressFlags;
			collisionFixTriStripsData->hasVertices = triStripsData->hasVertices;
			collisionFixTriStripsData->vertices = triStripsData->vertices;
			collisionFixTriStripsData->bsVectorFlags = triStripsData->bsVectorFlags;
			collisionFixTriStripsData->hasNormals = triStripsData->hasNormals;
			collisionFixTriStripsData->normals = triStripsData->normals;
			collisionFixTriStripsData->tangents = triStripsData->tangents;
			collisionFixTriStripsData->bitangents = triStripsData->bitangents;
			collisionFixTriStripsData->center = triStripsData->center;
			collisionFixTriStripsData->radius = triStripsData->radius;
			collisionFixTriStripsData->hasVertexColors = triStripsData->hasVertexColors;
			collisionFixTriStripsData->vertexColors = triStripsData->vertexColors;
			collisionFixTriStripsData->uvSets = triStripsData->uvSets;
			collisionFixTriStripsData->consistencyFlags = triStripsData->consistencyFlags;
			collisionFixTriStripsData->materialCRC = triStripsData->materialCRC;

			collisionFixTriStripsData->numTriangles = triStripsData->numTriangles;
			collisionFixTriStripsData->numStrips = triStripsData->numStrips;
			collisionFixTriStripsData->stripLengths = triStripsData->stripLengths;
			collisionFixTriStripsData->hasPoints = triStripsData->hasPoints;
			collisionFixTriStripsData->points = triStripsData->points;


			for (std::vector<unsigned short> &tri : collisionFixTriStripsData->points) {
				unsigned short buffer = tri.at(0);
				tri.at(0) = tri.at(1);
				tri.at(1) = buffer;
			}

			file.add(collisionFixTriStripsData);
			//file.blocks.push_back(std::move(collisionFixTriStripsData));
			//int collisionFixtriStripsDataRef = file.blocks.size() - 1;

			triStripsShape->partRefs->add(collisionFixTriStripsData);
			//triStripsShape->partRefs.push_back(collisionFixtriStripsDataRef);
			//triStripsShape->numParts++;

			triStripsShape->filters.push_back(f);
			triStripsShape->numFilters++;

			// -------------- </temporary fix for reversed triangle faces of collision object> -------------------------



			//add new data to blocks
			file.add(triStripsData);
			//file.blocks.push_back(std::move(triStripsData));
			//int triStripsDataRef = file.blocks.size() - 1;

			//connect data to shape
			triStripsShape->partRefs->add(triStripsData);
			//triStripsShape->partRefs.push_back(triStripsDataRef);
			//triStripsShape->numParts++;


			triStripsShape->filters.push_back(f);
			triStripsShape->numFilters++;


			//replace shape
			file.markForDeletion(packedNiTriStripsShape->dataRef->get());
			//file.blocks[shapeRef] = std::move(triStripsShape);
			file.replace(shapeRef, triStripsShape);


		} else if (auto convexVerticesShape = dynamic_cast<bhkConvexVerticesShape*>(shape)) {

			if (!isShapeAlreadyScaled) {
				alreadyScaledShapes.push_back(shapeRef);

				for (size_t i = 0; i < convexVerticesShape->vertices.size(); i++) {
					convexVerticesShape->vertices.at(i).x *= scale;
					convexVerticesShape->vertices.at(i).y *= scale;
					convexVerticesShape->vertices.at(i).z *= scale;
				}

				for (size_t i = 0; i < convexVerticesShape->normals.size(); i++) {
					convexVerticesShape->normals.at(i).w *= scale;
				}

				convexVerticesShape->radius *= scale;

			}

		} else if (auto boxShape = dynamic_cast<bhkBoxShape*>(shape)) {

			if (!isShapeAlreadyScaled) {
				alreadyScaledShapes.push_back(shapeRef);
				boxShape->dimensions.x *= scale;
				boxShape->dimensions.y *= scale;
				boxShape->dimensions.z *= scale;

				//boxShape->radius *= scale;
			}

		} else if (auto sphereShape = dynamic_cast<bhkSphereShape*>(shape)) {

			if (!isShapeAlreadyScaled) {
				alreadyScaledShapes.push_back(shapeRef);
				sphereShape->radius *= scale;
			}

		} else if (auto capsuleShape = dynamic_cast<bhkCapsuleShape*>(shape)) {

			if (!isShapeAlreadyScaled) {
				alreadyScaledShapes.push_back(shapeRef);
				capsuleShape->radius *= scale;
				capsuleShape->radius1 *= scale;
				capsuleShape->radius2 *= scale;
				capsuleShape->point1.x *= scale;
				capsuleShape->point1.y *= scale;
				capsuleShape->point1.z *= scale;
				capsuleShape->point2.x *= scale;
				capsuleShape->point2.y *= scale;
				capsuleShape->point2.z *= scale;
			}

		} else if (auto transformShape = dynamic_cast<bhkTransformShape*>(shape)) {

			// NEEDS Attention!!!!!!!!!!!!!!  implement corectly

			Vector3 dim;
			transformShape->transform.getRow(3, dim);
			dim.x *= scale;
			dim.y *= scale;
			dim.z *= scale;
			transformShape->transform.setRow(3, dim);

			//process subshape
			int childShapeRef = transformShape->shapeRef->get()->getIndex();
			auto childShape = dynamic_cast<bhkShape*>(file.blocks[childShapeRef].get());
			convert_collision_shape(file, childShape, childShapeRef, alreadyScaledShapes);

		} else if (auto listShape = dynamic_cast<bhkListShape*>(shape)) {

			for (size_t i = 0; i < listShape->subShapeRefs->get().size(); i++) {
				int childShapeRef = listShape->subShapeRefs->get()[i]->get()->getIndex();
				auto childShape = dynamic_cast<bhkShape*>(file.blocks[childShapeRef].get());
				convert_collision_shape(file, childShape, childShapeRef, alreadyScaledShapes);
			}

		} else if (auto convexListShape = dynamic_cast<bhkConvexListShape*>(shape)) {

			for (size_t i = 0; i < convexListShape->shapeRefs->get().size(); i++) {
				int childShapeRef = convexListShape->shapeRefs->get()[i]->get()->getIndex();
				auto childShape = dynamic_cast<bhkShape*>(file.blocks[childShapeRef].get());
				convert_collision_shape(file, childShape, childShapeRef, alreadyScaledShapes);
			}

			bhkListShape* newListShape = new bhkListShape();

			for (auto ref : convexListShape->shapeRefs->get()) {
				if (ref->get()) {
					newListShape->subShapeRefs->add(ref->get());
				}
			}


			newListShape->material = convexListShape->material;
			newListShape->childShapeProperty = convexListShape->childShapeProp;

			file.replace(shapeRef, newListShape);
			//auto swap = std::unique_ptr<NiObject>(newListShape);
			//file.blocks[shapeRef].swap(swap);

		}

	}


	void convert_collision(NiFile& file) {

		const float scale = 0.1f;
		std::vector<int> alreadyScaledShapes;

		for (size_t blocIdx = 0; blocIdx < file.blocks.size(); blocIdx++) {

			if (auto node = dynamic_cast<NiNode*>(file.blocks[blocIdx].get())) {


				if (node->collisionRef->get()) {
					if (auto collision = dynamic_cast<bhkNiCollisionObject*>(file.blocks[node->collisionRef->get()->getIndex()].get())) {


						int bodyRef = collision->bodyRef->get()->getIndex();
						if (auto body = dynamic_cast<bhkRigidBody*>(file.blocks[bodyRef].get())) {


							int shapeRef = body->shapeRef->get()->getIndex();
							auto shape = dynamic_cast<bhkShape*>(file.blocks[shapeRef].get());

							convert_collision_shape(file, shape, shapeRef, alreadyScaledShapes);

							body->translation.x = body->translation.x * scale;
							body->translation.y = body->translation.y * scale;
							body->translation.z = body->translation.z * scale;

							body->center.x = body->center.x * scale;
							body->center.y = body->center.y * scale;
							body->center.z = body->center.z * scale;



							std::fill(body->bhkWorldObject_unknown1, body->bhkWorldObject_unknown1 + 4, 0);
							std::fill(body->bhkWorldObject_unknown2, body->bhkWorldObject_unknown2 + 3, 0);

							body->bhkRigidBody_unknown1 = 0;
							std::fill(body->bhkRigidBody_unknown2, body->bhkRigidBody_unknown2 + 4, 0);
							std::fill(body->bhkRigidBody_unknown3, body->bhkRigidBody_unknown3 + 4, 0);
							body->bhkRigidBody_unknown4 = 0;
							std::fill(body->bhkRigidBody_unknown5, body->bhkRigidBody_unknown5 + 4, 0);
							std::fill(body->bhkRigidBody_unknown6, body->bhkRigidBody_unknown6 + 12, 0);


							/*if (newRigidBody->collisionFilter.layer == 1 || body->motionSystem == 7) { //static
							newRigidBody->motionSystem = 0;

							} else if (newRigidBody->collisionFilter.layer == 2 && body->motionSystem == 6) { //animated static: doors, dumster
							newRigidBody->motionSystem = 4;
							} else {
							newRigidBody->motionSystem = body->motionSystem;
							}*/

							if (body->motionSystem == 7) { //motionSystem fixed: buildings, etc
								body->motionSystem = 0;
							} else if (body->motionSystem == 6) { //motionSystem keyframed: doors, dumster
								body->motionSystem = 4;
							} else {
								//body->motionSystem = body->motionSystem;
							}

							if (body->deactivatorType != 0) {
								body->enableDeactivation = true;
							}




							/*

							std::unique_ptr<bhkRigidBody> newRigidBody = nullptr;

							if (dynamic_cast<bhkRigidBodyT*>(body)) {
							newRigidBody = std::make_unique<bhkRigidBodyT>();
							}
							else {
							newRigidBody = std::make_unique<bhkRigidBody>();
							}


							newRigidBody->rotation = body->rotation;

							newRigidBody->translation.x = body->translation.x * scale;
							newRigidBody->translation.y = body->translation.y * scale;
							newRigidBody->translation.z = body->translation.z * scale;

							newRigidBody->center.x = body->center.x * scale;
							newRigidBody->center.y = body->center.y * scale;
							newRigidBody->center.z = body->center.z * scale;


							newRigidBody->shapeRef = body->shapeRef;


							newRigidBody->filter = body->filter;
							newRigidBody->broadPhaseType = body->broadPhaseType;
							newRigidBody->cInfoProperty = body->cInfoProperty;

							newRigidBody->collisionResponse = body->collisionResponse;
							newRigidBody->processContactCallbackDelay = body->processContactCallbackDelay;
							newRigidBody->filterCopy = body->filterCopy;

							newRigidBody->linearVelocity = body->linearVelocity;
							newRigidBody->angularVelocity = body->angularVelocity;
							newRigidBody->mass = body->mass;
							newRigidBody->linearDamping = body->linearDamping;
							newRigidBody->angularDamping = body->angularDamping;
							newRigidBody->timeFactor = body->timeFactor;
							newRigidBody->gravityFactor = body->gravityFactor;
							newRigidBody->friction = body->friction;

							newRigidBody->rollingFrictionMult = body->rollingFrictionMult;
							newRigidBody->restitution = body->restitution;
							newRigidBody->maxLinearVelocity = body->maxLinearVelocity;
							newRigidBody->maxAngularVelocity = body->maxAngularVelocity;
							newRigidBody->penetrationDepth = body->penetrationDepth;

							//if (newRigidBody->collisionFilter.layer == 1 || body->motionSystem == 7) { //static
							//	newRigidBody->motionSystem = 0;

							//} else if (newRigidBody->collisionFilter.layer == 2 && body->motionSystem == 6) { //animated static: doors, dumster
							//	newRigidBody->motionSystem = 4;
							//} else {
							//	newRigidBody->motionSystem = body->motionSystem;
							///

							if (body->motionSystem == 7) { //motionSystem fixed: buildings, etc
							newRigidBody->motionSystem = 0;
							}
							else if (body->motionSystem == 6) { //motionSystem keyframed: doors, dumster
							newRigidBody->motionSystem = 4;
							}
							else {
							newRigidBody->motionSystem = body->motionSystem;
							}

							newRigidBody->deactivatorType = body->deactivatorType;
							newRigidBody->solverDeactivation = body->solverDeactivation;
							newRigidBody->qualityType = body->qualityType;
							//newRigidBody->autoRemoveLevel = body->autoRemoveLevel;
							//newRigidBody->responseModifierFlag = body->responseModifierFlag;
							//newRigidBody->numShapeKeysInContactPointProps = body->numShapeKeysInContactPointProps;
							//newRigidBody->forceCollideOntoPpu = body->forceCollideOntoPpu;

							newRigidBody->numConstraints = body->numConstraints;
							newRigidBody->constraints = body->constraints;

							if (newRigidBody->deactivatorType != 0) {
							newRigidBody->enableDeactivation = true;
							}


							file.deleteBlock(bodyRef);


							file.blocks.push_back(std::move(newRigidBody));
							int newRigidBodyRef = file.blocks.size() - 1;

							collision->bodyRef = newRigidBodyRef;
							*/

						} else if (auto simpleShapePhantom = dynamic_cast<bhkSimpleShapePhantom*>(file.blocks[bodyRef].get())) {

							int childShapeRef = simpleShapePhantom->shapeRef->get()->getIndex();
							auto childShape = dynamic_cast<bhkShape*>(file.blocks[childShapeRef].get());
							convert_collision_shape(file, childShape, childShapeRef, alreadyScaledShapes);

						}


					}
				}
			}

		}


	}



	template<typename T_CONTROLLER, typename T_PROPERTY>
	void fix_controller_sequences_controller_block(NiFile& file, NiSequence::ControllerBlock* controlledBlock) {

		if (dynamic_cast<T_CONTROLLER*>(controlledBlock->controllerRef->get())) {


			if (file.strings[controlledBlock->controllerType].get() != T_CONTROLLER::GET_BLOCK_NAME()) {

				std::vector<NiString<unsigned int>>::iterator controllerTypeIt = std::find_if(file.strings.begin(), file.strings.end(), [&](const NiString<unsigned int>& str) {
					return str.get() == T_CONTROLLER::GET_BLOCK_NAME();
				});

				if (controllerTypeIt == file.strings.end()) {
					controlledBlock->controllerType = file.addString(T_CONTROLLER::GET_BLOCK_NAME());
				} else {
					controlledBlock->controllerType = std::distance(file.strings.begin(), controllerTypeIt);
				}

				std::vector<NiString<unsigned int>>::iterator propertyTypeIt = std::find_if(file.strings.begin(), file.strings.end(), [&](const NiString<unsigned int>& str) {
					return str.get() == T_PROPERTY::GET_BLOCK_NAME();
				});

				if (propertyTypeIt == file.strings.end()) {
					controlledBlock->propType = file.addString(T_PROPERTY::GET_BLOCK_NAME());
				} else {
					controlledBlock->propType = std::distance(file.strings.begin(), propertyTypeIt);
				}

			}

		}

	}


	void fix_controller_sequences(NiFile& file) {

		for (size_t blocIdx = 0; blocIdx < file.blocks.size(); blocIdx++) {
			if (auto sequence = dynamic_cast<NiControllerSequence*>(file.blocks[blocIdx].get())) {
				for (auto& controlledBlock : sequence->controlledBlocks) {

					fix_controller_sequences_controller_block<BSLightingShaderPropertyFloatController, BSLightingShaderProperty>(file, controlledBlock.get());
					fix_controller_sequences_controller_block<BSLightingShaderPropertyColorController, BSLightingShaderProperty>(file, controlledBlock.get());

					fix_controller_sequences_controller_block<BSEffectShaderPropertyFloatController, BSEffectShaderProperty>(file, controlledBlock.get());
					fix_controller_sequences_controller_block<BSEffectShaderPropertyColorController, BSEffectShaderProperty>(file, controlledBlock.get());

				}
			}
		}

	}









	void clear(NiFile& file) {

		for (size_t i = 0; i < file.blocks.size(); i++) {


			if (file.blocks[i]->getBlockName() == NiTexturingProperty::GET_BLOCK_NAME()) {
				file.markForDeletion(file.blocks[i].get());
				continue;
			}

			if (file.blocks[i]->getBlockName() == NiMaterialProperty::GET_BLOCK_NAME()) {
				file.markForDeletion(file.blocks[i].get());
				continue;
			}

			if (file.blocks[i]->getBlockName() == NiStencilProperty::GET_BLOCK_NAME()) {
				file.markForDeletion(file.blocks[i].get());
				continue;
			}

			if (file.blocks[i]->getBlockName() == NiSourceTexture::GET_BLOCK_NAME()) {
				file.markForDeletion(file.blocks[i].get());
				continue;
			}

			if (file.blocks[i]->getBlockName() == "BSFurnitureMarker") {
				file.markForDeletion(file.blocks[i].get());
				continue;
			}

			if (file.blocks[i]->getBlockName() == BSDecalPlacementVectorExtraData::GET_BLOCK_NAME()) {
				file.markForDeletion(file.blocks[i].get());
				continue;
			}


		}

		if (NiNode* rootNode = file.getRootNode()) {
			for (auto del : file.blocksToDelete) {

				file.remove(del->getIndex());

				//NiExtraData* extraData = new NiExtraData();
				//file.replace(del->getIndex(), extraData);
				//rootNode->extraDataList->add(extraData);
			}
			file.blocksToDelete.clear();
		}


	}

	
	



	void processNif(const char* iFilename, const char* oFilename, std::unordered_map<std::string, std::string>& names) {



		NiFile nifFile(iFilename, names);
		nifFile.userVersion = 12;
		nifFile.userVersion2 = 130;


		convert_shapes(nifFile);
		fix_controller_sequences(nifFile);
		convert_collision(nifFile);

		//generate_material_files(nifFile, iFilename);

		clear(nifFile);

		
		nifFile.save(oFilename);



	}



	void processFolders(const char* in, const char* out, std::unordered_map<std::string, std::string>& names) {

		for (auto& p : std::experimental::filesystem::directory_iterator(in)) {
			if (std::experimental::filesystem::is_directory(p.status())) {

				std::string _out(out);
				_out.append("\\");
				_out.append(p.path().filename().u8string().c_str());
				std::experimental::filesystem::create_directories(_out);

				processFolders(p.path().u8string().c_str(), _out.c_str(), names);

			} else if (std::experimental::filesystem::is_regular_file(p.status()) && p.path().extension() == ".nif") {


				std::string _out(out);
				_out.append("\\");
				_out.append(p.path().filename().u8string().c_str());

				std::cout << "Converting: " << p.path().u8string().c_str() << std::endl;


				try {
					processNif(p.path().u8string().c_str(), _out.c_str(), names);
				} catch (...) {
					std::cout << "Failed to convert: " << p.path().u8string().c_str() << std::endl;
					std::ofstream file("nif_converter_errors.txt", std::ios_base::app);
					file << "Failed to convert: " << p.path().u8string().c_str() << std::endl;
					file.close();
				}

			}
		}


	}

}














/*

void convert_lod_terrain(NiFile& file) {

BSMultiBoundNode* chunkNode = dynamic_cast<BSMultiBoundNode*>(file.blocks[0].get());
BSMultiBoundNode* waterNode = nullptr;
BSMultiBound* waterBSMultiBound = nullptr;
BSMultiBoundAABB* waterBSMultiBoundAABB = nullptr;
std::vector<BSEffectShaderProperty*> waterShaders;

for (size_t i = 0; i < file.blocks.size(); i++) {

//chunk
if (auto node = dynamic_cast<BSMultiBoundNode*>(file.blocks[i].get())) {

if (node->nameRef == -1) {
node->nameRef = file.addString("chunk");
} else {
file.changeString(node->nameRef, "chunk");
}

node->translation = Vector3();


}

//land




if (auto shape = dynamic_cast<BSTriShape*>(file.blocks[i].get())) {

shape->translation = Vector3();

if (shape->shaderPropertyRef->get()) {

if (shape->nameRef == -1) {
shape->nameRef = file.addString("Land");
} else {
file.changeString(shape->nameRef, "Land");
}

} else {

std::vector<NiReferenceInterface*> temp;
for (auto ref : shape->getReferencedBy()) {
temp.push_back(ref);
}

for (auto ref : temp) {
ref->clear();
}


if (!waterNode) {

waterBSMultiBoundAABB = new BSMultiBoundAABB();

waterBSMultiBound = new BSMultiBound();
waterBSMultiBound->dataRef->set(waterBSMultiBoundAABB);

waterNode = new BSMultiBoundNode();
waterNode->multiBoundRef->set(waterBSMultiBound);

chunkNode->children->add(waterNode);

}

waterNode->children->add(shape);

BSEffectShaderProperty* shader = new BSEffectShaderProperty();
shader->shaderFlags1 = 0x80000000;
shader->shaderFlags2 = 0x00000001;
waterShaders.push_back(shader);

shape->shaderPropertyRef->set(shader);

}

}





if (auto aabb = dynamic_cast<BSMultiBoundAABB*>(file.blocks[i].get())) {

aabb->position.x = aabb->extent.x;
aabb->position.y = aabb->extent.y;

}





}



if (waterNode) {

for (auto shader : waterShaders) {
file.add(shader);
}

file.add(waterBSMultiBoundAABB);
file.add(waterBSMultiBound);
file.add(waterNode);
waterNode->nameRef = file.addString("WATER");

}

//calculate bound for Water
for (auto ref : waterNode->children.get()->get()) {
if (ref->get()) {
if (auto shape = dynamic_cast<BSTriShape*>(ref->get())) {

}
}
}


}



// <terrain>
convert_lod_terrain(nifFile);

std::string fileName = std::experimental::filesystem::path(oFilename).filename().u8string().c_str();

auto lvlPosStart = fileName.find("level", 0) + 5;
auto xPosStart = fileName.find("x", lvlPosStart) + 1;
auto yPosStart = fileName.find("y", xPosStart) + 1;
auto end = fileName.find(".nif", yPosStart) + 1;

std::string s("xxxWasteland.");
s = s + fileName.substr(lvlPosStart, xPosStart - 2 - lvlPosStart) + ".";
s = s + fileName.substr(xPosStart, yPosStart - 2 - xPosStart) + ".";
s = s + fileName.substr(yPosStart, end - 1 - yPosStart) + ".btr";


std::string path = std::experimental::filesystem::path(oFilename).parent_path().u8string().c_str();

path = path + "\\" + s;

nifFile.save(path);
// </terrain>


*/









/*
void calcLodCoordiate(const int& x, const int& y, const int& level, int& resultX, int& resultY) {

const int dumper = 32768;

resultX = ((x + dumper) - ((x + dumper) % level)) - dumper;
resultY = ((y + dumper) - ((y + dumper) % level)) - dumper;
}


void convert_lod_objects(NiFile& file) {

BSMultiBoundNode* root = dynamic_cast<BSMultiBoundNode*>(file.blocks[0].get());
NiNode* newRoot = new NiNode();

newRoot->children.get()->add(root);

file.insert(0, newRoot);





}


// <terrain 0bj>

convert_lod_objects(nifFile);

std::string fileName = std::experimental::filesystem::path(oFilename).filename().u8string().c_str();

auto lvlPosStart = fileName.find("level", 0) + 5;
auto xPosStart = fileName.find("x", lvlPosStart) + 1;
auto yPosStart = fileName.find("y", xPosStart) + 1;
auto end = fileName.find(".nif", yPosStart) + 1;


int level = 4;
std::string levelStr(fileName.substr(lvlPosStart, xPosStart - 2 - lvlPosStart));
if (levelStr == "4.high") {
level = 8;
}

std::string inXStr(fileName.substr(xPosStart, yPosStart - 2 - xPosStart));
int inX = stoi(inXStr);

std::string inYStr(fileName.substr(yPosStart, end - 1 - yPosStart));
int inY = stoi(inYStr);

int outX, outY = 0;
calcLodCoordiate(inX, inY, level, outX, outY);

std::string s("xxxWasteland.");
s = s + std::to_string(level) + ".";
s = s + std::to_string(outX) + ".";
s = s + std::to_string(outY) + ".bto";


std::string path = std::experimental::filesystem::path(oFilename).parent_path().u8string().c_str();

path = path + "\\" + s;



nifFile.save(path);
// </terrain 0bj>

*/