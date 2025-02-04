#ifndef LIMITSCONFIG_HPP
#define LIMITSCONFIG_HPP

struct LimitsConfig {
  int ConnectRetry;
  int IdleTimeout;
  int MaxConnections;
  int MaxConnectionsIP;
  int MaxJoins;
  int MaxNickLength;
  int MaxPenaltyTime;
  int MaxListSize;
  int PingTimeout;
  int PongTimeout;
};

#endif /* LIMITSCONFIG_HPP */
