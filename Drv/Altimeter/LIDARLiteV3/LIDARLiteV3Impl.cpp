// ====================================================================== 
// \title  LIDARLiteV3Impl.cpp
// \author Gerik Kubiak
// \brief  cpp file for LIDARLiteV3 component implementation class
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


#include <Drv/Altimeter/LIDARLiteV3/LIDARLiteV3Impl.hpp>
#include "Fw/Types/BasicTypes.hpp"

#include <Drv/I2CDriverPorts/I2CPortHelper.hpp>
#include <Drv/Altimeter/LIDARLiteV3/LIDARLiteV3ImplCfg.hpp>

#include <stdio.h>

namespace Drv {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction 
  // ----------------------------------------------------------------------

  LIDARLiteV3ComponentImpl ::
#if FW_OBJECT_NAMES == 1
    LIDARLiteV3ComponentImpl(
        const char *const compName
    ) :
      LIDARLiteV3ComponentBase(compName),
#else
    LIDARLiteV3Impl(void)
      :
#endif
      init_state(LLV3_INIT_CONFIG),
      init_registers_idx(0),
      i2c_state(LLV3_I2C_WAITING),
      statusByte(0u),
      numTries(0u),
      i2cWriteBuffer(),
      i2cReadBuffer(),
      seq(0u)
  {

  }

