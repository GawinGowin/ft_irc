#ifndef CHANELINFO_HPP
#define CHANELINFO_HPP

#include <string>
#include <stdexcept>
#include <list>

class Channel {
public:
    Channel(std::string name, std::string topic);
    Channel(const Channel &other);
    Channel &operator=(const Channel &other);
    std::string getName() const;
    std::string getTopic() const;
private:
    Channel();
    std::string _name;
    std::string _topic;
};

#endif /* CHANELINFO_HPP */
