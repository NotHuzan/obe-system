#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Forward declarations
class Program;
class PLO;
class Course;
class CLO;
class Topic;
class Evaluation;

// Function declarations
void saveData(const vector<Program *> &programs);
void loadData(vector<Program *> &programs);
bool isCLOtestedTwice(vector<Evaluation *> evaluations, CLO *clo);

class Person
{
protected:
    string name;

public:
    Person(string n) : name(n) {}
    string getName() const { return name; }
};

class AcademicOfficer : public Person
{
public:
    AcademicOfficer(string n) : Person(n) {}
    void addProgram(vector<Program *> &programs);
    void addPLO(Program *program);
    void addCourse(PLO *plo);
    void addCLO(Course *course);
    void addTopic(CLO *clo);
};

class Teacher : public Person
{
private:
    vector<Evaluation *> evaluations;

public:
    Teacher(string n) : Person(n) {}
    void evaluateCLO(Evaluation *evaluation, CLO *clo, Course *course);
    vector<Evaluation *> getEvaluations() const { return evaluations; }
};

class Program
{
    string name;
    vector<PLO *> plos;

public:
    Program(string n) : name(n) {}
    string getName() const { return name; }
    void addPLO(PLO *plo) { plos.push_back(plo); }
    vector<PLO *> getPLOs() const { return plos; }
};

class PLO
{
    string id;
    vector<Course *> courses;

public:
    PLO(string i) : id(i) {}
    string getId() const { return id; }
    void addCourse(Course *course) { courses.push_back(course); }
    vector<Course *> getCourses() const { return courses; }
};

class Course
{
    string code;
    vector<CLO *> clos;
    vector<CLO *> courseTestedCLOs;

public:
    Course(string c) : code(c) {}
    string getCode() const { return code; }
    void addCLO(CLO *clo) { clos.push_back(clo); }
    vector<CLO *> getCLOs() const { return clos; }
    void addTestedCLO(CLO *clo) { courseTestedCLOs.push_back(clo); }
    vector<CLO *> getCourseTestedCLOs() const { return courseTestedCLOs; }
    CLO *findCLOByID(const string &cloID) const;
};

class CLO
{
    string id;
    vector<Topic *> topics;

public:
    CLO(string i) : id(i) {}
    string getId() const { return id; }
    void addTopic(Topic *topic) { topics.push_back(topic); }
    vector<Topic *> getTopics() const { return topics; }
};

class Topic
{
    string name;

public:
    Topic(string n) : name(n) {}
    string getName() const { return name; }
};

class Evaluation
{
protected:
    vector<CLO *> testedCLOs;

public:
    void addTestedCLO(CLO *clo) { testedCLOs.push_back(clo); }
    vector<CLO *> getTestedCLOs() const { return testedCLOs; }
};

class Assignment : public Evaluation
{
};
class Quiz : public Evaluation
{
};
class MidTerm : public Evaluation
{
};
class Final : public Evaluation
{
};

void AcademicOfficer::addProgram(vector<Program *> &programs)
{
    string name;
    cout << "Enter program name: ";
    cin >> name;
    programs.push_back(new Program(name));
}

void AcademicOfficer::addPLO(Program *program)
{
    string id;
    cout << "Enter PLO id: ";
    cin >> id;
    program->addPLO(new PLO(id));
}

void AcademicOfficer::addCourse(PLO *plo)
{
    string code;
    cout << "Enter course code: ";
    cin >> code;
    plo->addCourse(new Course(code));
}

void AcademicOfficer::addCLO(Course *course)
{
    string id;
    cout << "Enter CLO id: ";
    cin >> id;
    course->addCLO(new CLO(id));
}

void AcademicOfficer::addTopic(CLO *clo)
{
    string topic;
    cout << "Enter topic: ";
    cin >> topic;
    clo->addTopic(new Topic(topic));
}

// Method to find a CLO by its ID
CLO *Course::findCLOByID(const string &cloID) const
{
    for (CLO *clo : clos)
    {
        if (clo->getId() == cloID)
        {
            return clo;
        }
    }
    return nullptr; // CLO with given ID not found
}

