#include "pch.h"  
#include "../../Project1/Project1/auth.h"
#include "../../Project1/Project1/auth.cpp"

#include <fstream>  
#include <gtest/gtest.h>   
#include "../../Project1/Project1/club.h"
#include "../../Project1/Project1/member.h"
#include "../../Project1/Project1/event.h"
#include "../../Project1/Project1/coach.h"

#include "../../Project1/Project1/club.cpp"
#include "../../Project1/Project1/member.cpp"
#include "../../Project1/Project1/event.cpp"
#include "../../Project1/Project1/coach.cpp"
#include "../../Project1/Project1/team.cpp"

#include"../../Project1/Project1/CsvUtil.h"
#include <string>
using namespace std;  
TEST(TestCaseName, TestName) {  
  EXPECT_EQ(1, 1);  
  EXPECT_TRUE(true);  
}  


class AuthTest : public ::testing::Test  
{  
    const string dataFile = "credentials.txt";

    void SetUp() override  
    {  
        std::remove(dataFile.c_str());
    }  

    void TearDown() override 
    {

        std::remove(dataFile.c_str());
    }


};


//empty file


TEST_F(AuthTest, LoadNonexistentFile) {
    EXPECT_FALSE(loadcredentials());
    auto all = getAllCredentials();
    EXPECT_TRUE(all.empty());
}


// Register ¡ú Login ¡ú Logout ¡ú Delete 

TEST_F(AuthTest, RegisterLoginLogoutDelete)
{
    //login before register
    EXPECT_FALSE(login("alice", "alice_password"));

    //register new user
    EXPECT_TRUE(registeruser("alice", "alice_password", Role::ATHLETE));

    //same name 
    EXPECT_FALSE(registeruser("alice", "alice_password", Role::ATHLETE));

    //check current user (only alice)
    {
        auto all = getAllCredentials();

      

        ASSERT_EQ(all.size(), 1u);
        EXPECT_EQ(all[0].username,"alice");
        EXPECT_EQ(all[0].password, "alice_password");
        EXPECT_EQ(all[0].role, Role::ATHLETE);


    }
    //wrong password
    EXPECT_FALSE(login("alice", "wrong"));

    EXPECT_TRUE(login("alice", "alice_password"));


    // alice login 
    {
        Credential* cur = getCurrentCredential();
        ASSERT_NE(cur, nullptr);
        EXPECT_EQ(cur->username, "alice");
        EXPECT_EQ(cur->role, Role::ATHLETE);

    }
    logout();

    EXPECT_EQ(getCurrentCredential(), nullptr);

    EXPECT_TRUE(deleteuser("alice"));
    EXPECT_FALSE(login("alice", "alice_password"));
    EXPECT_TRUE(getAllCredentials().empty());



}





// Register two users and save to disk
// Register a third user and confirm that there are 3 in memory
// After calling loadcredentials(), the memory returns to only the first two users and can log in

TEST_F(AuthTest, SaveLoad)
{


    EXPECT_TRUE(registeruser("u1", "p1", Role::ADMIN));
    EXPECT_TRUE(registeruser("u2", "p2", Role::COACH));

    {
        auto before = getAllCredentials();
        ASSERT_EQ(before.size(), 2u);

    }

    EXPECT_TRUE(savecredentials());


    EXPECT_TRUE(registeruser("u3", "p3", Role::ATHLETE));
    {
        auto mid = getAllCredentials();
        ASSERT_EQ(mid.size(), 3u);


    }


    EXPECT_TRUE(loadcredentials());
    {
        auto after = getAllCredentials();
        ASSERT_EQ(after.size(), 3u);
        EXPECT_EQ(after[0].username, "u1");
        EXPECT_EQ(after[1].username, "u2");
    }


    // log in these 2 user
    EXPECT_TRUE(login("u1", "p1"));
    logout();
    EXPECT_TRUE(login("u2", "p2"));
    logout();
}




class ClubAppTest :public::testing::Test 
{
protected:
    Club* club = nullptr;
    Member* alice = nullptr;
    Member* bob = nullptr;
    Coach* coach = nullptr;
    Event* ev = nullptr;
    string appsFile = "applications.csv";


    void SetUp()override
    {

        {
            std::fstream fs(appsFile,
                std::ios::in    
                | std::ios::out   
                | std::ios::trunc 
            );
            
        }


        club = new Club("TestClub");

        alice = new Member("Alice", 22, "Athlete", 1);
        bob = new Member("Bob", 25, "Athlete", 2);
        coach = new Coach("CoachX", "football", 100);
        ev = new Event(10,"TestEvent", "2025-08-01", "TestVenue");
    }

    void TearDown()override
    {
        delete club;
        delete alice;
        delete bob;
        delete ev;
        delete coach;



     
       
    }


};

