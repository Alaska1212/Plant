//Plant
//done by Puchek Polina (comp-math 2)
//main classes

#ifndef PLANT_H
#define PLANT_H


#include <string>
#include <vector>
#include <vector>
class Equipment {
private:
    std::string name;
    double cost;
    int quantity;

public:
    Equipment() : name(""), cost(0.0), quantity(0) {}

    Equipment(const std::string& name, double cost, int quantity)
        : name(name), cost(cost), quantity(quantity) {}

    std::string getName() const { return name; }
    void setName(const std::string& name) { this->name = name; }

    double getCost() const { return cost; }
    void setCost(double cost) { this->cost = cost; }

    int getQuantity() const { return quantity; }
    void setQuantity(int quantity) { this->quantity = quantity; }
};

class Person {
protected:
    std::string name;
    std::string lastname;

public:
    Person() : name(""), lastname("") {}

    Person(const std::string& name, const std::string& lastname)
        : name(name), lastname(lastname) {}

    std::string getName() const { return name; }
    void setName(const std::string& name) { this->name = name; }

    std::string getLastname() const { return lastname; }
    void setLastname(const std::string& lastname) { this->lastname = lastname; }

    virtual void print_salary() const = 0;
    virtual void print_salary_to_file(std::ofstream& file) const = 0;

    virtual ~Person() = default;
};

class Profession {
    std::string name;
    double baseSalary;

public:
    Profession() : name(""), baseSalary(0.0) {}

    Profession(const std::string& name, double baseSalary)
        : name(name), baseSalary(baseSalary) {}

    std::string getName() const { return name; }
    void setName(const std::string& name) { this->name = name; }

    double getBaseSalary() const { return baseSalary; }
    void setBaseSalary(double baseSalary) { this->baseSalary = baseSalary; }
};

class Worker : public Person {
private:
    Profession profession;
    int experience;
    double productionAmount;
    Equipment equipment;
    std::string headName;

public:
    Worker(const std::string& name, const std::string& lastname, const Profession& profession,
           int experience, double productionAmount, const Equipment& equipment, const std::string& headName)
        : Person(name, lastname), profession(profession), experience(experience),
          productionAmount(productionAmount), equipment(equipment), headName(headName) {}

    Profession getProfession() const { return profession; }
    void setProfession(const Profession& profession) { this->profession = profession; }

    int getExperience() const { return experience; }
    void setExperience(int experience) { this->experience = experience; }

    double getProductionAmount() const { return productionAmount; }
    void setProductionAmount(double productionAmount) { this->productionAmount = productionAmount; }

    Equipment getEquipment() const { return equipment; }
    void setEquipment(const Equipment& equipment) { this->equipment = equipment; }

    std::string getHeadName() const { return headName; }
    void setHeadName(const std::string& headName) { this->headName = headName; }

    double calculate_worker_salary() const;
    void print_salary() const override;
    void print_salary_to_file(std::ofstream& file) const override;
};

class Head : public Person {
private:
    Profession profession;
    int experience;

public:
    Head(const std::string& name, const std::string& lastname, const Profession& profession, int experience)
        : Person(name, lastname), profession(profession), experience(experience) {}

    double calculate_head_salary() const;
    void print_salary() const override;
    void print_salary_to_file(std::ofstream& file) const override;
};

class PartTimer : public Person {
private:
    Profession profession;
    int experience;
    double productionAmount;
    Equipment equipment;
    std::string headName;

public:
    PartTimer(const std::string& name, const std::string& lastname, const Profession& profession,
              int experience, double productionAmount, const Equipment& equipment, const std::string& headName)
        : Person(name, lastname), profession(profession), experience(experience),
          productionAmount(productionAmount), equipment(equipment), headName(headName) {}

    double calculate_part_timer_salary() const;
    void print_salary() const override;
    void print_salary_to_file(std::ofstream& file) const override;
};

// Function declarations for use in test.cpp
std::vector<Worker> read_equipment_from_file(const std::string& fileName);
std::vector<Person*> read_workers_from_file(const std::string& filename);
std::vector<std::pair<std::string, double>> calculate_equipment_cost_for_head(const std::vector<Worker>& workers);
void print_salaries(const std::vector<Person*>& people);
void print_equipment_in_consol(const std::vector<Worker>& workers);
void write_salary_and_equipment_to_file(const std::vector<Person*>& people, const std::vector<Worker>& workers, const std::string& filename);


#endif //PLANT_H
