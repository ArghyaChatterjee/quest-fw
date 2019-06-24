#include <Components.hpp>

enum {
    CORE_NONE = -1,
    CORE_0 = 0,
    CORE_1 = 1,
    CORE_2 = 2,
    CORE_3 = 3,

    CORE_CDH = CORE_0,
    CORE_RPC = CORE_1,
    CORE_CAM = CORE_2,
    CORE_GNC = CORE_3
};

#include <Fw/Types/Assert.hpp>
#include <Os/Task.hpp>
#include <Os/Log.hpp>
#include <Fw/Types/MallocAllocator.hpp>
#include <Fw/Types/MmapAllocator.hpp>

#if defined TGT_OS_TYPE_LINUX || TGT_OS_TYPE_DARWIN
#include <getopt.h>
#include <stdlib.h>
#include <ctype.h>
#endif

#if defined TGT_OS_TYPE_LINUX
#include <sys/wait.h>
#endif

#include <unistd.h>

#define DEBUG_PRINT(x,...) printf(x,##__VA_ARGS__); fflush(stdout)
//#define DEBUG_PRINT(x,...)

//#define LINUX_DEV

#define PRM_PATH "/eng/CARREFPrmDb.dat"

// Registry
#if FW_OBJECT_REGISTRATION == 1
static Fw::SimpleObjRegistry simpleReg;
#endif

Fw::MallocAllocator seqMallocator;
Fw::MallocAllocator buffMallocator;
Fw::MmapAllocator hiresMallocator;

// Component instance pointers

Svc::CommandDispatcherImpl* cmdDisp_ptr = 0;
Svc::CmdSequencerComponentImpl* cmdSeq_ptr = 0;
Svc::ActiveTextLoggerComponentImpl* textLogger_ptr = 0;
Svc::ActiveLoggerImpl* eventLogger_ptr = 0;
Svc::TlmChanImpl* chanTlm_ptr = 0;
Svc::PrmDbImpl* prmDb_ptr = 0;
Svc::SocketGndIfImpl* sockGndIf_ptr = 0;
SnapdragonFlight::SnapdragonHealthComponentImpl* snapHealth_ptr = 0;
Svc::TimeConvertComponentImpl* timeConvert_ptr = 0;
HLProc::HLRosIfaceComponentImpl* hlRosIface_ptr = 0;
Gnc::AckermannIfaceComponentImpl* ackermannIface_ptr = 0;
Gnc::FilterIfaceComponentImpl* filterIface_ptr = 0;
Gnc::GroundTruthIfaceComponentImpl* gtIface_ptr = 0;
ROS::RosSeqComponentImpl* rosSeq_ptr = 0;

Svc::RateGroupDecouplerComponentImpl* rgDecouple_ptr = 0;
Svc::QueuedDecouplerComponentImpl* passiveDataPasser_ptr = 0;
Svc::ActiveDecouplerComponentImpl* imuDecouple_ptr = 0;
Svc::ActiveDecouplerComponentImpl* actDecouple_ptr = 0;
Svc::RateGroupDriverImpl* rgDcplDrv_ptr = 0;
Svc::RateGroupDriverImpl* rgGncDrv_ptr = 0;
Svc::PassiveRateGroupImpl* rgOp_ptr = 0;
Svc::PassiveRateGroupImpl* rgTlm_ptr = 0;
Svc::PassiveRateGroupImpl* rgDev_ptr = 0;
Svc::LinuxTimeImpl* linuxTime_ptr = 0;
Svc::AssertFatalAdapterComponentImpl* fatalAdapter_ptr = 0;
Svc::FatalHandlerComponentImpl* fatalHandler_ptr = 0;
Gnc::FrameTransformComponentImpl* ctrlXest_ptr = 0;
Gnc::ImuProcComponentImpl* imuProc_ptr = 0;
Gnc::ActuatorAdapterComponentImpl* actuatorAdapter_ptr = 0;
Gnc::SigGenComponentImpl* sigGen_ptr = 0;
Gnc::AttFilterComponentImpl* attFilter_ptr = 0;
Gnc::AckermannConverterComponentImpl* ackermannConverter_ptr = 0;
Drv::MPU9250ComponentImpl* mpu9250_ptr = 0;

SnapdragonFlight::BlspSpiDriverComponentImpl* spiDrvSnap_ptr = 0;
SnapdragonFlight::BlspI2CDriverComponentImpl* i2cDrvSnap_ptr = 0;
SnapdragonFlight::BlspGpioDriverComponentImpl* imuDRIntSnap_ptr = 0;
SnapdragonFlight::BlspGpioDriverComponentImpl* hwEnablePinSnap_ptr = 0;
SnapdragonFlight::BlspPwmDriverComponentImpl* escPwmSnap_ptr = 0;

Drv::LinuxSpiDriverComponentImpl* spiDrv_ptr = 0;
Drv::LinuxI2CDriverComponentImpl* i2cDrv_ptr = 0;
Drv::LinuxGpioDriverComponentImpl* imuDRInt_ptr = 0;
Drv::LinuxGpioDriverComponentImpl* hwEnablePin_ptr = 0;
Drv::LinuxPwmDriverComponentImpl* escPwm_ptr = 0;

SnapdragonFlight::MVCamComponentImpl* mvCam_ptr = 0;
SnapdragonFlight::StereoCamComponentImpl* stereoCam_ptr = 0;
SnapdragonFlight::HiresCamComponentImpl* hiresCam_ptr = 0;
Svc::IPCRelayComponentImpl* ipcRelay_ptr = 0;

