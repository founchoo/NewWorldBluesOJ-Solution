#include <iostream>
#include <vector>

using namespace std;

enum Direction {
	Left,
	Up,
	Right,
	Down
};

void fillNumber(Direction curDir, vector<vector<int>>& res, int len, int row, int col, int fillNum) {
	res[row][col] = fillNum;
	fillNum++;
	bool canLeft = col - 1 >= 0 && res[row][col - 1] == -1;
	bool canUp = row - 1 >= 0 && res[row - 1][col] == -1;
	bool canRight = col + 1 < len && res[row][col + 1] == -1;
	bool canDown = row + 1 < len && res[row + 1][col] == -1;
	if (!canLeft && !canUp && !canRight && !canDown) {
		return;
	}
	switch (curDir) {
		case Direction::Left:
			if (canLeft) {
				fillNumber(curDir, res, len, row, col - 1, fillNum);
			} else {
				curDir = Direction::Up;
				fillNumber(curDir, res, len, row - 1, col, fillNum);
			}
			break;
		case Direction::Up:
			if (canUp) {
				fillNumber(curDir, res, len, row - 1, col, fillNum);
			} else {
				curDir = Direction::Right;
				fillNumber(curDir, res, len, row, col + 1, fillNum);
			}
			break;
		case Direction::Right:
			if (canRight) {
				fillNumber(curDir, res, len, row, col + 1, fillNum);
			} else {
				curDir = Direction::Down;
				fillNumber(curDir, res, len, row + 1, col, fillNum);
			}
			break;
		case Direction::Down:
			if (canDown) {
				fillNumber(curDir, res, len, row + 1, col, fillNum);
			} else {
				curDir = Direction::Left;
				fillNumber(curDir, res, len, row, col - 1, fillNum);
			}
			break;
		default:
			break;
	}
}

int main() {
	int len;
	cin >> len;
	vector<vector<int>> res(len, vector<int>(len, -1));
	fillNumber(Direction::Down, res, len, 0, len - 1, 1);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			cout << res[i][j];
			if (j < len - 1) {
				cout << " ";
			}
		}
		if (i < len - 1) {
			cout << endl;
		}
	}
}