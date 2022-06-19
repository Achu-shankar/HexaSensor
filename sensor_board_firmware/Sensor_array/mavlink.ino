void InitialisePixhawkComm(){
  Mav_Request_Data();
}

void Mav_Request_Data()
{
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
//  int sysid = 1;                   ///< ID 20 for this airplane. 1 PX, 255 ground station
//  int compid = 158;                ///< The component sending the message
//  int type = MAV_TYPE_QUADROTOR;   ///< This system is an airplane / fixed wing
// 
//  // Define the system type, in this case an airplane -> on-board controller
//  uint8_t system_type = MAV_TYPE_GENERIC;
//  uint8_t autopilot_type = MAV_AUTOPILOT_INVALID;
// 
//  uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
//  uint32_t custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
//  uint8_t system_state = MAV_STATE_STANDBY; ///< System ready for flight
//
//  // Initialize the required buffers
////  mavlink_message_t msg;
////  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
// 
//  // Pack the message
//  mavlink_msg_heartbeat_pack(1,0, &msg, type, autopilot_type, system_mode, custom_mode, system_state);
// 
//  // Copy the message to the send buffer
//  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
//  pxSerial.write(buf, len);

  // STREAMS that can be requested
  /*
   * Definitions are in common.h: enum MAV_DATA_STREAM
   *   
   * MAV_DATA_STREAM_ALL=0, // Enable all data streams
   * MAV_DATA_STREAM_RAW_SENSORS=1, /* Enable IMU_RAW, GPS_RAW, GPS_STATUS packets.
   * MAV_DATA_STREAM_EXTENDED_STATUS=2, /* Enable GPS_STATUS, CONTROL_STATUS, AUX_STATUS
   * MAV_DATA_STREAM_RC_CHANNELS=3, /* Enable RC_CHANNELS_SCALED, RC_CHANNELS_RAW, SERVO_OUTPUT_RAW
   * MAV_DATA_STREAM_RAW_CONTROLLER=4, /* Enable ATTITUDE_CONTROLLER_OUTPUT, POSITION_CONTROLLER_OUTPUT, NAV_CONTROLLER_OUTPUT.
   * MAV_DATA_STREAM_POSITION=6, /* Enable LOCAL_POSITION, GLOBAL_POSITION/GLOBAL_POSITION_INT messages.
   * MAV_DATA_STREAM_EXTRA1=10, /* Dependent on the autopilot
   * MAV_DATA_STREAM_EXTRA2=11, /* Dependent on the autopilot
   * MAV_DATA_STREAM_EXTRA3=12, /* Dependent on the autopilot
   * MAV_DATA_STREAM_ENUM_END=13,
   * 
   * Data in PixHawk available in:
   *  - Battery, amperage and voltage (SYS_STATUS) in MAV_DATA_STREAM_EXTENDED_STATUS
   *  - Gyro info (IMU_SCALED) in MAV_DATA_STREAM_EXTRA1
   */

  // To be setup according to the needed information to be requested from the Pixhawk
  const int  maxStreams = 3;
  const uint8_t MAVStreams[maxStreams] = {MAV_DATA_STREAM_POSITION, MAV_DATA_STREAM_EXTENDED_STATUS, MAV_DATA_STREAM_EXTRA1};
  const uint16_t MAVRates[maxStreams] = {0x2,0x10,0x10};
//  const uint8_t  MAVStreams[maxStreams] = {MAV_DATA_STREAM_EXTENDED_STATUS,MAV_DATA_STREAM_EXTRA1,MAV_DATA_STREAM_EXTRA2,MAV_DATA_STREAM_EXTRA3};
//  const uint16_t MAVRates[maxStreams] = {0x02,0x02,0x02,0x02};
//  const uint8_t  MAVStreams[maxStreams] = {MAV_DATA_STREAM_ALL};
//  const uint16_t MAVRates[maxStreams] = {0x02};
    
  for (int i=0; i < maxStreams; i++) {   
    /*
     * mavlink_msg_request_data_stream_pack(system_id, component_id, 
     *    &msg, 
     *    target_system, target_component, 
     *    MAV_DATA_STREAM_POSITION, 10000000, 1);
     *    
     * mavlink_msg_request_data_stream_pack(uint8_t system_id, uint8_t component_id, 
     *    mavlink_message_t* msg,
     *    uint8_t target_system, uint8_t target_component, uint8_t req_stream_id, 
     *    uint16_t req_message_rate, uint8_t start_stop)
     * 
     */
    mavlink_msg_request_data_stream_pack(2, 200, &msg, 1, 0, MAVStreams[i], MAVRates[i], 1);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    pxSerial.write(buf, len);
  }
}

