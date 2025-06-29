/*
 * Count the number of intersecting or overlapping rectangles.
 * Assumes no degenerate rectangles in input (all rectangles have non-zero area).
 * Assumes all vertices have integer coordinates.
 * Rectangles sharing a boundary/vertex are considered intersecting.
 */

// Requires C++20 or above

#include <iostream>
#include <queue>
#include <vector>
#include <concepts>
#include <optional>
#include <functional>

// a structure to store points on a 2d plane with integer coordinates only
struct point_2d {
    int x, y;

    point_2d(int x, int y): x(x), y(y) {}
};

// a structure to store rectangles with all vertices on integer coordinates
struct rectangle {
    int id;
    point_2d bottom_left, top_right;

    rectangle(int id, point_2d bottom_left, point_2d top_right): id(id), bottom_left(bottom_left), top_right(top_right) {}
};

// delineation of events used in event-driven intersection detection in descending order of priority
enum event_type {
    RECTANGLE_START,
    RECTANGLE_END,
};

// a structure to store an event with ordering logic for use in event-driven intersection detection
struct event {
    int x_coord;
    rectangle *rect;
    event_type type;

    event(rectangle &rect, event_type type):
        rect(&rect), type(type),
        x_coord(type == RECTANGLE_END ? rect.top_right.x : rect.bottom_left.x) {}

    bool operator>(const event &other) const {
        if (x_coord == other.x_coord) {
            return type > other.type;
        }

        return x_coord > other.x_coord;
    }
};

// Iinterval search tree to store "active" rectangle edges in event-driven intersection detection.
class interval_search_tree {
private:
    // using a left-leaning red-black (LLRB) tree to maintain a balanced interval tree
    struct llrb_node {
        int lo, hi, tree_hi;
        int val;
        llrb_node *left, *right;
        int size;
        bool is_red;

        // static reference to sentinel node
        static llrb_node *NIL;

        // sentinel constructor
        llrb_node() : size(0), is_red(false), left(this), right(this) {}

        llrb_node(int lo, int hi, int val) :
            lo(lo), hi(hi), val(val), tree_hi(hi),
            size(1), is_red(true), left(NIL), right(NIL) {}
    };

    llrb_node *root;

    // ###########################################
    //              LLRB HELPERS
    // ###########################################

    llrb_node *rotate_left(llrb_node *current) {
        llrb_node *temp = current->right;

        current->right = temp->left;
        temp->left = current;

        temp->is_red = current->is_red;
        current->is_red = true;

        temp->size = 1 + temp->left->size + temp->right->size;
        current->size = 1 + current->left->size + current->right->size;

        current->tree_hi = current->hi;
        if (current->left != llrb_node::NIL && current->left->tree_hi > current->tree_hi) {
            current->tree_hi = current->left->tree_hi;
        }
        if (current->right != llrb_node::NIL && current->right->tree_hi > current->tree_hi) {
            current->tree_hi = current->right->tree_hi;
        }

        temp->tree_hi = temp->hi;
        if (temp->left != llrb_node::NIL && temp->left->tree_hi > temp->tree_hi) {
            temp->tree_hi = temp->left->tree_hi;
        }
        if (temp->right != llrb_node::NIL && temp->right->tree_hi > temp->tree_hi) {
            temp->tree_hi = temp->right->tree_hi;
        }

        return temp;
    }

    llrb_node *rotate_right(llrb_node *current) {
        llrb_node *temp = current->left;

        current->left = temp->right;
        temp->right = current;

        temp->is_red = current->is_red;
        current->is_red = true;

        temp->size = 1 + temp->left->size + temp->right->size;
        current->size = 1 + current->left->size + current->right->size;

        current->tree_hi = current->hi;
        if (current->left != llrb_node::NIL && current->left->tree_hi > current->tree_hi) {
            current->tree_hi = current->left->tree_hi;
        }
        if (current->right != llrb_node::NIL && current->right->tree_hi > current->tree_hi) {
            current->tree_hi = current->right->tree_hi;
        }

        temp->tree_hi = temp->hi;
        if (temp->left != llrb_node::NIL && temp->left->tree_hi > temp->tree_hi) {
            temp->tree_hi = temp->left->tree_hi;
        }
        if (temp->right != llrb_node::NIL && temp->right->tree_hi > temp->tree_hi) {
            temp->tree_hi = temp->right->tree_hi;
        }

        return temp;
    }

