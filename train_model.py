import pandas as pd


df = pd.read_csv("synthetic-asthma-dataset.csv")
df_asthma = df[df["Has_Asthma"] == 1]

selected_features = [
    "Age",
    "BMI",
    "Air_Pollution_Level",
    "Physical_Activity_Level",
    "Medication_Adherence",
    "Number_of_ER_Visits",
    "Peak_Expiratory_Flow",
    "FeNO_Level",
    "Asthma_Control_Level"   # hedef değişken
]

df_selected = df_asthma[selected_features]
print(df_selected.head())
print(df_selected["Asthma_Control_Level"].value_counts())
df_selected.info()
