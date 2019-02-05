#include "VRControl.h"

using namespace Leap;

/**
------------------------------------------------------------------------------------
|				Class Name: LeapControl
|	     Description: class that implements LeapControl
------------------------------------------------------------------------------------
*/
LeapControl::LeapControl() :VRControl() {

	this->typeDevice = 1;

	/// Add the sample listener when done
	this->controller.addListener(*this);
	//Interaction
	/*if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
	//*/
	//std::cout << "LeapMotion: Constructor\n";
	std::cout << "LeapMotion: Add Listener\n";
}
LeapControl::~LeapControl() {
	//std::cout << "LeapMotion: Destructor\n";
	/// Remove the sample listener when done
	controller.removeListener(*this);
	std::cout << "LeapMotion: Remove Listener\n";
}

void LeapControl::runProcedure() {
}

/**
| Function that update the skeleton hand
*/
void LeapControl::onFrame(const Leap::Controller& controller) {

	float scale = 0.01f;
	//Vector position(0, -2.0, 0);
	Vector position(0, 0.0, 0);

	const Frame frame = controller.frame();
	HandList handsList = frame.hands();

	for (int offHand = 0; offHand < handsList.count(); offHand++) {

		if (!handsList[offHand].isValid()) {
			std::cout << "LeapMotion: NOT TRACKING " << (handsList[offHand].isLeft() ? "LEFT" : "RIGHT") << "\n";
			continue;
		}

		int indexHand = handsList[offHand].isLeft() ? leftHand : rightHand;

		HandModel &h = this->hand[indexHand];

		if (!h.unable)
			continue;

		const Vector vVel = handsList[offHand].palmVelocity()*scale;

		Leap::Matrix handTransform = handsList[offHand].basis();
		handTransform.origin = handsList[offHand].palmPosition()*scale + position;

		vec3 valsHand[5];
		vec3 valsFinger[25];/// 5 bones for finger		

		const Vector vPalm = handTransform.origin;
		const Vector vDir = handTransform.xBasis;
		const Vector vNor = handTransform.yBasis;
		const Vector vRig = handTransform.zBasis;

		valsHand[0].init(vVel[0], vVel[1], vVel[2]);
		valsHand[1].init(vPalm[0], vPalm[1], vPalm[2]);
		valsHand[2].init(vDir[0], vDir[1], vDir[2]);
		valsHand[3].init(vNor[0], vNor[1], vNor[2]);
		valsHand[4].init(vRig[0], vRig[1], vRig[2]);

		/// get the coor the hands ///

		if (h.sizeParticlesHand > 0) {

			const FingerList fingers = handsList[offHand].fingers();

			for (int i = 0, ei = fingers.count(); i < ei; i++) {
				const Finger& finger = fingers[i];
				for (int j = Leap::Bone::TYPE_METACARPAL; j <= Leap::Bone::TYPE_DISTAL; j++) {
					const Leap::Bone& bone = finger.bone(static_cast<Leap::Bone::Type>(j));

					if (j == Leap::Bone::TYPE_METACARPAL) {
						Vector v = bone.prevJoint()*scale + position;
						valsFinger[i * 5 + j].init(v[0], v[1], v[2]);
						v = bone.nextJoint()*scale + position;
						valsFinger[i * 5 + j + 1].init(v[0], v[1], v[2]);
					}
					else {
						Vector v = bone.nextJoint()*scale + position;
						valsFinger[i * 5 + j + 1].init(v[0], v[1], v[2]);
					}
				}
			}
		}

		/// fin get the coor the hands ///
		if (h.type == leftHand) {
			valsHand[2].x *= -1.0f; valsHand[2].y *= -1.0f; valsHand[2].z *= -1.0f;
		}
		this->updateHandModel(h.type, valsHand, valsFinger);
	}
}


void LeapControl::onInit(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Initialized" << std::endl;
}

void LeapControl::onConnect(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Connected" << std::endl;
}

void LeapControl::onDisconnect(const Leap::Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "LeapMotion: Disconnected" << std::endl;
}

void LeapControl::onExit(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Exited" << std::endl;
}

void LeapControl::onFocusGained(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Focus Gained" << std::endl;
}

void LeapControl::onFocusLost(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Focus Lost" << std::endl;
}

void LeapControl::onDeviceChange(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Device Changed" << std::endl;
	const DeviceList devices = controller.devices();
	/*
	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}*/
}

void LeapControl::onServiceConnect(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Service Connected" << std::endl;
}

void LeapControl::onServiceDisconnect(const Leap::Controller& controller) {
	std::cout << "LeapMotion: Service Disconnected" << std::endl;
}