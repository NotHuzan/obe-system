#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Forward declarations
class Program;
class PLO;
class Course;
class CLO;
class Topic;
class Evaluation;

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
    void addProgram(Program *program);
    void addPLO(Program *program, PLO *plo);
    void addCourse(PLO *plo, Course *course);
    void addCLO(Course *course, CLO *clo);
    void addTopic(CLO *clo, Topic *topic);
};

class Teacher : public Person
{
public:
    Teacher(string n) : Person(n) {}
    void evaluateCLO(Evaluation *evaluation, CLO *clo);
};

class Program
{
    string name;
    // unordered_map<string, PLO*> plos;
    vector<PLO *> plos;

public:
    Program(string n) : name(n) {}
    string getName() const { return name; }
    void addPLO(PLO *plo)
    {
        // plos[plo->getId()] = plo;
        plos.push_back(plo);
    }
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

public:
    Course(string c) : code(c) {}
    string getCode() const { return code; }
    void addCLO(CLO *clo) { clos.push_back(clo); }
    vector<CLO *> getCLOs() const { return clos; }
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


// Academic Officer functions implementation

// void AcademicOfficer::addProgram(Program *program)
// {
//     // Implementation to add a program
// }

void AcademicOfficer::addPLO(Program *program, PLO *plo)
{
    program->addPLO(plo);
}

void AcademicOfficer::addCourse(PLO *plo, Course *course)
{
    plo->addCourse(course);
}

void AcademicOfficer::addCLO(Course *course, CLO *clo)
{
    course->addCLO(clo);
}

void AcademicOfficer::addTopic(CLO *clo, Topic *topic)
{
    clo->addTopic(topic);
}

// Teacher function implementation
void Teacher::evaluateCLO(Evaluation *evaluation, CLO *clo)
{
    evaluation->addTestedCLO(clo);
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
    vector<CLO *> clos = course->getCLOs();
    for (CLO *clo : clos)
    {
        bool tested = false;
        for (Evaluation *evaluation : evaluations)
        {
            vector<CLO *> testedCLOs = evaluation->getTestedCLOs();
            if (find(testedCLOs.begin(), testedCLOs.end(), clo) != testedCLOs.end())
            {
                tested = true;
                break;
            }
        }
        if (!tested)
            return false;
    }
    return true;
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

int main()
{
    // Usage example
    AcademicOfficer ao("John Doe");
    Teacher teacher("Jane Smith");
    Program program("Computer Science");

    // Adding PLOs
    PLO *plo1 = new PLO("PLO1");
    PLO *plo2 = new PLO("PLO2");
    ao.addPLO(&program, plo1);
    ao.addPLO(&program, plo2);

    // Adding courses
    Course *course1 = new Course("CS101");
    Course *course2 = new Course("CS102");
    // Course *course3 = new Course("CSE103");
    ao.addCourse(plo1, course1);
    ao.addCourse(plo2, course2);
    // ao.addCourse(plo1, course3);

    // Adding CLOs
    CLO *clo1 = new CLO("CLO1");
    CLO *clo2 = new CLO("CLO2");
    // CLO *clo3 = new CLO("CLO3");
    ao.addCLO(course1, clo1);
    ao.addCLO(course2, clo2);
    // ao.addCLO(course1, clo3);

    // Adding topics
    Topic *topic1 = new Topic("Topic 1");
    Topic *topic2 = new Topic("Topic 2");
    ao.addTopic(clo1, topic1);
    ao.addTopic(clo2, topic2);

    // Teacher evaluating CLOs
    Assignment assignment;
    Quiz quiz;
    teacher.evaluateCLO(&assignment, clo1);
    teacher.evaluateCLO(&quiz, clo1);
    teacher.evaluateCLO(&quiz, clo2);

    // Reports
    cout << "CLO1 tested twice: " << isCLOtestedTwice({&assignment, &quiz}, clo1) << endl;
    cout << "All CLOs of CS101 tested: " << areAllCLOsTested({&assignment, &quiz}, course1) << endl;
    vector<Course *> coursesForPLO = getCoursesForPLO(&program, "PLO1");
    cout << "Courses for PLO1: ";
    for (Course *course : coursesForPLO)
    {
        cout << course->getCode() << " ";
    }
    cout << endl;

    return 0;
}
