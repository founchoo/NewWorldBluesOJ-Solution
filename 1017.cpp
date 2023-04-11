#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool check(string num) {
	for (int i = 0; i < num.length() / 2; i++) {
		if (num[i] != num[num.length() - 1 - i]) {
			return false;
		}
	}
	return true;
}

int getValue(string str, int index) {
	if (index < 0 || index >= str.length()) {
		return -1;
	}
	char ch = str[index];
	if ('0' <= ch && ch <= '9') {
		return ch - '0';
	} else if ('A' <= ch && ch <= 'F') {
		return ch - 'A' + 10;
	}
	return -1;
}

char convert(int num) {
	if (0 <= num && num <= 9) {
		return num + '0';
	} else if (10 <= num && num <= 15) {
		return num - 10 + 'A';
	}
	return ' ';
}

string add(int base, string num1, string num2) {
	string res = "";
	int sum = -1;
	int addToNext = 0;
	for (int i = 0; ; i++) {
		int value1 = getValue(num1, num1.length() - 1 - i);
		int value2 = getValue(num2, num2.length() - 1 - i);
		sum = (value1 == -1 ? 0 : value1) + (value2 == -1 ? 0 : value2) + addToNext;
		if (value1 == -1 && value2 == -1 && sum == 0) {
			break;
		}
		res = convert(sum % base) + res;
		addToNext = sum / base;
	}
	return res;
}

int main() {
	int base;
	string baseNum;
	cin >> base >> baseNum;
	for (int i = 0; i < baseNum.length(); i++) {
		if ('a' <= baseNum[i] && baseNum[i] <= 'f') {
			baseNum[i] += 'A' - 'a';
		}
	}
	int steps = 0;
	while (!check(baseNum)) {
		if (steps == 30) {
			cout << "Impossible!";
			return 0;
		}
		steps++;
		string reversedBaseNum = baseNum;
		reverse(reversedBaseNum.begin(), reversedBaseNum.end());
		baseNum = add(base, baseNum, reversedBaseNum);
	}
	cout << "STEP=" << steps;
}