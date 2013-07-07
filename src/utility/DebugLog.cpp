#include "DebugLog.hpp"

#if DEBUG_LOG_ENABLE

#include <exception>
#include <string>

#include <boost/lexical_cast.hpp>
#include <string>

int BornanderLog::indentation = 0;
std::ostream* BornanderLog::stream = &std::cout;

BornanderLog::BornanderLog(const std::string& ctx_func, const std::string& ctx_file,
                           const int ctx_line)
  : context(std::string(ctx_func + " @ " + ctx_file + ":" + boost::lexical_cast<std::string>(ctx_line))), start_time(clock())
{
  write_indentation();
  *stream << "--> " << context << std::endl;
  ++indentation;
  stream->flush();
}

BornanderLog::~BornanderLog()
{
  --indentation;
  write_indentation(std::uncaught_exception() ? '*' : ' ');
  *stream << "<-- " << context;
  *stream << " in " << ((double)(clock() - start_time) / CLOCKS_PER_SEC) << "s";
  *stream << std::endl;
  stream->flush();
}

void BornanderLog::set_stream(std::ostream& stream)
{
  BornanderLog::stream = &stream;
}


void BornanderLog::write_indentation()
{
  write_indentation(' ');
}

void BornanderLog::write_indentation(const char prefix)
{
  *stream << prefix;
  for(int i = 0; i < indentation * 2; ++i)
  {
    *stream << " ";
  }
}

void BornanderLog::message(const std::string& message)
{
  write_indentation();
  *stream << message << std::endl;
  stream->flush();
}

#endif

