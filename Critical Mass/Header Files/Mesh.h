#ifndef _MESH
#define _MESH

#include "glsMath.h"
#include <vector>
#include <fstream>

struct tKeyFrame
{
	float m_TimeInSeconds;
	Matrix4x4f localXForm;
	Matrix4x4f worldXForm;
};

struct tKeyJoints
{
	std::vector<tKeyFrame> m_KeyFrames;
};


struct tTriangle
{
	size_t Indicies[3];
};

struct tJoint
{
	char m_Name[256];
	unsigned int m_Index;
	int m_ParentIndex;
	Matrix4x4f m_LocalXForm;
	std::vector<tKeyFrame> m_KeyFrames;
};

struct tSkeletonNode
{
	size_t m_JointIndex;
	std::vector<tSkeletonNode> m_Children;
};

struct tInfluence
{
	size_t		m_JointIndex; 
	float		m_Weight; 
};

struct tUniqueVertex
{
	Vec3f	m_Position;
	Vec3f	m_Normal;
	Vec2f	m_UVs;
};

struct Vec3D
{
	Vec3f v;
};

struct Vec2D
{
	Vec2f v;
};

class CMesh
{
	//friend class Model;

	//std::vector<tUniqueVertex>				m_UniqueVerts;
	std::vector<Vec3D>						m_Positions;
	std::vector<Vec3D>						m_Normals;
	std::vector<Vec2D>						m_UVs;
	std::vector<unsigned int>				m_Indices;
	std::vector<Vec3D>						m_SkinVerts;

	std::vector<std::vector<tInfluence>>	m_Influences;
	std::vector<tTriangle>					m_Triangles;
	std::vector<char*>						m_TextureNames;
	std::vector<tJoint>						m_Joints;

	tSkeletonNode							m_RootNode;
	
	void ReadSkeletonHierarcy(tSkeletonNode* _curSkeletonNode, std::fstream* _meshFile);

public:

	//std::vector<tUniqueVertex>& GetVerts(void) {return m_UniqueVerts;}
	std::vector<Vec3D>& GetPositions(void) { return m_Positions; }
	std::vector<Vec3D>& GetSkinVerts(void) { return m_SkinVerts; }
	std::vector<Vec3D>& GetNormals(void) { return m_Normals; }
	std::vector<Vec2D>& GetUVs(void) { return m_UVs; }
	std::vector<unsigned int>& GetIndices(void) { return m_Indices; }

	std::vector<std::vector<tInfluence>>& GetInfluences(void) { return m_Influences; }
	std::vector<char*>& GetTextures(void) { return m_TextureNames; }
	std::vector<tTriangle>& GetTriangles(void) { return m_Triangles; }
	std::vector<tJoint>& GetJoints(void) {return m_Joints;}
	tSkeletonNode& GetSkeletonRoot(void) { return m_RootNode; }

	void LoadMeshFromFile(char* FileName);
	void ApplySkinning(std::vector<tKeyFrame> _CurFrame, std::vector<tKeyJoints> _BindPose);
};

//class CModel
//{
//	std::vector<CMesh> m_Meshes;
//public:
//	std::vector<CMesh> &GetMeshes(void) { return m_Meshes; }
//};

#endif _MESH