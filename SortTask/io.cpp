#include "lesson.h"
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

void PrintLesson(const Lesson& L, int index)
{
    cout << index << ") "
        << L.title << " | "
        << L.teacher << " | "
        << L.date << " " << L.time << " | "
        << "ауд. " << L.room << " | "
        << L.type << " | "
        << L.priority;
    if (L.deleted)
    {
        cout << " [DELETED]";
    }
    cout << endl;
}

void menu_display()
{
    cout << "Сортировка и поиск данных в массивах с использованием индексов-массивов" << endl;
    cout << "Выбранная структура данных: Расписание занятий" << endl;
    cout << "Авторы:" << endl;
    cout << "Фролов Б. В." << endl;
    cout << "Фролов Б. В." << endl;
    cout << endl;
    cout << "Программа умеет: вводить, выводить, сортировать данные расписания и выполнять поиск по индексам." << endl;
    cout << "Выберите опцию (для выбора опции введите её номер):" << endl;
    cout << "1 - Ввод данных с клавиатуры" << endl;
    cout << "2 - Вывод данных на экран в порядке ввода" << endl;
    cout << "3 - Вывод данных в текстовый файл" << endl;
    cout << "4 - Ввод данных из текстового файла" << endl;
    cout << "5 - Вывод элементов по индексу названий (по возрастанию)" << endl;
    cout << "6 - Вывод элементов по индексу названий (по убыванию)" << endl;
    cout << "7 - Поиск элементов по значениям ключевых атрибутов" << endl;
    cout << "8 - Редактирование записи" << endl;
    cout << "9 - Логическое удаление записи" << endl;
    cout << "10 - Физическое удаление помеченных записей" << endl;
    cout << "0 - Выход" << endl;
    cout << "Ваш выбор> ";
}

void addLessonFromKeyboard()
{
    if (LessonCount >= MAX_LESSONS)
    {
        cout << "Массив записей расписания переполнен!" << endl;
        return;
    }

    Lesson L;
    L.deleted = false;

	// очищаем буфер ввода
	// numeric_limits<streamsize>::max() - максимально возможное значение типа streamsize
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Введите название занятия> ";
    getline(cin, L.title);

    cout << "Введите ФИО преподавателя> ";
    getline(cin, L.teacher);

    cout << "Введите дату (DD.MM.YYYY)> ";
    getline(cin, L.date);

    cout << "Введите время (HH:MM)> ";
    getline(cin, L.time);

    cout << "Введите аудиторию> ";
    getline(cin, L.room);

    cout << "Введите тип занятия> ";
    getline(cin, L.type);

    cout << "Введите приоритет> ";
    getline(cin, L.priority);

    Lessons[LessonCount] = L;
    LessonCount++;

    cout << "Запись добавлена. Текущее количество записей: " << LessonCount << endl;
}

void printAllLessons()
{
	// выводим все записи в порядке ввода
    if (LessonCount == 0)
    {
        cout << "Нет записей для вывода." << endl;
        return;
    }
    cout << "Вывод записей в порядке ввода:" << endl;
    cout << "Название | Преподаватель | Дата Время | Аудитория | Тип | Приоритет" << endl;
    for (int i = 0; i < LessonCount; i++)
    {
        PrintLesson(Lessons[i], i);
    }
}

void saveToFile(const string& fileName, bool appendMode)
{
    ofstream OutFile;

    if (appendMode)
    {
        OutFile.open(fileName, ios::app);
    }
    else
    {
        OutFile.open(fileName);
    }

    if (!OutFile.is_open())
    {
        cout << "Файл не был открыт для записи!" << endl;
        return;
    }

    for (int i = 0; i < LessonCount; i++)
    {
        Lesson& L = Lessons[i];
        OutFile << L.title << ";"
            << L.teacher << ";"
            << L.date << ";"
            << L.time << ";"
            << L.room << ";"
            << L.type << ";"
            << L.priority << ";"
            << (L.deleted ? 1 : 0)
            << endl;
    }

    OutFile.close();
    cout << "Данные записаны в файл: " << fileName << endl;
}

void loadFromFile(const string& fileName)
{
    ifstream InFile(fileName);

    if (!InFile.is_open())
    {
        cout << "Файл не был открыт для чтения!" << endl;
        return;
    }

    LessonCount = 0;
    string InLineVar;

    while (getline(InFile, InLineVar))
    {
        if (InLineVar.empty()) continue;

        Lesson L;
        L.deleted = false;

        string fields[8];
        int fieldIndex = 0;
        string current = "";

        for (size_t i = 0; i < InLineVar.size(); i++)
        {
            char c = InLineVar[i];
            if (c == ';')
            {
                if (fieldIndex < 8)
                {
                    fields[fieldIndex] = current;
                    fieldIndex++;
                    current = "";
                }
            }
            else
            {
                current += c;
            }
        }
        if (fieldIndex < 8)
        {
            fields[fieldIndex] = current;
            fieldIndex++;
        }

        if (fieldIndex < 7)
        {
            continue;
        }

        L.title = fields[0];
        L.teacher = fields[1];
        L.date = fields[2];
        L.time = fields[3];
        L.room = fields[4];
        L.type = fields[5];
        L.priority = fields[6];
        if (fieldIndex >= 8)
        {
            L.deleted = (fields[7] == "1");
        }

        if (LessonCount < MAX_LESSONS)
        {
            Lessons[LessonCount] = L;
            LessonCount++;
        }
        else
        {
            cout << "Массив заполнен, дальнейшие строки не будут загружены." << endl;
            break;
        }
    }

    InFile.close();
    cout << "Чтение завершено. Загружено записей: " << LessonCount << endl;
}
