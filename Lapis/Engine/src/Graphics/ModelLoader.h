#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Windows.h"
#include <DirectXMath.h>
#include "Graphics/Objects/GameObject.h"
class ModelLoader
{
    

public:

    static void LoadOBJModel(std::string filename, std::vector<Vertex>& vertices, std::vector<DWORD>& indices);
       
   
};

