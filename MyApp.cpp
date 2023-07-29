// Copyright 2023
// My Fitness Application....
// ...
// Author: Ali Rezazadah

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

const int NUM_EXERCISES = 3;
const int NUM_SETS = 5;
const int STARTING_REPS = 5;

// Function prototypes
void printMenu();
bool login(const string &username, const string &password);
void signUp();
void clearInputBuffer();
void applicationOne();
void applicationTwo();
void applicationThree();
void setMaxLiftValues(double lifts[]);
void writeMaxLiftValuesToFile(const double lifts[]);
void readMaxLiftValuesFromFile(double lifts[]);
vector<vector<pair<int, double>>> generateSetsAndReps(const double lifts[]);
void writeSetsAndRepsToFile(const vector<vector<pair<int, double>>> &allSetsAndReps, const string &fileName);

void printMenu()
{
    cout << "===== Main Menu =====" << endl;
    cout << "1. Login" << endl;
    cout << "2. Sign Up" << endl;
    cout << "3. Exit" << endl;
    cout << "4. Application One (MaxLifts/Sets/Reps)" << endl;
    cout << "5. Application Two (Calorie Calculator)" << endl;
    cout << "6. Application Three (Water intake)" << endl;
}

bool isUsernameExists(const string &username)
{
    ifstream userFile("users.txt");
    if (!userFile)
    {
        cout << "Unable to open users.txt file." << endl;
        return false;
    }

    string line;
    while (getline(userFile, line))
    {
        size_t found = line.find(username);
        if (found != string::npos)
        {
            userFile.close();
            return true; // Username already exists
        }
    }

    userFile.close();
    return false; // Username does not exist
}

// Function to add a new user to the user database
void addUser(const string &username, const string &password)
{
    ofstream userFile("users.txt", ios::app);
    if (!userFile)
    {
        cout << "Unable to open users.txt file." << endl;
        return;
    }

    userFile << username << " " << password << endl;
    userFile.close();
    cout << "User successfully registered." << endl;
}

bool login(const string &username, const string &password)
{
    ifstream userFile("users.txt");
    if (!userFile)
    {
        cout << "Unable to open users.txt file." << endl;
        return false;
    }

    string line;
    while (getline(userFile, line))
    {
        size_t found = line.find(username + " " + password);
        if (found != string::npos)
        {
            userFile.close();
            return true; // Login successful
        }
    }

    userFile.close();
    return false; // Login failed
}

void signUp()
{
    string username, password;

    cout << "Enter a new username: ";
    cin >> username;

    if (isUsernameExists(username))
    {
        cout << "Username already exists. Please choose a different username." << endl;
        return;
    }

    cout << "Enter a password: ";
    cin >> password;

    addUser(username, password);
}

void setMaxLiftValues(double lifts[])
{
    string exerciseNames[NUM_EXERCISES] = {"Bench Press", "Squat", "Deadlift"};

    for (int i = 0; i < NUM_EXERCISES; ++i)
    {
        cout << "Enter your maximum lift value for " << exerciseNames[i] << " (in kilograms): ";
        cin >> lifts[i];
    }

    // Write the maximum lift values to max_lifts.txt
    writeMaxLiftValuesToFile(lifts);
}

void writeMaxLiftValuesToFile(const double lifts[])
{
    ofstream outputFile("max_lifts.txt");
    if (outputFile.is_open())
    {
        for (int i = 0; i < NUM_EXERCISES; ++i)
        {
            outputFile << lifts[i] << endl;
        }
        outputFile.close();
    }
    else
    {
        cout << "Unable to open max_lifts.txt. Cannot write maximum lift values." << endl;
    }
}

void readMaxLiftValuesFromFile(double lifts[])
{
    ifstream inputFile("max_lifts.txt");
    if (inputFile.is_open())
    {
        for (int i = 0; i < NUM_EXERCISES; ++i)
        {
            inputFile >> lifts[i];
        }
        inputFile.close();
    }
    else
    {
        cout << "Unable to open max_lifts.txt. Using default values." << endl;
        setMaxLiftValues(lifts);
    }
}

void writeSetsAndRepsToFile(const vector<vector<pair<int, double>>> &allSetsAndReps, const string &fileName)
{
    ofstream outputFile(fileName);
    if (outputFile.is_open())
    {
        string exerciseNames[NUM_EXERCISES] = {"Bench Press", "Squat", "Deadlift"};

        for (int exercise = 0; exercise < NUM_EXERCISES; ++exercise)
        {
            outputFile << "\nRecommended sets and reps for " << exerciseNames[exercise] << ":" << endl;
            for (int set = 0; set < NUM_SETS; ++set)
            {
                outputFile << "Set " << allSetsAndReps[exercise][set].first << ": "
                           << allSetsAndReps[exercise][set].second << " kg" << endl;
            }
        }

        outputFile.close();
        cout << "Generated sets and reps have been written to " << fileName << "." << endl;
    }
    else
    {
        cout << "Unable to open " << fileName << ". Cannot write generated sets and reps." << endl;
    }
}

