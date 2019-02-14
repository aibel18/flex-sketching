/**
====================================================================================
|            @name: ToolModel.h
|          @author: abel ticona
|           @email: jaticona@gmail.com
====================================================================================
*/

#include "ToolModel.h"
#include <fstream>      // std::ifstream

/**
------------------------------------------------------------------------------------
|				Class Name: ToolModel.cpp
|	     Description: class that implements ToolModel.h
------------------------------------------------------------------------------------
*/
static void shapeBrushLineH(float radius, int with, std::vector<vec3> *position) {
	with *= 2;
	vec3 centerObject(float(with / 2));

	for (int x = 0; x < with; ++x) {
		vec3 offset(x - centerObject.x, 0, 0);
		position->push_back(offset*radius);
	}
}
static void shapeBrushLineV(float radius, int with, std::vector<vec3> *position) {
	with *= 2;
	vec3 centerObject(float(with / 2));

	for (int z = 0; z < with; ++z) {
		vec3 offset(0, 0, float(-z));
		position->push_back(offset*radius);
	}
}
static void shapeBrushArc(float radius, int with, std::vector<vec3> *position) {

}

static void shapeBrushPlaneH(float radius, int with, std::vector<vec3> *position) {

	vec3 centerObject(float(with / 2));

	for (int x = 0; x < with; ++x) {
		for (int z = 0; z < with; ++z) {
			vec3 offset(x - centerObject.x, 0, z - centerObject.z);
			position->push_back(offset*radius);
		}
	}
}
static void shapeBrushPlaneV(float radius, int with, std::vector<vec3> *position) {

	vec3 centerObject(float(with / 2));

	for (int y = 0; y < with; ++y) {
		for (int z = 0; z < with; ++z) {
			vec3 offset(0, y - centerObject.y, z - centerObject.z);
			position->push_back(offset*radius);
		}
	}
}

static void shapeBrushCircleH(float radius, int with, std::vector<vec3> *position) {
	vec3 centerObject(float(with / 2));
	float rr = centerObject.x * centerObject.x;

	for (int x = 0; x < with; ++x) {
		for (int z = 0; z < with; ++z) {
			vec3 offset(x - centerObject.x, 0, z - centerObject.z);
			if ((offset.x * offset.x + offset.y * offset.y + offset.z * offset.z) <= rr) {
				position->push_back(offset*radius);
			}
		}
	}
}
static void shapeBrushCircleV(float radius, int with, std::vector<vec3> *position) {
	vec3 centerObject(float(with / 2));
	float rr = centerObject.x * centerObject.x;

	for (int y = 0; y < with; ++y) {
		for (int z = 0; z < with; ++z) {
			vec3 offset(0, y - centerObject.y, z - centerObject.z);
			if (( offset.x * offset.x + offset.y * offset.y + offset.z * offset.z) <= rr) {
				position->push_back(offset*radius);
			}
		}
	}
}

static void shapeBrushCube(float radius, int with, std::vector<vec3> *position) {

	vec3 centerObject(float(with / 2));

	for (int x = 0; x < with; ++x) {
		for (int y = 0; y < with; ++y) {
			for (int z = 0; z < with; ++z) {

				vec3 offset(x - centerObject.x, y - centerObject.y, z - centerObject.z);
				position->push_back(offset*radius);
			}
		}
	}
}
static void shapeBrushSphere(float radius, int with, std::vector<vec3> *position) {

	vec3 centerObject(float(with / 2));
	float rr = centerObject.x * centerObject.x;

	for (int x = 0; x < with; ++x) {
		for (int y = 0; y < with; ++y) {
			for (int z = 0; z < with; ++z) {

				vec3 offset(x - centerObject.x, y - centerObject.y, z - centerObject.z);
				if ((offset.x * offset.x + offset.y * offset.y + offset.z * offset.z) <= rr) {
					position->push_back(offset*radius);
				}

			}
		}
	}
}




ToolModel::ToolModel(){
	/// volume
	this->brushes[0] = &::shapeBrushSphere;
	this->brushes[1] = &::shapeBrushCube;
	

	/// superfice
	//this->brushes[2] = &::shapeBrushPlaneH;
	this->brushes[3] = &::shapeBrushPlaneV;
	//this->brushes[4] = &::shapeBrushCircleH;
	this->brushes[2] = &::shapeBrushCircleV;

	/// line
	this->brushes[4] = &::shapeBrushLineH;
	this->brushes[5] = &::shapeBrushLineV;
	//this->brushes[8] = &::shapeBrushArc;

	
}


ToolModel::~ToolModel(){
}
bool ToolModel::bindHand(TypeHand type, VRControl *vrcontrol, ContainerModel *container) {
	
	this->handModel = &vrcontrol->hand[type];
	this->controlModel = &vrcontrol->con[type];

	this->container = container;
	this->handModel->type = type;


	//*//////////////
	///load particles tool
	std::ifstream file("../../data/controller.objx");	

	file >> this->numberParticles;
	float x,y,z;
	for (int i=0;i< this->numberParticles;i++) {
		file >> x >> y >> z;
		//std::cout << x << " " << y << " " << z << std::endl;
		this->particles.push_back( vec3(x,y,z));
	}
	std::cout << "number particles tool: " << this->numberParticles << std::endl;
	file.close();
	//////////////*/

	return true;
}

void ToolModel::init(bool toolEmitter) {
	this->isEmitter = false;
	this->toolEmitter = toolEmitter;

	if (this->toolEmitter) {
		/// Init position Emitter
		this->emitter.positionIni.init(0.0f, 0.0f, -1.5f);
		/// Init position Hand Control
		float offset = this->handModel->type == rightHand ? 0.9f : -0.9f;
		this->handModel->position.init(offset, 0.0f, 0.0f);
	}
}



