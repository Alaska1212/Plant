//Plant
//done by Puchek Polina (comp-math 2)
//The main file for starting the program

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "Plant.h"


int main() {
    std::vector<Person*> people;
    std::vector<Worker> workers;

    const std::string file_eq = "C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_CPP\\Plant_test.dat(e).txt";
    const std::string file_people = "C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_CPP\\Plant_test.dat(p).txt";


    people = read_workers_from_file(file_people);  // Capture the result from reading workers
    workers = read_equipment_from_file(file_eq);   // Capture the result from reading equipment

    int choice;
    // Output method selection
    std::cout << "Choose output method:\n";
    std::cout << "1. Output data to console\n";
    std::cout << "2. Output data to file\n";
    std::cout << "Your choice: ";
    std::cin >> choice;

    if (choice == 1) {
        // Output to console
        auto equipment_cost_for_head = calculate_equipment_cost_for_head(workers);
        std::cout << std::endl;
        std::cout << "Total equipment cost per head:" << std::endl;
        for (const auto& pair : equipment_cost_for_head) {
            const std::string& manager = pair.first;
            double totalCost = pair.second;
            std::cout << "Head: " << manager << ", Total cost: " << totalCost << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Employee Salaries:\n";
        print_salaries(people);
        std::cout << std::endl;
        // Output inventory of equipment
        print_equipment_in_consol(workers);

        // Clean up memory
        for (auto& person : people) {
            delete person;
        }
    } else if (choice == 2) {
        // Output to file
        write_salary_and_equipment_to_file(people, workers, "C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_CPP\\salary_and_inventory_report.txt");
    }


    // Clean up memory
    for (auto& person : people) {
        delete person;
    }

    return 0;
}