void allocComps() {
    sockGndIf_ptr = new Svc::SocketGndIfImpl
#if FW_OBJECT_NAMES == 1
                        ("SGIF")
#endif
;

    textLogger_ptr = new Svc::ActiveTextLoggerComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("TLOG")
#endif
;

    eventLogger_ptr = new Svc::ActiveLoggerImpl
#if FW_OBJECT_NAMES == 1
                        ("ELOG")
#endif
;

    chanTlm_ptr = new Svc::TlmChanImpl
#if FW_OBJECT_NAMES == 1
                        ("TLM")
#endif
;

    cmdDisp_ptr = new Svc::CommandDispatcherImpl
#if FW_OBJECT_NAMES == 1
                        ("CMDDISP")
#endif
;

    cmdSeq_ptr = new Svc::CmdSequencerComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("CMDSEQ")
#endif
;

    prmDb_ptr = new Svc::PrmDbImpl
#if FW_OBJECT_NAMES == 1
                        ("PRM",PRM_PATH)
#else
                        (PRM_PATH)
#endif
;

    snapHealth_ptr = new SnapdragonFlight::SnapdragonHealthComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SDHEALTH")
#endif
;

    timeConvert_ptr = new Svc::TimeConvertComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("TIMECONV")
#endif
;

    mvCam_ptr = new SnapdragonFlight::MVCamComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("MVCAM")
#endif
;

    stereoCam_ptr = new SnapdragonFlight::StereoCamComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SCAM")
#endif
;

    ipcRelay_ptr = new Svc::IPCRelayComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("IPCRELAY")
#endif
;

    hiresCam_ptr = new SnapdragonFlight::HiresCamComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("HIRESCAM")
#endif
;

    rosSeq_ptr = new ROS::RosSeqComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("ROSSEQ")
#endif
;

    hlRosIface_ptr = new HLProc::HLRosIfaceComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SDROSIFACE")
#endif
;

    ackermannIface_ptr = new Gnc::AckermannIfaceComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("ACKMNIFACE")
#endif
;

    filterIface_ptr = new Gnc::FilterIfaceComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("FILTIFACE")
#endif
;

    gtIface_ptr = new Gnc::GroundTruthIfaceComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("FILTIFACE")
#endif
;

    rgDecouple_ptr = new Svc::RateGroupDecouplerComponentImpl(
#if FW_OBJECT_NAMES == 1
                        "RGDECOUPLE",
#endif
                        5) // multiply by sleep duration in run1backupCycle
;

    passiveDataPasser_ptr = new Svc::QueuedDecouplerComponentImpl(
#if FW_OBJECT_NAMES == 1
                        "IMUDATPASSER"
#endif
                                                            )
;

    imuDecouple_ptr = new Svc::ActiveDecouplerComponentImpl(
#if FW_OBJECT_NAMES == 1
                        "IMUDECOUPLE"
#endif
                                                            )
;

    actDecouple_ptr = new Svc::ActiveDecouplerComponentImpl(
#if FW_OBJECT_NAMES == 1
                        "ACTDECOUPLE"
#endif
                                                            )
;

    NATIVE_UINT_TYPE rgDevContext[Svc::PassiveRateGroupImpl::CONTEXT_SIZE] = {
        Drv::MPU9250_SCHED_CONTEXT_OPERATE, // mpu9250
    };

    rgDev_ptr = new Svc::PassiveRateGroupImpl(
#if FW_OBJECT_NAMES == 1
                            "RGDEV",
#endif
                            rgDevContext,FW_NUM_ARRAY_ELEMENTS(rgDevContext));
;

    NATIVE_UINT_TYPE rgTlmContext[Svc::PassiveRateGroupImpl::CONTEXT_SIZE] = {
        Drv::MPU9250_SCHED_CONTEXT_TLM, // mpu9250
        Gnc::ATTFILTER_SCHED_CONTEXT_TLM, // attFilter
        Gnc::SIGGEN_SCHED_CONTEXT_TLM, // sigGen
        0, // mvCam
        0, // hiresCam
        Gnc::ACTADAP_SCHED_CONTEXT_TLM, // adapter
        0, // snapHealth
        0, // cmdSeq
        0, // stereoCam
        0, // chanTlm
    };

    rgTlm_ptr = new Svc::PassiveRateGroupImpl(
#if FW_OBJECT_NAMES == 1
                            "RGTLM",
#endif
                            rgTlmContext,FW_NUM_ARRAY_ELEMENTS(rgTlmContext));
;

    NATIVE_INT_TYPE rgDcplDivs[] = {1, 1};

    rgDcplDrv_ptr = new Svc::RateGroupDriverImpl(
#if FW_OBJECT_NAMES == 1
                        "RGDCPLDRV",
#endif
                        rgDcplDivs,FW_NUM_ARRAY_ELEMENTS(rgDcplDivs));

    NATIVE_INT_TYPE rgGncDivs[] = {1, 50};

    rgGncDrv_ptr = new Svc::RateGroupDriverImpl(
#if FW_OBJECT_NAMES == 1
                        "RGDRV",
#endif
                        rgGncDivs,FW_NUM_ARRAY_ELEMENTS(rgGncDivs));

    NATIVE_UINT_TYPE rgOpContext[Svc::PassiveRateGroupImpl::CONTEXT_SIZE] = {
        0, // passiveDataPasser
        Gnc::ATTFILTER_SCHED_CONTEXT_FILT, // attFilter
        Gnc::SIGGEN_SCHED_CONTEXT_OP, // sigGen
        0, // ackermannIface
        Gnc::ACTADAP_SCHED_CONTEXT_ARM, // adapter - for arming
        0, // cmdSeq
        0, // hlRosIface
        0, // mrCtrlIface
        0, // filterIface
        0, // gtIface
    };

    rgOp_ptr = new Svc::PassiveRateGroupImpl(
#if FW_OBJECT_NAMES == 1
                            "RGOP",
#endif
                            rgOpContext,FW_NUM_ARRAY_ELEMENTS(rgOpContext));
;

    linuxTime_ptr = new Svc::LinuxTimeImpl
#if FW_OBJECT_NAMES == 1
                        ("LTIME")
#endif
;

    fatalAdapter_ptr = new Svc::AssertFatalAdapterComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("fatalAdapter")
#endif
;

    fatalHandler_ptr = new Svc::FatalHandlerComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("fatalHandler")
#endif
;

    ctrlXest_ptr = new Gnc::FrameTransformComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("CTRLXEST")
#endif
;

    imuProc_ptr = new Gnc::ImuProcComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("IMUPROC")
#endif
;

    actuatorAdapter_ptr = new Gnc::ActuatorAdapterComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("ACTADAP")
#endif
;

    sigGen_ptr = new Gnc::SigGenComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SIGGEN")
#endif
;

    attFilter_ptr = new Gnc::AttFilterComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("ATTFILTER")
#endif
;

    ackermannConverter_ptr = new Gnc::AckermannConverterComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("ACKMNCONV")
#endif
;

    mpu9250_ptr = new Drv::MPU9250ComponentImpl(
#if FW_OBJECT_NAMES == 1
                        "MPU9250",
#endif
                         false) // don't use magnetometer for now
;

    spiDrvSnap_ptr = new SnapdragonFlight::BlspSpiDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SNAPSPIDRV")
#endif
;

    i2cDrvSnap_ptr = new SnapdragonFlight::BlspI2CDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                    ("SNAPI2CDRV")
#endif
;

    imuDRIntSnap_ptr = new SnapdragonFlight::BlspGpioDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SNAPDRINT")
#endif
;

    hwEnablePinSnap_ptr = new SnapdragonFlight::BlspGpioDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SNAPHWENPIN")
#endif
;

    escPwmSnap_ptr = new SnapdragonFlight::BlspPwmDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SNAPESCPWM")
#endif
;

    spiDrv_ptr = new Drv::LinuxSpiDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SPIDRV")
#endif
;

    i2cDrv_ptr = new Drv::LinuxI2CDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                    ("I2CDRV")
#endif
;

    imuDRInt_ptr = new Drv::LinuxGpioDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("IMUDRINT")
#endif
;

    hwEnablePin_ptr = new Drv::LinuxGpioDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("HWENPIN")
#endif
;

    escPwm_ptr = new Drv::LinuxPwmDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("ESCPWM")
#endif
;

}

