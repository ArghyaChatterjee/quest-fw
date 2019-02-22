// ======================================================================
// \title  BufferLogger/test/ut/GTestBase.cpp
// \author Auto-generated
// \brief  cpp file for BufferLogger component Google Test harness base class
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

#include "GTestBase.hpp"

namespace Svc {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  BufferLoggerGTestBase ::
    BufferLoggerGTestBase(
#if FW_OBJECT_NAMES == 1
        const char *const compName,
        const U32 maxHistorySize
#else
        const U32 maxHistorySize
#endif
    ) : 
        BufferLoggerTesterBase (
#if FW_OBJECT_NAMES == 1
            compName,
#endif
            maxHistorySize
        )
  {

  }

  BufferLoggerGTestBase ::
    ~BufferLoggerGTestBase(void)
  {

  }

  // ----------------------------------------------------------------------
  // Commands
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertCmdResponse_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ((unsigned long) size, this->cmdResponseHistory->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of command response history\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->cmdResponseHistory->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertCmdResponse(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CommandResponse response
    )
    const
  {
    ASSERT_LT(index, this->cmdResponseHistory->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into command response history\n"
      << "  Expected: Less than size of command response history (" 
      << this->cmdResponseHistory->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const CmdResponse& e = this->cmdResponseHistory->at(index);
    ASSERT_EQ(opCode, e.opCode)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Opcode at index "
      << index
      << " in command response history\n"
      << "  Expected: " << opCode << "\n"
      << "  Actual:   " << e.opCode << "\n";
    ASSERT_EQ(cmdSeq, e.cmdSeq)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Command sequence number at index "
      << index
      << " in command response history\n"
      << "  Expected: " << cmdSeq << "\n"
      << "  Actual:   " << e.cmdSeq << "\n";
    ASSERT_EQ(response, e.response)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Command response at index "
      << index
      << " in command resopnse history\n"
      << "  Expected: " << response << "\n"
      << "  Actual:   " << e.response << "\n";
  }

  // ----------------------------------------------------------------------
  // Telemetry
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertTlm_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->tlmSize)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Total size of all telemetry histories\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->tlmSize << "\n";
  }

