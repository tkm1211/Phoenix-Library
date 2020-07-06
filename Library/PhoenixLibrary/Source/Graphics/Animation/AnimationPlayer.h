#pragma once

#include "Phoenix/Graphics/Animation.h"
#include "Phoenix/Math/PhoenixMath.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �A�j���[�V�����v���C���[�N���X
		//****************************************************************************
		class AnimationPlayer : public IAnimationPlayer
		{
		private:
			struct AccessKey
			{
				s32 scaleID = 0;
				s32 rotateID = 0;
				s32 translateID = 0;
			};

		private:
			std::shared_ptr<IAnimationResource>	resource;
			std::vector<AccessKey> accessKeys;
			const AnimationData::Clip* currentClip = nullptr;
			f32 blendLength = 0.0f;
			f32 blendTime = 0.0f;
			f32 blendRate = 0.0f;
			f32 speed = 1.0f;
			f32 currentTime = 0.0f;
			f32 beginTime = 0.0f;
			f32 endTime = 0.0f;
			f32 loopBeginTime = 0.0f;
			f32 loopEndTime = 0.0f;
			bool playing = false;
			bool loop = false;
			bool reverse = false;

		public:
			AnimationPlayer() {}
			~AnimationPlayer() override { Finalize(); }

		public:
			// ������
			bool Initialize(std::shared_ptr<IAnimationResource>& resource) override;

			// �I����
			void Finalize() override;

			// �X�V
			void Update(f32 elapsed_time) override;

			// �X�P�[���l�v�Z
			void CalculateScale(s32 nodeID, Math::Vector3& scale) override;

			// ��]�l�v�Z
			void CalculateRotate(s32 nodeID, Math::Quaternion& rotate) override;

			// �ړ��l�v�Z
			void CalculateTranslate(s32 nodeID, Math::Vector3& translate) override;

			// �Đ�����
			bool IsPlaying() override;

			// �Đ�
			void Play(s32 index) override;
			void Play(const char* name) override;

			// �����߂��čĐ�
			void Rewind() override;

			// �ꎞ��~/�ĊJ
			void Pause(bool pause) override;

			// ��~
			void Stop() override;

			// ���[�v�Đ��ݒ�
			void SetLoop(bool loop) override;

			// ���[�v�Đ���
			bool IsLoop() override;

			// �t�Đ��ݒ�
			void SetReverse(bool reverse) override;

			// �t�Đ���
			bool IsReverse() override;

			// �Đ����Ԓ�(�b)�擾
			f32 GetLength() override;

			// �Đ����x�{���ݒ�
			void SetSpeed(f32 speed) override;

			// �Đ����x�{���擾
			f32 GetSpeed() override;

			// �u�����h���Ԑݒ�
			void SetBlendTime(f32 seconds) override;

			// �u�����h���Ԏ擾
			f32 GetBlendTime() override;

			// �u�����h����
			bool IsBlend() override;

			// �Đ��ʒu�ݒ�
			void SetCurrentTime(f32 seconds) override;

			// �Đ��ʒu�擾
			f32 GetCurrentTime() override;

			// �c��Đ����Ԏ擾
			f32 GetLastTime() override;

			// �Đ��J�n�ʒu�ݒ�
			void SetBeginTime(f32 seconds) override;

			// �Đ��J�n�ʒu�擾
			f32 GetBeginTime() override;

			// �Đ��I���ʒu�ݒ�
			void SetEndTime(f32 seconds) override;

			// �Đ��I���ʒu�擾
			f32 GetEndTime() override;

			// ���[�v�Đ��J�n�ʒu�ݒ�
			void SetLoopBeginTime(f32 seconds) override;

			// ���[�v�Đ��J�n�ʒu�擾
			f32 GetLoopBeginTime() override;

			// ���[�v�Đ��I���ʒu�ݒ�
			void SetLoopEndTime(f32 seconds) override;

			// ���[�v�Đ��I���ʒu�擾
			f32 GetLoopEndTime() override;
		};
	} // namespace Graphics
} // namespace Phoenix