#include "club.h"
#include <algorithm>
#include <iostream>

#include <vector>
#include<string>
#include <sstream>
#include <fstream>
#include "member.h"
#include "CsvUtil.h"
#include<memory>

using namespace std;

// Constructor to initialize the club with a given name
Club::Club(const std::string& name) : name(name) 
{
    loadMembers();
    loadCoaches();
    loadEvents();
    loadApplications();
    loadTeams();
}

// Destructor to clean up all dynamically allocated memory
Club::~Club() {
    // Delete all members
    for (auto member : members) {
        delete member;
    }
    members.clear();

    // Delete all coaches
    for (auto coach : coaches) {
        delete coach;
    }
    coaches.clear();

    // Delete all teams
    for (auto team : teams) {
        delete team;
    }
    teams.clear();

    // Delete all events
    for (auto event : events) {
        delete event;              //controversial with test.cpp
    }
    events.clear();
    saveAll();

}

// Add a member to the club
void Club::addMember(Member* member) {
    // Check if the member with the same ID already exists
    for (const auto& existing_member : members) 
    {
        

        if (existing_member->getId() == member->getId())
            throw std::invalid_argument("Duplicate member ID");

    }
    members.push_back(member);
}

// Remove a member from the club
void Club::removeMember(Member* member) {
    std::cout << "Attempting to remove member: " << member->getName() << std::endl;
    auto it = std::find(members.begin(), members.end(), member);
    if (it != members.end()) {
        // Remove the member from all teams
        std::cout << "Removing member from teams..." << std::endl;
        for (auto& team : teams) {
            team->removeMember(member);
        }

        // Remove the member from all events
        std::cout << "Removing member from events..." << std::endl;
        for (auto& event : events) {
            event->removeParticipant(member);
        }

        // Delete the member object and remove the pointer from the vector
        std::cout << "Deleting member object..." << std::endl;
        Member* toDelete = *it;
        members.erase(it);
      
        std::cout << "Removed and deleted member: " << member->getName() << std::endl;
        delete toDelete;

    }
    else {
        std::cout << "Member not found in club: " << member->getName() << std::endl;
    }
}



// Add a coach to the club
void Club::addCoach(Coach* coach) {
    // Check if the coach with the same ID already exists
    for (const auto& existing_coach : coaches) {
        if (*existing_coach == *coach) {
            throw std::invalid_argument("Coach with this ID already exists in the club");
        }
    }
    coaches.push_back(coach);
}

// Remove a coach from the club
void Club::removeCoach(Coach* coach) {
    auto it = std::find(coaches.begin(), coaches.end(), coach);
    if (it == coaches.end()) {
        cout << "Coach not found: " << coach->getName() << std::endl;
        return;
    }
    for (auto* t : teams)
    {
        if (t->getCoach() == coach)
        {
            t->removeCoach();
        }
    }

    coaches.erase(it);
    string name = coach->getName();
    delete coach;



}

// Add a team to the club
void Club::addTeam(Team* team) 

 {
    for (const auto& existing : teams)
    {
        if (existing->getId() == team->getId())
        {
            throw std::invalid_argument("Team with this ID already exists in the club");
        }
    }

       teams.push_back(team);
       nextTeamId_ = max(nextTeamId_, team->getId() + 1);

}


Team* Club::createTeam(const string& sportType, Coach* coach)
{
    Team* t = new Team(sportType, coach, nextTeamId_++);
    teams.push_back(t);
    return t;
}

void Club::loadTeams()
{
    ifstream fin(teamsFile_);
    if (!fin) return;
    string line;

    while (getline(fin, line))
    {
        if (line.empty()) continue;
        istringstream iss(line);
        string token;

        getline(iss, token, ',');
        int id = std::stoi(token);
        string sport;
        getline(iss, sport, ',');
        getline(iss, token, ',');
        int coachId = std::stoi(token);
        Coach* coach = findCoachById(coachId);
        Team* team = new Team(sport, coach, id);
        teams.push_back(team);
        nextTeamId_ = std::max(nextTeamId_, id + 1);
    }
}




// Remove a team from the club
void Club::removeTeam(Team* team) {
    auto it = std::find(teams.begin(), teams.end(), team);
    if (it != teams.end()) {
        // Remove the team from all events
        for (auto& event : events) {
            event->removeTeam(team);
        }

        // Delete the team object and remove the pointer from the vector
        Team* toDelete = *it;
        teams.erase(it);
        delete toDelete;
    }
}