#if FW_OBJECT_REGISTRATION == 1

void dumparch(void) {
    simpleReg.dump();
}

#if FW_OBJECT_NAMES == 1
void dumpobj(const char* objName) {
    simpleReg.dump(objName);
}
#endif

#endif

void manualConstruct(bool internalIMUProp) {
#ifdef BUILD_SDFLIGHT // SDFLIGHT vs LINUX
#ifdef SOC_8074
    actuatorAdapter_ptr->set_outputEnable_OutputPort(0, hwEnablePinSnap_ptr->get_gpioRead_InputPort(0));
#else // SOC_8074
    // TODO(mereweth) - connect when we add enable pin
#endif
#else // LINUX
#ifdef LINUX_DEV
    // TODO(mereweth) - open devices
#endif // LINUX_DEV
#endif

    // switch based on command line options
    if (internalIMUProp) {
        attFilter_ptr->set_odometry_OutputPort(0, ctrlXest_ptr->get_odomInB_InputPort(0));
    }
    else {
        gtIface_ptr->set_Odometry_OutputPort(0, ctrlXest_ptr->get_odomInB_InputPort(0));
    }

    // imu decoupler
    rgDcplDrv_ptr->set_CycleOut_OutputPort(0, imuDecouple_ptr->get_DataIn_InputPort(0));
    imuDecouple_ptr->set_DataOut_OutputPort(0, rgDev_ptr->get_CycleIn_InputPort(0));

    // passive data passer
    imuProc_ptr->set_DownsampledImu_OutputPort(0, passiveDataPasser_ptr->get_DataIn_InputPort(0));
    passiveDataPasser_ptr->set_DataOut_OutputPort(0, attFilter_ptr->get_Imu_InputPort(0));

    // TODO(Mereweth) - all connections into passive rgs from another thread
    const U32 NUM_CMD_PORTS = Svc::CommandDispatcherImpl::NUM_CMD_PORTS;

    // NOTE(Mereweth) - decrement cmdDisp port #; increment passiveDataPasser port #; others are 0

    U32 cmdIdx = 1;
    U32 dpIdx = 1;
    cmdDisp_ptr->set_compCmdSend_OutputPort(NUM_CMD_PORTS - cmdIdx, passiveDataPasser_ptr->get_DataIn_InputPort(dpIdx));
    passiveDataPasser_ptr->set_DataOut_OutputPort(dpIdx, attFilter_ptr->get_CmdDisp_InputPort(0));
    attFilter_ptr->set_CmdReg_OutputPort(0, cmdDisp_ptr->get_compCmdReg_InputPort(NUM_CMD_PORTS - cmdIdx));
    attFilter_ptr->set_CmdStatus_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    /*cmdIdx = dpIdx = 2;
    cmdDisp_ptr->set_compCmdSend_OutputPort(NUM_CMD_PORTS - cmdIdx, passiveDataPasser_ptr->get_DataIn_InputPort(dpIdx));
    passiveDataPasser_ptr->set_DataOut_OutputPort(dpIdx, se3Ctrl_ptr->get_CmdDisp_InputPort(0));
    se3Ctrl_ptr->set_CmdReg_OutputPort(0, cmdDisp_ptr->get_compCmdReg_InputPort(NUM_CMD_PORTS - cmdIdx));
    se3Ctrl_ptr->set_CmdStatus_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    cmdIdx = dpIdx = 3;
    cmdDisp_ptr->set_compCmdSend_OutputPort(NUM_CMD_PORTS - cmdIdx, passiveDataPasser_ptr->get_DataIn_InputPort(dpIdx));
    passiveDataPasser_ptr->set_DataOut_OutputPort(dpIdx, mixer_ptr->get_CmdDisp_InputPort(0));
    mixer_ptr->set_CmdReg_OutputPort(0, cmdDisp_ptr->get_compCmdReg_InputPort(NUM_CMD_PORTS - cmdIdx));
    mixer_ptr->set_CmdStatus_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));*/

    cmdIdx = dpIdx = 4;
    cmdDisp_ptr->set_compCmdSend_OutputPort(NUM_CMD_PORTS - cmdIdx, passiveDataPasser_ptr->get_DataIn_InputPort(dpIdx));
    passiveDataPasser_ptr->set_DataOut_OutputPort(dpIdx, actuatorAdapter_ptr->get_CmdDisp_InputPort(0));
    actuatorAdapter_ptr->set_CmdReg_OutputPort(0, cmdDisp_ptr->get_compCmdReg_InputPort(NUM_CMD_PORTS - cmdIdx));
    actuatorAdapter_ptr->set_CmdStatus_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    cmdIdx = dpIdx = 5;
    cmdDisp_ptr->set_compCmdSend_OutputPort(NUM_CMD_PORTS - cmdIdx, passiveDataPasser_ptr->get_DataIn_InputPort(dpIdx));
    passiveDataPasser_ptr->set_DataOut_OutputPort(dpIdx, sigGen_ptr->get_CmdDisp_InputPort(0));
    sigGen_ptr->set_CmdReg_OutputPort(0, cmdDisp_ptr->get_compCmdReg_InputPort(NUM_CMD_PORTS - cmdIdx));
    sigGen_ptr->set_CmdStatus_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    cmdIdx = dpIdx = 6;
    cmdDisp_ptr->set_compCmdSend_OutputPort(NUM_CMD_PORTS - cmdIdx, passiveDataPasser_ptr->get_DataIn_InputPort(dpIdx));
    passiveDataPasser_ptr->set_DataOut_OutputPort(dpIdx, ctrlXest_ptr->get_CmdDisp_InputPort(0));
    ctrlXest_ptr->set_CmdReg_OutputPort(0, cmdDisp_ptr->get_compCmdReg_InputPort(NUM_CMD_PORTS - cmdIdx));
    ctrlXest_ptr->set_CmdStatus_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    cmdIdx = dpIdx = 7;
    cmdDisp_ptr->set_compCmdSend_OutputPort(NUM_CMD_PORTS - cmdIdx, passiveDataPasser_ptr->get_DataIn_InputPort(dpIdx));
    passiveDataPasser_ptr->set_DataOut_OutputPort(dpIdx, imuProc_ptr->get_CmdDisp_InputPort(0));
    imuProc_ptr->set_CmdReg_OutputPort(0, cmdDisp_ptr->get_compCmdReg_InputPort(NUM_CMD_PORTS - cmdIdx));
    imuProc_ptr->set_CmdStatus_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    // actuator decoupler
    ackermannConverter_ptr->set_actuators_OutputPort(0, actDecouple_ptr->get_DataIn_InputPort(0));
    actDecouple_ptr->set_DataOut_OutputPort(0, actuatorAdapter_ptr->get_motor_InputPort(0));

    sigGen_ptr->set_motor_OutputPort(0, actDecouple_ptr->get_DataIn_InputPort(1));
    actDecouple_ptr->set_DataOut_OutputPort(1, actuatorAdapter_ptr->get_motor_InputPort(0));

    /*mrCtrlIface_ptr->set_boolStamped_OutputPort(0, actDecouple_ptr->get_DataIn_InputPort(2));
      actDecouple_ptr->set_DataOut_OutputPort(2, actuatorAdapter_ptr->get_flySafe_InputPort(0));*/

    hlRosIface_ptr->set_ActuatorsData_OutputPort(0, actDecouple_ptr->get_DataIn_InputPort(3));
    actDecouple_ptr->set_DataOut_OutputPort(3, actuatorAdapter_ptr->get_motor_InputPort(1));

    rgOp_ptr->set_RateGroupMemberOut_OutputPort(4, actDecouple_ptr->get_DataIn_InputPort(4));
    actDecouple_ptr->set_DataOut_OutputPort(4, actuatorAdapter_ptr->get_sched_InputPort(0));

    rgTlm_ptr->set_RateGroupMemberOut_OutputPort(5, actDecouple_ptr->get_DataIn_InputPort(5));
    actDecouple_ptr->set_DataOut_OutputPort(5, actuatorAdapter_ptr->get_sched_InputPort(0));

    hiresCam_ptr->set_CmdStatus_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(0));
    // doesn't matter which compCmdStat port # for cmdDisp
    ipcRelay_ptr->set_proc2Out_OutputPort(0, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    hiresCam_ptr->set_Tlm_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(1));
    // doesn't matter which TlmRecv port # for pktTlm
    ipcRelay_ptr->set_proc2Out_OutputPort(1, chanTlm_ptr->get_TlmRecv_InputPort(0));

    hiresCam_ptr->set_Log_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(2));
    // doesn't matter which LogRecv port # for eventLogger
    ipcRelay_ptr->set_proc2Out_OutputPort(2, eventLogger_ptr->get_LogRecv_InputPort(0));

    hiresCam_ptr->set_LogText_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(3));
    // doesn't matter which TextLogger port # for textLogger
    ipcRelay_ptr->set_proc2Out_OutputPort(3, textLogger_ptr->get_TextLogger_InputPort(0));

    /*hiresCam_ptr->set_ProcSend_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(4));
      ipcRelay_ptr->set_proc2Out_OutputPort(4, buffAccumHiresCamProc_ptr->get_bufferSendInFill_InputPort(0));*/

    /*hiresCam_ptr->set_UnprocSend_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(5));
    ipcRelay_ptr->set_proc2Out_OutputPort(5, buffAccumHiresCamUnproc_ptr->get_bufferSendInFill_InputPort(0));*/


    stereoCam_ptr->set_CmdStatus_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(6));
    // doesn't matter which compCmdStat port # for cmdDisp
    ipcRelay_ptr->set_proc2Out_OutputPort(6, cmdDisp_ptr->get_compCmdStat_InputPort(0));

    stereoCam_ptr->set_Tlm_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(7));
    // doesn't matter which TlmRecv port # for pktTlm
    ipcRelay_ptr->set_proc2Out_OutputPort(7, chanTlm_ptr->get_TlmRecv_InputPort(0));

    stereoCam_ptr->set_Log_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(8));
    // doesn't matter which LogRecv port # for eventLogger
    ipcRelay_ptr->set_proc2Out_OutputPort(8, eventLogger_ptr->get_LogRecv_InputPort(0));

    stereoCam_ptr->set_LogText_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(9));
    // doesn't matter which TextLogger port # for textLogger
    ipcRelay_ptr->set_proc2Out_OutputPort(9, textLogger_ptr->get_TextLogger_InputPort(0));

    /*stereoCam_ptr->set_ProcSend_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(10));
      ipcRelay_ptr->set_proc2Out_OutputPort(10, buffAccumstereoCamProc_ptr->get_bufferSendInFill_InputPort(0));*/

    /*stereoCam_ptr->set_UnprocSend_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(11));
    ipcRelay_ptr->set_proc2Out_OutputPort(11, buffAccumStereoCamUnproc_ptr->get_bufferSendInFill_InputPort(0));*/

    stereoCam_ptr->set_GncBufferSend_OutputPort(0, ipcRelay_ptr->get_proc1In_InputPort(12));
    ipcRelay_ptr->set_proc2Out_OutputPort(12, hlRosIface_ptr->get_ImageRecv_InputPort(1));
}

