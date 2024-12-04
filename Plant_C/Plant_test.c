//Plant
//done by Puchek Polina (comp-math 2)
//The main file for starting the program

#include "Plant.h"
#include <stdio.h>


int main() {
    Worker workers[100];
    PartTimer part_timers[100];
    Head heads[50];

    int worker_count = 0;
    int head_count = 0;
    int part_timer_count = 0;

    int output_choice;

    printf("Choose output method: \n");
    printf("1. Output  to console\n");
    printf("2. Output to file\n");
    printf("Your choice: ");
    scanf("%d", &output_choice);

    if (output_choice == 1) {
        // Read the equipment Plant_test.dat(e).txt from file
        int eq_count = read_eq_from_file("C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_C\\Plant_test.dat(e).txt", workers, NULL, MAX_WORKERS);

        if (eq_count > 0) {
            double total_cost = calculate_total_equipment_cost(workers, eq_count);
            printf("The total cost of the equipment: %.2f\n", total_cost);
            printf("\n ");
            group_equipment_by_head(workers, eq_count);
            printf("\n ");
            print_full_equipment_inventory(workers, eq_count);  // Display the full inventory
        }

        printf("\n ");

        // Read the worker Plant_test.dat(p).txt from file
        int status = read_workers_from_file("C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_C\\Plant_test.dat(p).txt", workers, &worker_count, part_timers, &part_timer_count, heads, &head_count);
        if (status == 0) {
            print_salaries(workers, worker_count, heads, head_count, part_timers, part_timer_count);
        }
    }
    else if (output_choice == 2) {
        int status = read_workers_from_file("C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_C\\Plant_test.dat(p).txt", workers, &worker_count, part_timers, &part_timer_count, heads, &head_count);
        if (status == 0) {
            write_salary_to_file("C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_C\\salaries.txt",
                                      workers, worker_count, heads, head_count, part_timers, part_timer_count);
        }
        int eq_count = read_eq_from_file("C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_C\\Plant_test.dat(e).txt", workers, NULL, MAX_WORKERS);
        write_equipment_to_file("C:\\Users\\polin\\OneDrive\\Documents\\Ci\\Proj\\Plant_C\\equipment.txt", workers, eq_count);
    }





    return 0;
}
