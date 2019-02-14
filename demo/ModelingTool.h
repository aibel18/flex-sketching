#include "ToolModel.h"

static const Point3 vertices[24] =
{
	/// front
	Point3(1.0f, 1.0f, 1.0f),
	Point3(-1.0f, 1.0f, 1.0f),
	Point3(1.0f, -1.0f, 1.0f),
	Point3(-1.0f, -1.0f, 1.0f),

	/// right
	Point3(1.0f, 1.0f, -1.0f),
	Point3(1.0f, 1.0f, 1.0f),
	Point3(1.0f, -1.0f, -1.0f),
	Point3(1.0f, -1.0f, 1.0f),

	/// back
	Point3(-1.0f, 1.0f, -1.0f),
	Point3(1.0f, 1.0f, -1.0f),
	Point3(-1.0f, -1.0f, -1.0f),
	Point3(1.0f, -1.0f, -1.0f),

	/// left
	Point3(-1.0f, 1.0f, 1.0f),
	Point3(-1.0f, 1.0f, -1.0f),
	Point3(-1.0f, -1.0f, 1.0f),
	Point3(-1.0f, -1.0f, -1.0f),

	/// up
	Point3(1.0f, 1.0f, -1.0f),
	Point3(-1.0f, 1.0f, -1.0f),
	Point3(1.0f, 1.0f, 1.0f),
	Point3(-1.0f, 1.0f, 1.0f),

	/// down
	Point3(1.0f, -1.0f, 1.0f),
	Point3(-1.0f, -1.0f, 1.0f),
	Point3(1.0f, -1.0f, -1.0f),
	Point3(-1.0f, -1.0f, -1.0f)
};

static const Vec3 normals[24] =
{
	/// front
	Vec3(0.0f, 0.0f, 1.0f),
	Vec3(0.0f, 0.0f, 1.0f),
	Vec3(0.0f, 0.0f, 1.0f),
	Vec3(0.0f, 0.0f, 1.0f),

	/// right
	Vec3(1.0f, 0.0f, 0.0f),
	Vec3(1.0f, 0.0f, 0.0f),
	Vec3(1.0f, 0.0f, 0.0f),
	Vec3(1.0f, 0.0f, 0.0f),

	/// back
	Vec3(0.0f, 0.0f, -1.0f),
	Vec3(0.0f, 0.0f, -1.0f),
	Vec3(0.0f, 0.0f, -1.0f),
	Vec3(0.0f, 0.0f, -1.0f),

	/// left
	Vec3(-1.0f, 0.0f, 0.0f),
	Vec3(-1.0f, 0.0f, 0.0f),
	Vec3(-1.0f, 0.0f, 0.0f),
	Vec3(-1.0f, 0.0f, 0.0f),

	/// up
	Vec3(0.0f, 1.0f, 0.0f),
	Vec3(0.0f, 1.0f, 0.0f),
	Vec3(0.0f, 1.0f, 0.0f),
	Vec3(0.0f, 1.0f, 0.0f),

	/// down
	Vec3(0.0f, -1.0f, 0.0f),
	Vec3(0.0f, -1.0f, 0.0f),
	Vec3(0.0f, -1.0f, 0.0f),
	Vec3(0.0f, -1.0f, 0.0f)
};

static const int indices[36] =
{
	/// front
	0, 1, 2,
	3, 2, 1,

	/// right
	4, 5, 6,
	7, 6, 5,

	/// back
	8, 9, 10,
	11, 10, 9,

	/// left
	12, 13, 14,
	15, 14, 13,

	/// up
	16, 17, 18,
	19, 18, 17,

	/// down
	20, 21, 22,
	23, 22, 21
};

void addMeshFaceforPoint(Mesh* m, int pIndex, int iIndex, int size) {
	
	for (int i = pIndex, k = 0; k < 4;i++, k++  ) {		
		m->m_positions.push_back( vertices[i] );
		m->m_normals.push_back( normals[i]);
	}
	for (int i = iIndex, k = 0; k < 6; i++, k++) {
		m->m_indices.push_back(size + indices[i]);
	}

}

void addMeshforPoint(Mesh* m, vec3 *position, int neight, float* center,float radius ){

	int sizeLast = m->m_positions.size();
	int offset = 0;
	if (neight & 1) 
		addMeshFaceforPoint(m, 0, 0, sizeLast);
	else
		offset += 4;

	if (neight & 2)
		addMeshFaceforPoint(m, 4, 6, sizeLast - offset);
	else
		offset += 4;
	if (neight & 4)
		addMeshFaceforPoint(m, 8, 12, sizeLast - offset);
	else
		offset += 4;
	if (neight & 8)
		addMeshFaceforPoint(m, 12, 18, sizeLast - offset);
	else
		offset += 4;
	if (neight & 16)
		addMeshFaceforPoint(m, 16, 24, sizeLast - offset);
	else
		offset += 4;
	if (neight & 32)
		addMeshFaceforPoint(m, 20, 30, sizeLast - offset);
	else
		offset += 4;
	/// update real position for new mesh
	int size = m->m_positions.size();
	Point3 p(position->x, position->y, position->z);
	Point3 c(center[0], center[1], center[2]);
	for (int i=sizeLast;i<size;i++) {
		m->m_positions[i] = (m->m_positions[i] * radius + p);

		m->m_positions[i][0] -= center[0];
		m->m_positions[i][1] -= center[1];
		m->m_positions[i][2] -= center[2];
	}
}

///////////////////////////////
/// Martchcub

static vec3 cornerOffsets[] = {
		vec3(0.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(1.0f, 0.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(0.0f, 1.0f, 1.0f)
};

static vec3 edgeVertexOffsets[][2] = {
		{ vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) },
		{ vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) },
		{ vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) },
		{ vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) },
		{ vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f) },
		{ vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
		{ vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
		{ vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f) },
		{ vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f) },
		{ vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f) },
		{ vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f) },
		{ vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f) }
};

