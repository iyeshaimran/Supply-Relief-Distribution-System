/* Menu-driven program that scores each emergency request (0-100) and decides
   how many trucks and supply packs to send, based on what is left in stock. */

#include <cctype>   // headerfile for lower function
#include <cmath>    // headerfile for power and Napier's constant
#include <iomanip>  // headerfile for output stream formatting
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
    const float k = 60.0;       // controls how quickly the curve flattens out
    const int max_score = 150;  // maximum possible raw score

    // Widths for table layouts
    const int value_width = 13;
    const int request_label_width = 18;
    const int supply_label_width = 18;
    const int summary_label_width = 20;
    const int summary_value_width = 10;

    // Inventory levels
    int trucks = 10;
    int supply_packs = 200;

    // Operational summary tracking counters
    int requests = 0;
    int handled_cases = 0;
    int deferred_cases = 0;

    // Variable to store user choice
    int choice;

    // Need score calculated using inputs
    float need_score;

    // Menu-driven program loop continues until user enters 2
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
            requests++;

            int provided_trucks = 0;
            int provided_supply_packs = 0;

            char answer; // reused for every char prompt
            int damage_level, families_affected;

            // Boolean conversion for inputs having only two possible values
            bool roads_access, flood_warning, medical_need, previous_aid;

            // Color codes to concatenate according to condition
            string output_damage_level;
            string color_damage_level = "", color_roads_access = "";

            while (true)
            {
                cout << "\nEnter Damage Level ['h' for high / 'm' for medium / 'l' for low]: ";
                cin >> answer;
                answer = tolower(answer);

                switch (answer)
                {
                case 'h':
                    damage_level = 3;
                    output_damage_level = "High";
                    color_damage_level = red;
                    break;

                case 'm':
                    damage_level = 2;
                    output_damage_level = "Medium";
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

                break;
            }

            // Loop to get the number within range
            do
            {
                cout << "\nEnter the number of families affected [10-200]: ";
                cin >> families_affected;

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
                    color_roads_access = red;
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

            // Base score calculation
            float base_score = 25 * pow(normalised_damage_level, 1.5) + 20 * pow(normalised_families_affected, 2)
                + (roads_access ? 0 : 15) + (flood_warning ? 10 : 0)
                + (medical_need ? 20 : 0) + (previous_aid ? -5 : 10);

            // Risk condition bonuses
            int two_conditions_bonus = ((medical_need && flood_warning) ? 10 : 0) + ((medical_need && !roads_access) ? 10 : 0) + ((flood_warning && !previous_aid) ? 5 : 0) + ((!roads_access && !previous_aid) ? 5 : 0);
            int three_conditions_bonus = ((damage_level == 3 && medical_need && flood_warning) ? 10 : 0) + (((families_affected >= 100) && medical_need && !roads_access) ? 10 : 0);

            float total_score = base_score + two_conditions_bonus + three_conditions_bonus;

            // Exponential transformation
            need_score = 100 * ((1 - exp(-total_score / k)) / (1 - exp(-max_score / k)));
            need_score = (need_score < 0) ? 0 : need_score;

            // Request information table
            cout << "\n------- Request Information --------\n"
                << '+' << setfill('-') << setw(request_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl
                << '|' << right << setw(request_label_width) << "Damage Level" << " | " << color_damage_level << left << setw(value_width) << output_damage_level << reset << '|' << endl
                << '|' << right << setw(request_label_width) << "Families Affected" << " | " << left << setw(value_width) << families_affected << '|' << endl
                << '|' << right << setw(request_label_width) << "Access Route" << " | " << color_roads_access << left << setw(value_width) << (roads_access ? "Open" : "Damaged") << reset << '|' << endl
                << '|' << right << setw(request_label_width) << "Flood Warning" << " | " << left << setw(value_width) << (flood_warning ? "Yes" : "No") << '|' << endl
                << '|' << right << setw(request_label_width) << "Medical Need" << " | " << left << setw(value_width) << (medical_need ? "Yes" : "No") << '|' << endl
                << '|' << right << setw(request_label_width) << "Previous Aid" << " | " << left << setw(value_width) << (previous_aid ? "Received" : "Not Received") << '|' << endl
                << '+' << setfill('-') << setw(request_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl;

            cout << "\n------ Calculated Need Score -------\n"
                << "Need Score: " << fixed << setprecision(3) << need_score << endl;

            // Decides aid distribution
            cout << "\n--------- Selected Action ----------\n";

            if ((need_score > 75) && (need_score <= 100))       //This case prioritizes Full Relief.
            {
                if ((trucks >= 3) && (supply_packs >= 60))
                {
                    cout << "This case will get Full Relief." << endl;
                    provided_trucks = 3;
                    provided_supply_packs = 60;
                }
                else
                {
                    cout << "Full Relief unavailable. Checking Standard Relief..." << endl;     //Checks for Standard Relief if Full Relief is not possible.

                    if ((trucks >= 2) && (supply_packs >= 50))
                    {
                        cout << "\nThis case will get Standard Relief." << endl;
                        provided_trucks = 2;
                        provided_supply_packs = 50;
                    }
                    else if ((trucks >= 1) && (supply_packs >= 30))
                    {
                        cout << "\nStandard Relief unavailable. Providing Emergency Kit." << endl;   //Checks for availabilty of Emergency Kit if Standard Relief is not possible.
                        provided_trucks = 1;
                        provided_supply_packs = 30;
                    }
                }
            }
            else if ((need_score > 50) && (need_score <= 75))       //This case prioritizes Standard Relief.
            {
                if ((trucks >= 2) && (supply_packs >= 50))
                {
                    cout << "This case will get Standard Relief." << endl;
                    provided_trucks = 2;
                    provided_supply_packs = 50;
                }
                else if ((trucks >= 1) && (supply_packs >= 30))
                {
                    cout << "\nStandard Relief unavailable. Providing Emergency Kit." << endl;  //Checks for availability of Emergency Kit if Standard Relief is not possible.
                    provided_trucks = 1;
                    provided_supply_packs = 30;
                }
            }
            else if ((need_score > 25) && (need_score <= 50))       //This case tends to provide an Emergency Kit.
            {
                if ((trucks >= 1) && (supply_packs >= 30))
                {
                    cout << "This case will get an Emergency Kit." << endl;
                    provided_trucks = 1;
                    provided_supply_packs = 30;
                }
            }

            // Updates counters and remaining resources
            if (provided_trucks > 0)
            {
                handled_cases++;        //Handled cases will be incrememted only if trucks are provided.
                trucks -= provided_trucks;
                supply_packs -= provided_supply_packs;
            }
            else
            {
                deferred_cases++;

                if (need_score <= 25)
                {
                    cout << yellow
                        << "Request Deferred: Need score is too low."       //Defers a case based on low need score.
                        << reset << endl;
                }
                else if (trucks < 1 && supply_packs < 30)
                {
                    cout << yellow
                        << "Request Deferred: Insufficient trucks and supply packs."    //Defers a case if both trucks and supply packs are insufficient.
                        << reset << endl;
                }
                else if (trucks < 1)
                {
                    cout << yellow
                        << "Request Deferred: Insufficient trucks."                    //Defers a case if required number of trucks are not available.
                        << reset << endl;
                }
                else
                {
                    cout << yellow
                        << "Request Deferred: Insufficient supply packs."              //Defers a case if required number of supply packs are not available.
                        << reset << endl;
                }
            }

            // Table for supplies delivered
            cout << "\n-------- Supply Distribution --------\n"
                << '+' << setfill('-') << setw(supply_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl
                << '|' << right << setw(supply_label_width) << "Trucks" << " | " << left << setw(value_width) << provided_trucks << '|' << endl
                << '|' << right << setw(supply_label_width) << "Supply Packs" << " | " << left << setw(value_width) << provided_supply_packs << '|' << endl
                << '+' << setfill('-') << setw(supply_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl;

            // Table for supplies remaining
            cout << "\n-------- Remaining Supplies --------\n"
                << '+' << setfill('-') << setw(supply_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl
                << '|' << right << setw(supply_label_width) << "Trucks" << " | " << left << setw(value_width) << trucks << '|' << endl
                << '|' << right << setw(supply_label_width) << "Supply Packs" << " | " << left << setw(value_width) << supply_packs << '|' << endl
                << '+' << setfill('-') << setw(supply_label_width + 3 + value_width) << "" << '+' << setfill(' ') << endl;

            cout << endl;
            break;
        }

        case 2:
            break;

        default:
            cout << red << "\nInvalid choice! Please enter one of the given choices.\n" << reset;
        }

    } while (choice != 2);

    // Print the summary when the user exits
    cout << "\n=== SUPPLY DISTRIBUTION SUMMARY ===\n"
        << '+' << setfill('-') << setw(summary_label_width + 3 + summary_value_width) << "" << '+' << setfill(' ') << endl
        << '|' << right << setw(summary_label_width) << "Requests Received" << " | " << left << setw(summary_value_width) << requests << '|' << endl
        << '|' << right << setw(summary_label_width) << "Requests Handled" << " | " << left << setw(summary_value_width) << handled_cases << '|' << endl
        << '|' << right << setw(summary_label_width) << "Requests Deferred" << " | " << left << setw(summary_value_width) << deferred_cases << '|' << endl
        << '+' << setfill('-') << setw(summary_label_width + 3 + summary_value_width) << "" << '+' << setfill(' ') << endl;

    return 0;
}
