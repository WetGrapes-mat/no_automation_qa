# CI/CD

[![auto_build](https://github.com/WetGrapes-mat/no_automation_qa/actions/workflows/main.yml/badge.svg?branch=main&event=push)](https://github.com/WetGrapes-mat/no_automation_qa/actions/workflows/main.yml)

# Команды

## Аналоги команд под MacOS

Просмотр зависимостей:

- ldd file_name

Аналог:

- otool -L file_name

Подмена библиотек в бинарнике:

- patchelf --replace-needed lib.so.0 /path/to/new/lib.so.0 your_binary_file

Аналог:

- install_name_tool -change /path/to/old/lib.dylib /path/to/new/lib.dylib your_binary_file
