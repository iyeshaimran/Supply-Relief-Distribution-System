/*Menu-driven program that scores each emergency request (0-100) and decides
how many trucks and supply packs to send, based on what is left in stock. */

#include <cctype> //headerfile for lower function
#include <cmath> // headerfile for power and Napier's constant
#include <iomanip> // headerfile for output stream formatting
#include <iostream>
#include <string>

using namespace std;

int main()
{
    // ANSI escape codes for coloured terminal output
    const string red = "\033[31m";
    const string yellow = "\033[33m";
    const string reset = "\033[0m";

    // Scoring constants
    const float k = 60.0; // controls how quickly the curve flattens out
    const int max_score = 150; // maximum possible raw score

    // Widths for table layouts
    const int value_width = 13;
    const int request_label_width = 18;

    // variable to store user choice;
    int choice;

    // need score calculated using inputs
    float need_score;

    // menu driven program, loop continues untill user enters 2
    do
    {
        cout << "==== RELIEF SUPPLY DISTRIBUTION SYSTEM ====\n"
             << "Select an option [1/2]\n"
             << "1. Submit an Emergency Request.\n"
             << "2. Exit the Relief Supply System.\n\n"
             << "Option: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                // Variables declared inside this block so they start fresh for each request

                char answer; // reused for every char prompt

                int damage_level, families_affected;
                
                // Boolean conversion for inputs having only two possible values
                bool roads_access, flood_warning, medical_need, previous_aid;
                
                // color codes to concatenate according to condition
                string output_damage_level;
                string color_damage_level = "", color_roads_access = ""; // empty = default terminal color

                while (true)
                {
                    cout << "\nEnter Damage Level ['h' for high / 'm' for medium / 'l' for low]: ";
                    cin >> answer;
                    answer = tolower(answer);

                    switch(answer)
                    {
                        case 'h':
                            damage_level = 3;
                            output_damage_level = "High";
                            color_damage_level = red; // High is highlighted as red
                            // breaks only the switch statement
                            break;

                        case 'm':
                            damage_level = 2;
                            output_damage_level = "Medium"; // Medium is highlighted as yellow
                            color_damage_level = yellow;
                            break;

                        case 'l':
                            damage_level = 1;
                            output_damage_level = "Low";
                            break;

                        default:
                            cout << red << "\nInvalid choice! Enter one of the given choices.\n" << reset;
                            continue;
                    }

                    // break the while loop once a valid option is entered
                    break;
                }

                // loop to get the number within range
                do
                {
                    cout << "\nEnter the number of families affected [10-200]: ";
                    cin >> families_affected;

                    // prints a message to the console in case of inadmissible input
                    if (families_affected < 10 || families_affected > 200)
                        cout << red << "\nInvalid number! Enter the number within range.\n" << reset;

                } while (families_affected < 10 || families_affected > 200);

                while (true)
                {
                    cout << "\nAre the roads open? [y/n]: ";
                    cin >> answer;
                    answer = tolower(answer);

                    if (answer == 'y')
                    {
                        roads_access = true;
                        break;
                    }

                    else if (answer == 'n')
                    {
                        roads_access = false;
                        color_roads_access = red; // damaged roads are highlighted as red
                        break;
                    }

                    else
                    {
                        cout << red << "\nInvalid choice! Please enter one of the given choices.\n" << reset;
                    }
                }

                while (true)
                {
                    cout << "\nIs there a flood warning? [y/n]: ";
                    cin >> answer;
                    answer = tolower(answer);

                    if (answer == 'y')
                    {
                        flood_warning = true;
                        break;
                    }

                    else if (answer == 'n')
                    {
                        flood_warning = false;
                        break;
                    }

                    else
                    {
                        cout << red << "\nInvalid choice! Please enter one of the given choices.\n" << reset;
                    }
                }

                while (true)
                {
                    cout << "\nIs there a medical need? [y/n]: ";
                    cin >> answer;
                    answer = tolower(answer);

                    if (answer == 'y')
                    {
                        medical_need = true;
                        break;
                    }

                    else if (answer == 'n')
                    {
                        medical_need = false;
                        break;
                    }

                    else
                    {
                        cout << red << "\nInvalid choice! Please enter one of the given choices.\n" << reset;
                    }
                }

                while (true)
                {
                    cout << "\nHas previous aid been received? [y/n]: ";
                    cin >> answer;
                    answer = tolower(answer);

                    if (answer == 'y')
                    {
                        previous_aid = true;
                        break;
                    }

                    else if (answer == 'n')
                    {
                        previous_aid = false;
                        break;
                    }

                    else
                    {
                        cout << red << "\nInvalid choice! Please enter one of the given choices.\n" << reset;
                    }
                }

                // Scale the values for damage level and families affected to 0-1
                float normalised_families_affected = (families_affected - 10) / 190.0;
                float normalised_damage_level = (damage_level - 1) / 2.0;

                // Base score is weighted sum of the inputs, penalty only if previous aid is received
                float base_score = 25 * pow(normalised_damage_level, 1.5) + 20 * pow(normalised_families_affected, 2) + (roads_access ? 0 : 15) + (flood_warning ? 10 : 0)
                + (medical_need ? 20 : 0) + (previous_aid? -5 : 10);

                // Bonus if three two factors occur together (maximum 30)
                int two_conditions_bonus = ((medical_need && flood_warning) ? 10 : 0) + ((medical_need && !roads_access) ? 10 : 0) + ((flood_warning && !previous_aid) ? 5 : 0) + ((!roads_access && !previous_aid) ? 5 : 0);

                // Bonus if three risk factors occur together (maximum 20)
                int three_conditions_bonus = ((damage_level == 3 && medical_need && flood_warning) ? 10 : 0)  + (((families_affected >= 100) && medical_need && !roads_access) ? 10 : 0);

                float total_score = base_score + two_conditions_bonus + three_conditions_bonus;

                // exponential transformation so high scoring cases get progressively more priority
                need_score = 100 * ((1 - exp(-total_score/k))/ (1 - exp(-max_score/k)));

                // if the score is negative for a very low raw score, cap it at 0 
                need_score = (need_score < 0) ? 0 : need_score;
                         
                // Print the table for request information
                cout << "\n------- Request Information --------\n"
                      << '+' << setfill('-') << setw(request_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl
                      << '|' << right << setw(request_label_width) << "Damage Level" << " | " << color_damage_level << left << setw(value_width) << output_damage_level << reset << '|' << endl
                      << '|' << right << setw(request_label_width) << "Families Affected" << " | " << left << setw(value_width) << families_affected << '|' << endl
                      << '|' << right << setw(request_label_width) << "Access Route" << " | " << color_roads_access << left << setw(value_width) << (roads_access ? "Open" : "Damaged") << reset << '|' << endl
                      << '|' << right << setw(request_label_width) << "Flood Warning" << " | " << left << setw(value_width) << (flood_warning ? "Yes" : "No") << '|' << endl
                      << '|' << right << setw(request_label_width) << "Medical Need" << " | " << left << setw(value_width) << (medical_need ? "Yes" : "No") << '|' << endl
                      << '|' << right << setw(request_label_width) << "Previous Aid" << " | " << left << setw(value_width) << (previous_aid ? "Received" : "Not Received") << '|' << endl
                      << '+' << setfill('-') << setw(request_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl;
                      
                cout << "\n----- Calculated Need Score -----\n"
                     << "Need Score: " << fixed << setprecision(3) << need_score << endl;

                break;
            }

            case 2:
            {
                break;
            }

            default:
            {
                cout << red << "\nInvalid choice! Please enter one of the given choices.\n" << reset;
            }
        }

    } while (choice != 2);

    return 0;
}