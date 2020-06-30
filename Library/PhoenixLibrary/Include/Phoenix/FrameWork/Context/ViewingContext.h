#pragma once

#include <set>
#include <vector>
#include "Phoenix/Math/Vector.h"
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �O���錾
		class IViewingEventListener;

		//****************************************************************************
		// �r���[����C���^�[�t�F�[�X
		//****************************************************************************
		class IViewingContext
		{
		public:
			// ���݂̃r���[�Ŏw��̃I�u�W�F�N�g���t���[�����ɂ����߂�邩
			virtual bool CanFrame(const std::vector<IObject*>& items) = 0;

			// ���݂̃r���[�Ɏw��̃I�u�W�F�N�g���t���[�����ɂ����߂�
			virtual void Frame(const std::vector<IObject*>& items) = 0;

			// �r���[����C�x���g���X�i�[�o�^
			virtual void RegisterEventListener(IViewingEventListener* listener) = 0;

			// �r���[����C�x���g���X�i�[�o�^����
			virtual void UnregisterEventListener(IViewingEventListener* listener) = 0;
		};

		//****************************************************************************
		// �r���[����C�x���g���X�i�[
		//****************************************************************************
		class IViewingEventListener
		{
		public:
			// �w��̃I�u�W�F�N�g���t���[�����ɂ����߂鎞�ɌĂ΂��
			virtual void OnFrame(Math::Vector3& center, f32 radius) = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix