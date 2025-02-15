#pragma once

#include <DirectXMath.h>

namespace dxw
{

struct TransformBuffer
{
    DirectX::XMMATRIX transform;
    DirectX::XMMATRIX projection;
};
}