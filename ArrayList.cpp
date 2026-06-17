#include <iostream>
#include <stdexcept>

//Загально - вузол в мене тут це масив всередині посилань вперед і назад
//

// Фіксована місткість масиву всередині одного вузла списку
const size_t NODE_CAPACITY = 4;

// клас структури Перелік масивів
template <typename T>
class ArrayList {
private:
    // Структура самого вузла переліку
    struct Node {
        T data[NODE_CAPACITY]; // Статичний масив (буфер) для зберігання елементів
        size_t size = 0;       // Скільки реально елементів зараз записано в цей вузол
        Node* next = nullptr;  // Вказівник на наступний вузол (Посилання вперед)
        Node* prev = nullptr;  // Вказівник на попередній вузол (Посилання назад)
    };

    Node* head;         // Вказівник на перший вузол переліку
    Node* tail;         // Вказівник на останній вузол переліку
    size_t total_size;  // Загальна кількість присутніх елементів у всій структурі

public:
    // Створюємо пустий перелік масивів
    ArrayList() : head(nullptr), tail(nullptr), total_size(0) {}

    // видаляємо всі вузли з пам'яті, аби уникнути витіку
    ~ArrayList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }
    }

    // Додавання елементів в кінець
    void push_back(const T& value) {
        // Якщо перелік взагалі пустий або в останньому вузлі вже немає місця
        if (tail == nullptr || tail->size == NODE_CAPACITY) {
            Node* new_node = new Node(); // Створюємо новий вузол
            
            if (head == nullptr) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node; // Зв'язуємо вперед
                new_node->prev = tail; // Зв'язуємо назад за схемою
                tail = new_node; // Зсуважмо зад
            }
        }

        //Додаємо елемент у масив нашого останнього вузла
        tail->data[tail->size] = value;
        tail->size++;
        total_size++;
    }

    //додавання елементів за індексом
    void insert(size_t index, const T& value) {
        // помилка вихід за межі поточного розміру
        if (index > total_size) {
            throw std::out_of_range("Error, Insert index is out of range");
        }

        // Тут помилка була, виправлено
        if (index == total_size) {
            push_back(value);
            return;
        }

        // Шукаємо, в який саме вузол та на яку позицію масиву потрапляє індекс
        Node* current = head;
        size_t current_index = index;

        while (current != nullptr && current_index > current->size) {
            current_index -= current->size;
            current = current->next;
        }

        // Якщо вузол, куди вставляємо, не заповнений повністю
        if (current->size < NODE_CAPACITY) {
            // Зсуваємо елементи всередині масиву вузла вправо
            for (size_t i = current->size; i > current_index; --i) {
                current->data[i] = current->data[i - 1];
            }
            current->data[current_index] = value;
            current->size++;
        } 
        // Якщо вузол повністю забитий
        else {
            Node* new_node = new Node();
            
            // Переносимо половину елементів у новий вузол
            size_t move_count = NODE_CAPACITY / 2;
            for (size_t i = 0; i < move_count; ++i) {
                new_node->data[i] = current->data[NODE_CAPACITY - move_count + i];
                new_node->size++;
            }
            current->size -= move_count;

            //дивимося куди вставити наш новий елемент у старий чи новий вузол
            if (current_index <= current->size) {
                for (size_t i = current->size; i > current_index; --i) {
                    current->data[i] = current->data[i - 1];
                }
                current->data[current_index] = value;
                current->size++;
            } else {
                size_t new_node_idx = current_index - current->size;
                for (size_t i = new_node->size; i > new_node_idx; --i) {
                    new_node->data[i] = new_node->data[i - 1];
                }
                new_node->data[new_node_idx] = value;
                new_node->size++;
            }

            // Вбудовуємо вузол у двонаправлений список
            new_node->next = current->next;
            new_node->prev = current;
            if (current->next != nullptr) {
                current->next->prev = new_node;
            } else {
                tail = new_node;
            }
            current->next = new_node;
        }

        total_size++;
    }

    // 3. Видалення ел-ів за індексом
    void erase(size_t index) {
        // Для помилок
        if (total_size == 0) {
            throw std::underflow_error("Error, ArrayList is empty");
        }
        if (index >= total_size) {
            throw std::out_of_range("Error, Delete index is out of range");
        }

        // Шукаємо потрібний вузол
        Node* current = head;
        size_t current_index = index;

        while (current != nullptr && current_index >= current->size) {
            current_index -= current->size;
            current = current->next;
        }

        // Зсуваємо елементи масиву вліво, видаляючи видалений
        for (size_t i = current_index; i < current->size - 1; ++i) {
            current->data[i] = current->data[i + 1];
        }
        current->size--;
        total_size--;

        // Якщо вузол став повністю пустим, видаляємо його зі списку
        if (current->size == 0) {
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                head = current->next; // Якщо видалили голову
            }

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev; // Якщо видалили кінець
            }

            delete current;
        }
    }

    //Отримання елементу за індексом
    T& get(size_t index) const {
        if (index >= total_size) {
            throw std::out_of_range("Error, Get index is out of range");
        }

        Node* current = head;
        size_t current_index = index;

        while (current != nullptr && current_index >= current->size) {
            current_index -= current->size;
            current = current->next;
        }

        return current->data[current_index];
    }

    // Отримання к-стіі присутніх ел-ів
    size_t getSize() const {
        return total_size;
    }

    //Отримання кількості ел у буфері або загальна місткість виділених масивів
    size_t getCapacity() const {
        size_t nodes_count = 0;
        Node* current = head;
        while (current != nullptr) {
            nodes_count++;
            current = current->next;
        }
        return nodes_count * NODE_CAPACITY;
    }

    // Допоміжна ф-ія візуалізації структури через консолі
    void printStructure() const {
        if (total_size == 0) {
            std::cout << "[ ArrayList is empty ]\n";
            return;
        }

        Node* current = head;
        size_t node_idx = 1;
        while (current != nullptr) {
            std::cout << "Node " << node_idx << " (size: " << current->size << "/" << NODE_CAPACITY << "): [ ";
            for (size_t i = 0; i < current->size; ++i) {
                std::cout << current->data[i] << " ";
            }
            // Покажемо порожні комірки виділеного буфера, це буде зручно
            for (size_t i = current->size; i < NODE_CAPACITY; ++i) {
                std::cout << "_ ";
            }
            std::cout << "]\n";
            current = current->next;
            node_idx++;
        }
        std::cout << "Total elements: " << getSize() << "Total Buffer Capacity: " << getCapacity() << "\n";
    }
};

//кажемо компілятору зібрати int, аби main.cpp його побачив
template class ArrayList<int>;
