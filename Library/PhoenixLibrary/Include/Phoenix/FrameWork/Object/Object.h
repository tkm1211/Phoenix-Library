#pragma once

#include <vector>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Animation.h"
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

		private:
			std::shared_ptr<Graphics::IModelResource> modelResource;
			std::unique_ptr<Animator> animator;
			std::vector<Node> nodes;
			std::unique_ptr<OS::IResourceManager> resourceManamger;
			std::unique_ptr<OS::IFileStream> file;

			std::vector<Math::Matrix> boneTransform;
			u32 boneTransformCount = 0;

		public:
			ModelObject() {}
			~ModelObject() {}

		public:
			// 初期化
			void Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// モデルの読み込み
			void Load(const char* filename);

			// アニメーションの読み込み
			void LoadAnimation(const char* filename, s32 index);

			// ローカル変換行列計算
			void CalculateLocalTransform();

			// ワールド変換行列計算
			void CalculateWorldTransform(const Math::Matrix& worldTransform);

			// アニメーションの再生
			void PlayAnimation(u32 bank, u32 clip, f32 fadeTime = 0.0f);

			// アニメーションの更新
			void UpdateAnimation(f32 elapsedTime);

			// モデルリソースの取得
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// ノードの取得
			std::vector<Node>& GetNodes() { return nodes; }

			// ボーントランスフォームの取得
			Math::Matrix* GetBoneTransforms() { return boneTransform.data(); }

			// ボーントランスフォームのサイズ取得
			u32 GetBoneTransformCount() { return boneTransformCount; }
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

				this->nodes = &model->GetNodes();
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
				// TODO : animationがうまくロード出来てない
				LoadResource(resourceManamger, animation);
			}

			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
			{
				animation.resource = resourceManamger->LoadImmediate<Graphics::IAnimationResource>(animation.filename.c_str());
				animation.player = Graphics::IAnimationPlayer::Create();
				animation.player->Initialize(animation.resource);
				BindAnimationNodes(animation);
			}

			// 再生
			void Play(u32 bank, u32 clip, f32 fadeTime = 0.0f)
			{
				if (bank < 0 || bank >= animations.size())
				{
					return;
				}

				Animation& animation = animations.at(bank);
				const Graphics::AnimationData& data = animation.resource->GetAnimationData();

				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				currentAnimation = &animation;
				animation.player->Play(clip);
				animation.player->SetBlendTime(fadeTime);
			}

			// アニメーションバンクインデックス取得
			u32 GetAnimationBankIndex(const char* name) const
			{
				for (size_t i = 0; i < animations.size(); ++i)
				{
					if (0 == FND::StrCmp(name, OS::Path::GetFileNameWithoutExtension(animations.at(i).filename.c_str())))
					{
						return i;
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
						s32 animationNodeCount = nodes->size();
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