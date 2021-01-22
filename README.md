# Tasks
 Various tasks for programming practice
 ## Описание
Данный репозиторий содержит в себе 2 проекта Clusterizer & Vectorizer

Clusterizer - Производит кластеризацию пространственных данных. На вход поступает какой либо растр(.jpg), выполняющий роль фона, и точки в пределах растра. Ближайщие друг к другу точки объединяются в кластер. Расстояние объединения можно изменять. На растре кластеры отображаются разными цветами и могут иметь свои иконки с указанием количества точек в них. Отображение кластеров может настраиваться с помощью конфига в папке с .exe/аргумеентов командной строки/консольного меню/панели инструментов в окне отображения растра. Имеет два режима объединения. 1: Сливание кластеров включено. Когда два кластера имеют точку которая подходит по растоянию к ним обоим то они сливаются в один. 2: Сливание кластеров отключено. Когда два кластера имеют точку подходящщую по растоянию к ним обоим они не сливаются. Точка остается в кластере "ближайщего соседа".  Реализованно с помощью OpenCV & Boost. [Более подробно](https://github.com/AntKerf/Tasks/tree/main/Clusterizer).

Vectorizer - На вход принимает бинарный растр с черными объектами на нем, может возращать скелет этих обьектов(линии проходящие по центру объекта, векторно описывающие его форму),контуры объектов или все вместе. Растр и параметры могут задаваться через конфиг в папке с .exe/аргументы командной строки/консольное меню приложения/панели инструментов в окне отображения растра.Реализованно с помощью OpenCV & Boost. [Более подробно](https://github.com/AntKerf/Tasks/tree/main/Vectorizer).

В корневой папке репозитория имеется CMakeLists.txt который позволяет собрать оба проекта сразу. Так же каждый проект имеет свой CMakeLists.txt что опозволяет собрать определнный проект. [Более подробно](https://github.com/AntKerf/Tasks/wiki/Build).

Для работы проекта необходим OpenCV собранный с привязкой к QT,для отображения окна просмотра и панели инструментов. Так же необоходим Boost::filesystem, Boost::algorithm, Boost::date_time, boost::property_tree.

## Сборка
Этапы сборки проекта описаны [здесь](https://github.com/AntKerf/Tasks/wiki/Build).
