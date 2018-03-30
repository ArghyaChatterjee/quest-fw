// ======================================================================
// \title  MPU9250Reg.hpp
// \author mereweth
// \brief  hpp file for MPU9250 registers
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

namespace Drv {

enum MPU9250Reg {
    MPU9250_REG_WHOAMI            = 0x75,
    MPU9250_REG_SMPLRT_DIV        = 0x19,
    MPU9250_REG_CONFIG            = 0x1A,
    MPU9250_REG_GYRO_CONFIG        = 0x1B,
    MPU9250_REG_ACCEL_CONFIG        = 0x1C,
    MPU9250_REG_ACCEL_CONFIG2    = 0x1D,
    MPU9250_REG_LPACCEL_ODR        = 0x1E,
    MPU9250_REG_WOM_THRESH        = 0x1F,
    MPU9250_REG_FIFO_EN            = 0x23,
    MPU9250_REG_I2C_MST_CTRL        = 0x24,
    MPU9250_REG_I2C_SLV0_ADDR    = 0x25,
    MPU9250_REG_I2C_SLV0_REG        = 0x26,
    MPU9250_REG_I2C_SLV0_CTRL    = 0x27,
    MPU9250_REG_I2C_SLV1_ADDR    = 0x28,
    MPU9250_REG_I2C_SLV1_REG        = 0x29,
    MPU9250_REG_I2C_SLV1_CTRL    = 0x2A,
    MPU9250_REG_I2C_SLV2_ADDR    = 0x2B,
    MPU9250_REG_I2C_SLV2_REG        = 0x2C,
    MPU9250_REG_I2C_SLV2_CTRL    = 0x2D,
    MPU9250_REG_I2C_SLV3_ADDR    = 0x2E,
    MPU9250_REG_I2C_SLV3_REG        = 0x2F,
    MPU9250_REG_I2C_SLV3_CTRL    = 0x30,
    MPU9250_REG_I2C_SLV4_ADDR    = 0x31,
    MPU9250_REG_I2C_SLV4_REG        = 0x32,
    MPU9250_REG_I2C_SLV4_DO        = 0x33,
    MPU9250_REG_I2C_SLV4_CTRL    = 0x34,
    MPU9250_REG_I2C_SLV4_DI        = 0x35,
    MPU9250_REG_I2C_MST_STATUS    = 0x36,
    MPU9250_REG_INT_PIN_CFG        = 0x37,
    MPU9250_REG_INT_ENABLE        = 0x38,
    MPU9250_REG_INT_STATUS        = 0x3A,
    MPU9250_REG_ACCEL_XOUT_H        = 0x3B,
    MPU9250_REG_ACCEL_XOUT_L        = 0x3C,
    MPU9250_REG_ACCEL_YOUT_H        = 0x3D,
    MPU9250_REG_ACCEL_YOUT_L        = 0x3E,
    MPU9250_REG_ACCEL_ZOUT_H        = 0x3F,
    MPU9250_REG_ACCEL_ZOUT_L        = 0x40,
    MPU9250_REG_TEMP_OUT_H        = 0x41,
    MPU9250_REG_TEMP_OUT_L        = 0x42,
    MPU9250_REG_GYRO_XOUT_H        = 0x43,
    MPU9250_REG_GYRO_XOUT_L        = 0x44,
    MPU9250_REG_GYRO_YOUT_H        = 0x45,
    MPU9250_REG_GYRO_YOUT_L        = 0x46,
    MPU9250_REG_GYRO_ZOUT_H        = 0x47,
    MPU9250_REG_GYRO_ZOUT_L        = 0x48,
    MPU9250_REG_EXT_SENS_DATA_00    = 0x49,
    MPU9250_REG_I2C_SLV0_D0        = 0x63,
    MPU9250_REG_I2C_SLV1_D0        = 0x64,
    MPU9250_REG_I2C_SLV2_D0        = 0x65,
    MPU9250_REG_I2C_SLV3_D0        = 0x66,
    MPU9250_REG_I2C_MST_DELAY_CTRL    = 0x67,
    MPU9250_REG_SIGNAL_PATH_RESET    = 0x68,
    MPU9250_REG_MOT_DETECT_CTRL    = 0x69,
    MPU9250_REG_USER_CTRL        = 0x6A,
    MPU9250_REG_PWR_MGMT_1        = 0x6B,
    MPU9250_REG_PWR_MGMT_2        = 0x6C,
    MPU9250_REG_FIFO_COUNTH        = 0x72,
    MPU9250_REG_FIFO_COUNTL        = 0x73,
    MPU9250_REG_FIFO_R_W            = 0x74
};

enum MPU9250Bits {
    MPU9250_BITS_SLEEP            = 0x40,
    MPU9250_BITS_H_RESET            = 0x80,

