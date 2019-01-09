#ifndef _ACTIVE_FILE_LOGGER_STREAMS_HPP_
#define _ACTIVE_FILE_LOGGER_STREAMS_HPP_

typedef enum {
    AFL_SERIAL_DATA = 0,
    AFL_HLROSIFACE_IMUNOCOV,
    AFL_HLROSIFACE_ODOMNOCOV, 
    AFL_HLROSIFACE_ACCEL_CMD,
    AFL_ACTADAP_ESC,
    NUM_AFL_STREAMS,
} active_file_logger_stream_t;

#endif //_ACTIVE_FILE_LOGGER_STREAMS_HPP_
