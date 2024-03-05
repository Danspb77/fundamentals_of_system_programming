#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Функция для поиска строки в файле
void search_in_file(const char *filename, const char *search_str) {
    FILE *file = fopen(filename, "r"); // Открытие файла для чтения
    if (file == NULL) {
        fprintf(stderr, "Could not open file: %s\n", filename); // Вывод сообщения об ошибке, если файл не удалось открыть
        return;
    }

    char buffer[1024]; // Буфер для чтения строк из файла
    while (fgets(buffer, sizeof(buffer), file) != NULL) { // Чтение файла построчно
        if (strstr(buffer, search_str) != NULL) { // Поиск строки в текущей строке
            printf("Found in file: %s\n", filename); // Вывод сообщения, если строка найдена
            break; // Прекращение чтения файла после нахождения первого вхождения строки
        }
    }

    fclose(file); // Закрытие файла
}

// Функция для рекурсивного поиска строки в директории
void search_in_directory(const char *path, const char *search_str) {
    DIR *dir; // Структура для работы с директориями
    struct dirent *entry; // Структура для хранения информации о текущем элементе директории
    struct stat statbuf; // Структура для получения информации о файле

    if ((dir = opendir(path)) == NULL) { // Открытие директории
        fprintf(stderr, "Could not open directory: %s\n", path); // Вывод сообщения об ошибке, если директорию не удалось открыть
        return;
    }

    while ((entry = readdir(dir)) != NULL) { // Перебор всех элементов в директории
        char full_path[1024]; // Полный путь к текущему элементу
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name); // Формирование полного пути к текущему элементу

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // Пропуск элементов "." и ".."
            continue;

        if (stat(full_path, &statbuf) == -1) { // Получение информации о текущем элементе
            fprintf(stderr, "Failed to get file status: %s\n", full_path); // Вывод сообщения об ошибке, если информацию о файле не удалось получить
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) { // Проверка, является ли текущий элемент директорией
            search_in_directory(full_path, search_str); // Рекурсивный вызов функции для обработки вложенной директории
        } else {
            search_in_file(full_path, search_str); // Вызов функции для поиска строки в файле
        }
    }

    closedir(dir); // Закрытие директории
}

int main(int argc, char *argv[]) {
    if (argc != 3) { // Проверка количества аргументов командной строки
        printf("Usage: %s <directory> <search_string>\n", argv[0]); // Вывод сообщения о правильном использовании программы
        return 1;
    }

    const char *directory = argv[1]; // Путь к директории для поиска
    const char *search_str = argv[2]; // Строка для поиска

    search_in_directory(directory, search_str); // Вызов функции для начала поиска в указанной директории

    return 0;
}
