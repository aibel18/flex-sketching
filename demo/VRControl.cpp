/**
====================================================================================
|            @name: VRControl.h
|          @author: abel ticona
|           @email: jaticona@gmail.com
====================================================================================
*/

#include "VRControl.h"


/**
------------------------------------------------------------------------------------
|				Class Name: VRControl
|	     Description: class that implements VRControl
------------------------------------------------------------------------------------
*/

/**
| Constructor default
*/
VRControl::VRControl() {
}
VRControl::~VRControl() {
}

VRControl* VRControl::createControl(int device, void(*managerEvent)(vr::VREvent_t event),float scale, float center[3]) {

	switch (device) {
		case 1:{
			return new LeapControl();
			return 0;
		}
		case 2:{
			OpenControl *oControl = new OpenControl();
			oControl->managerEvent = managerEvent;

			//// calibration
			oControl->scale = scale;
			oControl->correction.init(center[0], center[1], center[2]);
			return oControl;
		}
		default:{
			return NULL;
		}
	}
}
void VRControl::bindHand(TypeHand type, int particleIndex, bool withParticles=false) {

	this->hand[type].init( particleIndex, withParticles);
}
int VRControl::sizeParticlesHand(int index) {
	return this->hand[index].sizeParticlesHand;
}
/**
| Function that update the skeleton hand
*/
void VRControl::updateHandModel(int index, vec3 *val, vec3 *val2) {
	this->hand[index].update(val);
	if(this->hand[index].sizeParticlesHand > 0)
		this->hand[index].updateFingers(val2);
}
void VRControl::updateHandModel(int index, vec3 *val, float m[16]) {
	
	/// update position hand
	this->hand[index].update(val);

	/// update position fingers
	if (this->hand[index].sizeParticlesHand > 0){
		this->hand[index].calculateFingers(m);
	}
}
void VRControl::runProcedure( ) {
}
bool VRControl::setupHMD( void(*createFrame)(FrameBuffer *fb, int w, int h), void(*destroyFrame)(FrameBuffer *fb), void(*drawFrame)(FrameBuffer *fb, int w, int h))
{
	return false;
}
void VRControl::renderHMD() {	
}

void VRControl::haptic() {
}






/**
------------------------------------------------------------------------------------
|				Class Name: OpenControl
|	     Description: class that implements OpenControl
------------------------------------------------------------------------------------
*/

const float EPSILON = 0.00001f;

OpenControl::OpenControl() :VRControl() {

	this->typeDevice = 2;
	

	/// Definition of the init error
	vr::EVRInitError eError = vr::VRInitError_None;
	/// load steamVR Runtime
	//vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
	vrSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None) {
		vrSystem = NULL;
		printf("System VR: Unable to init VR runtime\n");
		return;
	}	

	/// Verify Compositor
	if (!vr::VRCompositor()) {
		printf("Compositor initialization failed. See log file for details\n");
		return;
	}

	printf("System VR: Constructor\n");
}
OpenControl::~OpenControl() {
	printf( "System VR: Destructor\n");
	this->shutDown();
}



