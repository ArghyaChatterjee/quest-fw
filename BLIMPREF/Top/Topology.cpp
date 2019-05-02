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
#include <BLIMPREF/Top/TargetInit.hpp>
#include <Os/Task.hpp>
#include <Os/Log.hpp>
#include <Fw/Types/MallocAllocator.hpp>

#if defined TGT_OS_TYPE_LINUX || TGT_OS_TYPE_DARWIN
#include <getopt.h>
#include <stdlib.h>
#include <ctype.h>
#endif

#include <unistd.h>

#define DEBUG_PRINT(x,...) printf(x,##__VA_ARGS__); fflush(stdout)
//#define DEBUG_PRINT(x,...)

#define DECOUPLE_ACTUATORS

#define PRM_PATH "/eng/BLIMPREFPrmDb.dat"

// Registry
#if FW_OBJECT_REGISTRATION == 1
static Fw::SimpleObjRegistry simpleReg;
#endif

Fw::MallocAllocator seqMallocator;

// Component instance pointers

Svc::CommandDispatcherImpl* cmdDisp_ptr = 0;
Svc::CmdSequencerComponentImpl* cmdSeq_ptr = 0;
Svc::ActiveTextLoggerComponentImpl* textLogger_ptr = 0;
Svc::ActiveLoggerImpl* eventLogger_ptr = 0;
Svc::TlmChanImpl* chanTlm_ptr = 0;
Svc::PrmDbImpl* prmDb_ptr = 0;
Svc::SocketGndIfImpl* sockGndIf_ptr = 0;
SnapdragonFlight::SnapdragonHealthComponentImpl* snapHealth_ptr = 0;

HLProc::HLRosIfaceComponentImpl* hlRosIface_ptr = 0;
Gnc::MultirotorCtrlIfaceComponentImpl* mrCtrlIface_ptr = 0;
Gnc::FilterIfaceComponentImpl* filterIface_ptr = 0;
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
Gnc::FixedAxisSe3AdapterComponentImpl* axSe3Adap_ptr = 0;
Gnc::Se3CtrlComponentImpl* se3Ctrl_ptr = 0;
Gnc::WrenchMixerComponentImpl* mixer_ptr = 0;
Gnc::ActuatorAdapterComponentImpl* actuatorAdapter_ptr = 0;
Gnc::SigGenComponentImpl* sigGen_ptr = 0;
Gnc::AttFilterComponentImpl* attFilter_ptr = 0;
Drv::MPU9250ComponentImpl* mpu9250_ptr = 0;

SnapdragonFlight::BlspSpiDriverComponentImpl* spiDrvSnap_ptr = 0;
SnapdragonFlight::BlspI2CDriverComponentImpl* i2cDrvSnap_ptr = 0;
SnapdragonFlight::BlspGpioDriverComponentImpl* imuDRIntSnap_ptr = 0;
SnapdragonFlight::BlspGpioDriverComponentImpl* hwEnablePinSnap_ptr = 0;

Drv::LinuxSpiDriverComponentImpl* spiDrv_ptr = 0;
Drv::LinuxI2CDriverComponentImpl* i2cDrv_ptr = 0;
Drv::LinuxGpioDriverComponentImpl* imuDRInt_ptr = 0;
Drv::LinuxGpioDriverComponentImpl* hwEnablePin_ptr = 0;
Drv::LinuxPwmDriverComponentImpl* escPwm_ptr = 0;

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

    mrCtrlIface_ptr = new Gnc::MultirotorCtrlIfaceComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("MRCTRLIFACE")
#endif
;

    filterIface_ptr = new Gnc::FilterIfaceComponentImpl
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
        Gnc::SE3CTRL_SCHED_CONTEXT_TLM, // se3Ctrl
        0, // mixer
        Gnc::ACTADAP_SCHED_CONTEXT_TLM, // adapter
        0, // snapHealth
        0, // chanTlm
    };

    rgTlm_ptr = new Svc::PassiveRateGroupImpl(
#if FW_OBJECT_NAMES == 1
                            "RGTLM",
#endif
                            rgTlmContext,FW_NUM_ARRAY_ELEMENTS(rgTlmContext));