void ToolModel::eventController(vr::VREvent_t event) {

	if (!this->handModel || !this->handModel->unable || this->handModel->deviceID != event.trackedDeviceIndex) {
		return;
	}

	switch (event.data.controller.button) {
		case vr::k_EButton_SteamVR_Trigger:
			switch (event.eventType) {
				case vr::VREvent_ButtonPress:
					this->isEmitter = true;
					break;

				case vr::VREvent_ButtonUnpress:
					this->isEmitter = false;
					break;
			}
			break;
		case vr::k_EButton_Grip:
			switch (event.eventType) {
				case vr::VREvent_ButtonPress:
					this->isGrasp = true;
					break;
				case vr::VREvent_ButtonUnpress:
					this->isGrasp = false;
					break;
			}
			break;
		case vr::k_EButton_SteamVR_Touchpad:
			switch (event.eventType) {
				case vr::VREvent_ButtonPress:
					/// change material
					this->emitter.typeMaterial = ++this->emitter.typeMaterial % TypeMaterialMax;
					break;

					/*case vr::VREvent_ButtonUnpress:
						break;

					case vr::VREvent_ButtonTouch:
						//joystickActive = true;
						break;
						*/
				case vr::VREvent_ButtonUntouch:
					joystickActive = false;
					break;
			}
			break;

		case vr::k_EButton_ApplicationMenu:
			switch (event.eventType) {
			case vr::VREvent_ButtonPress:
				this->isEmitter = false;
				if ( this->container->numberNewparticles > 0){
					this->container->newObject = true;

					/// REVIEW
					this->emitter.typeMaterial = this->emitter.typeMaterialLast;
					this->container->typeMaterialTemp = this->emitter.typeMaterial;
				}
				break;

			case vr::VREvent_ButtonUnpress:
				break;
			}
			break;
		//case vr::k_EButton_A:
			
		default: {
			//std::cout << "OTRO" << event.data.controller.button << std::endl;
		}
	}
}


void ToolModel::eventControllerJoystick() {
	if (joystickActive)
		return;

	float umbral = 0.8f;
	float x = this->controlModel->valJoystick[0];
	float y = this->controlModel->valJoystick[1];

	////////////// axis X
	if (x > umbral) {
		/// change brush
		this->emitter.formBrush = (++this->emitter.formBrush + FormBrushMax) % FormBrushMax;
		joystickActive = true;
	}
	else if (x < -umbral) {
		/// change brush
		this->emitter.formBrush = (--this->emitter.formBrush + FormBrushMax) % FormBrushMax;
		joystickActive = true;
	}
	////////////// axis Y
	else if (y > umbral) {
		/// aument tam
		this->emitter.width += 2;
		this->emitter.width = this->emitter.width > WidthMax ? WidthMax : this->emitter.width;
		joystickActive = true;
	}
	else if (y < -umbral) {
		/// reduce tam
		this->emitter.width -= 2;
		this->emitter.width = this->emitter.width < 0 ? 1 : this->emitter.width;
		joystickActive = true;
	}

	/// call function for update tool
	if (joystickActive) {

	}
}

bool ToolModel::update() {
	if (!this->handModel || !this->handModel->unable)
		return false;

	if (this->controlModel->active) {
		this->eventControllerJoystick();
	}

	/// update matrix for tool
	this->handModel->getMatrix44(this->matrixTool);

	/// update matrix for brush
	multiple4xTranslation(this->matrixBrush, this->matrixTool, this->emitter.positionIni);

	/// update position emitterwebview-panel:webview-panel/webview-0
	this->emitter.position.x = this->matrixBrush[12];
	this->emitter.position.y = this->matrixBrush[13];
	this->emitter.position.z = this->matrixBrush[14];

	/// update velocity emitter
	this->emitter.velocity.init(this->handModel->velocity.normalize());
	this->emitter.speed = this->handModel->velocity.magnitude();

	

	return true;
}

void ToolModel::managerParticles(int phase) {
	std::vector<vec3> particlesShapeBrush;

	this->interpolationVelocity(&particlesShapeBrush);
	size_t size = particlesShapeBrush.size();

	/// Emitter Particles
	if ( this->emitter.typeMaterial < 3 ) {
		for (int i=0;i < size ; i++) {
			this->container->verifyState(particlesShapeBrush[i], phase );
		}
		this->emitter.typeMaterialLast = this->emitter.typeMaterial;
	}
	/// Remove particles
	else {		
		int indexRemove = -1;
		bool entro = false;
		for (int i = 0; i < size; i++) {
			this->container->verifyStateInv(particlesShapeBrush[i], &indexRemove);
		}		
	}
}

void ToolModel::refuseParticles() {

}

void ToolModel::interpolationVelocity(std::vector<vec3> *list) {

	int top = MyRound( this->emitter.speed  / this->container->radius * 0.125f);

	std::vector<vec3> particlesShapeBrush;
	(*this->brushes[this->emitter.formBrush])(this->container->radius, this->emitter.width, &particlesShapeBrush);
	size_t size = particlesShapeBrush.size();


	/// copy matrix
	float m[16];
	for (int i = 0; i < 16 ; i++) {
		m[i] = this->matrixBrush[i];
	}	

	for (int i = 0;i <= top ;i++) {
		vec3 vp = this->emitter.position + this->emitter.velocity * (this->container->radius * -i);
		m[12] = vp.x;
		m[13] = vp.y;
		m[14] = vp.z;
		for (int j = 0; j < size; j++) {
			vec3 pr;
			multiple4x3(pr,m, particlesShapeBrush[j]);
			list->push_back(vec3(pr));
		}		
		
	}
}