//float scale = 10.0f;
//float error = 0;// 0.2f*scale;
//float positionY = -0.3f*scale - error;//3.5f;//3.6f;
bool OpenControl::setupHMD(void(*createFrame) (FrameBuffer *fb,int w, int h), void(*destroyFrame) (FrameBuffer *fb), void(*drawFrame) (FrameBuffer *fb, int w, int h) ) {

	/// Init OpenGL	
	if (!vrSystem)
		return false;


	///////////////// CAMERA
	vr::HmdMatrix44_t matProjLeft = this->vrSystem->GetProjectionMatrix(vr::Eye_Left, this->nearClip, this->farClip);
	vr::HmdMatrix44_t matProjRight = this->vrSystem->GetProjectionMatrix(vr::Eye_Right, this->nearClip, this->farClip);

	this->getMatrix4x4(this->frameLeft.matrixProj, matProjLeft);
	this->getMatrix4x4(this->frameRight.matrixProj, matProjRight);

	vr::HmdMatrix34_t matEyeLeft = this->vrSystem->GetEyeToHeadTransform(vr::Eye_Left);
	vr::HmdMatrix34_t matEyeRight = this->vrSystem->GetEyeToHeadTransform(vr::Eye_Right);

	this->getMatrix3x4(this->frameLeft.matrixEye, matEyeLeft);
	this->getMatrix3x4(this->frameRight.matrixEye, matEyeRight);

	//this->getCorrection(this->frameLeft.matrixEye,this->scale);
	//this->getCorrection(this->frameRight.matrixEye, this->scale);

	this->getScale(this->frameLeft.matrixEye, this->scale);
	this->getScale(this->frameRight.matrixEye, this->scale);

	this->getInvMatrix4x4(this->frameLeft.matrixEye);
	this->getInvMatrix4x4(this->frameRight.matrixEye);	


	for (unsigned int id=0;id<16;id++) {
		this->mFrameCenter[id] = this->frameRight.matrixEye[id];
	}
	this->mFrameCenter[12] = 0.0f;
	

	//std::cout << "System VR: SETUP CAMERA \n";


	///////////////// STEREO
	this->vrSystem->GetRecommendedRenderTargetSize(&this->renderWidth, &this->renderHeight);

	std::cout << "DIMENSION  : " << this->renderWidth << " " << this->renderHeight << std::endl;

	this->frameLeft.active = false;
	this->frameRight.active = false;
	(*createFrame)(&frameLeft, this->renderWidth, this->renderHeight);
	(*createFrame)(&frameRight, this->renderWidth, this->renderHeight);

	if (!this->frameLeft.active || !this->frameRight.active) {
		return false;
	}
	/// bind
	this->destroyFrame = destroyFrame;
	this->drawFrame = drawFrame;

	this->unableHMD = true;
	printf("System VR: SETUP SETEREO RENDER\n");
	//////
	
	return true;	
}
void OpenControl::shutDown() {
	if (vrSystem != NULL) {
		vr::VR_Shutdown();
		vrSystem = NULL;
	}
	if (frameLeft.active) {
		(*this->destroyFrame)(&frameLeft);
	}
	if (frameRight.active) {
		(*this->destroyFrame)(&frameRight);
	}

}
void OpenControl::runProcedure() {
	if (!vrSystem)
		return;	
	handleInput();
	tracking();
}
void OpenControl::handleInput() {
	/// Process SteamVR events
	vr::VREvent_t event;

	while (vrSystem->PollNextEvent(&event, sizeof(event))) {
		switch (event.eventType) {
			case vr::VREvent_TrackedDeviceActivated:
				printf("System VR: EVENT Device : %d attached\n", event.trackedDeviceIndex);
				break;
			default:				
				if (event.eventType >= 200 && event.eventType <= 203){ //Button events range from 200-203					
					(*this->managerEvent)(event);
				}
				else{
					//printf("System VR: Event: %d\n", event.eventType);
				}
		}
	}
}

