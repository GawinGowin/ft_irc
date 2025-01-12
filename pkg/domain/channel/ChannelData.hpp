#ifndef CHANNELDATA_HPP
#define CHANNELDATA_HPP

#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <ctime>
#include <cmath>

const std::string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
const int channelIdLength = 5;

class ChannelMode;

class ChannelData {
public:
    ChannelData(std::string name);
    ChannelData(const ChannelData &other);
    const std::string getName() const;
    const std::string getTopic() const;
    const std::string getChannelId() const;
    void setTopic(std::string topic);
private:
    ChannelData(); // not implemented
    ChannelData &operator=(const ChannelData &other);  // not implemented
    const std::string _name;
    const std::string _channelid;
    std::string _topic;
};

#endif /* CHANNELDATA_HPP */
