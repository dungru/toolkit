class Solution {
public:
    vector<int> x_que;
    vector<int> y_que;
    void solve(vector<vector<char>>& board) {
        int row_size, col_size;
        int i, j, k;
        int row, col;
        row_size = board.size();
        if(row_size == 0) return;
        col_size = board[0].size();
        for(i = 0; i < row_size; i++) {
            if(board[i][0] == 'O') {
                x_que.push_back(0);
                y_que.push_back(i);
            }
            if(board[i][col_size-1] == 'O') {
                x_que.push_back(col_size-1);
                y_que.push_back(i);
            }
        }
        
        for(i = 0; i < col_size; i++) {
            if(board[0][i] == 'O') {
                x_que.push_back(i);
                y_que.push_back(0);
            }
            if(board[row_size - 1][i] == 'O') {
                x_que.push_back(i);
                y_que.push_back(row_size-1);
            }
        }
        k = 0;
        while(k < x_que.size()) {
            row = y_que[k];
            col = x_que[k];
            board[row][col] = 'Y';
            if(row > 0 && board[row-1][col] == 'O') {x_que.push_back(col); y_que.push_back(row-1);}
            if(row < row_size - 1 && board[row+1][col] == 'O') {x_que.push_back(col); y_que.push_back(row+1);}
            if(col > 0 && board[row][col-1] == 'O') {x_que.push_back(col-1); y_que.push_back(row);}
            if(col < col_size - 1 && board[row][col+1] == 'O') {x_que.push_back(col+1); y_que.push_back(row);}
            k++;
        }
        for(i = 0; i < row_size; i++) {
            for(j = 0; j < col_size; j++) {
                if(board[i][j] == 'O') board[i][j] = 'X';
                if(board[i][j] == 'Y') board[i][j] = 'O';
            }
        }
        
    }
};



class Solution16ms {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty()) return;
        int row = board.size(), col = board[0].size();
        for (int i = 0; i < row; ++i) {
            check(board, i, 0);             // first column
            check(board, i, col - 1);       // last column
        }
        for (int j = 1; j < col - 1; ++j) {
            check(board, 0, j);             // first row
            check(board, row - 1, j);       // last row
        }
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
                if (board[i][j] == 'O') board[i][j] = 'X';
                else if (board[i][j] == '1') board[i][j] = 'O';
    }
    
    void check(vector<vector<char>>& board, int i, int j) {
        if (board[i][j] == 'O') {
            board[i][j] = '1';
            if (i > 1) check(board, i - 1, j);
            if (j > 1) check(board, i, j - 1);
            if (i + 1 < board.size()) check(board, i + 1, j);
            if (j + 1 < board[0].size()) check(board, i, j + 1);
        }
    }
};
