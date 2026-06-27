import pandas as pd

# Load dataset
df = pd.read_csv("hospital_deterioration_hourly_panel.csv")

# Columns to keep (13)
keep_columns = [
    "patient_id",
    "hour_from_admission",
    "heart_rate",
    "respiratory_rate",
    "spo2_pct",
    "temperature_c",
    "systolic_bp",
    "diastolic_bp",
    "age",
    "gender",
    "comorbidity_index",
    "deterioration_event",
    "deterioration_next_12h"
]

df = df[keep_columns]


# Get first 3000 unique patients
selected_patients = df["patient_id"].unique()[:3000]

# Filter dataset
df = df[df["patient_id"].isin(selected_patients)]


df = df[df["hour_from_admission"] <= 10]


aggregated_df = df.groupby("patient_id").agg({

    # Vitals → avg, max, min
    "heart_rate": ["mean", "max", "min"],
    "respiratory_rate": ["mean", "max", "min"],
    "spo2_pct": ["mean", "max", "min"],
    "temperature_c": ["mean", "max", "min"],
    "systolic_bp": ["mean", "max", "min"],
    "diastolic_bp": ["mean", "max", "min"],

    # Static values → first value
    "age": "first",
    "gender": "first",
    "comorbidity_index": "first",

    # Outcome
    "deterioration_event": "max",
    "deterioration_next_12h": "max"
})


# Flatten column names
aggregated_df.columns = [
    f"{col[0]}_{col[1]}" if col[1] != "first" else col[0]
    for col in aggregated_df.columns
]

aggregated_df = aggregated_df.reset_index()


aggregated_df.to_csv("final_patient_level_dataset.csv", index=False)

print("✅ Final dataset created!")
print("Total rows:", len(aggregated_df))
print("Total columns:", len(aggregated_df.columns))


# =========================
# STEP 0: Libraries
# =========================
import pandas as pd


# =========================
# STEP 1: Load dataset + keep selected columns
# =========================
df = pd.read_csv("hospital_deterioration_hourly_panel.csv")

keep_columns = [
    "patient_id",
    "hour_from_admission",
    "heart_rate",
    "respiratory_rate",
    "spo2_pct",
    "temperature_c",
    "systolic_bp",
    "diastolic_bp",
    "age",
    "gender",
    "comorbidity_index",
    "deterioration_event",
    "deterioration_next_12h"
]

df = df[keep_columns]


# =========================
# STEP 2: First 3000 patients
# =========================
selected_patients = df["patient_id"].unique()[:3000]
df = df[df["patient_id"].isin(selected_patients)]


# =========================
# STEP 3: First 10 hours only
# =========================
df = df[df["hour_from_admission"] <= 10]


# =========================
# STEP 4: Aggregation + hours_observed
# =========================
aggregated_df = df.groupby("patient_id").agg({

    "heart_rate": ["mean", "max", "min"],
    "respiratory_rate": ["mean", "max", "min"],
    "spo2_pct": ["mean", "max", "min"],
    "temperature_c": ["mean", "max", "min"],
    "systolic_bp": ["mean", "max", "min"],
    "diastolic_bp": ["mean", "max", "min"],

    "hour_from_admission": "count",

    "age": "first",
    "gender": "first",
    "comorbidity_index": "first",

    "deterioration_event": "max",
    "deterioration_next_12h": "max"
})


# =========================
# STEP 5: Clean column names
# =========================
aggregated_df.columns = [
    "hours_observed" if col[0] == "hour_from_admission" else
    f"{col[0]}_{col[1]}" if col[1] != "first" else col[0]
    for col in aggregated_df.columns
]

aggregated_df = aggregated_df.reset_index()


# Move hours_observed just after patient_id
cols = aggregated_df.columns.tolist()

# Remove hours_observed from current position
cols.remove("hours_observed")

# Insert it after patient_id (index 1)
cols.insert(1, "hours_observed")

# Reorder dataframe
aggregated_df = aggregated_df[cols]



# =========================
# STEP 6: Save final CSV
# =========================
aggregated_df.to_csv("final_patient_level_dataset.csv", index=False)

print("✅ Final dataset created successfully")
print("Rows:", len(aggregated_df))
print("Columns:", len(aggregated_df.columns))
