#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct SPageTableRegister {
    int *pageTableAddr; // 页表地址
    int pageTableLen;   // 页表长度
    SPageTableRegister();
} ptr;

SPageTableRegister::SPageTableRegister() {
    pageTableLen = 26;
    pageTableAddr = new int[pageTableLen];
    int *source = new int[pageTableLen];
    for (int i = 0; i < pageTableLen; i++) {
        source[i] = i;
    }
    // random_shuffle(source, source + pageTableLen);
    for (int i = 0; i < pageTableLen; i++) {
        pageTableAddr[i] = source[i];
    }
}

int main() {
    const int PageSize = 1024;  // 页面大小
    unsigned short logicAddr;   // 逻辑地址
    cin >> logicAddr;           // 输入逻辑地址
    int pageTableIndex = logicAddr / PageSize;  // 页表索引号
    if (pageTableIndex >= ptr.pageTableLen) {   // 页表索引号超出页表长度
        cout << "N";
        return 0;
    }
    int pageOffset = logicAddr % PageSize;  // 页内偏移量, 落在 [0, 1023] 之间
    bool legal = true;
    vector<int> count = vector<int>(ptr.pageTableLen, 0);   // 记数
    for (int i = 0; i < ptr.pageTableLen; i++) {
        if (ptr.pageTableAddr[i] < 0 || ptr.pageTableAddr[i] > 63) {    // 检验范围是 [0, 63]
            legal = false;
            break;
        }
        if (++count[ptr.pageTableAddr[i]] > 1) {    // 检验重复数字
            legal = false;
            break;
        }
    }
    if (!legal) {
        cout << "N";
    } else {
        int phyAddr = ptr.pageTableAddr[pageTableIndex] * PageSize + pageOffset;    // 物理地址
        cout << phyAddr;
    }
    return 0;
}