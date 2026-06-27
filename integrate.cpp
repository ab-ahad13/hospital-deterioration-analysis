#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

// ---------- mean ----------
double mean(const vector<double>& v) {
    double s = 0.0;
    for (double x : v) s += x;
    return v.empty() ? 0.0 : s / v.size();
}

int main() {

    string final_csv = "final_patient_level_dataset_age_below_70.csv";
    string new_csv   = "new_user_input_patient_data.csv";

    char again = 'y';

    while (again == 'y' || again == 'Y') {

        // ---------- INPUT ----------
        string patient_id, gender;
        int hours_observed, age;

        cout << "\nPatient ID: ";
        cin >> patient_id;

        cout << "Hours observed: ";
        cin >> hours_observed;

        cout << "Age: ";
        cin >> age;

        cout << "Gender (M/F): ";
        cin >> gender;

        // ---------- VITALS ----------
        vector<double> hr, rr, spo2, temp, sbp, dbp;

        for (int i = 0; i < hours_observed; i++) {
            double x;
            cout << "\nHour " << i + 1 << " data\n";
            cout << "Heart Rate: "; cin >> x; hr.push_back(x);
            cout << "Respiratory Rate: "; cin >> x; rr.push_back(x);
            cout << "SpO2: "; cin >> x; spo2.push_back(x);
            cout << "Temperature: "; cin >> x; temp.push_back(x);
            cout << "Systolic BP: "; cin >> x; sbp.push_back(x);
            cout << "Diastolic BP: "; cin >> x; dbp.push_back(x);
        }

        // ---------- FEATURES ----------
        double hr_mean = mean(hr), hr_max = *max_element(hr.begin(), hr.end()), hr_min = *min_element(hr.begin(), hr.end());
        double rr_mean = mean(rr), rr_max = *max_element(rr.begin(), rr.end()), rr_min = *min_element(rr.begin(), rr.end());
        double spo2_mean = mean(spo2), spo2_max = *max_element(spo2.begin(), spo2.end()), spo2_min = *min_element(spo2.begin(), spo2.end());
        double temp_mean = mean(temp), temp_max = *max_element(temp.begin(), temp.end()), temp_min = *min_element(temp.begin(), temp.end());
        double sbp_mean = mean(sbp), sbp_max = *max_element(sbp.begin(), sbp.end()), sbp_min = *min_element(sbp.begin(), sbp.end());
        double dbp_mean = mean(dbp), dbp_max = *max_element(dbp.begin(), dbp.end()), dbp_min = *min_element(dbp.begin(), dbp.end());

        double hr_range = hr_max - hr_min;
        int low_spo2_flag = spo2_mean < 90 ? 1 : 0;
        int high_hr_flag  = hr_mean > 100 ? 1 : 0;
        int full_10h_flag = hours_observed >= 10 ? 1 : 0;

        int deterioration_event_max = 0;
        int deterioration_next_12h_max = 0;
        double risk_score = (low_spo2_flag + high_hr_flag) * 0.5;

        // ---------- ROW ----------
        string row =
            patient_id + "," + to_string(hours_observed) + "," +
            to_string(hr_mean) + "," + to_string(hr_max) + "," + to_string(hr_min) + "," +
            to_string(rr_mean) + "," + to_string(rr_max) + "," + to_string(rr_min) + "," +
            to_string(spo2_mean) + "," + to_string(spo2_max) + "," + to_string(spo2_min) + "," +
            to_string(temp_mean) + "," + to_string(temp_max) + "," + to_string(temp_min) + "," +
            to_string(sbp_mean) + "," + to_string(sbp_max) + "," + to_string(sbp_min) + "," +
            to_string(dbp_mean) + "," + to_string(dbp_max) + "," + to_string(dbp_min) + "," +
            to_string(age) + "," + gender + "," +
            to_string(deterioration_event_max) + "," +
            to_string(deterioration_next_12h_max) + "," +
            to_string(hr_range) + "," +
            to_string(low_spo2_flag) + "," +
            to_string(high_hr_flag) + "," +
            to_string(full_10h_flag) + "," +
            to_string(risk_score);

        // ---------- NEW CSV (HEADER ALWAYS WRITTEN IF FILE NEW) ----------
        bool new_file = false;
        ifstream check(new_csv);
        if (!check.good()) new_file = true;
        check.close();

        ofstream new_out(new_csv, ios::app);

        if (new_file) {
            new_out <<
            "patient_id,hours_observed,"
            "heart_rate_mean,heart_rate_max,heart_rate_min,"
            "respiratory_rate_mean,respiratory_rate_max,respiratory_rate_min,"
            "spo2_pct_mean,spo2_pct_max,spo2_pct_min,"
            "temperature_c_mean,temperature_c_max,temperature_c_min,"
            "systolic_bp_mean,systolic_bp_max,systolic_bp_min,"
            "diastolic_bp_mean,diastolic_bp_max,diastolic_bp_min,"
            "age,gender,"
            "deterioration_event_max,deterioration_next_12h_max,"
            "heart_rate_range,low_spo2_flag,high_hr_flag,full_10h_flag,risk_score\n";
        }

        new_out << row << "\n";
        new_out.close();

        // ---------- APPEND TO FINAL CLEANED ----------
        ofstream final_out(final_csv, ios::app);
        final_out << row << "\n";
        final_out.close();

        cout << "\n? Data saved successfully.\n";
        cout << "\nAdd another patient? (y/n): ";
        cin >> again;
    }

    cout << "\nProgram finished.\n";
    return 0;
}

