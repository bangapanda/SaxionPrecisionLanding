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
	int height = 0;


	//while (!appState.exitRequested() && height < 1) {

		PX4_INFO("Precision Landing initiated!");
		uORB::Subscription vehicle_command_s(ORB_ID(vehicle_command));
		//uORB::Publication<vehicle_command_s> vehicle_command_p(ORB_ID(vehicle_command));
		/* subscribe to vehicle_acceleration topic */
		//uORB::Subscription vehicle_command_s(ORB_ID(vehicle_command));
		int sensor_sub_fd = orb_subscribe(ORB_ID(vehicle_acceleration));
		/* limit the update rate to 5 Hz */
		orb_set_interval(sensor_sub_fd, 200);

		/* advertise attitude topic */
		struct vehicle_command_s command{};
		struct vehicle_attitude_s att;
		memset(&command,0,sizeof(command));
		memset(&att, 0, sizeof(att));
		//orb_advert_t vehicle_command_pub = orb_advertise(ORB_ID(vehicle_command),&command);
		orb_advert_t command_pub = orb_advertise(ORB_ID(vehicle_command), &command);
		orb_advert_t att_pub = orb_advertise(ORB_ID(vehicle_attitude), &att);

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
		//command.param2 = (float)PX4_CUSTOM_MAIN_MODE_AUTO;
		//command.param3 = (float)PX4_CUSTOM_SUB_MODE_AUTO_LOITER;
		orb_publish(ORB_ID(vehicle_command),command_pub, &command);//


		/* one could wait for multiple topics with this technique, just using one here */
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
		//command.param2 = (float)PX4_CUSTOM_MAIN_MODE_AUTO;
		//command.param3 = (float)PX4_CUSTOM_SUB_MODE_AUTO_LOITER;
		orb_publish(ORB_ID(vehicle_command),command_pub, &command);



		struct vehicle_acceleration_s accel;
		/* copy sensors raw data into local buffer */
			orb_copy(ORB_ID(vehicle_acceleration), sensor_sub_fd, &accel);
			PX4_INFO("Accelerometer:\t%8.4f\t%8.4f\t%8.4f",
				 (double)accel.xyz[0],
				 (double)accel.xyz[1],
				 (double)accel.xyz[2]);

		/* set att and publish this information for other apps
		 the following does not have any meaning, it's just an example
		*/
		att.q[0] = accel.xyz[0];
		att.q[1] = accel.xyz[1];
		att.q[2] = accel.xyz[2];

		orb_publish(ORB_ID(vehicle_attitude), att_pub, &att);

		PX4_INFO("exiting");

		//return 0;






		height++;
	//}



	return 0;
}
