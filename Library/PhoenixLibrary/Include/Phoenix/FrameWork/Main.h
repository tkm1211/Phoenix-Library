#pragma once

#include <memory>
#include <string>
#include "Phoenix/OS/Main.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Container.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// ���C�����[�v
		//****************************************************************************
		class Main : public OS::IMain
		{
		protected:
			std::unique_ptr<OS::IDisplay> display;
			std::unique_ptr<Graphics::IGraphicsDevice> graphicsDevice;

			//bool exit = false;
			//uintPtr handle = 0;
			//std::string name;
			//
			//std::unique_ptr<Container> container;
			//IGameEngine* gameEngine = nullptr;

		public:
			Main() {}
			~Main() {}

		public:
			// ������
			bool Initialize(uintPtr instance) override;

			// �I����
			void Finalize() override;

			// �I����
			void Run() override;

			//// �I���v��
			//void Exit() override { exit = true; }

			//// �I���v���m�F
			//bool IsExit() override { return exit; }

			//// ���O�擾
			//virtual const char* GetName() override { return name.c_str(); }

			//// ���O�ݒ�
			//void SetName(const char* name) override { name = name; }

			//// �n���h��
			//uintPtr Handle() override { return handle; }

		protected:
			// �R���e�i�Z�b�g�A�b�v
			//virtual void OnSetupContainer(Container* container);

			// �Q�[���Z�b�g�A�b�v
			//virtual void OnSetupGame(IGame* game);

			// �X�V
			virtual void Update() = 0;

			// �`��
			virtual void Render() = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix