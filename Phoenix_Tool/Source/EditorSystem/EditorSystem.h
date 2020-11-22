#pragma once

#include <memory>
#include <vector>
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


class IEditor
{
public:
	// ������
	virtual bool Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice) = 0;

	// �I����
	virtual void Finalize() = 0;

	// �X�V
	virtual void Update(Phoenix::f32 elapsedTime) = 0;

	// �`��
	virtual void Draw(Phoenix::f32 elapsedTime) = 0;

	// GUI
	virtual void GUI() = 0;
};

class EditorSystem
{
private:
	std::vector<std::shared_ptr<IEditor>> editors;
	std::shared_ptr<IEditor> currentEditor;

public:
	EditorSystem() {}
	~EditorSystem() {}

public:
	// ����
	static std::unique_ptr<EditorSystem> Create();

	// ������
	bool Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice);

	// �I����
	void Finalize();

	// �X�V
	void Update(Phoenix::f32 elapsedTime);

	// �`��
	void Draw(Phoenix::f32 elapsedTime);

	// GUI
	void GUI();
};