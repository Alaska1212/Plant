//Plant
//done by Puchek Polina (comp-math 2)
//Functions for information processing and calculation

#include <stdio.h>
#include <string.h>
#include "Plant.h"

// read equipment from file
int read_eq_from_file(const char* filename, Worker workers[], Equipment equipments[], int max_workers) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open the file!\n");
        return 0;
    }

    char line[200];
    int eq_count = 0;

    while (fgets(line, sizeof(line), file) != NULL && eq_count < max_workers) {
        char name[MAX_NAME_LENGTH], lastname[MAX_NAME_LENGTH], manager[MAX_NAME_LENGTH];
        char equipment_type[MAX_EQUIPMENT_LENGTH];
        double equipment_cost;

        // Skip header line
        if (eq_count == 0 && line[0] == 'N') continue;

        // Read Plant_test.dat(p).txt from the file
        sscanf(line, "%49[^,],%49[^,],%49[^,],%29[^,],%lf", name, lastname, manager, equipment_type, &equipment_cost);

        // Fill the Worker structure
        Worker* worker = &workers[eq_count];
        strcpy(worker->person.name, name);
        strcpy(worker->person.lastname, lastname);
        strcpy(worker->head_name, manager);

        // Assign equipment if available
        if (equipment_cost > 0) {
            strcpy(worker->equipment.name, equipment_type);
            worker->equipment.cost = equipment_cost;
        } else {
            worker->equipment.name[0] = '\0';  // Clear the equipment field if no equipment is provided
            worker->equipment.cost = 0;
        }

        eq_count++;
    }

    fclose(file);
    return eq_count;
}

// calculate equipment cost
double calculate_total_equipment_cost(Worker workers[], int worker_count) {
    double total_cost = 0.0;

    // Sum up the equipment costs of all workers
    for (int i = 0; i < worker_count; i++) {
        total_cost += workers[i].equipment.cost;
    }

    return total_cost;
}

// calculate equipment cost by heads and print to consol
void group_equipment_by_head(Worker workers[], int worker_count) {
    // Create structure to track equipment and their counts by each head
    ManagerEquipment manager_equipment[MAX_MANAGERS];
    int manager_count = 0;

    // Initialize the list of heads
    for (int i = 0; i < worker_count; i++) {
        Worker* worker = &workers[i];
        int found = 0;

        if (worker->equipment.cost > 0) {  // If the worker has equipment
            // Check if this head already exists in the list
            for (int j = 0; j < manager_count; j++) {
                if (strcmp(manager_equipment[j].head_name, worker->head_name) == 0) {
                    // Check if this equipment is already in the list of the manager
                    int equipment_index = -1;
                    for (int k = 0; k < manager_equipment[j].equipment_count; k++) {
                        if (strcmp(manager_equipment[j].equipment[k].name, worker->equipment.name) == 0) {
                            equipment_index = k;
                            break;
                        }
                    }

                    // If the equipment exists, increment the count and update the cost
                    if (equipment_index != -1) {
                        manager_equipment[j].equipment[equipment_index].cost += worker->equipment.cost;
                        manager_equipment[j].equipment[equipment_index].quantity++;
                    } else {
                        // If the equipment is not found, add it to the list
                        strcpy(manager_equipment[j].equipment[manager_equipment[j].equipment_count].name, worker->equipment.name);
                        manager_equipment[j].equipment[manager_equipment[j].equipment_count].cost = worker->equipment.cost;
                        manager_equipment[j].equipment[manager_equipment[j].equipment_count].quantity = 1;
                        manager_equipment[j].equipment_count++;
                    }

                    found = 1;
                    break;
                }
            }

            // If the head is not found, add the head to the list
            if (!found) {
                strcpy(manager_equipment[manager_count].head_name, worker->head_name);
                strcpy(manager_equipment[manager_count].equipment[0].name, worker->equipment.name);
                manager_equipment[manager_count].equipment[0].cost = worker->equipment.cost;
                manager_equipment[manager_count].equipment[0].quantity = 1;
                manager_equipment[manager_count].equipment_count = 1;
                manager_count++;
            }
        }
    }

    // Output the total cost per head
    printf("Total equipment cost per head:\n");
    for (int i = 0; i < manager_count; i++) {
        float total_cost = 0;

        // Calculate the total cost for each head
        for (int j = 0; j < manager_equipment[i].equipment_count; j++) {
            total_cost += manager_equipment[i].equipment[j].cost;
        }

        // Output the result in the desired format
        printf("Head: %s, Total cost: %.2f\n", manager_equipment[i].head_name, total_cost);
    }
}

// calculate salaries
double calculate_employee_salary(Worker* worker) {
    double salary = worker->profession.base_salary + worker->experience_years * 1000;
    salary += worker->production_amount * 10;  // Add production amount bonus
    return salary;
}

double calculate_part_timer_salary(PartTimer* part_timer) {
    double salary = part_timer->profession.base_salary + part_timer->experience_years * 1000;
    salary += part_timer->production_amount * 5;  // Lower production rate for part-timers
    return salary;
}

