static const int DIM = 151;

Vec4 calculateCenterMass(const Vec4* positions, int posIni, int posFin) {
	Vec4 center;
	for (int i = posIni; i < posFin; i++) {
		center += positions[i];
	}
	center /= float(posFin - posIni);

	//return Vec3(center);
	return center;
}

int getMaterial(int idObj, int typeMaterial) {

	switch (typeMaterial) {

		/// solid
	case 0: {
		return NvFlexMakePhase(idObj, 0);
	}
					/// soft
	case 1: {
		return NvFlexMakePhase(idObj, eNvFlexPhaseSelfCollide | eNvFlexPhaseSelfCollideFilter);
	}
					/// fluid
	case 2: {
		return NvFlexMakePhase(idObj, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid);
	}
					/// gas
	case 3: {
		return NvFlexMakePhase(idObj, eNvFlexPhaseFluid);
	}
	default:
		return NvFlexMakePhase(idObj, 0);
	}

}

int getIdObject(int phase) {
	return eNvFlexPhaseGroupMask & phase;
}


void discretization(Vec3& position, int positionDis[3], float dimCon,float centerContainer[], int centerMatrix[]) {

	positionDis[0] = Round((position[0] /*- centerContainer[0]*/) * (DIM - 1) / dimCon + centerMatrix[0]);
	positionDis[1] = Round((position[1] /*- centerContainer[1]*/) * (DIM - 1) / dimCon + centerMatrix[1]);
	positionDis[2] = Round((position[2] /*- centerContainer[2]*/) * (DIM - 1) / dimCon + centerMatrix[2]);

	/*
	positionDis[0] = Round(position[0] * (DIM - 1) / dimCon + centerMatrix[0]);
	positionDis[1] = Round(position[1] * (DIM - 1) / dimCon + centerMatrix[1]);
	positionDis[2] = Round(position[2] * (DIM - 1) / dimCon + centerMatrix[2]);
	//*/
}
void discretizationInv(int positionDis[3], Vec3& position, float dimCon, float centerContainer[], int centerMatrix[]) {
	position[0] = (positionDis[0] - centerMatrix[0]) * dimCon / (DIM - 1) /*+ centerContainer[0]*/;
	position[1] = (positionDis[1] - centerMatrix[1]) * dimCon / (DIM - 1) /*+ centerContainer[1]*/;
	position[2] = (positionDis[2] - centerMatrix[2]) * dimCon / (DIM - 1) /*+ centerContainer[2]*/;

	/*	
	position[0] = (positionDis[0] - centerMatrix[0]) * dimCon / (DIM - 1);
	position[1] = (positionDis[1] - centerMatrix[1]) * dimCon / (DIM - 1);
	position[2] = (positionDis[2] - centerMatrix[2]) * dimCon / (DIM - 1);
	//*/
}
bool verifyStateWithNormalization(Vec3& position,int index, bool (*matrixModel)[DIM][DIM], int(*matrixModelIndex)[DIM][DIM], float dimCon, float centerContainer[], int centerMatrix[]) {
	int posdis[3];

	//std::cout << "##########" << std::endl;
	//std::cout << position[0] << " " << position[1] << " " << position[2] << std::endl;

	discretization(position, posdis, dimCon,centerContainer, centerMatrix);

	//std::cout << posdis[0] << " " << posdis[1] << " " << posdis[2] << std::endl;

	if (posdis[0] >= DIM || posdis[1] >= DIM || posdis[2] >= DIM || posdis[0] < 0 || posdis[1] < 0 || posdis[2] < 0) {
		//std::cout << "OVERFLOW" << std::endl;
		return false;
	}

	if (!matrixModel[posdis[0]][posdis[1]][posdis[2]]) {
		discretizationInv(posdis, position, dimCon, centerContainer, centerMatrix);
		matrixModel[posdis[0]][posdis[1]][posdis[2]] = true;
		matrixModelIndex[posdis[0]][posdis[1]][posdis[2]] = index;
		return true;
	}
	return false;
}
bool verifyStateWithNormalizationInv(Vec3& position, int *index, bool(*matrixModel)[DIM][DIM], int(*matrixModelIndex)[DIM][DIM], float dimCon, float centerContainer[], int centerMatrix[]) {
	int posdis[3];

	//std::cout << "##########" << std::endl;
	//std::cout << position[0] << " " << position[1] << " " << position[2] << std::endl;

	discretization(position, posdis, dimCon, centerContainer, centerMatrix);

	//std::cout << posdis[0] << " " << posdis[1] << " " << posdis[2] << std::endl;

	if (posdis[0] >= DIM || posdis[1] >= DIM || posdis[2] >= DIM || posdis[0] < 0 || posdis[1] < 0 || posdis[2] < 0) {
		//std::cout << "OVERFLOW" << std::endl;
		return false;
	}

	if (matrixModel[posdis[0]][posdis[1]][posdis[2]]) {
		discretizationInv(posdis, position, dimCon, centerContainer, centerMatrix);
		matrixModel[posdis[0]][posdis[1]][posdis[2]] = false;
		*index = matrixModelIndex[posdis[0]][posdis[1]][posdis[2]];
		matrixModelIndex[posdis[0]][posdis[1]][posdis[2]] = -1;
		return true;
	}
	return false;
}
bool verifyStateWithOutNormalization(Vec3& position,bool (*matrixModel)[DIM][DIM], float dimCon, float centerContainer[], int centerMatrix[]) {
	int posdis[3];
	discretization(position, posdis, dimCon,centerContainer, centerMatrix);

	if (!matrixModel[posdis[0]][posdis[1]][posdis[2]]) {
		matrixModel[posdis[0]][posdis[1]][posdis[2]] = true;
		return true;
	}
	return false;
}
void normalization(Vec3& position, float dimCon, float centerContainer[], int centerMatrix[]) {
	int posdis[3];
	discretization(position, posdis, dimCon, centerContainer, centerMatrix);
	discretizationInv(posdis, position, dimCon, centerContainer, centerMatrix);
}



