#include "ModelLoader.h"


void ModelLoader::LoadOBJModel(std::string filename, std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    std::vector<DirectX::XMFLOAT3> positions;
    std::vector<DirectX::XMFLOAT2> texCoords;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            DirectX::XMFLOAT3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (prefix == "vt") {
            DirectX::XMFLOAT2 tex;
            iss >> tex.x >> tex.y;
            texCoords.push_back(tex);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; i++) {
                std::string vertexData;
                iss >> vertexData;
                std::istringstream vertexStream(vertexData);
                std::string vertexIndex, texIndex;
                std::getline(vertexStream, vertexIndex, '/');
                std::getline(vertexStream, texIndex, '/');

                Vertex vertex;
                vertex.Pos = positions[std::stoi(vertexIndex) - 1];
                vertex.Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);  // Default white color

                if (!texIndex.empty() && !texCoords.empty()) {
                    int texIdx = std::stoi(texIndex) - 1;
                    if (texIdx >= 0 && texIdx < texCoords.size()) {
                        vertex.TexCoord = texCoords[texIdx];
                    }
                    else {
                        vertex.TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f); // Default texture coordinates
                    }
                }
                else {
                    vertex.TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f); // Default texture coordinates
                }

                vertices.push_back(vertex);
                indices.push_back(static_cast<DWORD>(vertices.size() - 1));
            }
        }
    }
}
