/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/08/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __MESHMANAGER_H__
#define __MESHMANAGER_H__

#include <string>
#include <map>

#include "BurgerEngine/Base/Singleton.h"


class StaticMesh;

class MeshManager:public Singleton<MeshManager>
{
	friend class Singleton<MeshManager>;

	public:
		//void loadMesh(const std::string& sName);
		StaticMesh* loadMesh(const std::string& sName);
		StaticMesh* getMesh(const std::string& sName);

		void clear();
	
	private:
		std::map<std::string,StaticMesh*> m_mMeshs;

		MeshManager();
		virtual ~MeshManager(){clear();};

};


#endif //__MESHMANAGER_H__