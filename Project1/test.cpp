// -------------------------  test.cpp  -------------------------
#include <cassert>
#include <iostream>
#include <algorithm>      // for std::find
#include "member.h"
#include "coach.h"
#include "team.h"
#include "event.h"
#include "club.h"

// 生成唯一 Event ID（简单自增）
static int g_evtId = 1;
inline int nextEventId() { return g_evtId++; }



// ------------------------------------------------------------------
// 1. Member tests
// ------------------------------------------------------------------
void testMember() {
    try {
        Member* m1 = new Member("John", 30, "Athlete", 1);

        assert(m1->getName() == "John");
        assert(m1->getAge() == 30);
        assert(m1->getRole() == "Athlete");
        assert(m1->getId() == 1);

        m1->updateDetails("John D.", 31);
        assert(m1->getName() == "John D." && m1->getAge() == 31);

        try {
            m1->updateDetails("", 31);
            std::cerr << "testMember failed: no exception on empty name\n";
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Caught expected exception for empty name: " << e.what() << '\n';
        }

        try {
            m1->updateDetails("John D.", -5);
            std::cerr << "testMember failed: no exception on negative age\n";
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Caught expected exception for negative age: " << e.what() << '\n';
        }

        try {
            Member dummy("Jane", 25, "Athlete", -1);
            std::cerr << "testMember failed: no exception on negative ID\n";
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Caught expected exception for negative ID: " << e.what() << '\n';
        }

        std::cout << "testMember passed\n";
        delete m1;
    }
    catch (const std::exception& e) {
        std::cerr << "testMember failed: " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "testMember failed: unknown exception\n";
    }
}

// ------------------------------------------------------------------
// 2. Coach tests
// ------------------------------------------------------------------
void testCoach() {
    try {
        Coach* c1 = new Coach("Jane", "Football", 1);
        assert(c1->getName() == "Jane");
        assert(c1->getSpecialty() == "Football");
        assert(c1->getId() == 1);

        c1->setSpecialty("Basketball");
        assert(c1->getSpecialty() == "Basketball");

        try {
            Coach* c2 = new Coach("John", "", 2);
            std::cerr << "testCoach failed: no exception on empty specialty\n";
            delete c2;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Caught expected exception for empty specialty: " << e.what() << '\n';
        }

        try {
            Coach dummy("John", "Football", -1);
            std::cerr << "testCoach failed: no exception on negative ID\n";
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Caught expected exception for negative ID: " << e.what() << '\n';
        }

        std::cout << "testCoach passed\n";
        delete c1;
    }
    catch (...) {
        std::cout << "testCoach failed\n";
    }
}

// ------------------------------------------------------------------
// 3. Team tests
// ------------------------------------------------------------------
void testTeam() {
    // Add member
    try {
        Coach* coach = new Coach("Jane", "Football", 1);
        Team team("Football", coach, 1);
        Member* m = new Member("John", 30, "Athlete", 1);
        team.addMember(m);
        assert(team.getMembers().size() == 1);
        std::cout << "testAddingMember passed\n";
        delete m;
        delete coach;
    }
    catch (...) {
        std::cout << "testAddingMember failed\n";
    }

    // Remove member
    try {
        Coach* coach = new Coach("Jane", "Football", 1);
        Team team("Football", coach, 1);
        Member* m = new Member("John", 30, "Athlete", 1);
        team.addMember(m);
        team.removeMember(m);
        assert(team.getMembers().empty());
        std::cout << "testRemovingMember passed\n";
        delete m;
        delete coach;
    }
    catch (...) {
        std::cout << "testRemovingMember failed\n";
    }

    // Remove non-existent member
    try {
        Coach* coach = new Coach("Jane", "Football", 1);
        Team team("Football", coach, 1);
        Member* m1 = new Member("John", 30, "Athlete", 1);
        Member* m2 = new Member("Jane", 25, "Athlete", 2);
        team.addMember(m1);
        team.removeMember(m2);               // should be no-op
        assert(team.getMembers().size() == 1);
        std::cout << "testRemovingNonExistentMember passed\n";
        delete m1; delete m2; delete coach;
    }
    catch (...) {
        std::cout << "testRemovingNonExistentMember failed\n";
    }

    // Negative team ID
    try {
        Coach* coach = new Coach("Jane", "Football", 1);
        Team* t = new Team("Football", coach, -1);
        std::cerr << "testTeam failed: no exception on negative ID\n";
        delete t; delete coach;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception for negative ID: " << e.what() << '\n';
    }
}