void Teacher::evaluateCLO(Evaluation *evaluation, CLO *clo, Course *course)
{
    evaluation->addTestedCLO(clo);
    evaluations.push_back(evaluation);

    // Check if the CLO is tested twice
    if (isCLOtestedTwice(evaluations, clo))
    {
        // Add the CLO to the courseTestedCLOs vector of the corresponding course
        course->addTestedCLO(clo); // Assuming there's a method to add the CLO to the courseTestedCLOs vector
    }
}

// OBE System functions
bool isCLOtestedTwice(vector<Evaluation *> evaluations, CLO *clo)
{
    int count = 0;
    for (Evaluation *evaluation : evaluations)
    {
        vector<CLO *> testedCLOs = evaluation->getTestedCLOs();
        if (find(testedCLOs.begin(), testedCLOs.end(), clo) != testedCLOs.end())
        {
            count++;
            if (count >= 2)
                return true;
        }
    }
    return false;
}

bool areAllCLOsTested(vector<Evaluation *> evaluations, Course *course)
{
    // Get all CLOs associated with the course
    vector<CLO *> allCLOs = course->getCLOs();

    // Get CLOs that have been tested twice for the course
    vector<CLO *> testedCLOs = course->getCourseTestedCLOs();

    // Check if all CLOs associated with the course are present in testedCLOs
    for (CLO *clo : allCLOs)
    {
        if (find(testedCLOs.begin(), testedCLOs.end(), clo) == testedCLOs.end())
        {
            return false; // If any CLO is not in the courseTestedCLOs, return false
        }
    }
    return true; // If all CLOs are tested twice, return true
}

vector<Course *> getCoursesForPLO(Program *program, string ploId)
{
    vector<Course *> relevantCourses;
    vector<PLO *> plos = program->getPLOs();
    for (PLO *plo : plos)
    {
        if (plo->getId() == ploId)
        {
            relevantCourses = plo->getCourses();
            break;
        }
    }
    return relevantCourses;
}

void saveData(const vector<Program *> &programs, const string &filename)
{
    ofstream file(filename);

    if (file.is_open())
    {
        for (Program *program : programs)
        {
            file << "Program:" << program->getName() << endl;
            vector<PLO *> plos = program->getPLOs();
            for (PLO *plo : plos)
            {
                file << "PLO:" << plo->getId() << endl;
                vector<Course *> courses = plo->getCourses();
                for (Course *course : courses)
                {
                    file << "Course:" << course->getCode() << endl;
                    // Save CLOs
                    vector<CLO *> clos = course->getCLOs();
                    for (CLO *clo : clos)
                    {
                        file << "CLO:" << clo->getId() << endl;
                        // Save topics
                        vector<Topic *> topics = clo->getTopics();
                        for (Topic *topic : topics)
                        {
                            file << "Topic:" << topic->getName() << endl;
                        }
                    }
                    // Save tested CLOs
                    vector<CLO *> testedCLOs = course->getCourseTestedCLOs();
                    for (CLO *testedCLO : testedCLOs)
                    {
                        file << "Tested CLO:" << testedCLO->getId() << endl;
                    }
                }
            }
        }
        file.close();
        cout << "Program information saved to " << filename << endl;
    }
    else
    {
        cout << "Unable to open file " << filename << endl;
    }
}

