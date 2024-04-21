## Команды

Команда для компиляции программы `static_lib.c` в исполняемый файл:

```bash
gcc -c static_lib.c -o static_lib.o
```
Команда для создания статической библиотеки из объектного файла: 

```bash
ar rcs libhello.a static_lib.o
```
Команда для компиляции программы `hello_static.c` в исполняемый файл:

```bash
gcc -o hello_static hello_static.c -L. -lhello
```
