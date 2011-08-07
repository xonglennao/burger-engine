#include "StaticMesh.h"

#include <sstream>
#include <fstream>
#include <assert.h>
#include <string>
#include <algorithm>

/// \todo put those in a separate file 
#define NEXT_INDICE do{i++;}while((cBuffer[i]<'0')&&(cBuffer[i]>'9'));


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StaticMesh::StaticMesh()
	: m_iBufferId(0)
	, m_iSizeVertex(0)
	, m_iSizeNormal(0)
	, m_iSizeTexture(0)
	, m_iSizeTangent(0)
	, m_pBoundingBox( NULL )
{
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StaticMesh::LoadMesh(std::string const& a_sFilename)
{
	
	if(a_sFilename.find(".obj") != std::string::npos) 
	{
		// \todo Create a macro ASSERT with message
		assert(LoadFromFile(a_sFilename));
		//std::cerr << "[ERROR]: " << a_sFilename << " couldn't be loaded" << std::endl;
		//return false;
	}
	else 
	{
		std::cerr << "[WARNING]: " << a_sFilename << " not a Valid format" << std::endl;
		return false;
	}


	ComputeTangents();
	
	BuildBuffer();



	return true;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StaticMesh::LoadFromFile(std::string const& a_sFilename)
{
	/// Create a pointer at the beginning of the file
	std::ifstream pFile(a_sFilename.c_str(), std::ios::binary);
	if(!pFile)
	{
		return false;
	}

	sMeshTriangle sTriangle;

	vec3 f3TemporaryVertex;
	vec2 f2TemporaryCoordinates;
	//char buftmp[64];
	long g;	// compteur pour le stockage des données lors de la seconde passe

	unsigned int	iIndiceTrianglePosition;
	unsigned int	iIndiceTriangleNormal;
	unsigned int	iIndiceTriangleCoord;

	std::vector<vec3>	vf3TempPosition;
	std::vector<vec3>	vf3TempNormal;
	std::vector<vec2>	vf2TempTexcoord;

	std::string sBuffer;

	// Start to read the file
	while(std::getline(pFile, sBuffer))
	{
		std::stringstream sStream(sBuffer);
		const char* cBuffer = sBuffer.c_str();

		// the read buffer is not empty
		if (sBuffer.size() > 0) 
		{
			std::string sTempBuffer;
			sStream >> sTempBuffer;

			if(sTempBuffer == "#") 
			{	
				// This is a comment
			}
			else if(sTempBuffer == "mtllib")
			{	
				/// \todo Handle material
				//materials->Load(&buf[7]);	
			}
			else if(sTempBuffer == "v") 
			{	
				// Found a Vertex
				sscanf(&sBuffer[2],"%f%f%f",&f3TemporaryVertex.x,&f3TemporaryVertex.y,&f3TemporaryVertex.z);

				vf3TempPosition.push_back(f3TemporaryVertex);
			}
			else if(sTempBuffer == "vt") 
			{	
				// Found a TextCoord
				sscanf(&sBuffer[2],"%f%f",&f2TemporaryCoordinates.x,&f2TemporaryCoordinates.y);
				f2TemporaryCoordinates.y = 1.0f - f2TemporaryCoordinates.y;
				vf2TempTexcoord.push_back(f2TemporaryCoordinates);
			} 
			else if(sTempBuffer == "vn")
			{
				// Found a Normal
				sscanf(&sBuffer[2],"%f%f%f",&f3TemporaryVertex.x,&f3TemporaryVertex.y,&f3TemporaryVertex.z);
				f3TemporaryVertex = normalize( f3TemporaryVertex );
				vf3TempNormal.push_back(f3TemporaryVertex);
			} 
			else if(sTempBuffer == "g") 
			{	
				// Found a Group
				if(sBuffer.size() > 1) 
				{
					sMeshGroup sGroup;
					sGroup.m_sName = &cBuffer[2];
					sGroup.m_lMaterial = 0;
					m_vGroup.push_back(sGroup);
				}
			} 
			else if(sTempBuffer == "usemtl") 
			{	
				// Found a material to use
				if(m_vGroup.size() <= 0)
				{
					sMeshGroup sGroup;
					sGroup.m_sName = "No Name";
					sGroup.m_lMaterial = 0;
					m_vGroup.push_back(sGroup);
					g = 0;
				}
				else 
				{
					g = (long)m_vGroup.size() - 1;
				}
				//m_tGroup[g].Material = materials->GetId(&buf[7]);	// on récupère son id
			}
			else if(sTempBuffer == "f")
			{	
				// Found a triangle
				int iMax;
				if( vf3TempPosition.size() >= vf3TempNormal.size() )
				{
					iMax = 0;
				}
				else
				{
					iMax = 1;
				}

				if(iMax == 0 && vf2TempTexcoord.size() > vf3TempPosition.size() )
				{
					iMax = 2;
				}
				
				if(iMax == 1 && vf2TempTexcoord.size() > vf3TempNormal.size() )
				{
					iMax = 2;
				}

				//m_tNormal.resize( m_tPosition.size() );
				//m_tTexcoord.resize( m_tPosition.size() );

				switch(iMax) 
				{
				case 0:
					m_vf3Position.resize( vf3TempPosition.size() );
					m_vf3Normal.resize( vf3TempPosition.size() );
					m_vf3Texcoord.resize( vf3TempPosition.size() );
					break;
				case 1: 
					m_vf3Position.resize( vf3TempNormal.size() );
					m_vf3Normal.resize( vf3TempNormal.size() );
					m_vf3Texcoord.resize( vf3TempNormal.size() );
					break;
				case 2: 
					m_vf3Position.resize( vf2TempTexcoord.size() );
					m_vf3Normal.resize( vf2TempTexcoord.size() );
					m_vf3Texcoord.resize( vf2TempTexcoord.size() );
					break;
				}

				if(m_vGroup.size() <= 0)
				{
					sMeshGroup sGroup;
					sGroup.m_sName = "No Name";
					sGroup.m_lMaterial = 0;
					m_vGroup.push_back(sGroup);
					g = 0;
				}
				else
				{
					g = (long)m_vGroup.size() - 1;
				}

				// We set the offset to the first value
				unsigned int i;
				for(i=0; (cBuffer[i] < '0') || (cBuffer[i] > '9') ;++i);	
				
				for(unsigned int v=0; v < 3 ;v++) 
				{ 
					// Triangle with 3 Vertex
					iIndiceTrianglePosition = 0;
					for(; (cBuffer[i] >= '0') && (cBuffer[i] <= '9') ;i++) 
					{	
						//First Vertex
						iIndiceTrianglePosition *= 10;	
						// 0x30 -> ASCII value of '0'
						iIndiceTrianglePosition += cBuffer[i]-0x30; 
					}

					// Indice goes from 0 to NumberOfFace-1
					iIndiceTrianglePosition--;	
					
					// We go to next value
					NEXT_INDICE;
					
					iIndiceTriangleCoord = 0;
					for(; (cBuffer[i] >= '0') && (cBuffer[i] <= '9') ;i++) 
					{	
						// First TextCoord
						iIndiceTriangleCoord *= 10;	
						iIndiceTriangleCoord += cBuffer[i]-0x30;
					}

					iIndiceTriangleCoord--;	
					NEXT_INDICE;

					iIndiceTriangleNormal = 0;
					for(; (cBuffer[i] >= '0') && (cBuffer[i] <= '9') ;i++) 
					{
						// First normal
						iIndiceTriangleNormal *= 10;	
						iIndiceTriangleNormal += cBuffer[i]-0x30;
					}

					iIndiceTriangleNormal--;	
					if(v < 2) 
					{
						NEXT_INDICE;
					}

					unsigned int iIndex = 0;
					switch(iMax) {
					case 0: {iIndex = iIndiceTrianglePosition;	break;}
					case 1: {iIndex = iIndiceTriangleNormal;	break;}
					case 2: {iIndex = iIndiceTriangleCoord;	break;}
					}

					m_vf3Position[iIndex] = vf3TempPosition[iIndiceTrianglePosition];
					m_vf3Normal[iIndex] = vf3TempNormal[iIndiceTriangleNormal];
					m_vf3Texcoord[iIndex] = vf2TempTexcoord[iIndiceTriangleCoord];
					sTriangle.ind[v] = iIndex;
		
				}

				//Save the triangle
				m_vGroup[g].m_vsTriangle.push_back(sTriangle); 			
			}
		}
	}
	pFile.close();

	FindSignificantVertex();

	vf3TempPosition.clear();
	vf3TempNormal.clear();
	vf2TempTexcoord.clear();

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StaticMesh::FindSignificantVertex()
{
	std::vector<vec3>::iterator oPosIt = m_vf3Position.begin();
	std::vector<vec3>::iterator oEnd = m_vf3Position.end();
	
	if( oPosIt != oEnd )
	{
		//xmin, xmax, ymin, ymax, zmin, zmax
		m_pBoundingBox = new float[6];

		m_pBoundingBox[0] = m_pBoundingBox[1] = (*oPosIt).x;
		m_pBoundingBox[2] = m_pBoundingBox[3] = (*oPosIt).y;
		m_pBoundingBox[4] = m_pBoundingBox[5] = (*oPosIt).z;
		
		++oPosIt;
		while( oPosIt != oEnd )
		{
			m_pBoundingBox[0] = min( m_pBoundingBox[0], (*oPosIt).x );
			m_pBoundingBox[1] = max( m_pBoundingBox[1], (*oPosIt).x );

			m_pBoundingBox[2] = min( m_pBoundingBox[2], (*oPosIt).y );
			m_pBoundingBox[3] = max( m_pBoundingBox[3], (*oPosIt).y );

			m_pBoundingBox[4] = min( m_pBoundingBox[4], (*oPosIt).z );
			m_pBoundingBox[5] = max( m_pBoundingBox[5], (*oPosIt).z );			
			++oPosIt;
		}

		/*
		oPosIt = m_vf3Position.begin();
		//Removes duplicates
		while( oPosIt != oEnd )
		{
			bool bAdd = true;
			std::vector<vec3>::iterator oInsertedPosIt = m_vf3SignificantVertex.begin();
			std::vector<vec3>::iterator oInsertedEnd = m_vf3SignificantVertex.end();
			while( oInsertedPosIt != oInsertedEnd )
			{
				if( (*oInsertedPosIt) == (*oPosIt) )
				{
					bAdd = false;
					break;
				}
				else
				{
					++oInsertedPosIt;
				}
					
			}
			if( bAdd )
			{
				m_vf3SignificantVertex.push_back( (*oPosIt) );
			}
			++oPosIt;
		}
		*/
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StaticMesh::ComputeTangents()
{
	m_vf3Tangent.resize( m_vf3Normal.size() );

	for(std::vector<sMeshGroup>::iterator itG=m_vGroup.begin(); itG!=m_vGroup.end(); itG++) 
	{
		for(std::vector<sMeshTriangle>::iterator itF=(*itG).m_vsTriangle.begin(); itF!=(*itG).m_vsTriangle.end(); itF++) 
		{
			GLuint* ind = ((GLuint*)(*itF).ind);
			vec3 f3Tangent;

			vec3 v0 = m_vf3Position[ind[0]];
			vec3 v1 = m_vf3Position[ind[1]];
			vec3 v2 = m_vf3Position[ind[2]];

			vec3 vect10 = v0-v1;
			vec3 vect12 = v2-v1;

			float deltaT10 = m_vf3Texcoord[ind[0]].y - m_vf3Texcoord[ind[1]].y;
			float deltaT12 = m_vf3Texcoord[ind[2]].y - m_vf3Texcoord[ind[1]].y;

			f3Tangent = (vect10 * deltaT12 ) - (vect12 * deltaT10 );
			f3Tangent = normalize( f3Tangent );

			m_vf3Tangent[ind[0]] = m_vf3Tangent[ind[1]] = m_vf3Tangent[ind[2]] = f3Tangent;

		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StaticMesh::BuildBuffer()
{
	glGenBuffers(1,  &m_iBufferId);
	assert(m_iBufferId);

	Bind();

	//Build the VBO
	if(!m_vf3Position.empty()) 
	{
		m_iSizeVertex = m_vf3Position.size()*sizeof(vec3);
	}
	else
	{
		std::cerr << "[ERROR] No position data !" << std::endl;
		return false;
	}

	// build normal
	if(!m_vf3Normal.empty())
	{
		m_iSizeNormal	= m_vf3Normal.size()*sizeof(vec3);
	}

	// build TextCoord
	if(!m_vf3Texcoord.empty())
	{
		m_iSizeTexture = m_vf3Texcoord.size()*sizeof(vec2);
	}
		
	// build tangent
	if(!m_vf3Tangent.empty())
	{
		m_iSizeTangent = m_vf3Tangent.size()*sizeof(vec3);
	}

	//Link the buffer
	glBufferData(GL_ARRAY_BUFFER, m_iSizeVertex + m_iSizeNormal + m_iSizeTexture + m_iSizeTangent, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER_ARB, 0,	m_iSizeVertex,	(const GLvoid*)(&m_vf3Position[0]));

	if(m_vf3Normal.size())
		glBufferSubData(GL_ARRAY_BUFFER_ARB, m_iSizeVertex,	m_iSizeNormal,	(const GLvoid*)(&m_vf3Normal[0]));

	if(m_vf3Texcoord.size())
		glBufferSubData(GL_ARRAY_BUFFER_ARB, m_iSizeVertex + m_iSizeNormal,	m_iSizeTexture,	(const GLvoid*)(&m_vf3Texcoord[0]));

	if(m_vf3Tangent.size())
		glBufferSubData(GL_ARRAY_BUFFER_ARB, m_iSizeVertex + m_iSizeNormal + m_iSizeTexture,	m_iSizeTangent,	(const GLvoid*)(&m_vf3Tangent[0]));

	Unbind();

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StaticMesh::Bind()
{
	//\todo better binder, better array (interleaved)
	glBindBuffer(GL_ARRAY_BUFFER, m_iBufferId);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StaticMesh::Unbind()
{
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StaticMesh::Render()
{
	Bind();

	//Enable GL option
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);

	if(!m_vf3Normal.empty()) 
	{
		glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex));
	}
	if (!m_vf3Texcoord.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer( 2,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal));
	}
	//Tangent
	if (!m_vf3Tangent.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE1);
		glTexCoordPointer( 3,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal+m_iSizeTexture));
	}
	
	for(std::vector<sMeshGroup>::iterator it=m_vGroup.begin(); it!=m_vGroup.end(); ++it)
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)(*it).m_vsTriangle.size()*3, GL_UNSIGNED_INT, &((*it).m_vsTriangle[0].ind[0]));
		//glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_INT, &((*it).m_vsTriangle[0].ind[0]));
	}
	
	//Disable GLOption
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 

	Unbind();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

void StaticMesh::Render( GLuint group )
{
	assert(group < (GLuint)m_vGroup.size());
	Bind();

	//Enable GL option
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);

	if(!m_vf3Normal.empty()) 
	{
		glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex));
	}
	if (!m_vf3Texcoord.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer( 2,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal));
	}
	//Tangent
	if (!m_vf3Tangent.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE1);
		glTexCoordPointer( 3,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal+m_iSizeTexture));
	}
	
	glDrawElements(GL_TRIANGLES, (GLsizei)m_vGroup[group].m_vsTriangle.size()*3, GL_UNSIGNED_INT, &(m_vGroup[group].m_vsTriangle[0].ind[0]));
	
	//Disable GLOption
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 

	Unbind();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StaticMesh::Destroy()
{
	glDeleteBuffersARB(1, &m_iBufferId);
	m_iBufferId = 0;

	m_vf3Position.clear();
	m_vf3Normal.clear();
	m_vf3Texcoord.clear();
	m_vf3Tangent.clear();

	for(std::vector<sMeshGroup>::iterator it=m_vGroup.begin(); it!=m_vGroup.end(); ++it)
	{
		(*it).m_vsTriangle.clear();
	}
	m_vGroup.clear();

	delete [] m_pBoundingBox;
	m_pBoundingBox = NULL;
}








