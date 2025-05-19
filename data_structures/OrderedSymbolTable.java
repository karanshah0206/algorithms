import java.util.Random;
import java.util.LinkedList;
import java.util.Queue;

public class OrderedSymbolTable<Key extends Comparable<Key>, Value> {
    private final static Random random = new Random();

    // Internal class representing a node in a binary search tree
    private class Node {
        Key key;
        Value value;
        Node left, right;
        int size;

        Node(Key key, Value value) {
            this.key = key;
            this.value = value;
            size = 1;
        }
    }

    private Node root;

    // Put key-vaue pair into the table
    public void put(Key key, Value value) {
        root = insert(root, new Node(key, value));
    }

    // Get value paired with given key (null if key not found)
    public Value get(Key key) {
        Node current = root;

        while (current != null) {
            int comparison = key.compareTo(current.key);

            if (comparison == 0) {
                return current.value;
            }

            current = (comparison < 0) ? current.left : current.right;
        }

        return null;
    }

    // Is the table empty?
    public boolean isEmpty() {
        return root == null;
    }

    // Is there a value paired with given key?
    public boolean contains(Key key) {
        return get(key) != null;
    }

    // Number of key-value pairs in table
    public int size() {
        return size(root);
    }

    // Smallest key in table (null if empty)
    public Key getMinimum() {
        if (isEmpty()) {
            return null;
        }

        Node current = root;

        while (true) {
            if (current.left == null) {
                return current.key;
            }

            current = current.left;
        }
    }

    // Largest key in table (null if empty)
    public Key getMaximum() {
        if (isEmpty()) {
            return null;
        }

        Node current = root;

        while (true) {
            if (current.right == null) {
                return current.key;
            }

            current = current.right;
        }
    }

    // Largest key in table less than or equal to given key (null if doesn't exist)
    public Key getFloor(Key key) {
        Node current = root;
        Key result = null;

        while (current != null) {
            int comparison = key.compareTo(current.key);

            if (comparison == 0) {
                result = current.key;
                break;
            }

            if (comparison > 0) {
                result = current.key;
                current = current.right;
            } else {
                current = current.left;
            }
        }

        return result;
    }

    // Smallest key in table greater than or equal to given key (null if doesn't exist)
    public Key getCeiling(Key key) {
        Node current = root;
        Key result = null;

        while (current != null) {
            int comparison = key.compareTo(current.key);

            if (comparison == 0) {
                result = current.key;
                break;
            }

            if (comparison < 0) {
                result = current.key;
                current = current.left;
            } else {
                current = current.right;
            }
        }

        return result;
    }

    // Number of keys less than given key
    public int rank(Key key) {
        Node current = root;
        int result = 0;

        while (current != null) {
            int comparison = key.compareTo(current.key);

            if (comparison == 0) {
                result += size(current.left);
                break;
            }

            if (comparison < 0) {
                current = current.left;
            } else {
                result += 1 + size(current.left);
                current = current.right;
            }
        }

        return result;
    }

    // Key at given rank (null of rank beyond table size bounds)
    public Key select(int rank) {
        Node current = root;
        Key result = null;

        while (current != null) {
            int smallerItemsCount = size(current.left);

            if (smallerItemsCount == rank) {
                result = current.key;
                break;
            }

            if (smallerItemsCount > rank) {
                current = current.left;
            } else {
                rank -= smallerItemsCount + 1;
                current = current.right;
            }
        }

        return result;
    }

    // Remove pair keyed at the given key in the table
    public void delete(Key key) {
        root = delete(root, key);
    }

    // Remove pair keyed at the smallest key in the table
    public void deleteMinimum() {
        if (!isEmpty()) {
            root = deleteMin(root);
        }
    }

    // Remove pair keyed at the largest key in the table
    public void deleteMaximum() {
        if (!isEmpty()) {
            root = deleteMax(root);
        }
    }

    // All keys in table in sorted order
    public Iterable<Key> keys() {
        // Queue<Key> orderedKeys = new Queue<>();
        Queue<Key> orderedKeys = new LinkedList<Key>();
        inorderTraverse(root, orderedKeys);
        return orderedKeys;
    }

    // Helper to determine number of nodes in binary search tree rooted at given node
    private int size(Node node) {
        return (node == null) ? 0 : node.size;
    }

    // Helper to insert node at appropriate position in binary search tree
    private Node insert(Node current, Node newNode) {
        if (current == null) {
            return newNode;
        }

        int comparison = newNode.key.compareTo(current.key);

        if (comparison == 0) {
            current.value = newNode.value;
        } else if (comparison < 0) {
            current.left = insert(current.left, newNode);
        } else {
            current.right = insert(current.right, newNode);
        }

        current.size = 1 + size(current.left) + size(current.right);

        return current;
    }

