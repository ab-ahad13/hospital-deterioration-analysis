#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int main() {

    string csv_path = "final_patient_level_dataset_age_below_70.csv";
    char again = 'y';

    while (again == 'y' || again == 'Y') {

        string search_id;
        cout << "\nEnter Patient ID: ";
        cin >> search_id;

        ifstream file(csv_path);
        if (!file.is_open()) {
            cout << "? Could not open dataset file.\n";
            return 0;
        }

        string line;
        getline(file, line); // skip header

        bool found = false;

        while (getline(file, line)) {

            stringstream ss(line);
            vector<string> cols;
            string value;

            while (getline(ss, value, ',')) {
                cols.push_back(value);
            }

            // Safety check
            if (cols.size() < 29) continue;

            // patient_id column = 0
            if (cols[0] == search_id) {

                found = true;

                // ---------------- DISPLAY BASIC DATA ----------------
                cout << "\n? PATIENT FOUND";
                cout << "\n---------------------------------\n";
                cout << "Patient ID: " << cols[0] << endl;
                cout << "Age: " << cols[20] << endl;
                cout << "Gender: " << cols[21] << endl;
                cout << "Heart Rate Mean: " << cols[2] << endl;
                cout << "SpO2 Mean: " << cols[8] << endl;
                cout << "Systolic BP Mean: " << cols[14] << endl;
                cout << "Diastolic BP Mean: " << cols[17] << endl;
                cout << "Risk Score: " << cols[28] << endl;

                // ---------------- CONVERSIONS ----------------
                double spo2_mean = stod(cols[8]);
                double sbp_mean  = stod(cols[14]);
                double dbp_mean  = stod(cols[17]);

                int low_spo2_flag = stoi(cols[25]);
                int high_hr_flag  = stoi(cols[26]);
                int full_10h_flag = stoi(cols[27]);
                double risk_score = stod(cols[28]);

                // ---------------- ALERTS ----------------
                cout << "\n?? ALERTS & MEDICAL GUIDANCE";
                cout << "\n---------------------------------\n";

                bool any_alert = false;

                // Oxygen / SpO2
                if (low_spo2_flag == 1 || spo2_mean < 90) {
                    cout << "? LOW OXYGEN LEVEL\n";
                    cout << "? Oxygen support required\n";
                    cout << "? Continuous SpO2 monitoring\n";
                    any_alert = true;
                }

                // Heart Rate
                if (high_hr_flag == 1) {
                    cout << "\n? HIGH HEART RATE\n";
                    cout << "? Cardiac monitoring advised\n";
                    cout << "? Avoid physical exertion\n";
                    any_alert = true;
                }

                // Blood Pressure - LOW
                if (sbp_mean < 90 || dbp_mean < 60) {
                    cout << "\n?? LOW BLOOD PRESSURE (HYPOTENSION)\n";
                    cout << "? Immediate doctor attention required\n";
                    cout << "? IV fluids / emergency care may be needed\n";
                    any_alert = true;
                }

                // Blood Pressure - HIGH
                if (sbp_mean > 140 || dbp_mean > 90) {
                    cout << "\n? HIGH BLOOD PRESSURE (HYPERTENSION)\n";
                    cout << "? BP monitoring required\n";
                    cout << "? Medication adjustment may be needed\n";
                    any_alert = true;
                }

                // Risk Score
                if (risk_score >= 1.0) {
                    cout << "\n?? HIGH RISK PATIENT\n";
                    cout << "? Urgent doctor review required\n";
                    cout << "? ICU / advanced monitoring recommended\n";
                    any_alert = true;
                }

                // Monitoring duration
                if (full_10h_flag == 0) {
                    cout << "\n? Monitoring duration insufficient\n";
                    cout << "? Continue observation for accurate assessment\n";
                }

                // Stable case
                if (!any_alert) {
                    cout << "\n? PATIENT STABLE\n";
                    cout << "? Routine monitoring sufficient\n";
                    cout << "? No immediate medical intervention required\n";
                }

                break;
            }
        }

        file.close();

        if (!found) {
            cout << "\n? Patient does not exist in dataset\n";
        }

        cout << "\n\nCheck another patient? (y/n): ";
        cin >> again;
    }

    cout << "\nProgram finished.\n";
    return 0;
}

