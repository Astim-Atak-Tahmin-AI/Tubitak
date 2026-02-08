import pandas as pd
import numpy as np
import joblib
import os
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, confusion_matrix
from sklearn.impute import SimpleImputer
from imblearn.over_sampling import SMOTE 

# --- DOSYA YOLLARI ---
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA_PATH = os.path.join(BASE_DIR, 'data', 'raw', 'asthma_disease_data_fixed.csv')
MODEL_PATH = os.path.join(BASE_DIR, 'models', 'risk_tahmin_modeli.pkl')
SCALER_PATH = os.path.join(BASE_DIR, 'models', 'scaler.pkl')

def train_risk_model():
    print("\n--- LOJİSTİK REGRESYON (OPTIMİZE EDİLMİŞ) ---")
    
    # 1. VERİ YÜKLEME
    if not os.path.exists(DATA_PATH):
        print(f"HATA: Dosya bulunamadı -> {DATA_PATH}")
        return
    
    df = pd.read_csv(DATA_PATH)
    print(f"✅ Veri yüklendi. Hasta Sayısı: {len(df)}")

    # 2. HEDEF BELİRLEME
    def risk_belirle(row):
        if row['Diagnosis'] == 1:
            # Eşik değerler
            if row['PollutionExposure'] > 5.5 or row['LungFunctionFEV1'] < 2.2:
                return 1 # YÜKSEK RİSK
            return 0 
        return 0

    df['Risk_Level'] = df.apply(risk_belirle, axis=1)

    # 3. ÖZELLİKLER
    features = [
        'Age', 'BMI', 'Smoking', 'PhysicalActivity', 
        'PollutionExposure', 'PollenExposure', 'DustExposure', 
        'LungFunctionFEV1', 'Wheezing', 'ShortnessOfBreath',
        'FamilyHistoryAsthma'
    ]
    
    X = df[features].copy()
    y = df['Risk_Level']

    # 4. EKSİK VERİ TAMAMLAMA
    imputer = SimpleImputer(strategy='mean')
    X_imputed = imputer.fit_transform(X)

    # 5. EĞİTİM / TEST AYRIMI
    X_train, X_test, y_train, y_test = train_test_split(
        X_imputed, y, test_size=0.2, random_state=42, stratify=y
    )

    # --- SMOTE AYARI (Doğruluğu Artıran Kısım) ---
    # sampling_strategy=0.5 -> Riskli veriyi, sağlıklı verinin yarısı kadar çoğalt.
    # Bu, modeli "paranoyak" yapmadan riskleri öğretir. Doğruluğu artırır.
    print("Veri optimize ediliyor (SMOTE)...")
    smote = SMOTE(sampling_strategy=0.5, random_state=42)
    X_train_resampled, y_train_resampled = smote.fit_resample(X_train, y_train)

    # 6. SCALING
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train_resampled)
    X_test_scaled = scaler.transform(X_test)

    # 7. MODEL EĞİTİMİ
    print("Model eğitiliyor...")
    model = LogisticRegression(solver='lbfgs', max_iter=2000, random_state=42)
    model.fit(X_train_scaled, y_train_resampled)

    # 8. SONUÇLAR
    y_pred = model.predict(X_test_scaled)
    
    tn, fp, fn, tp = confusion_matrix(y_test, y_pred).ravel()
    acc = accuracy_score(y_test, y_pred)
    risk_yakalama = tp / (tp + fn) if (tp + fn) > 0 else 0 
    
    # SADELEŞTİRİLMİŞ RAPOR
    print("\n" + "="*50)
    print("      📢 PROJE PERFORMANS RAPORU")
    print("="*50)
    print(f"✅ GENEL DOĞRULUK (Accuracy)       : %{acc * 100:.2f}")
    print("   (Sistemin genel başarısı)")
    print("-" * 50)
    print(f"🚨 RİSK YAKALAMA ORANI (Recall)    : %{risk_yakalama * 100:.2f}")
    print("   (Kriz geçiren hastaları tespit etme başarısı)")
    print("="*50)
    print(f"Toplam Test Edilen        : {len(y_test)}")
    print(f"Gerçekten Riskli Olanlar  : {tp + fn}")
    print(f"Başarıyla Tespit Edilen   : {tp}")
    print(f"Gözden Kaçan              : {fn}")
    print("="*50)

    # 9. KAYIT
    if not os.path.exists(os.path.dirname(MODEL_PATH)):
        os.makedirs(os.path.dirname(MODEL_PATH))
        
    joblib.dump(model, MODEL_PATH)
    joblib.dump(scaler, SCALER_PATH)
    print(f"\n💾 Model ve Scaler kaydedildi: {MODEL_PATH}")

if __name__ == "__main__":
    train_risk_model()