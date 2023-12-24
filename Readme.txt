Небольшой 3d движок. На данный момент умеет рендерить 3д модели (только obj) с материалами, UI-объекты, поддерживает кастомные шейдеры, пост-обработку и др.
Есть система ecs для пользователькой логики.

В проекте используются библиотеки: GLFW и GLAD (для OpenGL), GLM (матричная математика), stb_image.h (импорт изображений), FreeType (шрифты) 

Установка необходимых библиотек:

1)https://www.glfw.org/download.html
Собрать в CMake для VS. В Visual Studio сделать билд, результат glfw3.lib добавить в папку новую папку lib.
Папку include скопировать из скаченной библиотеки.

2)http://glad.dav1d.de/
Перейте в веб-службу GLAD. Установить язык на C++. В разделе API выберать версию OpenGL не ниже 3.3. 
Для профиля установить значение Core и отметить опцию «Создать загрузчик». Нажать «Создать».
Скачать zip-файл. Папки (glad и KHR) в директорию include и добавить файл Glad.c в папку проекта "AlienHeadersLib" с заменой файла.

3)http://glm.g-truc.net/0.9.5/index.html
Добавить в папку include

4)https://freetype.org/index.html
Собрать в СMake. Добавить соотвествующие файлы в папку include и Libs. .dll положить в корневую директорию.

5)Добавить stb_image.h в папку AlienHeadersLib

Прописать путь до include в Solution->Properties->Configeration properties->VC++ directories->Include Directories
Прописать путь до lib в Solution->Properties->Configuration properties->VC++ directories->Library Directories
В Solution->Properties->Linker->Input->Include Directories прописать строчку: glfw3.lib;OpenGL32;freetype.lib


