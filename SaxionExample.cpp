#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/tasks.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>
#include "saxion_example.h"
#include <px4_platform_common/time.h>
#include <unistd.h>
#include <stdio.h>
#include <commander/px4_custom_mode.h>
#include <uORB/uORB.h>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/vehicle_command.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/vehicle_acceleration.h>
#include <uORB/topics/vehicle_attitude.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/app.h>
#include <px4_platform_common/tasks.h>


px4::AppState SaxionExample::appState;



int SaxionExample::main()
{
	appState.setRunning(true);


	while (!appState.exitRequested()) {

		PX4_INFO("Precision Landing initiated!");

		/*Hier Worden toppics uit de firmware opgenomen en uitgelezen*/
		uORB::Subscription vehicle_command_s(ORB_ID(vehicle_command));
		int sensor_sub_fd = orb_subscribe(ORB_ID(vehicle_acceleration));
		/* limit the update rate to 5 Hz */
		orb_set_interval(sensor_sub_fd, 200);

		/* advertise attitude topic */
		struct vehicle_command_s command{};
		struct vehicle_attitude_s att;
		memset(&command,0,sizeof(command));
		memset(&att, 0, sizeof(att));
		
		orb_advert_t command_pub = orb_advertise(ORB_ID(vehicle_command), &command);
		orb_advert_t att_pub = orb_advertise(ORB_ID(vehicle_attitude), &att);

		//Commando naar de PX4 Versturen in dit geval was het commando arm om te testen
		command.timestamp = hrt_absolute_time();
		command.command = vehicle_command_s::VEHICLE_CMD_ARM_AUTHORIZATION_REQUEST;
		command.param1 = (float)1; // base mode
		command.param3 = (float)0; // sub mode
		command.target_system = 1;
		command.target_component = 1;
		command.source_system = 1;
		command.source_component = 1;
		command.confirmation = true;
		command.from_external = false;
		//Versturen van het commando
		orb_publish(ORB_ID(vehicle_command),command_pub, &command);

		//Commando naar de PX4 Versturen in dit geval was het commando takeoff om te testen
		command.timestamp = hrt_absolute_time();
		command.command = vehicle_command_s::VEHICLE_CMD_NAV_TAKEOFF;
		command.param1 = (float)1; // base mode
		command.param3 = (float)0; // sub mode
		command.target_system = 1;
		command.target_component = 1;
		command.source_system = 1;
		command.source_component = 1;
		command.confirmation = true;
		command.from_external = false;
		//Versturen van het commando
		orb_publish(ORB_ID(vehicle_command),command_pub, &command);

		/**
		if PrecisionLandingFailed = true {
			if GPSDenied=false{
			ReadGPSSensors()
			LandingProtocol()
			}
			Else
			ReadHeightSensor()
			LandingProtocol()
		}


		**/

		PX4_INFO("exiting");

		
	}



	return 0;
}
