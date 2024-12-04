//Plant
//done by Puchek Polina (comp-math 2)
//basic methods for calculating and reading information

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm> // Для std::find_if
#include "Plant.h"

std::vector<Worker> read_equipment_from_file(const std::string& fileName) {
    std::vector<Worker> workers;
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return workers;
    }

    std::string line;
    std::getline(file, line); // Skip the first line (header)

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string name, lastName, manager, equipmentType;
        double equipmentCost;
        int equipmentQuantity = 1; // For each entry, quantity = 1

        // Read data.txt separated by commas
        std::getline(stream, name, ',');
        std::getline(stream, lastName, ',');
        std::getline(stream, manager, ',');
        std::getline(stream, equipmentType, ',');
        stream >> equipmentCost;

        // Create an Equipment object
        Equipment equipment(equipmentType, equipmentCost, equipmentQuantity);

        // Create a Worker object
        Worker worker(name, lastName, Profession("Worker", 1000), 5, 10, equipment, manager);

        workers.push_back(worker);
    }

    file.close();
    return workers;
}

std::vector<std::pair<std::string, double>> calculate_equipment_cost_for_head(const std::vector<Worker>& workers) {
    std::vector<std::pair<std::string, double>> equipmentCostPerManager;

    for (const auto& worker : workers) {
        const std::string& managerName = worker.getHeadName();
        if (managerName != "None") {
            auto it = std::find_if(equipmentCostPerManager.begin(), equipmentCostPerManager.end(),
                [&managerName](const std::pair<std::string, double>& pair) {
                    return pair.first == managerName;
                });

            if (it != equipmentCostPerManager.end()) {
                it->second += worker.getEquipment().getCost();
            } else {
                equipmentCostPerManager.emplace_back(managerName, worker.getEquipment().getCost());
            }
        }
    }

    return equipmentCostPerManager;
}

void print_equipment_in_consol(const std::vector<Worker>& workers) {
    // Card for equipment accounting
    std::map<std::string, std::pair<int, double>> inventory;

    for (const Worker& worker : workers) {
        const Equipment& equipment = worker.getEquipment();
        const std::string& equipmentName = equipment.getName();
        int quantity = equipment.getQuantity();
        double cost = equipment.getCost();

        // Оновлення даних у мапі
        inventory[equipmentName].first += quantity;            // Total quantity
        inventory[equipmentName].second += quantity * cost;   // Total cost
    }

    // Accounting output
    std::cout << "Full Equipment Inventory in the Factory:\n";
    for (const auto& entry : inventory) {
        const std::string& equipmentName = entry.first;
        int totalQuantity = entry.second.first;
        double totalCost = entry.second.second;

        std::cout << "- " << equipmentName
                  << " (Quantity: " << totalQuantity
                  << ", Total Cost: " << std::fixed << std::setprecision(2)
                  << totalCost << ")\n";
    }
}

double Worker::calculate_worker_salary() const {
    double salary = profession.getBaseSalary() + experience * 1000;
    salary += productionAmount * 10;  // Додаємо бонус за виробіток
    return salary;
}

void Worker::print_salary() const {
    double salary = calculate_worker_salary();
    std::cout << getName() << " " << getLastname() << " - "
              << std::fixed << std::setprecision(2) << salary << " (Worker)" << std::endl;
}

double PartTimer::calculate_part_timer_salary() const {
    double salary = profession.getBaseSalary() + experience * 1000;
    salary += productionAmount * 5;
    return salary;
}

void PartTimer::print_salary() const {
    double salary = calculate_part_timer_salary();
    std::cout << getName() << " " << getLastname() << " - "
              << std::fixed << std::setprecision(2) << salary << " (PartTimer)" << std::endl;
}

double Head::calculate_head_salary() const {
    double salaryMultiplier = 1.2;
    double salary =  profession.getBaseSalary() * salaryMultiplier + experience * 1000;
    return salary;
}

void Head::print_salary() const {
    double salary = calculate_head_salary();
    std::cout << getName() << " " << getLastname() << " - "
              << std::fixed << std::setprecision(2) << salary << " (Head)" << std::endl;
}

