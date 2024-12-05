#pragma once
#include <unordered_map>
#include <functional>

#include "common/window.h"
#include "common/input.h"

#include "Camera/camera.h"

#include "mesh_pool.h"
#include "component_pool.h"
#include "component_manager.h"

#include "components/render.h"
#include "components/transform.h"

#include "entity_manager.h"
#include "filters.h"



class SceneManager
{
public:
	SceneManager(const Window& wnd);


	Camera& getCamera() noexcept;

	Graphics& getGraphic() noexcept;

	const Entity& createEntity() noexcept;

	bool destroyEntity(const Entity& entt) noexcept;

	bool destroyEntity(EntityID id) noexcept;

	
	/// @brief create simple view with include types only
	/// @tparam ...Args List of types we want to include in filter
	/// @return ComponentView
	template<class... Args> requires filters::has_not_filters<Args...>
	auto& view() noexcept
	{
		using with = filters::With<Args...>;
		using without = filters::Without<>;

		constexpr auto b = filters::is_component_filters<with, without>;
		return view<with, without>();
	}

	/// @brief create view by type of other view.
	/// @tparam TWith
	/// @tparam TWithout
	/// @return ComponentView
	template<class TWith, class TWithout = filters::Without<>> requires filters::is_component_filters< TWith, TWithout>
	auto& view() noexcept
	{
		// TODO each filter can be crate once if component manageer wont relocate em view on each filter once and then reuse it
		static filters::ComponentView<TWith, TWithout> m(_ComponentManager);
		return m;
	}
	

	/// @brief return view of all entities
	/// @return 
	auto view() noexcept
	{
		return _entityManager.entityView();
	}

	template<typename T>
	ComponentPool<T>& getPool()
	{
		return *(_ComponentManager.getPool<T>());
	}
	template<typename T>
	const ComponentPool<T>& getPool() const
	{
		return *(_ComponentManager.getPool<T>());
	}

	template<typename T>
	ComponentPool<T>& getPoolByGuid(std::string& guid)
	{

	}
	// ���������� ������ getComponent
	template<typename T>
	T& getComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		return _ComponentManager.getComponent<T>(entt_id);
	}
	// ���������� ������ getComponent
	template<typename T>
	T& getComponent(EntityID entt)
	{
		return _ComponentManager.getComponent<T>(entt);
	}


	template<typename T>
	T& addComponent(const Entity& entt)
	{
		auto entt_id = entt.getID();
		return addComponent<T>(entt_id);
	}
	template<typename T>
	T& addComponent(EntityID entt)
	{
		_entityManager.registerComponent(entt, Family::type_id<T>());
		return _ComponentManager.addComponent<T>(entt);
	}

	template<>
	Transform& addComponent(EntityID entt)
	{
		_entityManager.registerComponent(entt, Family::type_id<Transform>());
		auto& tr =  _ComponentManager.addComponent<Transform>(entt);
		tr.vertexConstantBuffer = VertexConstantBuffer<DirectX::XMMATRIX>(_gfx, tr.orientationMatrix);
		return tr;
	}

	template<>
	Render& addComponent(EntityID entt)
	{
		_entityManager.registerComponent(entt, Family::type_id<Render>());
		auto& r = _ComponentManager.addComponent<Render>(entt);
		r.topology= Topology(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return r;
	}



	template<typename T>
	void removeComponent(const Entity& entt)
	{
		removeComponent<T>(entt.getID());
	}

	template<typename T>
	void removeComponent(EntityID entt)
	{
		_entityManager.unregisterComponent(entt, Family::type_id<T>());
		_ComponentManager.removeComponent<T>(entt);
	}

	template<typename T>
	bool hasComponent(const Entity& entt) const noexcept
	{
		return entt.hasComponent(Family::type_id<T>());
	}
	template<typename T>
	bool hasComponent(EntityID entt) const noexcept
	{
		return _entityManager.get(entt).hasComponent(Family::type_id<T>());
	}

	/// @brief create new material and return ptr to it
	/// @param vertexShader - path to compiled vertex shader
	/// @param pixelShader  - path to compiled pixel shader
	/// @return - material with a given shaders
	std::shared_ptr<Material> makeMaterial(const wchar_t* vertexShader = L"VertexShader.cso", const wchar_t* pixelShader = L"PixelShader.cso");


	MeshPtr makeBoxMesh();

	MeshPtr makeCylinderMesh(unsigned int n = 8);

	MeshPtr make2SidedPlaneMesh();

	bool saveMesh(MeshPtr pMesh, const std::string& resourceID)
	{
		if (!pMesh)
			return false;
		_ResourceManager.saveMesh(pMesh, resourceID);
		return true;
	}

	MeshPtr loadMesh(const std::string& resourceID)
	{
		return _ResourceManager.getMesh(resourceID);
	}

	void onStartFrame();


	/// @brief update input with data from window
	/// @param wnd # window whose  as input handler
	void updateInput(const Window& wnd) noexcept;

	const Input& getInput() const noexcept;

	void onEndFrame();

	const Entity& getEntity(EntityID id) const noexcept;

	const auto& pools() const noexcept
	{
		return _ComponentManager.pools();
	}
private:
	Graphics _gfx;
	ResourceManager _ResourceManager;
	ComponentManager _ComponentManager;
	EntityManager _entityManager;
	Camera _mainCamera;
	Input _input;
};