double calculate_head_salary(Head* head) {
    double salary_multiplier = 1.2  ;            //Coefficient for salary calculation
    double salary = head->profession.base_salary * salary_multiplier + head->experience_years * 1000;
    return salary;
}


int read_workers_from_file(const char* filename, Worker workers[], int* worker_count,
                           PartTimer part_timers[], int* part_timer_count,
                           Head heads[], int* head_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char type[20], name[MAX_NAME_LENGTH], lastname[MAX_NAME_LENGTH], profession[MAX_PROFESSION_LENGTH];
        int experience_years = 0, product_count = 0;
        double base_rate = 0.0;

        sscanf(line, "%19[^,],%49[^,],%49[^,],%49[^,],%lf,%d,%d",
               type, name, lastname, profession, &base_rate, &experience_years, &product_count);

        if (strcmp(type, "Worker") == 0) {
            Worker new_worker = {0};
            strcpy(new_worker.person.name, name);
            strcpy(new_worker.person.lastname, lastname);
            strcpy(new_worker.profession.name, profession);
            new_worker.profession.base_salary = base_rate;
            new_worker.experience_years = experience_years;
            new_worker.production_amount = product_count;
            strcpy(new_worker.head_name, ""); // Initialize head name

            workers[*worker_count] = new_worker;
            (*worker_count)++;
        } else if (strcmp(type, "PartTimer") == 0) {
            PartTimer new_part_timer = {0};
            strcpy(new_part_timer.person.name, name);
            strcpy(new_part_timer.person.lastname, lastname);
            strcpy(new_part_timer.profession.name, profession);
            new_part_timer.profession.base_salary = base_rate;
            new_part_timer.experience_years = experience_years;
            new_part_timer.production_amount = product_count;

            part_timers[*part_timer_count] = new_part_timer;
            (*part_timer_count)++;
        } else if (strcmp(type, "Head") == 0) {
            Head new_head = {0};
            strcpy(new_head.person.name, name);
            strcpy(new_head.person.lastname, lastname);
            strcpy(new_head.profession.name, profession);
            new_head.profession.base_salary = base_rate;
            new_head.experience_years = experience_years;

            heads[*head_count] = new_head;
            (*head_count)++;
        }
    }

    fclose(file);
    return 0;
}


void print_salaries(Worker workers[], int worker_count,
                    Head heads[], int head_count,
                    PartTimer part_timers[], int part_timer_count) {
   
    printf("Salaries for Heads:\n");
    for (int i = 0; i < head_count; i++) {
        int subordinates_count = 0;
        for (int j = 0; j < worker_count; j++) {
            if (strcmp(workers[j].head_name, heads[i].person.name) == 0) {
                subordinates_count++;
            }
        }
        double salary = calculate_head_salary(&heads[i]);
        printf("%s %s - %.2f (Head)\n", heads[i].person.name, heads[i].person.lastname, salary);
    }

    printf("\nSalaries for Workers:\n");
    for (int i = 0; i < worker_count; i++) {
        double salary = calculate_employee_salary(&workers[i]);
        printf("%s %s - %.2f (Worker)\n", workers[i].person.name, workers[i].person.lastname, salary);
    }

    printf("\nSalaries for PartTimers:\n");
    for (int i = 0; i < part_timer_count; i++) {
        double salary = calculate_part_timer_salary(&part_timers[i]);
        printf("%s %s - %.2f (PartTimer)\n", part_timers[i].person.name, part_timers[i].person.lastname, salary);
    }
}
// print all equipment to consol
void print_full_equipment_inventory(Worker workers[], int worker_count) {
    Equipment factory_inventory[MAX_EQUIPMENT];
    int inventory_count = 0;

    // Loop through each worker and process their equipment
    for (int i = 0; i < worker_count; i++) {
        Worker* worker = &workers[i];

        if (worker->equipment.cost > 0) {  // If the worker has equipment
            int found = 0;

            // Check if the equipment already exists in the factory inventory
            for (int j = 0; j < inventory_count; j++) {
                if (strcmp(factory_inventory[j].name, worker->equipment.name) == 0) {
                    // If found, increase the quantity
                    factory_inventory[j].quantity++;
                    found = 1;
                    break;
                }
            }

            // If the equipment is not found, add it to the inventory
            if (!found) {
                strcpy(factory_inventory[inventory_count].name, worker->equipment.name);
                factory_inventory[inventory_count].cost = worker->equipment.cost;
                factory_inventory[inventory_count].quantity = 1;
                inventory_count++;
            }
        }
    }

    // Output the full equipment inventory
    printf("Full Equipment Inventory in the Factory:\n");
    for (int i = 0; i < inventory_count; i++) {
        printf("- %s (Quantity: %d, Total Cost: %.2f)\n",
               factory_inventory[i].name,
               factory_inventory[i].quantity,
               factory_inventory[i].cost * factory_inventory[i].quantity);
    }
}


