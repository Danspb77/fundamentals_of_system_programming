CC=gcc
CFLAGS=-Wall -Wextra -Werror

# Имя исполняемого файла
EXEC=my_executable

# Исходные файлы
SRCS=main.c

# Объектные файлы
OBJS=$(SRCS:.c=.o)

# Сборка исполняемого файла
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Компиляция исходных файлов в объектные файлы
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Цель для очистки проекта
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: clean
