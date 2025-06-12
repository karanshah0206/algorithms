import java.util.Queue;
import java.util.LinkedList;

// Implementation of an ordered symbol table using left-leaning red-black tree data structure
public class LLRBSymbolTable<Key extends Comparable<Key>, Value> {
    private class Node {
        Key key;
        Value value;
        Node left, right;
        int size;
        boolean isRed;

        Node(Key key, Value value) {
            this.key = key;
            this.value = value;
            size = 1;
            isRed = true;
        }
    }

    private Node root;

    // Put key-value pair into the table
    public void put(Key key, Value value) {
        root = insert(root, new Node(key, value));
        root.isRed = false;
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

        while (current.left != null) {
            current = current.left;
        }

        return current.key;
    }

    // Largest key in table (null if empty)
    public Key getMaximum() {
        if (isEmpty()) {
            return null;
        }

        Node current = root;

        while (current.right != null) {
            current = current.right;
        }

        return current.key;
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
        int rank = 0;

        while (current != null) {
            int comparison = key.compareTo(current.key);

            if (comparison == 0) {
                rank += size(current.left);
                break;
            }

            if (comparison > 0) {
                rank += 1 + size(current.left);
                current = current.right;
            } else {
                current = current.left;
            }
        }

        return rank;
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
                rank -= 1 + smallerItemsCount;
                current = current.right;
            }
        }

        return result;
    }

    // Remove pair keyed at the given key in the table
    public void delete(Key key) {
        if (contains(key)) {
            if (!isRed(root.left) && !isRed(root.right)) {
                root.isRed = true;
            }

            root = delete(root, key);

            if (!isEmpty()) {
                root.isRed = false;
            }
        }
    }

    // Remove pair keyed at the smallest key in the table
    public void deleteMinimum() {
        if (!isEmpty()) {
            if (!isRed(root.left) && !isRed(root.right)) {
                root.isRed = true;
            }

            root = deleteMin(root);

            if (!isEmpty()) {
                root.isRed = false;
            }
        }
    }

    // Remove pair keyed at the largest key in the table
    public void deleteMaximum() {
        if (!isEmpty()) {
            if (!isRed(root.left) && !isRed(root.right)) {
                root.isRed = true;
            }

            root = deleteMax(root);

            if (!isEmpty()) {
                root.isRed = false;
            }
        }
    }

    // All keys in table in sorted order
    public Iterable<Key> keys() {
        Queue<Key> orderedKeys = new LinkedList<Key>();
        inorderTraverse(root, orderedKeys);
        return orderedKeys;
    }

    // Helper to determine whether given node is connected to parent via red link
    private boolean isRed(Node node) {
        return (node == null) ? false : node.isRed;
    }

    // Helper to determine number of nodes in tree rooted at given node
    private int size(Node node) {
        return (node == null) ? 0 : node.size;
    }

    // Helper to orient a right-leaning red link to lean left
    private Node rotateLeft(Node node) {
        assert isRed(node.right);

        Node temp = node.right;

        node.right = temp.left;
        temp.left = node;

        temp.isRed = node.isRed;
        node.isRed = true;

        temp.size = 1 + size(temp.left) + size(temp.right);
        node.size = 1 + size(node.left) + size(node.right);

        return temp;
    }

    // Helper to orient a left-leaning red link to (temporarily) lean right
    private Node rotateRight(Node node) {
        assert isRed(node.left);

        Node temp = node.left;

        node.left = temp.right;
        temp.right = node;

        temp.isRed = node.isRed;
        node.isRed = true;

        temp.size = 1 + size(temp.left) + size(temp.right);
        node.size = 1 + size(node.left) + size(node.right);

        return temp;
    }

    // Helper to reinstate black height-balance when both children become red-linked
    private void flipColours(Node node) {
        node.isRed = !node.isRed;
        node.left.isRed = !node.left.isRed;
        node.right.isRed = !node.right.isRed;
    }

    // Helper to rebalance right-leaning red links or nodes with multiple red links
    private Node fixUp(Node current) {
        if (isRed(current.right) && !isRed(current.left)) {
            current = rotateLeft(current);
        }

        if (isRed(current.left) && isRed(current.left.left)) {
            current = rotateRight(current);
        }

        if (isRed(current.left) && isRed(current.right)) {
            flipColours(current);
        }

        current.size = 1 + size(current.left) + size(current.right);

        return current;
    }

    // Helper to insert node in tree while maintaining perfect black height-balance
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

        return fixUp(current);
    }

    // Helper to ensure the left child is part of a 3-node during recursive deletion
    private Node moveRedLeft(Node current) {
        assert isRed(current) && !isRed(current.left) && !isRed(current.left.left);

        flipColours(current);

        if (isRed(current.right.left)) {
            current.right = rotateRight(current.right);
            current = rotateLeft(current);
            flipColours(current);
        }

        return current;
    }

    // Helper to ensure the right child is part of a 3-node during recursive deletion
    private Node moveRedRight(Node current) {
        assert isRed(current) && !isRed(current.right) && !isRed(current.right.left);

        flipColours(current);

        if (isRed(current.left.left)) {
            current = rotateRight(current);
            flipColours(current);
        }

        return current;
    }

    // Helper to delete node keyed at target key from tree
    private Node delete(Node current, Key target) {
        if (target.compareTo(current.key) < 0) {
            if (!isRed(current.left) && !isRed(current.left.left)) {
                current = moveRedLeft(current);
            }

            current.left = delete(current.left, target);
        } else {
            if (isRed(current.left)) {
                current = rotateRight(current);
            }

            if (target.compareTo(current.key) == 0 && current.right == null) {
                return null;
            }

            if (!isRed(current.right) && !isRed(current.right.left)) {
                current = moveRedRight(current);
            }

            if (target.compareTo(current.key) == 0) {
                Node successor = current.right;

                while (successor.left != null) {
                    successor = successor.left;
                }

                current.key = successor.key;
                current.value = successor.value;

                current.right = deleteMin(current.right);
            } else {
                current.right = delete(current.right, target);
            }
        }

        return fixUp(current);
    }

    // Helper to delete node with smallest key from tree
    private Node deleteMin(Node current) {
        if (current.left == null) {
            return null;
        }

        if (!isRed(current.left) && !isRed(current.left.left)) {
            current = moveRedLeft(current);
        }

        current.left = deleteMin(current.left);

        return fixUp(current);
    }

    // Helper to delete node with largest key from tree
    private Node deleteMax(Node current) {
        if (isRed(current.left)) {
            current = rotateRight(current);
        }

        if (current.right == null) {
            return null;
        }

        if (!isRed(current.right) && !isRed(current.right.left)) {
            current = moveRedRight(current);
        }

        current.right = deleteMax(current.right);

        return fixUp(current);
    }

    // Helper to traverse tree nodes in order
    private void inorderTraverse(Node current, Queue<Key> queue) {
        if (current != null) {
            inorderTraverse(current.left, queue);
            queue.add(current.key);
            inorderTraverse(current.right, queue);
        }
    }

    // Test client
    public static void main(String[] args) {
        LLRBSymbolTable<Character, Character> rbt = new LLRBSymbolTable<Character, Character>();

        rbt.put('S', 'S');
        rbt.put('E', 'E');
        rbt.put('A', 'A');
        rbt.put('R', 'R');
        rbt.put('C', 'C');
        rbt.put('H', 'H');
        rbt.put('X', 'X');
        rbt.put('M', 'M');
        rbt.put('P', 'P');
        rbt.put('L', 'L');

        int i = 0;

        while (!rbt.isEmpty()) {
            if (i == 0) {
                System.out.println("\nInitial status:\n");
            } else if (i == 1) {
                System.out.println("\nPutting key K\n");
                rbt.put('K', 'K');
            } else if (i == 2) {
                System.out.println("\nDeleting key K\n");
                rbt.delete('K');
            } else if (i % 2 == 0) {
                System.out.println("\nDeleting smallest key\n");
                rbt.deleteMinimum();
            } else {
                System.out.println("\nDeleting largest key\n");
                rbt.deleteMaximum();
            }
            i++;

            System.out.println("Size: " + rbt.size());
            System.out.print("In order: ");
            for (Character c : rbt.keys()) {
                System.out.print(c + " ");
            }
            System.out.println("\nSmallest key: " + rbt.getMinimum());
            System.out.println("Largest key: " + rbt.getMaximum());
            System.out.println("Floor for K: " + rbt.getFloor('K'));
            System.out.println("Ceiling for K: " + rbt.getCeiling('K'));
            System.out.println("Rank for K: " + rbt.rank('K'));
            System.out.println("Selection at 4: " + rbt.select(4));
            System.out.println("Contains K: " + rbt.contains('K'));
        }
    }
}
