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
    string fileName;

    cout << msg.promptFileName;
    cin >> fileName;
    cout << endl;

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
            int mode;
            cout << msg.createOrAppendPrompt;
            cin >> mode;
            cout << endl;
            SaveToFile(fileName, mode == 2);
        }
        else if (choice == 4)
        {
            LoadFromFile(fileName);
        }
        else if (choice == 5)
        {
            BuildTitleIndex();
            PrintByTitleIndex(true);
        }
        else if (choice == 6)
        {
            BuildTitleIndex();
            PrintByTitleIndex(false);
        }
        else if (choice == 7)
        {
            int subChoice;
            cout << msg.searchMenuHeading << endl;
            cout << msg.searchByTitleOption << endl;
            cout << msg.searchByDateOption << endl;
            cout << msg.searchChoicePrompt;
            cin >> subChoice;
            cout << endl;

            if (subChoice == 1)
            {
                BuildTitleIndex();
                SearchByTitle();
            }
            else if (subChoice == 2)
            {
                BuildDateIndex();
                SearchByDateTime();
            }
            else
            {
                cout << msg.searchInvalidOption << endl;
            }
        }
        else if (choice == 8)
        {
            EditLesson();
        }
        else if (choice == 9)
        {
            LogicalDeleteLesson();
        }
        else if (choice == 10)
        {
            PhysicalDeleteMarked();
        }
        else if (choice == 11)
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
