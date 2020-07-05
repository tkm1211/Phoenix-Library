#pragma once

#include <set>
#include "Phoenix/Graphics/Light.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ���C�e�B���O�X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		class LightingState : public ILightingState
		{
		private:
			DirLight defaultDirLight;
			std::set<DirLight*> dirLights;
			std::set<BoxLight*> boxLights;
			std::set<PointLight*> pointLights;

			DirLight noDirLight;
			BoxLight noBoxLight;
			PointLight noPointLight;

		public:
			LightingState();
			~LightingState() override {}

		public:
			// �S�Ẵ��C�g�o�^����
			void ClearLight() override;

			// �f�B���N�V���i�����C�g�o�^
			void RegisterDirLight(DirLight* light) override;

			// �f�B���N�V���i�����C�g�o�^����
			void UnregisterDirLight(DirLight* light) override;

			// ���C���f�B���N�V���i�����C�g�擾
			DirLight* GetProminentDirLight() override;

			// �f�t�H���g�f�B���N�V���i�����C�g�擾
			DirLight* GetDefaultDirLight() override;

			// �{�b�N�X���C�g�o�^
			void RegisterBoxLight(BoxLight* light) override;

			// �{�b�N�X���C�g�o�^����
			void UnregisterBoxLight(BoxLight* light) override;

			// �|�C���g���C�g�o�^
			void RegisterPointLight(PointLight* light) override;

			// �|�C���g���C�g�o�^����
			void UnregisterPointLight(PointLight* light) override;

			// ���C�g���X�V
			void UpdateLightEnvironment(LightEnvironment& env, const Math::AABB& bounds) override;
		};
	} // namespace Graphics
} // namespace Phoenix