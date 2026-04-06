# Web Agent

Кроссплатформенный агент для выполнения задач по командам с сервера.

## Сборка
```bash
mkdir build && cd build
cmake ..
make
./web_agent
cat > src/main.cpp << 'EOF'
#include <iostream>

int main() {
    std::cout << "Web Agent started" << std::endl;
    return 0;
}
