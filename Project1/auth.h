#pragma once
#include <string>
#include <vector>


using namespace std;

enum class Role

{
	ATHLETE = 0,
	COACH = 1,
	ADMIN = 2

};


struct Credential
{
	string username;
	string password;

	Role  role;
	
};

bool loadcredentials();


bool savecredentials();


bool registeruser(const string& username,const string& password, Role role);

bool login (const string& username, const string& password);

void logout();

Credential* getCurrentCredential();

std::vector<Credential> getAllCredentials();

bool deleteuser(const std::string& username);


