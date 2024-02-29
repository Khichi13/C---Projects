#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <limits>
#include <ctime>
#include <fstream>
using namespace std;

// Functions for Validation
void wait()
{
    cout << "Press Enter to continue...";
    cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void inputValidation()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    system("clear");
}
bool dateValidationPast(const char* inputDate)
{
    char dayString[3], monthString[3], yearString[5];
    strncpy(dayString, inputDate, 2);
    dayString[2] = '\0';

    strncpy(monthString, inputDate + 3, 2);
    monthString[2] = '\0';

    strncpy(yearString, inputDate + 6, 4);
    yearString[4] = '\0';

    int day = atoi(dayString);
    int month = atoi(monthString);
    int year = atoi(yearString);

    if (month > 12)
        return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        daysInMonth[2] = 29;

    if (day > daysInMonth[month])
        return false;

   time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    if (year < localTime->tm_year + 1900 ||
        (year == localTime->tm_year + 1900 && (month < localTime->tm_mon + 1 ||
            (month == localTime->tm_mon + 1 && day < localTime->tm_mday)))) {
        return false;
    }

    return true;
}
bool dateValidation(const char* inputDate)
{
    char dayString[3], monthString[3], yearString[5];
    strncpy(dayString, inputDate, 2);
    dayString[2] = '\0';

    strncpy(monthString, inputDate + 3, 2);
    monthString[2] = '\0';

    strncpy(yearString, inputDate + 6, 4);
    yearString[4] = '\0';

    int day = atoi(dayString);
    int month = atoi(monthString);
    int year = atoi(yearString);

    if (month > 12)
        return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        daysInMonth[2] = 29;

    if (day > daysInMonth[month])
        return false;

    return true;
}

// Functions for File Handling
template<typename S>
bool saveArrayToFile(const char* filename, S* arrayName, int counter) 
{
    ofstream file(filename, ios::binary | ios::trunc);

    if (!file.is_open()) 
        return false;

    for (int i = 0; i < counter; ++i)
        arrayName[i].saveToStream(file);

    file.close();
    return true;
}
bool saveArraySizeToFile(int& trains, int& classes, int& capacities, int& reservations)
{
    ofstream outFile("data.bin", ios::binary | ios::trunc);

    if (!outFile)
        return false;

    outFile.write(reinterpret_cast<char*>(&trains), sizeof(trains));
    outFile.write(reinterpret_cast<char*>(&classes), sizeof(classes));
    outFile.write(reinterpret_cast<char*>(&capacities), sizeof(capacities));
    outFile.write(reinterpret_cast<char*>(&reservations), sizeof(reservations));

    outFile.close();
    return true;
}
bool saveCounterToFile(int trainCounter, int classCounter, int reservationCounter, int capacityCounter) 
{
    ofstream file("counter.bin", ios::binary | ios::trunc);

    if (!file.is_open()) 
        return false;

    file.write(reinterpret_cast<char*>(&trainCounter), sizeof(int));
    file.write(reinterpret_cast<char*>(&classCounter), sizeof(int));
    file.write(reinterpret_cast<char*>(&reservationCounter), sizeof(int));
    file.write(reinterpret_cast<char*>(&capacityCounter), sizeof(int));

    file.close();
    return true;
}
template<typename L>
bool loadArrayFromFile(const char* filename, L*& arrayName, int& counter) 
{
    ifstream file(filename, ios::binary | ios::ate);

    if (!file.is_open()) 
        return false;

    streampos size = file.tellg();
    file.seekg(0, ios::beg);

    int elements = size / sizeof(L);

    arrayName = new L[elements];

    for (int i = 0; i < elements; ++i)
        arrayName[i].loadFromStream(file);

    counter = elements;

    file.close();
    return true;
}
bool loadArraySizeFromFile(int& trains, int& classes, int& capacities, int& reservations)
{
    ifstream inFile("data.bin", ios::binary);

    if (!inFile)
        return false;

    inFile.read(reinterpret_cast<char*>(&trains), sizeof(trains));
    inFile.read(reinterpret_cast<char*>(&classes), sizeof(classes));
    inFile.read(reinterpret_cast<char*>(&capacities), sizeof(capacities));
    inFile.read(reinterpret_cast<char*>(&reservations), sizeof(reservations));
  
    inFile.close();
    return true;
}
bool loadCounterFromFile(int& trainCounter, int& classCounter, int& reservationCounter, int& capacityCounter) 
{
    ifstream file("counter.bin", ios::binary);

    if (!file.is_open())
        return false;

    file.read(reinterpret_cast<char*>(&trainCounter), sizeof(int));
    file.read(reinterpret_cast<char*>(&classCounter), sizeof(int));
    file.read(reinterpret_cast<char*>(&reservationCounter), sizeof(int));
    file.read(reinterpret_cast<char*>(&capacityCounter), sizeof(int));

    file.close();
    return true;
}

class Train
{
    int trainId;
    char trainName[50];
    char source[30];
    char destination[30];

public:
    // Getter Methods
    int getTrainId() const { return trainId; }
    const char* getTrainName() const { return trainName; }
    const char* getSource() const { return source; }
    const char* getDestination() const { return destination; }

    // Setter Methods
    void setTrainId(int tId)
    {
        trainId = tId;
    }

    void setTrainName(const char* cName)
    {
        strcpy(trainName, cName);
    }

    void setSource(const char* src)
    {
        strcpy(source, src);
    }

    void setDestination(const char* dest)
    {
        strcpy(destination, dest);
    }

    // Insert and Display Interface
    void insert(int id)
    {
        setTrainId(id);

        cout << "Enter train name: ";
        char temp[40];
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(temp, 40);
        setTrainName(temp);

        cout << "Enter source: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(temp, 40);
        setSource(temp);

        cout << "Enter destination: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(temp, 40);
        setDestination(temp);
    }

    void display()
    {
        cout << "\nTrain Id: " << trainId;
        cout << "\nTrain Name: " << trainName;
        cout << "\nSource: " << source;
        cout << "\nDestination: " << destination << endl << endl;
    }

    // Serialization and Deserialization Methods
    void saveToStream(ofstream& stream) 
    {
        stream.write(reinterpret_cast<const char*>(&trainId), sizeof(trainId));
        stream.write(reinterpret_cast<const char*>(&trainName), sizeof(trainName));
        stream.write(reinterpret_cast<const char*>(&source), sizeof(source));
        stream.write(reinterpret_cast<const char*>(&destination), sizeof(destination));
    }

    void loadFromStream(ifstream& stream) 
    {
        stream.read(reinterpret_cast<char*>(&trainId), sizeof(trainId));
        stream.read(reinterpret_cast<char*>(&trainName), sizeof(trainName));
        stream.read(reinterpret_cast<char*>(&source), sizeof(source));
        stream.read(reinterpret_cast<char*>(&destination), sizeof(destination));
    }
};
class TrainClass
{
    int classId;
    char className[30];

public:
    // Getter Methods
    int getClassId() const { return classId; }
    const char* getClassName() const { return className; }

    // Setter Methods
    void setClassId(int cId)
    {
        classId = cId;
    }

    void setClassName(const char* cName)
    {
        strcpy(className, cName);
    }

    // Insert and display functionalities
    void insert(int id)
    {
        setClassId(id);

        cout << "Enter class name: ";
        char temp[40];
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(temp, 40);
        setClassName(temp);
    }

    void display()
    {
        cout << "\nClass Id: " << classId;
        cout << "\nClass Name: " << className << endl << endl;
    }

    // Serialization and Deserialization Methods
    void saveToStream(ofstream& stream)
    {
        stream.write(reinterpret_cast<const char*>(&classId), sizeof(classId));
        stream.write(reinterpret_cast<const char*>(&className), sizeof(className));
    }

    void loadFromStream(ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&classId), sizeof(classId));
        stream.read(reinterpret_cast<char*>(&className), sizeof(className));
    }
};
class TrainCapacity
{
    int trainId;
    int classId;
    int totalSeats;
    int rate;

public:
    // Getter Methods
    int getTrainId() const { return trainId; }
    int getClassId() const { return classId; }
    int getTotalSeats() const { return totalSeats; }
    int getRate() const { return rate; }

    // Setter Methods
    void setTrainId(int tId)
    {
        trainId = tId;
    }

    void setClassId(int cId)
    {
        classId = cId;
    }

    void setTotalSeats(int seats)
    {
        totalSeats = seats;
    }

    void setRate(int price)
    {
        rate = price;
    }

    // Insert and display interface
    void insert(int tid, int cid)
    {
        setTrainId(tid);
        setClassId(cid);

        while (true)
        {
            cout << "Enter total seats: ";
            int seats;
            cin >> seats;

            if (cin.fail() || seats < 1)
            {
                inputValidation();
                cout << "Please enter a positive value.\n\n";
                continue;
            }
            setTotalSeats(seats);
            break;
        }


        while (true)
        {
            cout << "Enter rate: ";
            int rate;
            cin >> rate;

            if (cin.fail() || rate < 1)
            {
                inputValidation();
                cout << "Please enter a positive value.\n\n";
                continue;
            }
            setRate(rate);
            break;
        }
    }

    void display()
    {
        cout << "\nTrain Id: " << trainId;
        cout << "\nClass ID: " << classId;
        cout << "\nTotal Seats: " << totalSeats;
        cout << "\nRate: " << rate << endl << endl;
    }

    // Serialization and Deserialization Methods
    void saveToStream(ofstream& stream)
    {
        stream.write(reinterpret_cast<const char*>(&trainId), sizeof(trainId));
        stream.write(reinterpret_cast<const char*>(&classId), sizeof(classId));
        stream.write(reinterpret_cast<const char*>(&totalSeats), sizeof(totalSeats));
        stream.write(reinterpret_cast<const char*>(&rate), sizeof(rate));
    }

    void loadFromStream(ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&trainId), sizeof(trainId));
        stream.read(reinterpret_cast<char*>(&classId), sizeof(classId));
        stream.read(reinterpret_cast<char*>(&totalSeats), sizeof(totalSeats));
        stream.read(reinterpret_cast<char*>(&rate), sizeof(rate));
    }
};
class Reservation
{
    int trainId;
    int classId;
    int ticketNo;
    char date[11];
    char cnic[15];
    char passengerName[50];

public:
    // Getter Methods
    int getTrainId() const { return trainId; }
    int getClassId() const { return classId; }
    int getTicketNo() const { return ticketNo; }
    const char* getDate() const { return date; }
    const char* getCNIC() const { return cnic; }
    const char* getPassengerName() const { return passengerName; }

    // Setter Methods
    void setTrainId(int tId)
    {
        trainId = tId;
    }

    void setClassId(int cId)
    {
        classId = cId;
    }

    void setTicketNo(int tNo)
    {
        ticketNo = tNo;;
    }

    void setDate(const char* day)
    {
        strcpy(date, day);
    }

    void setCNIC(const char* nic)
    {
        strcpy(cnic, nic);
    }

    void setPassengerName(const char* name)
    {
        strcpy(passengerName, name);
    }

    // Insert and display interface
    void insert(int tid, int cid, int previousTicket, const char* date)
    {
        setTrainId(tid);
        setClassId(cid);
        setTicketNo(previousTicket + 1);
        setDate(date);

        char temp[50];
        cout << "Enter CNIC: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(temp, 50);
        setCNIC(temp);

        cout << "Enter passenger name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(temp, 50);
        setPassengerName(temp);
    }

