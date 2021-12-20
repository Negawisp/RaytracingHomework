#ifndef __SCENE_LOADER_H__
#define __SCENE_LOADER_H__

#include "loader_utils/hydraxml.h"
#include "cmesh.h"
#include "../utils/logging.h"

using std::shared_ptr;

class SceneMeshes {
public:
    shared_ptr<cmesh::SimpleMesh> getMesh(unsigned int number);
    bool LoadSceneXML(const std::string& scenePath);

private:
    void AddMeshFromFile(const std::string& meshPath);
    std::vector<shared_ptr<cmesh::SimpleMesh>> _meshes;
};


shared_ptr<cmesh::SimpleMesh> SceneMeshes::getMesh(unsigned int number) {
    return _meshes[number];
}

bool SceneMeshes::LoadSceneXML(const std::string& scenePath) {

    auto hscene_main = std::make_shared<hydra_xml::HydraScene>();
    auto res = hscene_main->LoadState(scenePath);

    if (res < 0) {
        RUN_TIME_ERROR("LoadSceneXML error");
        return false;
    }

    for (auto loc : hscene_main->MeshFiles()) {
        AddMeshFromFile(loc);
        auto instances = hscene_main->GetAllInstancesOfMeshLoc(loc);
    }

    return true;
}


void SceneMeshes::AddMeshFromFile(const std::string& meshPath)
{
    cmesh::SimpleMesh data = cmesh::LoadMeshFromVSGF(meshPath.c_str());

    if (data.VerticesNum() == 0) {
        RUN_TIME_ERROR(("can't load mesh at " + meshPath).c_str());
    }

    _meshes.push_back(std::make_shared<cmesh::SimpleMesh>(data));
}



#endif // !__SCENE_LOADER_H__