// ------------------------------------------------------------------
// 4. Event tests
// ------------------------------------------------------------------
void testEvent() {
    bool ok = true;

    try {
        Event evt(nextEventId(), "2024-05-01", "Stadium", "Football Match");
        assert(evt.getDate() == "2024-05-01");
    }
    catch (...) { ok = false; std::cout << "Event Creation Test failed\n"; }

    try {
        Event evt(nextEventId(), "2024-05-01", "Stadium", "Football Match");
        evt.reschedule("2024-06-01");
        assert(evt.getDate() == "2024-06-01");
    }
    catch (...) { ok = false; std::cout << "Reschedule Test failed\n"; }

    try {
        Event evt(nextEventId(), "2024-05-01", "Stadium", "Football Match");
        Member* m = new Member("John", 30, "Athlete", 1);
        evt.addParticipant(m);
        assert(evt.getParticipantCount() == 1);
        delete m;
    }
    catch (...) { ok = false; std::cout << "Add Participant Test failed\n"; }

    try {
        Event evt(nextEventId(), "2024-05-01", "Stadium", "Football Match");
        try {
            evt.addParticipant(nullptr);
            std::cerr << "Add Invalid Participant Test failed: no exception\n";
            ok = false;
        }
        catch (const std::invalid_argument&) { /* expected */ }
    }
    catch (...) { ok = false; std::cout << "Add Invalid Participant Test failed\n"; }

    std::cout << (ok ? "testEvent passed" : "testEvent failed") << '\n';
}

// ------------------------------------------------------------------
// 5. Club smoke test
// ------------------------------------------------------------------
void testClub() {
    try {
        Club club("Sports Club");
        Member* m1 = new Member("John", 30, "Athlete", 1);
        Member* m2 = new Member("Jane", 25, "Athlete", 2);
        club.addMember(m1);
        club.addMember(m2);

        club.removeMember(m1);
        delete m1;

        Coach* c1 = new Coach("Jane", "Football", 1);
        club.addCoach(c1);
        club.updateCoachSpecialty("Jane", "Basketball");

        Event* e1 = new Event(nextEventId(), "2024-04-19", "Stadium", "Football Match");
        club.organizeEvent(e1);
        club.cancelEvent(e1);

        e1 = new Event(nextEventId(), "2024-04-19", "Stadium", "Football Match");
        club.organizeEvent(e1);
        club.addMembersToEvent("Football Match", { m2 });

        // duplicate member
        try {
            Member* dup = new Member("Dup", 20, "Athlete", 2);
            club.addMember(dup);
            club.removeMember(dup); delete dup;
            std::cerr << "testClub failed: duplicate member ID not caught\n";
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Duplicate member correctly rejected: " << e.what() << '\n';
        }

        club.cancelEvent(e1);
        delete e1;                             
        club.removeCoach(c1);                 
        delete c1;
        // m2 仍留在 club, 析构时自动释放
        std::cout << "testClub passed\n";
    }
    catch (...) {
        std::cout << "testClub failed\n";
    }
}

// ------------------------------------------------------------------
// 6. Delete club (full cascade) test
// ------------------------------------------------------------------
void testDeleteClub() {
    try {
        Club* club = new Club("Cascade Club");
        Member* m1 = new Member("M1", 20, "Athlete", 1);
        club->addMember(m1);
        Coach* c1 = new Coach("C1", "Tennis", 1);
        club->addCoach(c1);
        Event* e1 = new Event(nextEventId(), "2024-04-19", "Stadium", "Match");
        club->organizeEvent(e1);
        Team* t1 = new Team("Tennis", c1, 1);
        t1->addMember(m1);
        club->addTeam(t1);

        delete club; // should clean everything
        std::cout << "testDeleteClub passed\n";
    }
    catch (...) {
        std::cout << "testDeleteClub failed\n";
    }
}

// ------------------------------------------------------------------
// 7. Event schedule conflict
// ------------------------------------------------------------------
void testEventScheduleConflict() {
    try {
        Club club("Conf Club");
        Event* e1 = new Event(nextEventId(), "2024-04-19", "Stadium", "Match1");
        club.organizeEvent(e1);
        assert(club.hasScheduleConflict("2024-04-19"));
        assert(!club.hasScheduleConflict("2024-04-20"));
        club.cancelEvent(e1);
        std::cout << "testEventScheduleConflict passed\n";
    }
    catch (...) {
        std::cout << "testEventScheduleConflict failed\n";
    }
}

