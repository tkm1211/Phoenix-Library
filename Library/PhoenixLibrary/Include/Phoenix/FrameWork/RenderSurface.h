#pragma once

#include "Phoenix/FND/NonCopyable.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Math/Color.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �����_�[�T�[�t�F�C�X
		//****************************************************************************
		class RenderSurface final : public FND::NonCopyable
		{
		private:
			std::unique_ptr<Graphics::IRenderTargetSurface>	rts;
			std::unique_ptr<Graphics::IDepthStencilSurface>	dss;
			Math::Color backgroundColor = Math::Color::Gray;
			u32 width = 0;
			u32 height = 0;

		public:
			// ������
			bool Initialize(Graphics::IDevice* device, u32 width, u32 height);

			// �I����
			void Finalize();

			// ���擾
			u32 GetWidth() const { return width; }

			// �����擾
			u32 GetHeight() const { return height; }

			// �w�i�F��ݒ�
			void SetBackgroundColor(const Math::Color& color) { backgroundColor = color; }

			// �w�i�F���擾
			const Math::Color& GetBackgroundColor() const { return backgroundColor; }

			// ���T�C�Y
			void Resize(Graphics::IDevice* device, u32 width, u32 height);

			// �����_�[�^�[�Q�b�g�T�[�t�F�C�X�擾
			Graphics::IRenderTargetSurface* GetRenderTargetSurface() { return rts.get(); }

			// �f�v�X�X�e���V���t�F�C�X�擾
			Graphics::IDepthStencilSurface* GetDepthStencilSurface() { return dss.get(); }

			// �e�N�X�`���擾
			Graphics::ITexture* GetTexture() const { return rts->GetTexture(); }
		};
	} // namespace FrameWork
} // namespace Phoenix