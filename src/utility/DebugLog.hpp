// Copyright 2010 Code Project Open Licence
// Author: Fredrik Bornander
// Source: http://www.codeproject.com/Articles/63736/Simple-debug-BornanderLog-for-C
//
// Notes(LPE): This code was intended for windows. I removed the stuff that
// didn't jive with gcc.

#pragma once

#ifndef DEBUG_LOG_ENABLE

#define DEBUG_USING_NAMESPACE

#define DEBUG_SET_STREAM(stream)
#define DEBUG_METHOD()
#define DEBUG_MESSAGE(debug_message)
#define DEBUG_VALUE_OF(variable)
//#define DEBUG_VALUE_AND_TYPE_OF(variable)

#else

#include <iostream>
#include <string>
#include <time.h>
#include <typeinfo>

#define DEBUG_SET_STREAM(stream) { BornanderLog::set_stream(stream); }
#define DEBUG_METHOD() BornanderLog _debugLog(__func__, __FILE__, __LINE__);
#define DEBUG_MESSAGE(debug_message) { _debugLog.message(debug_message); }
#define DEBUG_VALUE_OF(variable) { _debugLog.value_of(#variable, variable, true); }
//#define DEBUG_VALUE_AND_TYPE_OF(variable) { _debugLog.value_of(#variable, variable, true); }

enum list_segment
{
  all,
  top,
  bottom
};

class BornanderLog
{
  private:  // Members
    static int indentation;
    static std::ostream* stream;

    const std::string context;
    const clock_t start_time;

  private:  // Methods
    void write_indentation();
    void write_indentation(const char prefix);
  public:    // Methods
    void message(const std::string& message);
    template<class T> void value_of(const std::string& name, const T& value, const bool outputTypeInformation);
    static void set_stream(std::ostream& stream);
  public:    // Constructor, Destructor
    BornanderLog(const std::string& ctx_func, const std::string& ctx_file, const int ctx_line);

    ~BornanderLog();
};

template<class T> void BornanderLog::value_of(const std::string& name, const T& value, const bool outputTypeInformation)
{
  write_indentation();
  *stream << name;
  if (outputTypeInformation)
  {
    *stream << "(" << typeid(value).name() << ")";
  }
  *stream << "=[" << value << "]" << std::endl;
  stream->flush();
}

#endif
