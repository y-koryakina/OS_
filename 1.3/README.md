## Компиляция и сборка

Создание динамической библиотеки из файла `dynamic_lib.c`:

```bash
gcc -shared -o libdynamic_lib.so dynamic_lib.c -fPIC
```

Компиляция `hello_dynamic.c`:

```bash
gcc -o hello_dynamic hello_dynamic.c -L. -ldynamic_lib -Wl,-rpath=.
```
