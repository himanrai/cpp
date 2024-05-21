#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
public:
    enum gender_e {
        male,
        female,
        other
    };

    enum output_format_e {
        name_only,
        whole_name
    };

    Person() = default;
    Person(std::string first_name, std::string last_name, int age, gender_e gender) :
        m_first_name(first_name), m_last_name(last_name), m_age(age), m_gender(gender) {}
    
    bool is_female() {
        return m_gender == female;
    }

    std::string first_name() const {
        return m_first_name;
    }

    std::string last_name() const {
        return m_last_name;
    }

    std::string full_name() const {
        return m_first_name + " " + m_last_name;
    }

    int age() const {
        return m_age;
    }

    gender_e gender() const {
        return m_gender;
    }

    void print(std::ostream &out,
               Person::output_format_e format) const
    {
        if (format == Person::name_only) {
            out << first_name() << '\n';

        } else if (format == Person::whole_name) {
            out << full_name() << '\n';

        }
    }

private:
    std::string m_first_name;
    std::string m_last_name;
    int m_age;
    gender_e m_gender;
};

#endif //PERSON_H