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

/*void MeshManager::loadMesh(const std::string &sName)
{
	std::map<std::string , StaticMesh*>::iterator iter = m_mMeshs.find(sName);
	if (iter == m_mMeshs.end()) // We load a new mesh
	{	
		StaticMesh* pMesh = new StaticMesh();
		pMesh->Load("../../Data/obj/"+sName);
		std::cout<<"[LOADING] : "<<sName<<std::endl;
		m_mMeshs[sName] = pMesh;
	}
	else
		std::cerr<<"WARNING : Obj already loaded"<<sName<<std::endl;
}*/

StaticMesh* MeshManager::loadMesh(const std::string &sName)
{
	std::map<std::string , StaticMesh*>::iterator iter = m_mMeshs.find(sName);
	if (iter == m_mMeshs.end()) // We load a new mesh
	{	
		StaticMesh* pMesh = new StaticMesh();
		pMesh->LoadMesh(sName);
		std::cout<<"[LOADING] : "<<sName<<std::endl;
		m_mMeshs[sName] = pMesh;
		return pMesh;
	}
	else
	{
		std::cerr<<"WARNING : Obj already loaded"<<sName<<std::endl;
		return m_mMeshs[sName];
	}
}