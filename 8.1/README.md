## Команды

Команда для компиляции программы в исполняемый файл:

```bash
gcc program.c -o pr
```

Команды для создания текстового файла с правами "чтение только владельцу":

```bash
echo "Это тестовый файл." > /path/to/file.txt
chmod 400 /path/to/file.txt
```

Команда для установки SUID-бита на файл pr:

```bash
sudo chmod u+s /path/to/pr
```

Конмада для создания нового пользователя (если необходимо): 

```bash
sudo adduser newuser
```

Команда для запуска файла pr от имени другого пользователя:

```bash
su - newuser -c "/path/to/pr"
```

*В программе program.c также нужно заменить /path/to/file.txt на ваш путь к текстовому файлу.