#include <iostream>

// Попереднє оголошення класу
template <typename T>
class ArrayList {
public:
    ArrayList();
    ~ArrayList();
    void push_back(const T& value);
    void insert(size_t index, const T& value);
    void erase(size_t index);
    T& get(size_t index) const;
    size_t getSize() const;
    size_t getCapacity() const;
    void printStructure() const; //візуалізація буферів
};

int main() {
    ArrayList<int> list; // Створюємо об'єкт структури "Перелік масивів"
    int choice;

    do {
        std::cout << "ArrayList Menu \n";
        std::cout << "1. Push back (Add to end)\n";
        std::cout << "2. Insert element by index\n";
        std::cout << "3. Erase element by index\n";
        std::cout << "4. Get element by index\n";
        std::cout << "5. Show size and buffer capacity\n";
        std::cout << "6. Print full inner structure\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice option: ";
        std::cin >> choice;

        try {
            if (choice == 1) {
                int val;
                std::cout << "Enter number to push back: ";
                std::cin >> val;
                list.push_back(val);
                std::cout << "Added\n";
            }
            else if (choice == 2) {
                size_t idx;
                int val;
                std::cout << "Enter global index: ";
                std::cin >> idx;
                std::cout << "Enter number to insert: ";
                std::cin >> val;
                list.insert(idx, val);
                std::cout << "inserted\n";
            }
            else if (choice == 3) {
                size_t idx;
                std::cout << "Enter index to erase: ";
                std::cin >> idx;
                list.erase(idx);
                std::cout << "deleted\n";
            }
            else if (choice == 4) {
                size_t idx;
                std::cout << "Enter index to get: ";
                std::cin >> idx;
                std::cout << "Value at index " << idx << " is: " << list.get(idx) << "\n";
            }
            else if (choice == 5) {
                std::cout << "Present elements (Size): " << list.getSize() << "\n";
                std::cout << "Total allocated slots (Capacity): " << list.getCapacity() << "\n";
            }
            else if (choice == 6) {
                std::cout << "\nCurrent Structure state\n";
                list.printStructure();
            }
            else if (choice != 0) {
                std::cout << "Vrong choice\n";
            }
        }
        // Тут фіксуємо не правильні аргументи
        catch (const std::exception& e) {
            std::cerr << "\nArgument error" << e.what() << "\n";
        }

    } while (choice != 0);

    std::cout << "End\n";
    return 0;
}