  // ----------------------------------------------------------------------
  // Channel: BL_NumLoggedBuffers
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertTlm_BL_NumLoggedBuffers_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(this->tlmHistory_BL_NumLoggedBuffers->size(), size)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for telemetry channel BL_NumLoggedBuffers\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->tlmHistory_BL_NumLoggedBuffers->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertTlm_BL_NumLoggedBuffers(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const U32& val
    )
    const
  {
    ASSERT_LT(index, this->tlmHistory_BL_NumLoggedBuffers->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into history of telemetry channel BL_NumLoggedBuffers\n"
      << "  Expected: Less than size of history (" 
      << this->tlmHistory_BL_NumLoggedBuffers->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const TlmEntry_BL_NumLoggedBuffers& e =
      this->tlmHistory_BL_NumLoggedBuffers->at(index);
    ASSERT_EQ(val, e.arg)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value at index "
      << index
      << " on telmetry channel BL_NumLoggedBuffers\n"
      << "  Expected: " << val << "\n"
      << "  Actual:   " << e.arg << "\n";
  }

  // ----------------------------------------------------------------------
  // Channel: BL_NumLoggedBytes
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertTlm_BL_NumLoggedBytes_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(this->tlmHistory_BL_NumLoggedBytes->size(), size)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for telemetry channel BL_NumLoggedBytes\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->tlmHistory_BL_NumLoggedBytes->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertTlm_BL_NumLoggedBytes(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const U32& val
    )
    const
  {
    ASSERT_LT(index, this->tlmHistory_BL_NumLoggedBytes->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into history of telemetry channel BL_NumLoggedBytes\n"
      << "  Expected: Less than size of history (" 
      << this->tlmHistory_BL_NumLoggedBytes->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const TlmEntry_BL_NumLoggedBytes& e =
      this->tlmHistory_BL_NumLoggedBytes->at(index);
    ASSERT_EQ(val, e.arg)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value at index "
      << index
      << " on telmetry channel BL_NumLoggedBytes\n"
      << "  Expected: " << val << "\n"
      << "  Actual:   " << e.arg << "\n";
  }

  // ----------------------------------------------------------------------
  // Events
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Total size of all event histories\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_LogFileClosed
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileClosed_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventHistory_BL_LogFileClosed->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_LogFileClosed\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventHistory_BL_LogFileClosed->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileClosed(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const char *const file
    ) const
  {
    ASSERT_GT(this->eventHistory_BL_LogFileClosed->size(), index)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into history of event BL_LogFileClosed\n"
      << "  Expected: Less than size of history (" 
      << this->eventHistory_BL_LogFileClosed->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const EventEntry_BL_LogFileClosed& e =
      this->eventHistory_BL_LogFileClosed->at(index);
    ASSERT_STREQ(file, e.file.toChar())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument file at index "
      << index
      << " in history of event BL_LogFileClosed\n"
      << "  Expected: " << file << "\n"
      << "  Actual:   " << e.file.toChar() << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_LogFileOpenError
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileOpenError_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventHistory_BL_LogFileOpenError->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_LogFileOpenError\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventHistory_BL_LogFileOpenError->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileOpenError(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const U32 errornum,
        const char *const file
    ) const
  {
    ASSERT_GT(this->eventHistory_BL_LogFileOpenError->size(), index)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into history of event BL_LogFileOpenError\n"
      << "  Expected: Less than size of history (" 
      << this->eventHistory_BL_LogFileOpenError->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const EventEntry_BL_LogFileOpenError& e =
      this->eventHistory_BL_LogFileOpenError->at(index);
    ASSERT_EQ(errornum, e.errornum)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument errornum at index "
      << index
      << " in history of event BL_LogFileOpenError\n"
      << "  Expected: " << errornum << "\n"
      << "  Actual:   " << e.errornum << "\n";
    ASSERT_STREQ(file, e.file.toChar())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument file at index "
      << index
      << " in history of event BL_LogFileOpenError\n"
      << "  Expected: " << file << "\n"
      << "  Actual:   " << e.file.toChar() << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_LogFileValidationError
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileValidationError_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventHistory_BL_LogFileValidationError->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_LogFileValidationError\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventHistory_BL_LogFileValidationError->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileValidationError(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const char *const validationFile,
        const U32 status
    ) const
  {
    ASSERT_GT(this->eventHistory_BL_LogFileValidationError->size(), index)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into history of event BL_LogFileValidationError\n"
      << "  Expected: Less than size of history (" 
      << this->eventHistory_BL_LogFileValidationError->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const EventEntry_BL_LogFileValidationError& e =
      this->eventHistory_BL_LogFileValidationError->at(index);
    ASSERT_STREQ(validationFile, e.validationFile.toChar())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument validationFile at index "
      << index
      << " in history of event BL_LogFileValidationError\n"
      << "  Expected: " << validationFile << "\n"
      << "  Actual:   " << e.validationFile.toChar() << "\n";
    ASSERT_EQ(status, e.status)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument status at index "
      << index
      << " in history of event BL_LogFileValidationError\n"
      << "  Expected: " << status << "\n"
      << "  Actual:   " << e.status << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_LogFileWriteError
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileWriteError_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventHistory_BL_LogFileWriteError->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_LogFileWriteError\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventHistory_BL_LogFileWriteError->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertEvents_BL_LogFileWriteError(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const U32 errornum,
        const U32 bytesWritten,
        const U32 bytesToWrite,
        const char *const file
    ) const
  {
    ASSERT_GT(this->eventHistory_BL_LogFileWriteError->size(), index)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into history of event BL_LogFileWriteError\n"
      << "  Expected: Less than size of history (" 
      << this->eventHistory_BL_LogFileWriteError->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const EventEntry_BL_LogFileWriteError& e =
      this->eventHistory_BL_LogFileWriteError->at(index);
    ASSERT_EQ(errornum, e.errornum)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument errornum at index "
      << index
      << " in history of event BL_LogFileWriteError\n"
      << "  Expected: " << errornum << "\n"
      << "  Actual:   " << e.errornum << "\n";
    ASSERT_EQ(bytesWritten, e.bytesWritten)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument bytesWritten at index "
      << index
      << " in history of event BL_LogFileWriteError\n"
      << "  Expected: " << bytesWritten << "\n"
      << "  Actual:   " << e.bytesWritten << "\n";
    ASSERT_EQ(bytesToWrite, e.bytesToWrite)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument bytesToWrite at index "
      << index
      << " in history of event BL_LogFileWriteError\n"
      << "  Expected: " << bytesToWrite << "\n"
      << "  Actual:   " << e.bytesToWrite << "\n";
    ASSERT_STREQ(file, e.file.toChar())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument file at index "
      << index
      << " in history of event BL_LogFileWriteError\n"
      << "  Expected: " << file << "\n"
      << "  Actual:   " << e.file.toChar() << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_Activated
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_Activated_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_BL_Activated)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_Activated\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_BL_Activated << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_Deativated
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_Deativated_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_BL_Deativated)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_Deativated\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_BL_Deativated << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_NoLogFileOpenInitError
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_NoLogFileOpenInitError_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_BL_NoLogFileOpenInitError)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_NoLogFileOpenInitError\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_BL_NoLogFileOpenInitError << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: BL_FileFlushError
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertEvents_BL_FileFlushError_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventHistory_BL_FileFlushError->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for event BL_FileFlushError\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventHistory_BL_FileFlushError->size() << "\n";
  }

  void BufferLoggerGTestBase ::
    assertEvents_BL_FileFlushError(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 index,
        const char *const file,
        const U32 status
    ) const
  {
    ASSERT_GT(this->eventHistory_BL_FileFlushError->size(), index)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Index into history of event BL_FileFlushError\n"
      << "  Expected: Less than size of history (" 
      << this->eventHistory_BL_FileFlushError->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const EventEntry_BL_FileFlushError& e =
      this->eventHistory_BL_FileFlushError->at(index);
    ASSERT_STREQ(file, e.file.toChar())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument file at index "
      << index
      << " in history of event BL_FileFlushError\n"
      << "  Expected: " << file << "\n"
      << "  Actual:   " << e.file.toChar() << "\n";
    ASSERT_EQ(status, e.status)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Value of argument status at index "
      << index
      << " in history of event BL_FileFlushError\n"
      << "  Expected: " << status << "\n"
      << "  Actual:   " << e.status << "\n";
  }

  // ----------------------------------------------------------------------
  // From ports
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assertFromPortHistory_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->fromPortHistorySize)
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Total size of all from port histories\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->fromPortHistorySize << "\n";
  }

  // ----------------------------------------------------------------------
  // From port: bufferSendOut
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assert_from_bufferSendOut_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->fromPortHistory_bufferSendOut->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for from_bufferSendOut\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->fromPortHistory_bufferSendOut->size() << "\n";
  }

  // ----------------------------------------------------------------------
  // From port: pingOut
  // ----------------------------------------------------------------------

  void BufferLoggerGTestBase ::
    assert_from_pingOut_size(
        const char *const __callSiteFileName,
        const U32 __callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->fromPortHistory_pingOut->size())
      << "\n"
      << "  File:     " << __callSiteFileName << "\n"
      << "  Line:     " << __callSiteLineNumber << "\n"
      << "  Value:    Size of history for from_pingOut\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->fromPortHistory_pingOut->size() << "\n";
  }

} // end namespace Svc
