#include <iostream>
#include <string>
#include <clocale>
#include "lesson.h"
#include "messages.h"
#include "validation.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, GetLocaleConfig().localeName);
    const auto& msg = GetStrings();

    int choice;

    bool running = true;
    while (running)
    {
        MenuDisplay();
        cin >> choice;
        cout << endl;

        if (choice == 1)
        {
            AddLessonFromKeyboard();
        }
        else if (choice == 2)
        {
            PrintAllLessons();
        }
        else if (choice == 3)
        {
            string fileName;
            cout << msg.promptFileName;
            cin >> fileName;
            cout << endl;
            int mode;
            cout << msg.createOrAppendPrompt;
            cin >> mode;
            cout << endl;
            SaveToFile(fileName, mode == 2);
        }
        else if (choice == 4)
        {
            string fileName;
            cout << msg.promptFileName;
            cin >> fileName;
            cout << endl;
            LoadFromFile(fileName);
        }
        else if (choice == 5)
        {
            ChooseIndexField();
        }
        else if (choice == 6)
        {
            BuildIndexForCurrentField();
        }
        else if (choice == 7)
        {
            PrintByCurrentIndex(true);
        }
        else if (choice == 8)
        {
            PrintByCurrentIndex(false);
        }
        else if (choice == 9)
        {
            SearchByCurrentIndex();
        }
        else if (choice == 10)
        {
            EditLesson();
        }
        else if (choice == 11)
        {
            LogicalDeleteLesson();
        }
        else if (choice == 12)
        {
            PhysicalDeleteMarked();
        }
        else if (choice == 13)
        {
            RestoreDeletedLesson();
        }
        else if (choice == 0)
        {
            cout << msg.exitProgram << endl;
            running = false;
        }
        else
        {
            cout << msg.invalidMenuOption << endl;
        }

        cout << endl;
    }

    return 0;
}