;

    NATIVE_INT_TYPE rgDcplDivs[] = {1, 10};

    rgDcplDrv_ptr = new Svc::RateGroupDriverImpl(
#if FW_OBJECT_NAMES == 1
                        "RGDCPLDRV",
#endif
                        rgDcplDivs,FW_NUM_ARRAY_ELEMENTS(rgDcplDivs));
 
    NATIVE_INT_TYPE rgGncDivs[] = {1, 100};

    rgGncDrv_ptr = new Svc::RateGroupDriverImpl(
#if FW_OBJECT_NAMES == 1
                        "RGDRV",
#endif
                        rgGncDivs,FW_NUM_ARRAY_ELEMENTS(rgGncDivs));

    NATIVE_UINT_TYPE rgOpContext[Svc::PassiveRateGroupImpl::CONTEXT_SIZE] = {
        0, // passiveDataPasser
        Gnc::ATTFILTER_SCHED_CONTEXT_FILT, // attFilter
        Gnc::SIGGEN_SCHED_CONTEXT_OP, // sigGen
        Gnc::SE3CTRL_SCHED_CONTEXT_CTRL, // se3Ctrl
        Gnc::ACTADAP_SCHED_CONTEXT_ARM, // adapter - for arming
        0, // cmdSeq
	0, // hlRosIface
	0, // mrCtrlIface
	0, // filterIface
        0, // logQueue
        0, // kraitRouter
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
 
    axSe3Adap_ptr = new Gnc::FixedAxisSe3AdapterComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("AXSE3ADAP")
#endif
;
 
    se3Ctrl_ptr = new Gnc::Se3CtrlComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SE3CTRL")
#endif
;

    mixer_ptr = new Gnc::WrenchMixerComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("MIXER")
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
                        ("SNAPIMUDRINT")
#endif
;

    hwEnablePinSnap_ptr = new SnapdragonFlight::BlspGpioDriverComponentImpl
#if FW_OBJECT_NAMES == 1
                        ("SNAPHWENPIN")
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

void manualConstruct(void) {  
    // imu decoupler
    rgDcplDrv_ptr->set_CycleOut_OutputPort(0, imuDecouple_ptr->get_DataIn_InputPort(0));
    imuDecouple_ptr->set_DataOut_OutputPort(0, rgDev_ptr->get_CycleIn_InputPort(0));

    // passive data passer
    imuProc_ptr->set_DownsampledImu_OutputPort(0, passiveDataPasser_ptr->get_DataIn_InputPort(0));
    passiveDataPasser_ptr->set_DataOut_OutputPort(0, attFilter_ptr->get_Imu_InputPort(0));

    // TODO(Mereweth) - all connections into passive rgs
    
    // actuator decoupler
    mixer_ptr->set_motor_OutputPort(0, actDecouple_ptr->get_DataIn_InputPort(0));
    actDecouple_ptr->set_DataOut_OutputPort(0, actuatorAdapter_ptr->get_motor_InputPort(0));
        
    sigGen_ptr->set_motor_OutputPort(0, actDecouple_ptr->get_DataIn_InputPort(1));
    actDecouple_ptr->set_DataOut_OutputPort(1, actuatorAdapter_ptr->get_motor_InputPort(0));

    mrCtrlIface_ptr->set_boolStamped_OutputPort(9, actDecouple_ptr->get_DataIn_InputPort(2));
    actDecouple_ptr->set_DataOut_OutputPort(2, actuatorAdapter_ptr->get_flySafe_InputPort(0));
    
    hlRosIface_ptr->set_ActuatorsData_OutputPort(0, actDecouple_ptr->get_DataIn_InputPort(3));
    actDecouple_ptr->set_DataOut_OutputPort(3, actuatorAdapter_ptr->get_motor_InputPort(1));
    
    rgOp_ptr->set_RateGroupMemberOut_OutputPort(4, actDecouple_ptr->get_DataIn_InputPort(4));
    actDecouple_ptr->set_DataOut_OutputPort(4, actuatorAdapter_ptr->get_sched_InputPort(0));
}

void constructApp(unsigned int port_number,
		  char* hostname,
                  unsigned int boot_count,
		  bool startSocketNow) {
    allocComps();

    localTargetInit();

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
    snapHealth_ptr->setInitPowerState(SnapdragonFlight::SH_SAVER_DYNAMIC);
    sockGndIf_ptr->init(0);

    hlRosIface_ptr->init(0);
    mrCtrlIface_ptr->init(0);
    filterIface_ptr->init(0);
    rosSeq_ptr->init(0);
    
    // Initialize rate group driver
    rgGncDrv_ptr->init();
    rgDcplDrv_ptr->init();

    // Initialize the rate groups
    rgDecouple_ptr->init(10, 0); // designed to drop if full
    passiveDataPasser_ptr->init(200, 1000); // big entries - all passive components in rgOp use this
    imuDecouple_ptr->init(100, 20); // just need to serialize cycle port
    actDecouple_ptr->init(100, 500); // big message queue entry, few entries
    rgOp_ptr->init(0);
    rgTlm_ptr->init(2);
    rgDev_ptr->init(3);

    // Initialize the GNC components
    ctrlXest_ptr->init(0);
    imuProc_ptr->init(0);
    axSe3Adap_ptr->init(0);
    se3Ctrl_ptr->init(0);
    mixer_ptr->init(0);
    actuatorAdapter_ptr->init(0);
    sigGen_ptr->init(0);
    attFilter_ptr->init(0);
    mpu9250_ptr->init(0);

    spiDrvSnap_ptr->init(0);
    i2cDrvSnap_ptr->init(0);
    hwEnablePinSnap_ptr->init(1);
    imuDRIntSnap_ptr->init(0);

    spiDrv_ptr->init(0);
    i2cDrv_ptr->init(0);
    hwEnablePin_ptr->init(1);
    imuDRInt_ptr->init(0);
    escPwm_ptr->init(0);

    linuxTime_ptr->init(0);

    fatalAdapter_ptr->init(0);
    fatalHandler_ptr->init(0);

    // Connect rate groups to rate group driver
    constructBLIMPREFArchitecture();

    manualConstruct();

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
    se3Ctrl_ptr->regCommands();
    attFilter_ptr->regCommands();
    mixer_ptr->regCommands();
    actuatorAdapter_ptr->regCommands();
    sigGen_ptr->regCommands();
    
    prmDb_ptr->readParamFile();
    
    char logFileName[256];
    snprintf(logFileName, sizeof(logFileName), "/eng/TextLog_%u.txt", boot_count % 10);
    textLogger_ptr->set_log_file(logFileName,100*1024, 0);

    // Open devices

#ifdef BUILD_SDFLIGHT // SDFLIGHT vs LINUX
#ifdef SOC_8074
    // /dev/spi-1 on QuRT; connected to MPU9250
    spiDrvSnap_ptr->open(1, SnapdragonFlight::SPI_FREQUENCY_1MHZ);
    imuDRIntSnap_ptr->open(65, SnapdragonFlight::BlspGpioDriverComponentImpl::GPIO_INT);
    
    // J13-3, 5V level
    hwEnablePinSnap_ptr->open(28, SnapdragonFlight::BlspGpioDriverComponentImpl::GPIO_IN);

    // J15, BLSP9
    i2cDrvSnap_ptr->open(9, SnapdragonFlight::I2C_FREQUENCY_400KHZ);
#else 
    // /dev/spi-10 on 820; connected to MPU9250
    spiDrvSnap_ptr->open(10, SnapdragonFlight::SPI_FREQUENCY_1MHZ);
    imuDRIntSnap_ptr->open(78, SnapdragonFlight::BlspGpioDriverComponentImpl::GPIO_INT);
    
    // TODO(mereweth) - hardware enable pin

    // TODO(mereweth) - I2C port
#endif // SOC

#else // LINUX

    // TODO(mereweth) - open devices
#endif // SDFLIGHT vs LINUX

    // Active component startup
    imuDecouple_ptr->start(0, 91, 20*1024);
    // NOTE(mereweth) - GNC att & pos loops run in this thread:
    rgDecouple_ptr->start(0, 90, 20*1024);
    // NOTE(mereweth) - ESC I2C calls happen in this thread:
    actDecouple_ptr->start(0, 89, 20*1024);

    cmdDisp_ptr->start(0,60,20*1024);
    // start sequencer
    cmdSeq_ptr->start(0,50,20*1024);
    // start telemetry
    eventLogger_ptr->start(0,50,20*1024);
    chanTlm_ptr->start(0,60,20*1024);
    prmDb_ptr->start(0,50,20*1024);
    textLogger_ptr->start(0,30,20*1024);

    snapHealth_ptr->start(0,40,20*1024);
#ifdef BUILD_SDFLIGHT // SDFLIGHT vs LINUX
    imuDRIntSnap_ptr->startIntTask(99);
#else
    imuDRInt_ptr->startIntTask(99);
#endif

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

void run1backupCycle(void) {
    // call interrupt to emulate a clock
    Svc::InputCyclePort* port = rgDecouple_ptr->get_BackupCycleIn_InputPort(0);
    Svc::TimerVal cycleStart;
    cycleStart.take();
    port->invoke(cycleStart);
    Os::Task::delay(10);
}

void exitTasks(void) {
    rgDecouple_ptr->exit();
    imuDecouple_ptr->exit();
    actDecouple_ptr->exit();
    
#ifdef BUILD_SDFLIGHT // SDFLIGHT vs LINUX
    imuDRIntSnap_ptr->exitThread();
#else
    imuDRInt_ptr->exitThread();
#endif
    
    snapHealth_ptr->exit();
    cmdDisp_ptr->exit();
    eventLogger_ptr->exit();
    chanTlm_ptr->exit();
    prmDb_ptr->exit();
    textLogger_ptr->exit();
    cmdSeq_ptr->exit();
}


void print_usage() {
    (void) printf("Usage: ./SDREF [options]\n"
		  "-p\tport_number\n"
		  "-a\thostname/IP address\n"
		  "-l\tFor time-based cycles\n"
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

    // Removes ROS cmdline args as a side-effect
    ros::init(argc,argv,"SDREF", ros::init_options::NoSigintHandler);

    while ((option = getopt(argc, argv, "ifhlsp:x:a:u:t:o:b:z:")) != -1){
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
            case 'a':
                hostname = optarg;
                break;
            case 's':
                startSocketNow = true;
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
		 startSocketNow);
    //dumparch();
    
    ros::start();

    hlRosIface_ptr->startIntTask(30, 5*1000*1024);
    mrCtrlIface_ptr->startIntTask(30, 5*1000*1024);
    filterIface_ptr->startIntTask(30, 5*1000*1024);
    rosSeq_ptr->startIntTask(30, 5*1000*1024);

    hlRosIface_ptr->startPub();
    mrCtrlIface_ptr->startPub();
    filterIface_ptr->startPub();
    rosSeq_ptr->startPub();

    ros::console::shutdown();

    while (!mpu9250_ptr->isReady()) {
        Svc::InputCyclePort* port = rgDev_ptr->get_CycleIn_InputPort(0);
        Svc::TimerVal cycleStart;
        cycleStart.take();
        port->invoke(cycleStart);
        Os::Task::delay(10);
    }
    rgDecouple_ptr->setEnabled(true);
    
    int backupCycle = 0;

    while (!terminate) {
        run1backupCycle();
        backupCycle++;
    }

    DEBUG_PRINT("Stopping tasks\n");
    ros::shutdown();
    
    exitTasks();

    // Give time for threads to exit
    (void) printf("Waiting for threads...\n");
    Os::Task::delay(1000);

    (void) printf("Exiting...\n");
    return 0;
}
