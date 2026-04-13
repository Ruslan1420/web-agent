#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string getAccessCodePath() {
    return "access_code.txt";
}

std::string loadAccessCode() {
    std::ifstream file(getAccessCodePath());
    std::string code;
    if (file.is_open()) {
        std::getline(file, code);
    }
    return code;
}

void saveAccessCode(const std::string& code) {
    std::ofstream file(getAccessCodePath());
    file << code;
}

int main() {
    std::cout << "=== Web Agent v0.8 ===" << std::endl;
    
    std::string uid = "WEBAGENT010";
    std::string descr = "web-agent";
    std::string base_url = "https://xdev.arkcom.ru:9999/app/webagent1/api/";
    
    std::string access_code = loadAccessCode();
    
    if (access_code.empty()) {
        std::cout << "Нет сохранённого access_code. Регистрация..." << std::endl;
        
        json reg_body = {{"UID", uid}, {"descr", descr}};
        auto reg_response = cpr::Post(
            cpr::Url{base_url + "wa_reg/"},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Body{reg_body.dump()}
        );
        
        std::cout << "Ответ регистрации: " << reg_response.text << std::endl;
        
        auto reg_json = json::parse(reg_response.text);
        
        if (reg_json.contains("access_code")) {
            access_code = reg_json["access_code"];
            saveAccessCode(access_code);
            std::cout << "Зарегистрирован. Access code: " << access_code << std::endl;
        } else {
            std::cerr << "Ошибка: access_code не найден в ответе" << std::endl;
            return 1;
        }
    } else {
        std::cout << "Загружен access_code: " << access_code << std::endl;
    }
    
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
        
        std::cout << "Ответ сервера: " << task_response.text << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    return 0;
}
