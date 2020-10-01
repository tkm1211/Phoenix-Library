#pragma once

#include <memory>
#include <map>
#include "UI.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/Math/Vector.h"


class TargetMarkUI : public UI
{
private:
	struct TargetData
	{
		Phoenix::Math::Vector2 pos;
		Phoenix::Math::Vector2 goalPos;
		Phoenix::Math::Vector2 texPos;
	};

	enum TargetDataNames
	{
		leftUp,
		leftDown,
		rightUp,
		rightDown,
		center,
		centerUp,
		centerDown,
		centerLeft,
		centerRight,
	};

private:
	/*static constexpr*/ Phoenix::f32 TexWidth = 32.0f;
	/*static constexpr*/ Phoenix::f32 TexHeigth = 32.0f;

	/*static constexpr*/ Phoenix::f32 SizeX = 32.0f;
	/*static constexpr*/ Phoenix::f32 SizeY = 32.0f;

	/*static constexpr*/ Phoenix::f32 StartPosX = 80.0f;
	/*static constexpr*/ Phoenix::f32 StartPosY = 80.0f;

	/*static constexpr*/ Phoenix::f32 GoalPosX = 20.0f;
	/*static constexpr*/ Phoenix::f32 GoalPosY = 20.0f;

private:
	std::shared_ptr<Phoenix::Graphics::ITexture> mark;

	bool isUpdate = false;
	std::map<TargetDataNames, TargetData> marks;

	Phoenix::Math::Vector2 originPos = Phoenix::Math::Vector2(0.0f, 0.0f);

public:
	TargetMarkUI() {}
	~TargetMarkUI() {}

public:
	static std::shared_ptr<TargetMarkUI> Create();

	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override;

	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad) override;

	void MarkInitialize();

public:
	/// <summary>
	/// �^�[�Q�b�g�}�[�N�X�V
	/// </summary>
	/// <param name="targetPos"> ���[���h��Ԃɂ���^�[�Q�b�g���W���X�N���[����Ԃɕϊ��������W </param>
	void Update(Phoenix::Math::Vector2 targetPos);

	/// <summary>
	/// �^�[�Q�b�g�J�n
	/// </summary>
	/// <param name="targetPos"> �^�[�Q�b�g���W </param>
	void LockOnTarget();
};