std::vector<Person*> read_workers_from_file(const std::string& filename) {
    std::vector<Person*> people;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return people;
    }

    std::string line;
    std::getline(file, line); // Skip the first line with headers

    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string type, name, lastname, professionName;
        double baseSalary = 0, productionAmount = 0;
        int experience = 0;

        // Зчитуємо дані із рядка
        std::getline(ss, type, ',');
        std::getline(ss, name, ',');
        std::getline(ss, lastname, ',');
        std::getline(ss, professionName, ',');
        ss >> baseSalary;
        ss.ignore(); // Skip the comma
        ss >> experience;
        ss.ignore();
        ss >> productionAmount;

        // We create objects depending on the type
        Profession profession(professionName, baseSalary);

        if (type == "Worker") {
            people.push_back(new Worker(name, lastname, profession, experience, productionAmount, Equipment(), ""));
        } else if (type == "PartTimer") {
            people.push_back(new PartTimer(name, lastname, profession, experience, productionAmount, Equipment(), ""));
        } else if (type == "Head") {
            people.push_back(new Head(name, lastname, profession, experience));
        } else {
            std::cout << "Unknown type: " << type << std::endl;
        }
    }

    return people;
}

void print_salaries(const std::vector<Person*>& people) {
    for (const auto& person : people) {
        if (typeid(*person) == typeid(Worker)) {
            static_cast<Worker*>(person)->print_salary();
        } else if (typeid(*person) == typeid(PartTimer)) {
            static_cast<PartTimer*>(person)->print_salary();
        } else if (typeid(*person) == typeid(Head)) {
            static_cast<Head*>(person)->print_salary();
        } else {
            std::cerr << "Unknown person type!" << std::endl;
        }
    }
}


void Worker::print_salary_to_file(std::ofstream& file) const {
    double salary = calculate_worker_salary();
    file << getName() << " " << getLastname() << " - "
         << std::fixed << std::setprecision(2) << salary << " (Worker)\n";
}

void PartTimer::print_salary_to_file(std::ofstream& file) const {
    double salary = calculate_part_timer_salary();
    file << getName() << " " << getLastname() << " - "
         << std::fixed << std::setprecision(2) << salary << " (PartTimer)\n";
}

void Head::print_salary_to_file(std::ofstream& file) const {
    double salary = calculate_head_salary();
    file << getName() << " " << getLastname() << " - "
         << std::fixed << std::setprecision(2) << salary << " (Head)\n";
}

void write_salary_and_equipment_to_file(const std::vector<Person*>& people,
                                   const std::vector<Worker>& workers,
                                   const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Запис зарплат працівників
    file << "Employee Salaries:\n";
    for (const auto& person : people) {
        if (typeid(*person) == typeid(Worker)) {
            static_cast<Worker*>(person)->print_salary_to_file(file);
        } else if (typeid(*person) == typeid(PartTimer)) {
            static_cast<PartTimer*>(person)->print_salary_to_file(file);
        } else if (typeid(*person) == typeid(Head)) {
            static_cast<Head*>(person)->print_salary_to_file(file);
        } else {
            std::cerr << "Unknown person type!" << std::endl;
        }
    }

    // Запис обліку обладнання
    file << "\nFull Equipment Inventory in the Factory:\n";
    std::map<std::string, std::pair<int, double>> inventory;

    for (const Worker& worker : workers) {
        const Equipment& equipment = worker.getEquipment();
        const std::string& equipmentName = equipment.getName();
        int quantity = equipment.getQuantity();
        double cost = equipment.getCost();

        inventory[equipmentName].first += quantity;
        inventory[equipmentName].second += quantity * cost;
    }

    for (const auto& entry : inventory) {
        const std::string& equipmentName = entry.first;
        int totalQuantity = entry.second.first;
        double totalCost = entry.second.second;

        file << "- " << equipmentName << " (Quantity: " << totalQuantity
             << ", Total Cost: " << std::fixed << std::setprecision(2) << totalCost << ")\n";
    }

    auto equipment_cost_for_head = calculate_equipment_cost_for_head(workers);
    file << "\nTotal equipment cost per head: \n";
    for (const auto& pair : equipment_cost_for_head) {
        const std::string& manager = pair.first;
        double totalCost = pair.second;
        file  << "Head: " << manager << ", Total cost: " << totalCost << "\n";
    }

    file.close();
}

