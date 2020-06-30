#pragma once

#include <vector>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Animation.h"
#include "Phoenix/OS/Path.h"
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

		class Animator
		{
		private:
			struct Animation
			{
				std::string filename;
				std::shared_ptr<Graphics::IAnimationResource> resource;
				std::unique_ptr<Graphics::IAnimationPlayer> player;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

		private:
			std::vector<Animation> animations;
			std::unique_ptr<OS::IFileStream> file;

		public:
			void Initialize()
			{
				file = OS::IFileStream::Create();
				file->Initialize(nullptr);
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
				animation.resource = resourceManamger->LoadAsync<Graphics::IAnimationResource>(animation.filename.c_str());
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

		public:
			ModelObject() {}
			~ModelObject() {}

		public:
			// 初期化
			void Initialize(Graphics::IGraphicsDevice* graphicsDevice)
			{
				resourceManamger = Phoenix::OS::IResourceManager::Create();
				resourceManamger->Initialize(nullptr);
				resourceManamger->RegisterFactory("ani", Phoenix::Graphics::IAnimationResourceFactory::Create());
				resourceManamger->RegisterFactory("mdl", Phoenix::Graphics::IModelResourceFactory::Create(graphicsDevice->GetDevice()));

				file = OS::IFileStream::Create();
				file->Initialize(nullptr);
			}

			// モデルの読み込み
			void Load(const char* filename)
			{
				std::string modelFilename;
				modelFilename = OS::Path::ChangeFileExtension(filename, "mdl");

				if (OS::Path::CheckFileExtension(filename, "fbx") && !file->Exists(modelFilename.c_str()))
				{
					std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
					if (!loader->Initialize(filename))
					{
						return;
					}
					Graphics::ModelData data;
					if (!loader->Load(data))
					{
						return;
					}
					Graphics::ModelData::Serialize(data, modelFilename.c_str());
				}

				modelResource = resourceManamger->LoadImmediate<Graphics::IModelResource>(modelFilename.c_str());
				if (modelResource == nullptr)
				{
					return;
				}

				animator = std::make_unique<Animator>();
				animator->Initialize();
				LoadAnimation(filename, -1);

				const std::vector<Graphics::ModelData::Node>& resourceNodes = modelResource->GetModelData().nodes;
				nodes.resize(resourceNodes.size());

				for (size_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++)
				{
					auto&& src = resourceNodes.at(nodeIndex);
					auto&& dst = nodes.at(nodeIndex);

					dst.name = src.name.c_str();
					dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
					dst.scale = src.scale;
					dst.rotate = src.rotate;
					dst.translate = src.translate;
				}
			}

			// アニメーションの読み込み
			void LoadAnimation(const char* filename, s32 index)
			{
				animator->LoadResource(resourceManamger.get(), filename, index);
			}

			// ローカル変換行列計算
			void CalculateLocalTransform()
			{
				for (Node& node : nodes)
				{
					Math::Matrix scale, rotate, translate;
					scale = Math::MatrixScaling(node.scale.x, node.scale.y, node.scale.z);
					rotate = Math::MatrixRotationQuaternion(&Math::Quaternion(node.rotate.x, node.rotate.y, node.rotate.z, node.rotate.w));
					translate = Math::MatrixTranslation(node.translate.x, node.translate.y, node.translate.z);

					node.localTransform = scale * rotate * translate;
				}
			}

			// ワールド変換行列計算
			void CalculateWorldTransform(const Math::Matrix& worldTransform)
			{
				for (Node& node : nodes)
				{
					if (node.parent != nullptr)
					{
						node.worldTransform = node.localTransform * node.parent->worldTransform;
					}
					else
					{
						node.worldTransform = node.localTransform * worldTransform;
					}
				}
			}

			// モデルリソースの取得
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// ノードの取得
			const std::vector<Node>& GetNodes() { return nodes; }
		};
	}
}