void OpenControl::findDevice() {
	for (unsigned int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {

		if (!vrSystem->IsTrackedDeviceConnected(i))
			continue;
		vr::ETrackedDeviceClass trackedDeviceClass = vrSystem->GetTrackedDeviceClass(i);

		printf("DEVICE %d:\n", i);

		/// find type device
		if(trackedDeviceClass == vr::ETrackedDeviceClass::TrackedDeviceClass_HMD && this->unableHMD){
			idHMD = i;
			isHMD = true;
			printf( "HMD conected\n");
		}
		else if(trackedDeviceClass == vr::ETrackedDeviceClass::TrackedDeviceClass_Controller ){


			vr::ETrackedControllerRole role = vrSystem->GetControllerRoleForTrackedDeviceIndex(i);
			if (role == vr::TrackedControllerRole_Invalid) {
			}
			else if (role == vr::TrackedControllerRole_LeftHand && this->hand[leftHand].unable && !this->isHand[leftHand]) {
				this->hand[leftHand].deviceID = i;
				this->isHand[leftHand] = true;

				this->findAxis(leftHand);
				printf( "Left Hand conected\n");				
			}
			else if (role == vr::TrackedControllerRole_RightHand && this->hand[rightHand].unable && !this->isHand[rightHand]) {
				this->hand[rightHand].deviceID = i;
				this->isHand[rightHand] = true;

				this->findAxis(rightHand);
				printf( "Right Hand conected\n");
			}

			

		}
		else if(trackedDeviceClass == vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker){
		}
	}
}

void OpenControl::findAxis(int typeHand) {
	for (int x = 0; x < vr::k_unControllerStateAxisCount; x++){

		int prop = vrSystem->GetInt32TrackedDeviceProperty(this->hand[typeHand].deviceID,
			(vr::ETrackedDeviceProperty)(vr::Prop_Axis0Type_Int32 + x));

		if (prop == vr::k_eControllerAxis_Trigger) {
			prop -= 2;
			printf("TRIGER %d: %d\n", prop, x);
			this->con[typeHand].indexTrigger = prop;
		}
		else if (prop == vr::k_eControllerAxis_TrackPad || prop == vr::k_eControllerAxis_Joystick) {
			prop -= 2;
			printf("JOYSTICK %d: %d \n", prop, x);
			this->con[typeHand].indexJoystick = prop;
		}
		else {
			//printf("otro %d: %d \n", prop, x);
		}
	}

	this->con[typeHand].active = true;
}

vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
void OpenControl::tracking() {
	if( (this->unableHMD && !this->isHMD ) || (this->hand[leftHand].unable && !this->isHand[leftHand]) || (this->hand[rightHand].unable && !this->isHand[rightHand])){
		this->findDevice();
		//return;
		//printf( "BUSCAA\n" );
	}

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	if ( isHMD && vrSystem->IsTrackedDeviceConnected(idHMD) ) {
		
		getMatrix3x4(this->mHMD, m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
		
		getCorrection(this->mHMD, this->scale);
		getInvMatrix4x4(this->mHMD);

		/// projection eyes
		multiple4x4(this->frameLeft.matrix, this->frameLeft.matrixEye, this->mHMD);
		multiple4x4(this->frameRight.matrix, this->frameRight.matrixEye, this->mHMD);

		/// projection center eyes
		multiple4x4(this->mHMDEye, this->mFrameCenter, this->mHMD);
		/*
		for (int i=0;i<16;i++) {
			if(i==12)				
				this->mHMDEye[i] = this->frameRight.matrix[i];
		}
		this->mHMDEye[12] = 0.0f;*/
	}
	this->trackingHand(leftHand);
	this->trackingHand(rightHand);

}

void OpenControl::trackingHand(int typeHand) {
	vr::VRControllerState_t controllerState;
	/// get the coor the hands ///
	if (this->isHand[typeHand] && vrSystem->IsTrackedDeviceConnected(this->hand[typeHand].deviceID)) {

		vrSystem->GetControllerState(this->hand[typeHand].deviceID, &controllerState, sizeof(controllerState));

		float m[16];
		getMatrix3x4(m, m_rTrackedDevicePose[this->hand[typeHand].deviceID].mDeviceToAbsoluteTracking);
		getCorrection(m, scale);
		vr::HmdVector3_t &velocity = m_rTrackedDevicePose[this->hand[typeHand].deviceID].vVelocity;

		vec3 valsHand[5];

		valsHand[0].init(velocity.v[0], velocity.v[1], velocity.v[2]);
		valsHand[1].init(m[12], m[13], m[14]);

		valsHand[2].init(m[0], m[1], m[2]);
		valsHand[3].init(m[4], m[5], m[6]);
		valsHand[4].init(m[8], m[9], m[10]);
		this->updateHandModel(typeHand, valsHand, m);


		///////////
		// update force control
		this->con[typeHand].valTrigger = controllerState.rAxis[this->con[typeHand].indexTrigger].x;

		this->con[typeHand].valJoystick[0] = controllerState.rAxis[this->con[typeHand].indexJoystick].x;
		this->con[typeHand].valJoystick[1] = controllerState.rAxis[this->con[typeHand].indexJoystick].y;

		//printf("%d : %d %d -> tX=%f tY=%f  padx=%f pady=%f\n", typeHand,trigger[typeHand], joystick[typeHand], tx, ty, jx, jy);
	}
	/// fin get the coor the hands ///		
}

void OpenControl::haptic() {
	this->vrSystem->TriggerHapticPulse(this->hand[rightHand].deviceID, 0, 200);
	//this->vrSystem->TriggerHapticPulse(this->hand[rightHand].deviceID, 1, 100);
	//this->vrSystem->TriggerHapticPulse(this->hand[rightHand].deviceID, 2, 100);

	//vr::VRInput()->TriggerHapticVibrationAction(m_rHand[Left].m_actionHaptic, 0, 1, 4.f, 1.0f, vr::k_ulInvalidInputValueHandle);

}

void OpenControl::renderHMD() {

	// for now as fast as possible
	if (vrSystem && this->isHMD) {

		//std::cout<< "RENDER\n";
		//*
		//this->getCurrentViewProjectionMatrix(this->frameLeft.matrix, vr::Eye_Left);
		//this->getCurrentViewProjectionMatrix(this->frameRight.matrix, vr::Eye_Right);

		/*for (int i=0;i<16;i++) {
			this->frameRight.matrix[i] = this->ma
		}*/
		
		this->drawFrame(&this->frameLeft, this->renderWidth, this->renderHeight);
		this->drawFrame(&this->frameRight, this->renderWidth, this->renderHeight);

		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)this->frameLeft.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
		
		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)this->frameRight.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
		//*/
	}

}
void OpenControl::getCurrentViewProjectionMatrix(float *r, vr::Hmd_Eye eye) {

	//float t[16];
	
	if (eye == vr::Eye_Left){
		multiple4x4(r, this->frameLeft.matrixProj, this->frameLeft.matrixEye);
		//getInvMatrix4x4(r);
		//multiple4x4(r, t, this->mHMD);
	}
	else {
		multiple4x4(r, this->frameRight.matrixProj, this->frameRight.matrixEye);
		//getInvMatrix4x4(r);
		//multiple4x4(r, t, this->mHMD);
	}
}

