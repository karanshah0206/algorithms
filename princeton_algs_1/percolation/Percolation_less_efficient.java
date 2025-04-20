import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private final int n;
    private final boolean[][] grid;
    private final WeightedQuickUnionUF unionFind, unionFindWOBottomSentinel;

    private int openSitesCount;

    // creates an n-by-n grid, with all sites initially blocked
    public Percolation(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException("System must have at least one site.");
        }

        this.n = n;
        grid = new boolean[n][n];
        openSitesCount = 0;
        unionFind = new WeightedQuickUnionUF(n * n + 2);
        unionFindWOBottomSentinel = new WeightedQuickUnionUF(n * n + 1);
    }

    // opens site (row, col) if it isn't already open
    public void open(int row, int col) {
        if (row <= 0 || row > n || col <= 0 || col > n) {
            throw new IllegalArgumentException("Invalid site coordinates.");
        }

        if (isOpen(row, col)) {
            return;
        }

        grid[row - 1][col - 1] = true;
        openSitesCount++;
        int myIndex = (col - 1) + (row - 1) * n;

        // up
        if (row != 1 && grid[row - 2][col - 1]) {
            int theirIndex = (col - 1) + (row - 2) * n;
            unionFind.union(myIndex, theirIndex);
            unionFindWOBottomSentinel.union(myIndex, theirIndex);
        }

        // down
        if (row != n && grid[row][col - 1]) {
            int theirIndex = (col - 1) + row * n;
            unionFind.union(myIndex, theirIndex);
            unionFindWOBottomSentinel.union(myIndex, theirIndex);
        }

        // left
        if (col != 1 && grid[row - 1][col - 2]) {
            int theirIndex = (col - 2) + (row - 1) * n;
            unionFind.union(myIndex, theirIndex);
            unionFindWOBottomSentinel.union(myIndex, theirIndex);
        }

        // right
        if (col != n && grid[row - 1][col]) {
            int theirIndex = col + (row - 1) * n;
            unionFind.union(myIndex, theirIndex);
            unionFindWOBottomSentinel.union(myIndex, theirIndex);
        }

        // top row to sentinel
        if (row == 1) {
            unionFind.union(myIndex, n * n);
            unionFindWOBottomSentinel.union(myIndex, n * n);
        }

        // bottom row to sentinel
        if (row == n) {
            unionFind.union(myIndex, n * n + 1);
        }
    }

    // checks whether site (row, col) is open
    public boolean isOpen(int row, int col) {
        if (row <= 0 || row > n || col <= 0 || col > n) {
            throw new IllegalArgumentException("Invalid site coordinates.");
        }

        return grid[row - 1][col - 1];
    }

    // checks whether site (row, col) is full
    public boolean isFull(int row, int col) {
        if (row <= 0 || row > n || col <= 0 || col > n) {
            throw new IllegalArgumentException("Invalid site coordinates.");
        }

        int myIndex = (col - 1) + (row - 1) * n;
        return unionFindWOBottomSentinel.find(myIndex) == unionFindWOBottomSentinel.find(n * n);
    }

    // returns the number of open sites
    public int numberOfOpenSites() {
        return openSitesCount;
    }

    // checks if the system percolates
    public boolean percolates() {
        return unionFind.find(n * n) == unionFind.find(n * n + 1);
    }

    // test client
    public static void main(String[] args) {
        Percolation p = new Percolation(8);
        p.open(1, 3);
        p.open(1, 4);
        p.open(1, 5);
        p.open(2, 1);
        p.open(2, 4);
        p.open(2, 5);
        p.open(2, 6);
        p.open(2, 7);
        p.open(2, 8);
        p.open(3, 1);
        p.open(3, 2);
        p.open(3, 3);
        p.open(3, 6);
        p.open(3, 7);
        p.open(4, 3);
        p.open(4, 4);
        p.open(4, 6);
        p.open(4, 7);
        p.open(4, 8);
        p.open(5, 2);
        p.open(5, 3);
        p.open(5, 4);
        p.open(5, 6);
        p.open(5, 7);
        p.open(6, 2);
        p.open(6, 7);
        p.open(6, 8);
        p.open(7, 1);
        p.open(7, 3);
        p.open(7, 5);
        p.open(7, 6);
        p.open(7, 7);
        p.open(7, 8);
        p.open(8, 1);
        p.open(8, 2);
        p.open(8, 3);
        p.open(8, 4);
        p.open(8, 6);

        System.out.println(p.percolates());
        // true

        for (int i = 1; i <= 8; i++) {
            if (p.isOpen(8, i)) {
                System.out.println(i + ": " + p.isFull(8, i));
            }
        }
        // 1: false
        // 2: false
        // 3: false
        // 4: false
        // 6: true
    }
}
