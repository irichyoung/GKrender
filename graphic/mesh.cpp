#include"mesh.h"
#include"..\base\gkdebug.h"
#include"render.h"
#include<DirectXMath.h>
#include"WICTextureLoader.h"
#include<fstream>
#include"..\assimp\cimport.h"
using namespace gk::graphic;
bool mesh::load(const char*filename,float x,float y,float z)
{
	bool error = false;
	m_xpos = x;
	m_ypos = y;
	m_zpos = z;
	m_render = render::getinstance();
	Assimp::Importer imp;
	unsigned int ppsteps = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure | // perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		aiProcess_PreTransformVertices |
		0;
	aiPropertyStore* props = aiCreatePropertyStore();
	aiSetImportPropertyInteger(props, AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
	aiSetImportPropertyFloat(props, AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.f);
	aiSetImportPropertyInteger(props, AI_CONFIG_PP_SBP_REMOVE, false ? aiPrimitiveType_LINE | aiPrimitiveType_POINT : 0);

	aiSetImportPropertyInteger(props, AI_CONFIG_GLOB_MEASURE_TIME, 1);
	//const aiScene* pScene = imp.ReadFile(filename, ppsteps | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FixInfacingNormals | aiProcess_ConvertToLeftHanded | aiProcess_SortByPType);
	const aiScene* pScene = (aiScene*)aiImportFileExWithProperties(filename,
		ppsteps | /* configurable pp steps */
		aiProcess_GenSmoothNormals | // generate smooth normal vectors if not existing
		aiProcess_SplitLargeMeshes | // split large, unrenderable meshes into submeshes
		aiProcess_Triangulate | // triangulate polygons with more than 3 edges
		aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space
		aiProcess_SortByPType | // make 'clean' meshes which consist of a single typ of primitives
		0,
		NULL,
		props);
	if (!pScene){
		GKDebug("%s", imp.GetErrorString());
		return false;
	}
	for (UINT32 i = 0; i < pScene->mNumMeshes; i++){//pScene->mNumMeshes
		m_submesh.push_back(new submesh());
		int num = m_submesh.size() - 1;
		auto mat = pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex];
		if (mat){
			aiColor4D tem;
			if (AI_SUCCESS != aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &tem)){
				tem = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
			m_submesh[num]->diffuser = tem.r;
			m_submesh[num]->diffuseg = tem.g;
			m_submesh[num]->diffuseb = tem.b;
			m_submesh[num]->diffusea = tem.a;
		}
		if (pScene->mMeshes[i]->mTextureCoords[0]){
			aiString file;
			m_submesh[num]->m_hastex = true;
			if (AI_SUCCESS == aiGetMaterialString(mat, AI_MATKEY_TEXTURE_DIFFUSE(0), &file));
			loadTexture(UTF8TOWIDECHAR(std::string(file.C_Str())), m_submesh[num]);
		}
		if (pScene->mMeshes[i]->mPrimitiveTypes == aiPrimitiveType_LINE)
			m_submesh[num]->m_primitive = PRIMITIVE::GK_PRIMITIVE_LINE;
		else if (pScene->mMeshes[i]->mPrimitiveTypes == aiPrimitiveType_POINT)
			m_submesh[num]->m_primitive = PRIMITIVE::GK_PRIMITIVE_POINT;
		else m_submesh[num]->m_primitive = PRIMITIVE::GK_PRIMITIVE_TRIANGLE;
		for (UINT32 j = 0; j < pScene->mMeshes[i]->mNumFaces; j++){
			auto n = pScene->mMeshes[i]->mFaces[j].mNumIndices;
			if (n != 3 && n != 2 && n != 1){
				GKDebug("FILE:%s,Error:not triangle num:%d,primitive:%d\n", filename, i, pScene->mMeshes[i]->mPrimitiveTypes);
				m_submesh.pop_back();
				error = true;
				break;
			}
			if (m_submesh[num]->m_primitive == PRIMITIVE::GK_PRIMITIVE_POINT){
				m_submesh[num]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[0]);
			}
			else if (pScene->mMeshes[i]->mPrimitiveTypes == aiPrimitiveType_LINE){
				m_submesh[num]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[0]);
				m_submesh[num]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[1]);
			}
			else {
				m_submesh[num]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[0]);
				m_submesh[num]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[1]);
				m_submesh[num]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[2]);
			}
		}
		if (error){
			error = false;
			continue;
		}
		m_submesh[num]->m_mesh = this;
		m_submesh[num]->m_program = new cgprogram();
		m_submesh[num]->m_program->load(L"shader\\oridinary.fx");
		for (UINT32 j = 0; j < pScene->mMeshes[i]->mNumVertices; j++){
			vertex newv;
			if (pScene->mMeshes[i]->HasNormals()){
				newv.norx = pScene->mMeshes[i]->mNormals[j].x;
				newv.nory = pScene->mMeshes[i]->mNormals[j].y;
				newv.norz = pScene->mMeshes[i]->mNormals[j].z;
			}
			newv.posx = pScene->mMeshes[i]->mVertices[j].x;
			newv.posy = pScene->mMeshes[i]->mVertices[j].y;
			newv.posz = pScene->mMeshes[i]->mVertices[j].z;
			if (pScene->HasTextures()){
				newv.texu = pScene->mMeshes[i]->mTextureCoords[j][0].x;
				newv.texu = pScene->mMeshes[i]->mTextureCoords[j][0].y;
			}
			m_submesh[num]->m_vertex.push_back(newv);
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertex) * (pScene->mMeshes[i]->mNumVertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &m_submesh[num]->m_vertex[0];
		auto hr = m_render->getdevice()->CreateBuffer(&bd, &InitData, &m_submesh[num]->m_vertexbuff);
		if (hr != S_OK){
			GKDebug("Can not load:%s,Error:%s", filename, GKGetError(true).c_str());
			return false;
		}

		D3D11_BUFFER_DESC bdd;
		ZeroMemory(&bdd, sizeof(bdd));
		bdd.Usage = D3D11_USAGE_DEFAULT;
		bdd.ByteWidth = sizeof(UINT) * 3 * pScene->mMeshes[i]->mNumFaces;
		bdd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bdd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData1;
		ZeroMemory(&InitData1, sizeof(InitData1));
		InitData1.pSysMem = &m_submesh[num]->m_index[0];
		hr = m_render->getdevice()->CreateBuffer(&bdd, &InitData1, &m_submesh[num]->m_indexbuff);
		if (hr != S_OK){
			GKDebug("Can not load:%s,Error:%s", filename, GKGetError(true).c_str());
			return false;
		}
	}
	return true;
}
bool mesh::draw()
{
	for (auto &x : m_submesh){

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elenum = ARRAYSIZE(layout);
		ID3D11InputLayout *vertexlay;
		FUN_ERR(m_render->getdevice()->CreateInputLayout(layout, elenum, x->m_program->getm_vb()->GetBufferPointer(), x->m_program->getm_vb()->GetBufferSize(), &vertexlay));
		m_render->getcontext()->IASetInputLayout(vertexlay);

		UINT stride = sizeof(vertex);
		UINT offset = 0;
		m_render->getcontext()->IASetVertexBuffers(0, 1, &x->m_vertexbuff, &stride, &offset);
		m_render->getcontext()->IASetIndexBuffer(x->m_indexbuff, DXGI_FORMAT_R32_UINT, 0);
		if (x->m_primitive == PRIMITIVE::GK_PRIMITIVE_POINT)
			m_render->getcontext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		else if (x->m_primitive == PRIMITIVE::GK_PRIMITIVE_LINE)
			m_render->getcontext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		else m_render->getcontext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		auto world = DirectX::XMMatrixTranslation(m_xpos, m_ypos, m_zpos);
		auto view = m_render->getcurrentcamera()->getViewMatrix();
		auto proj = m_render->getcurrentcamera()->getProjMatrix();
		percbuff buff;
		buff.m_view = DirectX::XMMatrixTranspose(view);
		buff.m_proj = DirectX::XMMatrixTranspose(proj);
		buff.m_world = DirectX::XMMatrixTranspose(world);
		auto cbuff = m_render->getm_percbuff();
		m_render->getcontext()->UpdateSubresource(cbuff, 0, NULL, &buff, 0, 0);
		inscbuff ibuff;
		ibuff.m_diffuse = DirectX::XMFLOAT4(x->diffuser, x->diffuseg, x->diffuseb, x->diffusea);
		auto inscbuff = m_render->getm_inscbuff();
		m_render->getcontext()->UpdateSubresource(inscbuff, 0, NULL, &ibuff, 0, 0);

		if (x->m_hastex){
			m_render->getcontext()->PSSetShaderResources(0, 1, &x->m_srv);
			m_render->getcontext()->PSGetSamplers(0, 1, m_render->getm_sampler());
		}
		m_render->getcontext()->RSSetState(m_render->getm_raster());
		m_render->getcontext()->VSSetShader(x->m_program->getm_vs(), NULL, NULL);
		m_render->getcontext()->VSSetConstantBuffers(0, 1, &cbuff);
		m_render->getcontext()->PSSetShader(x->m_program->getm_ps(), NULL, NULL);
		m_render->getcontext()->PSSetConstantBuffers(1, 1, &inscbuff);
		m_render->getcontext()->DrawIndexed(x->m_index.size() * 3, 0, 0);
	}
	return true;
}
bool mesh::loadTexture(const std::wstring& file, submesh *submesh){
	FUN_ERR(DirectX::CreateWICTextureFromFile(m_render->getdevice(), file.c_str(), &submesh->m_tex, &submesh->m_srv));
	return true;
}