void constructApp(unsigned int port_number,
                  char* hostname,
                  unsigned int boot_count,
                  bool startSocketNow,
                  bool &isHiresChild, bool &isStereoChild,
                  bool internalIMUProp) {
    allocComps();

#if FW_PORT_TRACING
    Fw::PortBase::setTrace(false);
#endif

#if FW_ENABLE_TEXT_LOGGING
    // Queue needs to be large enough to process many text messages at once
    textLogger_ptr->init(500);
#endif

    eventLogger_ptr->init(500,0);

    chanTlm_ptr->init(60,0);

    cmdDisp_ptr->init(60,0);

    cmdSeq_ptr->init(60,0);
    cmdSeq_ptr->allocateBuffer(0,seqMallocator,100*1024);

    prmDb_ptr->init(60,0);
    snapHealth_ptr->init(60,0);
    snapHealth_ptr->setBootCount(boot_count);
    snapHealth_ptr->setInitPowerState(SnapdragonFlight::SH_SAVER_RAPID);
    sockGndIf_ptr->init(0);
    timeConvert_ptr->init();

    hlRosIface_ptr->init(0);
    ackermannIface_ptr->init(0);
    filterIface_ptr->init(0);
    gtIface_ptr->init(0);
    rosSeq_ptr->init(0);

    // Initialize rate group driver
    rgGncDrv_ptr->init();
    rgDcplDrv_ptr->init();

    // Initialize the rate groups
    rgDecouple_ptr->init(10, 0); // designed to drop if full
    passiveDataPasser_ptr->init(10000, 1000); // big entries - all passive components in rgOp use this
    imuDecouple_ptr->init(1000, 20); // just need to serialize cycle port
    actDecouple_ptr->init(1000, 500); // big message queue entry, few entries
    rgOp_ptr->init(0);
    rgTlm_ptr->init(2);
    rgDev_ptr->init(3);

    // Initialize the GNC components
    ctrlXest_ptr->init(0);
    imuProc_ptr->init(0);
    actuatorAdapter_ptr->init(0);
    sigGen_ptr->init(0);
    attFilter_ptr->init(0);
    ackermannConverter_ptr->init(0);
    mpu9250_ptr->init(0);

    //mpu9250_ptr->setOutputMode(Drv::MPU9250ComponentImpl::OUTPUT_100HZ_DLPF_ACCEL_41HZ_GYRO_41HZ);
    mpu9250_ptr->setOutputMode(Drv::MPU9250ComponentImpl::OUTPUT_50HZ_DLPF_ACCEL_20HZ_GYRO_20HZ);

    spiDrvSnap_ptr->init(0);
    i2cDrvSnap_ptr->init(0);
    hwEnablePinSnap_ptr->init(1);
    imuDRIntSnap_ptr->init(0);
    escPwmSnap_ptr->init(0);

    spiDrv_ptr->init(0);
    i2cDrv_ptr->init(0);
    hwEnablePin_ptr->init(1);
    imuDRInt_ptr->init(0);
    escPwm_ptr->init(0);

    linuxTime_ptr->init(0);

    fatalAdapter_ptr->init(0);
    fatalHandler_ptr->init(0);

    mvCam_ptr->init(60, 0);
    ipcRelay_ptr->init(60, IPC_RELAY_BUFFER_SIZE, 0);
    hiresCam_ptr->init(60, 0);
    stereoCam_ptr->init(60, 0);

    // Connect rate groups to rate group driver
    constructCARREFArchitecture();

    manualConstruct(internalIMUProp);

    /* Register commands */
    fatalHandler_ptr->regCommands();

    sockGndIf_ptr->regCommands();
    cmdSeq_ptr->regCommands();
    cmdDisp_ptr->regCommands();
    eventLogger_ptr->regCommands();
    prmDb_ptr->regCommands();
    snapHealth_ptr->regCommands();

    ctrlXest_ptr->regCommands();
    imuProc_ptr->regCommands();
    attFilter_ptr->regCommands();
    //ackermannConverter_ptr->regCommands();
    actuatorAdapter_ptr->regCommands();
    sigGen_ptr->regCommands();

    mvCam_ptr->regCommands();
    stereoCam_ptr->regCommands();
    hiresCam_ptr->regCommands();

    prmDb_ptr->readParamFile();

    ctrlXest_ptr->loadParameters();
    imuProc_ptr->loadParameters();
    attFilter_ptr->loadParameters();
    //ackermannConverter_ptr->loadParameters();
    actuatorAdapter_ptr->loadParameters();
    sigGen_ptr->loadParameters();

    mvCam_ptr->loadParameters();
    stereoCam_ptr->loadParameters();

    // set static time offset
    // TODO(mereweth) - change this if we start timing out on correspondences
    // TODO(mereweth) - check to make sure we are not using ROS sim time
    {
        Fw::InputTimePairPort* port = timeConvert_ptr->get_ClockTimes_InputPort(0);
        Fw::Time t1(TB_ROS_TIME, 0, 0, 0);
        Fw::Time t2(TB_WORKSTATION_TIME, 0, 0, 0);
        port->invoke(t1, t2);
    }

    char logFileName[256];
    snprintf(logFileName, sizeof(logFileName), "/eng/TextLog_%u.txt", boot_count % 10);
    textLogger_ptr->set_log_file(logFileName,100*1024, 0);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-local-typedefs"
    COMPILE_TIME_ASSERT(200 <= SnapdragonFlight::MVCAM_IMG_LP_BUFFER_POOL_SIZE,
                        NCAM_IMG_LP_BUFFER_ENOUGH);
    COMPILE_TIME_ASSERT(10 <= SnapdragonFlight::HIRESCAM_MAX_NUM_BUFFERS,
                        RCAM_BUFFER_ENOUGH);
    COMPILE_TIME_ASSERT(200 <= SnapdragonFlight::SCAM_IMG_LP_BUFFER_POOL_SIZE,
                        SCAM_IMG_LP_BUFFER_ENOUGH);
    #pragma GCC diagnostic pop

    mvCam_ptr->allocateBuffers(0, buffMallocator,
                               SnapdragonFlight::MVCAM_IMG_HP_BUFFER_POOL_SIZE,
                               200);
    hiresCam_ptr->allocateBuffers(0, hiresMallocator, 10);
    stereoCam_ptr->allocateBuffers(0, hiresMallocator,
                                   SnapdragonFlight::SCAM_IMG_HP_BUFFER_POOL_SIZE,
                                   200);

    // Open devices

#ifdef BUILD_SDFLIGHT // SDFLIGHT vs LINUX
#ifdef SOC_8074
    // /dev/spi-1 on QuRT; connected to MPU9250
    spiDrvSnap_ptr->open(1, SnapdragonFlight::SPI_FREQUENCY_1MHZ);
    imuDRIntSnap_ptr->open(65, SnapdragonFlight::BlspGpioDriverComponentImpl::GPIO_INT);

    // J13-3, 5V level
    hwEnablePinSnap_ptr->open(28, SnapdragonFlight::BlspGpioDriverComponentImpl::GPIO_IN);

    // J13 is already at 5V, so use for 2 pwm connectors
    NATIVE_UINT_TYPE pwmPins[2] = {29, 30};
    F32 duty[2] = {0.15, 0.15};
    // /dev/pwm-1 on QuRT
    escPwmSnap_ptr->open(1, pwmPins, 2, duty, 10 * 1000);
#else
    // /dev/spi-10 on 820; connected to MPU9250
    spiDrvSnap_ptr->open(10, SnapdragonFlight::SPI_FREQUENCY_1MHZ);
    imuDRIntSnap_ptr->open(78, SnapdragonFlight::BlspGpioDriverComponentImpl::GPIO_INT);

    // J12 is already at 5V, so use for 2 pwm connectors
    NATIVE_UINT_TYPE pwmPins[2] = {83, 84};
    F32 duty[2] = {0.15, 0.15};
    // /dev/pwm-1 on QuRT
    //escPwmSnap_ptr->open(1, pwmPins, 2, duty, 10 * 1000);
#endif // SOC

#else // LINUX
#ifdef LINUX_DEV
    // TODO(mereweth) - open devices
#endif // LINUX_DEV
#endif // SDFLIGHT vs LINUX

    isHiresChild = false;
    isStereoChild = false;
// fork works just fine on Darwin; just haven't got POSIX mq for IPC ports
#if defined TGT_OS_TYPE_LINUX
    int childPID = hiresCam_ptr->spawnChild();
    if (childPID == 0) { // we are in the child process
        isHiresChild = true;
#endif

        hiresCam_ptr->start(0,40,5*1000*1024, CORE_CAM);

#if defined TGT_OS_TYPE_LINUX
        return; // don't start any other threads in the child
    }
#endif

#if defined TGT_OS_TYPE_LINUX
    childPID = stereoCam_ptr->spawnChild();
    if (childPID == 0) { // we are in the child process
        isStereoChild = true;
#endif

        stereoCam_ptr->start(0,40,5*1000*1024, CORE_CAM);

#if defined TGT_OS_TYPE_LINUX
        return; // don't start any other threads in the child
    }
#endif

    // NOTE(mereweth) - putting this near the top in case we want to fork in ipcRelay instead
    ipcRelay_ptr->start(0,30,20*1024);

    // Active component startup
    imuDecouple_ptr->start(0, 91, 20*1024, CORE_RPC);
    // NOTE(mereweth) - GNC att & pos loops run in this thread:
    rgDecouple_ptr->start(0, 90, 20*1024, CORE_RPC);
    // NOTE(mereweth) - ESC I2C calls happen in this thread:
    actDecouple_ptr->start(0, 92, 20*1024, CORE_RPC);

    cmdDisp_ptr->start(0,60,20*1024);
    // start sequencer
    cmdSeq_ptr->start(0,50,20*1024);
    // start telemetry
    eventLogger_ptr->start(0,50,20*1024);
    chanTlm_ptr->start(0,60,20*1024);
    prmDb_ptr->start(0,50,20*1024);
    textLogger_ptr->start(0,30,20*1024);

    snapHealth_ptr->start(0,40,20*1024);

    mvCam_ptr->start(0, 80, 5*1000*1024, CORE_CAM);

    // Initialize socket server
    if (port_number && hostname) {
        if (startSocketNow) {
            sockGndIf_ptr->startSocketTask(40, 20*1024, port_number, hostname, Svc::SocketGndIfImpl::SEND_TCP);
        } else {
            sockGndIf_ptr->setSocketTaskProperties(40, 20*1024, port_number, hostname, Svc::SocketGndIfImpl::SEND_TCP);
        }
    }

#if FW_OBJECT_REGISTRATION == 1
    //simpleReg.dump();
#endif

}