    void display()
    {
        cout << "Train Id: " << trainId;
        cout << "\nClass ID: " << classId;
        cout << "\nTicket Number: " << ticketNo;
        cout << "\nDate: " << date;
        cout << "\nCNIC: " << cnic;
        cout << "\nPassenger Name: " << passengerName << endl << endl;
    }

    // Serialization and Deserialization Methods
    void saveToStream(ofstream& stream)
    {
        stream.write(reinterpret_cast<const char*>(&trainId), sizeof(trainId));
        stream.write(reinterpret_cast<const char*>(&classId), sizeof(classId));
        stream.write(reinterpret_cast<const char*>(&ticketNo), sizeof(ticketNo));
        stream.write(reinterpret_cast<const char*>(&date), sizeof(date));
        stream.write(reinterpret_cast<const char*>(&cnic), sizeof(cnic));
        stream.write(reinterpret_cast<const char*>(&passengerName), sizeof(passengerName));
    }

    void loadFromStream(ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&trainId), sizeof(trainId));
        stream.read(reinterpret_cast<char*>(&classId), sizeof(classId));
        stream.read(reinterpret_cast<char*>(&ticketNo), sizeof(ticketNo));
        stream.read(reinterpret_cast<char*>(&date), sizeof(date));
        stream.read(reinterpret_cast<char*>(&cnic), sizeof(cnic));
        stream.read(reinterpret_cast<char*>(&passengerName), sizeof(passengerName));
    }
};