vector<vector<pair<int, double>>> generateSetsAndReps(const double lifts[])
{
    double percentage;
    vector<vector<pair<int, double>>> allSetsAndReps;

    for (int exercise = 0; exercise < NUM_EXERCISES; ++exercise)
    {
        if (lifts[exercise] >= 100.0)
        {
            percentage = 0.8; // 80% of the max lift
        }
        else if (lifts[exercise] >= 50.0)
        {
            percentage = 0.7; // 70% of the max lift
        }
        else
        {
            percentage = 0.6; // 60% of the max lift
        }

        vector<pair<int, double>> setsAndReps;

        for (int i = 0; i < NUM_SETS; ++i)
        {
            setsAndReps.push_back(make_pair(STARTING_REPS, percentage * lifts[exercise]));
            percentage += 0.05; // Increase the percentage for each set
        }

        allSetsAndReps.push_back(setsAndReps);
    }

    return allSetsAndReps;
}

void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to calculate total caloric intake from macronutrients
double calorieCalculator(double carbs, double proteins, double fats)
{
    // Check if any input is negative
    if (carbs < 0 || proteins < 0 || fats < 0)
    {
        cout << "Please enter non-negative values for macronutrients." << endl;
        return -1;
    }

    // Calculate total calories from each macronutrient
    double carbCalories = carbs * 4;
    double proteinCalories = proteins * 4;
    double fatCalories = fats * 9;

    // Calculate total caloric intake
    double totalCalories = carbCalories + proteinCalories + fatCalories;
    return totalCalories;
}

// Function to calculate the recommended water intake per day
double calculateWaterIntake(double weight)
{
    const double waterPerKg = 30.0; // milliliters
    return waterPerKg * weight;
}

void applicationOne()
{
    // Assume that the user has already logged in or signed up
    double lifts[NUM_EXERCISES];

    // Read the maximum lift values from the file or set them manually
    readMaxLiftValuesFromFile(lifts);

    // Generate sets and reps for Application One (Bench Press)
    vector<vector<pair<int, double>>> allSetsAndReps = generateSetsAndReps(lifts);

    // Write the generated sets and reps to a file for Application One
    writeSetsAndRepsToFile(allSetsAndReps, "application_one_sets_reps.txt");
}

void applicationTwo()
{
    double totalCalories = 0.0;
    char continueInput = 'y';

    while (continueInput == 'y' || continueInput == 'Y')
    {
        double carbohydratesGrams, proteinsGrams, fatsGrams;

        cout << "Enter the number of grams of carbohydrates: ";
        cin >> carbohydratesGrams;

        cout << "Enter the number of grams of proteins: ";
        cin >> proteinsGrams;

        cout << "Enter the number of grams of fats: ";
        cin >> fatsGrams;

        double intakeCalories = calorieCalculator(carbohydratesGrams, proteinsGrams, fatsGrams);

        if (intakeCalories >= 0)
        {
            totalCalories += intakeCalories;
            cout << "Your total caloric intake is: " << totalCalories << " calories." << endl;

            if (totalCalories > 2000)
            {
                cout << "Warning: Your total caloric intake is exceeding 2000 calories." << endl;
            }
        }

        cout << "Do you want to continue (y/n)? ";
        cin >> continueInput;
    }
}

void applicationThree()
{
    double weight;
    cout << "Welcome to the Water Intake Tracker!" << endl;

    cout << "Please enter your weight in kilograms: ";
    cin >> weight;

    clearInputBuffer();

    double recommendedIntake = calculateWaterIntake(weight);
    cout << "Your recommended water intake per day is: " << recommendedIntake << " milliliters" << endl;

    // Tracking water intake
    double totalIntake = 0.0;
    char choice;
    do
    {
        double intake;
        cout << "Enter the amount of water you drank (in milliliters): ";
        cin >> intake;

        totalIntake += intake;

        cout << "Do you want to enter more intake? (Y/N): ";
        cin >> choice;
    } while (toupper(choice) == 'Y');

    int liters = totalIntake / 1000;
    int milliliters = static_cast<int>(totalIntake) % 1000;

    cout << "Total water intake for the day: " << liters << " liters and " << milliliters << " milliliters" << endl;
}

int main()
{
    int choice = 0;
    string username, password;
    double lifts[NUM_EXERCISES];

    do
    {
        printMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        // Clear the cin buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1: // Login
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            if (login(username, password))
            {
                cout << "Login successful!" << endl;
            }
            else
            {
                cout << "Login failed. Invalid username or password." << endl;
            }
            break;

        case 2: // Sign Up
            signUp();
            break;

        case 3: // Exit
            cout << "Exiting the application. Goodbye!" << endl;
            break;

        case 4: // Application One (Weightlifting Sets and Reps)
            setMaxLiftValues(lifts);
            applicationOne();
            break;

        case 5: // Application Two (Calorie Calculator)
            applicationTwo();
            break;

        case 6: // Application Three (Water Intake Tracker)
            applicationThree();
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }

        cout << endl;
    } while (choice != 3);

    return 0;
}
