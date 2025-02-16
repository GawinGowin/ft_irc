#ifndef ILOGGERSTREAM_HPP
#define ILOGGERSTREAM_HPP

#include "infra/logger/LogStream.hpp"

class ILoggerStream {
  virtual LogStream tracess(void) = 0;
  virtual LogStream debugss(void) = 0;
  virtual LogStream infoss(void) = 0;
  virtual LogStream warningss(void) = 0;
  virtual LogStream errorss(void) = 0;
};

#endif /* ILOGGERSTREAM_HPP */
