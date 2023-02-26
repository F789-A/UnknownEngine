Небольшой 3d движок. На данный момент умеет рендерить 3д модели (только obj) с материалами, поддерживает кастомные шейдеры, пост-обработку и др.
Есть система компонентов для пользователькой логики.

В проекте используются библиотеки: GLFW и GLAD (для OpenGL), GLM (математика), stb_image.h (импорт изображений) 

Установка необходимых библиотек:

https://www.glfw.org/download.html
Собрать в cMake для VS. В Visual Studio сделать билд, результат glfw3.lib добавить в папку новую папку lib.
Папку include скопировать из скаченной библиотеки.

http://glad.dav1d.de/
Перейте в веб-службу GLAD. Установить язык на C++. В разделе API выберать версию OpenGL не ниже 3.3. 
Для профиля установить значение Core и отметить опцию «Создать загрузчик». Нажать «Создать».
Скачать zip-файл. Папки (glad и KHR) в директорию include и добавить файл Glad.c в папку проекта "AlienHeadersLib" с заменой файла.

http://glm.g-truc.net/0.9.5/index.html
Добавить в папку include

Прописать путь до include в Solution->Properties->Configeration properties->VC++ directories->Include Directories
Прописать путь до lib в Solution->Properties->Configuration properties->VC++ directories->Library Directories
В Solution->Properties->Linker->Input->Include Directories прописать строчку: glfw3.lib;OpenGL32