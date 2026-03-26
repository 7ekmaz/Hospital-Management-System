#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department
{
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType
{
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //
class Patient
{
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:
    Patient(int pid, string n, int a, string c){
        this->id = pid;
        this->name = n;
        this->age = a;
        this->contact = c;
        this->isAdmitted = false;

    }

    // Getter methods
    int getAge() const // Needed in Hospital Class
    {
        return age;
    }

    string getContact() const // Needed in Hospital Class
    {
        return contact;
    }

    string getRoomTypeName() const // Needed in Hospital Class
    {
        switch (roomType)
        {
        case GENERAL_WARD:
            return "General Ward";
        case ICU:
            return "Intensive Care Unit (ICU)";
        case PRIVATE_ROOM:
            return "Private Room";
        case SEMI_PRIVATE:
            return "Semi-Private Room";
        default:
            return "Unknown";
        }
    }

    void admitPatient(RoomType type){
        if(isAdmitted){
            cout<<"Patient["<<name<<"] is already admitted";
            return; 
        }
        else{
            isAdmitted = true;
            roomType = type;
            cout<<"Patient["<<name<<"] admitted to ["<<getRoomTypeName()<<"]";
            return;
        }
    }
    void dischargePatient(){
            if (!isAdmitted) {
                cout << "Patient [" << name << "] is not currently admitted." << endl;
                return;
            }
            else {
                isAdmitted = false;
                cout << "Patient [" << name << "] has been discharged." << endl;
            }
    }
    void addMedicalRecord(string record){
            medicalHistory.push(record);
            cout << "Medical record added for [" << name << "]: " << record << endl;

    }
    void requestTest(string testName){
            testQueue.push(testName);
            cout << "Test requested for [" << name << "]: " << testName << endl;
    }
    string performTest(){
            if (testQueue.empty()) {
                cout << "No pending tests for [" << name << "]." << endl;
                return "";
            }
            else {
                string testName = testQueue.front();
                testQueue.pop();
                cout << "Performing test for [" << name << "]: " << testName << endl;
                return testName;
            }
    }
    void displayHistory(){
            cout << "Medical History for [" << name << "]:" << endl;

            if (medicalHistory.empty()) {
                cout << " No medical history." << endl;
                return;
            }

            stack<string> temp = medicalHistory;

            while (!temp.empty()) {
                cout << " - " << temp.top() << endl;
                temp.pop();
            }
    }

    int getId(){
        return id;
    }
    string getName(){
        return name;
    }
    bool getAdmissionStatus(){
        return isAdmitted;
    }
};


// ========== DOCTOR CLASS ========== //
class Doctor
{
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    // Constructor
    Doctor(int did, string n, Department d) {
        this->id = did;
        this->name = n;
        this->department = d;
    }

    // Adds a patient ID to the appointment queue (FIFO)
    void addAppointment(int patientId) {
        appointmentQueue.push(patientId);
        // Note: Expected output from test cases assumes the name already includes "Dr." 
        cout << "Appointment added for patient " << patientId << " with " << name << endl;
    }

    // Processes the next patient in the queue
    int seePatient() {
        if (appointmentQueue.empty()) {
            cout << "No appointments for " << name << "." << endl;
            return -1;
        } else {
            int patientId = appointmentQueue.front();
            appointmentQueue.pop();
            cout << name << " is seeing patient " << patientId << endl;
            return patientId;
        }
    }

    // Getters
    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    // Converts the enum to a human-readable string
    string getDepartment() {
        switch (department) {
            case CARDIOLOGY:
                return "Cardiology";
            case NEUROLOGY:
                return "Neurology";
            case ORTHOPEDICS:
                return "Orthopedics";
            case PEDIATRICS:
                return "Pediatrics";
            case EMERGENCY:
                return "Emergency";
            case GENERAL:
                return "General Practice";
            default:
                return "Unknown";
        }
    }

    int getAppointmentCount() const // Needed in Hospital Class
    {
        return appointmentQueue.size();
    }
};

// ========== HOSPITAL CLASS ========== //
class Hospital
{
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:
    // ---- Constructor ----
    Hospital() : patientCounter(1), doctorCounter(1) {}

    // ---- registerPatient ----
    int registerPatient(string name, int age, string contact)
    {
        Patient p(patientCounter, name, age, contact);
        patients.push_back(p);
        cout << "Patient registered: " << name
             << " (ID: " << patientCounter << ")" << endl;
        return patientCounter++;
    }

    // ---- addDoctor ----
    int addDoctor(string name, Department dept)
    {
        Doctor d(doctorCounter, name, dept);
        doctors.push_back(d);

        // Build department name for the print
        string deptName;
        switch (dept)
        {
        case CARDIOLOGY:
            deptName = "Cardiology";
            break;
        case NEUROLOGY:
            deptName = "Neurology";
            break;
        case ORTHOPEDICS:
            deptName = "Orthopedics";
            break;
        case PEDIATRICS:
            deptName = "Pediatrics";
            break;
        case EMERGENCY:
            deptName = "Emergency";
            break;
        case GENERAL:
            deptName = "General";
            break;
        }

        cout << "Doctor added: " << name
             << " (ID: " << doctorCounter << ") - " << deptName << endl;
        return doctorCounter++;
    }

    // ---- admitPatient ----
    void admitPatient(int patientId, RoomType type)
    {
        for (int i = 0; i < (int)patients.size(); i++)
        {
            if (patients[i].getId() == patientId)
            {
                patients[i].admitPatient(type);
                return;
            }
        }
        cout << "Patient not found." << endl;
    }

    // ---- addEmergency ----
    void addEmergency(int patientId)
    {
        emergencyQueue.push(patientId);
        cout << "Emergency added for patient " << patientId << endl;
    }

    // ---- handleEmergency ----
    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
            cout << "No emergency cases." << endl;
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handling emergency for patient " << patientId << endl;
        return patientId;
    }

    // ---- bookAppointment ----
    void bookAppointment(int doctorId, int patientId)
    {
        // Find doctor
        int doctorIndex = -1;
        for (int i = 0; i < (int)doctors.size(); i++)
        {
            if (doctors[i].getId() == doctorId)
            {
                doctorIndex = i;
                break;
            }
        }

        if (doctorIndex == -1)
        {
            cout << "Error: Doctor ID " << doctorId << " not found." << endl;
            return;
        }

        // Find patient
        bool patientFound = false;
        for (int i = 0; i < (int)patients.size(); i++)
        {
            if (patients[i].getId() == patientId)
            {
                patientFound = true;
                break;
            }
        }

        if (!patientFound)
        {
            cout << "Error: Patient ID " << patientId << " not found." << endl;
            return;
        }

        // Book appointment
        doctors[doctorIndex].addAppointment(patientId);
    }

    // ---- displayPatientInfo ----
    void displayPatientInfo(int patientId)
    {
        for (int i = 0; i < (int)patients.size(); i++)
        {
            if (patients[i].getId() == patientId)
            {
                cout << "=== Patient Information ===" << endl
                     << "ID: " << patients[i].getId() << endl
                     << "Name: " << patients[i].getName() << endl
                     << "Age: " << patients[i].getAge() << endl
                     << "Contact: " << patients[i].getContact() << endl
                     << "Admission Status: "
                     << (patients[i].getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
                if (patients[i].getAdmissionStatus())
                {
                    cout << "Room Type: " << patients[i].getRoomTypeName() << endl;
                }
                cout << "==========================" << endl;
                return;
            }
        }
        cout << "Patient not found." << endl;
    }

    // ---- displayDoctorInfo ----
    void displayDoctorInfo(int doctorId)
    {
        for (int i = 0; i < (int)doctors.size(); i++)
        {
            if (doctors[i].getId() == doctorId)
            {
                cout << "=== Doctor Information ===" << endl;
                cout << "ID: " << doctors[i].getId() << endl;
                cout << "Name: " << doctors[i].getName() << endl;
                cout << "Department: " << doctors[i].getDepartment() << endl;
                cout << "Pending Appointments: "
                     << doctors[i].getAppointmentCount()
                     << endl;
                cout
                    << "==========================" << endl;
                return;
            }
        }
        cout << "Doctor not found." << endl;
    }
};

// ========== MAIN PROGRAM ========== //
int main()
{
    Hospital hospital;

    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient

    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients

    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies

    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects

    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor

    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors

    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    return 0;
}
