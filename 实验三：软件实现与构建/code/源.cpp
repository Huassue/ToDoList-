#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <set>
#include <ctime>
#include <iomanip> // for std::put_time
#include <algorithm>
#include <thread>
#include <sstream>
#include <chrono>


// 控制台颜色
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define YELLOW "\033[33m"

// 提醒方式
class ReminderService {
public:
    // 提醒方式 1: 直接通过命令行提醒
    void notifyByConsole(const string& taskTitle) {
        cout << YELLOW << "提醒: 任务 \"" << taskTitle << "\" 已逾期！" << RESET << endl;
    }

    // 提醒方式 2: 短信提醒 (接口)
    void notifyBySMS(const string& phoneNumber, const string& taskTitle) {
        cout << "[短信提醒]: 已向 " << phoneNumber << " 发送任务 \"" << taskTitle << "\" 已逾期的提醒。" << endl;
    }

    // 提醒方式 3: 邮件提醒 (接口)
    void notifyByEmail(const string& email, const string& taskTitle) {
        cout << "[邮件提醒]: 已向 " << email << " 发送任务 \"" << taskTitle << "\" 已逾期的提醒。" << endl;
    }
};

// AIAssistant 类  具体功能留作TODO
class AIAssistant {
public:
    // 分析用户活动
    void analyzeUserActivity(User user) {
        // 接口：分析用户的任务活动，调整任务优先级或提醒方式
        cout << "分析用户 " << user.name << " 的任务活动..." << endl;
    }

    // 调整任务的提醒频率
    void adjustReminderFrequency(Task task) {
        // 接口：根据任务进展或用户行为调整提醒频率
        cout << "根据任务 \"" << task.title << "\" 的进展调整提醒频率..." << endl;
    }

    // 预测任务完成时间
    void predictTaskCompletion(Task task) {
        // 接口：通过 AI 预测任务完成所需的时间
        cout << "预测任务 \"" << task.title << "\" 的完成时间..." << endl;
    }
};

// Tag 类
class Tag {
public:
    string name;
    Tag(string name) : name(name) {}
};

// ReminderType 枚举类型
enum class ReminderType {
    Console,  // 通过命令行提醒
    SMS,      // 短信提醒
    Email     // 邮件提醒
};
// Task 类
class Task {
public:
    string title;
    string description;
    time_t deadline;
    vector<Tag> tags;
    //string category;
    string status; // "进行中", "已完成", "已逾期"

    ReminderType reminderType; // 新增成员变量: 提醒类型
    time_t reminderTime;       // 提醒时间

    Task(string title, string description, time_t deadline, ReminderType type = ReminderType::Console)
        : title(title), description(description), deadline(deadline), status("进行中"), reminderType(type) {}

    void addTag(const Tag& tag) {
        tags.push_back(tag);
    }

    void updateStatus() {
        time_t now = time(0);
        if (status == "进行中" && deadline < now) {
            status = "已逾期";
        }
    }

    string getStatusColor() const {

        if (status == "已完成") {
            return GREEN;
        }
        else if (status == "进行中") {
            return BLUE;
        }
        else { // 已逾期
            return RED;
        }
    }

    void displayWithColor() const {
        cout << getStatusColor() << title << " (" << status << ")" << RESET << endl;
    }

