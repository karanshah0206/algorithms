import edu.princeton.cs.algs4.WeightedQuickUnionUF;

// Model an n-by-n grid system to efficiently determine whether the system percolates.
 public class Percolation {
    private static final byte CLOSED = 0, OPEN = 1, TOP = 2, BOTTOM = 3, BOTH = 4;

    private final WeightedQuickUnionUF uf;
    private final byte[] grid;

    private int dimension;
    private int numberOfOpenSites;
    private boolean percolates;

    // creates an n-by-n grid, with all sites initially blocked
    public Percolation(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException("System must contain at least one site.");
        }

        dimension = n;
        numberOfOpenSites = 0;
        percolates = false;

        uf = new WeightedQuickUnionUF(dimension * dimension + 1);

        grid = new byte[dimension * dimension + 1];
        for (int i = 0; i < dimension * dimension; i++) {
            grid[i] = CLOSED;
        }
        grid[dimension * dimension] = TOP;
    }

    // opens site (row, col) if it isn't already open
    public void open(int row, int col) {
        int siteIndex = coordsToIndex(row, col);
        if (grid[siteIndex] == CLOSED) {
            grid[siteIndex] = OPEN;
            numberOfOpenSites++;

            // edge cases for top/bottom row sites
            if (row == 1 && row == dimension) {
                percolates = true;
                grid[siteIndex] = BOTH;
                grid[dimension * dimension] = BOTH;
                uf.union(siteIndex, dimension * dimension);
                grid[uf.find(siteIndex)] = BOTH;
                grid[uf.find(dimension * dimension)] = BOTH;
            } else if (row == 1) {
                grid[siteIndex] = TOP;
                uf.union(siteIndex, dimension * dimension);
                grid[uf.find(siteIndex)] = TOP;
                grid[uf.find(dimension * dimension)] = TOP;
            } else if (row == dimension) {
                grid[siteIndex] = BOTTOM;
            }

            // up
            if (row > 1 && isOpen(row - 1, col)) {
                unionizeNeighbours(uf.find(siteIndex), uf.find(coordsToIndex(row - 1, col)));
            }

            // down
            if (row < dimension && isOpen(row + 1, col)) {
                unionizeNeighbours(uf.find(siteIndex), uf.find(coordsToIndex(row + 1, col)));
            }

            // left
            if (col > 1 && isOpen(row, col - 1)) {
                unionizeNeighbours(uf.find(siteIndex), uf.find(coordsToIndex(row, col - 1)));
            }

            // right
            if (col < dimension && isOpen(row, col + 1)) {
                unionizeNeighbours(uf.find(siteIndex), uf.find(coordsToIndex(row, col + 1)));
            }
        }
    }

    // checks whether site (row, col) is open
    public boolean isOpen(int row, int col) {
        return grid[coordsToIndex(row, col)] != CLOSED;
    }

    // checks whether site (row, col) is full
    public boolean isFull(int row, int col) {
        return uf.find(coordsToIndex(row, col)) == uf.find(dimension * dimension);
    }

    // checks if the system percolates
    public boolean percolates() {
        return percolates;
    }

    // returns the number of open sites
    public int numberOfOpenSites() {
        return numberOfOpenSites;
    }

    // establish a workers' union to rant about the management
    private void unionizeNeighbours(int siteIndex, int neighbourIndex) {
        if (grid[siteIndex] == grid[neighbourIndex]) {
            uf.union(siteIndex, neighbourIndex);
            grid[uf.find(siteIndex)] = grid[siteIndex];
            grid[uf.find(neighbourIndex)] = grid[siteIndex];
        } else if (grid[siteIndex] == OPEN) {
            grid[siteIndex] = grid[neighbourIndex];
            uf.union(siteIndex, neighbourIndex);
            grid[uf.find(siteIndex)] = grid[neighbourIndex];
            grid[uf.find(neighbourIndex)] = grid[neighbourIndex];
        } else if (grid[neighbourIndex] == OPEN) {
            grid[neighbourIndex] = grid[siteIndex];
            uf.union(siteIndex, neighbourIndex);
            grid[uf.find(siteIndex)] = grid[siteIndex];
            grid[uf.find(neighbourIndex)] = grid[siteIndex];
        } else {
            percolates = true;
            grid[neighbourIndex] = BOTH;
            grid[siteIndex] = BOTH;
            uf.union(siteIndex, neighbourIndex);
            grid[uf.find(neighbourIndex)] = BOTH;
            grid[uf.find(siteIndex)] = BOTH;
        }
    }

    // convert from 2d indexing to 1d
    private int coordsToIndex(int row, int col) {
        if (row < 1 || col < 1 || row > dimension || col > dimension) {
            throw new IllegalArgumentException("Site coordinates out of bounds.");
        }

        return (col - 1) + (row - 1) * dimension;
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
