#include <iostream>
#include <vector>
#include <algorithm>

#include "../ch-02/person.h"

class Employee: public Person {
public:
    Employee() = default;
    Employee(std::string fname, std::string lname, int age,
             gender_e gender, std::string team) :
        Person(fname, lname, age, gender), m_team(team) {}

    std::string get_team() const {
        return m_team;
    }

private:
    std::string m_team;
};


class company_t {
public:
    std::string team_name_for(const Employee &) const;
    int count_team_members(const std::string &team_name) const;

    void add_employee(const Employee &employee);

private:
    std::vector<Employee> m_employees;

};

std::string company_t::team_name_for(const Employee &employee) const
{
    return employee.get_team();
}

int company_t::count_team_members(const std::string &team_name) const
{
    // Counting the number of members in the specified team
    // using a lambda.

    // We need to capture "this" because we’re using it when calling the
    // team_name_for member function, and we’ve captured team_name
    // because we need to use it for comparison.
    return std::count_if(
            m_employees.cbegin(), m_employees.cend(),
            [this, &team_name] (const Employee &employee)
            {
                return this->team_name_for(employee) == team_name;
            }
        );
}

void company_t::add_employee(const Employee &employee) {
    m_employees.push_back(employee);
}

int main(int argc, char *argv[])
{
    // Also for exercise, create an instance of the company_t object,
    // and test the count_team_members member function
    company_t company;

    std::vector<Employee> employees {
        {"Joe",    "Dell",    20,   Person::male,   "team1"},
        {"Maria",  "Watson",  30,   Person::female, "team2"},
        {"Tina",   "Wu",      52,   Person::female, "team3"},
        {"Peter",  "Smith",   28,   Person::male,   "team3"},
        {"Ella",   "Yun",     65,   Person::female, "team1"},
        {"Shaen",  "Martin",  18,   Person::male,   "team1"},
    };

    for (auto employee: employees) {
        company.add_employee(employee);
    }

    std::cout << "team1: " << company.count_team_members("team1") << std::endl;
    std::cout << "team2: " << company.count_team_members("team2") << std::endl;
    std::cout << "team3: " << company.count_team_members("team3") << std::endl;

}

/*
What happens in C++ when you write this? When compiled, the lambda expression will
turn into a new class that has two member variables—a pointer to the company_t object
and a reference to a std::string—one member for each captured variable. This class
will have the call operator with the same arguments and the same body of the lambda.
You’ll get a class equivalent to the following.

class lambda_implementation {
public:
    lambda_implementation(
            const company_t* _this,
            const std::string& team_name)
        : m_this(_this)
        , m_team_name(team_name)
    {
    }

    bool operator()(const person_t& employee) const
    {
        return m_this->team_name_for(employee) == m_team_name;
    }

private:
    const company_t* m_this;
    const std::string& m_team_name;
};

In addition to creating a class, evaluating the lambda expression also creates an
instance of that class called a "closure": an object containing some state or environment
along with code that should be executed on that state.

IMPORTANT:
One thing worth noting is that the call operator of lambdas is constant by default
(contrary to the other parts of the language, where you need to explicitly specify that
something is const). If you want to change the value of the captured variables, when
they’re captured by value and not by reference, you’ll need to declare the lambda as
"mutable". In the following example, you use the std::for_each algorithm to write all
words beginning with an uppercase letter, and you use the count variable to count the
number of words you wrote. This is sometimes useful for debugging, but mutable lambdas
should be avoided. Obviously, there are better ways to do this, but the point here is
to demonstrate how mutable lambdas work.

int count = 0;
std::vector<std::string> words{"An", "ancient", "pond"};

std::for_each(words.cbegin(), words.cend(),
        [count] (const std::string& word) mutable
        {
            if (isupper(word[0])) {
                std::cout << word << " " << count <<std::endl;
                count++;
            }
        }
    );

*/