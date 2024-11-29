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


// ����̨��ɫ
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define YELLOW "\033[33m"

// ���ѷ�ʽ
class ReminderService {
public:
    // ���ѷ�ʽ 1: ֱ��ͨ������������
    void notifyByConsole(const string& taskTitle) {
        cout << YELLOW << "����: ���� \"" << taskTitle << "\" �����ڣ�" << RESET << endl;
    }

    // ���ѷ�ʽ 2: �������� (�ӿ�)
    void notifyBySMS(const string& phoneNumber, const string& taskTitle) {
        cout << "[��������]: ���� " << phoneNumber << " �������� \"" << taskTitle << "\" �����ڵ����ѡ�" << endl;
    }

    // ���ѷ�ʽ 3: �ʼ����� (�ӿ�)
    void notifyByEmail(const string& email, const string& taskTitle) {
        cout << "[�ʼ�����]: ���� " << email << " �������� \"" << taskTitle << "\" �����ڵ����ѡ�" << endl;
    }
};

// AIAssistant ��  ���幦������TODO
class AIAssistant {
public:
    // �����û��
    void analyzeUserActivity(User user) {
        // �ӿڣ������û��������������������ȼ������ѷ�ʽ
        cout << "�����û� " << user.name << " ������..." << endl;
    }

    // �������������Ƶ��
    void adjustReminderFrequency(Task task) {
        // �ӿڣ����������չ���û���Ϊ��������Ƶ��
        cout << "�������� \"" << task.title << "\" �Ľ�չ��������Ƶ��..." << endl;
    }

    // Ԥ���������ʱ��
    void predictTaskCompletion(Task task) {
        // �ӿڣ�ͨ�� AI Ԥ��������������ʱ��
        cout << "Ԥ������ \"" << task.title << "\" �����ʱ��..." << endl;
    }
};

// Tag ��
class Tag {
public:
    string name;
    Tag(string name) : name(name) {}
};

// ReminderType ö������
enum class ReminderType {
    Console,  // ͨ������������
    SMS,      // ��������
    Email     // �ʼ�����
};
// Task ��
class Task {
public:
    string title;
    string description;
    time_t deadline;
    vector<Tag> tags;
    //string category;
    string status; // "������", "�����", "������"

    ReminderType reminderType; // ������Ա����: ��������
    time_t reminderTime;       // ����ʱ��

    Task(string title, string description, time_t deadline, ReminderType type = ReminderType::Console)
        : title(title), description(description), deadline(deadline), status("������"), reminderType(type) {}

    void addTag(const Tag& tag) {
        tags.push_back(tag);
    }

    void updateStatus() {
        time_t now = time(0);
        if (status == "������" && deadline < now) {
            status = "������";
        }
    }

    string getStatusColor() const {

        if (status == "�����") {
            return GREEN;
        }
        else if (status == "������") {
            return BLUE;
        }
        else { // ������
            return RED;
        }
    }

    void displayWithColor() const {
        cout << getStatusColor() << title << " (" << status << ")" << RESET << endl;
    }