// Function to load program information from a text file
void loadData(vector<Program *> &programs, const string &filename)
{
    ifstream file(filename);
    string line;
    Program *currentProgram = nullptr;
    PLO *currentPLO = nullptr;
    Course *currentCourse = nullptr;
    CLO *currentCLO = nullptr;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            string type, value;
            stringstream ss(line);
            getline(ss, type, ':');
            getline(ss, value);

            if (type == "Program")
            {
                currentProgram = new Program(value);
                programs.push_back(currentProgram);
            }
            else if (type == "PLO")
            {
                currentPLO = new PLO(value);
                currentProgram->addPLO(currentPLO);
            }
            else if (type == "Course")
            {
                currentCourse = new Course(value);
                currentPLO->addCourse(currentCourse);
            }
            else if (type == "CLO")
            {
                currentCLO = new CLO(value);
                currentCourse->addCLO(currentCLO);
            }
            else if (type == "Topic")
            {
                Topic *topic = new Topic(value);
                currentCLO->addTopic(topic);
            }
            else if (type == "Tested CLO")
            {
                // Find the corresponding CLO and add it to the course's tested CLOs
                if (currentCourse != nullptr && currentCLO != nullptr)
                {
                    // Find CLO by ID
                    CLO *testedCLO = currentCourse->findCLOByID(value);
                    if (testedCLO != nullptr)
                    {
                        currentCourse->addTestedCLO(testedCLO);
                    }
                    // cout << "Tested CLOs for Course " << currentCourse->getCode() << ":" << endl;
                    // for (CLO *clo : currentCourse->getCourseTestedCLOs())
                    // {
                    //     cout << clo->getId() << endl;
                    // }
                }
            }
        }
        file.close();
        cout << "Program information loaded from " << filename << endl;
    }
    else
    {
        cout << "Unable to open file " << filename << endl;
    }
}

