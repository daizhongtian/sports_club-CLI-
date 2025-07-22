// main.cpp
#include <iostream>
#include <string>
#include <limits>
#include "auth.h"
#include "club.h"
#include "member.h"
#include "coach.h"

using namespace std;


static Club g_club("Sport Club");
static int  g_nextId = 1;
static int  allocId() { return g_nextId++; }



void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void showMainMenu() {
    std::cout << "\n=== Sport Club System ===\n"
        << "1) Register\n"
        << "2) Login\n"
        << "0) Exit\n"
        << "> ";
}


void showAthleteMenu() {
    std::cout << "\n-- Athlete Menu --\n"
        << "1) View Profile\n"
        << "2) List& Register for Event\n"
        << "4) Cancel Registration(delete member) \n"
        << "0) Logout\n"
        << "> ";
}

void showCoachMenu() {
    std::cout << "\n-- Coach Menu --\n"
        << "1) View Profile\n"
        << "2) List Events\n"
        << "3) Register Member for Event\n"
        << "4) Cancel Event\n"
        << "5) Organize Event\n"
        << "0) Logout\n"
        << "> ";
}

void showAdminMenu() {
    std::cout << "\n-- Admin Menu --\n"
        << "1) List All Users\n"
        << "2) Delete User\n"
        << "3) List All Members\n"
        << "4) Add Member\n"
        << "5) Delete Member\n"
        << "6) List All Coaches\n"
        << "7) Add Coach\n"
        << "8) Delete Coach\n"
        << "9) List All Teams\n"
        << "10) Add Team\n"
        << "11) Delete Team\n"
        << "12) List All Events\n"
        << "13) Add Event\n"
        << "14) Cancel Event\n"
        << "0) Logout\n"
        << "> ";
}

void athleteloop()
{
    Credential* c = getCurrentCredential();
    Member* self = g_club.findMemberByName(c->username);

    string choice;
    while (true)
    {
        showAthleteMenu();
        getline(cin, choice);

        if (choice == "1")
        {
            std::cout << "\n[Your Profile]\n"
                << "Username: " << c->username << "\n"
                << "Role:     " << static_cast<int>(c->role) << "\n";
            pause();
        }

        else if (choice == "2")
        {
            vector<Event*> evs = g_club.getEvents();
            if (evs.empty())
            {
                cout << "\nNo events available.\n";
            }
            else
            {
                cout << "\n[All Events]\n";
                for (size_t i  = 0; i < evs.size(); ++i)
                {
                    Event* e = evs[i];
                    std::cout << "  " << (i + 1)
                        << ") ID=" << e->getId()
                        << " | " << e->getName()
                        << " | Date: " << e->getDate()
                        << " | Location: " << e->getLocation()
                        << "\n";
                }
                cout << "\nEnter event number to register (0 to cancel): ";
                string selection;
                getline(cin, selection);
                int idx = stoi(selection);
                if (idx > 0 && idx <= static_cast<int>(evs.size()))
                {
                    Event* chosen = evs[idx - 1];
                    cout << "Reason (optional): ";
                    string reason;
                    getline(std::cin, reason);

                }
                else
                {
                    cout << "  >> Registration cancelled.\n";
                }

            }


        }


        else if (choice == "3")
        {
            vector<Application*>apps = g_club.listMyApplications(self);
            if (apps.empty()) {
                std::cout << "\nYou have no applications.\n";
            }

            else
            {
                cout << "\n[My Applications]\n";

                for (Application* a : apps)
                {
                    cout << "  • ID=" << a->id
                        << " | Event=" << a->event->getName()
                        << " | Status="
                        << (a->status == AppStatus::PENDING ? "PENDING": a->status == AppStatus::APPROVED ? "APPROVED": "REJECTED")
                        << " | Reason=" << a->reason
                        << "\n";
                }
                cout << "\nEnter application ID to cancel (blank to skip): ";
                string id_entered;
                getline(cin, id_entered);
                if (!id_entered.empty()) {
                    // 用户输入了 ID，就把对应申请标记为 REJECTED（等同于“取消”）
                    int aid = std::stoi(id_entered);
                    g_club.reviewApplication(nullptr, aid, false);
                    std::cout << "  >> Application " << aid << " cancelled.\n";
                }

            }

        }







        else if(choice == "0")
        {
            logout();
            break;
        }
    }
}


