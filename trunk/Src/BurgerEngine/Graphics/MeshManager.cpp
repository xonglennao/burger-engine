#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/StaticMesh.h"


MeshManager::MeshManager()
{

}

void MeshManager::clear()
{
	for (std::map<std::string , StaticMesh*>::iterator iter = m_mMeshs.begin() ; iter != m_mMeshs.end() ; ++iter)
	{
		delete (*iter).second;
	}
	m_mMeshs.clear();
}


StaticMesh* MeshManager::getMesh(const std::string &sName)
{
	std::map<std::string , StaticMesh*>::iterator iter = m_mMeshs.find(sName);
	if (iter == m_mMeshs.end())
	{	
		std::cerr<<"ERROR : couldn't find "<<sName<<std::endl;
		return NULL;
	}
	else
		return (*iter).second;

}

StaticMesh* MeshManager::loadMesh(const std::string &sName)
{
	std::map<std::string , StaticMesh*>::iterator iter = m_mMeshs.find(sName);
	if (iter == m_mMeshs.end()) // We load a new mesh
	{	
		StaticMesh* pMesh = new StaticMesh();
		std::cout<<"[LOADING] : "<<sName<<std::endl;
		pMesh->LoadMesh(sName);
		m_mMeshs[sName] = pMesh;
		return pMesh;
	}
	else
	{
		return m_mMeshs[sName];
	}
}