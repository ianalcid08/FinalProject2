#include "../Header Files/stdafx.h"
#include "../Header Files/Mesh.h"
#include <fstream>
using namespace std;

void CMesh::LoadMeshFromFile(char* FileName)
{
	char buffer[256];
	// Opens the Mesh binary file
	float extraBytes;
	
	std::fstream meshFile;
	meshFile.open(FileName, ios_base::in | ios_base::binary);
	meshFile.seekg(0, ios_base::beg);

	if(!meshFile.is_open())
	{
		// Sends error message if broken
		MessageBoxA(0, "Mesh File not found.", FileName, 0);
		return;
	}

	// reads all the names of the textures
	size_t textureAmount = 0;
	meshFile.read((char*)&textureAmount, sizeof(size_t));
	meshFile.read((char*)&extraBytes, sizeof(float));
	for(unsigned int i = 0; i < textureAmount; i++)
	{
		size_t size;
		meshFile.read((char*)&size, sizeof(size_t));
		meshFile.read((char*)&extraBytes, sizeof(float));
		memset(buffer, 0, 256);
		meshFile.read(buffer, size);
		m_TextureNames.push_back(buffer);
	}

	// reads all the verts of the mesh
	size_t vertAmount = 0;
	meshFile.read((char*)&vertAmount, sizeof(size_t));
	meshFile.read((char*)&extraBytes, sizeof(float));

	for(unsigned int i = 0; i < vertAmount; i++)
	{
		/*tUniqueVertex tmpVerts;
		meshFile.read((char*)&tmpVerts.m_Position, sizeof(float)*3);
		meshFile.read((char*)&tmpVerts.m_Normal, sizeof(float)*3);
		meshFile.read((char*)&tmpVerts.m_UVs, sizeof(float)*2);*/

		Vec3D Pos, Norm;
		Vec2D UV;

		meshFile.read((char*)&Pos.v[0], sizeof(float));
		meshFile.read((char*)&Pos.v[1], sizeof(float));
		meshFile.read((char*)&Pos.v[2], sizeof(float));

		meshFile.read((char*)&Norm.v[0], sizeof(float));
		meshFile.read((char*)&Norm.v[1], sizeof(float));
		meshFile.read((char*)&Norm.v[2], sizeof(float));

		meshFile.read((char*)&UV.v[0], sizeof(float));
		meshFile.read((char*)&UV.v[1], sizeof(float));

		//m_UniqueVerts.push_back(tmpVerts);

		m_Positions.push_back(Pos);
		m_Normals.push_back(Norm);
		m_UVs.push_back(UV);

		//// reads the weights of the mesh
		size_t weightAmount = 0;
		meshFile.read((char*)&weightAmount, sizeof(size_t));
		meshFile.read((char*)&extraBytes, sizeof(float));
		vector<tInfluence> tmpInfluences;
		for(size_t i = 0; i < weightAmount; i++)
		{
			tInfluence tmpInfluence;
			meshFile.read((char*)&tmpInfluence.m_JointIndex, sizeof(unsigned int));
			meshFile.read((char*)&tmpInfluence.m_Weight, sizeof(float));
			tmpInfluences.push_back(tmpInfluence);
		}
		if(tmpInfluences.size()>0)
			m_Influences.push_back(tmpInfluences);
	}

	//reads the triangles of the mesh
	size_t triAmount = 0;
	meshFile.read((char*)&triAmount, sizeof(size_t));
	meshFile.read((char*)&extraBytes, sizeof(float));
	for(size_t i = 0; i < triAmount; i++)
	{
		tTriangle tmpTri;
		meshFile.read((char*)&tmpTri.Indicies[0], sizeof(size_t));
		meshFile.read((char*)&extraBytes, sizeof(float));
		meshFile.read((char*)&tmpTri.Indicies[1], sizeof(size_t));
		meshFile.read((char*)&extraBytes, sizeof(float));
		meshFile.read((char*)&tmpTri.Indicies[2], sizeof(size_t));
		meshFile.read((char*)&extraBytes, sizeof(float));
		m_Triangles.push_back(tmpTri);
	}

	//reads the bone data for the mesh
	size_t boneAmount = 0;
	meshFile.read((char*)&boneAmount, sizeof(size_t));
	meshFile.read((char*)&extraBytes, sizeof(float));
	double data = 0.0;
	for(size_t i = 0; i < boneAmount; i++)
	{
		
		tJoint tmpJoint;
		meshFile.read((char*)&tmpJoint.m_Index, sizeof(unsigned int));
		meshFile.read((char*)&tmpJoint.m_ParentIndex, sizeof(int));
		meshFile.read(tmpJoint.m_Name, 256);	

		// Reads the local transform

		//x axis
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[0] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[1] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[2] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[3] = (float)data;

		//y axis
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[4] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[5] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[6] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[7] = (float)data;

		//z axis
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[8] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[9] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[10] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[11] = (float)data;

		//w axis
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[12] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[13] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[14] = (float)data;
		meshFile.read((char*)&data, sizeof(double));
		tmpJoint.m_LocalXForm[15] = (float)data;

		m_Joints.push_back(tmpJoint);
	}

	// reading the skeletal hierarcy using a pre-ordered tree traversal
	if(m_Joints.size() > 0)
		ReadSkeletonHierarcy(&m_RootNode, &meshFile);

	meshFile.close();

	m_SkinVerts = m_Positions;
}