    void displayTaskDetails(){
        // 打印标题和状态
        updateStatus();
        cout << getStatusColor() << "\n---------------------------" << RESET << endl;
        cout <<  "任务标题: " << getStatusColor() << title << RESET << endl;
        cout << "状态: " << getStatusColor() << status << RESET << endl;
        
        // 打印描述
        cout << "描述: " << description << endl;

        // 打印截止时间
        std::tm timeinfo;
        localtime_s(&timeinfo, &deadline); // 将时间转换为本地时间
        cout << "截止日期: " << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << endl;

        // 打印标签
        if (tags.empty()) {
            cout << "标签: 无" << endl;
        }
        else {
            cout << "标签: ";
            for (size_t i = 0; i < tags.size(); ++i) {
                cout << tags[i].name;
                if (i < tags.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }

        cout << "提醒类型: ";
        switch (reminderType) {
            case ReminderType::Console: cout << "命令行提醒" << endl; break;
            case ReminderType::SMS: cout << "短信提醒" << endl; break;
            case ReminderType::Email: cout << "邮件提醒" << endl; break;
        }

        cout << getStatusColor() << "---------------------------" << RESET << endl;
    }
};


// User 类
class User {
public:
    string name;
    string email;
    string phone;
    vector<Task> tasks;

    User(string name, string email, string phone) : name(name), email(email), phone(phone) {}

    void addTask(const Task& task) {
        tasks.push_back(task);
    }

    // 启动提醒服务
    void startReminderService() {
        ReminderService reminderService; // 实例化提醒服务

        std::thread([this, &reminderService]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(10)); // 每10秒检查一次
                checkForReminders(reminderService);
            }
            }).detach(); // 将线程分离
    }

    // 检查提醒逻辑
    void checkForReminders(ReminderService& reminderService) {
        time_t now = time(0); // 获取当前时间

        for (auto& task : tasks) {
            task.updateStatus();
            if (task.status == "已逾期") {
                // 根据 ReminderType 调用不同的提醒方式
                switch (task.reminderType) {
                case ReminderType::Console:
                    reminderService.notifyByConsole(task.title);
                    break;
                case ReminderType::SMS:
                    reminderService.notifyBySMS(phone, task.title);
                    //TODO
                    break;
                case ReminderType::Email:
                    reminderService.notifyByEmail(email, task.title);
                    //TODO
                    break;
                }             

            }
        }
    }

    void displayTasks() {
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << i + 1 << ". ";
            tasks[i].updateStatus();
            tasks[i].displayWithColor();
        }
    }

    void displayTasksByTag(const string& tagName) {
        bool found = false;
        for (auto& task : tasks) {
            for (auto& tag : task.tags) {
                if (tag.name == tagName) {
                    task.updateStatus();
                    task.displayWithColor();
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            cout << "没有找到标签为 \"" << tagName << "\" 的任务。" << endl;
        }
    }

    void displayTimeline() {
        vector<Task> sortedTasks = tasks;

        // 按截止日期排序
        sort(sortedTasks.begin(), sortedTasks.end(), [](const Task& a, const Task& b) {
            return a.deadline < b.deadline;
            });

        cout << "时间线视图：" << endl;
        for (const auto& task : sortedTasks) {
            // 更新任务状态（逾期检测）
            const_cast<Task&>(task).updateStatus();
            // 打印任务信息
            std::tm timeinfo;
            localtime_s(&timeinfo, &task.deadline); // 使用 localtime_s 将时间转换为本地时间
            cout << task.getStatusColor() << task.title 
                << " | 截止日期: " << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") // 修改为使用 &timeinfo
                << " | 状态: " << task.status << RESET << endl;
        }
    }

    void displayAllTags(vector<string>& tagList) {
        set<string> uniqueTags; // 使用 set 确保标签唯一
        tagList.clear();        // 确保每次调用时清空列表

        for (const auto& task : tasks) {
            for (const auto& tag : task.tags) {
                uniqueTags.insert(tag.name);
            }
        }

        if (uniqueTags.empty()) {
            cout << "当前没有任何标签。" << endl;
        }
        else {
            cout << "所有标签：" << endl;
            size_t i = 0;
            for (const auto& tagName : uniqueTags) {
                tagList.push_back(tagName); // 将标签按序添加到列表
                cout << i + 1 << ". " << tagName << endl;
                i++;
            }
        }
    }
};

// 解析用户输入的日期时间为 time_t
time_t parseDateTime(const string& dateTimeStr) {
    istringstream iss(dateTimeStr);
    tm timeinfo = {};
    char delimiter;

    iss >> get_time(&timeinfo, "%Y-%m-%d,%H:%M");
    if (iss.fail() || timeinfo.tm_hour > 23 || timeinfo.tm_min > 59) {
        throw invalid_argument("输入的日期格式错误，应为 YYYY-MM-DD,HH:MM，时间有效范围为 00:00 至 23:59。");
    }

    return mktime(&timeinfo); // 转换为 time_t
}


void printToDoListArt() {
    /*
    cout << "  ______  ____      ____    ____     __     __    _----- ______    " << endl;
    cout << " |_   _| /  _ \    |  _ \  /  _ \   | |    | |   /  (--`|_   _|     " << endl;
    cout << "   | |  | | | |    | | | || | | |   | |    | |   \\  \\   | |        " << endl;
    cout << "   | |  | |_| |    | |_| || |_| |   | |___ | | ----)  |   | |    " << endl;
    cout << "   |_|  \____/     |____/ \____/    |_____||_| |_____/    |_|     " << endl;
    cout << endl;*/

    cout << YELLOW;
    cout << "  ______  ____      ____    ____     __     __    _----- ______    " << endl;
    cout << " |_   _| /  _ \\    |  _ \\  /  _ \\   | |    | |   /  (--`|_   _|     " << endl;
    cout << "   | |  | | | |    | | | || | | |   | |    | |   \\\\  \\\\   | |        " << endl;
    cout << "   | |  | |_| |    | |_| || |_| |   | |___ | | ----)  |   | |    " << endl;
    cout << "   |_|  \\____/     |____/ \\____/    |_____||_| |_____/    |_|     " << endl;
    cout << RESET << endl;
}

// 主函数
int main() {
    printToDoListArt();
    // 用户登录
    string Username, Useremail, Userphone;
    cout << "输入用户名：" << endl;
    cin >> Username;
    cout << "输入邮箱：" << endl;
    cin >> Useremail;
    cout << "输入电话号码：" << endl;
    cin >> Userphone;
    User user(Username, Useremail, Userphone);
    cout << "是否开启提醒服务？y/n" << endl;
    char ifReminder;
    cin >> ifReminder;
    if (ifReminder == 'y') {
        user.startReminderService();
    }

    // 主循环
    bool running = true;
    while (running) {
        cout << "\n---------------------------" << endl;
        cout << "请选择一个操作：" << endl;
        cout << "1. 创建任务" << endl;
        cout << "2. 查看/修改任务" << endl;
        cout << "3. 修改任务标签" << endl;
        cout << "4. 完成任务" << endl;
        cout << "5. 打印时间线" << endl;
        cout << "6. 按照标签打印任务" << endl;
        cout << "7. 退出" << endl;
        cout << "---------------------------" << endl;
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: { // 创建任务
            string title, description, deadlineStr;
            
            cout << "输入任务标题：" << endl;
            cin >> title;
            cout << "输入任务描述：" << endl;
            cin.ignore(); // 清除输入缓冲区
            getline(cin, description);

            time_t deadline;
            while (true) {
                try {
                    cout << "输入截止时间 (格式: YYYY-MM-DD,HH:MM): ";
                    cin >> deadlineStr;
                    deadline = parseDateTime(deadlineStr);
                    break; // 输入正确，跳出循环
                }
                catch (const invalid_argument& e) {
                    // 捕获输入错误并提示重新输入
                    cout << "错误: " << e.what() << endl;
                }
            }

            Task newTask(title, description, deadline);
            
            cout << "任务已创建！"  << endl;

            // 添加标签
            char addTagOption;
            cin.ignore(); // 清除输入缓冲区
            cout << "是否为任务添加标签？(y/n): ";
            cin >> addTagOption;
            while (addTagOption == 'y') {
                string tagName;
                cout << "输入标签名称：" << endl;
                cin >> tagName;
                newTask.addTag(Tag(tagName));

                cout << "是否继续添加标签？(y/n): ";
                cin >> addTagOption;
            }

            // 添加任务到用户
            user.addTask(newTask);
            cout << "任务已成功创建！" << endl;
            break;
        }

        case 2: { // 查看/修改任务
            cout << "当前任务列表：" << endl;
            user.displayTasks();

            int taskIndex;
            cout << "选择要修改的任务编号：" << endl;
            cin >> taskIndex;
            --taskIndex; // 转换为0索引

            if (taskIndex >= 0 && taskIndex < user.tasks.size()) {
                Task& task = user.tasks[taskIndex];
                bool modify = true;
                while (modify) {
                    //打印当前任务信息
                    task.displayTaskDetails();
                    cout << "\n---------------------------" << endl;
                    cout << "选择要修改的内容：" << endl;
                    cout << "1. 任务标题" << endl;
                    cout << "2. 任务描述" << endl;
                    cout << "3. 截止时间" << endl;
                    cout << "4. 完成修改" << endl;
                    cout << "---------------------------" << endl;
                    int choice;
                    cin >> choice;

                    switch (choice) {
                    case 1: { // 修改标题
                        string newTitle;
                        cout << "输入新的任务标题：" << endl;
                        cin.ignore();
                        getline(cin, newTitle);
                        task.title = newTitle;
                        cout << "任务标题已更新！" << endl;
                        break;
                    }
                    case 2: { // 修改描述
                        string newDescription;
                        cout << "输入新的任务描述：" << endl;
                        cin.ignore();
                        getline(cin, newDescription);
                        task.description = newDescription;
                        cout << "任务描述已更新！" << endl;
                        break;
                    }
                    case 3: { // 修改截止时间
                        string newDeadlineStr;
                        cout << "输入新的截止时间 (格式: YYYY-MM-DD, HH:MM): " << endl;
                        cin >> newDeadlineStr;

                        try {
                            time_t newDeadline = parseDateTime(newDeadlineStr);
                            task.deadline = newDeadline;
                            cout << "任务截止时间已更新！" << endl;
                        }
                        catch (const invalid_argument& e) {
                            cout << RED << "无效的日期格式！" << RESET << endl;
                        }
                        break;
                    }
                    case 4: { //完成修改
                        modify = false;
                        cout << "任务修改完成！" << endl;
                        break;
                    }
                    default:
                        cout << "无效的选项！" << endl;
                    }
                }
            }
            else {
                cout << "无效的任务编号！" << endl;
            }
            break;
        }

        case 3: { // 修改任务标签
            cout << "当前任务列表：" << endl;
            user.displayTasks();

            int taskIndex;
            cout << "选择要修改标签的任务编号：" << endl;
            cin >> taskIndex;
            --taskIndex;
            if (taskIndex >= 0 && taskIndex < user.tasks.size()) {
                while (true) {
                    cout << "\n---------------------------" << endl;
                    cout << "当前任务的标签：" << endl;
                    const auto& tags = user.tasks[taskIndex].tags;
                    if (tags.empty()) {
                        cout << "无标签" << endl;
                    }
                    else {
                        for (size_t i = 0; i < tags.size(); ++i) {
                            cout << i + 1 << ". " << tags[i].name << endl;
                        }
                    }

                    cout << "选择操作：" << endl;
                    cout << "1. 添加标签" << endl;
                    cout << "2. 删除标签" << endl;
                    cout << "3. 完成修改" << endl;
                    cout << "---------------------------" << endl;

                    int tagOption;
                    cin >> tagOption;

                    if (tagOption == 1) { // 添加标签
                        string tagName;
                        cout << "输入新标签名称：" << endl;
                        cin >> tagName;
                        user.tasks[taskIndex].addTag(Tag(tagName));
                        cout << "标签已添加！" << endl;

                    }
                    else if (tagOption == 2) { // 删除标签
                        if (tags.empty()) {
                            cout << "没有可删除的标签。" << endl;
                        }
                        else {
                            int tagIndex;
                            cout << "选择要删除的标签编号：" << endl;
                            cin >> tagIndex;
                            --tagIndex;
                            if (tagIndex >= 0 && tagIndex < tags.size()) {
                                user.tasks[taskIndex].tags.erase(user.tasks[taskIndex].tags.begin() + tagIndex);
                                cout << "标签已删除！" << endl;
                            }
                            else {
                                cout << "无效的标签编号！" << endl;
                            }
                        }

                    }
                    else if (tagOption == 3) { // 完成修改
                        cout << "标签修改完成！" << endl;
                        break;

                    }
                    else {
                        cout << "无效的选项，请重新选择。" << endl;
                    }
                }
            }
            else {
                cout << "无效的任务编号！" << endl;
            }
            break;
        }

        case 4: { // 完成任务
            cout << "当前任务列表：" << endl;
            user.displayTasks();

            int taskIndex;
            cout << "选择要标记为完成的任务编号：" << endl;
            cin >> taskIndex;
            --taskIndex;
            if (taskIndex >= 0 && taskIndex < user.tasks.size()) {
                user.tasks[taskIndex].status = "已完成";
                cout << "任务已标记为完成！" << endl;
            }
            else {
                cout << "无效的任务编号！" << endl;
            }
            break;
        }

        case 5: // 打印时间线
            user.displayTimeline();
            break;

        case 6: { // 按照标签打印任务
            vector<string> tagList; // 存储标签的顺序
            user.displayAllTags(tagList);

            if (tagList.empty()) {
                cout << "没有可用的标签。" << endl;
                break;
            }

            cout << "输入要筛选的标签序号：" << endl;
            int tagNum;
            cin >> tagNum;

            if (tagNum < 1 || tagNum > tagList.size()) {
                cout << "无效的标签序号。" << endl;
            }
            else {
                string tagName = tagList[tagNum - 1];
                cout << "筛选结果 - 标签: " << tagName << endl;
                user.displayTasksByTag(tagName);
            }
            break;
        }

        case 7: // 退出
            running = false;
            cout << "退出程序。" << endl;
            break;

        default:
            cout << "无效的选择，请重新输入！" << endl;
        }
    }

    return 0;
}
