#include <iostream>
#include <string>
#include <clocale>
#include "lesson.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    int choice;
    string fileName;

    cout << "Введите имя файла для работы с расписанием (например, schedule.txt)> ";
    cin >> fileName;
    cout << endl;

    do
    {
        menu_display();
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            addLessonFromKeyboard();
            break;

        case 2:
            printAllLessons();
            break;

        case 3:
        {
            int mode;
            cout << "1 - создать новый файл, 2 - дописать в конец существующего> ";
            cin >> mode;
            cout << endl;
            saveToFile(fileName, mode == 2);
            break;
        }

        case 4:
            loadFromFile(fileName);
            break;

        case 5:
            buildTitleIndex();
            printByTitleIndex(true);
            break;

        case 6:
            buildTitleIndex();
            printByTitleIndex(false);
            break;

        case 7:
        {
            int subChoice;
            cout << "Выберите вариант поиска:" << endl;
            cout << "1 - по названию (рекурсивный бинарный поиск)" << endl;
            cout << "2 - по дате и времени (итерационный бинарный поиск)" << endl;
            cout << "Ваш выбор> ";
            cin >> subChoice;
            cout << endl;

            if (subChoice == 1)
            {
                buildTitleIndex();
                searchByTitle();
            }
            else if (subChoice == 2)
            {
                buildDateIndex();
                searchByDateTime();
            }
            else
            {
                cout << "Нет такого варианта поиска." << endl;
            }
            break;
        }

        case 8:
            editLesson();
            break;

        case 9:
            logicalDeleteLesson();
            break;

        case 10:
            physicalDeleteMarked();
            break;

        case 0:
            cout << "Завершение работы программы." << endl;
            break;

        default:
            cout << "Нет такой опции меню." << endl;
            break;
        }

        cout << endl;

    } while (choice != 0);

    return 0;
}
