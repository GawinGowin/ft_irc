#ifndef CHANELDATA_HPP
#define CHANELDATA_HPP

#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

typedef std::list<std::string> CharsListSpecified;

static const int max_length = 50;
static const std::string delayed = "#";
static const std::string local = "&";
static const std::string mode_disabled = "+";
static const std::string channel_creator = "!";
static const CharsListSpecified forbiddenChars({":", ",", " ", "\a"});
static const CharsListSpecified channelNameInitialChars({delayed, local, mode_disabled, channel_creator});

class Channel {
public:
    Channel(std::string name);
    Channel(const Channel &other);
    std::string getName() const;
    std::string getMode() const;
private:
    Channel();
    Channel &operator=(const Channel &other);
    const std::string _name;
    const std::string _mode; // TODO: モードのフラグだが、enumにしたい。しかし、enumはstd::stringを扱えないので、どうするか？
};

#endif /* CHANELDATA_HPP */
