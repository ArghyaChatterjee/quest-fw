// ======================================================================
// \title  BufferLogger.hpp
// \author bocchino, dinkel, mereweth
// \brief  Svc Buffer Logger interface
//
// \copyright
// Copyright (C) 2015-2017 California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef Svc_BufferLogger_HPP
#define Svc_BufferLogger_HPP

#include "Svc/BufferLogger/BufferLoggerComponentAc.hpp"
#include "Os/File.hpp"
#include "Fw/Types/EightyCharString.hpp"
#include "Fw/Types/Assert.hpp"
#include "Os/Mutex.hpp"
#include "Utils/Hash/Hash.hpp"
#include "Svc/BufferLogger/BufferLoggerCfg.hpp"

namespace Svc {

  class BufferLogger :
    public BufferLoggerComponentBase
  {

    PRIVATE:

      // ----------------------------------------------------------------------
      // Types
      // ----------------------------------------------------------------------

      //! A BufferLogger file
      class File {

        public:

          //! The file mode
          struct Mode {
            typedef enum {
              CLOSED = 0,
              OPEN = 1
            } t;
          };

        public:

          //! Construct a File object
          File(
              BufferLogger& bufferLogger //!< The enclosing BufferLogger instance
          );

          //! Destroy a File object
          ~File(void);

        public:

          //! Set File object parameters
          void init(
              const char *const prefix, //!< The file name prefix
              const char *const suffix, //!< The file name suffix
              const U32 maxSize, //!< The maximum file size
              const U8 sizeOfSize, //!< The number of bytes to use when storing the size field and the start of each buffer)
              const BufferLoggerFileMode writeMode,  //! The write mode - direct, bulk, or looping
              const BufferLoggerCloseMode closeMode, //! The behavior when closing the file
              const U32 directChunkSize //! The filesystem chunk size - direct mode uses this
          );

          //! Set base file name
          void setBaseName(
              const Fw::EightyCharString& baseName //!< The base file name; used with prefix, unique counter value, and suffix
          );

          //! Log a buffer
          void logBuffer(
              const U8 *const data, //!< The buffer data
              const U32 size //!< The size
          );

          //! Close the file and emit an event
          void closeAndEmitEvent(void);

          //! Flush the file
          bool flush(void);

        PRIVATE:

          //! Open the file
          void open(void);

          //! Write a buffer to a file
          //! \return Success or failure
          bool writeBuffer(
              const U8 *const data, //!< The buffer data
              const U32 size //!< The number of bytes to write
          );

          //! Write the size field of a buffer
          //! \return Success or failure
          bool writeSize(
              const U32 size //!< The size
          );

          //! Write bytes to a file
          //! \return Success or failure
          bool writeBytes(
              const void *const data, //!< The data
              const U32 length //!< The number of bytes to write
          );

          //! Write a hash file
          void writeHashFile(void);

          //! Close the file
          void close(void);

        PRIVATE:

          //! The enclosing BufferLogger instance
          BufferLogger& bufferLogger;

          //! The prefix to use for file names
          Fw::EightyCharString prefix;

          //! The suffix to use for file names
          Fw::EightyCharString suffix;

          //! The file name base
          Fw::EightyCharString baseName;

          //! Has the file name base been set
          bool baseNameBeenSet;

          //! Has initLog been called
          bool initBeenCalled;

          //! The counter to use for the same file name
          NATIVE_UINT_TYPE fileCounter;

          //! The maximum file size
          U32 maxSize;

          //! The number of bytes to use when storing the size field at the start of each buffer
          U8 sizeOfSize;

          //! The name of the currently open file
          Fw::EightyCharString name;

          // The current mode
          Mode::t mode;

          //! The underlying Os::File representation
          Os::File osFile;

          //! The number of bytes written to the current file
          U32 bytesWritten;

          //! The write mode - direct, bulk, or looping
          BufferLoggerFileMode writeMode;

          //! The behavior when closing a file (from BufferLogger command)
          BufferLoggerCloseMode closeMode;

          //! The filesystem chunk size - direct mode uses this
          U32 directChunkSize;

          //! Temporary buffer for chunk-aligned direct writes
          BYTE tempBuffer[2 * BL_MAX_DIRECT_CHUNK_SIZE - 1];

      }; // class File

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Create a BufferLogger object
      BufferLogger(
#if FW_OBJECT_NAMES == 1
          const char *const compName /*!< The component name*/
#endif
      );

      //! Initialize a BufferLogger object
      void init(
          const NATIVE_INT_TYPE queueDepth, //!< The queue depth
          const NATIVE_INT_TYPE instance //!< The instance number
      );

      // ----------------------------------------------------------------------
      // Public methods
      // ----------------------------------------------------------------------

      //! Set up log file parameters
      void initLog(
          const char *const logFilePrefix, //!< The log file name prefix
          const char *const logFileSuffix, //!< The log file name suffix
          const U32 maxFileSize, //!< The maximum file size
          const U8 sizeOfSize, //!< The number of bytes to use when storing the size field and the start of each buffer)
          const U32 maxNumBuffers, //!< The max num buffers before starting a new file; 0 is unlimited
          const BufferLoggerFileMode writeMode,  //! The write mode - direct, bulk, or looping
          const BufferLoggerCloseMode closeMode, //! The behavior when closing the file
          const U32 directChunkSize //! The filesystem chunk size - direct mode uses this
      );

      //! Set base file name
      void setBaseName(
          const Fw::EightyCharString& baseName //!< The base file name; used with prefix, unique counter value, and suffix
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for bufferSendIn
      //!
      void bufferSendIn_handler(
          const NATIVE_INT_TYPE portNum, //!< The port number
          Fw::Buffer& fwBuffer
      );

      //! Handler implementation for comIn
      //!
      void comIn_handler(
          const NATIVE_INT_TYPE portNum, //!< The port number
          Fw::ComBuffer &data, //!< Buffer containing packet data
          U32 context //!< Call context value; meaning chosen by user
      );

      //! Handler implementation for pingIn
      //!
      void pingIn_handler(
          const NATIVE_INT_TYPE portNum, //!< The port number
          U32 key //!< Value to return to pinger
      );

      //! Handler implementation for schedIn
      //!
      void schedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< The call order*/
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for BL_OpenFile command handler
      //! Open a new log file with specified name; required before activating logging
      void BL_OpenFile_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& file
      );

      //! Implementation for BL_CloseFile command handler
      //! Close the currently open log file, if any
      void BL_CloseFile_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );

      //! Implementation for BL_SetLogging command handler
      //! Sets the volatile logging state
      void BL_SetLogging_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          LogState state
      );

      //! Implementation for BL_FlushFile command handler
      //! Flushes the current open log file to disk
      void BL_FlushFile_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );

  PRIVATE:

      // ----------------------------------------------------------------------
      // Private instance variables
      // ----------------------------------------------------------------------

      //! The logging state
      LogState m_state;

      //! The file
      File m_file;

      //! The number of logged buffers
      NATIVE_UINT_TYPE m_loggedBuffers;

      //! The number of logged bytes
      NATIVE_UINT_TYPE m_loggedBytes;

      //! The max number of buffers to log per file
      NATIVE_UINT_TYPE m_maxNumBuffers;
  };

}

#endif