void pixhawk_comm_receive() {
  mavlink_message_t msg;
  mavlink_status_t status;

  while(pxSerial.available()>0) {
    uint8_t c =pxSerial.read(); 

    // Try to get a new message
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
      
//    Serial.println("getting data from pixhawk");

      // Handle message
      switch(msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT:  // #0: Heartbeat
          {
            // E.g. read GCS heartbeat and go into
            // comm lost mode if timer times out
//            Serial.println("heart beat");
          }
          break;

        case MAVLINK_MSG_ID_ATTITUDE:  // #1: SYS_STATUS
          {
//            Serial.println("getting attitude data");
            /* Message decoding: PRIMITIVE
             *    mavlink_msg_sys_status_decode(const mavlink_message_t* msg, mavlink_sys_status_t* sys_status)
             */
            mavlink_attitude_t attitude;
            mavlink_msg_attitude_decode(&msg, &attitude);
            time_boot_ms_attitude = attitude.time_boot_ms;
            roll = attitude.roll;
            pitch = attitude.pitch;
            yaw   = attitude.yaw;
            rollspeed = attitude.rollspeed;
            pitchspeed = attitude.pitchspeed;
            yawspeed  = attitude.yawspeed;
            
//            Serial.print("roll :"); Serial.print(roll);  Serial.println();
            Serial.print("roll time stamp :"); Serial.print(time_boot_ms_attitude);  Serial.println();
          }
          break;

        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:  // #22: PARAM_VALUE
          {
            /* Message decoding: PRIMITIVE
             *    mavlink_msg_param_value_decode(const mavlink_message_t* msg, mavlink_param_value_t* param_value)
             */
            //mavlink_message_t* msg;
            mavlink_local_position_ned_t local_pos;
            mavlink_msg_local_position_ned_decode(&msg, &local_pos);
            time_boot_ms_loc_pos = local_pos.time_boot_ms;
            x = local_pos.x;
            y = local_pos.y;
            z = local_pos.z;
            vx = local_pos.vx;
            vy = local_pos.vy;
            vz = local_pos.vz;
            
            Serial.print("local pos time stamp :"); Serial.print(time_boot_ms_loc_pos);  Serial.println();
          }
          break;

        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:  // #27: RAW_IMU
          {
            /* Message decoding: PRIMITIVE
             *    static inline void mavlink_msg_raw_imu_decode(const mavlink_message_t* msg, mavlink_raw_imu_t* raw_imu)
             */
            mavlink_global_position_int_t global_pos;
            mavlink_msg_global_position_int_decode(&msg, &global_pos);
            time_boot_ms_pix_gps = global_pos.time_boot_ms;
            pix_lat = global_pos.lat;
            pix_lon = global_pos.lon;
            pix_alt = global_pos.alt;
            pix_relative_alt = global_pos.relative_alt;
            pix_gps_vx = global_pos.vx;
            pix_gps_vy = global_pos.vy;
            pix_gps_vz = global_pos.vz;
           
            
            Serial.print("gps time stamp :"); Serial.print(time_boot_ms_pix_gps);  Serial.println();
//            Serial.print("lat :"); Serial.print(global_pos.lat);  Serial.println();
          }
          break;

//        case MAVLINK_MSG_ID_ATTITUDE:  // #30
//          {
//            /* Message decoding: PRIMITIVE
//             *    mavlink_msg_attitude_decode(const mavlink_message_t* msg, mavlink_attitude_t* attitude)
//             */
//            mavlink_attitude_t attitude;
//            mavlink_msg_attitude_decode(&msg, &attitude);
//            Serial.print("attitude :"); Serial.print(attitude.roll);  Serial.println();
//
////            if(attitude.roll>1) leds_modo = 0;
////            else if(attitude.roll<-1) leds_modo = 2;
////            else leds_modo=1;
//          }
//          break;
//
//          case MAVLINK_MSG_ID_GPS_RAW_INT :  // #30
//          {
//            /* Message decoding: PRIMITIVE
//             *    mavlink_msg_attitude_decode(const mavlink_message_t* msg, mavlink_attitude_t* attitude)
//             */
//            mavlink_gps_raw_int_t gpsdata;
//            mavlink_msg_gps_raw_int_decode(&msg,&gpsdata);
//            Serial.print("Latitude :"); Serial.print(gpsdata.lat);  Serial.println();
////            Serial.print("Pixhawk TimeStamp : ");  Serial.println(gpsdata.time_boot_ms);
//            Serial.print("Num of Sat : ");  Serial.println(gpsdata.satellites_visible);
//            
//            
//          }
//          break;
          

        
       default:
          break;
      }
    }
  }
}