TEST_F(ClubAppTest, application)
{
    //empty application list for member and coach 
    EXPECT_TRUE(club->listMyApplications(alice).empty());
    EXPECT_TRUE(club->listPendingApplications(coach).empty());

    Application* app = club->applyForEvent(alice, ev, "Need to train");
    ASSERT_NE(app, nullptr);

    //alice application list 
    auto myApps = club->listMyApplications(alice);
    ASSERT_EQ(myApps.size(), 1u); //only 1 application

    EXPECT_EQ(myApps[0], app);//same point for listmyapplications and applyforvent

    EXPECT_EQ(app->applicant, alice);
    EXPECT_EQ(app->event, ev);
    EXPECT_EQ(app->status, AppStatus::PENDING);
    EXPECT_EQ(app->reason, "Need to train");

    auto pend = club->listPendingApplications(coach);
    ASSERT_EQ(pend.size(), 1u);
    EXPECT_EQ(pend[0], app);


}


//  Status changes after approval & removal from pending approval list

TEST_F(ClubAppTest, ReviewApplication_Approve)
{
    //new application 
    Application* app1 = club->applyForEvent(alice, ev, "Need to train");
    ASSERT_NE(app1, nullptr);

    auto pend0 = club->listPendingApplications(coach);
    ASSERT_EQ(pend0.size(), 1u);


    EXPECT_NO_THROW(club->reviewApplication(coach ,app1->id,true));

    auto myApps = club->listMyApplications(alice);
    ASSERT_EQ(myApps.size(), 1u);
    EXPECT_EQ(myApps[0]->status, AppStatus::APPROVED);
    EXPECT_TRUE(club->listPendingApplications(coach).empty());

}

//reject
TEST_F(ClubAppTest, ReviewApplication_Reject) 
{
    Application* app2 = club->applyForEvent(bob, ev, "NotReady");
    ASSERT_NE(app2, nullptr);
    EXPECT_NO_THROW(club->reviewApplication(coach,app2->id, false));
    auto myapps = club->listMyApplications(bob);
    ASSERT_EQ(myapps.size(), 1u);
    EXPECT_EQ(myapps[0]->status, AppStatus::REJECTED);
    EXPECT_TRUE(club->listPendingApplications(coach).empty());


}




 //same id,and no exist id 
TEST_F(ClubAppTest, ReviewApplication_InvalidThrows)
{
    Application* app1 = club->applyForEvent(alice, ev, "Train");
    ASSERT_NE(app1, nullptr);



    EXPECT_NO_THROW(club->reviewApplication(coach,app1->id,true));
    EXPECT_THROW(club->reviewApplication(coach, app1->id, false), std::invalid_argument);
    EXPECT_THROW(club->reviewApplication(coach,9999, true), std::invalid_argument);


}


TEST_F(ClubAppTest, MultipleMembersApplications) 
{
    Application* a1 = club->applyForEvent(alice, ev, "A");
    Application* b1 = club->applyForEvent(bob, ev, "B");
    ASSERT_NE(a1, nullptr);
    ASSERT_NE(b1, nullptr);
    auto aliceApps = club->listMyApplications(alice);
    auto bobApps = club->listMyApplications(bob);
    EXPECT_EQ(aliceApps.size(), 1u);
    EXPECT_EQ(bobApps.size(), 1u);
    EXPECT_EQ(aliceApps[0], a1);
    EXPECT_EQ(bobApps[0], b1);

    auto pend = club->listPendingApplications(coach);
    EXPECT_EQ(pend.size(), 2u);

    EXPECT_NE(std::find(pend.begin(), pend.end(), a1), pend.end());
    EXPECT_NE(std::find(pend.begin(), pend.end(), b1), pend.end());

}

//check id 


TEST_F(ClubAppTest, ApplicationIdIncrement) {
    Application* a1 = club->applyForEvent(alice, ev, "first");
    Application* a2 = club->applyForEvent(alice, ev, "second");
    ASSERT_NE(a1, nullptr);
    ASSERT_NE(a2, nullptr);
    EXPECT_LT(a1->id, a2->id) << "a2 should > a1";
}





class CsvTest :public::testing::Test
{
public:
    const std::string filePath = "csv_test.txt";

    void SetUp()override
    {
        ofstream ofs(filePath, ios::trunc);

        ofstream m("members.csv", ios::trunc);
        ofstream c("coaches.csv", ios::trunc);
        ofstream e("events.csv", ios::trunc);
        ofstream a("applications.csv", ios::trunc);
        ofstream t("teams.csv", ios::trunc);
    }

protected:
    void TearDown()override
    {
        std::ofstream ofs(filePath, std::ios::out | std::ios::trunc);

        std::ofstream m("members.csv", std::ios::out | std::ios::trunc);
        std::ofstream c("coaches.csv", std::ios::out | std::ios::trunc);
        std::ofstream e("events.csv", std::ios::out | std::ios::trunc);
        std::ofstream a("applications.csv", std::ios::out | std::ios::trunc);
        std::ofstream t("teams.csv", std::ios::out | std::ios::trunc);
    }

    

};




