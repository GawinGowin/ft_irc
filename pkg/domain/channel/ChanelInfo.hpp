#ifndef CHANELINFO_HPP
#define CHANELINFO_HPP

#include <string>

class Channel {
public:
    Channel();
    Channel(const Channel &other);
    Channel &operator=(const Channel &other);
    Channel(std::string name, std::string topic);
    std::string getName() const;
    std::string getTopic() const;
private:
    std::string _name;
    std::string _topic;
};

#endif /* CHANELINFO_HPP */
