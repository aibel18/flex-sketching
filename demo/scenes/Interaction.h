class Interaction : public Scene
{
public:

	Interaction(const char* name) : Scene(name){}	


	~Interaction() {
		//destroyDrawPoints(&this->idContainerVBO, &this->indicesContainerVBO);
		tool.destroy();
	}
	/// Interaction
	ModelingTool tool;
	//float centerContainer[3] = { 0.0f,-4.25f, 10.0f };
	//float centerContainer[3] = { 0.0f,2.25f, 0.0f };
	float centerContainer[3] = { 0.0f,-2.0f, 0.0f };
	void Initialize()
	{		
		float scale = 10.0f;//15.0f;

		/// Interaction
		if(!vrcontrol){
			vrcontrol = VRControl::createControl(2, &handleInputCallback, scale, centerContainer);
			

			/// Interaction
			vrcontrol->setupHMD(&createFrameCallback, &destroyFrameCallback, &drawFrameCallback);			
		}

		// no fluids or sdf based collision
		//g_solverDesc.featureMode = eNvFlexFeatureModeSimpleSolids;

		float radius = 0.1875f;// 0.1f;
		float restDistanceFluid = radius * 0.7f;//0.55f;
		float restDistanceSolid = radius * 0.65f;//0.55f;

		g_sceneLower = Vec3(0.0f);
		

		g_numSubsteps = 2;

		g_params.radius = radius;
		g_params.fluidRestDistance = restDistanceFluid;
		g_params.solidRestDistance = restDistanceSolid;
		g_params.numIterations = 4;
		g_params.viscosity = 1.0f;
		g_params.dynamicFriction = 0.5f;
		g_params.staticFriction = 0.0f;
		g_params.particleCollisionMargin = g_params.fluidRestDistance*0.5f;
		g_params.collisionDistance = g_params.fluidRestDistance*0.5f;
		g_params.vorticityConfinement = 120.0f;//40.0f;
		g_params.cohesion = 0.0025f;
		g_params.drag = 0.06f;
		g_params.lift = 0.f;
		g_params.solidPressure = 0.0f;
		g_params.smoothing = 1.0f;
		g_params.relaxationFactor = 1.0f;

		g_params.restitution = 0.001f;
				
		g_params.smoothing = 1.f;
		
		g_params.surfaceTension = 0.0f;

		// limit velocity to CFL condition
		g_params.maxSpeed = restDistanceFluid*g_numSubsteps / g_dt;

		
		g_params.solidPressure = 0.f;		

		g_maxDiffuseParticles = 128 * 1024 * 2;//5;
		g_diffuseScale = 0.25f;
		g_diffuseShadow = false;
		g_diffuseColor = 2.5f;
		g_diffuseMotionScale = 1.5f;
		g_params.diffuseThreshold *= 0.01f;
		g_params.diffuseBallistic = 35;

		g_fluidColor = Vec4(0.113f, 0.425f, 0.55f, 1.f);		

		g_numExtraParticles = 128 * 1024 * 1; //128 * 1024 * 5;

		g_lightDistance = 20.0f;

		g_waveFloorTilt = 0.0f;
		g_waveFrequency = 1.5f;
		g_waveAmplitude = 2.0f;

		g_warmup = true;

		// draw options		
		g_drawPoints = false;//true;//false;
		g_drawEllipsoids = true;//false;//true;
		g_drawDiffuse = true;

		this->numParticlesContainer = (DIM - 1) * 4;


		this->tool.start(true, true, false, false);
		this->tool.init(g_params.fluidRestDistance, centerContainer);
		
		//InitParticlesContainer();

		
	}

	virtual void CenterCamera()
	{
		/*
		g_camPos.x = -4.0;//-4.0f;
		g_camPos.y = 2.5f;
		g_camPos.z = -0.5f;

		g_camAngle = Vec3(-DegToRad(90.0f), -DegToRad(15.0f), 0.0f);
		//*/
		g_camPos.x = 0.0f + centerContainer[0];
		g_camPos.y = 4.0f ;//+ centerContainer[1];
		g_camPos.z = 0.0f + centerContainer[2];//15.0f;
		g_camAngle = Vec3(0.0f, -DegToRad(15.0f), 0.0f);
		//*/
	}

	virtual void Update() {

		this->tool.update();
	}

	virtual void Sync(){

		if( this->tool.toSync ){
			int rigidOffset = 0;
			if(g_buffers->rigidOffsets.size())
				rigidOffset = g_buffers->rigidOffsets.size() - 1;

		//if( false )
		//*
			NvFlexSetRigids(g_solver, g_buffers->rigidOffsets.buffer, g_buffers->rigidIndices.buffer, g_buffers->rigidLocalPositions.buffer, g_buffers->rigidLocalNormals.buffer, g_buffers->rigidCoefficients.buffer, g_buffers->rigidPlasticThresholds.buffer, g_buffers->rigidPlasticCreeps.buffer, g_buffers->rigidRotations.buffer, g_buffers->rigidTranslations.buffer, rigidOffset, g_buffers->rigidIndices.size());
			//* /

			this->tool.toSync = false;
		}
	}

	virtual void KeyDown(int key){
		this->tool.keyDown(key);
	}

	virtual void KeyUp(int key) {
		this->tool.keyUp(key);
	}
		
	virtual void EventController(vr::VREvent_t event) {

		this->tool.eventController(event);
		
	}
	virtual void Draw(float radius,float aspect, float fov, Matrix44 lightTransform) {

		//DrawSystemCoordinate(radius, 300.0f);

		this->tool.draw(radius, aspect, fov, lightTransform);

		//drawPoints(this->idContainerVBO, this->indicesContainerVBO, this->numParticlesContainer, 0, radius*0.90f, float(g_screenWidth), aspect, fov, g_lightPos, g_lightTarget, lightTransform, g_shadowMap);
	}


	void InitParticlesContainer() {

		initDrawPoints(&this->idContainerVBO, &this->indicesContainerVBO, this->numParticlesContainer);

		int with = DIM - 1;

		Vec3 centerObject(float(with / 2));

		int i = 0;
		for (int z = 0; z < with; ++z, i++) {
			Vec3 offset( - centerObject[0], 0.0F, z - centerObject[2]);
			this->particlesContainer.push_back(offset*g_params.fluidRestDistance);
			this->indicesContainer.push_back(i);
		}
		for (int z = 0; z < with; ++z, i++) {
			Vec3 offset(centerObject[0], 0.0F, z - centerObject[2]);
			this->particlesContainer.push_back(offset*g_params.fluidRestDistance);
			this->indicesContainer.push_back(i);
		}
		for (int z = 0; z < with; ++z, i++) {
			Vec3 offset(z- centerObject[0], 0.0F, - centerObject[2]);
			this->particlesContainer.push_back(offset*g_params.fluidRestDistance);
			this->indicesContainer.push_back(i);
		}
		for (int z = 0; z < with; ++z, i++) {
			Vec3 offset(z - centerObject[0], 0.0F, centerObject[2]);
			this->particlesContainer.push_back(offset*g_params.fluidRestDistance);
			this->indicesContainer.push_back(i);
		}

		updatePoints(this->idContainerVBO, this->indicesContainerVBO, (float*)&this->particlesContainer[0], &this->indicesContainer[0], this->numParticlesContainer);

	}

	unsigned int idContainerVBO;
	unsigned int indicesContainerVBO;
	int numParticlesContainer;
	std::vector<Vector3> particlesContainer;
	std::vector<int> indicesContainer;
	
};

