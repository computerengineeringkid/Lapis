#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Graphics/Objects/GameObject.h"

// Forward declarations for Assimp classes
struct aiNode;
struct aiScene;
struct aiMesh;

class ModelLoader
{
public:
    static void LoadModel(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices);

private:
    static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices);
    static void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices);
};