int main()
{
    AcademicOfficer ao("John Doe");
    Teacher teacher("Jane Smith");
    vector<Program *> programs;
    string filename = "programsInfo.txt";
    loadData(programs, filename);

    // cout << "Tested CLOs for Course " << currentCourse->getCode() << ":" << endl;
    // for (CLO *clo : currentCourse->getCourseTestedCLOs())
    // {
    //     cout << clo->getId() << endl;
    // }

    int choice;
    string topicName; // Declare topicName here
    vector<PLO *> plos;
    vector<Course *> coursesForCLO;
    vector<PLO *> plosForCLO;
    vector<CLO *> closForTopic;
    vector<Course *> coursesForTopic;
    vector<PLO *> plosForTopic;
    vector<Course *> courses;
    vector<CLO *> allCLOs;
    vector<CLO *> allCLOsForEval;
    vector<Course *> allCoursesForCase7;
    Course *courseOfCLO;
    CLO *cloToCheck;
    bool isTested;
    vector<Course *> allCoursesForCase8;
    vector<PLO *> allPLOsForCase9;
    int countForCase6 = 1;
    int countForCase7 = 1;
    int countForCase8 = 1;
    int countForCase9 = 1;

    do
    {
        cout << "Welcome to the OBE System!" << endl;
        cout << "Select an option:" << endl;
        cout << "1. Add Program" << endl;
        cout << "2. Add PLO" << endl;
        cout << "3. Add Course" << endl;
        cout << "4. Add CLO" << endl;
        cout << "5. Add Topic" << endl;
        cout << "6. Evaluate CLO" << endl;
        cout << "7. Check if a CLO is tested" << endl;
        cout << "8. Check if all CLOs of a course are tested" << endl;
        cout << "9. Generate a list of courses for a selected PLO" << endl;
        cout << "0. Exit" << endl;

        cin >> choice;

        switch (choice)
        {
        case 1:
            ao.addProgram(programs);
            break;
        case 2:
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }
            int programCh;
            cout << "Select a program to add PLO to:" << endl;
            for (int i = 0; i < programs.size(); ++i)
            {
                cout << i + 1 << ". " << programs[i]->getName() << endl;
            }
            cin >> programCh;
            if (programCh < 1 || programCh > programs.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            ao.addPLO(programs[programCh - 1]);
            break;
        case 3:
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }
            int programChoice;
            cout << "Select a program to add course to:" << endl;
            for (int i = 0; i < programs.size(); ++i)
            {
                cout << i + 1 << ". " << programs[i]->getName() << endl;
            }
            cin >> programChoice;
            if (programChoice < 1 || programChoice > programs.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            // Display PLOs of selected program
            cout << "Select a PLO to add course to:" << endl;
            plos = programs[programChoice - 1]->getPLOs();
            for (int i = 0; i < plos.size(); ++i)
            {
                cout << i + 1 << ". " << plos[i]->getId() << endl;
            }
            int ploChoice;
            cin >> ploChoice;
            if (ploChoice < 1 || ploChoice > plos.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            ao.addCourse(plos[ploChoice - 1]); // Adding course to the selected PLO
            break;

        case 4:
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }
            int programIndex;
            cout << "Select a program to add CLO to:" << endl;
            for (int i = 0; i < programs.size(); ++i)
            {
                cout << i + 1 << ". " << programs[i]->getName() << endl;
            }
            cin >> programIndex;
            if (programIndex < 1 || programIndex > programs.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            // Display PLOs of selected program
            cout << "Select a PLO to add CLO to:" << endl;
            plosForCLO = programs[programIndex - 1]->getPLOs();
            for (int i = 0; i < plosForCLO.size(); ++i)
            {
                cout << i + 1 << ". " << plosForCLO[i]->getId() << endl;
            }
            int ploIndexForCLO;
            cin >> ploIndexForCLO;
            if (ploIndexForCLO < 1 || ploIndexForCLO > plosForCLO.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            // Display courses of selected PLO
            cout << "Select a course to add CLO to:" << endl;
            coursesForCLO = plosForCLO[ploIndexForCLO - 1]->getCourses();
            for (int i = 0; i < coursesForCLO.size(); ++i)
            {
                cout << i + 1 << ". " << coursesForCLO[i]->getCode() << endl;
            }
            int courseIndexForCLO;
            cin >> courseIndexForCLO;
            if (courseIndexForCLO < 1 || courseIndexForCLO > coursesForCLO.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            ao.addCLO(coursesForCLO[courseIndexForCLO - 1]); // Adding CLO to the selected course
            break;

        case 5:
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }
            int programIndexForTopic;
            cout << "Select a program to add topic to:" << endl;
            for (int i = 0; i < programs.size(); ++i)
            {
                cout << i + 1 << ". " << programs[i]->getName() << endl;
            }
            cin >> programIndexForTopic;
            if (programIndexForTopic < 1 || programIndexForTopic > programs.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            // Display PLOs of selected program
            cout << "Select a PLO to add topic to:" << endl;
            plosForTopic = programs[programIndexForTopic - 1]->getPLOs();
            for (int i = 0; i < plosForTopic.size(); ++i)
            {
                cout << i + 1 << ". " << plosForTopic[i]->getId() << endl;
            }
            int ploIndexForTopic;
            cin >> ploIndexForTopic;
            if (ploIndexForTopic < 1 || ploIndexForTopic > plosForTopic.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            // Display courses of selected PLO
            cout << "Select a course to add topic to:" << endl;
            coursesForTopic = plosForTopic[ploIndexForTopic - 1]->getCourses();
            for (int i = 0; i < coursesForTopic.size(); ++i)
            {
                cout << i + 1 << ". " << coursesForTopic[i]->getCode() << endl;
            }
            int courseIndexForTopic;
            cin >> courseIndexForTopic;
            if (courseIndexForTopic < 1 || courseIndexForTopic > coursesForTopic.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }
            // Display CLOs of selected course
            cout << "Select a CLO to add topic to:" << endl;
            closForTopic = coursesForTopic[courseIndexForTopic - 1]->getCLOs();
            for (int i = 0; i < closForTopic.size(); ++i)
            {
                cout << i + 1 << ". " << closForTopic[i]->getId() << endl;
            }
            int cloIndexForTopic;
            cin >> cloIndexForTopic;
            if (cloIndexForTopic < 1 || cloIndexForTopic > closForTopic.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }

            ao.addTopic(programs[ploIndexForTopic - 1]->getPLOs()[courseIndexForTopic - 1]->getCourses()[0]->getCLOs()[cloIndexForTopic - 1]);

            break;

        case 6:
        {
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }
            countForCase6 = 1;
            // Display CLOs available for testing
            cout << "Available CLOs for testing:" << endl;

            vector<pair<CLO *, Course *>> allCLOsWithCourses; // Store CLOs along with their associated courses

            for (int i = 0; i < programs.size(); ++i)
            {
                vector<PLO *> plos = programs[i]->getPLOs();
                for (int j = 0; j < plos.size(); ++j)
                {
                    vector<Course *> courses = plos[j]->getCourses();
                    for (int k = 0; k < courses.size(); ++k)
                    {
                        vector<CLO *> clos = courses[k]->getCLOs();
                        for (int l = 0; l < clos.size(); ++l)
                        {
                            cout << countForCase6++ << ". "
                                 << "Program: " << programs[i]->getName()
                                 << ", PLO: " << plos[j]->getId()
                                 << ", Course: " << courses[k]->getCode()
                                 << ", CLO: " << clos[l]->getId() << endl;
                            allCLOsWithCourses.push_back(make_pair(clos[l], courses[k])); // Store CLOs with their associated courses
                        }
                    }
                }
            }

            int cloIndexForTest;
            cout << "Select a CLO to test: ";
            cin >> cloIndexForTest;
            if (cloIndexForTest < 1 || cloIndexForTest > allCLOsWithCourses.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }

            cout << "Select evaluation method:" << endl;
            cout << "1. Assignment" << endl;
            cout << "2. Quiz" << endl;
            cout << "3. Mid-Term" << endl;
            cout << "4. Final" << endl;
            int evalMethod;
            cin >> evalMethod;
            Evaluation *evaluation;
            switch (evalMethod)
            {
            case 1:
                evaluation = new Assignment();
                break;
            case 2:
                evaluation = new Quiz();
                break;
            case 3:
                evaluation = new MidTerm();
                break;
            case 4:
                evaluation = new Final();
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
            }

            if (evaluation != nullptr)
            {
                // Assuming cloIndexForTest is the index of the selected CLO
                // Assuming allCLOsWithCourses is a vector containing all available CLOs with their associated courses
                teacher.evaluateCLO(evaluation, allCLOsWithCourses[cloIndexForTest - 1].first, allCLOsWithCourses[cloIndexForTest - 1].second);
                cout << "CLO evaluated successfully." << endl;
            }
            else
            {
                cout << "Evaluation failed." << endl;
            }

            break;
        }

        case 7:
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }

            // Declare vector to hold all available CLOs

            // Collect all available CLOs
            countForCase7 = 1;

            for (int i = 0; i < programs.size(); ++i)
            {
                vector<PLO *> plos = programs[i]->getPLOs();
                for (int j = 0; j < plos.size(); ++j)
                {
                    vector<Course *> courses = plos[j]->getCourses();
                    for (int k = 0; k < courses.size(); ++k)
                    {
                        vector<CLO *> clos = courses[k]->getCLOs();
                        for (int l = 0; l < clos.size(); ++l)
                        {
                            cout << countForCase7++ << ". "
                                 << "Program: " << programs[i]->getName()
                                 << ", PLO: " << plos[j]->getId()
                                 << ", Course: " << courses[k]->getCode()
                                 << ", CLO: " << clos[l]->getId() << endl;
                        }
                        allCLOs.insert(allCLOs.end(), clos.begin(), clos.end());
                    }
                }
            }

            int cloIndexToCheck;
            cout << "Select a CLO to check if it's tested twice: ";
            cin >> cloIndexToCheck;
            if (cloIndexToCheck < 1 || cloIndexToCheck > allCLOs.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }

            if (isCLOtestedTwice(teacher.getEvaluations(), allCLOs[cloIndexToCheck - 1]))
            {
                cout << "The selected CLO has been tested." << endl;
            }
            else
            {
                cout << "The selected CLO has not been tested." << endl;
            }
            break;

            // case 7:
            //     if (programs.empty())
            //     {
            //         cout << "No programs available. Please add a program first." << endl;
            //         break;
            //     }

            //     // Collect all available CLOs and their courses
            //     // vector<CLO *> allCLOs;
            //     // vector<Course *> allCourses;

            //     countForCase7 = 1;

            //     for (Program *program : programs)
            //     {
            //         vector<PLO *> plos = program->getPLOs();
            //         for (PLO *plo : plos)
            //         {
            //             vector<Course *> courses = plo->getCourses();
            //             for (Course *course : courses)
            //             {
            //                 vector<CLO *> clos = course->getCLOs();
            //                 for (int l = 0; l < clos.size(); ++l)
            //                 {
            //                     cout << countForCase7++ << ". "
            //                          << "Program: " << program->getName()
            //                          << ", PLO: " << plo->getId()
            //                          << ", Course: " << course->getCode()
            //                          << ", CLO: " << clos[l]->getId() << endl;
            //                 }
            //                 allCLOs.insert(allCLOs.end(), clos.begin(), clos.end());
            //                 allCoursesForCase7.push_back(course);
            //             }
            //         }
            //     }

            //     int cloIndexToCheck;
            //     cout << "Select a CLO to check if it's tested twice: ";
            //     cin >> cloIndexToCheck;
            //     if (cloIndexToCheck < 1 || cloIndexToCheck > allCLOs.size())
            //     {
            //         cout << "Invalid choice." << endl;
            //         break;
            //     }

            //     courseOfCLO = allCoursesForCase7[cloIndexToCheck - 1];
            //     cloToCheck = allCLOs[cloIndexToCheck - 1];

            //     isTested = false;
            //     for (CLO *testedCLO : courseOfCLO->getCourseTestedCLOs())
            //     {
            //         if (testedCLO == cloToCheck)
            //         {
            //             isTested = true;
            //             break;
            //         }
            //     }

            //     if (isTested)
            //     {
            //         cout << "The selected CLO has been tested." << endl;
            //     }
            //     else
            //     {
            //         cout << "The selected CLO has not been tested." << endl;
            //     }
            //     break;

        case 8:
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }

            countForCase8 = 1;
            // Display courses available for selection
            cout << "Available Courses:" << endl;

            for (int i = 0; i < programs.size(); ++i)
            {
                vector<PLO *> plos = programs[i]->getPLOs();
                for (int j = 0; j < plos.size(); ++j)
                {
                    vector<Course *> courses = plos[j]->getCourses();
                    for (int k = 0; k < courses.size(); ++k)
                    {
                        cout << countForCase8++ << ". "
                             << "Program: " << programs[i]->getName()
                             << ", PLO: " << plos[j]->getId()
                             << ", Course: " << courses[k]->getCode() << endl;
                    }
                    allCoursesForCase8.insert(allCoursesForCase8.end(), courses.begin(), courses.end());
                }
            }

            int courseIndexToCheck;
            cout << "Select a course to check if all its CLOs are tested: ";
            cin >> courseIndexToCheck;
            if (courseIndexToCheck < 1 || courseIndexToCheck > allCoursesForCase8.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }

            if (areAllCLOsTested(teacher.getEvaluations(), allCoursesForCase8[courseIndexToCheck - 1]))
            {
                cout << "All CLOs for the selected course have been tested." << endl;
            }
            else
            {
                cout << "Not all CLOs for the selected course have been tested." << endl;
            }
            break;

        case 9:
            if (programs.empty())
            {
                cout << "No programs available. Please add a program first." << endl;
                break;
            }

            countForCase9 = 1;
            // Display PLOs available for selection
            cout << "Available PLOs:" << endl;

            for (int i = 0; i < programs.size(); ++i)
            {
                vector<PLO *> plos = programs[i]->getPLOs();
                for (int j = 0; j < plos.size(); ++j)
                {
                    cout << countForCase9++ << ". "
                         << "Program: " << programs[i]->getName()
                         << ", PLO: " << plos[j]->getId() << endl;
                }
                allPLOsForCase9.insert(allPLOsForCase9.end(), plos.begin(), plos.end());
            }

            int ploIndexForCourses;
            cout << "Select a PLO to generate a list of courses: ";
            cin >> ploIndexForCourses;
            if (ploIndexForCourses < 1 || ploIndexForCourses > allPLOsForCase9.size())
            {
                cout << "Invalid choice." << endl;
                break;
            }

            courses = getCoursesForPLO(programs[ploIndexForCourses - 1], allPLOsForCase9[ploIndexForCourses - 1]->getId());
            cout << "Courses for selected PLO:" << endl;
            for (int i = 0; i < courses.size(); ++i)
            {
                cout << i + 1 << ". " << courses[i]->getCode() << endl;
            }
            break;

        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    saveData(programs, filename);

    // Clean up dynamic memory for programs
    for (auto p : programs)
    {
        delete p;
    }

    return 0;
}