/// Inverse Matrix4x4
void OpenControl::getInvMatrix4x4(float m[16]) {
	
	// R^-1
	float r[9] = { m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10] };
	getInvMatrix3x3(r);
	m[0] = r[0];  m[1] = r[1];  m[2] = r[2];
	m[4] = r[3];  m[5] = r[4];  m[6] = r[5];
	m[8] = r[6];  m[9] = r[7];  m[10] = r[8];

	// -R^-1 * T
	float x = m[12];
	float y = m[13];
	float z = m[14];
	m[12] = -(r[0] * x + r[3] * y + r[6] * z);
	m[13] = -(r[1] * x + r[4] * y + r[7] * z);
	m[14] = -(r[2] * x + r[5] * y + r[8] * z);
}

/// Inverse Matrix3x3
void OpenControl::getInvMatrix3x3(float m[9]) {

	float determinant, invDeterminant;
	float tmp[9];

	tmp[0] = m[4] * m[8] - m[5] * m[7];
	tmp[1] = m[2] * m[7] - m[1] * m[8];
	tmp[2] = m[1] * m[5] - m[2] * m[4];
	tmp[3] = m[5] * m[6] - m[3] * m[8];
	tmp[4] = m[0] * m[8] - m[2] * m[6];
	tmp[5] = m[2] * m[3] - m[0] * m[5];
	tmp[6] = m[3] * m[7] - m[4] * m[6];
	tmp[7] = m[1] * m[6] - m[0] * m[7];
	tmp[8] = m[0] * m[4] - m[1] * m[3];

	// check determinant if it is 0
	determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
	if (fabs(determinant) <= EPSILON){
		m[0] = 1; m[1] = 0; m[2] = 0;
		m[3] = 0; m[4] = 1; m[5] = 0;
		m[6] = 0; m[7] = 0; m[8] = 1;
		return; // cannot inverse, make it idenety matrix
	}

	// divide by the determinant
	invDeterminant = 1.0f / determinant;
	m[0] = invDeterminant * tmp[0];
	m[1] = invDeterminant * tmp[1];
	m[2] = invDeterminant * tmp[2];
	m[3] = invDeterminant * tmp[3];
	m[4] = invDeterminant * tmp[4];
	m[5] = invDeterminant * tmp[5];
	m[6] = invDeterminant * tmp[6];
	m[7] = invDeterminant * tmp[7];
	m[8] = invDeterminant * tmp[8];
}

