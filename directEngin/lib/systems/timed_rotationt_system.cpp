#include "timed_rotationt_system.h"
#include "components/timed_rotation.h"
#include "../common/input.h"


void TimedRotationSystem::onUpdate(SceneManager& scene, float time)
{
	float angle = time;
	auto& trPool = scene.getPool<Transform>();

	auto& rotPool = scene.getPool<TimedRotation>();

	for (auto [entt,rot,tr]:scene.view<TimedRotation, Transform>())
	{
		tr.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle*rot.xSence, angle*rot.ySence, angle*rot.zSence);
	}
}
