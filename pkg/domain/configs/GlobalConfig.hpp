#ifndef GLOBALCONFIG_HPP
#define GLOBALCONFIG_HPP

#include <string>
#include <vector>

struct GlobalConfig {
  std::string Name;
  std::string AdminInfo1;
  std::string AdminInfo2;
  std::string AdminEMail;
  std::string HelpFile;
  std::string Info;
  std::string Listen;
  std::string MotdFile;
  std::string MotdPhrase;
  std::string Network;
  std::string Password;
  std::string PidFile;
  int Port;
  std::string ServerGID;
  std::string ServerUID;
};

#endif /* GLOBALCONFIG_HPP */
