#ifndef OPTIONSCONFIG_HPP
#define OPTIONSCONFIG_HPP

#include <string>

struct OptionsConfig {
  std::string AllowedChannelTypes;
  bool AllowRemoteOper;
  std::string ChrootDir;
  std::string CloakHost;
  std::string CloakHostModeX;
  std::string CloakHostSalt;
  bool CloakUserToNick;
  bool ConnectIPv6;
  bool ConnectIPv4;
  std::string DefaultUserModes;
  bool DNS;
  bool Ident;
  std::string IncludeDir;
  bool MorePrivacy;
  bool NoticeBeforeRegistration;
  bool OperCanUseMode;
  bool OperChanPAutoOp;
  bool OperServerMode;
  bool PAM;
  bool PAMIsOptional;
  std::string PAMServiceName;
  bool RequireAuthPing;
  bool ScrubCTCP;
  std::string SyslogFacility;
  std::string WebircPassword;
};

#endif /* OPTIONSCONFIG_HPP */
