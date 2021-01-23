# Clusterizer
## Описание
Clusterizer - Производит кластеризацию пространственных данных. На вход поступает какой либо растр(.jpg), выполняющий роль фона, и точки в пределах растра.
Ближайщие друг к другу точки объединяются в кластер. Расстояние объединения можно изменять. 
На растре кластеры отображаются разными цветами и могут иметь свои иконки с указанием количества точек в них.
Отображение кластеров может настраиваться с помощью конфига в папке с .exe/аргумеентов командной строки/консольного меню/панели инструментов в окне отображения растра.
Имеет два режима объединения.
1: Сливание кластеров включено. Когда два кластера имеют точку которая подходит по растоянию к ним обоим то они сливаются в один.
2: Сливание кластеров отключено. Когда два кластера имеют точку подходящщую по растоянию к ним обоим они не сливаются. Точка остается в кластере "ближайщего соседа".
Реализованно с помощью OpenCV & Boost.

## Управление
Приложение может быть из консоли с аргументами:

-i 'path To Image'  | Задает путь к растру

-p x y              | Устанавливает точку на растре с координатами x:y

-r m                | Задает радиус объединения m, растояние от точки в зоне которого она будет искать "соседей"

-s 'save directory' | Задает путь для сохранения растра

-d                  | отображает окно просмотра растра

-h                  | показывает справку, помощь

Приложение может быть запущено через .exe, в этом случает запуститься консольное меню с выбором команд

Есть возможно не задавать все точки через консоль, а поставить их вручную на растре в окно оторбажения растра с помощью Ctrl + ЛКМ.

Для изменения радиуса объединения снизу присутсвует TrackBar 'Radius'.

Так же в окне отображения можно открыть панель настроек(верхний правый значок)

Панель инструментов позволяет:
'Fonst size' - регулирует размер шрифта

'Clear' - очистка всех точек и кластеров

'Points' - отобрадение точек на растре

'Text' - отображение текстовой информации, количества точек в кластере

'Clusters' - отображение иконок кластера

'Lines' - соединяет точки кластера линиями и отображает

'Merging clusters' - переключение между режимами слияния кластеров

'Fast compute' - режим быстрого вычисления. В режиме быстрого вычесления не вычесляютя центры кластеров и иконки кластеров и текст оторбражается на последней входящей в кластер точке.

## Файлы
В папке 'Source' содержатся исходники кода на C++. 

Проект может быть собран через CMake. [Подробнее](https://github.com/AntKerf/Tasks/wiki/Build).
## Скриншот
![Clusterizer](https://i.ibb.co/60J7HLc/image.png)