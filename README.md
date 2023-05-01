# Поиск копий файлов по их crc
Проект появился в связи с потребностью почистить домашнюю коллекция фото от копий.
Копии могут быть даже с разными именами, может даже целые директории скопированы, иногда отобраны копии по тематике.

Следующих вериях развитие будет добавлено
- многопоточность при вычислении crc
- сохранение каталога, загрузка protobuf
- добавить возможность указать список расширений. Пока только одно
- добавить прогресс-вывод

Требуется придумать стратегию по устранению дублей.

Использование:
Usage: search_duplicates <filename_with_catalogue_list>

<filename_with_catalogue_list> contains:
<.extention>
<dir1>
<dir2>

Пример файла:
.cpp
D:/Data/C++/Git для Практикума С++
D:/Data/C++/Code
