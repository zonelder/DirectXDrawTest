#pragma once
#include "../graphics/Drawable/BindComponent/vertex_shader.h"
#include "../graphics/Drawable/BindComponent/pixel_shader.h"
#include "../graphics/Drawable/BindComponent/input_layout.h"
#include "../graphics/Drawable/BindComponent/texture.h" 

#include <memory>

struct Material
{
public:

	Material() = delete;
	Material(Graphics& gfx);
	void bind(Graphics& gfx);

	Texture texture;
	DirectX::XMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f };
	VertexShaderPtr pVertexShader;
	PixelShaderPtr pPixelShader;
};

using MaterialPtr = std::shared_ptr<Material>;
