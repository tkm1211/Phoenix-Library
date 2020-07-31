#pragma once

#include <vector>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Animation.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/STD.h"
#include "../Source/Loader/Loader.h"


namespace Phoenix
{
	namespace FrameWork
	{
		/*class ITransform
		{
		public:
			// スケール値を設定
			virtual void SetScale(const Math::Vector3& scale) = 0;

			// 回転値を設定
			virtual void SetRotate(const Math::Quaternion& rotate) = 0;

			// 移動値を設定
			virtual void SetTranslate(const Math::Vector3& translate) = 0;

			// スケール値を取得
			virtual const Math::Vector3& GetScale() = 0;

			// 回転値を取得
			virtual const Math::Quaternion& GetRotate() = 0;

			// 移動値を取得
			virtual const Math::Vector3& GetTranslate() = 0;

			// ローカルトランスフォームを取得
			virtual const Math::Matrix& GetLocalTransform() = 0;

			// ワールドトランスフォームを取得
			virtual const Math::Matrix& GetWorldTransform() = 0;
		};

		class IObject : public ITransform
		{
		public:
			// 名前設定
			virtual void SetName(const char* name) = 0;

			// 名前取得
			virtual const char* GetName() = 0;

			// アクティブ状態を設定
			virtual void SetActive(bool active) = 0;

			// アクティブ状態を取得
			virtual bool IsActive() = 0;

			// 親ゲームオブジェクトを取得
			virtual IObject* GetParent() = 0;

			// 子ゲームオブジェクト数取得
			virtual const std::vector<IObject*>& GetChildren() = 0;

			// 子を追加できるか判定
			virtual bool CanAddChild(IObject* child) = 0;

			// 子を追加
			virtual bool AddChild(IObject* child) = 0;

			// バウンディングボックスを取得
			virtual const Math::AABB& GetBounds() = 0;
		};*/

		struct CollisionData
		{
			Phoenix::Math::Vector3 pos;
			Phoenix::f32 radius = 0.0f;
			Phoenix::s32 boneIndex = 0;
		};

		class Animator;
		class ModelObject
		{
		public:
			struct Node
			{
				const char* name;
				Node* parent;
				Math::Vector3 scale;
				Math::Quaternion rotate;
				Math::Vector3 translate;
				Math::Matrix localTransform;
				Math::Matrix worldTransform;
			};

			struct MeshNode
			{
				std::vector<Math::Matrix> boneTransform;
				u32 boneTransformCount = 0;
			};

			struct Material
			{
				std::string name;
				std::vector<std::unique_ptr<Graphics::ITexture>> textures;
				std::vector<Math::Color> colors;
			};

		private:
			std::shared_ptr<Graphics::IModelResource> modelResource;
			std::unique_ptr<Animator> animator;
			std::vector<Node> nodes;
			std::vector<MeshNode> meshNodes;
			std::vector<Material> materials;
			std::unique_ptr<OS::IResourceManager> resourceManamger;
			std::unique_ptr<OS::IFileStream> file;

			// ボーン名列挙用
			std::vector<const char*> boneNames;

		public:
			ModelObject() {}
			~ModelObject() {}

		public:
			// 生成
			static std::unique_ptr<ModelObject> Create();

			// 初期化
			void Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// モデルの読み込み
			void Load(Graphics::IGraphicsDevice* graphicsDevice, const char* filename);

			// アニメーションの読み込み
			void LoadAnimation(const char* filename, s32 index);

			// 行列を更新
			void UpdateTransform(f32 elapsedTime);

			// ローカル変換行列を更新
			void UpdateLocalTransform();

			// ワールド変換行列を更新
			void UpdateWorldTransform();

			// ボーン変換行列を更新
			void UpdateBoneTransform();

			// アニメーションの再生
			void PlayAnimation(u32 bank, u32 clip, f32 fadeTime = 0.0f);

			// 一時停止/再開
			void PauseAnimation(bool pause);

			// 停止
			void StopAnimation();

			// ループ再生設定
			void SetLoopAnimation(bool loop);

			// 再生中
			bool IsPlaying();

			// ループ再生か
			bool IsLoopAnimation();

			// アニメーションの更新
			void UpdateAnimation(f32 elapsedTime);

			// 再生時間長(秒)取得
			f32 GetLength();

			// 再生速度倍率設定
			void SetSpeed(f32 speed);

			// 再生速度倍率取得
			f32 GetSpeed();

			// ブレンド中か
			bool IsBlend();

			// モデルリソースの取得
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// ノードの取得
			std::vector<Node>* GetNodes() { return &nodes; }

			// ボーン名取得
			std::vector<const char*> GetBoneNames() { return boneNames; }

			// ボーントランスフォームの取得
			Math::Matrix* GetBoneTransforms(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransform.data(); }
			Math::Matrix GetBoneTransforms(u32 meshIndex, u32 boneIndex) { return meshNodes.at(meshIndex).boneTransform.at(boneIndex); }

			// ボーンインデックスの取得
			u32 GetBoneIndex(const char* name)
			{
				for (u32 i = 0; i < nodes.size(); ++i)
				{
					if (FND::StrCmp(nodes[i].name, name) == 0)
					{
						return i;
					}
				}

				return -1;
			}

			// メッシュノードのサイズ取得
			sizeT GetMeshNodes() { return meshNodes.size(); }

			// ボーントランスフォームのサイズ取得
			u32 GetBoneTransformCount(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransformCount; }

			// マテリアルのテクスチャ取得
			Graphics::ITexture* GetTexture(u32 index, u32 texIndex)
			{
				return materials.at(index).textures.at(texIndex).get();
			}

			// マテリアルのサイズ取得
			sizeT GetMaterialSize() { return materials.size(); }

