#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Phoenix/Math/Quaternion.h"
#include "Phoenix/Math/Vector.h"
#include "Phoenix/OS/Resource.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �A�j���[�V�����f�[�^
		//****************************************************************************
		struct AnimationData
		{
			struct ScaleKey
			{
				f32 seconds;
				Math::Vector3 value;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct RotateKey
			{
				f32 seconds;
				Math::Quaternion value;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct TranslateKey
			{
				f32 seconds;
				Math::Vector3 value;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Node
			{
				//std::string name;
				std::vector<ScaleKey> scaleKeys;
				std::vector<RotateKey> rotateKeys;
				std::vector<TranslateKey> translateKeys;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Clip
			{
				std::string name;
				f32 secondsLength;
				u32 frameLength;
				std::vector<Node> nodes;

				void GetScale(s32 nodeID, f32 seconds, Math::Vector3& value, s32& keyID) const;
				void GetRotate(s32 nodeID, f32 seconds, Math::Quaternion& value, s32& keyID) const;
				void GetTranslate(s32 nodeID, f32 seconds, Math::Vector3& value, s32& keyID) const;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			std::vector<Clip> clips;
			std::vector<std::string> nodeNames;

			template<class Archive>
			void serialize(Archive& archive, u32 version);

			// �V���A���C�Y
			static void Serialize(const AnimationData& data, const char* filename);

			// �f�V���A���C�Y
			static bool Deserialize(AnimationData& data, const char* filename);
		};

		//****************************************************************************
		// �A�j���[�V�������\�[�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IAnimationResource : public OS::Resource
		{
		public:
			// �A�j���[�V�����f�[�^�擾
			virtual const AnimationData& GetAnimationData() = 0;
		};

		//****************************************************************************
		// �A�j���[�V�������\�[�X����I�u�W�F�N�g�����C���^�[�t�F�[�X
		//****************************************************************************
		class IAnimationResourceFactory : public OS::IResourceFactory
		{
		public:
			// ����
			static std::unique_ptr<IAnimationResourceFactory> Create();
		};

		//****************************************************************************
		// �A�j���[�V�����v���C���[����C���^�[�t�F�[�X
		//****************************************************************************
		class IAnimationPlayer : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IAnimationPlayer> Create();

			// ������
			virtual bool Initialize(std::shared_ptr<IAnimationResource>& resource) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �X�V
			virtual void Update(f32 elapsedTime) = 0;

			// �X�P�[���l�v�Z
			virtual void CalculateScale(s32 nodeID, Math::Vector3& scale) = 0;

			// ��]�l�v�Z
			virtual void CalculateRotate(s32 nodeID, Math::Quaternion& rotate) = 0;

			// �ړ��l�v�Z
			virtual void CalculateTranslate(s32 nodeID, Math::Vector3& translate) = 0;

			// �Đ�����
			virtual bool IsPlaying() = 0;

			// �Đ�
			virtual void Play(s32 index) = 0;
			virtual void Play(const char* name) = 0;

			// �����߂��čĐ�
			virtual void Rewind() = 0;

			// �ꎞ��~/�ĊJ
			virtual void Pause(bool pause) = 0;

			// ��~
			virtual void Stop() = 0;

			// ���[�v�Đ��ݒ�
			virtual void SetLoop(bool loop) = 0;

			// ���[�v�Đ���
			virtual bool IsLoop() = 0;

			// �t�Đ��ݒ�
			virtual void SetReverse(bool reverse) = 0;

			// �t�Đ���
			virtual bool IsReverse() = 0;

			// �Đ����Ԓ�(�b)�擾
			virtual f32 GetLength() = 0;

			// �Đ����x�{���ݒ�
			virtual void SetSpeed(f32 speed) = 0;

			// �Đ����x�{���擾
			virtual f32 GetSpeed() = 0;

			// �u�����h���Ԑݒ�
			virtual void SetBlendTime(f32 seconds) = 0;

			// �u�����h���Ԏ擾
			virtual f32 GetBlendTime() = 0;

			// �u�����h����
			virtual bool IsBlend() = 0;

			// �Đ��ʒu(�b)�ݒ�
			virtual void SetCurrentTime(f32 seconds) = 0;

			// �Đ��ʒu�擾
			virtual f32 GetCurrentTime() = 0;

			// �c��Đ����Ԏ擾
			virtual f32 GetLastTime() = 0;

			// �Đ��J�n�ʒu�ݒ�
			virtual void SetBeginTime(f32 seconds) = 0;

			// �Đ��J�n�ʒu�擾
			virtual f32 GetBeginTime() = 0;

			// �Đ��I���ʒu�ݒ�
			virtual void SetEndTime(f32 seconds) = 0;

			// �Đ��I���ʒu�擾
			virtual f32 GetEndTime() = 0;

			// ���[�v�Đ��J�n�ʒu�ݒ�
			virtual void SetLoopBeginTime(f32 seconds) = 0;

			// ���[�v�Đ��J�n�ʒu�擾
			virtual f32 GetLoopBeginTime() = 0;

			// ���[�v�Đ��I���ʒu�ݒ�
			virtual void SetLoopEndTime(f32 seconds) = 0;

			// ���[�v�Đ��I���ʒu�擾
			virtual f32 GetLoopEndTime() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix