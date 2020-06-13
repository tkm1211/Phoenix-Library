#pragma once


namespace Phoenix
{
	namespace FND
	{
		//****************************************************************************
		// �p�������N���X���R�s�[�s�ɂ���
		//****************************************************************************
		class NonCopyable
		{
		protected:
			NonCopyable() {}
			~NonCopyable() {}
		private:
			NonCopyable(const NonCopyable&);
			const NonCopyable& operator =(const NonCopyable&);
		};

	}	// namespace FND
}	// namespace Phoenix