    void flip_colours(llrb_node *current) {
        current->is_red = !current->is_red;
        if (current->left != llrb_node::NIL) {
            current->left->is_red = !current->left->is_red;
        }
        if (current->right != llrb_node::NIL) {
            current->right->is_red = !current->right->is_red;
        }
    }

    llrb_node *fix_up(llrb_node *current) {
        if (current->right->is_red && !current->left->is_red) {
            current = rotate_left(current);
        }
        if (current->left->is_red && current->left->left->is_red) {
            current = rotate_right(current);
        }
        if (current->left->is_red && current->right->is_red) {
            flip_colours(current);
        }

        current->size = 1 + current->left->size + current->right->size;

        current->tree_hi = current->hi;
        if (current->left != llrb_node::NIL && current->left->tree_hi > current->tree_hi) {
            current->tree_hi = current->left->tree_hi;
        }
        if (current->right != llrb_node::NIL && current->right->tree_hi > current->tree_hi) {
            current->tree_hi = current->right->tree_hi;
        }

        return current;
    }

    llrb_node *insert(llrb_node *newNode, llrb_node *current) {
        if (current == llrb_node::NIL) {
            return newNode;
        }

        if (newNode->lo == current->lo && newNode->hi == current->hi) {
            current->val = newNode->val;
            delete newNode;
            return current;
        } else if (newNode->lo < current->lo) {
            current->left = insert(newNode, current->left);
        } else {
            current->right = insert(newNode, current->right);
        }

        return fix_up(current);
    }

    llrb_node *move_red_left(llrb_node *current) {
        flip_colours(current);

        if (current->right->left->is_red) {
            current->right = rotate_right(current->right);
            current = rotate_left(current);
            flip_colours(current);
        }

        return current;
    }

    llrb_node *move_red_right(llrb_node *current) {
        flip_colours(current);

        if(current->left->left->is_red) {
            current = rotate_right(current);
            flip_colours(current);
        }

        return current;
    }

    llrb_node *del_min(llrb_node *current) {
        if (current->left == llrb_node::NIL) {
            delete current;
            return llrb_node::NIL;
        }

        if (!current->left->is_red && !current->left->left->is_red) {
            current = move_red_left(current);
        }

        current->left = del_min(current->left);

        return fix_up(current);
    }

    llrb_node *del(llrb_node *current, int lo, int hi) {
        if (lo < current->lo) {
            if (!current->left->is_red && !current->left->left->is_red) {
                current = move_red_left(current);
            }

            current->left = del(current->left, lo, hi);
        } else {
            if (current->left->is_red) {
                current = rotate_right(current);
            }

            if (lo == current->lo && hi == current->hi && current->right == llrb_node::NIL) {
                delete current;
                return llrb_node::NIL;
            }

            if (!current->right->is_red && !current->right->left->is_red) {
                current = move_red_right(current);
            }

            if (lo == current->lo && hi == current->hi) {
                llrb_node *successor = current->right;

                while (successor->left != llrb_node::NIL) {
                    successor = successor->left;
                }

                current->lo = successor->lo;
                current->hi = successor->hi;
                current->val = successor->val;

                current->right = del_min(current->right);
            } else {
                current->right = del(current->right, lo, hi);
            }
        }

        return fix_up(current);
    }

    // recursively find all intervals in the tree that intersect that query interval
    void find_intersections(llrb_node *current, int lo, int hi, std::vector<int> &intersections) const {
        if (current == llrb_node::NIL) {
            return;
        }

        if (current->lo <= hi && current->hi >= lo) {
            intersections.push_back(current->val);
        }

        if (current->left != llrb_node::NIL && current->left->tree_hi >= lo) {
            find_intersections(current->left, lo, hi, intersections);
        }

        if (current->lo > hi) return;

        if (current->right != llrb_node::NIL && current->right->tree_hi >= lo) {
            find_intersections(current->right, lo, hi, intersections);
        }
    }