// Organize a new event in the club
void Club::organizeEvent(Event* event) {
    events.push_back(event);
}

// Cancel an event in the club
void Club::cancelEvent(Event* event) {
    if (event == nullptr) {
        throw std::invalid_argument("empty point");
    }
    auto it = std::find(events.begin(), events.end(), event);
    if (it == events.end()) {
        throw std::runtime_error("empty event");
    }
    delete* it;
    events.erase(it);
    saveAll();
}


// Add members to an event by event name
void Club::addMembersToEvent(const std::string& eventName, const std::vector<Member*>& newMembers) {
    for (auto& event : events) {
        if (event->getName() == eventName) {
            for (auto& member : newMembers) {
                event->addParticipant(member);
            }
        }
    }
}

// Add a team to an event by event name
void Club::addTeamToEvent(const std::string& eventName, Team* team) {
    for (auto& event : events) {
        if (event->getName() == eventName) {
            event->addTeam(team);
        }
    }
}

// Get the name of the club
std::string Club::getClubInfo() const {
    return name;
}

// Get the list of members in the club
std::vector<Member*> Club::getMembers() const {
    return members;
}

// Get the list of coaches in the club
std::vector<Coach*> Club::getCoaches() const {
    return coaches;
}

// Get the list of teams in the club
std::vector<Team*> Club::getTeams() const {
    return teams;
}

// Get the list of events in the club
std::vector<Event*> Club::getEvents() const {
    return events;
}

// Find a member by name
Member* Club::findMemberByName(const std::string& name) const {
    for (const auto& member : members) {
        if (member->getName() == name) {
            return member;
        }
    }
    return nullptr;
}

// Find members by role
std::vector<Member*> Club::findMembersByRole(const std::string& role) const {
    std::vector<Member*> result;
    for (const auto& member : members) {
        if (member->getRole() == role) {
            result.push_back(member);
        }
    }
    return result;
}

// Find a coach by name
Coach* Club::findCoachByName(const std::string& name) const {
    for (const auto& coach : coaches) {
        if (coach->getName() == name) {
            return coach;
        }
    }
    return nullptr;
}

// Find a member by ID
Member* Club::findMemberById(int id) const {
    for (const auto& member : members) {
        if (member->getId() == id) {
            return member;
        }
    }
    return nullptr;
}

// Find a coach by ID
Coach* Club::findCoachById(int id) const {
    for (const auto& coach : coaches) {
        if (coach->getId() == id) {
            return coach;
        }
    }
    return nullptr;
}

// Find a person (either member or coach) by ID and print their details
void Club::findPersonById(int id) const {
    Member* member = findMemberById(id);
    if (member != nullptr) {
        std::cout << "Member found: " << std::endl;
        std::cout << "Name: " << member->getName() << std::endl;
        std::cout << "Age: " << member->getAge() << std::endl;
        std::cout << "Role: " << member->getRole() << std::endl;
        std::cout << "ID: " << member->getId() << std::endl;
        return;
    }

    Coach* coach = findCoachById(id);
    if (coach != nullptr) {
        std::cout << "Coach found: " << std::endl;
        std::cout << "Name: " << coach->getName() << std::endl;
        std::cout << "Specialty: " << coach->getSpecialty() << std::endl;
        std::cout << "ID: " << coach->getId() << std::endl;
        return;
    }

    std::cout << "No person found with ID: " << id << std::endl;
}


Event* Club::findEventById(int id)const
{
    for (size_t i = 0; i < events.size(); ++i)
    {

        if (events[i]->getId() == id)
        {
            return events[i];
        }
    }
    return nullptr;
}


// Update the specialty of a coach by name
void Club::updateCoachSpecialty(const std::string& name, const std::string& new_specialty) {
    auto coach = findCoachByName(name);
    if (coach) {
        coach->setSpecialty(new_specialty);
    }
}

// Check if there is a schedule conflict for a given date
bool Club::hasScheduleConflict(const std::string& date) const {
    for (const auto& event : events) {
        if (event->getDate() == date) {
            return true;
        }
    }
    return false;
}


Team* Club::findTeamById(int id) const {
    for (auto team : teams) {
        if (team->getId() == id) {
            return team;
        }
    }
    return nullptr; 
}

bool Club::removeTeamById(int id) {
    auto it = std::find_if(teams.begin(), teams.end(),
        [id](Team* t) { return t->getId() == id; });
    if (it == teams.end()) return false;
    delete* it;              
    teams.erase(it);         
    return true;
}




