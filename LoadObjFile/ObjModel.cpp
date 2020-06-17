#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ObjModel.h"

ObjModel::~ObjModel()
{
    // delete textures from gl buffers
    for (auto iter = textures_map.begin();
         iter != textures_map.end(); ++iter)
        glDeleteTextures(1, &iter->second);
}

void ObjModel::print_info()
{
    size_t mesh_num = meshes.size();
    for (size_t mh_id = 0; mh_id < mesh_num; ++mh_id)
    {
        MeshGLBuffer& mh = meshes[mh_id];
        std::cout << "mesh " << mh_id << ":\n"
            << "  " << mh.get_vertices().size() << " nodes\n"
            << "  " << mh.get_indices().size() << " indices\n"
            << "  " << mh.get_textures().size() << " textures\n";
        size_t tex_num = mh.get_textures().size();
        for (size_t t_id = 0; t_id < tex_num; ++t_id)
        {
            Texture& tex = mh.get_textures()[t_id];
            std::cout << "    " << tex.filename << " " << tex.type << " " << tex.id << "\n";
        }
    }
}

void ObjModel::load_model(const char *model_filename)
{
    std::string md_filename(model_filename);
    
    // import model
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(md_filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ASSIMP::ERROR::" << importer.GetErrorString() << "\n";
        return;
    }

    // retrieve the directory of filename
    // for loading textures
    directory = md_filename.substr(0, md_filename.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}

void ObjModel::process_node(aiNode* node, const aiScene* scene)
{
    // process meshes at current node
    for (GLuint mh_id = 0; mh_id < node->mNumMeshes; ++mh_id)
    {
        // node only has index to actual objects in scene. 
        // the scene has all data and uses tree to organize objects
        aiMesh* mesh = scene->mMeshes[node->mMeshes[mh_id]];
        meshes.emplace_back();
        MeshGLBuffer &mesh_buffer = meshes.back();
        process_mesh(mesh, scene, mesh_buffer);
    }

    // recursively process children nodes
    if (node->mNumChildren) node->mNumChildren = 2; //debug
    for (GLuint c_id = 0; c_id < node->mNumChildren; ++c_id)
        process_node(node->mChildren[c_id], scene);
}

void ObjModel::process_mesh(
    aiMesh* mesh,
    const aiScene* scene,
    MeshGLBuffer& buffer
    )
{
    std::vector<Vertex> &vertices = buffer.get_vertices();
    vertices.reserve(mesh->mNumVertices);
    for (GLuint v_id = 0; v_id < mesh->mNumVertices; ++v_id)
    {
        vertices.emplace_back();
        Vertex& vertex = vertices.back();
        // position
        glm::vec3 &pos = vertex.position;
        pos.x = mesh->mVertices[v_id].x;
        pos.y = mesh->mVertices[v_id].y;
        pos.z = mesh->mVertices[v_id].z;
        // normal
        glm::vec3& normal = vertex.normal;
        normal.x = mesh->mNormals[v_id].x;
        normal.y = mesh->mNormals[v_id].y;
        normal.z = mesh->mNormals[v_id].z;
        // texture coordinate
        glm::vec2& tex_coord = vertex.tex_coord;
        if (mesh->mTextureCoords[0]) // If has texture coordinates
        {
            // A vertex may have up to 8 texture coordinates
            // we just use the first one
            tex_coord.x = mesh->mTextureCoords[0][v_id].x;
            tex_coord.y = mesh->mTextureCoords[0][v_id].y;
        }
        else
        {
            tex_coord.x = 0.0f;
            tex_coord.y = 0.0f;
        }
        // tangent
        glm::vec3& tangent = vertex.tangent;
        tangent.x = mesh->mTangents[v_id].x;
        tangent.y = mesh->mTangents[v_id].y;
        tangent.z = mesh->mTangents[v_id].z;
        // bitangent
        glm::vec3 bitangent = vertex.bitangent;
        bitangent.x = mesh->mBitangents[v_id].x;
        bitangent.y = mesh->mBitangents[v_id].y;
        bitangent.z = mesh->mBitangents[v_id].z;
    }

    std::vector<GLuint> &indices = buffer.get_indices();
    indices.reserve(mesh->mNumFaces * 3);
    for (GLuint f_id = 0; f_id < mesh->mNumFaces; ++f_id)
    {
        aiFace &face = mesh->mFaces[f_id];
        for (GLuint i_id = 0; i_id < face.mNumIndices; ++i_id)
            indices.push_back(face.mIndices[i_id]);
    }

    std::vector<Texture> &textures = buffer.get_textures();
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // sampler names convention:
    // Each diffuse texture is named as 'texture_diffuseN' where
    // N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture below:
    // diffuse maps: texture_diffuseN
    load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
    // specular maps: texture_specularN
    load_material_textures(material, aiTextureType_SPECULAR, "texture_specular", textures);
    // normal maps: texture_normalN ??
    load_material_textures(material, aiTextureType_HEIGHT, "texture_normal", textures);
    // height maps: texture_heightN ??
    load_material_textures(material, aiTextureType_AMBIENT, "texture_height", textures);

    buffer.setup_gl_buffer();
}

// helper functions
static GLuint load_texture_from_file(
    const char* texture_filename,
    const std::string& directory
)
{
    std::string filename = std::string(texture_filename);
    filename = directory + '/' + filename;

    GLuint textureID = 0;
    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        switch (nrComponents)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            break;
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture from "
            << texture_filename << "\n";
        stbi_image_free(data);
    }

    return textureID;
};

void ObjModel::load_material_textures(
    aiMaterial* mat,
    aiTextureType type,
    const char *typeName,
    std::vector<Texture>& textures
    )
{
    GLuint texture_id;
    GLuint texture_num = mat->GetTextureCount(type);
    for (GLuint t_id = 0; t_id < texture_num; ++t_id)
    {
        aiString str;
        mat->GetTexture(type, t_id, &str);
        auto iter = textures_map.find(str.C_Str());
        if (iter != textures_map.end())
        {
            texture_id = iter->second;
        }
        else
        {
            texture_id = load_texture_from_file(str.C_Str(), this->directory);
            textures_map.insert(std::pair<std::string, GLuint>(str.C_Str(), texture_id));
            if (!texture_id)
            {
                std::cout << "Error::ObjModel:: can't find texture image " << str.C_Str()
                          << " in path " << directory << ".\n";
            }
        }
        
        if (texture_id)
        {
            textures.emplace_back();
            Texture &texture = textures.back();
            texture.filename = str.C_Str();
            texture.type = typeName;
            texture.id = texture_id;
        }
    }
}
