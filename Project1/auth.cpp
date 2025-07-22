

#include "Auth.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "event.h"
#include "club.h"
using namespace std;

static vector<Credential>g_credentials;
static int g_currentidx = -1; //no user log in
static const char* Cred_file = "credentials.txt";


namespace 
{
	vector<string>split(const string& line, char delimiter = ',')
  {
		vector<string>elems;
		stringstream ss(line); //read the line in ss
		string item;
		while (getline(ss, item, delimiter))//split
		{
			elems.push_back(item);
		}
		return elems;
 }
}



bool loadcredentials()
{
	g_credentials.clear();//clean the credential in current memory

	ifstream in(Cred_file);

	int lineno = 0;

	if (!in)
	{
		return true; //if no file also no credential
	}


	string line;

	while (getline(in, line))
	{
		++lineno;
		if (line.empty())
		{
			continue;
		}
		vector<string>v = split(line);

		if (v.size() != 3)
		{
			cout << "some unexpected error happned the line" << lineno<<"is skipped" <<line<< endl;
			continue;   
		}

		Credential c;

		c.username = v[0];
		c.password = v[1];
		c.role = static_cast<Role>(stoi(v[2]));
		g_credentials.push_back(c);



	}
	return true;
}



bool savecredentials()
{
	ofstream out(Cred_file, ios::trunc);

	if (!out)
	{
		return false;
	}

	for (auto& c : g_credentials)
	{
		out << c.username << ',' << c.password<<',' << static_cast<int>(c.role) << '\n';
	}

	return true;

}

bool registeruser(const string& username, const string& password, Role role)
{
	for (const auto& c : g_credentials)
	{
		if (c.username == username)
		{
			return false;
		}
	}

		g_credentials.push_back({ username,password,role });

		return savecredentials();
	


}


bool login(const string& username, const string& password)
{
	for (int i = 0; i < (int)g_credentials.size(); ++i)
	{
		const auto& c = g_credentials[i];
		if (c.username == username && c.password == password)
		{
			g_currentidx = i;
			return true;
		}
	}

	return false;
}

void logout()
{
	g_currentidx = -1;
}

Credential* getCurrentCredential()
{
	if (g_currentidx >= 0 && g_currentidx < (int)g_credentials.size())
	{
		return&g_credentials[g_currentidx];

	}

	return nullptr;
}


std::vector<Credential> getAllCredentials() {
	return g_credentials; 
}



bool deleteuser(const string& username)
{
	auto it = remove_if(g_credentials.begin(), g_credentials.end(), [&](const Credential& c)
		{
			return c.username == username;
		});

	if (it == g_credentials.end())
	{
		return false;
	}

	g_credentials.erase(it, g_credentials.end());
	return savecredentials();



}

