#include "coach.h"
#include <stdexcept>
#include <sstream>
#include <vector>
#include <fstream>


using namespace std;

// Constructor to initialize a Coach object with name, specialty, and ID
// Throws an exception if specialty is empty or ID is negative
Coach::Coach(const std::string& name, const std::string& specialty, int id)
    : name(name), specialty(specialty), id(id) {
    if (specialty.empty()) {
        throw std::invalid_argument("Specialty cannot be empty");
    }
    if (id < 0) {
        throw std::invalid_argument("Coach ID cannot be negative");
    }
}

// Getter for the name of the coach
std::string Coach::getName() const {
    return name;
}

// Getter for the specialty of the coach
std::string Coach::getSpecialty() const {
    return specialty;
}

// Setter to update the specialty of the coach
void Coach::setSpecialty(const std::string& new_specialty) {
    specialty = new_specialty;
}

// Getter for the ID of the coach
int Coach::getId() const {
    return id;
}

// Equality operator to compare two coaches
bool Coach::operator==(const Coach& other) const {
    return name == other.name && specialty == other.specialty;
}



Coach* Coach::fromCsv(const string& line)
{
    //check ","
    if (std::count(line.begin(), line.end(), ',') != 2) {
        throw std::invalid_argument("Malformed CSV for Coach: expected 3 fields");
    }


    istringstream iss(line);
    vector<string>tokens;
    string token;

    while (getline(iss, token, ','))
    {
        tokens.push_back(token);
    }


    if (tokens.size() != 3) {
        throw std::invalid_argument(
            " 3 fields, got " +
            std::to_string(tokens.size())
        );
    }

    int id = 0;
    try {
        id = std::stoi(tokens[0]);
    }
    catch (const std::exception& e) {

        throw std::invalid_argument(
            "Invalid id field in Coach CSV: " + tokens[0]
        );
    }
    const std::string& name = tokens[1];
    const std::string& specialty = tokens[2];

    Coach* c = new Coach(name, specialty, id);

    return c;

}

string Coach::toCsv()const
{
    return to_string(getId()) + "," + getName() +"," + getSpecialty();


}
