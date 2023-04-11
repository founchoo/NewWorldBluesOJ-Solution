#include <iostream>
#include <vector>
using namespace std;

struct SPCB {
    int pid;           // 进程号
    int arrivalTime;   // 到达时间
    int workTime;      // 服务时间
    int remainingTime; // 剩余服务时间
    int timeSlice;     // 剩余时间片
    int startTime;     // 开始时间。必要时，可在 mfqSchedule 函数中使用，用于记录进程开始执行的时间
    int endTime;       // 进程完成服务的时间。必要时，可在 mfqSchedule 函数中使用，用于记录进程完成服务的时间
};

void mfqSchedule(vector<SPCB> &pcb, const vector<int> &timeSlice);

int main() {
    int m, n; // m 为队列数量，n 为进程数量
    int i;

    cin >> m >> n;            // 输入队列数量和进程数量
    vector<int> timeSlice(m); // t 为整型数组，存放 m 级队列的时间片长度
    vector<SPCB> pcb(n);

    for (i = 0; i < m; i++) {
        cin >> timeSlice[i]; // 输入 m 级队列的时间片长度；一般来说，后一级队列的时间片长度大于前一级，比如，后一级时间片长度是前一级的 2 倍
    }

    for (i = 0; i < n; i++) {
        cin >> pcb[i].arrivalTime; // 输入第 i 个进程的到达时间
        cin >> pcb[i].workTime;    // 输入第 i 个进程的服务时间

        pcb[i].pid = i;                         // 第 i 个进程的进程号取为 i
        pcb[i].remainingTime = pcb[i].workTime; // 剩余服务时间初始化为服务时间
        pcb[i].timeSlice = timeSlice[0];        // 剩余时间片初始化为最高优先级队列的时间片长度
        pcb[i].startTime = -1;                  // 开始时间初始化为 -1，表示此时开始时间记录是无效值
    }

    mfqSchedule(pcb, timeSlice);

    return 0;
}

void mfqSchedule(vector<SPCB> &pcb, const vector<int> &timeSlice) {
    int queueCount = timeSlice.size(); // m 为队列数量
    int pcbCount = pcb.size();       // n 为进程数量

    // 队列数组，每个元素是一个队列，队列中存放的是进程号
    vector<vector<int>> queue(queueCount);

    // 根据到达时间对进程进行排序
    for (int i = 0; i < pcbCount - 1; i++) {
        for (int j = 0; j < pcbCount - i - 1; j++) {
            if (pcb[j].arrivalTime > pcb[j + 1].arrivalTime) {
                SPCB temp = pcb[j];
                pcb[j] = pcb[j + 1];
                pcb[j + 1] = temp;
            }
        }
    }

    int time = 0; // 当前时间
    int nextPCBIndex = 0; // 下一个要进入队列的进程的下标
    while (true) {
        // 下一个要进入队列的进程的到达时间等于当前时间时，将其加入队列
        while (nextPCBIndex < pcbCount && time == pcb[nextPCBIndex].arrivalTime) {
            queue[0].push_back(nextPCBIndex);
            nextPCBIndex++;
        }
        for (int i = 0; i < queueCount; i++) {
            // 仅执行非空且最高优先级队列中的进程
            if (queue[i].size() != 0) {
                int pcbIndex = queue[i][0]; // 取出队列中的第一个进程
                if (pcb[pcbIndex].startTime == -1) {
                    pcb[pcbIndex].startTime = time; // 记录进程开始执行的时间
                }
                // 执行当前进程
                pcb[pcbIndex].timeSlice--;
                pcb[pcbIndex].remainingTime--;
                // 当前进程执行完毕
                if (pcb[pcbIndex].remainingTime == 0) {
                    pcb[pcbIndex].endTime = time + 1;
                    queue[i].erase(queue[i].begin());
                } else {
                    // 时间片耗尽，移入下一优先级队列
                    if (pcb[pcbIndex].timeSlice == 0) {
                        queue[i].erase(queue[i].begin());
                        // 进程所在的队列已经是最低优先级
                        if (i == queueCount - 1) {
                            queue[i].push_back(pcbIndex);
                            pcb[pcbIndex].timeSlice = timeSlice[i];
                        } else {
                            queue[i + 1].push_back(pcbIndex);
                            pcb[pcbIndex].timeSlice = timeSlice[i + 1];
                        }
                    }
                }
                break;
            }
        }
        
        time++;

        bool exist = true;
        for (int i = 0; i < pcbCount; i++) {
            if (pcb[i].endTime == 0) {
                exist = false;
                break;
            }
        }
        if (exist) {
            break;
        }
    }

    // 计算平均周转时间
    double sum = 0;
    for (int i = 0; i < pcbCount; i++) {
        sum += pcb[i].endTime - pcb[i].arrivalTime;
    }
    printf("%.3f\n", sum / pcbCount);
    // 计算平均带权周转时间
    sum = 0;
    for (int i = 0; i < pcbCount; i++) {
        sum += (double)(pcb[i].endTime - pcb[i].arrivalTime) / pcb[i].workTime;
    }
    printf("%.3f", sum / pcbCount);
}
