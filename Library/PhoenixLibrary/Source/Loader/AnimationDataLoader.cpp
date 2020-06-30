#include "pch.h"
#include <functional>
#include "Utility.h"
#include "AnimationDataLoader.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// �A�j���[�V�����f�[�^���[�_�[
		//****************************************************************************
		// �A�j���[�V�����f�[�^�ǂݍ���
		bool AnimationDataLoader::Load(FbxScene* fbxScene, Graphics::AnimationData& data)
		{
			// ���ׂẴA�j���[�V���������擾
			FbxArray<FbxString*> fbxAnimStackNames;
			fbxScene->FillAnimStackNameArray(fbxAnimStackNames);

			int fbxAnimationCount = fbxAnimStackNames.Size();
			size_t startAnimation = data.clips.size();
			data.clips.resize(startAnimation + fbxAnimationCount);

			// �A�j���[�V�����̑ΏۂƂȂ�m�[�h��񋓂���
			std::vector<FbxNode*> fbxNodes;
			std::function<void(FbxNode*)> traverse = [&](FbxNode* fbxNode)
			{
				FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
				FbxNodeAttribute::EType fbxNodeAttributeType = FbxNodeAttribute::EType::eUnknown;
				if (fbxNodeAttribute != nullptr)
				{
					fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
				}

				switch (fbxNodeAttributeType)
				{
				case FbxNodeAttribute::eUnknown:
				case FbxNodeAttribute::eNull:
				case FbxNodeAttribute::eSkeleton:
				case FbxNodeAttribute::eMesh:
					fbxNodes.emplace_back(fbxNode);
					data.nodeNames.emplace_back(fbxNode->GetName());
					break;
				}

				for (int i = 0; i < fbxNode->GetChildCount(); i++)
				{
					traverse(fbxNode->GetChild(i));
				}
			};
			traverse(fbxScene->GetRootNode());

			// �A�j���[�V�����N���b�v�f�[�^�̎擾
			for (int fbxAnimationIndex = 0; fbxAnimationIndex < fbxAnimationCount; ++fbxAnimationIndex)
			{
				FbxTime::EMode fbxTimeMode = fbxScene->GetGlobalSettings().GetTimeMode();
				FbxTime fbxFrameTime;
				fbxFrameTime.SetTime(0, 0, 0, 1, 0, fbxTimeMode);

				u32 samplingRate = static_cast<u32>(fbxFrameTime.GetFrameRate(fbxTimeMode));
				f32 samplingTime = 1.0f / samplingRate;

				FbxString* fbxAnimStackName = fbxAnimStackNames.GetAt(fbxAnimationIndex);
				FbxAnimStack* fbxAnimStack = fbxScene->FindMember<FbxAnimStack>(fbxAnimStackName->Buffer());

				// �Đ�����A�j���[�V�������w�肷��B
				fbxScene->SetCurrentAnimationStack(fbxAnimStack);

				// �A�j���[�V�����̍Đ��J�n���ԂƍĐ��I�����Ԃ��擾����
				FbxTakeInfo* fbxTakeInfo = fbxScene->GetTakeInfo(fbxAnimStackName->Buffer());
				FbxTime fbxStartTime = 0;// fbxTakeInfo->mLocalTimeSpan.GetStart();
				FbxTime fbxEndTime = fbxTakeInfo->mLocalTimeSpan.GetStop();

				// ���o����f�[�^��60�t���[����ŃT���v�����O����
				FbxTime fbxSamplingStep;
				fbxSamplingStep.SetTime(0, 0, 1, 0, 0, fbxTimeMode);
				fbxSamplingStep = static_cast<FbxLongLong>(fbxSamplingStep.Get() * samplingTime);

				int startFrame = static_cast<int>(fbxStartTime.Get() / fbxSamplingStep.Get());
				int endFrame = static_cast<int>(fbxEndTime.Get() / fbxSamplingStep.Get());
				int frameCount = static_cast<int>((fbxEndTime.Get() - fbxStartTime.Get()) / fbxSamplingStep.Get());


				// �m�[�h���̃A�j���[�V�����f�[�^�𒊏o����
				Graphics::AnimationData::Clip& clip = data.clips.at(startAnimation + fbxAnimationIndex);
				clip.name = fbxAnimStackName->Buffer();
				clip.secondsLength = frameCount * samplingTime;
				clip.frameLength = frameCount;

				size_t nodeCount = fbxNodes.size();
				clip.nodes.resize(nodeCount);
				for (size_t i = 0; i < nodeCount; ++i)
				{
					FbxNode* fbxNode = fbxNodes[i];
					Graphics::AnimationData::Node& node = clip.nodes[i];

					//node.name = fbxNode->GetName();

					f32 seconds = 0.0f;
					u32 keyIndex = 0;
					// �S�t���[���̎p���f�[�^�����o���B
					node.scaleKeys.resize(frameCount + 1);
					node.rotateKeys.resize(frameCount + 1);
					node.translateKeys.resize(frameCount + 1);
					for (FbxTime fbxCurrentTime = fbxStartTime; fbxCurrentTime < fbxEndTime; fbxCurrentTime += fbxSamplingStep)
					{
						const FbxAMatrix& fbxLocalTransform = fbxNode->EvaluateLocalTransform(fbxCurrentTime);

						node.scaleKeys[keyIndex].seconds = seconds;
						node.rotateKeys[keyIndex].seconds = seconds;
						node.translateKeys[keyIndex].seconds = seconds;
						node.scaleKeys[keyIndex].value = Utility::FbxDoble4ToVector3(fbxLocalTransform.GetS());
						node.rotateKeys[keyIndex].value = Utility::FbxQuaternionToQuaternion(fbxLocalTransform.GetQ());
						node.translateKeys[keyIndex].value = Utility::FbxDoble4ToVector3(fbxLocalTransform.GetT());

						++keyIndex;
						seconds += samplingTime;
					}
				}
			}

			// ��n��
			for (int i = 0; i < fbxAnimationCount; i++)
			{
				delete fbxAnimStackNames[i];
			}

			return true;
		}

	}	// namespace fbx
}	// namespace ys