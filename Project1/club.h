#ifndef CLUB_H
#define CLUB_H

#include <vector>
#include <string>
#include "Member.h"
#include "Coach.h"
#include "Team.h"
#include "Event.h"
#include <memory>
#include <algorithm>
#include "CsvUtil.h"


enum class AppStatus { PENDING, APPROVED, REJECTED };


struct Application {
    int         id;
    Member* applicant;
    Event* event;
    std::string reason;
    AppStatus   status = AppStatus::PENDING;


    string toSCSv()const
    {
        return to_string(id) + "," + to_string(applicant->getId()) + "," + to_string(event->getId()) + "," + to_string(static_cast<int>(status)) + "," +
            reason;


    }

    static std::unique_ptr<Application>
        fromCsv(const string& line, Club& club)
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



};


class Club {
private:
    std::string name;
    std::vector<Member*> members;
    std::vector<Coach*> coaches;
    std::vector<Team*> teams;
    std::vector<Event*> events;




    int nextMemberId_ = 1;
    int nextCoachId_ = 1;
    int nextEventId_ = 1;

    const std::string membersFile_ = "members.csv";
    const std::string coachesFile_ = "coaches.csv";
    const std::string eventsFile_ = "events.csv";

    // �����������б�
    std::vector<std::unique_ptr<Application>> applications;
    int nextAppId = 1;

    Role currentRole = Role::ATHLETE;

    // CSV �־û�����
    void loadApplications();
    void saveApplications() const;





    void loadMembers();
    void loadCoaches();
    void loadEvents();
    void saveMembers()  const;
    void saveCoaches()  const;
    void saveEvents()   const;
    void loadApplications();
    void saveApplications() const;




public:
    explicit Club(const std::string& name);


 

    ~Club();


    // ��ɫ�л���
    void setCurrentCredential(Role r) { currentRole = r; }
    Role getCurrentRole() const { return currentRole; }

    // Athlete ���ã��ύ����
    Application* applyForEvent(Member* athlete, Event* ev, const std::string& reason = "");

    // Athlete ���ã��鿴�Լ�����
    std::vector<Application*> listMyApplications(Member* athlete) const;

    // Coach ���ã��鿴�����
    std::vector<Application*> listPendingApplications(Coach* coach) const;

    // Coach ���ã����
    void reviewApplication(Coach* coach, int appId, bool approve);


    const std::string applicationsFile = "applications.csv";





    void addMember(Member* member);
    void removeMember(Member* member);
    void addCoach(Coach* coach);
    void removeCoach(Coach* coach);
    void addTeam(Team* team);
    void removeTeam(Team* team);
    void organizeEvent(Event* event);
    void cancelEvent(Event* event);
    void addMembersToEvent(const std::string& eventName, const std::vector<Member*>& newMembers);
    void addTeamToEvent(const std::string& eventName, Team* team);  


    Member* findMemberByName(const std::string& name) const;
    std::vector<Member*> findMembersByRole(const std::string& role) const;
    Coach* findCoachByName(const std::string& name) const;
    void updateCoachSpecialty(const std::string& name, const std::string& new_specialty);
    bool hasScheduleConflict(const std::string& date) const;

    Member* findMemberById(int id) const;
    Coach* findCoachById(int id) const;
    void findPersonById(int id) const;
    Event* findEventById(int id) const;



    std::string getClubInfo() const;
    std::vector<Member*> getMembers() const;
    std::vector<Coach*> getCoaches() const;
    std::vector<Team*> getTeams() const;
    std::vector<Event*> getEvents() const;

    void saveAll() const;


};

#endif // CLUB_H
