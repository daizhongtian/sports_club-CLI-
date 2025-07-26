#ifndef CLUB_H
#define CLUB_H

#include <vector>
#include <string>
#include "member.h"
#include "coach.h"
#include "team.h"
#include "event.h"
#include <memory>
#include <algorithm>
#include "CsvUtil.h"
#include "auth.h"
#include <sstream>          

using namespace std;

class Club;


enum class AppStatus { PENDING, APPROVED, REJECTED };


struct Application {
    int id{ 0 };
    Member* applicant{ nullptr } ;
    Event* event{ nullptr };
    std::string reason;
    AppStatus   status = AppStatus::PENDING;


    string toCsv()const
    {
        return to_string(id) + "," + to_string(applicant->getId()) + "," + to_string(event->getId()) + "," + to_string(static_cast<int>(status)) + "," +
            reason;


    }

    static std::unique_ptr<Application>
    fromCsv(const std::string& line, Club& club);



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
    int nextTeamId_ = 1;

       

    const std::string membersFile_ = "members.csv";
    const std::string coachesFile_ = "coaches.csv";
    const std::string eventsFile_ = "events.csv";
    const std::string applicationsFile = "applications.csv";
    const std::string teamsFile_ = "teams.csv";



    // 新增：申请列表
    std::vector<std::unique_ptr<Application>> applications;
    int nextAppId = 1;

    Role currentRole = Role::ATHLETE;

    // CSV 持久化辅助
    void loadApplications();
    void saveApplications() const;


    void loadMembers();
    void loadCoaches();
    void loadEvents();
    void saveMembers()  const;
    void saveCoaches()  const;
    void saveEvents()   const;
    void loadTeams();
    void saveTeams() const;



public:
    explicit Club(const std::string& name);


 

    ~Club();


    // 角色切换用
    void setCurrentCredential(Role r) { currentRole = r; }
    Role getCurrentRole() const { return currentRole; }

    // Athlete 调用：提交申请
    Application* applyForEvent(Member* athlete, Event* ev, const std::string& reason = "");

    // Athlete 调用：查看自己申请
    std::vector<Application*> listMyApplications(Member* athlete) const;

    // Coach 调用：查看待审核
    std::vector<Application*> listPendingApplications(Coach* coach) const;

    // Coach 调用：审核
    void reviewApplication(Coach* coach, int appId, bool approve);







    void addMember(Member* member);
    void removeMember(Member* member);
    void addCoach(Coach* coach);


    void removeCoach(Coach* coach);
    


    void addTeam(Team* team);
    Team* createTeam(const std::string& sportType, Coach* coach);

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
    Team* findTeamById(int id) const;
    bool removeTeamById(int id);




    std::string getClubInfo() const;
    std::vector<Member*> getMembers() const;
    std::vector<Coach*> getCoaches() const;
    std::vector<Team*> getTeams() const;
    std::vector<Event*> getEvents() const;

    void saveAll() const;


};

#endif // CLUB_H
