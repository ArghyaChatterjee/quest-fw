// ======================================================================
// \title  UdpSender/test/ut/GTestBase.hpp
// \author Auto-generated
// \brief  hpp file for UdpSender component Google Test harness base class
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

#ifndef UdpSender_GTEST_BASE_HPP
#define UdpSender_GTEST_BASE_HPP

#include "TesterBase.hpp"
#include "gtest/gtest.h"

// ----------------------------------------------------------------------
// Macros for telemetry history assertions
// ----------------------------------------------------------------------

#define ASSERT_TLM_SIZE(size) \
  this->assertTlm_size(__FILE__, __LINE__, size)

#define ASSERT_TLM_US_PacketsSent_SIZE(size) \
  this->assertTlm_US_PacketsSent_size(__FILE__, __LINE__, size)

#define ASSERT_TLM_US_PacketsSent(index, value) \
  this->assertTlm_US_PacketsSent(__FILE__, __LINE__, index, value)

#define ASSERT_TLM_US_BytesSent_SIZE(size) \
  this->assertTlm_US_BytesSent_size(__FILE__, __LINE__, size)

#define ASSERT_TLM_US_BytesSent(index, value) \
  this->assertTlm_US_BytesSent(__FILE__, __LINE__, index, value)

// ----------------------------------------------------------------------
// Macros for event history assertions 
// ----------------------------------------------------------------------

#define ASSERT_EVENTS_SIZE(size) \
  this->assertEvents_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_US_PortOpened_SIZE(size) \
  this->assertEvents_US_PortOpened_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_US_SocketError_SIZE(size) \
  this->assertEvents_US_SocketError_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_US_SocketError(index, _error) \
  this->assertEvents_US_SocketError(__FILE__, __LINE__, index, _error)

#define ASSERT_EVENTS_US_SendError_SIZE(size) \
  this->assertEvents_US_SendError_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_US_SendError(index, _error) \
  this->assertEvents_US_SendError(__FILE__, __LINE__, index, _error)

namespace Svc {

  //! \class UdpSenderGTestBase
  //! \brief Auto-generated base class for UdpSender component Google Test harness
  //!
  class UdpSenderGTestBase :
    public UdpSenderTesterBase
  {

    protected:

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

      //! Construct object UdpSenderGTestBase
      //!
      UdpSenderGTestBase(
#if FW_OBJECT_NAMES == 1
          const char *const compName, /*!< The component name*/
          const U32 maxHistorySize /*!< The maximum size of each history*/
#else
          const U32 maxHistorySize /*!< The maximum size of each history*/
#endif
      );

      //! Destroy object UdpSenderGTestBase
      //!
      virtual ~UdpSenderGTestBase(void);

    protected:

      // ----------------------------------------------------------------------
      // Telemetry
      // ----------------------------------------------------------------------

      //! Assert size of telemetry history
      //!
      void assertTlm_size(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Channel: US_PacketsSent
      // ----------------------------------------------------------------------

      //! Assert telemetry value in history at index
      //!
      void assertTlm_US_PacketsSent_size(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

      void assertTlm_US_PacketsSent(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 index, /*!< The index*/
          const U32& val /*!< The channel value*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Channel: US_BytesSent
      // ----------------------------------------------------------------------

      //! Assert telemetry value in history at index
      //!
      void assertTlm_US_BytesSent_size(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

      void assertTlm_US_BytesSent(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 index, /*!< The index*/
          const U32& val /*!< The channel value*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Events
      // ----------------------------------------------------------------------

      void assertEvents_size(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: US_PortOpened
      // ----------------------------------------------------------------------

      void assertEvents_US_PortOpened_size(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: US_SocketError
      // ----------------------------------------------------------------------

      void assertEvents_US_SocketError_size(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

      void assertEvents_US_SocketError(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 index, /*!< The index*/
          const char *const error 
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: US_SendError
      // ----------------------------------------------------------------------

      void assertEvents_US_SendError_size(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

      void assertEvents_US_SendError(
          const char *const __callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __callSiteLineNumber, /*!< The line number of the call site*/
          const U32 index, /*!< The index*/
          const char *const error 
      ) const;

  };

} // end namespace Svc

#endif