    void displayTaskDetails(){
        // ��ӡ�����״̬
        updateStatus();
        cout << getStatusColor() << "\n---------------------------" << RESET << endl;
        cout <<  "�������: " << getStatusColor() << title << RESET << endl;
        cout << "״̬: " << getStatusColor() << status << RESET << endl;
        
        // ��ӡ����
        cout << "����: " << description << endl;

        // ��ӡ��ֹʱ��
        std::tm timeinfo;
        localtime_s(&timeinfo, &deadline); // ��ʱ��ת��Ϊ����ʱ��
        cout << "��ֹ����: " << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << endl;

        // ��ӡ��ǩ
        if (tags.empty()) {
            cout << "��ǩ: ��" << endl;
        }
        else {
            cout << "��ǩ: ";
            for (size_t i = 0; i < tags.size(); ++i) {
                cout << tags[i].name;
                if (i < tags.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }

        cout << "��������: ";
        switch (reminderType) {
            case ReminderType::Console: cout << "����������" << endl; break;
            case ReminderType::SMS: cout << "��������" << endl; break;
            case ReminderType::Email: cout << "�ʼ�����" << endl; break;
        }

        cout << getStatusColor() << "---------------------------" << RESET << endl;
    }
};


// User ��
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

    // �������ѷ���
    void startReminderService() {
        ReminderService reminderService; // ʵ�������ѷ���

        std::thread([this, &reminderService]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(10)); // ÿ10����һ��
                checkForReminders(reminderService);
            }
            }).detach(); // ���̷߳���
    }

    // ��������߼�
    void checkForReminders(ReminderService& reminderService) {
        time_t now = time(0); // ��ȡ��ǰʱ��

        for (auto& task : tasks) {
            task.updateStatus();
            if (task.status == "������") {
                // ���� ReminderType ���ò�ͬ�����ѷ�ʽ
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
            cout << "û���ҵ���ǩΪ \"" << tagName << "\" ������" << endl;
        }
    }

    void displayTimeline() {
        vector<Task> sortedTasks = tasks;

        // ����ֹ��������
        sort(sortedTasks.begin(), sortedTasks.end(), [](const Task& a, const Task& b) {
            return a.deadline < b.deadline;
            });

        cout << "ʱ������ͼ��" << endl;
        for (const auto& task : sortedTasks) {
            // ��������״̬�����ڼ�⣩
            const_cast<Task&>(task).updateStatus();
            // ��ӡ������Ϣ
            std::tm timeinfo;
            localtime_s(&timeinfo, &task.deadline); // ʹ�� localtime_s ��ʱ��ת��Ϊ����ʱ��
            cout << task.getStatusColor() << task.title 
                << " | ��ֹ����: " << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") // �޸�Ϊʹ�� &timeinfo
                << " | ״̬: " << task.status << RESET << endl;
        }
    }

    void displayAllTags(vector<string>& tagList) {
        set<string> uniqueTags; // ʹ�� set ȷ����ǩΨһ
        tagList.clear();        // ȷ��ÿ�ε���ʱ����б�

        for (const auto& task : tasks) {
            for (const auto& tag : task.tags) {
                uniqueTags.insert(tag.name);
            }
        }

        if (uniqueTags.empty()) {
            cout << "��ǰû���κα�ǩ��" << endl;
        }
        else {
            cout << "���б�ǩ��" << endl;
            size_t i = 0;
            for (const auto& tagName : uniqueTags) {
                tagList.push_back(tagName); // ����ǩ������ӵ��б�
                cout << i + 1 << ". " << tagName << endl;
                i++;
            }
        }
    }
};

