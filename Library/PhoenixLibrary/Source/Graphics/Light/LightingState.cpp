#include "pch.h"
#include "Phoenix/Math/Arithmetic.h"
#include "LightingState.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ���C�e�B���O�X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		// �R���X�g���N�^
		LightingState::LightingState()
		{
			defaultDirLight.ambient = defaultDirLight.diffuse = defaultDirLight.specular = Math::Color(1, 1, 1, 1);
			defaultDirLight.direction = Math::Vector3(0.0f, 1.0f, 0.0f);

			noDirLight.ambient = noDirLight.diffuse = noDirLight.specular = Math::Color(0, 0, 0, 0);
			noDirLight.direction = Math::Vector3(0.0f, -1.0f, 0.0f);

			noBoxLight.ambient = noBoxLight.diffuse = noBoxLight.specular = Math::Color(0, 0, 0, 0);
			noBoxLight.min = Math::Vector3(Math::MAX_F32, Math::MAX_F32, Math::MAX_F32);
			noBoxLight.max = Math::Vector3(-Math::MAX_F32, -Math::MAX_F32, -Math::MAX_F32);

			noPointLight.ambient = noPointLight.diffuse = noPointLight.specular = Math::Color(0, 0, 0, 0);
			noPointLight.position = Math::Vector4(0, 0, 0, 0);
		}

		// �S�Ẵ��C�g�o�^����
		void LightingState::ClearLight()
		{
			dirLights.clear();
			boxLights.clear();
			pointLights.clear();
		}

		// �f�B���N�V���i�����C�g�o�^
		void LightingState::RegisterDirLight(DirLight* light)
		{
			dirLights.insert(light);
		}

		// �f�B���N�V���i�����C�g�o�^����
		void LightingState::UnregisterDirLight(DirLight* light)
		{
			dirLights.erase(light);
		}

		// ���C���f�B���N�V���i�����C�g�擾
		DirLight* LightingState::GetProminentDirLight()
		{
			DirLight* light = GetDefaultDirLight();
			if (dirLights.size())
			{
				light = *(dirLights.begin());
			}
			return light;
		}

		// �f�t�H���g�f�B���N�V���i�����C�g�擾
		DirLight* LightingState::GetDefaultDirLight()
		{
			return &defaultDirLight;
		}

		// �{�b�N�X���C�g�o�^
		void LightingState::RegisterBoxLight(BoxLight* light)
		{
			boxLights.insert(light);
		}

		// �{�b�N�X���C�g�o�^����
		void LightingState::UnregisterBoxLight(BoxLight* light)
		{
			boxLights.erase(light);
		}

		// �|�C���g���C�g�o�^
		void LightingState::RegisterPointLight(PointLight* light)
		{
			pointLights.insert(light);
		}

		// �|�C���g���C�g�o�^����
		void LightingState::UnregisterPointLight(PointLight* light)
		{
			pointLights.erase(light);
		}

		// ���C�g���X�V
		void LightingState::UpdateLightEnvironment(LightEnvironment& env, const Math::AABB& bounds)
		{
			env.numDirLights = 0;
			env.numBoxLights = 0;
			env.numPointLights = 0;

			// �f�B���N�V���i�����C�g���W
			if (dirLights.size() > 0)
			{
				for (auto it = dirLights.begin(); it != dirLights.end(); ++it)
				{
					env.dirLights[env.numDirLights++] = *(*it);
					if (env.numDirLights >= MaxDirLights)
					{
						break;
					}
				}
			}
			else
			{
				// �Ȃ��ꍇ�̓f�t�H���g���C�g
				env.dirLights[env.numDirLights++] = defaultDirLight;
			}

			//// �{�b�N�X���C�g���W
			//for (auto it = boxLights.begin(); it != boxLights.end(); ++it)
			//{
			//	BoxLight  light = *(*it);
			//	Math::AABB lightBounds(light.min, light.max);

			//	if (bounds.Intersect(lightBounds))
			//	{
			//		env.boxLights[env.numBoxLights++] = light;
			//		if (env.numBoxLights >= MaxBoxLights)
			//		{
			//			break;
			//		}
			//	}
			//}

			//// �|�C���g���C�g���W
			//for (auto it = pointLights.begin(); it != pointLights.end(); ++it)
			//{
			//	PointLight  light = *(*it);

			//	Math::Vector3 pos = Math::Vector3(light.position.x, light.position.y, light.position.z);
			//	f32 radius = light.position.w;
			//	Math::Vector3 ll(pos.x - radius, pos.y - radius, pos.z - radius);
			//	Math::Vector3 ur(pos.x + radius, pos.y + radius, pos.z + radius);
			//	Math::AABB sphereBounds(ll, ur);
			//	if (bounds.Intersect(sphereBounds))
			//	{
			//		env.pointLights[env.numPointLights++] = light;
			//		if (env.numPointLights >= MaxPointLights)
			//		{
			//			break;
			//		}
			//	}
			//}

			// ���g�p�̃��C�g�X���b�g����ɐݒ�
			for (u32 i = env.numDirLights; i < MaxDirLights; ++i)
			{
				env.dirLights[i] = noDirLight;
			}
			for (u32 i = env.numBoxLights; i < MaxBoxLights; ++i)
			{
				env.boxLights[i] = noBoxLight;
			}
			for (u32 i = env.numPointLights; i < MaxPointLights; ++i)
			{
				env.pointLights[i] = noPointLight;
			}
		}


	}	// namespace grp
}	// namespace ys