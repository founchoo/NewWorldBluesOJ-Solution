#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// lru 函数声明
// 输入参数描述：
//   pageframeNum：操作系统分配给某进程的页框数目；
//   pageCallSequence：页面调用序列，序列中的每一项是被调用页面的页面号。
void lru(int pageframeNum, vector<int> &pageCallSequence);

int main() {
    int i, pageframeNum, n;
    cin>>pageframeNum; // 输入分配给某进程的页框数目(教材上称“页框”为：物理块)
    cin>>n; // 输入该进程的页面调用序列的长度
    vector<int> pageCallSequence(n); // 定义页面调用序列，序列中的每一项是被调用页面的页面号
    for(i = 0; i < n; i++) {    // 输入 n 个页面号，构建页面调用序列
        cin>>pageCallSequence[i];
    }

    lru(pageframeNum, pageCallSequence); // 模拟最近最久未使用页面置换算法

    return 0;
}

void lru(int pageframeNum, vector<int> &pageCallSequence) {
    
    int pageFaultNum = 0;   // 缺页次数
    int emptyPageFrameFlag = -1;    // 用于标记页框为空
    vector<int> pageFrame(pageframeNum, emptyPageFrameFlag);  // 定义页框，初始化为 -1
    vector<int> stayTime(pageframeNum, 0);    // 定义页框中页面的停留时间，初始化为 0
    for (int i = 0; i < pageCallSequence.size(); i++) {
        int page = pageCallSequence[i];
        int pageIndex = -1;
        for (int j = 0; j < pageFrame.size(); j++) {
            if (pageFrame[j] == page) {
                pageIndex = j;
                break;
            }
        }
        if (pageIndex == -1) {    // 页框中没有该页面
            pageFaultNum++;
            int emptyPageIndex = -1;
            for (int j = 0; j < pageFrame.size(); j++) {
                if (pageFrame[j] == emptyPageFrameFlag) {
                    emptyPageIndex = j;
                    break;
                }
            }
            if (emptyPageIndex == -1) {   // 页框已满
                int maxStayTimePageIndex = -1;
                int maxStayTime = -1;
                for (int j = 0; j < pageFrame.size(); j++) {
                    if (stayTime[j] > maxStayTime) {
                        maxStayTime = stayTime[j];
                        maxStayTimePageIndex = j;
                    }
                }
                pageFrame[maxStayTimePageIndex] = page;
                stayTime[maxStayTimePageIndex] = 0;
            } else {    // 页框未满
                pageFrame[emptyPageIndex] = page;
                stayTime[emptyPageIndex] = 0;
            }
        } else {
            stayTime[pageIndex] = 0;
        }
        for (int j = 0; j < pageFrame.size(); j++) {    // 为每个非空页框中的页面停留时间加 1
            if (pageFrame[j] != emptyPageFrameFlag) {
                stayTime[j]++;
            }
        }
        for (int j = 0; j < pageFrame.size(); j++) {
            cout << pageFrame[j];
            if (j < pageFrame.size() - 1) {
                cout << ",";
            }
        }
        cout << endl;
    }
    cout << fixed << setprecision(3) << (double)pageFaultNum / pageCallSequence.size();
}