    // Helper to delete node keyed at target key from binary search tree
    private Node delete(Node current, Key target) {
        if (current != null) {
            int comparison = current.key.compareTo(target);

            if (comparison < 0) {
                current.right = delete(current.right, target);
            } else if (comparison > 0) {
                current.left = delete(current.left, target);
            } else {
                if (current.size == 1) {
                    return null;
                } else if (current.left == null) {
                    current = current.right;
                } else if (current.right == null) {
                    current = current.left;
                } else {
                    Node currentInSubtree = current.right;
                    Node successor = current.right;

                    while (true) {
                        if (currentInSubtree.left == null) {
                            successor = currentInSubtree;
                            break;
                        }

                        currentInSubtree = currentInSubtree.left;
                    }

                    current.key = successor.key;
                    current.value = successor.value;
                    current.right = deleteMin(current.right);
                }
            }

            current.size = 1 + size(current.left) + size(current.right);
        }

        return current;
    }

    // Helper to delete node with smallest key from binary search tree
    private Node deleteMin(Node current) {
        if (current.left == null) {
            current = current.right;
        } else {
            current.left = deleteMin(current.left);
            current.size = 1 + size(current.left) + size(current.right);
        }
        return current;
    }

    // Helper to delete node with largest key from binary search tree
    private Node deleteMax(Node current) {
        if (current.right == null) {
            current = current.left;
        } else {
            current.right = deleteMax(current.right);
            current.size = 1 + size(current.left) + size(current.right);
        }
        return current;
    }

    // Helper to traverse binary search tree nodes in order
    private void inorderTraverse(Node current, Queue<Key> queue) {
        if (current != null) {
            inorderTraverse(current.left, queue);
            // queue.enqueue(current.key);
            queue.add(current.key);
            inorderTraverse(current.right, queue);
        }
    }

    // Test client
    public static void main(String[] args) {
        OrderedSymbolTable<Integer, Integer> bst = new OrderedSymbolTable<Integer, Integer>();

        // Create symbol table
        System.out.print("Input sequence:\t");
        while (bst.size() < 20) {
            int randomInputInt = -100 + random.nextInt(101 + 100);
            if (!bst.contains(randomInputInt)) {
                System.out.print(randomInputInt + " ");
                bst.put(randomInputInt, randomInputInt);
            }
        }

        // Iterate over all keys
        System.out.print("\nKeys in table:\t");
        for (Integer k : bst.keys()) System.out.print(k + " ");
        System.out.print("\nValues at key:\t");
        for (Integer k : bst.keys()) System.out.print(bst.get(k) + " ");

        // Show table stats
        System.out.println("\n\nTable size: " + bst.size());
        System.out.println("Min key in table: " + bst.getMinimum());
        System.out.println("Max key in table: " + bst.getMaximum());

        // Test select and rank methods
        int i = random.nextInt(bst.size());
        int selectionResult = bst.select(i);
        System.out.println("\nSelect key at rank " + i + ": " + selectionResult);
        System.out.println("Get rank of key " + selectionResult + ": " + bst.rank(selectionResult));

        // Test floor and ceiling methods
        i = -101 + random.nextInt(102 + 101);
        System.out.println("\nKey when flooring with " + i + ": " + bst.getFloor(i));
        System.out.println("Key when ceiling with " + i + ": " + bst.getCeiling(i));

        // Testing delete
        System.out.println("\nDeleting key " + selectionResult);
        bst.delete(selectionResult);
        System.out.print("Keys in table:\t");
        for (Integer k : bst.keys()) System.out.print(k + " ");
        System.out.print("\nValues at key:\t");
        for (Integer k : bst.keys()) System.out.print(bst.get(k) + " ");
        System.out.println("\nTable size: " + bst.size());

        // Testing delete minimum
        System.out.println("\nDeleting min key");
        bst.deleteMinimum();
        System.out.print("Keys in table:\t");
        for (Integer k : bst.keys()) System.out.print(k + " ");
        System.out.print("\nValues at key:\t");
        for (Integer k : bst.keys()) System.out.print(bst.get(k) + " ");
        System.out.println("\nTable size: " + bst.size());

        // Testing delete maximum
        System.out.println("\nDeleting max key");
        bst.deleteMaximum();
        System.out.print("Keys in table:\t");
        for (Integer k : bst.keys()) System.out.print(k + " ");
        System.out.print("\nValues at key:\t");
        for (Integer k : bst.keys()) System.out.print(bst.get(k) + " ");
        System.out.println("\nTable size: " + bst.size());
    }
}
