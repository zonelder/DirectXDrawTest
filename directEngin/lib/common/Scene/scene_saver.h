#ifndef __SCENE_SAVER_H__
#define __SCENE_SAVER_H__

#include "resmngr/xml_node.h"
#include "scene_manager.h"


#include <iostream>
#include <fstream>

class SceneSaver {
public:
    void save(const std::string& path, const SceneManager& scene) {
        // ������� �������� XML
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("Scene");  // ������� �������� ���� "Scene"

        // ����������� SceneManager
        Serializer<SceneManager>::serialize(root, scene);

        // ��������� �������� � ����
        std::ofstream file(path);
        doc.save(file, "\t", pugi::format_default);
    }
};

#endif 