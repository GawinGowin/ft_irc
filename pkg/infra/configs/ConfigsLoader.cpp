#include "infra/configs/ConfigsLoader.hpp"

// TODO: 使用していない値は削除する
ConfigsLoader::ConfigsLoader() {
  Configs &conf = this->_configs;
  // [Global]
  conf.Global.Name = "irc.example.net";
  conf.Global.AdminInfo1 = "Debian User";
  conf.Global.AdminInfo2 = "Debian City";
  conf.Global.AdminEMail = "irc@irc.example.com";
  conf.Global.HelpFile = "/usr/share/doc/ngircd/Commands.txt";
  conf.Global.Info = "Yet another IRC Server running on Debian GNU/Linux";
  conf.Global.Listen = "127.0.0.1";
  conf.Global.MotdFile = "/etc/ngircd/ngircd.motd";
  conf.Global.MotdPhrase = "Hello. This is the Debian default MOTD sentence";
  conf.Global.Network = "aIRCnetwork";
  conf.Global.Password = "wealllikedebian";
  conf.Global.PidFile = "/var/run/ngircd/ngircd.pid";
  conf.Global.Port = 6667;
  conf.Global.ServerGID = "irc";
  conf.Global.ServerUID = "irc";

  // [Limits]
  conf.Limits.ConnectRetry = 60;
  conf.Limits.IdleTimeout = 0;
  conf.Limits.MaxConnections = 500;
  conf.Limits.MaxConnectionsIP = 10;
  conf.Limits.MaxJoins = 10;
  conf.Limits.MaxNickLength = 9;
  conf.Limits.MaxPenaltyTime = -1;
  conf.Limits.MaxListSize = 100;
  conf.Limits.PingTimeout = 120;
  conf.Limits.PongTimeout = 20;
  conf.Limits.MaxBufferSize = 1024;

  // [Options]
  conf.Options.AllowedChannelTypes = "#&+";
  conf.Options.AllowRemoteOper = false;
  conf.Options.ChrootDir = "/var/empty";
  conf.Options.CloakHost = "cloaked.host";
  conf.Options.CloakHostModeX = "cloaked.user";
  conf.Options.CloakHostSalt = "abcdefghijklmnopqrstuvwxyz";
  conf.Options.CloakUserToNick = true;
  conf.Options.ConnectIPv6 = true;
  conf.Options.ConnectIPv4 = true;
  conf.Options.DefaultUserModes = "i";
  conf.Options.DNS = true;
  conf.Options.Ident = true;
  conf.Options.IncludeDir = "/etc/ngircd/conf.d";
  conf.Options.MorePrivacy = false;
  conf.Options.NoticeBeforeRegistration = false;
  conf.Options.OperCanUseMode = true;
  conf.Options.OperChanPAutoOp = true;
  conf.Options.OperServerMode = false;
  conf.Options.PAM = false;
  conf.Options.PAMIsOptional = false;
  conf.Options.PAMServiceName = "ngircd";
  conf.Options.RequireAuthPing = false;
  conf.Options.ScrubCTCP = false;
  conf.Options.SyslogFacility = "local1";
  conf.Options.WebircPassword = "xyz";

  // [Operator]
  conf.Operators.push_back(OperatorConfig("TheOper", "ThePwd", "*!ident@somewhere.example.com"));

  // [Channel]
  conf.Channels.push_back(ChannelConfig(
      "#ngircd", "Our ngircd testing channel", "+tnk mykey +l 5", "/etc/ngircd/#chan.key"));
}

ConfigsLoader::~ConfigsLoader() {}

ConfigsLoader::ConfigsLoader(const Configs &configs) { this->_configs = configs; }

ConfigsLoader &ConfigsLoader::operator=(const ConfigsLoader &other) {
  if (this != &other) {
    this->_configs = other._configs;
  }
  return *this;
}

const Configs &ConfigsLoader::getConfigs(void) const { return this->_configs; }

void ConfigsLoader::setConfigs(const Configs &configs) { this->_configs = configs; }

void ConfigsLoader::setPort(const int &port) { this->_configs.Global.Port = port; }

void ConfigsLoader::setPassword(const std::string &password) {
  this->_configs.Global.Password = password;
}