    // called by destructor
    void destroy_subtree(llrb_node *current) noexcept {
        if (current != llrb_node::NIL) {
            destroy_subtree(current->left);
            destroy_subtree(current->right);
            delete current;
        }
    }

public:
    // default constructor
    interval_search_tree() {
        root = llrb_node::NIL;
    }

    // destructor
    ~interval_search_tree() noexcept {
        destroy_subtree(root);
    }

    // no copy constructor/assignment
    interval_search_tree(const interval_search_tree &) = delete;
    interval_search_tree &operator=(const interval_search_tree &) = delete;

    // move constructor
    interval_search_tree(interval_search_tree &&other) noexcept {
        root = other.root;
        other.root = llrb_node::NIL;
    }

    // move assignment
    interval_search_tree &operator=(interval_search_tree &&other) noexcept {
        if (this != &other) {
            destroy_subtree(root);
            root = other.root;
            other.root = llrb_node::NIL;
        }

        return *this;
    }

    // Add interval [lo, hi], or update its value if it is already in the tree
    void put(int lo, int hi, int val) {
        root = insert(new llrb_node(lo, hi, val), root);
        root->is_red = false;
    }

    // Get interval [lo, hi] if it exists in the tree
    std::optional<int> get(int lo, int hi) const {
        llrb_node *current = root;

        while (current != llrb_node::NIL) {
            if (lo == current->lo && hi == current->hi) {
                return current->val;
            }

            current = (lo < current->lo) ? current->left : current->right;
        }

        return std::nullopt;
    }

    // Delete interval [lo, hi] if it exists in the tree
    void del(int lo, int hi) {
        if (get(lo, hi)) {
            if (!root->left->is_red && !root->right->is_red) {
                root->is_red = true;
            }

            root = del(root, lo, hi);

            if (root != llrb_node::NIL) {
                root->is_red = false;
            }
        }
    }

    // Get all intervals that intersect with interval [lo, hi]
    std::vector<int> find_intersections(int lo, int hi) const {
        std::vector<int> intersections;
        find_intersections(root, lo, hi, intersections);
        return intersections;
    }
};

// initialising sentinel node used in interval search tree's llrb node
interval_search_tree::llrb_node *interval_search_tree::llrb_node::NIL = [] {
    static llrb_node sentinel;
    return &sentinel;
}();

// test client
int main() {
    std::vector<rectangle> rectangles;
    rectangles.emplace_back(0, point_2d(0, 2), point_2d(10, 4));
    rectangles.emplace_back(1, point_2d(1, 0), point_2d(7, 1));
    rectangles.emplace_back(2, point_2d(3, 6), point_2d(9, 9));
    rectangles.emplace_back(3, point_2d(4, 3), point_2d(6, 12));
    rectangles.emplace_back(4, point_2d(10, 4), point_2d(11, 5));

    std::priority_queue<event, std::vector<event>, std::greater<event>> events;

    for (rectangle &rect : rectangles) {
        events.push(event(rect, RECTANGLE_START));
        events.push(event(rect, RECTANGLE_END));
    }

    int intersections_counter = 0;
    interval_search_tree ist;
    while (!events.empty()) {
        const event &rect_event = events.top();
        if (rect_event.type == RECTANGLE_START) {
            std::vector<int> i = ist.find_intersections(rect_event.rect->bottom_left.y, rect_event.rect->top_right.y);
            if (!i.empty()) {
                std::cout << "Rect ID " << rect_event.rect->id <<" intersects rect IDs: ";
                for (int id : i) {
                    std::cout << id << " ";
                }
                std::cout << "\n";
            }
            intersections_counter += i.size();
            ist.put(rect_event.rect->bottom_left.y, rect_event.rect->top_right.y, rect_event.rect->id);
        } else {
            ist.del(rect_event.rect->bottom_left.y, rect_event.rect->top_right.y);
        }
        events.pop();
    }

    std::cout << "Total number of intersections: " << intersections_counter << "\n";
}
