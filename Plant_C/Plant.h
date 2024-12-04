//Plant
//done by Puchek Polina (comp-math 2)
// implementation of basic structures

#ifndef PLANT_H
#define PLANT_H

#define MAX_NAME_LENGTH 50
#define MAX_PROFESSION_LENGTH 30
#define MAX_EQUIPMENT_LENGTH 30
#define MAX_WORKERS 50
#define MAX_MANAGERS 50
#define MAX_EQUIPMENT 100


// Structure for equipment
typedef struct {
    char name[MAX_EQUIPMENT_LENGTH];  // Equipment name
    double cost;                      // Unit cost
    int quantity;                     // Quantity of this equipment for the manager's subordinates
} Equipment;


// Basic structure for Person
typedef struct {
    char name[MAX_NAME_LENGTH];
    char lastname[MAX_NAME_LENGTH];
} Person;

// Structure for the profession
typedef struct {
    char name[MAX_PROFESSION_LENGTH]; // name of the profession
    double base_salary;
} Profession;

// Structure for the employee
typedef struct {
    Person person;                      // information about the person
    Profession profession;              // profession of the employee
    int experience_years;
    double production_amount;
    Equipment equipment;
    char head_name[MAX_NAME_LENGTH];    // head`s name
} Worker;

// Structure for the head
typedef struct {
    Person person;
    int experience_years;
    Profession profession;
} Head;

// Structure for parttimer
typedef struct {
    Person person;
    Profession profession;
    int experience_years;
    double production_amount;
    Equipment equipment;
    char head_name[MAX_NAME_LENGTH];
} PartTimer;


// Additional structure for equipment accounting under the head
typedef struct {
    char head_name[MAX_NAME_LENGTH];  // Manager's name
    Equipment equipment[MAX_WORKERS]; // Equipment used by the manager's subordinates
    int equipment_count;              // Number of distinct equipment types
} ManagerEquipment;



// declaration of functions
int read_eq_from_file(const char* filename, Worker workers[], Equipment equipments[], int max_workers);
double calculate_total_equipment_cost(Worker workers[], int worker_count);
void group_equipment_by_head(Worker workers[], int worker_count);

int read_workers_from_file(const char* filename, Worker workers[], int* worker_count,
                           PartTimer part_timers[], int* part_timer_count,
                           Head heads[], int* head_count);

void print_salaries(Worker workers[], int worker_count,
                    Head heads[], int head_count,
                    PartTimer part_timers[], int part_timer_count);

double calculate_head_salary(Head *head);
double calculate_employee_salary(Worker *worker);
double calculate_part_timer_salary(PartTimer* part_timer);
void print_full_equipment_inventory(Worker workers[], int worker_count);

void read_workers_from_console(Worker workers[], int* worker_count,
                               PartTimer part_timers[], int* part_timer_count,
                               Head heads[], int* head_count);

void write_salary_to_file(const char* filename, Worker workers[], int worker_count,
                                Head heads[], int head_count,
                                PartTimer part_timers[], int part_timer_count);

void write_equipment_to_file(const char* filename,Worker workers[], int worker_count);
void file_equipment_by_head(const char* filename,Worker workers[], int worker_count);
void write_all_data_to_file(const char* filename, Worker workers[], int worker_count,
                                Head heads[], int head_count,
                                PartTimer part_timers[], int part_timer_count, int eq_count);
#endif // PLANT_H