// �����û����������ʱ��Ϊ time_t
time_t parseDateTime(const string& dateTimeStr) {
    istringstream iss(dateTimeStr);
    tm timeinfo = {};
    char delimiter;

    iss >> get_time(&timeinfo, "%Y-%m-%d,%H:%M");
    if (iss.fail() || timeinfo.tm_hour > 23 || timeinfo.tm_min > 59) {
        throw invalid_argument("��������ڸ�ʽ����ӦΪ YYYY-MM-DD,HH:MM��ʱ����Ч��ΧΪ 00:00 �� 23:59��");
    }

    return mktime(&timeinfo); // ת��Ϊ time_t
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

// ������
int main() {
    printToDoListArt();
    // �û���¼
    string Username, Useremail, Userphone;
    cout << "�����û�����" << endl;
    cin >> Username;
    cout << "�������䣺" << endl;
    cin >> Useremail;
    cout << "����绰���룺" << endl;
    cin >> Userphone;
    User user(Username, Useremail, Userphone);
    cout << "�Ƿ������ѷ���y/n" << endl;
    char ifReminder;
    cin >> ifReminder;
    if (ifReminder == 'y') {
        user.startReminderService();
    }

    // ��ѭ��
    bool running = true;
    while (running) {
        cout << "\n---------------------------" << endl;
        cout << "��ѡ��һ��������" << endl;
        cout << "1. ��������" << endl;
        cout << "2. �鿴/�޸�����" << endl;
        cout << "3. �޸������ǩ" << endl;
        cout << "4. �������" << endl;
        cout << "5. ��ӡʱ����" << endl;
        cout << "6. ���ձ�ǩ��ӡ����" << endl;
        cout << "7. �˳�" << endl;
        cout << "---------------------------" << endl;
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: { // ��������
            string title, description, deadlineStr;
            
            cout << "����������⣺" << endl;
            cin >> title;
            cout << "��������������" << endl;
            cin.ignore(); // ������뻺����
            getline(cin, description);

            time_t deadline;
            while (true) {
                try {
                    cout << "�����ֹʱ�� (��ʽ: YYYY-MM-DD,HH:MM): ";
                    cin >> deadlineStr;
                    deadline = parseDateTime(deadlineStr);
                    break; // ������ȷ������ѭ��
                }
                catch (const invalid_argument& e) {
                    // �������������ʾ��������
                    cout << "����: " << e.what() << endl;
                }
            }

            Task newTask(title, description, deadline);
            
            cout << "�����Ѵ�����"  << endl;

            // ��ӱ�ǩ
            char addTagOption;
            cin.ignore(); // ������뻺����
            cout << "�Ƿ�Ϊ������ӱ�ǩ��(y/n): ";
            cin >> addTagOption;
            while (addTagOption == 'y') {
                string tagName;
                cout << "�����ǩ���ƣ�" << endl;
                cin >> tagName;
                newTask.addTag(Tag(tagName));

                cout << "�Ƿ������ӱ�ǩ��(y/n): ";
                cin >> addTagOption;
            }

            // ��������û�
            user.addTask(newTask);
            cout << "�����ѳɹ�������" << endl;
            break;
        }

        case 2: { // �鿴/�޸�����
            cout << "��ǰ�����б�" << endl;
            user.displayTasks();

            int taskIndex;
            cout << "ѡ��Ҫ�޸ĵ������ţ�" << endl;
            cin >> taskIndex;
            --taskIndex; // ת��Ϊ0����

            if (taskIndex >= 0 && taskIndex < user.tasks.size()) {
                Task& task = user.tasks[taskIndex];
                bool modify = true;
                while (modify) {
                    //��ӡ��ǰ������Ϣ
                    task.displayTaskDetails();
                    cout << "\n---------------------------" << endl;
                    cout << "ѡ��Ҫ�޸ĵ����ݣ�" << endl;
                    cout << "1. �������" << endl;
                    cout << "2. ��������" << endl;
                    cout << "3. ��ֹʱ��" << endl;
                    cout << "4. ����޸�" << endl;
                    cout << "---------------------------" << endl;
                    int choice;
                    cin >> choice;

                    switch (choice) {
                    case 1: { // �޸ı���
                        string newTitle;
                        cout << "�����µ�������⣺" << endl;
                        cin.ignore();
                        getline(cin, newTitle);
                        task.title = newTitle;
                        cout << "��������Ѹ��£�" << endl;
                        break;
                    }
                    case 2: { // �޸�����
                        string newDescription;
                        cout << "�����µ�����������" << endl;
                        cin.ignore();
                        getline(cin, newDescription);
                        task.description = newDescription;
                        cout << "���������Ѹ��£�" << endl;
                        break;
                    }
                    case 3: { // �޸Ľ�ֹʱ��
                        string newDeadlineStr;
                        cout << "�����µĽ�ֹʱ�� (��ʽ: YYYY-MM-DD, HH:MM): " << endl;
                        cin >> newDeadlineStr;

                        try {
                            time_t newDeadline = parseDateTime(newDeadlineStr);
                            task.deadline = newDeadline;
                            cout << "�����ֹʱ���Ѹ��£�" << endl;
                        }
                        catch (const invalid_argument& e) {
                            cout << RED << "��Ч�����ڸ�ʽ��" << RESET << endl;
                        }
                        break;
                    }
                    case 4: { //����޸�
                        modify = false;
                        cout << "�����޸���ɣ�" << endl;
                        break;
                    }
                    default:
                        cout << "��Ч��ѡ�" << endl;
                    }
                }
            }
            else {
                cout << "��Ч�������ţ�" << endl;
            }
            break;
        }

        case 3: { // �޸������ǩ
            cout << "��ǰ�����б�" << endl;
            user.displayTasks();

            int taskIndex;
            cout << "ѡ��Ҫ�޸ı�ǩ�������ţ�" << endl;
            cin >> taskIndex;
            --taskIndex;
            if (taskIndex >= 0 && taskIndex < user.tasks.size()) {
                while (true) {
                    cout << "\n---------------------------" << endl;
                    cout << "��ǰ����ı�ǩ��" << endl;
                    const auto& tags = user.tasks[taskIndex].tags;
                    if (tags.empty()) {
                        cout << "�ޱ�ǩ" << endl;
                    }
                    else {
                        for (size_t i = 0; i < tags.size(); ++i) {
                            cout << i + 1 << ". " << tags[i].name << endl;
                        }
                    }

                    cout << "ѡ�������" << endl;
                    cout << "1. ��ӱ�ǩ" << endl;
                    cout << "2. ɾ����ǩ" << endl;
                    cout << "3. ����޸�" << endl;
                    cout << "---------------------------" << endl;

                    int tagOption;
                    cin >> tagOption;

                    if (tagOption == 1) { // ��ӱ�ǩ
                        string tagName;
                        cout << "�����±�ǩ���ƣ�" << endl;
                        cin >> tagName;
                        user.tasks[taskIndex].addTag(Tag(tagName));
                        cout << "��ǩ����ӣ�" << endl;

                    }
                    else if (tagOption == 2) { // ɾ����ǩ
                        if (tags.empty()) {
                            cout << "û�п�ɾ���ı�ǩ��" << endl;
                        }
                        else {
                            int tagIndex;
                            cout << "ѡ��Ҫɾ���ı�ǩ��ţ�" << endl;
                            cin >> tagIndex;
                            --tagIndex;
                            if (tagIndex >= 0 && tagIndex < tags.size()) {
                                user.tasks[taskIndex].tags.erase(user.tasks[taskIndex].tags.begin() + tagIndex);
                                cout << "��ǩ��ɾ����" << endl;
                            }
                            else {
                                cout << "��Ч�ı�ǩ��ţ�" << endl;
                            }
                        }

                    }
                    else if (tagOption == 3) { // ����޸�
                        cout << "��ǩ�޸���ɣ�" << endl;
                        break;

                    }
                    else {
                        cout << "��Ч��ѡ�������ѡ��" << endl;
                    }
                }
            }
            else {
                cout << "��Ч�������ţ�" << endl;
            }
            break;
        }

        case 4: { // �������
            cout << "��ǰ�����б�" << endl;
            user.displayTasks();

            int taskIndex;
            cout << "ѡ��Ҫ���Ϊ��ɵ������ţ�" << endl;
            cin >> taskIndex;
            --taskIndex;
            if (taskIndex >= 0 && taskIndex < user.tasks.size()) {
                user.tasks[taskIndex].status = "�����";
                cout << "�����ѱ��Ϊ��ɣ�" << endl;
            }
            else {
                cout << "��Ч�������ţ�" << endl;
            }
            break;
        }

        case 5: // ��ӡʱ����
            user.displayTimeline();
            break;

        case 6: { // ���ձ�ǩ��ӡ����
            vector<string> tagList; // �洢��ǩ��˳��
            user.displayAllTags(tagList);

            if (tagList.empty()) {
                cout << "û�п��õı�ǩ��" << endl;
                break;
            }

            cout << "����Ҫɸѡ�ı�ǩ��ţ�" << endl;
            int tagNum;
            cin >> tagNum;

            if (tagNum < 1 || tagNum > tagList.size()) {
                cout << "��Ч�ı�ǩ��š�" << endl;
            }
            else {
                string tagName = tagList[tagNum - 1];
                cout << "ɸѡ��� - ��ǩ: " << tagName << endl;
                user.displayTasksByTag(tagName);
            }
            break;
        }

        case 7: // �˳�
            running = false;
            cout << "�˳�����" << endl;
            break;

        default:
            cout << "��Ч��ѡ�����������룡" << endl;
        }
    }

    return 0;
}