void DrawCircle(const Vec3& pos, const Vec3& u, const Vec3& v, float radius, int segments) {
	BeginLines();

	Vec3 start = pos + radius * v;

	for (int i = 1; i <= segments; ++i)
	{
		float theta = k2Pi * (float(i) / segments);
		Vec3 end = pos + radius * sinf(theta)*u + radius * cosf(theta)*v;

		DrawLine(start, end, Vec4(1.0f));

		start = end;
	}
	EndLines();
}
void DrawCube(const Vec3& pos, const Vec3& u, const Vec3& v, float radius, int segments) {

	Vec3 cube[8];

	cube[0] = Vec3(pos[0] + radius, pos[1] + radius, pos[2] + radius);
	cube[1] = Vec3(pos[0] + radius, pos[1] + radius, pos[2] - radius);
	cube[2] = Vec3(pos[0] - radius, pos[1] + radius, pos[2] - radius);
	cube[3] = Vec3(pos[0] - radius, pos[1] + radius, pos[2] + radius);

	cube[4] = Vec3(pos[0] + radius, pos[1] - radius, pos[2] + radius);
	cube[5] = Vec3(pos[0] + radius, pos[1] - radius, pos[2] - radius);
	cube[6] = Vec3(pos[0] - radius, pos[1] - radius, pos[2] - radius);
	cube[7] = Vec3(pos[0] - radius, pos[1] - radius, pos[2] + radius);

	BeginLines();

	DrawLine(cube[0], cube[1], Vec4(1.0f));
	DrawLine(cube[1], cube[2], Vec4(1.0f));
	DrawLine(cube[2], cube[3], Vec4(1.0f));
	DrawLine(cube[3], cube[0], Vec4(1.0f));

	DrawLine(cube[4], cube[5], Vec4(1.0f));
	DrawLine(cube[5], cube[6], Vec4(1.0f));
	DrawLine(cube[6], cube[7], Vec4(1.0f));
	DrawLine(cube[7], cube[4], Vec4(1.0f));

	DrawLine(cube[0], cube[4], Vec4(1.0f));
	DrawLine(cube[1], cube[5], Vec4(1.0f));
	DrawLine(cube[2], cube[6], Vec4(1.0f));
	DrawLine(cube[3], cube[7], Vec4(1.0f));

	EndLines();
}


void DrawSystemCoordinate(float radius,float scale) {

	Vec3 c(0.0f);

	Vec3 x(radius*scale ,0, 0);
	Vec3 y(0, radius*scale, 0);
	Vec3 z(0, 0, radius*scale);


	BeginLines();

	DrawLine(c, x, Vec4(1.0f,0,0,1.0f));
	DrawLine(c, y, Vec4(0,1.0f,0, 1.0f));
	DrawLine(c, z, Vec4(0,0,1.0f, 1.0f));

	EndLines();
}