static vector<string>splitcsv(const string& line)
{
    vector<string >out;
    istringstream iss(line);

    string token;
    while (getline(iss, token, ','))
    {
        out.push_back(token);
    }

    return out;
}



//load

void Club::loadMembers()
{
    loadEntities<Member>(membersFile_, members, nextMemberId_);
}

void Club::loadCoaches()

{
    loadEntities<Coach>(coachesFile_, coaches, nextCoachId_);
}

void Club::loadEvents() {
    loadEntities<Event>(eventsFile_, events, nextEventId_);
}



//save

void Club::saveMembers() const {
    SaveEntities<Member>(membersFile_, members);
}
void Club::saveCoaches() const {
    SaveEntities<Coach>(coachesFile_, coaches);
}
void Club::saveEvents() const {
    SaveEntities<Event>(eventsFile_, events);
}



void Club::saveTeams() const {
    std::ofstream fout(teamsFile_, std::ios::trunc);
    if (!fout.is_open()) return;
    for (auto* t : teams) {
        int cid = t->getCoach() ? t->getCoach()->getId() : 0;
        fout << t->getId() << "," << t->getSportType() << "," << cid << "\n";
    }
}


void Club::saveAll() const {
    saveMembers();
    saveCoaches();
    saveEvents();
    saveTeams();

}


void Club::loadApplications()
{
    applications.clear();          

    std::ifstream fin(applicationsFile);
    if (!fin) return;              

    std::string line;
    while (std::getline(fin, line))
    {
        if (line.empty()) continue;

   
        auto app = Application::fromCsv(line, *this);

     
        nextAppId = std::max(nextAppId, app->id + 1);

   
        applications.emplace_back(std::move(app));
    }
}




    void Club::saveApplications() const
    {  
        ofstream fout(applicationsFile, ios::trunc);  
        if (!fout.is_open())  
        {  
            return;  
        }  

        for (size_t i = 0; i < applications.size(); ++i)  
        {  
            const unique_ptr<Application>& appPtr = applications[i]; // Define appPtr as a reference to the unique_ptr  
            fout << appPtr->toCsv() << "\n";
        }  
    }

    Application* Club::applyForEvent(Member* athlete, Event* ev, const string& reason)
    {
        unique_ptr<Application>appPtr(new Application());

        appPtr->id = nextAppId++;
		appPtr->applicant = athlete;
        appPtr->event = ev;
        appPtr->status = AppStatus::PENDING;
        appPtr->reason = reason;
        applications.push_back(std::move(appPtr));
        saveApplications();
		return applications.back().get();

    }


    std::vector<Application*> Club::listMyApplications(Member* athlete) const


    {
        vector<Application*>result;
        for (size_t i = 0; i < applications.size(); ++i)
        {
            const unique_ptr<Application>& appPtr = applications[i];

            if (appPtr->applicant == athlete)
            {
                result.push_back(appPtr.get());
            }
        }

        return result;
	}


    vector<Application*>Club::listPendingApplications(Coach*)const
    {
        vector<Application*>result;
        for (size_t i = 0; i < applications.size(); ++i)
        {
            const unique_ptr<Application>& appPtr = applications[i];
            if (appPtr->status == AppStatus::PENDING)
            {
                result.push_back(appPtr.get());
            }
        }
		return result;
    }

    void Club::reviewApplication(Coach* coach, int appId, bool approve) {
        for (auto& app : applications) {
            if (app->id == appId) 
            {
                // only for pending
                if (app->status != AppStatus::PENDING) {
                    throw std::invalid_argument("Application already reviewed");
                }


                if (approve) {
                    app->status = AppStatus::APPROVED;
                } else {
                    app->status = AppStatus::REJECTED;
                }
                saveApplications();
                return;
            }
        }
        throw std::invalid_argument("Application not found");

	}


    std::unique_ptr<Application>
     Application::fromCsv(const std::string& line, Club& club)
    {
        istringstream iss(line);

        string tok;

        auto app = make_unique<Application>();


        getline(iss, tok, ','); app->id = std::stoi(tok);
        getline(iss, tok, ','); app->applicant = club.findMemberById(std::stoi(tok));
        getline(iss, tok, ','); app->event = club.findEventById(std::stoi(tok));
        getline(iss, tok, ','); app->status = static_cast<AppStatus>(std::stoi(tok));
        getline(iss, app->reason);

        return app;
    }