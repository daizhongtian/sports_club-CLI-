#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <string>
#include "member.h"
#include "team.h"

class Event {
private:
    std::string date;
    std::string location;
    std::string name;
    std::vector<Member*> participants;
    std::vector<Team*> teams;  

    int id;


public:
    Event(int id ,const std::string& date, const std::string& location, const std::string& name);

    void reschedule(const std::string& new_date);
    void addParticipant(Member* participant);
    void removeParticipant(Member* participant);

    std::string getDate() const;
    std::string getLocation() const;
    std::string getName() const;
    std::vector<Member*> getParticipants() const;

    void addTeam(Team* team);  
    void removeTeam(Team* team);  


   
    std::vector<Team*> getTeams() const;  


    bool operator==(const Event& other) const;
    size_t getParticipantCount() const;

    static Event* fromCsv(const std::string& line);

    std::string toCsv() const;


    int getId() const;

};

#endif // EVENT_H
