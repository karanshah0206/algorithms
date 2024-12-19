#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

// Container adapter for efficient retrieval of the largest (by default) element.
// The user can supply a different Container type and Compare type.
template<typename T, typename Container = std::vector<T>, typename Compare = std::less<T>>
class priority_queue {
private:
    Container c;
    Compare comp;

    // Helper function to maintain the heap property
    void heapify_up(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (comp(c[parent], c[index])) {
                std::swap(c[parent], c[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Helper function to maintain the heap property
    void heapify_down(size_t index) {
        size_t left_child = 2 * index + 1;
        size_t right_child = 2 * index + 2;
        size_t largest = index;

        if (left_child < c.size() && comp(c[largest], c[left_child])) {
            largest = left_child;
        }

        if (right_child < c.size() && comp(c[largest], c[right_child])) {
            largest = right_child;
        }

        if (largest != index) {
            std::swap(c[largest], c[index]);
            heapify_down(largest);
        }
    }

public:
    // Default constructor
    priority_queue() = default;

    // Get the top element of the priority queue
    const T& top() const {
        if (c.empty()) {
            throw std::out_of_range("priority_queue is empty");
        }

        return c.front();
    }

    // Check if the priority queue is empty
    bool empty() const {
        return c.empty();
    }

    // Get the size of the priority queue
    size_t size() const {
        return c.size();
    }

    // Push an element (l-value) into the priority queue
    void push(const T& value) {
        c.push_back(value); // Add to the end of the container
        heapify_up(c.size() - 1); // Restore heap property
    }

    // Push an element (r-value) into the priority queue
    void push(T&& value) {
        c.push_back(std::move(value)); // Add to the end of the container
        heapify_up(c.size() - 1); // Restore heap property
    }

    // Emplace an element into the priority queue
    template<typename... Args>
    void emplace(Args&&... args) {
        c.emplace_back(std::forward<Args>(args)...); // Emplace at the end of the container
        heapify_up(c.size() - 1); // Restore heap property
    }

    // Pop the top element from the priority queue
    void pop() {
        if (!c.empty()) {
            std::swap(c.front(), c.back()); // Move the last element to the top
            c.pop_back(); // Remove the last element
            heapify_down(0); // Restore heap property
        }
    }

    // Swap contents with another priority queue
    void swap(priority_queue& other) {
        std::swap(c, other.c);
        std::swap(comp, other.comp);
    }
};

// Non-member swap function
template<typename T, typename Container, typename Compare>
void swap(priority_queue<T, Container, Compare>& lhs, priority_queue<T, Container, Compare>& rhs) {
    lhs.swap(rhs); // Forward to the member swap function
}

// Testing the data structure
int main() {
    try {
        priority_queue<int> pq;

        pq.push(10);
        pq.push(20);
        pq.push(15);
        pq.push(30);

        std::cout << "Size of the priority queue: " << pq.size() << std::endl;

        std::cout << "Top element: " << pq.top() << std::endl;

        pq.pop();
        std::cout << "Top element after pop: " << pq.top() << std::endl;

        std::cout << "Checking if the priority queue is empty: " << (pq.empty() ? "Yes" : "No")  << std::endl;

        while (!pq.empty()) {
            pq.pop();
        }

        std::cout << "Size of the priority queue after emptying: " << pq.size() << std::endl;

        int val = pq.top();
        std::cout << "Top element from empty priority queue: " << val << ". This line should never have been printed!" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Exception correctly thrown when fetching top from empty priority queue: " << e.what() << std::endl;
    }

    priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

    min_pq.push(10);
    min_pq.push(20);
    min_pq.push(15);
    min_pq.push(30);

    std::cout << "Top element in min priority queue: " << min_pq.top() << std::endl;

    min_pq.pop();

    std::cout << "Top element after pop: " << min_pq.top() << std::endl;

    priority_queue<int, std::vector<int>, std::greater<int>> min_pq_2 = min_pq;

    std::cout << "Top element in copied min priority queue: " << min_pq_2.top() << std::endl;

    std::cout << "Popping from copied min priority queue, then swapping with the original min priority queue." << std::endl;
    min_pq_2.pop();
    swap(min_pq, min_pq_2);

    std::cout << "Original min priority queue: ";
    while (!min_pq.empty()) {
        std::cout << min_pq.top() << " ";
        min_pq.pop();
    }
    std::cout << std::endl;

    std::cout << "Copied min priority queue: ";
    while (!min_pq_2.empty()) {
        std::cout << min_pq_2.top() << " ";
        min_pq_2.pop();
    }
    std::cout << std::endl;

    return 0;
}
