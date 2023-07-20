#include "VertexBuffer.h"

void VertexBuffer::bind(Graphics& gfx) noexcept
{
	const UINT offset = 0u;
	gfx.getContext()->IASetVertexBuffers(0u, 1, _pVertexBuffer.GetAddressOf(), &_stride, &offset);
}
