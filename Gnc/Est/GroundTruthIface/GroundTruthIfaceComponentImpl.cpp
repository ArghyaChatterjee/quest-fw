// ======================================================================
// \title  GroundTruthIfaceImpl.cpp
// \author mereweth
// \brief  cpp file for GroundTruthIface component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged. Any commercial use must be negotiated with the Office
// of Technology Transfer at the California Institute of Technology.
//
// This software may be subject to U.S. export control laws and
// regulations.  By accepting this document, the user agrees to comply
// with all U.S. export laws and regulations.  User has the
// responsibility to obtain export licenses, or other export authority
// as may be required before exporting such information to foreign
// countries or providing access to foreign persons.
// ======================================================================


#include <Gnc/Est/GroundTruthIface/GroundTruthIfaceComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"

#include <math.h>
#include <stdio.h>

#include <ros/callback_queue.h>

#include <Os/File.hpp>

//#define DEBUG_PRINT(x,...) printf(x,##__VA_ARGS__); fflush(stdout)
#define DEBUG_PRINT(x,...)

namespace Gnc {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  GroundTruthIfaceComponentImpl ::
#if FW_OBJECT_NAMES == 1
    GroundTruthIfaceComponentImpl(
        const char *const compName
    ) :
      GroundTruthIfaceComponentBase(compName),
#else
    GroundTruthIfaceImpl(void),
#endif
    m_rosInited(false),
    m_tbDes(TB_NONE),
    m_nodeHandle(NULL),
    m_odometrySet() // zero-initialize instead of default-initializing
  {
      for (int i = 0; i < FW_NUM_ARRAY_ELEMENTS(m_odometrySet); i++) {
          m_odometrySet[i].fresh = false;
          m_odometrySet[i].overflows = 0u;
      }
  }

    void GroundTruthIfaceComponentImpl ::
      init(
          const NATIVE_INT_TYPE instance
      )
    {
      GroundTruthIfaceComponentBase::init(instance);
    }

    GroundTruthIfaceComponentImpl ::
      ~GroundTruthIfaceComponentImpl(void)
    {

    }

    void GroundTruthIfaceComponentImpl ::
      setTBDes(TimeBase tbDes) {
        this->m_tbDes = tbDes;
    }

    Os::Task::TaskStatus GroundTruthIfaceComponentImpl ::
      startIntTask(NATIVE_INT_TYPE priority,
                   NATIVE_INT_TYPE stackSize,
                   NATIVE_INT_TYPE cpuAffinity) {
        Os::TaskString name("FILTIFACE");
        Os::Task::TaskStatus stat = this->m_intTask.start(name, 0, priority,
          stackSize, GroundTruthIfaceComponentImpl::intTaskEntry, this, cpuAffinity);

        if (stat != Os::Task::TASK_OK) {
            DEBUG_PRINT("Task start error: %d\n",stat);
        }

        return stat;
    }

    void GroundTruthIfaceComponentImpl ::
      disableRos() {
        this->m_rosInited = false;
    }
  
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

    void GroundTruthIfaceComponentImpl ::
      sched_handler(
          const NATIVE_INT_TYPE portNum,
          NATIVE_UINT_TYPE context
      )
    {
        for (int i = 0; i < FW_NUM_ARRAY_ELEMENTS(m_odometrySet); i++) {
            m_odometrySet[i].mutex.lock();
            if (m_odometrySet[i].fresh) {
                if (this->isConnected_Odometry_OutputPort(i)) {
                    // mimics driver hardware getting and sending sensor data
                    this->Odometry_out(i, m_odometrySet[i].odometry);
                }
                else {
                    DEBUG_PRINT("IMU state update port %d not connected\n", i);
                }
                m_odometrySet[i].fresh = false;
            }
            // TODO(mereweth) - notify that no new odometry received?
            m_odometrySet[i].mutex.unLock();
        }
    }

    void GroundTruthIfaceComponentImpl ::
      pingIn_handler(
          const NATIVE_INT_TYPE portNum,
          U32 key
      )
    {
        // TODO(mereweth) - check that message-wait task is OK if we add one
        this->pingOut_out(portNum, key);
    }


    // ----------------------------------------------------------------------
    // Member function definitions
    // ----------------------------------------------------------------------

    //! Entry point for task waiting for messages
    void GroundTruthIfaceComponentImpl ::
      intTaskEntry(void * ptr) {
        // TODO(mereweth) - prevent calling twice; free m_nodeHandle
        DEBUG_PRINT("GroundTruthIface task entry\n");

        FW_ASSERT(ptr);
        GroundTruthIfaceComponentImpl* compPtr = (GroundTruthIfaceComponentImpl*) ptr;
        //compPtr->log_ACTIVITY_LO_HLROSIFACE_IntTaskStarted();

        compPtr->m_nodeHandle = new ros::NodeHandle();
        ros::NodeHandle* n = compPtr->m_nodeHandle;
        FW_ASSERT(n);
        ros::CallbackQueue localCallbacks;
        n->setCallbackQueue(&localCallbacks);

        if (ros::isShuttingDown()) {
            return;
        }

        OdometryHandler updateHandler(compPtr, 0);
        updateHandler.tbDes = compPtr->m_tbDes;

        ros::Subscriber updateSub = n->subscribe("odom_in", 10,
                                                 &OdometryHandler::odometryCallback,
                                                 &updateHandler,
                                                 ros::TransportHints().tcpNoDelay());

        compPtr->m_rosInited = true;
	
        while (1) {
            // TODO(mereweth) - check for and respond to ping
            localCallbacks.callAvailable(ros::WallDuration(0, 10 * 1000 * 1000));
        }
    }