int main()
{
    bool validation = false;
    system("clear");
    cout << "+-------------------------------------------------+\n";
    cout << "|      Welcome to Railway Reservation System      |\n";
    cout << "+-------------------------------------------------+\n";
    cout << "|  Please enter the following details for setup   |\n";
    cout << "+-------------------------------------------------+\n\n";

    Train* trains;
    TrainClass* trainClasses;
    TrainCapacity* trainCapacities;
    Reservation* reservations;

    int menuChoice, trainCounter, capacityCounter, classCounter, reservationCounter, noOfTrains, noOfClasses, noOfCapacities, noOfReservations;
    
    // Loading Data
    if (loadArrayFromFile("trains.bin", trains, trainCounter)&&loadArrayFromFile("trainClasses.bin", trainClasses, classCounter)&&loadArrayFromFile("reservations.bin", reservations, reservationCounter)&&loadArrayFromFile("trainCapacities.bin", trainCapacities, capacityCounter)&&loadCounterFromFile(trainCounter, classCounter, reservationCounter, capacityCounter)&&loadArraySizeFromFile(noOfTrains, noOfClasses, noOfCapacities, noOfReservations))
      cout << "Data loaded from saved files.\n\n";
    else
    {
        // Default Initialization
        noOfTrains = 4, noOfClasses = 4;
        noOfCapacities = noOfTrains * noOfClasses;
        noOfReservations = 100;

        trains = new Train[noOfTrains];
        trainClasses = new TrainClass[noOfClasses];
        trainCapacities = new TrainCapacity[noOfCapacities];
        reservations = new Reservation[noOfReservations];

        trainCounter = 0, capacityCounter = 0, classCounter = 0, reservationCounter = 0;
        cout << "Couldn't find saved data. Arrays initialzed with default values.\n\n";
    }

    wait();
    system("clear");

    while (true)
    {
        validation = false;
        // Main Menu
        do
        {
            cout << "+--------------------------------------+\n";
            cout << "|               Main Menu              |\n";
            cout << "+--------------------------------------+\n";
            cout << "| 1. Train                             |\n";
            cout << "| 2. Train Class Menu                  |\n";
            cout << "| 3. Train Capacity Menu               |\n";
            cout << "| 4. Reservation Menu                  |\n";
            cout << "| 5. Save Data                         |\n";
            cout << "| 6. Exit Program                      |\n";
            cout << "+--------------------------------------+\n";
            cout << "Enter your choice: ";
            cin >> menuChoice;
            if (cin.fail() || menuChoice < 1 || menuChoice > 6)
            {
                inputValidation();
                cout << "Please select a number from the list.\n\n";
            }
            else
            {
                validation = true;
                system("clear");
            }
        } while (validation == false);

        // Train Operations
        if (menuChoice == 1)
        {
            system("clear");
            int trainChoice;
            while (true)
            {
                validation = false;
                // Train Menu
                do
                {
                    cout << "+------------------------------+\n";
                    cout << "|          Train Menu          |\n";
                    cout << "+------------------------------+\n";
                    cout << "| 1. Insert record             |\n";
                    cout << "| 2. Update train name         |\n";
                    cout << "| 3. Delete a record           |\n";
                    cout << "| 4. Display a specific record |\n";
                    cout << "| 5. Display all records       |\n";
                    cout << "| 6. Return to main menu       |\n";
                    cout << "+------------------------------+\n";
                    cout << "Enter your choice: ";
                    cin >> trainChoice;
                    if (cin.fail() || trainChoice < 1 || trainChoice > 6)
                    {
                        inputValidation();
                        cout << "Please select a number from the list.\n\n";
                    }
                    else
                    {
                        validation = true;
                        system("clear");
                    }
                } while (validation == false);

                // Insertion
                if (trainChoice == 1)
                {
                    if (trainCounter == noOfTrains)
                    {
                            noOfTrains *= 2;
                            Train* temp = new Train[noOfTrains];
                            for (int i = 0; i < trainCounter; i++)
                                temp[i] = trains[i];
                            delete[] trains;
                            trains = temp;

                            noOfCapacities = noOfTrains * noOfClasses;
                            TrainCapacity* temp2 = new TrainCapacity[noOfCapacities];
                            for (int i = 0; i < capacityCounter; i++)
                                temp2[i] = trainCapacities[i];
                            delete[] trainCapacities;
                            trainCapacities = temp2;
                    }

                    bool unique;
                    int id;
                    validation = false;
                    do
                    {
                        unique = true;
                        cout << "Enter train ID: ";
                        cin >> id;

                        if (id < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }
                        else
                        {
                            validation = true;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == id)
                            {
                                unique = false;
                                system("clear");
                                cout << "Please enter a unique ID.\n\n";
                            }

                    } while (validation == false || unique == false);

                    trains[trainCounter].insert(id);
                    trainCounter++;
                    system("clear");
                    cout << "Record inserted successfully.\n\n";
                    wait();
                    system("clear");
                }

                // Updation
                if (trainChoice == 2)
                {
                    // Searching for record
                    validation = false;
                    while (true)
                    {
                        cout << "Enter train ID for updation: ";
                        int id;
                        cin >> id;

                        if (cin.fail() || id < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == id)
                            {
                                validation = true;

                                // Updation Mechanism
                                system("clear");
                                cout << "Enter new train name: ";
                                char temp[40];
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cin.get(temp, 40);
                                trains[i].setTrainName(temp);
                            }

                        if (validation == true)
                        {
                            system("clear");
                            cout << "Record updated successfully.\n\n";
                            wait();
                            system("clear");
                            break;
                        }
                        else
                        {
                            system("clear");
                            cout << "Record not found.\n\n";
                            wait();
                            system("clear");
                            break;
                        }
                    }
                }

                // Deletion
                if (trainChoice == 3)
                {
                    // Searching for record
                    validation = false;
                    while (true)
                    {
                        cout << "Enter train ID for deletion: ";
                        int id;
                        cin >> id;

                        if (cin.fail() || id < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        for (int i = 0; i < capacityCounter; i++)
                            if (trainCapacities[i].getTrainId() == id)
                            {
                                validation = true;
                                break;
                            }

                        if (validation == true)
                        {
                            system("clear");
                            cout << "Record is present in train capacity class.\n\n";
                            break;
                        }
                        // Deletion Mechanism
                        else
                        {
                            bool found = false;
                            for (int i = 0; i < trainCounter; i++)
                            {
                                if (trains[i].getTrainId() == id)
                                {
                                    found = true;
                                    for (i; i < trainCounter - 1; i++)
                                        trains[i] = trains[i + 1];

                                    cout << "Record deleted.\n\n";
                                    wait();
                                    system("clear");
                                    trainCounter--;
                                    break;
                                }
                            }
                            if (!found)
                            {
                                cout << "Record not found.\n\n";
                                wait();
                                system("clear");
                            }
                            break;
                        }
                    }
                }

                // Displaying specific record
                if (trainChoice == 4)
                {
                    validation = false;
                    while (true)
                    {
                        cout << "Enter train ID: ";
                        int id;
                        cin >> id;

                        if (cin.fail() || id < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == id)
                            {
                                trains[i].display();
                                validation = true;
                            }

                        if (validation == false)
                            cout << "\nRecord not found.\n\n";

                        wait();
                        system("clear");
                        break;
                    }
                }

                // Displaying all records
                if (trainChoice == 5)
                {
                    for (int i = 0; i < trainCounter; i++)
                        trains[i].display();

                    if (trainCounter == 0)
                        cout << "No records found.\n\n";

                    wait();
                    system("clear");
                }

                // Exiting to main menu
                if (trainChoice == 6)
                    break;
            }
        }

        // Train Class Operations
        if (menuChoice == 2)
        {
            system("clear");
            int classChoice;
            while (true)
            {
                validation = false;
                // Train Class Menu
                do
                {
                    cout << "+--------------------------------+\n";
                    cout << "|        Train Class Menu        |\n";
                    cout << "+--------------------------------+\n";
                    cout << "| 1. Insert record               |\n";
                    cout << "| 2. Update class name           |\n";
                    cout << "| 3. Delete a record             |\n";
                    cout << "| 4. Display a specific record   |\n";
                    cout << "| 5. Display all records         |\n";
                    cout << "| 6. Return to main menu         |\n";
                    cout << "+--------------------------------+\n";
                    cout << "Enter your choice: ";
                    cin >> classChoice;
                    if (cin.fail() || classChoice < 1 || classChoice > 6)
                    {
                        inputValidation();
                        cout << "Please select a number from the list.\n\n";
                    }
                    else
                    {
                        validation = true;
                        system("clear");
                    }
                } while (validation == false);

                // Insertion
                if (classChoice == 1)
                {
                    if (classCounter == noOfClasses)
                    {
                        noOfClasses *= 2;
                        TrainClass* temp = new TrainClass[noOfClasses];
                        for (int i = 0; i < classCounter; i++)
                            temp[i] = trainClasses[i];
                        delete[] trainClasses;
                        trainClasses = temp;

                        noOfCapacities = noOfTrains * noOfClasses;
                        TrainCapacity* temp2 = new TrainCapacity[noOfCapacities];
                        for (int i = 0; i < capacityCounter; i++)
                            temp2[i] = trainCapacities[i];
                        delete[] trainCapacities;
                        trainCapacities = temp2;
                    }

                    bool unique;
                    int id;
                    validation = false;
                    do
                    {
                        unique = true;
                        cout << "Enter class ID: ";
                        cin >> id;

                        if (id < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }
                        else
                        {
                            validation = true;
                        }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == id)
                            {
                                unique = false;
                                system("clear");
                                cout << "Please enter a unique ID.\n\n";
                            }

                    } while (validation == false || unique == false);

                    trainClasses[classCounter].insert(id);
                    classCounter++;
                    system("clear");
                    cout << "Record inserted successfully.\n\n";
                    wait();
                    system("clear");
                }

                // Updation
                if (classChoice == 2)
                {
                    // Searching for record
                    validation = false;
                    while (true)
                    {
                        cout << "Enter class ID for updation: ";
                        int id;
                        cin >> id;

                        if (cin.fail() || id < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == id)
                            {
                                validation = true;

                                // Updation Mechanism
                                system("clear");
                                cout << "Enter new class name: ";
                                char temp[40];
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cin.get(temp, 40);
                                trainClasses[i].setClassName(temp);
                            }

                        if (validation == true)
                        {
                            system("clear");
                            cout << "Record updated successfully.\n\n";
                            wait();
                            system("clear");
                            break;
                        }
                        else
                        {
                            system("clear");
                            cout << "Record not found.\n\n";
                            wait();
                            system("clear");
                            break;
                        }
                    }
                }

                // Deletion
                if (classChoice == 3)
                {
                    // Searching for record
                    validation = false;
                    while (true)
                    {
                        cout << "Enter class ID for deletion: ";
                        int id;
                        cin >> id;

                        if (cin.fail() || id < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        for (int i = 0; i < capacityCounter; i++)
                            if (trainCapacities[i].getClassId() == id)
                            {
                                validation = true;
                                break;
                            }

                        if (validation == true)
                        {
                            system("clear");
                            cout << "Record is present in train capacity class.\n\n";
                            break;
                        }
                        // Deletion Mechanism
                        else
                        {
                            bool found = false;
                            for (int i = 0; i < classCounter; i++)
                            {
                                if (trainClasses[i].getClassId() == id)
                                {
                                    found = true;
                                    for (i; i < classCounter - 1; i++)
                                        trainClasses[i] = trainClasses[i + 1];

                                    cout << "Record deleted.\n\n";
                                    wait();
                                    system("clear");
                                    classCounter--;
                                    break;
                                }
                            }
                            if (!found)
                            {
                                cout << "Record not found.\n\n";
                                wait();
                                system("clear");
                            }
                            break;
                        }
                    }
                }

                // Displaying specific record
                if (classChoice == 4)
                {
                    validation = false;
                    while (true)
                    {
                        cout << "Enter class ID: ";
                        int id;
                        cin >> id;

                        if (cin.fail() || id < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == id)
                            {
                                trainClasses[i].display();
                                validation = true;
                            }

                        if (validation == false)
                            cout << "\nRecord not found.\n\n";

                        wait();
                        system("clear");
                        break;
                    }
                }

                // Displaying all records
                if (classChoice == 5)
                {
                    for (int i = 0; i < classCounter; i++)
                        trainClasses[i].display();

                    if (classCounter == 0)
                        cout << "No records found.\n\n";

                    wait();
                    system("clear");
                }

                // Exiting to main menu
                if (classChoice == 6)
                    break;
            }
        }

        // Train Capacity Operations
        if (menuChoice == 3)
        {
            system("clear");
            int capacityChoice;
            while (true)
            {
                validation = false;
                // Train Capacity Menu
                do
                {
                    cout << "+-------------------------------+\n";
                    cout << "|      Train Capacity Menu      |\n";
                    cout << "+-------------------------------+\n";
                    cout << "| 1. Insert record              |\n";
                    cout << "| 2. Update attributes          |\n";
                    cout << "| 3. Delete a record            |\n";
                    cout << "| 4. Display a specific record  |\n";
                    cout << "| 5. Display all records        |\n";
                    cout << "| 6. Return to main menu        |\n";
                    cout << "+-------------------------------+\n";
                    cout << "Enter your choice: ";
                    cin >> capacityChoice;
                    if (cin.fail() || capacityChoice < 1 || capacityChoice > 6)
                    {
                        inputValidation();
                        cout << "Please select a number from the list.\n\n";
                    }
                    else
                    {
                        validation = true;
                        system("clear");
                    }
                } while (validation == false);

                // Insertion
                if (capacityChoice == 1)
                {
                    if (capacityCounter == noOfCapacities)
                    {
                        system("clear");
                        cout << "Data has been inserted for all trains and classes. Please create another train or class.\n\n";
                        continue;
                    }

                    bool trainExists, classExists;
                    int tid, cid;
                    trainExists = false;
                    classExists = false;

                    while (true)
                    {
                        cout << "Enter train ID: ";
                        cin >> tid;

                        if (tid < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID: ";
                            cin >> cid;

                            if (cid < 1 || cin.fail())
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == tid)
                            {
                                trainExists = true;
                                break;
                            }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == cid)
                            {
                                classExists = true;
                                break;
                            }

                        if (!trainExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n\n";
                            break;
                        }

                        if (!classExists)
                        {
                            system("clear");
                            cout << "Class doesn't exist.\n\n";
                            break;
                        }

                        for (int i = 0; i < capacityCounter; i++)
                        {
                            if (trainCapacities[i].getTrainId() == tid && trainCapacities[i].getClassId() == cid)
                            {
                                trainExists = false;
                                classExists = false;
                                system("clear");
                                cout << "Please enter unique train and class IDs.\n\n";
                                break;
                            }
                        }
                        break;
                    }

                    if (trainExists == true && classExists == true)
                    {
                        trainCapacities[capacityCounter].insert(tid, cid);
                        capacityCounter++;
                        system("clear");
                        cout << "Record inserted successfully.\n\n";
                    }
                    wait();
                    system("clear");
                }

                // Updation
                if (capacityChoice == 2)
                {
                    // Searching for record
                    validation = false;
                    while (true)
                    {
                        int tid, cid;
                        cout << "Enter train ID for updation: ";
                        cin >> tid;

                        if (cin.fail() || tid < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "\nEnter class ID for updation: ";
                            cin >> cid;

                            if (cin.fail() || cid < 1)
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < capacityCounter; i++)
                            if (trainCapacities[i].getClassId() == cid && trainCapacities[i].getTrainId() == tid)
                            {
                                validation = true;

                                // Updation Mechanism
                                system("clear");
                                while (true)
                                {
                                    cout << "Enter new total seats: ";
                                    int seats;
                                    cin >> seats;
                                    system("clear");

                                    if (cin.fail() || seats < 1)
                                    {
                                        inputValidation();
                                        cout << "Please enter a positive value.\n\n";
                                        continue;
                                    }
                                    trainCapacities[i].setTotalSeats(seats);
                                    break;
                                }

                                while (true)
                                {
                                    cout << "Enter new rate: ";
                                    int rate;
                                    cin >> rate;
                                    system("clear");

                                    if (cin.fail() || rate < 1)
                                    {
                                        inputValidation();
                                        cout << "Please enter a positive value.\n\n";
                                        continue;
                                    }
                                    trainCapacities[i].setRate(rate);
                                    break;
                                }
                                break;
                            }

                        if (validation == true)
                        {
                            system("clear");
                            cout << "Record updated successfully.\n\n";
                            wait();
                            system("clear");
                            break;
                        }
                        else
                        {
                            system("clear");
                            cout << "Record not found.\n\n";
                            wait();
                            system("clear");
                            break;
                        }
                    }
                }

                // Deletion
                if (capacityChoice == 3)
                {
                    // Searching for record
                    validation = false;
                    while (true)
                    {
                        int tid, cid;
                        cout << "Enter train ID for deletion: ";
                        cin >> tid;

                        if (cin.fail() || tid < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID for deletion: ";
                            cin >> cid;

                            if (cin.fail() || cid < 1)
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < reservationCounter; i++)
                            if (reservations[i].getClassId() == cid && reservations[i].getTrainId() == tid)
                            {
                                validation = true;
                                break;
                            }

                        if (validation == true)
                        {
                            system("clear");
                            cout << "Record is present in reservation class.\n\n";
                            break;
                        }
                        // Deletion Mechanism
                        else
                        {
                            bool found = false;
                            for (int i = 0; i < capacityCounter; i++)
                            {
                                if (trainCapacities[i].getClassId() == cid && trainCapacities[i].getTrainId() == tid)
                                {
                                    found = true;
                                    for (i; i < capacityCounter - 1; i++)
                                        trainCapacities[i] = trainCapacities[i + 1];

                                    system("clear");
                                    cout << "Record deleted.\n\n";
                                    wait();
                                    system("clear");
                                    capacityCounter--;
                                    break;
                                }
                            }
                            if (!found)
                            {
                                cout << "Record not found.\n\n";
                                wait();
                                system("clear");
                            }
                            break;
                        }
                    }
                }

                // Displaying specific record
                if (capacityChoice == 4)
                {
                    validation = false;
                    while (true)
                    {
                        int tid, cid;
                        cout << "Enter train ID: ";
                        cin >> tid;

                        if (cin.fail() || tid < 1)
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID: ";
                            cin >> cid;

                            if (cin.fail() || cid < 1)
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < capacityCounter; i++)
                            if (trainCapacities[i].getClassId() == cid && trainCapacities[i].getTrainId() == tid)
                            {
                                trainCapacities[i].display();
                                validation = true;
                                break;
                            }

                        if (validation == false)
                            cout << "\nRecord not found.\n\n";

                        wait();
                        system("clear");
                        break;
                    }
                }

                // Displaying all records
                if (capacityChoice == 5)
                {
                    for (int i = 0; i < capacityCounter; i++)
                        trainCapacities[i].display();

                    if (capacityCounter == 0)
                        cout << "No records found.\n\n";

                    wait();
                    system("clear");
                }

                // Exiting to main menu
                if (capacityChoice == 6)
                    break;
            }
        }

        // Reservation Operations
        if (menuChoice == 4)
        {
            int reservationChoice;
            while (true)
            {
                validation = false;
                // Reservation Menu
                do
                {
                    system("clear");
                    cout << "+----------------------------------------+\n";
                    cout << "|            Reservation Menu            |\n";
                    cout << "+----------------------------------------+\n";
                    cout << "| 1. Insert record                       |\n";
                    cout << "| 2. Update attributes                   |\n";
                    cout << "| 3. Delete a specific record            |\n";
                    cout << "| 4. Display records for specific date   |\n";
                    cout << "| 5. Display all records                 |\n";
                    cout << "| 6. Display available seats             |\n";
                    cout << "| 7. Check booking                       |\n";
                    cout << "| 8. Return to main menu                 |\n";
                    cout << "+----------------------------------------+\n";
                    cout << "Enter your choice: ";
                    cin >> reservationChoice;
                    if (cin.fail() || reservationChoice < 1 || reservationChoice > 8)
                    {
                        inputValidation();
                        cout << "Please select a number from the list.\n\n";
                    }
                    else
                    {
                        validation = true;
                        system("clear");
                    }
                } while (validation == false);

                // Insertion
                if (reservationChoice == 1)
                {
                    bool trainExists, classExists, ticketValidation;
                    int tid, cid;
                    trainExists = false;
                    classExists = false;

                    while (true)
                    {
                        cout << "Enter train ID: ";
                        cin >> tid;

                        if (tid < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID: ";
                            cin >> cid;

                            if (cid < 1 || cin.fail())
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == tid)
                            {
                                trainExists = true;
                                break;
                            }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == cid)
                            {
                                classExists = true;
                                break;
                            }

                        if (!trainExists && !classExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n";
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!classExists)
                        {
                            system("clear");
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!trainExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        break;
                    }

                    if (trainExists == true && classExists == true)
                    {
                        int ticket = 0, ticketNo = 0;
                        char date[50];
                        do
                        {
                            cout << "Enter date in given format(dd/mm/yyyy): ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get(date, 50);
                        } while (!dateValidationPast(date));

                        for (int i = 0; i < reservationCounter; i++)
                        {
                            if (reservations[i].getTrainId() == tid && reservations[i].getClassId() == cid)
                            {   if (strcmp(reservations[i].getDate(), date) == 0)
                                {    
                                    ticket++;
                                    ticketNo = reservations[i].getTicketNo();
                                }
                            }
                        }

                        for (int i = 0; i < capacityCounter; i++)
                        {
                            if (trainCapacities[i].getTrainId() == tid && trainCapacities[i].getClassId() == cid && trainCapacities[i].getTotalSeats() == ticket)
                            {
                                system("clear");
                                cout << "There are no available seats.\n\n";
                                wait();
                                break;
                            }
                            else
                            {
                                if (reservationCounter == noOfReservations)
                                {
                                    noOfReservations *= 2;
                                    Reservation* temp = new Reservation[noOfReservations];
                                    for (int i = 0; i < reservationCounter; i++)
                                        temp[i] = reservations[i];
                                    delete[] reservations;
                                    reservations = temp;
                                }
                                reservations[reservationCounter].insert(tid, cid, ticketNo, date);
                                reservationCounter++;
                                system("clear");
                                cout << "Your ticket number is: " << ticketNo + 1;
                                cout << "\n\nRecord inserted successfully.\n\n";
                                wait();
                                break;
                            }
                        }

                    }
                    system("clear");
                }

                // Updation
                if (reservationChoice == 2)
                {
                    // Entering details
                    bool trainExists, classExists;
                    int tid, cid, ticket;
                    trainExists = false;
                    classExists = false;
                    validation = false;

                    while (true)
                    {
                        cout << "Enter train ID for updation: ";
                        cin >> tid;

                        if (tid < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID for updation: ";
                            cin >> cid;

                            if (cid < 1 || cin.fail())
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == tid)
                            {
                                trainExists = true;
                                break;
                            }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == cid)
                            {
                                classExists = true;
                                break;
                            }

                        if (!trainExists && !classExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n";
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!classExists)
                        {
                            system("clear");
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!trainExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else
                            validation = true;
                        break;

                    }

                    while (validation == true)
                    {
                        cout << "Enter ticket number for updation: ";
                        cin >> ticket;

                        if (ticket < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }
                        break;
                    }
                    validation = false;

                    if (trainExists == true && classExists == true)
                    {
                        char date[50];
                        do
                        {
                            cout << "Enter date in given format(dd/mm/yyyy): ";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get(date, 50);
                        } while (!dateValidationPast(date));

                        // Searching for record
                        for (int i = 0; i < reservationCounter; i++)
                        {
                            if (reservations[i].getTrainId() == tid && reservations[i].getClassId() == cid)
                            {
                                if (strcmp(reservations[i].getDate(), date) == 0 && reservations[i].getTicketNo() == ticket)
                                {
                                    // Updation mechanism
                                    system("clear");
                                    cout << "Please enter new CNIC: ";
                                    char temp[50];
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cin.get(temp, 50);
                                    reservations[i].setCNIC(temp);
                                    cout << "\nPlease enter new passenger name: ";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cin.get(temp, 50);
                                    reservations[i].setPassengerName(temp);
                                    validation = true;
                                    break;
                                }
                            }
                        }

                        if (validation == true)
                        {
                            system("clear");
                            cout << "Record updated successfully.\n\n";
                            wait();
                            system("clear");
                        }
                        else
                        {
                            system("clear");
                            cout << "Record not found.\n\n";
                            wait();
                            system("clear");
                        }
                    }
                }

                // Deletion
                if (reservationChoice == 3)
                {
                    // Entering details
                    bool trainExists, classExists;
                    int tid, cid, ticket;
                    trainExists = false;
                    classExists = false;
                    validation = false;

                    while (true)
                    {
                        cout << "Enter train ID for deletion: ";
                        cin >> tid;

                        if (tid < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID for deletion: ";
                            cin >> cid;

                            if (cid < 1 || cin.fail())
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == tid)
                            {
                                trainExists = true;
                                break;
                            }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == cid)
                            {
                                classExists = true;
                                break;
                            }

                        if (!trainExists && !classExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n";
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!classExists)
                        {
                            system("clear");
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!trainExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else
                            validation = true;
                        break;

                    }

                    while (validation == true)
                    {
                        cout << "Enter ticket number for deletion: ";
                        cin >> ticket;

                        if (ticket < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }
                        break;
                    }
                    validation = false;

                    if (trainExists == true && classExists == true)
                    {
                        int index;
                        char date[50];
                        do
                        {
                            cout << "Enter date in given format(dd/mm/yyyy): ";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get(date, 50);
                        } while (!dateValidation(date));

                        // Searching for record
                        for (int i = 0; i < reservationCounter; i++)
                            if (reservations[i].getTrainId() == tid && reservations[i].getClassId() == cid)
                                if (strcmp(reservations[i].getDate(), date) == 0 && reservations[i].getTicketNo() == ticket)
                                {
                                    index = i;
                                    validation = true;
                                }

                        if (validation == true)
                        {
                            // Deletion mechanism
                            for (index; index < reservationCounter - 1; index++)
                                reservations[index] = reservations[index + 1];
                            reservationCounter--;
                            system("clear");
                            cout << "Record deleted successfully.\n\n";
                            wait();
                            system("clear");
                        }
                        else
                        {
                            system("clear");
                            cout << "Record not found.\n\n";
                            wait();
                            system("clear");
                        }
                    }
                }

                // Displaying records for specific date
                if (reservationChoice == 4)
                {
                    // Entering details
                    bool trainExists, classExists;
                    int tid, cid;
                    trainExists = false;
                    classExists = false;
                    validation = false;

                    while (true)
                    {
                        cout << "Enter train ID: ";
                        cin >> tid;

                        if (tid < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID: ";
                            cin >> cid;

                            if (cid < 1 || cin.fail())
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == tid)
                            {
                                trainExists = true;
                                break;
                            }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == cid)
                            {
                                classExists = true;
                                break;
                            }

                        if (!trainExists && !classExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n";
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!classExists)
                        {
                            system("clear");
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!trainExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        break;
                    }

                    if (trainExists == true && classExists == true)
                    {
                        char date[50];
                        do
                        {
                            cout << "Enter date in given format(dd/mm/yyyy): ";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get(date, 50);
                        } while (!dateValidation(date));

                        system("clear");
                        // Searching for record
                        for (int i = 0; i < reservationCounter; i++)
                            if (reservations[i].getTrainId() == tid && reservations[i].getClassId() == cid && strcmp(reservations[i].getDate(), date) == 0)
                            {
                                reservations[i].display();
                                validation = true;
                            }

                        if (!validation)
                        {
                            system("clear");
                            cout << "No record found.\n\n";
                            wait();
                            system("clear");
                        }
                        else
                        {
                            wait();
                            system("clear");
                        }
                    }
                }

                // Displaying all records
                if (reservationChoice == 5)
                {
                    for (int i = 0; i < reservationCounter; i++)
                        reservations[i].display();

                    if (reservationCounter == 0)
                        cout << "No records found.\n\n";

                    wait();
                    system("clear");
                }

                // Displaying available seats
                if (reservationChoice == 6)
                {
                    // Entering details
                    bool trainExists, classExists;
                    int tid, cid;
                    trainExists = false;
                    classExists = false;
                    validation = false;

                    while (true)
                    {
                        cout << "Enter train ID: ";
                        cin >> tid;

                        if (tid < 1 || cin.fail())
                        {
                            inputValidation();
                            cout << "Please enter a positive integer.\n\n";
                            continue;
                        }

                        while (true)
                        {
                            cout << "Enter class ID: ";
                            cin >> cid;

                            if (cid < 1 || cin.fail())
                            {
                                inputValidation();
                                cout << "Please enter a positive integer.\n\n";
                                continue;
                            }
                            break;
                        }

                        for (int i = 0; i < trainCounter; i++)
                            if (trains[i].getTrainId() == tid)
                            {
                                trainExists = true;
                                break;
                            }

                        for (int i = 0; i < classCounter; i++)
                            if (trainClasses[i].getClassId() == cid)
                            {
                                classExists = true;
                                break;
                            }

                        if (!trainExists && !classExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n";
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!classExists)
                        {
                            system("clear");
                            cout << "Class doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        else if (!trainExists)
                        {
                            system("clear");
                            cout << "Train doesn't exist.\n\n";
                            wait();
                            break;
                        }
                        break;
                    }

                    if (trainExists == true && classExists == true)
                    {
                        int ticket = 0;
                        char date[50];
                        do
                        {
                            cout << "Enter date in given format(dd/mm/yyyy): ";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get(date, 50);
                        } while (!dateValidation(date));

                        // Searching for latest ticket
                        for (int i = 0; i < reservationCounter; i++)
                            if (reservations[i].getTrainId() == tid && reservations[i].getClassId() == cid && strcmp(reservations[i].getDate(), date) == 0)
                                ticket++;

                        system("clear");
                        cout << "Remaining tickets: ";
                        for (int i = 0; i < capacityCounter; i++)
                            if (trainCapacities[i].getTrainId() == tid && trainCapacities[i].getClassId() == cid)
                                cout << trainCapacities[i].getTotalSeats() - ticket << endl << endl;
                        wait();
                        system("clear");
                    }
                }

                // Checking specific booking
                if (reservationChoice == 7)
                {
                    validation = false;
                    system("clear");
                    char temp[20];
                    cout << "Enter CNIC: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get(temp, 20);
                    system("clear");

                    for (int i = 0; i < reservationCounter; i++)
                        if (strcmp(reservations[i].getCNIC(), temp) == 0)
                        {
                            reservations[i].display();
                            validation = true;
                        }

                    if (!validation)
                    {
                        system("clear");
                        cout << "No record found.\n\n";
                        wait();
                        system("clear");
                    }
                    else
                    {
                        wait();
                        system("clear");
                    }
                }

                // Exiting to main menu
                if (reservationChoice == 8)
                    break;
            }
        }

        // Data Saving
        if (menuChoice == 5)
        {
            if (saveArrayToFile("trains.bin", trains, sizeof(Train) * trainCounter)&&
                saveArrayToFile("trainClasses.bin", trainClasses, sizeof(TrainClass) * classCounter)&&
                saveArrayToFile("reservations.bin", reservations, sizeof(Reservation) * reservationCounter)&&
                saveArrayToFile("trainCapacities.bin", trainCapacities, sizeof(TrainCapacity) * capacityCounter)&&
                saveCounterToFile(trainCounter, classCounter, reservationCounter, capacityCounter)&&
                saveArraySizeToFile(noOfTrains, noOfClasses, noOfCapacities, noOfReservations))
                cout << "Data saved successfully\n\n";
            else
                cout << "Couldn't save data.\n\n";
        }

        // Deallocation and Termination
        if (menuChoice == 6)
        {
            delete[] trains;
            delete[] trainCapacities;
            delete[] trainClasses;
            delete[] reservations;
            return 0;
        }
    }
}