// write information to file
void write_salary_to_file(const char* filename, Worker workers[], int worker_count,
                                Head heads[], int head_count,
                                PartTimer part_timers[], int part_timer_count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    fprintf(file, "Salaries for Heads:\n");
    for (int i = 0; i < head_count; i++) {
        double salary = calculate_head_salary(&heads[i]);
        fprintf(file, "%s %s - %.2f (Head)\n", heads[i].person.name, heads[i].person.lastname, salary);
    }

    fprintf(file, "\nSalaries for Workers:\n");
    for (int i = 0; i < worker_count; i++) {
        double salary = calculate_employee_salary(&workers[i]);
        fprintf(file, "%s %s - %.2f (Worker)\n", workers[i].person.name, workers[i].person.lastname, salary);
    }

    fprintf(file, "\nSalaries for PartTimers:\n");
    for (int i = 0; i < part_timer_count; i++) {
        double salary = calculate_part_timer_salary(&part_timers[i]);
        fprintf(file, "%s %s - %.2f (PartTimer)\n", part_timers[i].person.name, part_timers[i].person.lastname, salary);
    }

    fclose(file);
    printf("Calculations written to file: %s\n", filename);
}

void write_equipment_to_file(const char* filename, Worker workers[], int worker_count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }
    Equipment factory_inventory[MAX_EQUIPMENT];
    int inventory_count = 0;

    // Loop through each worker and process their equipment
    for (int i = 0; i < worker_count; i++) {
        Worker* worker = &workers[i];

        if (worker->equipment.cost > 0) {  // If the worker has equipment
            int found = 0;

            // Check if the equipment already exists in the factory inventory
            for (int j = 0; j < inventory_count; j++) {
                if (strcmp(factory_inventory[j].name, worker->equipment.name) == 0) {
                    // If found, increase the quantity
                    factory_inventory[j].quantity++;
                    found = 1;
                    break;
                }
            }

            // If the equipment is not found, add it to the inventory
            if (!found) {
                strcpy(factory_inventory[inventory_count].name, worker->equipment.name);
                factory_inventory[inventory_count].cost = worker->equipment.cost;
                factory_inventory[inventory_count].quantity = 1;
                inventory_count++;
            }
        }
    }

    // Output the full equipment inventory
    fprintf(file,"\nFull Equipment Inventory in the Factory:\n");
    for (int i = 0; i < inventory_count; i++) {
        fprintf(file,"- %s (Quantity: %d, Total Cost: %.2f)\n",
               factory_inventory[i].name,
               factory_inventory[i].quantity,
               factory_inventory[i].cost * factory_inventory[i].quantity);
    }
// Create structure to track equipment and their counts by each head
    ManagerEquipment manager_equipment[MAX_MANAGERS];
    int manager_count = 0;

    // Initialize the list of heads
    for (int i = 0; i < worker_count; i++) {
        Worker* worker = &workers[i];
        int found = 0;

        if (worker->equipment.cost > 0) {  // If the worker has equipment
            // Check if this head already exists in the list
            for (int j = 0; j < manager_count; j++) {
                if (strcmp(manager_equipment[j].head_name, worker->head_name) == 0) {
                    // Check if this equipment is already in the list of the manager
                    int equipment_index = -1;
                    for (int k = 0; k < manager_equipment[j].equipment_count; k++) {
                        if (strcmp(manager_equipment[j].equipment[k].name, worker->equipment.name) == 0) {
                            equipment_index = k;
                            break;
                        }
                    }

                    // If the equipment exists, increment the count and update the cost
                    if (equipment_index != -1) {
                        manager_equipment[j].equipment[equipment_index].cost += worker->equipment.cost;
                        manager_equipment[j].equipment[equipment_index].quantity++;
                    } else {
                        // If the equipment is not found, add it to the list
                        strcpy(manager_equipment[j].equipment[manager_equipment[j].equipment_count].name, worker->equipment.name);
                        manager_equipment[j].equipment[manager_equipment[j].equipment_count].cost = worker->equipment.cost;
                        manager_equipment[j].equipment[manager_equipment[j].equipment_count].quantity = 1;
                        manager_equipment[j].equipment_count++;
                    }

                    found = 1;
                    break;
                }
            }

            // If the head is not found, add the head to the list
            if (!found) {
                strcpy(manager_equipment[manager_count].head_name, worker->head_name);
                strcpy(manager_equipment[manager_count].equipment[0].name, worker->equipment.name);
                manager_equipment[manager_count].equipment[0].cost = worker->equipment.cost;
                manager_equipment[manager_count].equipment[0].quantity = 1;
                manager_equipment[manager_count].equipment_count = 1;
                manager_count++;
            }
        }
    }

    // Output the total cost per head
    fprintf(file,"\nTotal equipment cost per head:\n");
    for (int i = 0; i < manager_count; i++) {
        float total_cost = 0;

        // Calculate the total cost for each head
        for (int j = 0; j < manager_equipment[i].equipment_count; j++) {
            total_cost += manager_equipment[i].equipment[j].cost;
        }

        // Output the result in the desired format
        fprintf(file,"Head: %s, Total cost: %.2f\n", manager_equipment[i].head_name, total_cost);
    }

    fclose(file);
    printf("Calculations written to file: %s\n", filename);

}