			// マテリアルのテクスチャサイズ取得
			sizeT GetTextureSize(u32 index) { return materials.at(index).textures.size(); }
		};

		class Animator
		{
		private:
			struct Animation
			{
				std::string filename;
				std::shared_ptr<Graphics::IAnimationResource> resource;
				std::unique_ptr<Graphics::IAnimationPlayer> player;
				std::vector<s16> bindNodeIDs;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

		private:
			std::vector<Animation> animations;
			std::vector<ModelObject::Node>* nodes;
			std::unique_ptr<OS::IFileStream> file;

			Animation* currentAnimation = nullptr;

		public:
			void Initialize(ModelObject* model)
			{
				file = OS::IFileStream::Create();
				file->Initialize(nullptr);

				nodes = model->GetNodes();
			}

			// アニメーションリソース読み込み
			void LoadResource(OS::IResourceManager* resourceManamger, const char* filename, s32 index)
			{
				if (index < 0)
				{
					index = static_cast<s32>(animations.size());
					animations.emplace_back(Animation());
				}

				Animation& animation = animations.at(index);

				animation.filename = OS::Path::ChangeFileExtension(filename, "ani");

				if (OS::Path::CheckFileExtension(filename, "fbx") && !file->Exists(animation.filename.c_str()))
				{
					std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
					if (!loader->Initialize(filename))
					{
						return;
					}
					Graphics::AnimationData data;
					if (!loader->Load(data))
					{
						return;
					}
					Graphics::AnimationData::Serialize(data, animation.filename.c_str());
				}
				LoadResource(resourceManamger, animation);
			}

			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
			{
				animation.resource = resourceManamger->LoadImmediate<Graphics::IAnimationResource>(animation.filename.c_str());

				if (animation.resource)
				{
					animation.player = Graphics::IAnimationPlayer::Create();
					animation.player->Initialize(animation.resource);
					BindAnimationNodes(animation);
				}
			}

			// 再生
			void Play(u32 bank, u32 clip, f32 fadeTime = 0.0f)
			{
				if (bank < 0 || bank >= animations.size())
				{
					return;
				}

				Animation& animation = animations.at(bank);
				if (!animation.resource)
				{
					return;
				}
				const Graphics::AnimationData& data = animation.resource->GetAnimationData();

				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				currentAnimation = &animation;
				animation.player->Play(clip);
				animation.player->SetBlendTime(fadeTime);
			}

			// 一時停止/再開
			void Pause(bool pause)
			{
				currentAnimation->player->Pause(pause);
			}

			// 停止
			void Stop()
			{
				currentAnimation->player->Stop();
			}

			// ループ再生設定
			void SetLoop(bool loop)
			{
				currentAnimation->player->SetLoop(loop);
			}

			// 再生中
			bool IsPlaying()
			{
				return currentAnimation->player->IsPlaying();
			}

			// ループ再生か
			bool IsLoop()
			{
				return currentAnimation->player->IsLoop();
			}

			// 再生時間長(秒)取得
			f32 GetLength()
			{
				return currentAnimation->player->GetLength();
			}

			// 再生速度倍率設定
			void SetSpeed(f32 speed)
			{
				currentAnimation->player->SetSpeed(speed);
			}

			// 再生速度倍率取得
			f32 GetSpeed()
			{
				return currentAnimation->player->GetSpeed();
			}

			// ブレンド中か
			bool IsBlend()
			{
				return currentAnimation->player->IsBlend();
			}

			// アニメーションバンクインデックス取得
			u32 GetAnimationBankIndex(const char* name) const
			{
				for (size_t i = 0; i < animations.size(); ++i)
				{
					if (0 == FND::StrCmp(name, OS::Path::GetFileNameWithoutExtension(animations.at(i).filename.c_str())))
					{
						return static_cast<u32>(i);
					}
				}
				return -1;
			}

			void Update(f32 elapsedTime)
			{
				if (currentAnimation != nullptr)
				{
					std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = currentAnimation->player;
					animationPlayer->Update(elapsedTime);

					if (animationPlayer->IsPlaying())
					{
						s32 animationNodeCount = static_cast<s32>(nodes->size());
						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							s16 bindNodeID = currentAnimation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node = nodes->at(animationNodeID);

							Math::Vector3 scale = node.scale;
							Math::Quaternion rotate = node.rotate;
							Math::Vector3 translate = node.translate;

							animationPlayer->CalculateScale(animationNodeID, scale);
							animationPlayer->CalculateRotate(animationNodeID, rotate);
							animationPlayer->CalculateTranslate(animationNodeID, translate);

							node.scale = scale;
							node.rotate = rotate;
							node.translate = translate;
						}
					}
				}
			}

			// アニメーションノードとバインド
			void BindAnimationNodes(Animation& animation)
			{
				const Graphics::AnimationData& data = animation.resource->GetAnimationData();
				animation.bindNodeIDs.resize(data.nodeNames.size());

				for (size_t i = 0; i < data.nodeNames.size(); ++i)
				{
					animation.bindNodeIDs.at(i) = -1;

					const std::string& nodeName = data.nodeNames[i];
					for (size_t j = 0; j < nodes->size(); ++j)
					{
						ModelObject::Node& node = nodes->at(j);
						if (nodeName == node.name)
						{
							animation.bindNodeIDs.at(i) = static_cast<s16>(static_cast<s32>(j));
						}
					}
				}
			}

			// アニメーションクリップを検索
			const Graphics::AnimationData::Clip* FindAnimationClip(const char* name)
			{
				for (Animation& animation : animations)
				{
					const Graphics::AnimationData& data = animation.resource->GetAnimationData();
					for (const Graphics::AnimationData::Clip& clip : data.clips)
					{
						if (clip.name == name)
						{
							return &clip;
						}
					}
				}
				return nullptr;
			}

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	}
}