#include "mesh_pool.h"
#include "components/mesh.h"
#include "components/material.h"
#include "graphics/Drawable/BindComponent/vertex_shader.h"
#include "graphics/Drawable/BindComponent/pixel_shader.h"

#include <filesystem>
#include <fstream>

namespace fileSystem
{
	/// @brief get extension of file
	/// @param file name. or full file name
	/// @return 
	std::string getExtension(const std::string& file)
	{
		size_t lastDot = file.find_last_of('.', file.length());
		if (lastDot != std::string::npos)
		{
			return file.substr(lastDot + 1);
		}
		return {};
	}

	/// @brief get local file name without extension
	/// @param file some file name.
	/// @return 
	std::string getFilename(const std::string& file)
	{
		size_t lastSlash = file.find_last_of("/\\");
		size_t lastDot = file.find_last_of('.', file.length());
		if (lastSlash == std::string::npos)
		{
			return file;
		}

		return file.substr(lastSlash + 1, lastDot);
	}

	bool fileExist(const std::string& file)
	{
		return std::filesystem::exists(file);
	}
}

/// @brief //////////////////////RESOURCE MANAGER ////////////////////



ResourceManager::ResourceManager(Graphics& gfx) : _gfx(gfx)
{
}

/// @brief getPtr to exact resource.if resource isnot loaded then load it and return to user;
/// @param resourceID full path to resource;
/// @return 
MeshPtr ResourceManager::getMesh(const std::string& resourceID)
{
	auto it = meshes_.find(resourceID);
	if (it != meshes_.end())
	{
		return (it->second);
	}

	if (fileSystem::getExtension(resourceID) != "syrenmesh")
	{
		//TODO(log) an error
		return nullptr;
	}
	meshes_[resourceID] = std::make_shared<Mesh>();

	auto res = loadMeshInternal(meshes_[resourceID],resourceID);
	if (!res)
	{
		meshes_.erase(resourceID);
		return nullptr;
	}
	return meshes_[resourceID];
}

bool ResourceManager::saveMesh(const MeshPtr pMesh, const std::string& resourceID)
{
	if (fileSystem::getExtension(resourceID) != "syrenmesh")
	{
		//TODO(log) an error
		return false;
	}
	//TODO check if this pMesh is unhandled pMesh and make it handeled;
	return saveMeshInternal(pMesh, resourceID);
}

bool ResourceManager::loadMeshInternal(MeshPtr pMesh,const std::string& file)
{
	if(!fileSystem::fileExist(file))
		return false;

	std::ifstream fileHandler(file);
	if (!fileHandler.is_open()) {
		//std::cerr << "�� ������� ������� ����: " << filename << std::endl;
		return false;
	}
	std::string line;
	size_t vertexCount = 0u;
	size_t indexCount = 0u;
	size_t colorCount =  0u;

	if (!(fileHandler >> vertexCount))
	{
		return false;
	}
	pMesh->vertexes.reserve(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		auto& v = pMesh->vertexes.emplace_back();
		auto pos = v.position.m128_f32;
		if (!(fileHandler >> pos[0] >> pos[1] >> pos[2]))
		{
			return false;
		}

		if (!(fileHandler >> v.uv.x >> v.uv.y))
		{
			return false;
		}
	}


	if (!(fileHandler >> indexCount))
	{
		return false;
	}

	pMesh->indices.reserve(indexCount);

	for (size_t i = 0; i < indexCount; ++i)
	{
		auto& index = pMesh->indices.emplace_back();
		if (!(fileHandler >> index))
		{
			return false;
		}
	}

	if (!(fileHandler >> colorCount))
	{
		return false;
	}

	pMesh->colors.reserve(colorCount);

	for (size_t i = 0; i < colorCount; ++i)
	{
		auto& color = pMesh->colors.emplace_back();
		if (!(fileHandler >> color.r >> color.g >> color.b >> color.a))
		{
			return false;
		}
	}

	fileHandler.close();
	pMesh->resourceID = file;
	meshHelpers::updateBB(pMesh.get());

	return true;

}

bool ResourceManager::saveMeshInternal(const MeshPtr pMesh, const std::string& filename)
{
	std::ofstream file(filename);
	if (!file.is_open()) 
	{
		return false;
	}

	file << pMesh->vertexes.size() << std::endl;
	for (const auto& vertex : pMesh->vertexes) 
	{
		auto pos = vertex.position.m128_f32;
		file << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
		file << vertex.uv.x << " " << vertex.uv.y << std::endl;
	}

	file << pMesh->indices.size() << std::endl;
	for (const auto& index : pMesh->indices) 
	{
		file << index << std::endl;
	}

	file << pMesh->colors.size() << std::endl;
	for (const auto& color : pMesh->colors) 
	{
		file << color.r << " " << color.g << " " << color.b << " " << color.a << std::endl;
	}

	file.close();
	return true;
}