void coachLoop() {
    Credential* c = getCurrentCredential();
    std::string choice;

    while (true) {
        showCoachMenu();
        std::getline(std::cin, choice);
        if (choice == "1") {
            std::cout << "\n[Your Profile]\n"
                << "Username:  " << c->username << "\n"
                << "Role:      " << static_cast<int>(c->role) << "\n";
            pause();
        }

        else if (choice == "2")
        {
            std::cout << "\n[All Events]\n";
            vector<Event*>events = g_club.getEvents();
            for (Event* e : events)
            {
                cout << "  • " << e->getName()
                    << "  | Date: " << e->getDate()
                    << "  | Location: " << e->getLocation()
                    << "\n";
            }

            pause();

        }


        else if (choice == "3")
        {
            string memberName, eventName;
            cout << "member Username to register";
            getline(cin, memberName);
            std::cout << "Event Name: ";
            std::getline(std::cin, eventName);


            Member* m = g_club.findMemberByName(memberName);

            if (!m)
            {
                std::cout << "Member not found.\n";

            }

            else
            {
                try
                {
                    g_club.addMembersToEvent(eventName, { m });
                }
                catch (const exception&)
                {
                    cout << "Registration failed due to internal error.\n";
                }
               
            }
            pause();
        }


        else if (choice == "4")
        {
           string eventName;
           std::cout << "Event Name to cancel: ";
           getline(cin, eventName);

           Event* target = nullptr;

           for (Event* e : g_club.getEvents())
           {
               if (e->getName() == eventName)
               {
                   target = e;
                   break;
               }
           }

           if (!target)
           {
               std::cout << "Event not found.\n";

           }

           else
           {
               try
               {
                   g_club.cancelEvent(target);             
                   cout << "Event cancelled successfully.\n";
               }


           
           catch (const exception&)
           {
               cout << "Registration failed due to internal error.\n";
           }

           }


        }



        else if (choice == "5")
        {
            string name, date, location;
            std::cout << "Event Name: ";
            std::getline(std::cin, name);
            std::cout << "Date (YYYY-MM-DD): ";
            std::getline(std::cin, date);
            std::cout << "Location: ";
            std::getline(std::cin, location);

            if (g_club.hasScheduleConflict(date))
            {
                std::cout << "Schedule conflict on " << date
                    << ". Please choose another date.\n";
            }

            else
            {
                try
                {
                    int eventId = allocId();
                    Event* e = new Event(eventId,date, location, name);
                    g_club.organizeEvent(e);
                    std::cout << "Event organized successfully.\n";


                }

                catch (const std::exception& ex) {
                    std::cout << "Failed to organize event: "
                        << ex.what() << "\n";
                }

            }
            pause();

            

        }

        else if (choice == "0") {
            logout();
            break;
        }
    }
}



