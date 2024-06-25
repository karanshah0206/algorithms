#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>

using namespace std;

tuple<string, string, int> needleman_wunsch(string seq1, string seq2, int match_score = 5, int mismatch_score = -3, int gap_penalty = -4) {
    // Initialize the matrix
    vector<vector<int>> matrix(seq1.length() + 1, vector<int>(seq2.length() + 1, 0));

    // Initialize the first row and column with gap penalties
    for (int i = 1; i <= seq1.length(); ++i) {
        matrix[i][0] = matrix[i - 1][0] + gap_penalty;
    }
    for (int j = 1; j <= seq2.length(); ++j) {
        matrix[0][j] = matrix[0][j - 1] + gap_penalty;
    }

    // Fill in the matrix
    for (int i = 1; i <= seq1.length(); ++i) {
        for (int j = 1; j <= seq2.length(); ++j) {
            int score;
            if (seq1[i - 1] == seq2[j - 1]) {
                score = match_score;
            } else {
                score = mismatch_score;
            }
            int diagonal = matrix[i - 1][j - 1] + score;
            int up = matrix[i - 1][j] + gap_penalty;
            int left = matrix[i][j - 1] + gap_penalty;
            matrix[i][j] = max({diagonal, up, left});
        }
    }

    // Traceback to find the alignment
    string alignment_seq1 = "";
    string alignment_seq2 = "";
    int i = seq1.length();
    int j = seq2.length();
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && matrix[i][j] == matrix[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? match_score : mismatch_score)) {
            alignment_seq1 = seq1[i - 1] + alignment_seq1;
            alignment_seq2 = seq2[j - 1] + alignment_seq2;
            i -= 1;
            j -= 1;
        } else if (i > 0 && matrix[i][j] == matrix[i - 1][j] + gap_penalty) {
            alignment_seq1 = seq1[i - 1] + alignment_seq1;
            alignment_seq2 = '-' + alignment_seq2;
            i -= 1;
        } else {
            alignment_seq1 = '-' + alignment_seq1;
            alignment_seq2 = seq2[j - 1] + alignment_seq2;
            j -= 1;
        }
    }

    return make_tuple(alignment_seq1, alignment_seq2, matrix[seq1.length()][seq2.length()]);
}

int main() {
    // Test the function
    string seq1 = "GTCGACGCA";
    string seq2 = "CATTACA";
    auto alignment_result = needleman_wunsch(seq1, seq2);
    string alignment_seq1 = get<0>(alignment_result);
    string alignment_seq2 = get<1>(alignment_result);
    int score = get<2>(alignment_result);

    cout << "Optimal Alignment:" << endl;
    cout << "Sequence 1: " << alignment_seq1 << endl;
    cout << "Sequence 2: " << alignment_seq2 << endl;
    cout << "Alignment Score: " << score << endl;

    return 0;
}