void run1testCycle(void) {
    // call interrupt to emulate a clock
    Svc::InputCyclePort* port = rgDcplDrv_ptr->get_CycleIn_InputPort(0);
    Svc::TimerVal cycleStart;
    cycleStart.take();
    port->invoke(cycleStart);
    Os::Task::delay(1);
}

void run1backupCycle(void) {
    // call interrupt to emulate a clock
    Svc::InputCyclePort* port = rgDecouple_ptr->get_BackupCycleIn_InputPort(0);
    Svc::TimerVal cycleStart;
    cycleStart.take();
    port->invoke(cycleStart);
    Os::Task::delay(50);
}

void exitTasks(bool isHiresChild, bool isStereoChild) {
#if defined TGT_OS_TYPE_LINUX
    if (isHiresChild) {
#endif
        hiresCam_ptr->exit();
        hiresCam_ptr->deallocateBuffers(hiresMallocator);
        hiresCam_ptr->join(NULL);
        DEBUG_PRINT("after hires exit\n");
#if defined TGT_OS_TYPE_LINUX
        return;
    }
#endif

#if defined TGT_OS_TYPE_LINUX
    if (isStereoChild) {
#endif
        stereoCam_ptr->exit();
        stereoCam_ptr->deallocateBuffers(hiresMallocator);
        stereoCam_ptr->join(NULL);
        DEBUG_PRINT("after stereo exit\n");
#if defined TGT_OS_TYPE_LINUX
        return;
    }
#endif

    ipcRelay_ptr->exit();
    DEBUG_PRINT("after IPCRelay exit\n");

    mvCam_ptr->exit();
    mvCam_ptr->deallocateBuffers(buffMallocator);
    mvCam_ptr->join(NULL);

#ifdef BUILD_SDFLIGHT // SDFLIGHT vs LINUX
    imuDRIntSnap_ptr->exitThread();
    imuDRIntSnap_ptr->joinThread(NULL);
#else
    imuDRInt_ptr->exitThread();
    imuDRInt_ptr->joinThread(NULL);
#endif
    DEBUG_PRINT("after dataready intr thread join\n");

    rgDecouple_ptr->exit();
    imuDecouple_ptr->exit();
    actDecouple_ptr->exit();

    snapHealth_ptr->exit();
    cmdDisp_ptr->exit();
    eventLogger_ptr->exit();
    chanTlm_ptr->exit();
    prmDb_ptr->exit();
    textLogger_ptr->exit();
    cmdSeq_ptr->exit();
}


