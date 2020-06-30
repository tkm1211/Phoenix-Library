#pragma once

#include <any>


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �C���X�^���V���O����C���^�[�t�F�[�X
		//****************************************************************************
		class IInstancingContext
		{
		public:
			// �R�s�[�\��
			virtual bool CanCopy() = 0;

			// �R�s�[
			virtual std::any Copy() = 0;

			// �}���\��
			virtual bool CanInsert(std::any& object) = 0;

			// �}��
			virtual void Insert(std::any& object) = 0;

			// �폜�\��
			virtual bool CanDelete() = 0;

			// �폜
			virtual void Delete() = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix