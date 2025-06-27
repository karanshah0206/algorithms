// Requires C++20 or above

#include <iostream>
#include <concepts>
#include <vector>
#include <optional>

// A balanced tree data structure to hold intervals and values associated with them.
// Efficiently insert, delete, and fetch intervals.
// Efficiently identify intersecting interval(s) with query interval.
template<std::totally_ordered Key, typename Value>
class interval_search_tree {
private:
    // using a left-leaning red-black (LLRB) tree to maintain a balanced interval tree
    struct llrb_node {
        // `tree_hi` stores the largest `hi` key in the tree rooted at this node
        Key lo, hi, tree_hi;
        Value val;
        llrb_node *left, *right;
        int size;
        bool is_red;

        // static reference to sentinel node
        static llrb_node *NIL;

        // sentinel constructor
        llrb_node() : size(0), is_red(false), left(this), right(this) {}

        llrb_node(Key lo, Key hi, Value val) :
            lo(lo), hi(hi), val(val), tree_hi(hi),
            size(1), is_red(true), left(NIL), right(NIL) {}
    };

    llrb_node *root;

    // LLRB Helper: perform a left-rotation on the given node
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

    // LLRB Helper: perform a right-rotation on the given node
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

    // LLRB Helper: flip colour of given node and its immediate children (if any)
    void flip_colours(llrb_node *current) {
        current->is_red = !current->is_red;
        if (current->left != llrb_node::NIL) {
            current->left->is_red = !current->left->is_red;
        }
        if (current->right != llrb_node::NIL) {
            current->right->is_red = !current->right->is_red;
        }
    }

    // LLRB Helper: rebalances right-leaning/double-sided red links, and recomputes size and tree_hi on given node
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

    // LLRB Helper: insert node into tree while maintaining perfect black height-balance
    llrb_node *insert(llrb_node *newNode, llrb_node *current) {
        if (current == llrb_node::NIL) {
            return newNode;
        }

        if (newNode->lo == current->lo && newNode->hi == current->hi) {
            current->val = newNode->val;
        } else if (newNode->lo < current->lo) {
            current->left = insert(newNode, current->left);
        } else {
            current->right = insert(newNode, current->right);
        }

        return fix_up(current);
    }

    // LLRB Helper: ensures left child is isomorphic to a member of a 3-node in a 2-3 tree during recursive deletion
    llrb_node *move_red_left(llrb_node *current) {
        flip_colours(current);

        if (current->right->left->is_red) {
            current->right = rotate_right(current->right);
            current = rotate_left(current);
            flip_colours(current);
        }

        return current;
    }

    // LLRB Helper: ensures right child is isomorphic to a member of a 3-node in a 2-3 tree during recursive deletion
    llrb_node *move_red_right(llrb_node *current) {
        flip_colours(current);

        if(current->left->left->is_red) {
            current = rotate_right(current);
            flip_colours(current);
        }

        return current;
    }

    // LLRB Helper: deletes the node with the smallest key in the tree rooted on the give node
    llrb_node *del_min(llrb_node *current) {
        if (current->left == llrb_node::NIL) {
            return llrb_node::NIL;
        }

        if (!current->left->is_red && !current->left->left->is_red) {
            current = move_red_left(current);
        }

        current->left = del_min(current->left);

        return fix_up(current);
    }

    // LLRB Helper: deletes the node with keyed on the [lo, hi] interval
    llrb_node *del(llrb_node *current, Key lo, Key hi) {
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
    void find_intersections(llrb_node *current, Key lo, Key hi, std::vector<Value> &intersections) const {
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
    void destroy_subtree(llrb_node *current) {
        if (current != llrb_node::NIL) {
            destroy_subtree(current->left);
            destroy_subtree(current->right);
            delete current;
        }
    }

public:
    interval_search_tree() {
        root = llrb_node::NIL;
    }

    ~interval_search_tree() {
        destroy_subtree(root);
    }

    // Add interval [lo, hi], or update its value if it is already in the tree
    void put(Key lo, Key hi, Value val) {
        root = insert(new llrb_node(lo, hi, val), root);
        root->is_red = false;
    }

    // Get interval [lo, hi] if it exists in the tree
    std::optional<Value> get(Key lo, Key hi) const {
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
    void del(Key lo, Key hi) {
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
    std::vector<Value> find_intersections(Key lo, Key hi) const {
        std::vector<Value> intersections;
        find_intersections(root, lo, hi, intersections);
        return intersections;
    }

    // Get an interval that intersects with interval [lo, hi]
    std::optional<Value> find_intersection(Key lo, Key hi) const {
        llrb_node *current = root;
        while (current != llrb_node::NIL) {
            if (current->lo <= hi && current->hi >= lo) {
                return current->val;
            }

            if (current->left == llrb_node::NIL) {
                current = current->right;
            } else if (current->left->tree_hi < lo) {
                current = current->right;
            } else {
                current = current->left;
            }
        }

        return std::nullopt;
    }
};

// initialising sentinel node
template<std::totally_ordered Key, typename Value>
typename interval_search_tree<Key, Value>::llrb_node
    *interval_search_tree<Key, Value>::llrb_node::NIL =
    new interval_search_tree<Key, Value>::llrb_node();

// test client
int main() {
    interval_search_tree<int, std::string> ist;

    ist.put(21, 24, "(21, 24)");
    ist.put(5, 8, "(5, 8)");
    ist.put(17, 19, "(17, 19)");
    ist.put(4, 8, "(4, 8)");
    ist.put(15, 18, "(15, 18)");
    ist.put(7, 10, "(7, 10)");
    ist.put(16, 22, "(16, 22)");

    std::pair<int, int> intersection_query = { 21, 23 };
    // check single intersection interval
    std::optional<std::string> intersection_result = ist.find_intersection(intersection_query.first, intersection_query.second);
    if (intersection_result) {
        std::cout << *intersection_result << "\n";
    } else {
        std::cout << "No interval intersecting query interval.\n";
    }
    // check all intersection intervals
    for (std::string &s : ist.find_intersections(intersection_query.first, intersection_query.second)) {
        std::cout << s << " ";
    }
    std::cout << "\n";
}