// ------------------------------------------------------------------
// 8. Remove member & impact
// ------------------------------------------------------------------
void testRemoveMember() {
    Club club("RemoveMem");
    Member* m1 = new Member("A", 20, "Athlete", 1);
    Member* m2 = new Member("B", 22, "Athlete", 2);
    club.addMember(m1); club.addMember(m2);
    Coach* c = new Coach("Coach", "Football", 1);
    club.addCoach(c);
    Event* e = new Event(nextEventId(), "2024-04-19", "Stadium", "Match");
    club.organizeEvent(e);
    club.addMembersToEvent("Match", { m1, m2 });
    Team* t = new Team("Football", c, 1);
    t->addMember(m1); t->addMember(m2);
    club.addTeam(t);

    club.removeMember(m1);                
    bool okTeam = (t->getMembers().size() == 1 && t->getMembers()[0]->getId() == 2);
    bool okEvt = (e->getParticipants().size() == 1 && e->getParticipants()[0]->getId() == 2);
    std::cout << (okTeam && okEvt ? "testRemoveMember passed" : "testRemoveMember failed") << '\n';

    club.cancelEvent(e);             
                          
    club.removeCoach(c);             
                          
    club.removeTeam(t);              
                      

}

// ------------------------------------------------------------------
// 9. Remove coach
// ------------------------------------------------------------------
void testRemoveCoach() {
    Club club("RemoveCoach");
    Member* m1 = new Member("A", 20, "Athlete", 1);
    club.addMember(m1);
    Coach* c1 = new Coach("C1", "Football", 1);
    club.addCoach(c1);
    Team* t1 = new Team("Football", c1, 1);
    t1->addMember(m1);
    club.addTeam(t1);

    club.removeCoach(c1);
    bool stillInClub = std::find(club.getCoaches().begin(), club.getCoaches().end(), c1) != club.getCoaches().end();
    std::cout << (!stillInClub ? "testRemoveCoach passed" : "testRemoveCoach failed") << '\n';

    club.removeTeam(t1);
}

// ------------------------------------------------------------------
// 10. Demo flow (print heavy)
// ------------------------------------------------------------------
void printDivider(const std::string& title) {
    std::cout << "\n-----------------------------------\n" << title << '\n'
        << "-----------------------------------\n";
}

void testClubFunctionality() {
    printDivider("Creating Club: Elite Sports Club");
    Club club("Elite Sports Club");

    printDivider("Adding Members: Jack, Kelly and Bob");
    Member* m1 = new Member("Jack", 24, "Athlete", 1);
    Member* m2 = new Member("Kelly", 26, "Athlete", 2);
    Member* m3 = new Member("Bob", 30, "Athlete", 3);
    club.addMember(m1); club.addMember(m2); club.addMember(m3);

    printDivider("Adding Coaches: John (Tennis) and Sam (Swimming)");
    Coach* c1 = new Coach("John", "Tennis", 1);
    Coach* c2 = new Coach("Sam", "Swimming", 2);
    club.addCoach(c1); club.addCoach(c2);

    printDivider("Creating Teams");
    Team* t1 = new Team("Tennis", c1, 1);
    Team* t2 = new Team("Swimming", c2, 2);
    t1->addMember(m1); t1->addMember(m2); t2->addMember(m3);
    club.addTeam(t1); club.addTeam(t2);

    printDivider("Organizing Events");
    Event* e1 = new Event(nextEventId(), "2024-08-20", "Arena", "Tennis Tournament");
    Event* e2 = new Event(nextEventId(), "2024-09-10", "Aquatic", "Swimming Competition");
    club.organizeEvent(e1); club.organizeEvent(e2);
    club.addMembersToEvent("Tennis Tournament", { m1, m2 });
    club.addMembersToEvent("Swimming Competition", { m3 });

    printDivider("Updating Coach Specialty");
    club.updateCoachSpecialty("John", "Basketball");

    printDivider("Schedule conflicts?");
    std::cout << "2024-08-20: " << (club.hasScheduleConflict("2024-08-20") ? "Yes" : "No") << '\n';
    std::cout << "2024-09-10: " << (club.hasScheduleConflict("2024-09-10") ? "Yes" : "No") << '\n';

    printDivider("Cleaning everything up");
    club.cancelEvent(e1); delete e1;           
    club.cancelEvent(e2); delete e2;           
    club.removeTeam(t1);  delete t1;           
    club.removeTeam(t2);  delete t2;          
    club.removeCoach(c1); delete c1;           
    club.removeCoach(c2); delete c2;          
    club.removeMember(m1); delete m1;          
    club.removeMember(m2); delete m2;         
    club.removeMember(m3); delete m3;          

    std::cout << "testClubFunctionality finished\n";
}



int main() {
    testMember();
    testCoach();
    testTeam();
    testEvent();
    testClub();
    testDeleteClub();
    testEventScheduleConflict();
    testRemoveMember();
    testRemoveCoach();
    testClubFunctionality();

    std::cout << "\nAll tests executed!\n";
    return 0;
}
