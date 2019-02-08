#ifndef VR_CONTROL_H
#define VR_CONTROL_H

/**
====================================================================================
|            @name: VRControl.h
|          @author: abel ticona
|           @email: jaticona@gmail.com
====================================================================================
*/

#include "HandModel.h"
#include "headers/openvr.h"

//using namespace vr;

struct FrameBuffer{
	bool active = false;
	unsigned int m_nDepthBufferId;
	unsigned int m_nRenderTextureId;
	unsigned int m_nRenderFramebufferId;
	unsigned int m_nResolveTextureId;
	unsigned int m_nResolveFramebufferId;

	float matrix[16];
	float matrixProj[16];
	float matrixEye[16];
};
/**
------------------------------------------------------------------------------------
|				Class Name: VRControl
|	     Description: class that represent the class for comunication the HandModel
										with the simulation
------------------------------------------------------------------------------------
*/

class VRControl {
public:

	HandModel hand[2];
	ControlModel con[2];


	/// pointer for to the VR system
	vr::IVRSystem * vrSystem = NULL;
	int typeDevice = 0;	

	///////////////////////
	float scale;
	vec3 correction;	

	/// HMD

	float mHMD[16] = {1.0f,0.0f,0.0f,0.0f,
										0.0f,1.0f,0.0f,0.0f,
										0.0f,0.0f,1.0f,0.0f,
										0.0f,0.0f,0.0f,1.0f };
	float mHMDEye[16] = { 1.0f,0.0f,0.0f,0.0f,
										0.0f,1.0f,0.0f,0.0f,
										0.0f,0.0f,1.0f,0.0f,
										0.0f,0.0f,0.0f,1.0f };
	float mFrameCenter[16] = { 1.0f,0.0f,0.0f,0.0f,
										0.0f,1.0f,0.0f,0.0f,
										0.0f,0.0f,1.0f,0.0f,
										0.0f,0.0f,0.0f,1.0f };

	bool isHMD = false;
	int idHMD = -1;
	bool unableHMD = false;

	//////////////////
	FrameBuffer frameLeft;
	FrameBuffer frameRight;

	unsigned int renderWidth;
	unsigned int renderHeight;

	float nearClip = 0.01f;
	float farClip = 100.0f;
	/// HMD


	VRControl();
	virtual ~VRControl();

	static VRControl* createControl(int device, void(*managerEvent)(vr::VREvent_t event), float scale, float center[3]);

	virtual void bindHand(TypeHand type,int size, bool withParticles);
	virtual int sizeParticlesHand(int);

	virtual void updateHandModel(int index,vec3 *val, float val2[16]);
	virtual void updateHandModel(int index,vec3 *val, vec3 *val2);
	virtual void runProcedure( );
	virtual bool setupHMD( void(*createFrame) (FrameBuffer *fb, int w, int h), void(*destroyFrame) (FrameBuffer *fb), void(*drawFrame) (FrameBuffer *fb, int w, int h));
	virtual void renderHMD();

	virtual void haptic(bool, bool);

	void(*managerEvent)(vr::VREvent_t event);
};


/**
------------------------------------------------------------------------------------
|				Class Name: OpenControl
|	     Description: class that represent the class for manager the device openVR
------------------------------------------------------------------------------------
*/
class OpenControl : public VRControl {
public:
	OpenControl();
	virtual ~OpenControl();
	virtual void runProcedure();
	virtual bool setupHMD(void(*createFrame) (FrameBuffer *fb, int w, int h), void(*destroyFrame) (FrameBuffer *fb), void(*drawFrame) (FrameBuffer *fb, int w, int h));
	virtual void renderHMD();
	virtual void haptic(bool, bool);
		
private:


	void (*drawFrame)(FrameBuffer *fb, int w, int h);
	void(*destroyFrame)(FrameBuffer *fb);

	bool isHand[2] = {false,false};

	virtual void findDevice();
	virtual void shutDown();	
	virtual void handleInput();
	virtual void tracking();
	virtual void trackingHand(int type);
	virtual void findAxis(int type);



	virtual void getCurrentViewProjectionMatrix(float *r, vr::Hmd_Eye eye);
	virtual vr::HmdQuaternion_t getRotation(vr::HmdMatrix34_t matrix);
	virtual void getMatrix3x4(float m[16], vr::HmdMatrix34_t &matrix);
	virtual void getMatrix4x4(float m[16], vr::HmdMatrix44_t &matrix);
	virtual void getInvMatrix4x4(float m[16]);
	virtual void getInvMatrix3x3(float m[9]);
	virtual void getScale4x4(float *m,float scale);
	virtual void getCorrection(float *m);
	virtual void getCorrection(float *m,float);
	virtual void getScale(float *m, float);

};

/**
------------------------------------------------------------------------------------
|				Class Name: LeapControl
|	     Description: class that represent the class for manager the LeapMotion
------------------------------------------------------------------------------------
*/


#include <Leap.h>

class LeapControl : public Leap::Listener, public VRControl {
public:
	// Create a sample listener and controller
	Leap::Controller controller;
	LeapControl();
	virtual ~LeapControl();
	virtual void runProcedure();
private:
	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
	virtual void onDeviceChange(const Leap::Controller&);
	virtual void onServiceConnect(const Leap::Controller&);
	virtual void onServiceDisconnect(const Leap::Controller&);
};
#endif // VR_CONTROL_H
