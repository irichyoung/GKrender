#include"mesh.h"
#include"..\base\gkdebug.h"
#include"graphic\render.h"
#include<fstream>
using namespace gk::graphic;
bool mesh::load(const char*filename)
{
	m_render = render::getinstance();
	Assimp::Importer imp;
	const aiScene*pScene = imp.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (!pScene){
		GKDebug("%s", imp.GetErrorString());
		return false;
	}

	for (UINT32 i = 0; i < pScene->mNumMeshes; i++){
		m_submesh.push_back(new submesh());
		/*Load the vertex into system memory*/
		for (UINT32 j = 0; j < pScene->mMeshes[i]->mNumVertices; j++){
			vertex newv;
			newv.norx = pScene->mMeshes[i]->mNormals[j].x;
			newv.nory = pScene->mMeshes[i]->mNormals[j].y;
			newv.norz = pScene->mMeshes[i]->mNormals[j].z;
			newv.posx = pScene->mMeshes[i]->mVertices[j].x;
			newv.posy = pScene->mMeshes[i]->mVertices[j].y;
			newv.posz = pScene->mMeshes[i]->mVertices[j].z;
			newv.texu = pScene->mMeshes[i]->mTextureCoords[j][0].x;
			newv.texu = pScene->mMeshes[i]->mTextureCoords[j][0].y;
			m_submesh[i]->m_vertex.push_back(newv);
		}
		/*Load the index into system memory*/
		for (UINT32 j = 0; j < pScene->mMeshes[i]->mNumFaces; j++){
			if (pScene->mMeshes[i]->mFaces[j].mNumIndices != 3){
				GKDebug("FILE:%s,Error:not triangle", filename);
				return false;
			}
			m_submesh[i]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[0]);
			m_submesh[i]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[1]);
			m_submesh[i]->m_index.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[2]);
		}
		/*Create the Vertex/Index buffer*/
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertex) * (pScene->mMeshes[i]->mNumVertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &m_submesh[i]->m_vertex[0];
		auto hr = render::getinstance()->getdevice()->CreateBuffer(&bd, &InitData, &m_submesh[i]->m_vertexbuff);
		if (hr != S_OK){
			GKDebug("Can not load:%s,Error:%s", filename, GKGetError(true).c_str());
			return false;
		}

		D3D11_BUFFER_DESC bdd;
		bdd.Usage = D3D11_USAGE_DEFAULT;
		bdd.ByteWidth = sizeof(UINT32) * 3 * pScene->mMeshes[i]->mNumFaces;
		bdd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bdd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData1;
		ZeroMemory(&InitData1, sizeof(InitData1));
		InitData1.pSysMem = &m_submesh[i]->m_index[0];
		hr = render::getinstance()->getdevice()->CreateBuffer(&bdd, &InitData1, &m_submesh[i]->m_indexbuff);
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
		m_render->getcontext()->IASetIndexBuffer(x->m_indexbuff, DXGI_FORMAT_R32_UINT, 0);
		m_render->getcontext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		UINT stride = sizeof(vertex);
		UINT offset = 0;
		m_render->getcontext()->IASetVertexBuffers(0, 1, &x->m_vertexbuff, &stride, &offset);
		setEffect(x, L"oridinary.fxo");
		auto tec = x->m_effect->GetTechniqueByName("tec");
		auto pass = tec->GetPassByName("p0");
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};
		UINT elenum = ARRAYSIZE(layout);
		D3DX11_PASS_DESC passdes;
		ID3D11InputLayout *vertexlay;
		FUN_ERR(pass->GetDesc(&passdes));
		FUN_ERR(m_render->getdevice()->CreateInputLayout(layout, elenum, passdes.pIAInputSignature, passdes.IAInputSignatureSize, &vertexlay));
		m_render->getcontext()->IASetInputLayout(vertexlay);
		pass->Apply(0,m_render->getcontext());
		m_render->getcontext()->DrawIndexed(x->m_index.size(), 0, 0);
	}
	return true;
}
bool mesh::add(){
	m_render->addObject(this);
	return true;
}
bool mesh::setEffect(submesh * m,std::wstring e){
	std::ifstream fin(e.c_str(), std::ifstream::binary);
	fin.seekg(0, std::ifstream::end);
	int size = static_cast<int>(fin.tellg());
	fin.seekg(0, std::ifstream::beg);
	std::vector<char>compiledshader(size);
	fin.read(&compiledshader[0], size);
	fin.close();
	FUN_ERR(D3DX11CreateEffectFromMemory(&compiledshader[0], size, 0, m_render->getdevice(), &m->m_effect));

}