void print_usage() {
    (void) printf("Usage: ./CARREF [options]\n"
                  "-p\tport_number\n"
                  "-a\thostname/IP address\n"
                  "-i\tUse odometry from internal IMU propagation\n"
                  "-b\tBoot count\n"
                  "-s\tStart socket immediately\n");
}


#include <signal.h>
#include <stdio.h>

extern "C" {
    int main(int argc, char* argv[]);
};

volatile sig_atomic_t terminate = 0;

static void sighandler(int signum) {
    terminate = 1;
    ros::shutdown();
}

void dummy() {
    while(!terminate) {
        Os::Task::delay(1000);
    }
}

int main(int argc, char* argv[]) {
    U32 port_number = 0;
    I32 option = 0;
    char *hostname = NULL;
    bool startSocketNow = false;
    U32 boot_count = 0;
    bool internalIMUProp = false;
    bool isHiresChild = false;
    bool isStereoChild = false;

    // Removes ROS cmdline args as a side-effect
    ros::init(argc,argv,"CARREF", ros::init_options::NoSigintHandler);

    while ((option = getopt(argc, argv, "hisp:a:b:u:z:")) != -1){
        switch(option) {
            case 'h':
                print_usage();
                return 0;
                break;
            case 'b':
                boot_count = atoi(optarg);
                break;
            case 'p':
                port_number = atoi(optarg);
                break;
            case 'u':
                break;
            case 'z':
                break;
            case 'a':
                hostname = optarg;
                break;
            case 's':
                startSocketNow = true;
                break;
            case 'i':
                internalIMUProp = true;
                break;
            case '?':
                return 1;
            default:
                print_usage();
                return 1;
        }
    }

    signal(SIGINT,sighandler);
    signal(SIGTERM,sighandler);
    signal(SIGKILL,sighandler);
    signal(SIGPIPE, SIG_IGN);

    (void) printf("Hit Ctrl-C to quit\n");

    constructApp(port_number,
                 hostname, boot_count,
                 startSocketNow,
                 isHiresChild, isStereoChild,
                 internalIMUProp);
    //dumparch();

    if (!isHiresChild && !isStereoChild) {
        ros::start();

        hlRosIface_ptr->startIntTask(30, 5*1000*1024);
        ackermannIface_ptr->startIntTask(30, 5*1000*1024);
        filterIface_ptr->startIntTask(30, 5*1000*1024);
        gtIface_ptr->startIntTask(30, 5*1000*1024);
        rosSeq_ptr->startIntTask(30, 5*1000*1024);

        hlRosIface_ptr->startPub();
        ackermannIface_ptr->startPub();
        filterIface_ptr->startPub();
        gtIface_ptr->startPub();
        rosSeq_ptr->startPub();

        ros::console::shutdown();

        while (!mpu9250_ptr->isReady()) {
            Svc::InputCyclePort* port = rgDev_ptr->get_CycleIn_InputPort(0);
            Svc::TimerVal cycleStart;
            cycleStart.take();
            port->invoke(cycleStart);
            Os::Task::delay(10);
        }
#ifdef BUILD_SDFLIGHT // SDFLIGHT vs LINUX
        imuDRIntSnap_ptr->startIntTask(99);
#else

#ifdef LINUX_DEV
        imuDRInt_ptr->startIntTask(99);
#endif // LINUX_DEV
#endif
        rgDecouple_ptr->setEnabled(true);
    } //!isHiresChild && !isStereoChild

    int backupCycle = 0;

    while (!terminate) {
        if (isHiresChild) {
            DEBUG_PRINT("Hires Child Cycle %d\n",backupCycle);
        }
        else if (isStereoChild) {
            DEBUG_PRINT("Stereo Child Cycle %d\n",backupCycle);
        }
        else {
            //DEBUG_PRINT("Parent Cycle %d\n",cycle);
        }

        backupCycle++;
        if (!isHiresChild && !isStereoChild) {
            run1backupCycle();
#if !defined(LINUX_DEV) and !defined(BUILD_SDFLIGHT)
            run1testCycle();
#endif // LINUX_DEV
        }
        else {
            Os::Task::delay(1000);
        }
    }

    if (!isHiresChild && !isStereoChild) {
        rgDecouple_ptr->setEnabled(false);

        DEBUG_PRINT("Stopping tasks\n");
        ros::shutdown();
    } // !isHiresChild && !isStereoChild

    exitTasks(isHiresChild, isStereoChild);

    // Give time for threads to exit
#if defined TGT_OS_TYPE_LINUX
    if (!isHiresChild && !isStereoChild) {
        (void) printf("Waiting for child...\n");
        wait(NULL);
#endif
        (void) printf("Waiting for threads...\n");
        Os::Task::delay(1000);

        (void) printf("Exiting...\n");

#if defined TGT_OS_TYPE_LINUX
    }
#endif
    return 0;
}