    GroundTruthIfaceComponentImpl :: TimeBaseHolder ::
      TimeBaseHolder() :
      tbDes(TB_NONE)
    {
    }
  
    GroundTruthIfaceComponentImpl :: OdometryHandler ::
      OdometryHandler(GroundTruthIfaceComponentImpl* compPtr,
                      int portNum) :
      compPtr(compPtr),
      portNum(portNum)
    {
        FW_ASSERT(compPtr);
        FW_ASSERT(portNum < NUM_ODOMETRY_OUTPUT_PORTS); //compPtr->getNum_Odometry_OutputPorts());
    }

    GroundTruthIfaceComponentImpl :: OdometryHandler :: ~OdometryHandler()
    {

    }

    void GroundTruthIfaceComponentImpl :: OdometryHandler ::
      odometryCallback(const nav_msgs::Odometry::ConstPtr& msg)
    {
        FW_ASSERT(this->compPtr);
        FW_ASSERT(this->portNum < NUM_ODOMETRY_OUTPUT_PORTS);

        DEBUG_PRINT("odometry port handler %d\n", this->portNum);

        if (!std::isfinite(msg->header.stamp.sec) ||
            !std::isfinite(msg->header.stamp.nsec)) {
            //TODO(mereweth) - EVR
            return;
        }

        if (!std::isfinite(msg->pose.pose.position.x) ||
            !std::isfinite(msg->pose.pose.position.y) ||
            !std::isfinite(msg->pose.pose.position.z) ||

            !std::isfinite(msg->pose.pose.orientation.x) ||
            !std::isfinite(msg->pose.pose.orientation.y) ||
            !std::isfinite(msg->pose.pose.orientation.z) ||
            !std::isfinite(msg->pose.pose.orientation.w) ||

            !std::isfinite(msg->twist.twist.linear.x) ||
            !std::isfinite(msg->twist.twist.linear.y) ||
            !std::isfinite(msg->twist.twist.linear.z) ||

            !std::isfinite(msg->twist.twist.angular.x) ||
            !std::isfinite(msg->twist.twist.angular.y) ||
            !std::isfinite(msg->twist.twist.angular.z)) {
            //TODO(mereweth) - EVR
            return;
        }

        Fw::Time rosTime(TB_ROS_TIME, 0,
                         msg->header.stamp.sec,
                         msg->header.stamp.nsec / 1000);

        // if port is not connected, default to no conversion
        Fw::Time convTime = rosTime;

        if (this->compPtr->isConnected_convertTime_OutputPort(0)) {
            bool success = false;
            convTime = this->compPtr->convertTime_out(0, rosTime, this->tbDes, 0, success);
            if (!success) {
                // TODO(Mereweth) - EVR
                return;
            }
        }
        
        {
            using namespace ROS::std_msgs;
            using namespace ROS::nav_msgs;
            using namespace ROS::geometry_msgs;

            F64 dummy[36] = { 0.0 };

            OdometryAccel odometry(
              Header(msg->header.seq,
                     convTime,
                     // TODO(mereweth) - convert frame id
                     0/*Fw::EightyCharString(msg->header.frame_id.data())*/),
              0/*Fw::EightyCharString(msg->child_frame_id.data())*/,
              PoseWithCovariance(Pose(Point(msg->pose.pose.position.x,
                                            msg->pose.pose.position.y,
                                            msg->pose.pose.position.z),
                                      Quaternion(msg->pose.pose.orientation.x,
                                                 msg->pose.pose.orientation.y,
                                                 msg->pose.pose.orientation.z,
                                                 msg->pose.pose.orientation.w)),
				 dummy, FW_NUM_ARRAY_ELEMENTS(dummy)),
              TwistWithCovariance(Twist(Vector3(msg->twist.twist.linear.x,
                                                msg->twist.twist.linear.y,
                                                msg->twist.twist.linear.z),
                                        Vector3(msg->twist.twist.angular.x,
                                                msg->twist.twist.angular.y,
                                                msg->twist.twist.angular.z)),
				  dummy, FW_NUM_ARRAY_ELEMENTS(dummy)),
	      AccelWithCovariance(Accel(Vector3(0.0, 0.0, 0.0),
                                        Vector3(0.0, 0.0, 0.0)),
				  dummy, FW_NUM_ARRAY_ELEMENTS(dummy))
            ); // end Odometry constructor

            this->compPtr->m_odometrySet[this->portNum].mutex.lock();
            if (this->compPtr->m_odometrySet[this->portNum].fresh) {
                this->compPtr->m_odometrySet[this->portNum].overflows++;
                DEBUG_PRINT("Overwriting odometry port %d before Sched\n", this->portNum);
            }
            this->compPtr->m_odometrySet[this->portNum].odometry = odometry;
            this->compPtr->m_odometrySet[this->portNum].fresh = true;
        }
        this->compPtr->m_odometrySet[this->portNum].mutex.unLock();
    }

} // end namespace