void CMesh::ReadSkeletonHierarcy(tSkeletonNode* _curSkeletonNode, std::fstream* _meshFile)
{
	float extraBytes;
	//reads the current node
	_meshFile->read((char*)&_curSkeletonNode->m_JointIndex, sizeof(unsigned int));
	size_t childAmount = 0;
	_meshFile->read((char*)&childAmount, sizeof(size_t));
	_meshFile->read((char*)&extraBytes, sizeof(float));
	for(size_t i = 0; i < childAmount; i++)
	{
		// Recurses for all the children of that node
		tSkeletonNode tmpNode;
		ReadSkeletonHierarcy(&tmpNode, _meshFile);
		_curSkeletonNode->m_Children.push_back(tmpNode);
	}
}

void CMesh::ApplySkinning(std::vector<tKeyFrame> _CurFrame, std::vector<tKeyJoints> _BindPose)
{
	for(unsigned int i = 0; i < m_Influences.size(); i++)
	{
		m_SkinVerts[i].v[0] = 0;
		m_SkinVerts[i].v[1] = 0;
		m_SkinVerts[i].v[2] = 0;

		for(unsigned int x = 0; x < m_Influences[i].size(); x++)
		{
			unsigned int bone = m_Influences[i][x].m_JointIndex;
			Matrix4x4f mInv; //, mTrans;
			//glsTransposeMatrix4x4(mTrans, _BindPose[bone].m_KeyFrames[0].worldXForm);
			glsInvertMatrix4x4(mInv, _BindPose[bone].m_KeyFrames[0].worldXForm);

			Vec4f invertedVert;
			Vec4f notSkin = { m_Positions[i].v[0], m_Positions[i].v[1], m_Positions[i].v[2], 1 };
			glsTransformVector4(invertedVert, notSkin, mInv);
			Vec4f rigidSkin;
			//Matrix4x4f mTransFrame;
			//glsTransposeMatrix4x4(mTransFrame, _CurFrame[bone].worldXForm);
			glsTransformVector4(rigidSkin, invertedVert, _CurFrame[bone].worldXForm);

			glsScaleVector4(rigidSkin, m_Influences[i][x].m_Weight);
			m_SkinVerts[i].v[0] += rigidSkin[0];
			m_SkinVerts[i].v[1] += rigidSkin[1];
			m_SkinVerts[i].v[2] += rigidSkin[2];
		}
	}
}