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
        menu_display();
        cin >> choice;
        cout << endl;

        if (choice == 1)
        {
            addLessonFromKeyboard();
        }
        else if (choice == 2)
        {
            printAllLessons();
        }
        else if (choice == 3)
        {
            int mode;
            cout << msg.createOrAppendPrompt;
            cin >> mode;
            cout << endl;
            saveToFile(fileName, mode == 2);
        }
        else if (choice == 4)
        {
            loadFromFile(fileName);
        }
        else if (choice == 5)
        {
            buildTitleIndex();
            printByTitleIndex(true);
        }
        else if (choice == 6)
        {
            buildTitleIndex();
            printByTitleIndex(false);
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
                cout << msg.searchInvalidOption << endl;
            }
        }
        else if (choice == 8)
        {
            editLesson();
        }
        else if (choice == 9)
        {
            logicalDeleteLesson();
        }
        else if (choice == 10)
        {
            physicalDeleteMarked();
        }
        else if (choice == 11)
        {
            restoreDeletedLesson();
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