  void LIDARLiteV3ComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    ) 
  {
    LIDARLiteV3ComponentBase::init(instance);
  }

  LIDARLiteV3ComponentImpl ::
    ~LIDARLiteV3ComponentImpl(void)
  {

  }

  const LIDARLiteV3ComponentImpl::LLV3InitReg LIDARLiteV3ComponentImpl::init_registers[] = {
      {LLV3_SIG_COUNT_VAL_REG, LLV3_SIG_COUNT_VAL},
      {LLV3_ACQ_CONFIG_REG, LLV3_ACQ_CONFIG_VAL},
      {LLV3_THRESHOLD_BYPASS_REG, LLV3_THRESHOLD_BYPASS_VAL}
  };


  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void LIDARLiteV3ComponentImpl::reset_i2c_buffers()
  {
    this->i2cWriteBuffer.set(0, 0, reinterpret_cast<U64>(i2cWriteBufferArr), LLV3_WRITE_BUFF_LEN);
    this->i2cReadBuffer.set(0, 0, reinterpret_cast<U64>(i2cReadBufferArr), LLV3_READ_BUFF_LEN);
  }

  void LIDARLiteV3ComponentImpl::initialize_sm()
  {
    bool should_block;
    Drv::I2CStatus i2cStatus;

    // Execute state machine actions/transitions:
    switch (this->init_state)
    {
        case LLV3_INIT_WAITING:
            // do nothing; wait for startup from previous device
            break;

        case LLV3_INIT_CONFIG:

            this->I2CConfig_out(0, 100000, LLV3_ADDR, 0);

            this->init_state = LLV3_INIT_WRITE_REG;
            break;

        case LLV3_INIT_WRITE_REG:
        {
            this->reset_i2c_buffers();

            I2CWriteSingleRegisterHelper(LIDARLiteV3ComponentImpl::init_registers[init_registers_idx].reg,
                                         LIDARLiteV3ComponentImpl::init_registers[init_registers_idx].value,
                                         this->i2cWriteBuffer,
                                         this->i2cReadBuffer);

            this->I2CWriteRead_out(0, this->i2cWriteBuffer, this->i2cReadBuffer);

            this->init_state = LLV3_INIT_WRITE_STATUS;
            break;
        }
        case LLV3_INIT_WRITE_STATUS:
        {
            should_block = false;

            i2cStatus = this->I2CWriteReadStatus_out(0, should_block);

            switch (i2cStatus) {
                case I2C_STATUS_BUSY:
                {
                    // Do nothing. Wait for transaction to complete
                    break;
                }
                case I2C_STATUS_OK:
                {
                    // Continue with initialization
                    this->init_registers_idx++;

                    if (this->init_registers_idx >= FW_NUM_ARRAY_ELEMENTS(LIDARLiteV3ComponentImpl::init_registers))
                    {
                        this->log_ACTIVITY_LO_LLV3_InitComplete();
                        this->init_state = LLV3_INIT_COMPLETE;
                    } else {
                        this->init_state = LLV3_INIT_WRITE_REG;
                    }

                    break;
                }
                case I2C_STATUS_TX_NACK:
                {
                    // Retry initialization
                    this->init_state = LLV3_INIT_WRITE_REG;
                    break;
                }
                case I2C_STATUS_RX_NACK:
                {
                    // Did not send a Rx transaction. Something went very wrong
                    FW_ASSERT(false);
                    break;
                }
                default:
                {
                    FW_ASSERT(false);
                    break;
                }
            }

            break;
        }
        case LLV3_INIT_COMPLETE:
        {
            // Done with initialization
            break;
        }
        default:
        {
            FW_ASSERT(false);
            break;
        }
    }
  }

  void LIDARLiteV3ComponentImpl ::
    send_measurement()
  {
    U32 measurement_dn;
    F32 measurement_m;

    measurement_dn = static_cast<U32>(i2cReadBufferArr[0]) << 8 |
                     static_cast<U32>(i2cReadBufferArr[1]);

    // Convert cm to m
    measurement_m = measurement_dn * 0.01;

    {
        using namespace ROS::sensor_msgs;
        using namespace ROS::std_msgs;

        Range r(Header(this->seq, this->getTime(), 0), // TODO(mereweth) - frame id
                this->statusByte, //NOTE(Mereweth) - we co-opt this field to send status byte
                0.008, // beam spread
                0.1, // min distance
                100.0, // max distance
                measurement_m);
        if (this->isConnected_AltimeterSend_OutputPort(0)) {
            this->AltimeterSend_out(0, r);
        }
    }
    this->tlmWrite_LLV3_Distance(measurement_m);
  }

  bool LIDARLiteV3ComponentImpl ::
    verify_status()
  {
    this->statusByte = i2cReadBufferArr[0];
    U8 exp_mask_val = LLV3_STATUS_HEALTH_OK;

    return ((this->statusByte & LLV3_STATUS_ERR_MASK) == exp_mask_val) ? true : false;
  }

  void LIDARLiteV3ComponentImpl ::
    SchedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    Drv::I2CStatus i2cStatus;

    if (context == LLV3_RG_FAST) {
        if (this->init_state != LLV3_INIT_COMPLETE)
        {
            this->initialize_sm();
        } else {
            switch (this->i2c_state)
            {
                case LLV3_I2C_WAITING:
                    break;
                case LLV3_I2C_ACQ_CMD:
                    this->reset_i2c_buffers();
                    I2CWriteSingleRegisterHelper(LLV3_ACQ_REG, LLV3_ACQ_CMD,
                                                 this->i2cWriteBuffer,
                                                 this->i2cReadBuffer);

                    this->I2CWriteRead_out(0,
                                           this->i2cWriteBuffer,
                                           this->i2cReadBuffer);

                    this->i2c_state = LLV3_I2C_ACQ_CMD_STATUS;
                    break;

                case LLV3_I2C_ACQ_CMD_STATUS:
                    
                    i2cStatus = this->I2CWriteReadStatus_out(0, false);
                    
                    switch (i2cStatus)
                    {
                        case I2C_STATUS_BUSY:
                            break;
                        case I2C_STATUS_OK:
                            this->numTries = 0;
                            this->i2c_state = LLV3_I2C_WAITING;
                            break;
                        case I2C_STATUS_TX_NACK:
                            // EVR - TODO(mereweth) - do another ACQ cmd?
                            if (++this->numTries >= LLV3_NUM_TRIES) {
                                this->i2c_state = LLV3_I2C_RESET_CMD;
                                break;
                            }
                            this->i2c_state = LLV3_I2C_ACQ_CMD;
                            break;
                        case I2C_STATUS_RX_NACK:
                            FW_ASSERT(false);
                            break;
                        default:
                            FW_ASSERT(false);
                            break;
                    }

                    break;
                case LLV3_I2C_STATUS_CMD:

                    this->reset_i2c_buffers();
                    I2CReadRegisterHelper(LLV3_STATUS_REG, 1,
                                          this->i2cWriteBuffer,
                                          this->i2cReadBuffer);

                    this->I2CWriteRead_out(0,
                                           this->i2cWriteBuffer,
                                           this->i2cReadBuffer);

                    this->i2c_state = LLV3_I2C_STATUS_CMD_STATUS;
                    break;
                case LLV3_I2C_STATUS_CMD_STATUS:

                    i2cStatus = this->I2CWriteReadStatus_out(0, false);
                    
                    switch (i2cStatus)
                    {
                        case I2C_STATUS_BUSY:
                            break;
                        case I2C_STATUS_OK:
                            if (0b10101010 == this->i2cReadBufferArr[0]) {
                                this->i2c_state = LLV3_I2C_RESET_CMD;
                            }
                            else if (this->verify_status()) {

                                if (this->i2cReadBufferArr[0] & LLV3_STATUS_BUSY) {
                                    if (++this->numTries >= LLV3_NUM_TRIES) {
                                        this->i2c_state = LLV3_I2C_RESET_CMD;
                                        break;
                                    }
                                    // Still taking measurement. Wait - other RG context will put us back in STATUS_CMD
                                    this->i2c_state = LLV3_I2C_WAITING;
                                } else {
                                    this->numTries = 0;
                                    this->i2c_state = LLV3_I2C_MEASURE_CMD;
                                }
                            }
                            else {
                                // Invalid status
                                // EVR
                                this->i2c_state = LLV3_I2C_RESET_CMD;
                            }
                            break;
                        case I2C_STATUS_TX_NACK:
                        case I2C_STATUS_RX_NACK:
                            // EVR
                            if (++this->numTries >= LLV3_NUM_TRIES) {
                                this->i2c_state = LLV3_I2C_RESET_CMD;
                                break;
                            }
                            this->i2c_state = LLV3_I2C_STATUS_CMD;
                            break;
                        default:
                            FW_ASSERT(false);
                            break;
                    }

                    break;
                case LLV3_I2C_MEASURE_CMD:

                    this->reset_i2c_buffers();
                    I2CReadRegisterHelper(LLV3_MEASURE_REG | LLV3_READ_SEQ,
                                          2,
                                          this->i2cWriteBuffer,
                                          this->i2cReadBuffer);

                    this->I2CWriteRead_out(0,
                                           this->i2cWriteBuffer,
                                           this->i2cReadBuffer);
                    this->i2c_state = LLV3_I2C_MEASURE_CMD_STATUS;
                    break;

                case LLV3_I2C_MEASURE_CMD_STATUS:

                    i2cStatus = this->I2CWriteReadStatus_out(0, false);

                    switch (i2cStatus)
                    {
                        case I2C_STATUS_BUSY:
                            break;
                        case I2C_STATUS_OK:
                            this->numTries = 0;
                            this->i2c_state = LLV3_I2C_ACQ_CMD;
                            this->send_measurement();
                            break;
                        case I2C_STATUS_TX_NACK:
                        case I2C_STATUS_RX_NACK:
                            // EVR
                            if (++this->numTries >= LLV3_NUM_TRIES) {
                                this->i2c_state = LLV3_I2C_RESET_CMD;
                                break;
                            }
                            this->i2c_state = LLV3_I2C_MEASURE_CMD;
                            break;
                        default:
                            FW_ASSERT(false);
                            break;
                    }

                    break;
                    
                case LLV3_I2C_RESET_CMD:
                    this->numTries = 0;
                    this->reset_i2c_buffers();
                    I2CWriteSingleRegisterHelper(LLV3_ACQ_REG, LLV3_RESET_CMD,
                                                 this->i2cWriteBuffer,
                                                 this->i2cReadBuffer);

                    this->I2CWriteRead_out(0,
                                           this->i2cWriteBuffer,
                                           this->i2cReadBuffer);

                    this->i2c_state = LLV3_I2C_RESET_CMD_STATUS;
                    break;

                case LLV3_I2C_RESET_CMD_STATUS:
                    
                    i2cStatus = this->I2CWriteReadStatus_out(0, false);
                    
                    switch (i2cStatus)
                    {
                        case I2C_STATUS_BUSY:
                            break;
                        case I2C_STATUS_OK:
                            this->i2c_state = LLV3_I2C_WAITING;
                            this->init_state = LLV3_INIT_CONFIG;
                            break;
                        case I2C_STATUS_TX_NACK:
                            // EVR - TODO(mereweth) - do another RESET cmd?
                            this->i2c_state = LLV3_I2C_RESET_CMD;
                            break;
                        case I2C_STATUS_RX_NACK:
                            FW_ASSERT(false);
                            break;
                        default:
                            FW_ASSERT(false);
                            break;
                    }

                    break;

            }
        }
    } else if (context == LLV3_RG_MEASURE) {

        if (this->init_state == LLV3_INIT_COMPLETE) {
            if (this->i2c_state == LLV3_I2C_WAITING) {
                // TODO(mereweth) - separate timeout for number of busy status commands?
                this->i2c_state = LLV3_I2C_STATUS_CMD;
            } else {
                // EVR here
            }
        }
    } else {
        FW_ASSERT(false);
    }
  }

} // end namespace Drv
