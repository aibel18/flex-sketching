
struct MyMesh {
	GpuMesh *mMesh;
	void(*tranformation)(MyMesh* mesh, float radius, int with);

	//Matrix44 mTransforms;

	void updateMesh(float radius, int with) {
		(*tranformation)(this, radius, with);
		//std::cout << "AQUI " << this->mMesh << std::endl;
	}
};
class ToolPiece
{
public:
	ToolPiece(){
	};
	~ToolPiece(){
	};

	static void meshBrushCube(MyMesh*myMesh, float radius, int with ) {

		Mesh* mt = CreateCubeMesh();
		//mt->Transform(ScaleMatrix(radius*2));
		mt->Transform( ScaleMatrix(radius*with) );
		myMesh->mMesh = CreateGpuMesh(mt);

		delete mt;		
	}
	static void meshBrushSphere(MyMesh*myMesh, float radius, int with) {

		Mesh* mt = CreateSphere(16,16, 0.5f*radius*with);
		//mt->Transform(ScaleMatrix(radius*with));
		myMesh->mMesh = CreateGpuMesh(mt);

		delete mt;
	}
	static void meshBrushPlaneH(MyMesh*myMesh, float radius, int with) {

		Mesh* mt = CreateCubeMesh();
		//mt->Transform(ScaleMatrix(radius * 2));
		mt->Transform( ScaleMatrix(Vec3(radius*with,radius , radius*with)));
		myMesh->mMesh = CreateGpuMesh(mt);

		delete mt;
	}
	static void meshBrushPlaneV(MyMesh*myMesh, float radius, int with) {

		Mesh* mt = CreateCubeMesh();
		//mt->Transform(ScaleMatrix(radius * 2));
		mt->Transform(ScaleMatrix(Vec3(radius, radius*with, radius*with)));
		myMesh->mMesh = CreateGpuMesh(mt);

		delete mt;
	}
	static void meshBrushCircleH(MyMesh*myMesh, float radius, int with) {

		Mesh* mt = CreateCylindre(16, 0.5f*radius*with, radius);
		myMesh->mMesh = CreateGpuMesh(mt);

		delete mt;
	}
	static void meshBrushCircleV(MyMesh*myMesh, float radius, int with) {

		Mesh* mt = CreateCylindre(16, 0.5f*radius*with, radius);
		mt->Transform(RotationMatrix(DegToRad(90), Vec3(0, 0, 1) ));
		myMesh->mMesh = CreateGpuMesh(mt);

		delete mt;
	}

	static void meshBrushLineH(MyMesh*myMesh, float radius, int with) {

		with *= 2;
		Mesh* mt = CreateCubeMesh();
		//mt->Transform(ScaleMatrix(radius * 2));
		mt->Transform(/*TranslationMatrix(Point3(0.0f, 0.0f, -with * radius* 0.5f))*/ScaleMatrix(Vec3(radius*with, radius, radius)));
		myMesh->mMesh = CreateGpuMesh(mt);		

		delete mt;
	}
	static void meshBrushLineV(MyMesh*myMesh, float radius, int with) {

		with *= 2;
		Mesh* mt = CreateCubeMesh();
		//mt->Transform(ScaleMatrix(radius * 2));
		mt->Transform(TranslationMatrix(Point3(0.0f, 0.0f, -with * radius* 0.5f))*ScaleMatrix(Vec3(radius, radius, radius*with)));
		myMesh->mMesh = CreateGpuMesh(mt);

		delete mt;
	}

	//static void 
};




//void 

