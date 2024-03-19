#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Функция для поиска заданной последовательности байтов в файле
void search_in_file(const char *filename, const char *search_sequence) {
    FILE *file = fopen(filename, "rb"); // Открытие файла в бинарном режиме для чтения
    if (file == NULL) { // Проверка успешного открытия файла
        fprintf(stderr, "Could not open file: %s\n", filename); // Вывод сообщения об ошибке
        return;
    }

    // Конвертация последовательности поиска из шестнадцатеричного в бинарный формат
    size_t search_len = strlen(search_sequence) - 2; // Длина последовательности в шестнадцатеричном формате
    unsigned char *search_bytes = (unsigned char *)malloc((search_len / 2) * sizeof(unsigned char)); // Выделение памяти под байты поиска
    if (search_bytes == NULL) { // Проверка успешного выделения памяти
        fprintf(stderr, "Memory allocation failed\n"); // Вывод сообщения об ошибке
        fclose(file); // Закрытие файла
        return;
    }

    // Конвертация строкового представления шестнадцатеричных байт в бинарный формат
    sscanf(search_sequence + 2, "%2hhx", &search_bytes[0]); // Конвертация первого байта
    for (size_t i = 1; i < search_len / 2; i++) { // Конвертация остальных байт
        sscanf(search_sequence + 2 * i + 2, "%2hhx", &search_bytes[i]);
    }

    unsigned char buffer[1024]; // Буфер для чтения байтов из файла
    size_t bytes_read;
    size_t match_index = 0; // Индекс для отслеживания прогресса совпадений

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) { // Чтение файла порциями
        for (size_t i = 0; i < bytes_read; i++) { // Перебор байтов в текущей порции
            if (buffer[i] == search_bytes[match_index]) { // Проверка совпадения текущего байта
                match_index++; // Увеличение индекса совпадений
                if (match_index == search_len / 2) { // Если вся последовательность найдена
                    printf("Found sequence in file: %s\n", filename); // Вывод сообщения о найденной последовательности
                    fclose(file); // Закрытие файла
                    free(search_bytes); // Освобождение выделенной памяти
                    return;
                }
            } else {
                match_index = (buffer[i] == search_bytes[0]) ? 1 : 0; // Сброс индекса, если текущий байт не соответствует последовательности
            }
        }
    }

    fclose(file); // Закрытие файла
    free(search_bytes); // Освобождение выделенной памяти
}

// Функция для рекурсивного поиска в директории и поддиректориях
void search_in_directory(const char *path, const char *search_sequence) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if ((dir = opendir(path)) == NULL) { // Открытие директории
        fprintf(stderr, "Could not open directory: %s\n", path); // Вывод сообщения об ошибке
        return;
    }

    while ((entry = readdir(dir)) != NULL) { // Перебор элементов в директории
        char full_path[1024]; // Полный путь к текущему элементу
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name); // Формирование полного пути

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // Пропуск специальных элементов
            continue;

        if (stat(full_path, &statbuf) == -1) { // Получение информации о текущем элементе
            fprintf(stderr, "Failed to get file status: %s\n", full_path); // Вывод сообщения об ошибке
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) { // Проверка, является ли текущий элемент директорией
            search_in_directory(full_path, search_sequence); // Рекурсивный вызов для поддиректории
        } else {
            search_in_file(full_path, search_sequence); // Вызов функции поиска для файла
        }
    }

    closedir(dir); // Закрытие директории
}

// Основная функция программы
int main(int argc, char *argv[]) {
    if (argc != 3) { // Проверка наличия правильного числа аргументов
        printf("Usage: %s <directory> <search_sequence>\n", argv[0]); // Вывод сообщения о правильном использовании программы
        return 1;
    }

    const char *directory = argv[1]; // Путь к директории для поиска
    const char *search_sequence = argv[2]; // Последовательность для поиска

    search_in_directory(directory, search_sequence); // Вызов функции для начала поиска в указанной директории

    return 0;
}
