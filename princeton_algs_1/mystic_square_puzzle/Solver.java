import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.StdOut;

public class Solver {
    // internal search node data structure for use with A* game tree
    private class SearchNode implements Comparable<SearchNode> {
        final SearchNode parent;
        final Board board;
        final int manhattan;
        int moves;

        SearchNode(SearchNode parent, Board board, int moves) {
            this.parent = parent;
            this.board = board;
            manhattan = board.manhattan();
            this.moves = moves;
        }

        public int compareTo(SearchNode other) {
            return (moves + manhattan) - (other.moves + other.manhattan);
        }
    }

    private SearchNode solvedSearchNode;

    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) {
        if (initial == null) {
            throw new IllegalArgumentException("Cannot initialize Solver with null Board.");
        }

        solvedSearchNode = null;

        final MinPQ<SearchNode> pq = new MinPQ<SearchNode>();
        pq.insert(new SearchNode(null, initial, 0));

        final MinPQ<SearchNode> twinPq = new MinPQ<SearchNode>();
        twinPq.insert(new SearchNode(null, initial.twin(), 0));

        while (!pq.isEmpty()) {
            SearchNode searchNode = pq.delMin();
            Board currentBoard = searchNode.board;

            if (currentBoard.isGoal()) {
                solvedSearchNode = searchNode;
                break;
            }

            for (Board child : currentBoard.neighbors()) {
                if (searchNode.parent == null || !searchNode.parent.board.equals(child)) {
                    pq.insert(new SearchNode(searchNode, child, searchNode.moves + 1));
                }
            }

            if (!twinPq.isEmpty()) {
                SearchNode twinSearchNode = twinPq.delMin();
                Board twinCurrentBoard = twinSearchNode.board;

                if (twinCurrentBoard.isGoal()) {
                    break;
                }

                for (Board twinChild : twinCurrentBoard.neighbors()) {
                    if (twinSearchNode.parent == null || !twinSearchNode.parent.board.equals(twinChild)) {
                        twinPq.insert(new SearchNode(twinSearchNode, twinChild, twinSearchNode.moves + 1));
                    }
                }
            }
        }
    }

    // can the puzzle board be solved?
    public boolean isSolvable() {
        return solvedSearchNode != null;
    }

    // minimum number of moves taken to solve the board (-1 if unsolvable)
    public int moves() {
        if (!isSolvable()) {
            return -1;
        }

        return solvedSearchNode.moves;
    }

    // sequence of boards in a shortest solution (null if unsolvable)
    public Iterable<Board> solution() {
        if (!isSolvable()) {
            return null;
        }

        Stack<Board> solution = new Stack<Board>();
        SearchNode currentSearchNode = solvedSearchNode;
        while (currentSearchNode != null) {
            solution.push(currentSearchNode.board);
            currentSearchNode = currentSearchNode.parent;
        }
        return solution;
    }

    // test client
    public static void main(String[] args) {
        In in = new In(args[0]);

        // get dimensions for tiles grid
        int n = in.readInt();

        // make tiles grid
        int[][] tiles = new int[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                tiles[i][j] = in.readInt();
            }
        }

        // solve the puzzle
        Solver solver = new Solver(new Board(tiles));

        // print solution to standard output
        if (!solver.isSolvable()) {
            StdOut.println("No solution possible");
        } else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution()) {
                StdOut.println(board);
            }
        }
    }
}
