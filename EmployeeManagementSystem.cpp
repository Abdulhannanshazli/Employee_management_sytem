#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>

class Employee {
private:
    int id;
    std::string name;
    std::string department;
    std::string position;
    double basicSalary;
    double allowances;
    double overtimeHours;
    const double OVERTIME_RATE = 1.5;

public:
    Employee(int _id, std::string _name, std::string _dept, std::string _pos, double _salary, double _allowances = 0)
        : id(_id), name(_name), department(_dept), position(_pos), 
          basicSalary(_salary), allowances(_allowances), overtimeHours(0) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDepartment() const { return department; }
    std::string getPosition() const { return position; }
    double getBasicSalary() const { return basicSalary; }
    
    // Setters
    void setPosition(const std::string& pos) { position = pos; }
    void setSalary(double salary) { basicSalary = salary; }
    void setOvertimeHours(double hours) { overtimeHours = hours; }
    
    // Calculate total salary including overtime
    double calculateTotalSalary() const {
        return basicSalary + allowances + (overtimeHours * (basicSalary / 160) * OVERTIME_RATE);
    }
};

class EmployeeManagement {
private:
    std::vector<Employee> employees;
    const std::string FILENAME = "employees.csv";

    bool isIdUnique(int id) const {
        return std::none_of(employees.begin(), employees.end(),
            [id](const Employee& emp) { return emp.getId() == id; });
    }

public:
    bool addEmployee(int id, const std::string& name, const std::string& dept,
                    const std::string& pos, double salary, double allowances = 0) {
        if (!isIdUnique(id)) {
            std::cout << "Error: Employee ID already exists!\n";
            return false;
        }
        
        employees.emplace_back(id, name, dept, pos, salary, allowances);
        return true;
    }

    Employee* findEmployee(int id) {
        auto it = std::find_if(employees.begin(), employees.end(),
            [id](const Employee& emp) { return emp.getId() == id; });
        return (it != employees.end()) ? &(*it) : nullptr;
    }

    bool updateEmployee(int id, const std::string& pos, double salary) {
        if (Employee* emp = findEmployee(id)) {
            emp->setPosition(pos);
            emp->setSalary(salary);
            return true;
        }
        return false;
    }

    bool removeEmployee(int id) {
        auto it = std::find_if(employees.begin(), employees.end(),
            [id](const Employee& emp) { return emp.getId() == id; });
        if (it != employees.end()) {
            employees.erase(it);
            return true;
        }
        return false;
    }

    void displayEmployeesByDepartment() const {
        std::map<std::string, std::vector<const Employee*>> deptMap;
        
        for (const auto& emp : employees) {
            deptMap[emp.getDepartment()].push_back(&emp);
        }

        for (const auto& dept : deptMap) {
            std::cout << "\nDepartment: " << dept.first << "\n";
            std::cout << std::setw(5) << "ID" << std::setw(20) << "Name" 
                      << std::setw(15) << "Position" << std::setw(12) << "Salary\n";
            std::cout << std::string(52, '-') << "\n";
            
            for (const auto* emp : dept.second) {
                std::cout << std::setw(5) << emp->getId()
                          << std::setw(20) << emp->getName()
                          << std::setw(15) << emp->getPosition()
                          << std::setw(12) << emp->getBasicSalary() << "\n";
            }
        }
    }

    void saveToFile() const {
        std::ofstream file(FILENAME);
        if (!file) {
            std::cerr << "Error: Unable to open file for writing!\n";
            return;
        }

        for (const auto& emp : employees) {
            file << emp.getId() << ","
                 << emp.getName() << ","
                 << emp.getDepartment() << ","
                 << emp.getPosition() << ","
                 << emp.getBasicSalary() << "\n";
        }
    }

    void loadFromFile() {
        std::ifstream file(FILENAME);
        if (!file) {
            std::cerr << "Note: No existing employee data file found.\n";
            return;
        }

        employees.clear();
        std::string line;
        while (std::getline(file, line)) {
            // Parse CSV line and create Employee objects
            // Implementation details omitted for brevity
        }
    }

    double calculateAverageSalary() const {
        if (employees.empty()) return 0.0;
        
        double total = 0.0;
        for (const auto& emp : employees) {
            total += emp.getBasicSalary();
        }
        return total / employees.size();
    }

    size_t getTotalEmployees() const {
        return employees.size();
    }
}; 

