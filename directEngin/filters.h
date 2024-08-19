#pragma once
#include <tuple>
#include "common/Scene/component_manager.h"
#include "common/Scene/Entity.h"

namespace filters
{

	template<class Wt, class Wtout>
	class ComponentView;
	struct filter_type {};
	template<class... Args>
	struct With
	{
		using ComponentFilterWith = filter_type;
	};



	template<class... Args>
	struct Without
	{
		using t = With<Args...>;
		using ComponentFilterWithout = filter_type;
	};


	template<class TWith, class TWithout>
	concept is_component_filters = requires
	{
		typename TWith::ComponentFilterWith;
		typename TWithout::ComponentFilterWithout;
	};

	template<class T>
	concept is_with_filter = requires{
		typename T::ComponentFilterWith;
	};

	template<class T>
	concept is_without_filter = requires{
		typename T::ComponentFilterWithout;
	};

	template<class T>
	concept is_component_filter = is_with_filter<T> || is_without_filter<T>;

	template<class ...Args>
	concept has_component_filters = (is_component_filter<Args> ||...);

	template<class... Args>
	concept has_not_filters = !has_component_filters<Args...>;

	template<typename Pools, typename Entity, size_t I = 0>
	inline bool all_of(const Pools& pools, const Entity& entt) noexcept
	{
		if constexpr (I >= std::tuple_size_v<Pools>)
			return true;
		else
		{
			if (!std::get<I>(pools)->contains(entt))
				return false;
			return all_of<Pools, Entity, I + 1>(pools, entt);
		}
	}

	template<typename Pools, typename Entity, size_t I = 0>
	inline bool none_of(const Pools& pools, const Entity& entt) noexcept
	{
		if constexpr (I >= std::tuple_size_v<Pools>)
			return true;
		else
		{
			if (std::get<I>(pools)->contains(entt))
				return false;
			return none_of<Pools, Entity, I + 1>(pools, entt);
		}
	}

	template<class... WithArgs, class... WithoutArgs>
	class ComponentView<With<WithArgs...>, Without<WithoutArgs...>>
	{
	public:

		using with_tuple = std::tuple< ComponentPool<WithArgs>*...>;
		using without_tuple = std::tuple<ComponentPool< WithoutArgs>*...>;
		using entity_iterator = std::vector<EntityID>::iterator;

		ComponentView(ComponentManager& scene) :
			_includes({ scene.getPool<WithArgs>()... }),
			_excludes({ scene.getPool<WithoutArgs>()... })
		{

		}

		class iterator
		{
			using entity_iterator_type = entity_iterator;
		public:
			iterator(with_tuple& include, without_tuple& exclude, entity_iterator_type it, entity_iterator_type end) : _include(include), _exclude(exclude), _it(it), _end(end)
			{

			}

			iterator(with_tuple& include, without_tuple& exclude, entity_iterator_type it) : iterator(include, exclude, it, it) {}

			iterator operator++(int) const noexcept
			{
				auto copy = *this;
				++(*this);

				return copy;
			}

			iterator& operator++() noexcept
			{
				while (++_it != _end)
				{
					auto entt = *_it;
					if (all_of(_include, entt) && none_of(_exclude, entt))
						break;
				}
				return *this;
			}

			bool operator!=(const iterator& other) const noexcept
			{
				return _it != other._it;
			}

			bool operator==(const iterator& other) const noexcept
			{
				return _it == other._it;
			}

			auto operator*() noexcept
			{
				const auto& entt = *_it;
				return std::tuple<const EntityID&, WithArgs&...>(
					entt,
					(std::get<ComponentPool<WithArgs>*>(_include)->operator[](entt))...
				);
			}


		private:

			with_tuple& _include;
			without_tuple& _exclude;
			entity_iterator_type _it;
			entity_iterator_type _end;
		};


		template<class T>
		T& get(const Entity& entt) noexcept
		{
			return get<T>(entt.getID());
		}

		template<class T>
		auto& get(const EntityID& entt) noexcept
		{
			static_assert (isWith<T>);
			return std::get< ComponentPool<T>*>(_includes)->operator[](entt);
		}


		template<class T>
		auto contains(const EntityID& entt) noexcept
		{
			return all_of(_includes, entt) && none_of(_excludes, entt);
		}

		template<class T>
		auto has(const EntityID& entt) noexcept
		{
			static_assert (isWith<T>);
			return std::get< ComponentPool<T>*>(_includes)->contains(entt);
		}

		template<class T>
		static constexpr bool isWith = (... || std::is_same_v<T, WithArgs>);

		iterator begin() noexcept
		{
			auto* pool = std::get<0>(_includes);
			return iterator(_includes, _excludes, pool->ebegin(), pool->eend());
		}

		iterator end() noexcept
		{
			auto* pool = std::get<0>(_includes);
			return iterator(_includes, _excludes, pool->eend());
		}

	private:
		with_tuple _includes;
		without_tuple _excludes;
	};
}