void OpenControl::getScale4x4(float* m, float s) {
	m[0] *= s;   m[4] *= s;   m[8] *= s;   m[12] *= s;
	m[1] *= s;   m[5] *= s;   m[9] *= s;   m[13] *= s;
	m[2] *= s;   m[6] *= s;   m[10] *= s;   m[14] *= s;
}
void OpenControl::getMatrix3x4(float m[16], vr::HmdMatrix34_t &matrix) {

	/// rotation x
	m[0] = matrix.m[0][0];
	m[1] = matrix.m[1][0];
	m[2] = matrix.m[2][0];
	m[3] = 0;//matrix.m[3][0];

	/// rotation y
	m[4] = matrix.m[0][1];
	m[5] = matrix.m[1][1];
	m[6] = matrix.m[2][1];
	m[7] = 0;//matrix.m[3][1];

	/// rotation z
	m[8] = matrix.m[0][2];
	m[9] = matrix.m[1][2];
	m[10] = matrix.m[2][2];
	m[11] = 0;//matrix.m[3][2];

	/// translation
	m[12] = matrix.m[0][3];// * scale;
	m[13] = matrix.m[1][3];// * scale - positionY;
	m[14] = matrix.m[2][3];// * scale;
	m[15] = 1;//matrix.m[3][3] * scale;

}
void OpenControl::getMatrix4x4(float m[16], vr::HmdMatrix44_t &matrix) {

	/// rotation x
	m[0] = matrix.m[0][0];
	m[1] = matrix.m[1][0];
	m[2] = matrix.m[2][0];
	m[3] = matrix.m[3][0];

	/// rotation y
	m[4] = matrix.m[0][1];
	m[5] = matrix.m[1][1];
	m[6] = matrix.m[2][1];
	m[7] = matrix.m[3][1];

	/// rotation z
	m[8] = matrix.m[0][2];
	m[9] = matrix.m[1][2];
	m[10] = matrix.m[2][2];
	m[11] = matrix.m[3][2];

	/// translation
	m[12] = matrix.m[0][3];//* scale;
	m[13] = matrix.m[1][3];//* scale - positionY;
	m[14] = matrix.m[2][3];//* scale;
	m[15] = matrix.m[3][3];//* scale;

}
vr::HmdQuaternion_t OpenControl::getRotation(vr::HmdMatrix34_t matrix) {
	vr::HmdQuaternion_t q;

	q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
	q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
	q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);

	return q;
}


void OpenControl::getCorrection(float *matrix) {
	matrix[12] += this->correction.x;
	matrix[13] += this->correction.y;
	matrix[14] += this->correction.z;
}
void OpenControl::getCorrection(float *matrix,float scale) {
	matrix[12] = matrix[12] * scale + this->correction.x;
	matrix[13] = matrix[13] * scale + this->correction.y;
	matrix[14] = matrix[14] * scale + this->correction.z;
}
void OpenControl::getScale(float* matrix, float scale) {
	matrix[12] = matrix[12] * scale;
	matrix[13] = matrix[13] * scale;
	matrix[14] = matrix[14] * scale;
}