#include <iostream>
#include <vector>
using namespace std;

void banker(vector<int> &R, vector<vector<int>> &A, vector<vector<int>> &C, vector<vector<int>> &reqestSequence);

int main() {
    int n, m; // 系统中有 n 个进程、m 种不同类型的资源
    int N; // 进程请求资源的次数
    int i, j;

    cin >> n; // 输入进程数量
    cin >> m; // 输入资源类型数

    vector<int> R(m); // R 向量表示系统中每种资源的的总量

    for (i = 0; i < m; i++) { // 对 m 种资源，输入每种资源的总量，保存到向量 R 中
        cin >> R[i];
    }

    vector<vector<int>> C(n, vector<int>(m)); // C 为 Claim 矩阵，即最大需求矩阵，教材上称 Max 矩阵
    for (i = 0; i < n; i++) { // 输入 C 矩阵
        for (j = 0; j < m; j++) {
            cin >> C[i][j];
        }
    }

    vector<vector<int>> A(n, vector<int>(m)); // A 为 Allocation 矩阵，即分配矩阵
    for (i = 0; i < n; i++) { // 输入 A 矩阵
        for (j = 0; j < m; j++) {
            cin >> A[i][j];
        }
    }

    // 欲针对当前状态做 N 次资源请求测试，故这里一次性输入 N 次资源请求，每次资源请
    // 求需要输入 m+1 个整数：第 0 个整数表示申请资源的进程号（取值范围：0 至 n-1）;
    // 第 1 至 m 个整数分别表示当前状态下，该进程对 m 种资源的申请数量。
    cin >> N; // 输入进程请求资源的次数

    vector<vector<int>> requestSequence(N, vector<int>(m+1));
    for (i = 0; i < N; i++) {
        for (j = 0; j <= m; j++) {
            cin >> requestSequence[i][j];
        }
    }

    banker(R, A, C, requestSequence);

    return 0;
}

void banker(vector<int> &R, vector<vector<int>> &A, vector<vector<int>> &C, vector<vector<int>> &requestSequence) {
    for (int i = 0; i < requestSequence.size(); i++) {
        int pid = requestSequence[i][0]; // 进程号
        // 最大需求向量
        vector<vector<int>> max(C.begin(), C.end());
        // 已分配向量
        vector<vector<int>> allocation(A.begin(), A.end());
        // 可利用资源向量
        vector<int> available(R.begin(), R.end());
        for (int j = 0; j < available.size(); j++) {
            for (int k = 0; k < allocation.size(); k++) {
                available[j] -= allocation[k][j];
            }
        }
        // 需求向量
        vector<vector<int>> need;
        for (int j = 0; j < max.size(); j++) {
            vector<int> temp;
            for (int k = 0; k < max[j].size(); k++) {
                temp.push_back(max[j][k] - allocation[j][k]);
            }
            need.push_back(temp);
        }
        vector<int> request(
            requestSequence[i].begin() + 1,
            requestSequence[i].end()); // 申请资源向量
        bool requestLowestWithNeed = true; // 申请资源是否小于需求
        for (int j = 0; j < request.size(); j++) {
            if (request[j] > need[pid][j]) {
                requestLowestWithNeed = false;
                break;
            }
        }
        // 申请资源是否小于可利用资源
        bool requestLowestWithAvailable = true;
        for (int j = 0; j < request.size(); j++) {
            if (request[j] > available[j]) {
                requestLowestWithAvailable = false;
                break;
            }
        }
        if (requestLowestWithNeed && requestLowestWithAvailable) {
            // 尝试分配资源
            for (int j = 0; j < request.size(); j++) {
                available[j] -= request[j];
                allocation[pid][j] += request[j];
                need[pid][j] -= request[j];
            }
            vector<int> process(need.size());
            for (int j = 0; j < process.size(); j++) {
                process[j] = j;
            }
            bool needZero = true; // 是否进程需求为 0
            for (int j = 0; j < need[pid].size(); j++) {
                if (need[pid][j] != 0) {
                    needZero = false;
                    break;
                }
            }
            // 安全性检查
            vector<int> safeSequence;
            bool exit = false;
            while (!exit) {
                exit = true;
                for (int pid = 0; pid < process.size(); pid++) {
                    bool exist = false;
                    for (int j = 0; j < safeSequence.size(); j++) {
                        if (process[pid] == safeSequence[j]) {
                            exist = true;
                            break;
                        }
                    }
                    bool needLowestWithAvailable = true;
                    for (int j = 0; j < need[pid].size(); j++) {
                        if (need[pid][j] > available[j]) {
                            needLowestWithAvailable = false;
                            break;
                        }
                    }
                    if (!exist && needLowestWithAvailable) {
                        exit = false;
                        safeSequence.push_back(pid);
                        for (int j = 0; j < available.size(); j++) {
                            available[j] += allocation[pid][j];
                        }
                    }
                }
            }
            if (safeSequence.size() == need.size()) {
                cout << "0" << endl;
                for (int j = 0; j < safeSequence.size(); j++) {
                    if (safeSequence[j] == pid && needZero) {
                        continue;
                    }
                    cout << safeSequence[j];
                    if (j < safeSequence.size() - 1) {
                        cout << ",";
                    }
                }
            } else {
                cout << "1";
            }
        } else {
            cout << "2";
        }
        if (i < requestSequence.size() - 1) {
            cout << endl;
        }
    }
}
