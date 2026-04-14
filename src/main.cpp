#include <iostream>
#include <string>
#include <fstream>
//Что делает: Работа с файлами (чтение/запись)
//Зачем: Чтобы сохранять access_code в файл и читать его оттуда
//Бесполезная библиотека на данный момент
#include <chrono>
//Для интервалов между запросами с СЕРВЕРОМ! (для работы со временем)
#include <thread>
//Тоже для интервалов в 5 секунд (для работы с потоками)
#include <cpr/cpr.h>
//Для работы с HTTP: отсылает POST запросы на сервер
#include <nlohmann/json.hpp>
//Для работы с JSON
//JSON — это способ передавать данные в виде текста, понятного и компьютеру, и человеку.
// Расшифровка: JavaScript Object Notation (формат данных как в JavaScript)

using namespace std;
using json = nlohmann::json;
//Пишем просто json вместо nlohmann::json

// Определение ОС
#ifdef _WIN32
    #define OS_WINDOWS
    #define PATH_SEP "\\"
#elif __APPLE__
    #define OS_MACOS
    #define PATH_SEP "/"
#elif __linux__
    #define OS_LINUX
    #define PATH_SEP "/"
#else
    #define PATH_SEP "/"
#endif

// Создание папки для логов
void createLogsDirectory() {
#ifdef _WIN32
    system("if not exist logs mkdir logs");
#else
    system("mkdir -p logs");
#endif
}

// Выполнение любой команды от сервера
int executeCommand(const string& cmd) {
    cout << "Выполнение команды: " << cmd << endl;
    return system(cmd.c_str());
}

// Работа с access_code
string loadAccessCode() {
    ifstream file("access_code.txt");
    string code;
    if (file.is_open()) {
        getline(file, code);
        //Читает первую строку файла
    }
    return code;
}

//По факту бесполезная функция на данный момент (можно было сделать своими ручками)
void saveAccessCode(const string& code) {
    ofstream file("access_code.txt");
    //ofstream - открытие файла для записи
    file << code;
}

int main() {
    cout << "=== Web Agent v0.9 (Cross-platform) ===" << endl;
    cout << "Платформа: ";
#ifdef _WIN32
    cout << "Windows" << endl;
#elif __APPLE__
    cout << "macOS" << endl;
#elif __linux__
    cout << "Linux" << endl;
#else
    cout << "Unknown" << endl;
#endif
    
    // Создаём папку для логов
    createLogsDirectory();
    
    string uid = "Руслан Мазаев ИУ5-23б";
    string descr = "web-agent";
    string base_url = "https://xdev.arkcom.ru:9999/app/webagent1/api/";
    
    string access_code = loadAccessCode();
    
    //Загрузка и получение access_code 
    if (access_code.empty()) {
        cout << "Нет сохранённого access_code. Регистрация..." << endl;
        
        json reg_body = {{"UID", uid}, {"descr", descr}};
        auto reg_response = cpr::Post(
            cpr::Url{base_url + "wa_reg/"},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Body{reg_body.dump()}
        );
        
        cout << "Ответ регистрации: " << reg_response.text << endl;
        
        auto reg_json = json::parse(reg_response.text);
        
        if (reg_json.contains("access_code")) {
            access_code = reg_json["access_code"];
            saveAccessCode(access_code);
            cout << "Зарегистрирован. Access code: " << access_code << endl;
        } else {
            cerr << "Ошибка: access_code не найден в ответе" << endl;
            return 1;
        }
    } else {
        cout << "Загружен access_code: " << access_code << endl;
    }
    
    int poll_interval = 5;
    
    while (true) {
        json task_body = {
            {"UID", uid},
            {"descr", descr},
            {"access_code", access_code}
        };
        
        auto task_response = cpr::Post(
            cpr::Url{base_url + "wa_task/"},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Body{task_body.dump()}
        );
        
        cout << "Ответ сервера: " << task_response.text << endl;
        
        //Обработка ответа сервера
        if (task_response.status_code == 200) {
            auto resp_json = json::parse(task_response.text);
            string code = resp_json.value("code_response", "");
            
            if (code == "1") {
                string task_code = resp_json.value("task_code", "");
                string options = resp_json.value("options", "");
                string session_id = resp_json.value("session_id", "");
                
                cout << "Получено задание: " << task_code << endl;
                cout << "Опции: " << options << endl;
                
                if (task_code == "TASK" || task_code == "FILE") {
                    int result = executeCommand(options);
                    cout << "Команда выполнена. Код: " << result << endl;
                    
                } else if (task_code == "TIMEOUT") {
                    if (!options.empty()) {
                        poll_interval = stoi(options);
                        cout << "Интервал опроса изменён на " << poll_interval << " сек" << endl;
                    }
                    
                } else if (task_code == "CONF") {
                    cout << "Изменение конфигурации: " << options << endl;
                }
            }
        }
        
        this_thread::sleep_for(chrono::seconds(poll_interval));
    }
    
    return 0;
}
