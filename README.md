# Web Agent

Кроссплатформенный агент для выполнения задач по командам с сервера.

## Сборка
```bash
cd ~/web-agent
rm -rf build
mkdir build && cd build
cp ~/web-agent/access_code.txt ~/web-agent/build/
cmake ..
make
./web_agent
