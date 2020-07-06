#include "pch.h"
#include "Phoenix/FND/Assert.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/Math/Arithmetic.h"
#include "AnimationPlayer.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �A�j���[�V�����v���C���[
		//****************************************************************************
		// ����
		std::unique_ptr<IAnimationPlayer> IAnimationPlayer::Create()
		{
			return std::make_unique<AnimationPlayer>();
		}

		// ������
		bool AnimationPlayer::Initialize(std::shared_ptr<IAnimationResource>& resource)
		{
			this->resource = resource;
			accessKeys.resize(resource->GetAnimationData().nodeNames.size());

			return true;
		}

		// �I����
		void AnimationPlayer::Finalize()
		{

		}

		// �X�V
		void AnimationPlayer::Update(f32 elapsedTime)
		{
			if (IsPlaying())
			{
				blendRate = 1.0f;

				if (blendTime < blendLength)						// ���⊮���̌v�Z
				{													// �{���͑O�̃|�[�Y���玟�̃|�[�Y�̕⊮�����|�[�Y��
					blendTime += elapsedTime * speed;				// ���݂̃|�[�Y���������邱�Ƃ��Y��ȃ|�[�Y�ɂ��������A
					if (blendTime >= blendLength)					// �|�[�Y�̌v�Z�ʂ��O�Ǝ��ƌ��݂�3�{������A�܂��A
					{												// �������邽�߂̃|�[�Y��ۑ�����ꎞ�o�b�t�@���K�v�Ȃ��߁A
						blendTime = blendLength;					// ���݂̃|�[�Y���玟�̃|�[�Y�̕⊮�ŁA�⊮����K�オ���
					}												// �傫�����邱�Ƃŋ[���I�ɍČ����Ă���B
					blendRate = blendTime / blendLength;
					blendRate = blendRate * blendRate;
				}


				f32 fEndTime;
				if (reverse)
				{
					// �t�Đ�
					currentTime -= elapsedTime * speed;
					fEndTime = loop ? loopBeginTime : beginTime;
					if (currentTime <= fEndTime)
					{
						if (loop)
						{
							currentTime = (currentTime - fEndTime) + loopEndTime;
						}
						else
						{
							currentTime = beginTime;
							playing = false;
						}
					}
				}
				else
				{
					// �ʏ�Đ�
					currentTime += elapsedTime * speed;
					fEndTime = loop ? loopEndTime : endTime;
					if (currentTime >= fEndTime)
					{
						if (loop)
						{
							currentTime = (currentTime - fEndTime) + loopBeginTime;
						}
						else
						{
							currentTime = fEndTime;
							playing = false;
						}
					}
				}
			}
		}

		// �X�P�[���l�v�Z
		void AnimationPlayer::CalculateScale(s32 nodeID, Math::Vector3& scale)
		{
			PHOENIX_ASSERT(currentClip != nullptr);

			if (Math::Equal(blendRate, 1.0f))
			{
				// �ʏ�̌v�Z
				currentClip->GetScale(nodeID, currentTime, scale, accessKeys.at(nodeID).scaleID);
			}
			else if (Math::IsZero(blendRate))
			{
				// �������Ȃ�
			}
			else
			{
				// �u�����h�v�Z
				Math::Vector3 value;
				currentClip->GetScale(nodeID, currentTime, value, accessKeys.at(nodeID).scaleID);

				scale = Math::Vector3Lerp(scale, value, blendRate);
			}
		}

		// ��]�l�v�Z
		void AnimationPlayer::CalculateRotate(s32 nodeID, Math::Quaternion& rotate)
		{
			PHOENIX_ASSERT(currentClip != nullptr);

			if (Math::Equal(blendRate, 1.0f))
			{
				// �ʏ�̌v�Z
				currentClip->GetRotate(nodeID, currentTime, rotate, accessKeys.at(nodeID).rotateID);
			}
			else if (Math::IsZero(blendRate))
			{
				// �������Ȃ�
			}
			else
			{
				// �u�����h�v�Z
				Math::Quaternion value;
				currentClip->GetRotate(nodeID, currentTime, value, accessKeys.at(nodeID).rotateID);

				rotate = Math::QuaternionSlerp(rotate, value, blendRate);
			}
		}

		// �ړ��l�v�Z
		void AnimationPlayer::CalculateTranslate(s32 nodeID, Math::Vector3& translate)
		{
			PHOENIX_ASSERT(currentClip != nullptr);

			if (Math::Equal(blendRate, 1.0f))
			{
				// �ʏ�̌v�Z
				currentClip->GetTranslate(nodeID, currentTime, translate, accessKeys.at(nodeID).translateID);
			}
			else if (Math::IsZero(blendRate))
			{
				// �������Ȃ�
			}
			else
			{
				// �u�����h�v�Z
				Math::Vector3 value;
				currentClip->GetTranslate(nodeID, currentTime, value, accessKeys.at(nodeID).translateID);

				translate = Math::Vector3Lerp(translate, value, blendRate);
			}
		}

		// �Đ�����
		bool AnimationPlayer::IsPlaying()
		{
			return playing && currentClip != nullptr;
		}

		// �Đ�
		void AnimationPlayer::Play(s32 index)
		{
			currentClip = nullptr;
			playing = false;
			currentTime = 0.0f;

			const Graphics::AnimationData& data = resource->GetAnimationData();

			if (index < 0 || index >= static_cast<s32>(data.clips.size()))
			{
				return;
			}

			const Graphics::AnimationData::Clip& clip = data.clips.at(index);

			currentClip = &clip;
			playing = true;
			beginTime = loopBeginTime = 0.0f;
			endTime = loopEndTime = clip.secondsLength;
			currentTime = reverse ? endTime : beginTime;

		}

		// �Đ�
		void AnimationPlayer::Play(const char* name)
		{
			const Graphics::AnimationData& data = resource->GetAnimationData();
			for (size_t i = 0; i < data.clips.size(); ++i)
			{
				if (data.clips.at(i).name == name)
				{
					Play(static_cast<s32>(i));
				}
			}
		}

		// �����߂��čĐ�
		void AnimationPlayer::Rewind()
		{
			if (currentClip != nullptr)
			{
				playing = true;
				currentTime = reverse ? endTime : beginTime;
			}
		}

		// �ꎞ��~/�ĊJ
		void AnimationPlayer::Pause(bool pause)
		{
			playing = !pause;
		}

		// ��~
		void AnimationPlayer::Stop()
		{
			playing = false;
		}

		// ���[�v�Đ��ݒ�
		void AnimationPlayer::SetLoop(bool loop)
		{
			this->loop = loop;
		}

		// ���[�v�Đ���
		bool AnimationPlayer::IsLoop()
		{
			return loop;
		}

		// �t�Đ��ݒ�
		void AnimationPlayer::SetReverse(bool reverse)
		{
			this->reverse = reverse;
		}

		// �t�Đ���
		bool AnimationPlayer::IsReverse()
		{
			return reverse;
		}

		// �Đ����Ԓ�(�b)�擾
		f32 AnimationPlayer::GetLength()
		{
			if (currentClip != nullptr)
			{
				return currentClip->secondsLength;
			}
			return 0.0f;
		}

		// �Đ����x�{���ݒ�
		void AnimationPlayer::SetSpeed(f32 speed)
		{
			this->speed = speed;
		}

		// �Đ����x�{���擾
		f32 AnimationPlayer::GetSpeed()
		{
			return speed;
		}

		// �u�����h���Ԑݒ�
		void AnimationPlayer::SetBlendTime(f32 seconds)
		{
			blendTime = 0.0f;
			blendLength = seconds;
		}

		// �u�����h���Ԏ擾
		f32 AnimationPlayer::GetBlendTime()
		{
			return blendLength;
		}

		// �u�����h����
		bool AnimationPlayer::IsBlend()
		{
			return blendRate != 1.0f ? true : false;
		}

		// �Đ��ʒu�ݒ�
		void AnimationPlayer::SetCurrentTime(f32 seconds)
		{
			currentTime = seconds;
		}

		// �Đ��ʒu�擾
		f32 AnimationPlayer::GetCurrentTime()
		{
			return currentTime;
		}

		// �c��Đ����Ԏ擾
		f32 AnimationPlayer::GetLastTime()
		{
			if (reverse)
			{
				return currentTime - beginTime;
			}
			else
			{
				return endTime - currentTime;
			}
		}

		// �Đ��J�n�ʒu�ݒ�
		void AnimationPlayer::SetBeginTime(f32 seconds)
		{
			beginTime = seconds;
		}

		// �Đ��J�n�ʒu�擾
		f32 AnimationPlayer::GetBeginTime()
		{
			return beginTime;
		}

		// �Đ��I���ʒu�ݒ�
		void AnimationPlayer::SetEndTime(f32 seconds)
		{
			endTime = seconds;
		}

		// �Đ��I���ʒu�擾
		f32 AnimationPlayer::GetEndTime()
		{
			return endTime;
		}

		// ���[�v�Đ��J�n�ʒu�ݒ�
		void AnimationPlayer::SetLoopBeginTime(f32 seconds)
		{
			loopBeginTime = seconds;
		}

		// ���[�v�Đ��J�n�ʒu�擾
		f32 AnimationPlayer::GetLoopBeginTime()
		{
			return loopBeginTime;
		}

		// ���[�v�Đ��I���ʒu�ݒ�
		void AnimationPlayer::SetLoopEndTime(f32 seconds)
		{
			loopEndTime = seconds;
		}

		// ���[�v�Đ��I���ʒu�擾
		f32 AnimationPlayer::GetLoopEndTime()
		{
			return loopEndTime;
		}

	}	// namespace grp
}	// namespace ys