void adminloop()
{
    string choice;
    while (true)
    {
        showAdminMenu();
        std::getline(std::cin, choice);
        if (choice == "1") {
            std::cout << "\n[All Registered Users]\n";
            auto all = getAllCredentials();
            for (const auto& u : all) {
                std::cout << "  - " << u.username
                    << " (role=" << static_cast<int>(u.role) << ")\n";

                pause();
            }

        }
        else if (choice == "2")
        {
            cout << "enter username to delete :";
            string user;
            getline(cin, user);

            if (deleteuser(user))
            {
                cout << "deleted successfully";
            }
            else
            {
                cout << "user not found ";

            }
            pause();

        }

        else if (choice == "3") {
         
            std::cout << "\n[All Members]\n";
            for (auto m : g_club.getMembers()) {
                std::cout << "  Name: " << m->getName()
                    << " | ID: " << m->getId() << "\n";
            }
            pause();
        }


        else if (choice == "4")
        {
            string name, role, age_s;
            cout << "Member Name: ";
            getline(cin, name);
            cout << "Role: ";
            getline(cin, role);
            cout << "Age: ";
            getline(cin, age_s);

            try 
            {
                int age = stoi(age_s);
                int id = allocId();
                Member* m = new Member(name, age, role, id);
                g_club.addMember(m);
                cout << "Member added (ID=" << id << ").\n";

            }

            catch (const std::exception& ex) {
                std::cout << "Failed to add member: " << ex.what() << "\n";
            }

            pause();

        }


        else if (choice == "5")
        {
            string idtodelete;
            cout << "Member ID to delete: ";
            int id = stoi(idtodelete);
            Member* m = g_club.findMemberById(id);

            if (m != nullptr)

            {
                g_club.removeMember(m);
                cout << "Member deleted successfully.\n";


            }
            else
            {
                cout << "Member not found.\n";
            }

            pause();

        }



        else if (choice == "6")
        {
            vector<Coach*>coaches = g_club.getCoaches();
            for (vector<Coach*>::iterator it = coaches.begin(); it != coaches.end(); ++it)
            {
                Coach* cch = *it;
                std::cout << "Name: " << cch->getName()
                    << " | ID: " << cch->getId()
                    << " | Specialty: " << cch->getSpecialty() << "\n";
            }

            pause();
        }

        else if (choice == "7")
        {
            string name, spec;
            std::cout << "Coach Name: ";

            getline(cin, name);
            std::cout << "Specialty: ";
            getline(cin, spec);
            int id = allocId();
            Coach* cch = new Coach(name, spec, id);
            g_club.addCoach(cch);
            std::cout << "Coach added successfully.\n";
            pause();

        }


        else if (choice == "8")
        {
            string idtodelete;
            cout << "Coach Name: ";
            getline(cin, idtodelete);
            int id = stoi(idtodelete);
            Coach* cch = g_club.findCoachById(id);
            if (cch != nullptr)
            {
                g_club.removeCoach(cch);
                cout << "Coach deleted successfully.\n";
            }
            else
            {
                cout << "Coach not found.\n";
            }
        
            pause();
        }


        else if (choice == "9")
        {
            vector<Team*>teams = g_club.getTeams();
            for (vector<Team*>::iterator it = teams.begin(); it != teams.end(); ++it)
            {
                Team* t = *it;
                cout << "Sport: " << t->getSportType()
                    << " | ID: " << t->getId() << "\n";
                cout << "Coach: ";

                Coach* teamcoach = t->getCoach();

                if (teamcoach != nullptr)
                {
                    cout << teamcoach->getName() << "\n";
                }
                else {
                    std::cout << "None\n";
                }
            }

            pause();

        }

        else if (choice == "10")
        {
           
         }


        else if (choice == "0")
        {
            logout();
            break;
        }


    }

    
}





int main()
{
    if (!loadcredentials()) {
        std::cerr << "Error: failed to load credentials.\n";
        return 1;
    }

    string choice;

    while (true)
    {
        showMainMenu();
        getline(cin, choice);

        if (choice == "1")
        {
            string username, password;
            int roleint;
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << "Password: ";
            std::getline(std::cin, password);
            std::cout << "Role (0=Athlete, 1=Coach, 2=Admin): ";
            std::string rs;
            std::getline(std::cin, rs);
            roleint = std::stoi(rs);

            if (registeruser(username, password, static_cast<Role>(roleint)))
            {
                std::cout << "Registration successful.\n";
            }
            else {
                std::cout << "Username already exists.\n";
                continue;
            }

            bool clubOk = false;

            try {
                switch (static_cast<Role>(roleint))
                {
                case Role::ATHLETE: {
                    string tmp;
                    cout << "Age: "; getline(cin, tmp);
                    int age = stoi(tmp);
                    Member* m = new Member(username, age, "athlete", allocId());
                    g_club.addMember(m);
                    clubOk = true;
                    break;

                }
                case Role::COACH:
                {
                    cout << "Specialty: "; string sp; getline(cin, sp);
                    Coach* c = new Coach(username, sp, allocId());
                    g_club.addCoach(c);                                 
                    clubOk = true;
                    break;
                }
                case Role::ADMIN:

                    clubOk = true;     
                    break;
                }



            }

                catch (const std::exception&) {
                    cout << "error。\n";
                }


                if (clubOk)
                    cout << "Added to club roster.\n";
                pause();

            
        }

        else if (choice == "2")
        {
            // Login
            std::string username, password;
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << "Password: ";
            std::getline(std::cin, password);


            if (login(username, password))
            {
                auto* c = getCurrentCredential();
                std::cout << "Login successful. Role=" << static_cast<int>(c->role) << "\n";

                switch (c->role)
                {
                case Role::ATHLETE:
                    athleteloop();
                    break;
                case Role::COACH:
                    coachLoop();
                    break;
                case Role::ADMIN:
                    adminloop();
                    break;
                }
            }

            else
            {
                std::cout << "Login failed.\n";
                pause();
            }

        }
        else if (choice == "0")
        {
            break;

        }
        else 
        {
            std::cout << "Invalid option.\n";
            pause();
        }




        
    }

cout << "Goodbye!\n";

return 0;



}