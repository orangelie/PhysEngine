#include "pch.h"
#include "Material.h"
#include "Engine.h"

void Material::Update()
{
	for (uint32 i = 0; i < SRV_REGISTER_COUNT; ++i)
	{
		if (_srvHandles[i].ptr == 0)
			continue;

		// 5 = CBV_REGISTER_COUNT
		GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->SetSRV(_srvHandles[i], static_cast<SRV_REGISTER>(i + 5));
	}

	_shader->Update();
	_mesh->Render();
}