    MPU9250_BITS_USER_CTRL_FIFO_EN        = 0x40,
    MPU9250_BITS_USER_CTRL_FIFO_RST        = 0x04,
    MPU9250_BITS_USER_CTRL_I2C_MST_EN      = 0x20,
    MPU9250_BITS_USER_CTRL_I2C_IF_DIS      = 0x10,
    MPU9250_BITS_USER_CTRL_I2C_MST_RST      = 0x02,

    MPU9250_BITS_CONFIG_FIFO_MODE_OVERWRITE    = 0x00,
    MPU9250_BITS_CONFIG_FIFO_MODE_STOP    = 0x40,

    MPU9250_BITS_GYRO_ST_X            = 0x80,
    MPU9250_BITS_GYRO_ST_Y            = 0x40,
    MPU9250_BITS_GYRO_ST_Z            = 0x20,
    MPU9250_BITS_FS_250DPS            = 0x00,
    MPU9250_BITS_FS_500DPS            = 0x08,
    MPU9250_BITS_FS_1000DPS            = 0x10,
    MPU9250_BITS_FS_2000DPS            = 0x18,
    MPU9250_BITS_FS_MASK            = 0x18,
// This is FCHOICE_B which is the inverse of FCHOICE
    MPU9250_BITS_BW_3600HZ            = 0x02,
// The FCHOICE bits are the same for all Bandwidths below 3600 Hz.
    MPU9250_BITS_BW_LT3600HZ        = 0x00,

    MPU9250_BITS_DLPF_CFG_250HZ        = 0x00,
    MPU9250_BITS_DLPF_CFG_184HZ        = 0x01,
    MPU9250_BITS_DLPF_CFG_92HZ        = 0x02,
    MPU9250_BITS_DLPF_CFG_41HZ        = 0x03,
    MPU9250_BITS_DLPF_CFG_20HZ        = 0x04,
    MPU9250_BITS_DLPF_CFG_10HZ        = 0x05,
    MPU9250_BITS_DLPF_CFG_5HZ        = 0x06,
    MPU9250_BITS_DLPF_CFG_3600HZ    = 0x07,
    MPU9250_BITS_DLPF_CFG_MASK        = 0x07,

    MPU9250_BITS_FIFO_ENABLE_TEMP_OUT    = 0x80,
    MPU9250_BITS_FIFO_ENABLE_GYRO_XOUT    = 0x40,
    MPU9250_BITS_FIFO_ENABLE_GYRO_YOUT    = 0x20,
    MPU9250_BITS_FIFO_ENABLE_GYRO_ZOUT    = 0x10,
    MPU9250_BITS_FIFO_ENABLE_ACCEL        = 0x08,
    MPU9250_BITS_FIFO_ENABLE_SLV2        = 0x04,
    MPU9250_BITS_FIFO_ENABLE_SLV1        = 0x02,
    MPU9250_BITS_FIFO_ENABLE_SLV0        = 0x01,

    MPU9250_BITS_ACCEL_CONFIG_16G        = 0x18,

// This is ACCEL_FCHOICE_B which is the inverse of ACCEL_FCHOICE
    MPU9250_BITS_ACCEL_CONFIG2_BW_1130HZ    = 0x08,

    MPU9250_BITS_I2C_SLV0_EN    = 0x80,
    MPU9250_BITS_I2C_SLV0_READ_8BYTES = 0x08,
    MPU9250_BITS_I2C_SLV1_EN    = 0x80,
    MPU9250_BITS_I2C_SLV1_DIS   = 0x00,
    MPU9250_BITS_I2C_SLV2_EN    = 0x80,
    MPU9250_BITS_I2C_SLV4_EN    = 0x80,
    MPU9250_BITS_I2C_SLV4_DONE  = 0x40,

    MPU9250_BITS_SLV4_DLY_EN    = 0x10,
    MPU9250_BITS_SLV3_DLY_EN    = 0x08,
    MPU9250_BITS_SLV2_DLY_EN    = 0x04,
    MPU9250_BITS_SLV1_DLY_EN    = 0x02,
    MPU9250_BITS_SLV0_DLY_EN    = 0x01,

    MPU9250_BITS_INT_STATUS_FIFO_OVERFLOW    = 0x10,

    MPU9250_BITS_I2C_MST_CLK_400_KHZ    = 0x0D
};

} // end namespace Drv