static int triangleTable[][16] = 
{
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
		{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
		{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
		{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
		{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
		{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
		{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
		{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
		{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
		{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
		{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
		{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
		{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
		{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
		{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
		{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
		{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
		{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
		{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
		{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
		{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
		{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
		{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
		{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
		{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
		{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
		{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
		{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
		{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
		{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
		{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
		{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
		{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
		{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		/// 64
		{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
		{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
		{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
		{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
		{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
		{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
		{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
		{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
		{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
		{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
		{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
		{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
		{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
		{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
		{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
		{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
		{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
		{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
		{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
		{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
		{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
		{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
		{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
		{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
		{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
		{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
		{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
		{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
		{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
		{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
		{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
		{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
		{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
		{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
		{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
		{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
		{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
		{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
		{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
		{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
		{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
		{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
		{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
		{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
		{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
		{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
		{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
		{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
		{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
		{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
		{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
		{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
		{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
		{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
		{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
		{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
		{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
		{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
		{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
		{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
		{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
		{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
		{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
		{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
		{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
		{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
		{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
		{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
		{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
		{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
		{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
		{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
		{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
		{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
		{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
		{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
		{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
		{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
		{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
		{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
		{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
		{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
		{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
		{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
		{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
		{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
		{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
		{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
		{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
		{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
		{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
		{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
		{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
		{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
		{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
		{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
		{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
		{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
		{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
		{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
		{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
		{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
		{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
		{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
		{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
		{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
		{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
		{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
		{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
		{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
		{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
		{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
		{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
		{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
		{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
		{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
		{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
		{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
		{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
		{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
		{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
		{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
		{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
		{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
		{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
		{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
		{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
		{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
		{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
		{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
		{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
		{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
		{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
		{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
		{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
		{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};


void marchCube(ContainerModel* container, Mesh* m, vec3 minCornerPos, float* center)
{
	// construct case index from 8 corner samples
	int caseIndex = 0;
	for (int i = 0, j=7; i < 8; i++,j--)
	{
		int xx = int(minCornerPos.x + cornerOffsets[i].x);
		int yy = int(minCornerPos.y + cornerOffsets[i].y);
		int zz = int(minCornerPos.z + cornerOffsets[i].z);
		//std::cout <<" > " << xx << " " << yy << " " << zz << std::endl;

		if ( container->verifyCorner(xx, yy, zz)) {
			caseIndex |= 1 << i;
			//std::cout<< " <<<< " << i << std::endl;
		}
	}

	// early out if entirely inside or outside the volume
	if (caseIndex == 0 || caseIndex == 0xFF)
		return;

	float radius = container->radius;
	int caseVert = 0;
	for (int i = 0; i < 5; i++)
	{
		//for (int tri = 0; tri < 3; tri++)
		for (int tri = 2; tri >= 0; tri--)
		{
			int edgeCase = triangleTable[caseIndex][ 3*i + tri];
			if (edgeCase == -1)
				return;
			vec3 vert1 = minCornerPos + edgeVertexOffsets[edgeCase][ 0]; // beginning of the edge
			vec3 vert2 = minCornerPos + edgeVertexOffsets[edgeCase][ 1]; // end of the edge

			//vec3 vertPos = (vert1 + vert2) / 2.0f;			


			float s1 = 0.0f;
			float s2 = 0.0f;

			if( container->verifyCorner(vert1.x, vert1.y, vert1.z) )
				s1 = 1;
			if (container->verifyCorner(vert2.x, vert2.y, vert2.z) )
				s2 = 1;

			// interpolate along the edge
			//float s1 = SampleValue(minCornerPos + edgeVertexOffsets[edgeCase, 0]);
			//float s2 = SampleValue(minCornerPos + edgeVertexOffsets[edgeCase, 1]);
			float dif = s1 - s2;
			if (dif == 0.0f)
				dif = 0.5f;
			else
				dif = s1 / dif;
			// Lerp /// Interpolation
			vec3 vertPos = vert1 + ((vert2 - vert1) * dif);
			




			/// scale
			Point3 p(vertPos.x - container->centerMatrix[0], vertPos.y - container->centerMatrix[1], vertPos.z - container->centerMatrix[2]);

			p = p * radius;
			p[0] -= center[0];
			p[1] -= center[1] - radius * 0.5f;
			p[2] -= center[2];

			m->m_positions.push_back(p );
			m->m_indices.push_back( m->m_positions.size() - 1 );
			

			caseVert++;
		}
	}
}


Mesh* createMyMeshNew(ContainerModel* container, float cX, float cY, float cZ) {
	Mesh* m = new Mesh();
	float center[] = { cX,cY,cZ };

	std::vector<vec3> vertexRender;
	for (int i = 0; i < container->activeParticles; i++) {
		if (container->phases[i] != -1 && container->indexAdjacent[i] > 0) {

			int posDis[3];
			container->discretization(container->particles[i], posDis);
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						int xx = posDis[0] + j;
						int yy = posDis[1] + k;
						int zz = posDis[2] + l;

						if (container->verifyCellRender(xx, yy, zz)) {
							xx = xx - 1;
							yy = yy - 1;
							zz = zz - 1;
							vertexRender.push_back(vec3(float(xx), float(yy), float(zz)));
						}
					}
				}
			}
		}
	}

	int size = vertexRender.size();
	//std::cout << "SIZE PARTICLES " << size << std::endl;
	for (int i = 0; i < size; i++) {
		//std::cout << vertexRender[i].x << " " << vertexRender[i].y << " " << vertexRender[i].z << std::endl;
		::marchCube(container, m, vertexRender[i], center);
	}
	m->CalculateNormalsInv();

	/// reinice grid for render
	for (int i = 0; i < size; i++) {
		container->reiniceCellRender(int(vertexRender[i].x + 1), int(vertexRender[i].y + 1 ), int(vertexRender[i].z + 1));
	}

	return m;
}
GpuMesh* createMyMeshNew2(ContainerModel* container, float cX, float cY, float cZ) {
	Mesh* m = new Mesh();
	float center[] = { cX,cY,cZ };

	std::vector<vec3> vertexRender;
	for (int i = 0; i < container->activeParticles; i++) {
		if (container->phases[i] != -1 && container->indexAdjacent[i] > 0) {

			int posDis[3];
			container->discretization(container->particles[i], posDis);
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						int xx = posDis[0] + j;
						int yy = posDis[1] + k;
						int zz = posDis[2] + l;

						if (container->verifyCellRender(xx, yy, zz)) {
							xx = xx - 1;
							yy = yy - 1;
							zz = zz - 1;
							vertexRender.push_back(vec3(float(xx), float(yy), float(zz)));
						}
					}
				}
			}
		}
	}

	int size = vertexRender.size();
	//std::cout << "SIZE PARTICLES " << size << std::endl;
	for (int i = 0; i < size; i++) {
		//std::cout << vertexRender[i].x << " " << vertexRender[i].y << " " << vertexRender[i].z << std::endl;
		::marchCube(container, m, vertexRender[i], center);
	}
	
	m->CalculateNormalsInv();

	GpuMesh* newMesh = CreateGpuMesh(m);
	delete m;

	/// reinice grid for render
	for (int i = 0; i < size; i++) {
		container->reiniceCellRender(int(vertexRender[i].x + 1), int(vertexRender[i].y + 1), int(vertexRender[i].z + 1));
	}

	return newMesh;
}

//////////////////////////////////
Mesh* createMyMesh(ContainerModel* container, float cX, float cY, float cZ) {
	
	Mesh* m = new Mesh();
	float center[] = {cX,cY,cZ};
	float radius = container->radius*0.5f;
	for (int i = 0; i < container->activeParticles; i++) {
		if (container->phases[i] != -1 && container->indexAdjacent[i] > 0) {
			::addMeshforPoint(m, container->particles + i , container->indexAdjacent[i], center, radius);
			//std::cout<< i << " >> " << container->indexAdjacent[i] << std::endl;
		}
	}
	return m;
}
GpuMesh* createMyMesh2(ContainerModel* container, float cX, float cY, float cZ) {

	Mesh* m = new Mesh();
	float center[] = { cX,cY,cZ };
	float radius = container->radius*0.5f;
	for (int i = 0; i < container->activeParticles; i++) {
		if (container->phases[i] != -1 && container->indexAdjacent[i] > 0) {
			::addMeshforPoint(m, container->particles + i, container->indexAdjacent[i], center, radius);
			//std::cout<< i << " >> " << container->indexAdjacent[i] << std::endl;
		}
	}

	//std::cout<< " number points" << m->m_positions.size() << std::endl;
	//std::cout << " number indices" << m->m_indices.size() << std::endl;

	GpuMesh* newMesh = CreateGpuMesh(m);
	delete m;

	return newMesh;
}


struct Material {
	/// fluid
	float viscosity;
	float cohesion;
	float vorticity;

	/// solid
	float stiffnes;
	float friction;
	float dissipation;

	/// soft
	float relaxationFactor;
};

class ModelingTool{
/////////////////////////////////////////
//*
	struct Instance
	{
		Instance() :

			mColor(0.5f, 0.5f, 1.0f),

			mScale(2.0f),
			mTranslation(0.0f, 1.0f, 0.0f),

			mClusterSpacing(1.0f),
			mClusterRadius(0.0f),
			mClusterStiffness(0.5f),

			mLinkRadius(0.0f),
			mLinkStiffness(1.0f),

			mGlobalStiffness(0.0f),

			mSurfaceSampling(0.0f),
			mVolumeSampling(4.0f),

			mSkinningFalloff(2.0f),
			mSkinningMaxDistance(100.0f),

			mClusterPlasticThreshold(0.0f),
			mClusterPlasticCreep(0.0f)
		{}

		Vec3 mColor;

		Vec3 mScale;
		Vec3 mTranslation;

		float mClusterSpacing;
		float mClusterRadius;
		float mClusterStiffness;

		float mLinkRadius;
		float mLinkStiffness;

		float mGlobalStiffness;

		float mSurfaceSampling;
		float mVolumeSampling;

		float mSkinningFalloff;
		float mSkinningMaxDistance;

		float mClusterPlasticThreshold;
		float mClusterPlasticCreep;
	};
	
//private:

	struct RenderingInstance
	{
		GpuMesh* mGpuMesh;
		Mesh* mMesh;
		std::vector<int> mSkinningIndices;
		std::vector<float> mSkinningWeights;
		vector<Vec3> mRigidRestPoses;
		Vec3 mColor;
		int mOffset;
		int particlesOffset;
		int particles;

		int type;
	};

	std::vector<RenderingInstance> objects;

///////////////////////////////////////
public:
	ToolModel toolRight;
	ToolModel toolLeft;

	GpuMesh* meshToolRight;
	GpuMesh* meshToolLeft;

	int numberParticlesLeft = 0;
	int numberParticlesRight = 0;

	ContainerModel container;

	/// function that create type brushes
	void(*brushes[FormBrushMax]) (float, int, vector<Vec3>*);
	/// function that create type brushes
	MyMesh meshBrushLeft;
	MyMesh meshBrushRight;

	/// function that generate the form of the brush
	void(*tranformation[FormBrushMax])(MyMesh* mesh, float radius, int with);

	/// rendering future particles 
	unsigned int idVBO;
	unsigned int indicesVBO;

	int idObjects = 0;
	int numberHand = 0;
	bool toSync = false;
	
	/// array that content mesh of the brushes

	ModelingTool(): toolLeft(), toolRight() {
		//std::cout<< "constructor tool \n";
		this->tranformation[0] = &ToolPiece::meshBrushSphere;
		this->tranformation[1] = &ToolPiece::meshBrushCube;		
		this->tranformation[2] = &ToolPiece::meshBrushCircleV;
		//this->tranformation[2] = &ToolPiece::meshBrushPlaneH;
		this->tranformation[3] = &ToolPiece::meshBrushPlaneV;
		//this->tranformation[4] = &ToolPiece::meshBrushCircleH;
		
		this->tranformation[4] = &ToolPiece::meshBrushLineH;
		this->tranformation[5] = &ToolPiece::meshBrushLineV;
	}
	~ModelingTool() {
		//std::cout << "destructor tool \n";
		/// Interaction
		this->destroy();
	}

	void start( bool isLeftHand, bool isRightHand, bool isParticleHandLeft, bool isParticleHandRight) {

		this->destroy();
		this->idObjects = 0;
		this->toSync = false;

		//int phaseSolid = NvFlexMakePhase(this->idObjects, eNvFlexPhaseSelfCollide );
		int phaseSolid = NvFlexMakePhase(this->idObjects, 0);

		if( isLeftHand ){
			if( this->toolLeft.bindHand( leftHand, vrcontrol, &container) ){

				/// create particles for tool Left
				if (this->toolLeft.numberParticles) {
					this->toolLeft.offsetParticles = g_buffers->positions.size();
					for (int i = 0; i < this->toolLeft.numberParticles; i++) {
						g_buffers->positions.push_back(Vec4(0, 0, 0, 0));
						g_buffers->velocities.push_back(Vec3(0, 0, 0));
						g_buffers->phases.push_back(phaseSolid);
					}
					//this->idObjects++;
					//phaseSolid = NvFlexMakePhase(this->idObjects, 0);
				}

				vrcontrol->bindHand(leftHand, g_buffers->positions.size(),isParticleHandLeft);

				int size = vrcontrol->sizeParticlesHand(leftHand);				
				for (int i = 0; i < size; i++) {
					g_buffers->positions.push_back(Vec4(0, 0, 0, 0));
					g_buffers->velocities.push_back(Vec3(0, 0, 0));
					g_buffers->phases.push_back(phaseSolid);
				}
				//if (size > 0) {
					this->idObjects++;
					phaseSolid = NvFlexMakePhase(this->idObjects, 0);
				//}
				std::cout << "Hand: Number of particles Hand LEFT: " << size << std::endl;
			}
		}

		this->numberParticlesLeft = g_buffers->positions.size();
		
		if( isRightHand ){
			if(this->toolRight.bindHand( rightHand, vrcontrol, &container)){

				/// create particles for tool Right
				if (this->toolRight.numberParticles) {
					this->toolRight.offsetParticles = g_buffers->positions.size();
					for (int i = 0; i < this->toolRight.numberParticles; i++) {
						g_buffers->positions.push_back(Vec4(0, 0, 0, 0));
						g_buffers->velocities.push_back(Vec3(0, 0, 0));
						g_buffers->phases.push_back(phaseSolid);
					}
					//this->idObjects++;
					//phaseSolid = NvFlexMakePhase(this->idObjects, 0);
				}

				vrcontrol->bindHand(rightHand, g_buffers->positions.size(), isParticleHandRight);

				int size = vrcontrol->sizeParticlesHand(rightHand);
				
				for (int i = 0; i < size; i++) {
					g_buffers->positions.push_back(Vec4(0, 0, 0, 0));
					g_buffers->velocities.push_back(Vec3(0, 0, 0));
					g_buffers->phases.push_back(phaseSolid);
				}
				//if (size > 0) {
					this->idObjects++;
					//phaseSolid = NvFlexMakePhase(this->idObjects, 0);
				//}
				std::cout << "Hand: Number of particles Hand RIGHT: " << size << std::endl;
			}
		}
		this->numberHand = this->idObjects;

		this->numberParticlesRight = g_buffers->positions.size();
		g_numSolidParticles = g_buffers->positions.size();
	}

	void init(float radius, float center[3]) {

		this->container.init(radius, center);
		initDrawPoints(&this->container.idVBO, &this->container.indicesVBO, SIZE_PARTICLES);

		if (this->toolLeft.container) {
			this->toolLeft.init();
			this->loadMeshTool(1,radius);

			(*this->tranformation[this->toolLeft.emitter.formBrush])(&this->meshBrushLeft, radius, this->toolLeft.emitter.width);
			std::cout << "VRControl : INIT TOOL LEFT" << std::endl;
		}
		if (this->toolRight.container) {
			this->toolRight.init();
			this->loadMeshTool(2,radius);

			(*this->tranformation[this->toolRight.emitter.formBrush])(&this->meshBrushRight, radius, this->toolRight.emitter.width);
			std::cout << "VRControl : INIT TOOL RIGHT" << std::endl;
		}
	}
	void loadMeshTool(int type, float radius) {
		Mesh* meshControl = ImportMesh(GetFilePathByPlatform("../../data/controllerx.obj").c_str());
		//meshControl->Transform(ScaleMatrix(0.01));
		meshControl->Transform(ScaleMatrix(0.001));
		meshControl->Transform(RotationMatrix(DegToRad(180), Vec3(0, 1, 0)) * RotationMatrix(DegToRad(90), Vec3(1, 0, 0)));

		if( type ==1 )
			this->meshToolLeft = CreateGpuMesh(meshControl);
		else
			this->meshToolRight = CreateGpuMesh(meshControl);


		//CreateParticleShape(meshControl, radius*0.75f, Vec3(0.0f), 1.0f, true, 0.8f, NvFlexMakePhase(this->idObjects++, 0), false, 0.0f);

		delete meshControl;
	}
	///
	void destroy() {
		if(this->container.idVBO > 0){
			destroyDrawPoints(&this->container.idVBO, &this->container.indicesVBO );
			this->container.idVBO = 0;
			this->container.indicesVBO = 0;
		}
		for(int i=0;i< this->objects.size();i++){
			if(this->objects[i].type == 0){
				DestroyGpuMesh(this->objects[i].mGpuMesh);
				//delete this->objects[i].mGpuMesh;
			}
			else
				delete this->objects[i].mMesh;
		}
		this->objects.clear();
	}

	void update() {
		
		if ( this->toolLeft.update() ) {
			/// update position of the tool particles
			if (this->toolLeft.numberParticles) {
				for (int i = 0, k = this->toolLeft.offsetParticles; i < this->toolLeft.numberParticles; i++, k++) {

					vec3 v;
					multiple4x3(v, this->toolLeft.matrixTool, this->toolLeft.particles[i]);

					g_buffers->positions[k][0] = v.x;
					g_buffers->positions[k][1] = v.y;
					g_buffers->positions[k][2] = v.z;

					//g_buffers->velocities[k] = Vec3(0.0f);
				}
			}
			/// update form brush because event joysstick
			if( this->toolLeft.joystickActive )
				(*this->tranformation[this->toolLeft.emitter.formBrush])(&this->meshBrushLeft, this->container.radius, this->toolLeft.emitter.width);			
		}
		if ( this->toolRight.update() ) {
			/// update position of the tool particles
			if (this->toolRight.numberParticles) {
				for (int i = 0, k = this->toolRight.offsetParticles; i < this->toolRight.numberParticles; i++, k++) {

					vec3 v;
					multiple4x3(v, this->toolRight.matrixTool, this->toolRight.particles[i]);

					g_buffers->positions[k][0] = v.x;
					g_buffers->positions[k][1] = v.y;
					g_buffers->positions[k][2] = v.z;

					//g_buffers->velocities[k] = Vec3(0.0f);
				}
			}
			/// update form brush because event joysstick
			if (this->toolRight.joystickActive)
				(*this->tranformation[this->toolRight.emitter.formBrush])(&this->meshBrushRight, this->container.radius, this->toolRight.emitter.width);			
		}

		if( !this->container.newObject ){
			/// if emitter particles with the left hand
			if (this->toolLeft.isEmitter) {
				int phase = getMaterial(idObjects, this->toolLeft.emitter.typeMaterial);
				this->toolLeft.managerParticles(phase);
			}

			/// if emitter particles with the right hand
			if (this->toolRight.isEmitter) {
				int phase = getMaterial(idObjects, this->toolRight.emitter.typeMaterial);
				this->toolRight.managerParticles(phase);				
			}

			/// update the new particles
			if(this->toolLeft.isEmitter || this->toolRight.isEmitter){

				this->container.rebuild();
				updatePoints(this->container.idVBO, this->container.indicesVBO, (float*)&this->container.particlesResult[0], &this->container.indicesResult[0], this->container.numberNewparticles);

				std::cout << "EMITIENDO\n";
			}


			this->collistionTool();
		}
		/// if create new Object
		else {
			if (this->createObjetc()) {
				this->container.reset();
				this->idObjects++;
				this->toSync = true;
			}
		}
	}
	
	void draw(float radius, float aspect, float fov, Matrix44 lightTransform) {

		for ( int i=0; i< this->objects.size(); i++ ) {
			if (!g_drawMesh)
				break;
			if(this->objects[i].type == 0){
				int indexSolid = this->objects[i].mOffset;
				Matrix44 xform = TranslationMatrix(Point3(g_buffers->rigidTranslations[indexSolid]))*RotationMatrix(Quat(g_buffers->rigidRotations[indexSolid]));
				
				DrawGpuMesh(this->objects[i].mGpuMesh, xform, this->objects[i].mColor);
				//DrawGpuMesh(this->objects[i].mGpuMesh, Matrix44::kIdentity, this->objects[i].mColor);
				
			}
			else if (this->objects[i].type == 1) {
				this->drawSoft(this->objects[i]);
			}
			
		}
		

		if ( this->toolLeft.toolEmitter ) {
			/// draw mesh tool
			DrawGpuMesh(this->meshToolLeft, Matrix44(this->toolLeft.matrixTool), Vec3(myColors[5]));
			/// draw mesh brush
			DrawGpuMesh2(this->meshBrushLeft.mMesh, Matrix44(this->toolLeft.matrixBrush), Vec4(myColors[this->toolLeft.emitter.typeMaterial]));
		}
		if (this->toolRight.toolEmitter) {
			/// draw mesh tool
			DrawGpuMesh(this->meshToolRight, Matrix44(this->toolRight.matrixTool), Vec3(myColors[5]));
			//DrawGpuMesh(this->meshToolRight.mMesh, Matrix44::kIdentity, Vec3(myColors[5]));

			/// draw mesh brush
			DrawGpuMesh2(this->meshBrushRight.mMesh, Matrix44(this->toolRight.matrixBrush), Vec4(myColors[this->toolRight.emitter.typeMaterial]));
		}
		/// draw future particles
		if (this->container.activeParticles > 0 ) {
			drawPoints(this->container.idVBO, this->container.indicesVBO, this->container.numberNewparticles, 0, radius, float(g_screenWidth), aspect, fov, g_lightPos, g_lightTarget, lightTransform, g_shadowMap);
		}
	}
	
	void eventController(vr::VREvent_t event) {		

		this->toolLeft.eventController( event );
		this->toolRight.eventController( event );
	}

	bool createObjetc() {

		int particlesActives = NvFlexGetActiveCount(g_solver);

		if (particlesActives + this->container.numberNewparticles > g_buffers->positions.size()){
			std::cout << "OVERLOAD THE PARTICLES: " << g_buffers->positions.size() << " " << particlesActives + this->container.numberNewparticles << std::endl;
			return false;
		}

		for (int i = 0, k = particlesActives; i < this->container.numberNewparticles; i++) {
			g_buffers->positions[k] = Vec4(this->container.particlesResult[i].x, this->container.particlesResult[i].y, this->container.particlesResult[i].z,1.0f);
			g_buffers->velocities[k] = Vec3(0.0f);
			g_buffers->phases[k] = this->container.phaseResult[i];
			k++;
		}

		switch (this->container.typeMaterialTemp) {
			case 0: {

				// compact instances
				/// star index
				if (g_buffers->rigidIndices.empty()) {
					g_buffers->rigidOffsets.push_back(0);
					std::cout << "INIT SOLID " << std::endl;
				}

				/// calculate mass
				Vec4 center = calculateCenterMass(&g_buffers->positions[0], particlesActives, particlesActives + this->container.numberNewparticles);


				RenderingInstance renderingInstance;
				renderingInstance.type = 0;
				
				renderingInstance.mColor = Vec3(g_colors[getMaterial(idObjects, this->container.typeMaterialTemp) % 8]);
				renderingInstance.mOffset = g_buffers->rigidTranslations.size();

				//createMyMeshNew2(&this->container, center[0], center[1], center[2]);
				renderingInstance.mGpuMesh = createMyMeshNew2(&this->container, center[0], center[1], center[2]);
				
				renderingInstance.particlesOffset = particlesActives;
				renderingInstance.particles = renderingInstance.particlesOffset + this->container.numberNewparticles;				

				g_buffers->rigidCoefficients.push_back(1.0f);
				g_buffers->rigidTranslations.push_back(Vec3(center[0], center[1], center[2]));
				g_buffers->rigidRotations.push_back(Quat());

				for (int i = 0, k = particlesActives; i < this->container.numberNewparticles; i++, k++) {
					g_buffers->rigidLocalPositions.push_back(Vec3(g_buffers->positions[k] - center));
					g_buffers->rigidIndices.push_back(k);
				}
				g_buffers->rigidOffsets.push_back(int(g_buffers->rigidIndices.size()));

				/// create mesh for new object
				this->objects.push_back(renderingInstance);

				std::cout << "CREATE SOLID\n";
				break;
			}
			case 1: {

				if (g_buffers->rigidIndices.empty()) {
					g_buffers->rigidOffsets.push_back(0);
				}

				// soft body scenes
				Instance instance;
				instance.mClusterSpacing = 1.5f;
				instance.mClusterRadius = 0.0f;
				instance.mClusterStiffness = 0.5f;
				
				this->CreateSoftBody(instance, particlesActives);
				std::cout << "CREATE SOFT\n";
				break;
			}
			case 2: {
				std::cout << "CREATE LIQUID\n";
				break;
			}
		}

		for (int i = 0, k = particlesActives; i < this->container.numberNewparticles; i++, k++) {
			g_buffers->activeIndices.push_back(k);
		}
		
		return true;
	}

	void CreateSoftBody(Instance instance, int particlesActives){
		RenderingInstance renderingInstance;
		renderingInstance.type = 1;
		renderingInstance.particlesOffset = particlesActives;
		renderingInstance.particles = renderingInstance.particlesOffset + this->container.numberNewparticles;
		
		renderingInstance.mOffset = g_buffers->rigidTranslations.size();

		double createStart = GetSeconds();

		// create soft body definition
		NvFlexExtAsset* asset = NvFlexExtCreateSoftFromVoxel(
			(float*)&this->container.particlesResult[ 0 ],
			this->container.numberNewparticles,

			instance.mClusterSpacing*this->container.radius,
			instance.mClusterRadius*this->container.radius,
			instance.mClusterStiffness,

			instance.mLinkRadius*this->container.radius,
			instance.mLinkStiffness,
			instance.mGlobalStiffness,

			instance.mClusterPlasticThreshold,
			instance.mClusterPlasticCreep);

		double createEnd = GetSeconds();
		

		int translationOffset = g_buffers->rigidTranslations.size();
		
		//const int particleOffset = g_buffers->positions.size();
		int particleOffset = NvFlexGetActiveCount(g_solver);
		const int indexOffset = g_buffers->rigidOffsets.back();
		
		// add shape data to solver
		for (int i = 0; i < asset->numShapeIndices; ++i){
			g_buffers->rigidIndices.push_back(asset->shapeIndices[i] + particleOffset);
		}
		for (int i = 0; i < asset->numShapes; ++i){
			g_buffers->rigidOffsets.push_back(asset->shapeOffsets[i] + indexOffset);
			Vec3 vTemp(&asset->shapeCenters[i * 3]);
			g_buffers->rigidTranslations.push_back(vTemp);
			g_buffers->rigidRotations.push_back(Quat());
			g_buffers->rigidCoefficients.push_back(asset->shapeCoefficients[i]);
		}

		for (int i = 0,k = translationOffset; i < asset->numShapes; ++i,++k){

			const int startIndex = g_buffers->rigidOffsets[k];
			const int endIndex = g_buffers->rigidOffsets[k + 1];

			for (int j = startIndex; j < endIndex; ++j) {
				const int r = g_buffers->rigidIndices[j];
				g_buffers->rigidLocalPositions.push_back(Vec3(g_buffers->positions[r]) - g_buffers->rigidTranslations[k]);
			}			
		}

		Vec3 center(0.0f);

		/// create mesh for soft
		renderingInstance.mMesh = createMyMeshNew(&this->container, center[0], center[1], center[2]);
		renderingInstance.mColor = Vec3(g_colors[getMaterial(idObjects, this->container.typeMaterialTemp) % 8]);
		

		// create skinning
		const int maxWeights = 4;

		renderingInstance.mSkinningIndices.resize(renderingInstance.mMesh->m_positions.size()*maxWeights);
		renderingInstance.mSkinningWeights.resize(renderingInstance.mMesh->m_positions.size()*maxWeights);

		
		for (int i = 0; i < asset->numShapes; ++i)
			renderingInstance.mRigidRestPoses.push_back(Vec3(&asset->shapeCenters[i * 3]));

		double skinStart = GetSeconds();
		
		NvFlexExtCreateSoftMeshSkinning(
			(float*)&renderingInstance.mMesh->m_positions[0],
			renderingInstance.mMesh->m_positions.size(),
			asset->shapeCenters,
			asset->numShapes,
			instance.mSkinningFalloff,
			instance.mSkinningMaxDistance,
			&renderingInstance.mSkinningWeights[0],
			&renderingInstance.mSkinningIndices[0]);

		double skinEnd = GetSeconds();

		printf("Created soft in %f ms Skinned in %f ms\n", (createEnd - createStart)*1000.0f, (skinEnd - skinStart)*1000.0f);
		

		//printf("%d %d, %d\n", asset->numParticles, asset->numShapes, asset->numShapeIndices);

		/*
		// add plastic deformation data to solver, if at least one asset has non-zero plastic deformation coefficients, leave the according pointers at NULL otherwise
		if (plasticDeformation)
		{
			if (asset->shapePlasticThresholds && asset->shapePlasticCreeps)
			{
				for (int i = 0; i < asset->numShapes; ++i)
				{
					g_buffers->rigidPlasticThresholds.push_back(asset->shapePlasticThresholds[i]);
					g_buffers->rigidPlasticCreeps.push_back(asset->shapePlasticCreeps[i]);
				}
			}
			else
			{
				for (int i = 0; i < asset->numShapes; ++i)
				{
					g_buffers->rigidPlasticThresholds.push_back(0.0f);
					g_buffers->rigidPlasticCreeps.push_back(0.0f);
				}
			}
		}
		else
		{
			if (asset->shapePlasticThresholds && asset->shapePlasticCreeps)
			{
				int oldBufferSize = g_buffers->rigidCoefficients.size() - asset->numShapes;

				g_buffers->rigidPlasticThresholds.resize(oldBufferSize);
				g_buffers->rigidPlasticCreeps.resize(oldBufferSize);

				for (int i = 0; i < oldBufferSize; i++)
				{
					g_buffers->rigidPlasticThresholds[i] = 0.0f;
					g_buffers->rigidPlasticCreeps[i] = 0.0f;
				}

				for (int i = 0; i < asset->numShapes; ++i)
				{
					g_buffers->rigidPlasticThresholds.push_back(asset->shapePlasticThresholds[i]);
					g_buffers->rigidPlasticCreeps.push_back(asset->shapePlasticCreeps[i]);
				}

				plasticDeformation = true;
			}
		}
		//*/
		/*
		// add link data to the solver 
		for (int i = 0; i < asset->numSprings; ++i)
		{
			g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 0]);
			g_buffers->springIndices.push_back(asset->springIndices[i * 2 + 1]);

			g_buffers->springStiffness.push_back(asset->springCoefficients[i]);
			g_buffers->springLengths.push_back(asset->springRestLengths[i]);
		}//*/

		NvFlexExtDestroyAsset(asset);

		objects.push_back(renderingInstance);
		//*/
	}
	
	void drawSoft(const RenderingInstance& object) {
		if (!g_drawMesh)
			return;

			const RenderingInstance& instance = object;

			Mesh m;
			m.m_positions.resize(instance.mMesh->m_positions.size());
			m.m_normals.resize(instance.mMesh->m_normals.size());
			m.m_indices = instance.mMesh->m_indices;

			for (int i = 0; i < int(instance.mMesh->m_positions.size()); ++i)
			{
				Vec3 softPos;
				Vec3 softNormal;

				for (int w = 0; w < 4; ++w)
				{
					const int cluster = instance.mSkinningIndices[i * 4 + w];
					const float weight = instance.mSkinningWeights[i * 4 + w];

					if (cluster > -1)
					{
						// offset in the global constraint array
						int rigidIndex = cluster + instance.mOffset;

						Vec3 localPos = Vec3(instance.mMesh->m_positions[i]) - instance.mRigidRestPoses[cluster];

						Vec3 skinnedPos = g_buffers->rigidTranslations[rigidIndex] + Rotate(g_buffers->rigidRotations[rigidIndex], localPos);
						Vec3 skinnedNormal = Rotate(g_buffers->rigidRotations[rigidIndex], instance.mMesh->m_normals[i]);

						softPos += skinnedPos * weight;
						softNormal += skinnedNormal * weight;
					}
				}

				m.m_positions[i] = Point3(softPos);
				m.m_normals[i] = softNormal;
			}

			DrawMesh(&m, instance.mColor);
	}

	void keyDown(int key) {
		if (!this->toolRight.handModel || !this->toolRight.handModel->unable)
			return;
		//std::cout<<"NOSE " << key << endl;
		switch (key) {
		case ' ': {
			this->toolRight.isEmitter = !this->toolRight.isEmitter;
			break;
		}
							/// change brush
		case 39: {
			this->toolRight.emitter.formBrush = (++this->toolRight.emitter.formBrush + FormBrushMax) % FormBrushMax;
			(*this->tranformation[this->toolRight.emitter.formBrush])(&this->meshBrushRight, this->container.radius, this->toolRight.emitter.width);
			break;
		}
						 /// change material
		case 91: {
			this->toolRight.emitter.typeMaterial = ++this->toolRight.emitter.typeMaterial % TypeMaterialMax;
			(*this->tranformation[this->toolRight.emitter.formBrush])(&this->meshBrushRight, this->container.radius, this->toolRight.emitter.width);
			break;
		}
		case 92: {
			this->toolRight.emitter.width -= 2;
			this->toolRight.emitter.width = this->toolRight.emitter.width < 0 ? 1 : this->toolRight.emitter.width;

			(*this->tranformation[this->toolRight.emitter.formBrush])(&this->meshBrushRight, this->container.radius, this->toolRight.emitter.width);
			break;
		}
		case 93: {
			this->toolRight.emitter.width += 2;
			this->toolRight.emitter.width = this->toolRight.emitter.width > WidthMax ? WidthMax : this->toolRight.emitter.width;

			(*this->tranformation[this->toolRight.emitter.formBrush])(&this->meshBrushRight, this->container.radius, this->toolRight.emitter.width);

			break;
		}

		default:
			break;
		}
	}

	void keyUp(int key) {
		if (!this->toolRight.handModel || !this->toolRight.handModel->unable)
			return;
	}
	int idObjectSelect = -1;
	std::vector<Vec3> posObjectSelect;
	void collistionTool() {

		int* neighbors = (int*)NvFlexMap(neighborsBuffer, 0);
		int* counts = (int*)NvFlexMap(countsBuffer, 0);
		int* apiToInternal = (int*)NvFlexMap(apiToInternalBuffer, 0);
		int* internalToApi = (int*)NvFlexMap(internalToApiBuffer, 0);


		int stride = g_buffers->activeIndices.size();
		bool isCollitionLeft = false;
		bool isCollitionRight = false;

		int phaseObject = 0;

		if (!this->toolLeft.isGrasp && !this->toolRight.isGrasp) {
			/// collision for hand left
			for (int i = 0; i < this->numberParticlesLeft; ++i)
			{
				// find offset in the neighbors buffer
				int offset = apiToInternal[i];
				int count = counts[offset];

				Vec3 posTem(g_buffers->positions[i]);
				for (int c = 0; c < count; ++c)
				{
					int neighbor = internalToApi[neighbors[c*stride + offset]];

					if ( neighbor >= g_numSolidParticles ) {
					float res = Distance(posTem, Vec3(g_buffers->positions[neighbor]));
						if(res < this->container.radius ){
							isCollitionLeft = true;
							phaseObject = g_buffers->phases[neighbor];
							break;
						}
					
					}
				}
				if ( isCollitionLeft ) {
					break;
				}
			}
			/// collision for hand right
			for (int i = this->numberParticlesLeft; i < this->numberParticlesRight; ++i)
			{
				// find offset in the neighbors buffer
				int offset = apiToInternal[i];
				int count = counts[offset];

				Vec3 posTem(g_buffers->positions[i]);
				for (int c = 0; c < count; ++c)
				{
					int neighbor = internalToApi[neighbors[c*stride + offset]];

					if (neighbor >= g_numSolidParticles) {
						float res = Distance(posTem, Vec3(g_buffers->positions[neighbor]));
						if (res < this->container.radius) {
							isCollitionRight = true;
							phaseObject = g_buffers->phases[neighbor];
							break;
						}

					}
				}
				if (isCollitionRight) {
					break;
				}
			}
		}
		else if (idObjectSelect >= 0) {
			/*if (!(phaseObject & eNvFlexPhaseFluid))
				this->selectObject(idObjectSelect);*/
		}

		if ( (isCollitionLeft || isCollitionRight) && idObjectSelect < 0) {
			//idObjectSelect = getIdObject(phaseObject) - this->numberHand;
			vrcontrol->haptic(isCollitionLeft, isCollitionRight);
		}
		/*
		else if (idObjectSelect>=0 && !this->toolLeft.isGrasp && !this->toolRight.isGrasp ) {
			this->DeselectObject(idObjectSelect);
			idObjectSelect = -1;			
		}
		*/
		///Interaction
		NvFlexUnmap(neighborsBuffer);
		NvFlexUnmap(countsBuffer);
		NvFlexUnmap(apiToInternalBuffer);
		NvFlexUnmap(internalToApiBuffer);
	}

	void selectObject(int idObject) {
		std::cout << "Id object " << idObject << std::endl;

		//int numParticlesSelect = this->objects[idObject].particles - this->objects[idObject].particlesOffset;

		//std::cout << this->objects[idObject].particlesOffset << " " << this->objects[ idObject ].particles << std::endl;

		std::vector<int> indexSelect;

		Matrix44 m(this->toolRight.matrixTool);

		for (int i = this->objects[idObject].particlesOffset,k=0; i < this->objects[idObject].particles; i++, k++) {
			//indexSelect.push_back(i);
			g_buffers->positions[i].w = 0.0f;		// increase picked particle's mass to force it towards the point
			
			g_buffers->positions[i] = m * g_buffers->positions[i];
			g_buffers->velocities[i] = Vec3(0.0f);		// increase picked particle's mass to force it towards the point
		}
	}
	void DeselectObject(int idObject) {

		for (int i = this->objects[idObject].particlesOffset, k = 0; i < this->objects[idObject].particles; i++, k++) {
			g_buffers->positions[i].w = 1.0f;		// increase picked particle's mass to force it towards the point
			//g_buffers->velocities[i] = Vec3(0.0f);		// increase picked particle's mass to force it towards the point
		}
	}
	void selectParticles(std::vector<int> *particlesSel) {

		// mouse went down, pick new particle
		if (g_mousePicked)
		{
			assert(g_mouseParticle == -1);

			Vec3 origin, dir;
			GetViewRay(g_lastx, g_screenHeight - g_lasty, origin, dir);

			const int numActive = NvFlexGetActiveCount(g_solver);

			g_mouseParticle = PickParticle(origin, dir, &g_buffers->positions[0], &g_buffers->phases[0], numActive, g_params.radius*0.8f, g_mouseT);

			if (g_mouseParticle != -1)
			{
				printf("picked: %d, mass: %f v: %f %f %f\n", g_mouseParticle, g_buffers->positions[g_mouseParticle].w, g_buffers->velocities[g_mouseParticle].x, g_buffers->velocities[g_mouseParticle].y, g_buffers->velocities[g_mouseParticle].z);

				g_mousePos = origin + dir * g_mouseT;
				g_mouseMass = g_buffers->positions[g_mouseParticle].w;
				g_buffers->positions[g_mouseParticle].w = 0.0f;		// increase picked particle's mass to force it towards the point
			}

			g_mousePicked = false;
		}

		// update picked particle position
		if (g_mouseParticle != -1)
		{
			Vec3 p = Lerp(Vec3(g_buffers->positions[g_mouseParticle]), g_mousePos, 0.8f);
			Vec3 delta = p - Vec3(g_buffers->positions[g_mouseParticle]);

			g_buffers->positions[g_mouseParticle].x = p.x;
			g_buffers->positions[g_mouseParticle].y = p.y;
			g_buffers->positions[g_mouseParticle].z = p.z;

			g_buffers->velocities[g_mouseParticle].x = delta.x / g_dt;
			g_buffers->velocities[g_mouseParticle].y = delta.y / g_dt;
			g_buffers->velocities[g_mouseParticle].z = delta.z / g_dt;
		}
	}
};