//member
TEST_F(CsvTest, MemberCsv)
{
    Member m1("alice", 28, "Athlete", 100);
    string line = m1.toCsv();
    Member* m2 = Member::fromCsv(line);
    EXPECT_TRUE(m1 == *m2);
    delete m2;
}

//coach
TEST_F(CsvTest, CoachCsvRoundTrip) {
    Coach c1("Bob", "Tennis", 200);
    std::string line = c1.toCsv();
    Coach *c2 = Coach::fromCsv(line);
    EXPECT_TRUE(c1 == *c2);
    delete c2;
}


//event


TEST_F(CsvTest, EventCsv)
{
    Event e1 (10,"2025-07-01", "park", "game");
    string line = e1.toCsv();
    Event* e2 = Event::fromCsv(line);
    EXPECT_EQ(e1.getDate(), e2->getDate());
    EXPECT_EQ(e1.getLocation(), e2->getLocation());
    EXPECT_EQ(e1.getName(), e2->getName());
    delete e2;


}


//application

TEST_F(CsvTest, ApplicationCsv)
{
    Club club("CSVClub");
    Member* m = new Member("Cathy", 30, "Athlete", 10);
    Event* e = new Event(11,"2025-08-08", "Arena", "Final");
    club.addMember(m);
    club.organizeEvent(e);

    Application app;

    app.id = 5;
    app.applicant = m;
    app.event = e;
    app.status = AppStatus::APPROVED;
    app.reason = "All good";


    string line = app.toCsv();
    unique_ptr<Application> app2 = Application::fromCsv(line, club);
    EXPECT_EQ(app.id, app2->id);
    EXPECT_EQ(app.applicant, app2->applicant);
    EXPECT_EQ(app.event, app2->event);
    EXPECT_EQ(app.status, app2->status);
    EXPECT_EQ(app.reason, app2->reason);

}

//missing error

//missing member filed
TEST_F(CsvTest, MemberCsv_error)

{
    EXPECT_THROW(Member::fromCsv("1,OnlyName,NoAge"), std::exception);
}

//missing coach filed
TEST_F(CsvTest, CoachCsvMalformed) {
    EXPECT_THROW(Coach::fromCsv("2,Name,Sport,Extra"), std::exception);
}



TEST_F(CsvTest, ApplicationCsvMalformed) {
    Club club("BadApp");

    //no reason
    EXPECT_THROW(
        { Application::fromCsv("1,99,99,0", club); },
        std::exception
    );



}


//  save/load


TEST_F(CsvTest, CsvUtilRoundTrip) 
{
    vector<Member*> members;
    members.push_back(new Member("X", 1, "Role", 1));
    members.push_back(new Member("Y", 2, "Role", 2));
    int nextId = 0;

    SaveEntities<Member>(filePath, members);

    for (auto* m : members)
    {
        delete m;

    }
    members.clear();
     
    loadEntities<Member>(filePath, members, nextId);

    ASSERT_EQ(members.size(), 2u);
    EXPECT_EQ(members[0]->getId(), 1);
    EXPECT_EQ(members[1]->getId(), 2);


    for (auto* m : members) {
        delete m;
    }
    members.clear();

}






class TeamOpTest : public ::testing::Test {
protected:
    std::string csvPath;

    void SetUp() override {
        csvPath = "teams_unit_test.csv";
        std::ofstream(csvPath, std::ios::trunc).close();
    }
    void TearDown() override {
        std::remove(csvPath.c_str());
    }
    
    static size_t lineCount(const std::string& file) {
        std::ifstream ifs(file);
        return std::count(std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>(), '\n');
    }
};



TEST_F(TeamOpTest, CreateTeamAndPersist_WithAutoId) {
    Club club("MyClub");
    Coach* coach = new Coach("Zoe", "Basketball", 42);
    club.addCoach(coach);

    Team* t = club.createTeam("Basketball", coach);
    ASSERT_NE(t, nullptr);
    int assignedId = t->getId();

    SaveEntities<Team>(csvPath, club.getTeams());
    EXPECT_EQ(lineCount(csvPath), 1u);

    std::ifstream ifs(csvPath);
    std::string line;
    ASSERT_TRUE(std::getline(ifs, line));

    std::string sid = std::to_string(assignedId);
    EXPECT_EQ(line.substr(0, sid.size()), sid);

    EXPECT_NE(line.find("Basketball"), std::string::npos);


}

TEST_F(TeamOpTest, RemoveCoachAndPersist) {
    Coach* coachB = new Coach("Chris", "Swim", 99);
    Team team("Swim", coachB, 701);
    EXPECT_EQ(team.getCoach(), coachB);

    team.removeCoach();                      
    EXPECT_EQ(team.getCoach(), nullptr);

    std::vector<Team*> vec{ &team };
    SaveEntities<Team>(csvPath, vec);

    std::ifstream ifs(csvPath); std::string line; std::getline(ifs, line);

    EXPECT_EQ(line.find(",99,"), std::string::npos);

    delete coachB;
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}