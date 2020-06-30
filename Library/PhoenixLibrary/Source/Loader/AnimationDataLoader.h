#pragma once

#include <fbxsdk.h>
#include "Phoenix/Graphics/Animation.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// �A�j���[�V�����f�[�^���[�_�[�N���X
		//****************************************************************************
		class AnimationDataLoader
		{
		public:
			// �A�j���[�V�����f�[�^�ǂݍ���
			bool Load(FbxScene* fbxScene, Graphics::AnimationData& data);
		};

	} // namespace Loader
} // namespace Phoenix