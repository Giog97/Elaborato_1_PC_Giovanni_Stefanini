# 🎨 **Mean Shift Image Segmentation**  
🚀 *Confronto tra implementazioni **sequenziale** e **parallela***  

> Questo progetto esplora il clustering **Mean Shift** per la segmentazione di immagini, confrontando una versione **sequenziale** e una **parallela** ottimizzata con **OpenMP**.  

---

## 🛠️ **Caratteristiche**
- ✂️ **Segmentazione immagini** tramite **Mean Shift**.
- ⏱️ **Confronto delle performance** tra versioni sequenziale e parallela.
- 📁 Supporto per file immagine `.jpg`, `.png` e `.bmp`.
- 💾 Salvataggio dei risultati in una cartella dedicata.

---

## 🖥️ **Linguaggi e librerie usati**
✅ **C++**  
✅ **OpenCV** (per l'elaborazione delle immagini).  
✅ **OpenMP** (per la parallelizzazione).  
✅ **MinGW** (per compilare).  
✅ **CMake** (per build multi-piattaforma).  

---

## ⚙️ **Utilizzo**
1. **Aggiungi immagini** nella directory `./img`. Alcune immagini di esempio 512x512 sono già presenti all'interno del progetto
2. **Esegui il programma**
3. **Seleziona un'immagine** dalla lista visualizzata nel terminale (immagini presenti nella directory `./img`).
4. Guarda i risultati nella directory `./img_results` e confronta i tempi di esecuzione dei due algoritmi 

💡 *Il programma mostrerà i tempi di esecuzione per entrambe le implementazioni!*  

---

## 🔧 **Parametri Personalizzabili**
- 📏 **Bandwidth**: Ampiezza della finestra di ricerca (*più grande = segmentazione meno dettagliata*).  
- 🎯 **Epsilon**: Soglia di convergenza (*più piccolo = risultati più precisi, ma più lenti*).  
- 🔍 **Resize Factor**: Ridimensiona l'immagine per test più veloci (1 = no scaling).  

---

## 🖼️ **Esempio di Output**
### 📊 **Performance:**
```
Thread disponibili: 8
Tempo di esecuzione (sequenziale): 12.34 secondi.
Tempo di esecuzione (parallelo): 3.45 secondi.
```

### 🗂️ **Immagini Risultanti:**
Le immagini saranno salvate in:  
- **`./img_results/sequential_segmented_image_X.jpg`**  
- **`./img_results/parallel_segmented_image_X.jpg`**  
- **`./img_results/resized_image_X.jpg`**

---

## 🎉 **Contributi**
💡 Questo progetto è stato realizzato da **Giovanni Stefanini**, per il superamento dell'esame di Parallel Computing.  

---

### 👀 **Demo Visiva**
Esempio di segmentazione "prima e dopo":   
| **Input Originale** | **Segm. Sequenziale** | **Segm. Parallela** |  
|---------------------|------------------------------|------------------------------|  
| ![Original](./cmake-build-debug/img_results/resized_image_1.jpg) | ![Sequential](./cmake-build-debug/img_results/sequential_segmented_image_1.jpg) | ![Parallel](./cmake-build-debug/img_results/parallel_segmented_image_1.jpg) |  
| ![Original](./cmake-build-debug/img_results/resized_image_2.jpg) | ![Sequential](./cmake-build-debug/img_results/sequential_segmented_image_2.jpg) | ![Parallel](./cmake-build-debug/img_results/parallel_segmented_image_2.jpg) |  
| ![Original](./cmake-build-debug/img_results/resized_image_3.jpg) | ![Sequential](./cmake-build-debug/img_results/sequential_segmented_image_3.jpg) | ![Parallel](./cmake-build-debug/img_results/parallel_segmented_image_3.jpg) |  
