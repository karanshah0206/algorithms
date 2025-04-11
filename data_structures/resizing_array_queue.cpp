#include <iostream>

template<typename T>
class resizing_array_queue {
private:
    int capacity, first_filled_index, next_to_fill_index;
    T *arr;

    void resize(int new_capacity) {
        T *new_arr = new T[new_capacity];

        if (!is_empty()) {
            T *old_arr = arr;
            int i;
            for (i = 0; i < size(); i++) {
                new_arr[i] = std::move(old_arr[(first_filled_index + i) % capacity]);
            }
            first_filled_index = 0;
            next_to_fill_index = i;
            delete[] old_arr;
        }

        capacity = new_capacity;
        arr = new_arr;
    }

public:
    explicit resizing_array_queue() {
        first_filled_index = -1;
        next_to_fill_index = 0;
        resize(4);
    }

    resizing_array_queue(const resizing_array_queue &other) {
        capacity = other.capacity;
        first_filled_index = 0;
        next_to_fill_index = other.size();

        arr = new T[capacity];

        for (int i = 0; i < other.size(); i++) {
            arr[i] = other.arr[(other.first_filled_index + i) % other.capacity];
        }
    }

    resizing_array_queue &operator=(const resizing_array_queue &other) {
        if (this == &other) {
            return *this;
        }

        delete[] arr;

        capacity = other.capacity;
        first_filled_index = 0;
        next_to_fill_index = other.size();

        arr = new T[capacity];

        for (int i = 0; i < other.size(); i++) {
            arr[i] = other.arr[(other.first_filled_index + i) % other.capacity];
        }

        return *this;
    }

    ~resizing_array_queue() {
        delete[] arr;
    }

    void enqueue(T payload) {
        if (size() == capacity) {
            resize(capacity * 2);
        }

        if (is_empty()) {
            first_filled_index = next_to_fill_index;
        }

        arr[next_to_fill_index++] = payload;

        if (next_to_fill_index == capacity) {
            next_to_fill_index = 0;
        }
    }

    T dequeue() {
        if (is_empty()) {
            throw std::out_of_range("Queue is empty.");
        }

        T payload = arr[first_filled_index++];

        if (first_filled_index == capacity) {
            first_filled_index = 0;
        }

        if (first_filled_index == next_to_fill_index) {
            first_filled_index = -1;
        }

        if (size() * 4 <= capacity && capacity / 2 >= 4) {
            resize(capacity / 2);
        }

        return payload;
    }

    int size() const {
        if (first_filled_index == -1) {
            return 0;
        } else if (next_to_fill_index > first_filled_index) {
            return next_to_fill_index - first_filled_index;
        } else {
            return capacity - first_filled_index + next_to_fill_index;
        }
    }

    bool is_empty() const {
        return size() <= 0;
    }

    // just here for testing purposes, should be removed in any real program.
    friend void show_internal_status(resizing_array_queue<T>& q) {
        std::cout << "Size: " << q.size() << " Capacity: " << q.capacity << " First at: " << q.first_filled_index << " Next at : " << q.next_to_fill_index << std::endl;
    }
};

// Testing
int main() {
    resizing_array_queue<int> q;
    std::string s;
    while (std::cin >> s) {
        if (s == "push") {
            int x;
            std::cin >> x;
            q.enqueue(x);
        } else if (s == "pop") {
            std::cout << q.dequeue() << std::endl;
        } else {
            show_internal_status(q);
            if (s == "exit") {
                break;
            }
        }
    }
    return 0;
}
