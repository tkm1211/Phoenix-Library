#pragma once

#include <memory>
#include "UI.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/Math/Vector.h"


class TutorialUI : public UI
{
private:
#if (defined(DEBUG) | defined(_DEBUG))
	static constexpr Phoenix::f32 SizeWidth = 1280.0f;
	static constexpr Phoenix::f32 SizeHeigth = 720.0f;
#else
	static constexpr Phoenix::f32 SizeWidth = 1920.0f;
	static constexpr Phoenix::f32 SizeHeigth = 1080.0f;
#endif

	static constexpr Phoenix::f32 WaitMaxCnt = 100.0f;

private:
	std::shared_ptr<Phoenix::Graphics::ITexture> attack;
	std::shared_ptr<Phoenix::Graphics::ITexture> attackDetail;

	std::shared_ptr<Phoenix::Graphics::ITexture> dedge;
	std::shared_ptr<Phoenix::Graphics::ITexture> dedgeDetail;

	std::shared_ptr<Phoenix::Graphics::ITexture> lockOn;
	std::shared_ptr<Phoenix::Graphics::ITexture> lockOnDetail;

	Phoenix::s32 state = 0;
	Phoenix::f32 waitCnt = 0.0f;

	bool toDraw = false;
	bool cleared[4] = { false, false, false, false };

public:
	TutorialUI() {}
	~TutorialUI() {}

public:
	// ����
	static std::shared_ptr<TutorialUI> Create();

	// �R���X�g���N�^
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override;

	// ������
	void Initialize() override;

	// �`��
	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad) override;

public:
	// �X�V
	void Update(Phoenix::f32 elapsedTime);

	// �X�e�[�g�ڍs
	void TransitionState(bool condition, Phoenix::f32 elapsedTime);

	void ClearWeakAttack();
	void ClearStrongAttack();
	void ClearDedge();
	void ClearLockOn();

	void ToDraw(bool toDraw);
};

