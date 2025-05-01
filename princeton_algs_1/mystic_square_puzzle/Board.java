import java.util.ArrayList;

public class Board {
    private final int dimension;
    private final int[][] tiles;
    private final boolean isGoal;

    private int hamming;

    // create a board from an n-by-n array of tiles,
    // where tiles[row][col] = tile at (row, col)
    public Board(int[][] tiles) {
        dimension = tiles.length;
        this.tiles = deepCloneTiles(dimension, tiles);

        hamming = 0;
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if (this.tiles[i][j] != j + dimension * i + 1 && this.tiles[i][j] != 0) {
                    hamming++;
                }
            }
        }

        isGoal = hamming == 0;
    }

    // string representation of this board
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append(dimension);
        stringBuilder.append("\n");
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                stringBuilder.append(tiles[i][j]);
                stringBuilder.append(" ");
            }
            stringBuilder.append("\n");
        }
        return stringBuilder.toString();
    }

    // returns the board's dimension
    public int dimension() {
        return dimension;
    }

    // returns the number of tiles out of place
    public int hamming() {
        return hamming;
    }

    // returns the sum of Manhattan distances between tiles and goal
    public int manhattan() {
        int netManhattanDistance = 0;
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if (tiles[i][j] != 0) {
                    netManhattanDistance += Math.abs(i - ((tiles[i][j] - 1) / dimension)) + Math.abs(j - ((tiles[i][j] - 1) % dimension));
                }
            }
        }
        return netManhattanDistance;
    }

    // is this board in the goal state?
    public boolean isGoal() {
        return isGoal;
    }

    // is this board same as the Object y?
    public boolean equals(Object y) {
        if (this == y) {
            return true;
        }

        if (y == null || y.getClass() != getClass()) {
            return false;
        }

        Board other = (Board) y;

        if (other.dimension != dimension) {
            return false;
        }

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if (tiles[i][j] != other.tiles[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    // iterable collection of all neighbor boards
    public Iterable<Board> neighbors() {
        ArrayList<Board> neighbors = new ArrayList<Board>();

        int blankRowIdx = 0, blankColIdx = 0;
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if (tiles[i][j] == 0) {
                    blankRowIdx = i;
                    blankColIdx = j;
                    break;
                }
            }
        }

        if (blankRowIdx > 0) {
            int[][] deepClonedTiles = deepCloneTiles(dimension, tiles);
            deepClonedTiles[blankRowIdx][blankColIdx] = deepClonedTiles[blankRowIdx - 1][blankColIdx];
            deepClonedTiles[blankRowIdx - 1][blankColIdx] = 0;

            neighbors.add(new Board(deepClonedTiles));
        }

        if (blankRowIdx + 1 < dimension) {
            int[][] deepClonedTiles = deepCloneTiles(dimension, tiles);
            deepClonedTiles[blankRowIdx][blankColIdx] = deepClonedTiles[blankRowIdx + 1][blankColIdx];
            deepClonedTiles[blankRowIdx + 1][blankColIdx] = 0;

            neighbors.add(new Board(deepClonedTiles));
        }

        if (blankColIdx > 0) {
            int[][] deepClonedTiles = deepCloneTiles(dimension, tiles);
            deepClonedTiles[blankRowIdx][blankColIdx] = deepClonedTiles[blankRowIdx][blankColIdx - 1];
            deepClonedTiles[blankRowIdx][blankColIdx - 1] = 0;

            neighbors.add(new Board(deepClonedTiles));
        }

        if (blankColIdx + 1 < dimension) {
            int[][] deepClonedTiles = deepCloneTiles(dimension, tiles);
            deepClonedTiles[blankRowIdx][blankColIdx] = deepClonedTiles[blankRowIdx][blankColIdx + 1];
            deepClonedTiles[blankRowIdx][blankColIdx + 1] = 0;

            neighbors.add(new Board(deepClonedTiles));
        }

        return neighbors;
    }

    // returns a board obtained by exchanging any pair of tiles
    public Board twin() {
        int[][] deepClonedTiles = deepCloneTiles(dimension, tiles);

        int i1 = 0, j1 = 0, i2 = 1, j2 = 1;
        if (deepClonedTiles[i1][j1] == 0) j1++;
        else if (deepClonedTiles[i2][j2] == 0) j2--;

        int temp = deepClonedTiles[i1][j1];
        deepClonedTiles[i1][j1] = deepClonedTiles[i2][j2];
        deepClonedTiles[i2][j2] = temp;

        return new Board(deepClonedTiles);
    }

    private int[][] deepCloneTiles(int tilesDimension, int[][] someTiles) {
        int[][] deepClonedTiles = new int[tilesDimension][tilesDimension];
        for (int i = 0; i < tilesDimension; i++) {
            for (int j = 0; j < tilesDimension; j++) {
                deepClonedTiles[i][j] = someTiles[i][j];
            }
        }
        return deepClonedTiles;
    }
}
