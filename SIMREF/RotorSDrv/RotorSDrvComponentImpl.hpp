// ======================================================================
// \title  RotorSDrvImpl.hpp
// \author mereweth
// \brief  hpp file for RotorSDrv component implementation class
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

#ifndef RotorSDrv_HPP
#define RotorSDrv_HPP

#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "mav_msgs/Actuators.h"

#include "Os/Mutex.hpp"
#include "Os/Task.hpp"

#include "SIMREF/RotorSDrv/RotorSDrvComponentAc.hpp"
#include "SIMREF/RotorSDrv/RotorSDrvComponentImplCfg.hpp"

namespace SIMREF {

  class RotorSDrvComponentImpl :
    public RotorSDrvComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

        //! Construct object RotorSDrv
        //!
        RotorSDrvComponentImpl(
  #if FW_OBJECT_NAMES == 1
            const char *const compName /*!< The component name*/
  #else
            void
  #endif
        );

        //! Initialize object RotorSDrv
        //!
        void init(
            const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
        );

        //! Destroy object RotorSDrv
        //!
        ~RotorSDrvComponentImpl(void);

        //! Initialize publishers
        //!
        void startPub(void);

        //! Start interrupt task
        Os::Task::TaskStatus startIntTask(NATIVE_INT_TYPE priority,
                                          NATIVE_INT_TYPE stackSize,
                                          NATIVE_INT_TYPE cpuAffinity = -1);

      // ----------------------------------------------------------------------
      // Utility classes for enumerating callbacks
      // ----------------------------------------------------------------------

        class ImuHandler
        {
          public:
              ImuHandler(RotorSDrvComponentImpl* compPtr,
                         int portNum);

              ~ImuHandler();

              void imuCallback(const sensor_msgs::Imu::ConstPtr& msg);

          PRIVATE:

              RotorSDrvComponentImpl* compPtr;

              const unsigned int portNum;

        }; // end class ImuHandler

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
    
        //! Handler implementation for motor
        //!
        void motor_handler(
            const NATIVE_INT_TYPE portNum, /*!< The port number*/
            ROS::mav_msgs::Actuators &actuator
        );

        //! Handler implementation for sched
        //!
        void sched_handler(
            const NATIVE_INT_TYPE portNum, /*!< The port number*/
            NATIVE_UINT_TYPE context /*!< The call order*/
        );

        //! Handler implementation for pingIn
        //!
        void pingIn_handler(
            const NATIVE_INT_TYPE portNum, /*!< The port number*/
            U32 key /*!< Value to return to pinger*/
        );

      // ----------------------------------------------------------------------
      // Member variables
      // ----------------------------------------------------------------------

        //! Entry point for task waiting for interrupt
        static void intTaskEntry(void * ptr);

        //! Task object for RTI task
        //!
        Os::Task m_intTask;

        bool m_rosInited;

        ros::NodeHandle* m_nodeHandle;

        //! Publisher for motor speeds
        //!
        ros::Publisher m_motorPub;

        struct ImuSet {
            Os::Mutex mutex; //! Mutex lock to guard imu object
            ROS::sensor_msgs::ImuNoCov imu; //! imu object
            bool fresh; //! Whether object has been updated
            NATIVE_UINT_TYPE overflows; //! Number of times port overwritten
        } m_imuSet[NUM_SIMIMU_OUTPUT_PORTS];
    };

} // end namespace SIMREF

#endif
