//Interaction
#include "VRControl.h"
//#include "headers/openvr.h"

int main() {
	//VRControl *vr = VRControl::createControl(2,NULL);
	
	OpenControl *oc = new OpenControl();

	if (!oc->setupHMD(0,0,0)) {
		printf("SALIMOS\n");
		return 0;
	}
	Sleep(2000);
	printf("DALE\n");
	while (true) {
		oc->runProcedure();
		Sleep(1);
	}

	printf("SALIOO\n");
	delete oc;

	/*vr::EVRInitError eError = vr::VRInitError_None;
	/// load steamVR Runtime
	//vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
	vr::IVRSystem *vrSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None) {
		vrSystem = NULL;
		printf("System VR: Unable to init VR runtime\n");
		return 0;
	}


	//Sleep(2000);
	printf("DALE\n");
	while (true) {
		/// Process SteamVR events
		vr::VREvent_t event;
		//if (vrSystem->PollNextEvent(&event, sizeof(event))) {

		if (vrSystem->PollNextEvent(&event, sizeof(event))) {
			switch (event.eventType) {
			case vr::VREvent_TrackedDeviceActivated:
				printf("System VR: EVENT Device : %d attached\n", event.trackedDeviceIndex);
				break;
			default:
				if (event.eventType >= 200 && event.eventType <= 203) { //Button events range from 200-203
					printf("evento button\n");
					//(*this->managerEvent)(event);
				}
				else {
					//printf("System VR: Event: %d\n", event.eventType);
				}
			}
		}
		//Sleep(1);
	}*/








	return 0;
}