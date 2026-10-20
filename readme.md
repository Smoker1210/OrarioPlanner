# OrarioPlanner 📅

**OrarioPlanner** è un'applicazione desktop sviluppata in **C++** e **Qt** per la gestione, pianificazione e generazione automatica dei report settimanali degli orari di lavoro dei dipendenti.

Il progetto è progettato per garantire la massima compatibilità retroattiva ed è convalidato per l'esecuzione su sistemi **Windows 10** tramite compilazione in **Qt 5.15 LTS**.

---

## 🚀 Caratteristiche Principali

* **Pianificazione Settimanale**: Gestione intuitiva dei turni per ciascun dipendente (turni continuati, spezzati e gestione assenze).
* **Parsing Intelligente degli Orari**:
  * Normalizzazione automatica dei formati orario (es. `13.00`, `13.0`, `13.` convertiti in formato standard).
  * Gestione dei turni spezzati tramite separatore `/`.
  * Supporto alle assenze speciali e trasferte (es. `R`, `M`, `104`, `CORSO`, `T: NomeCittà`).
* **Esportazione PDF/HTML**:
  * Generazione di report PDF settimanali in formato A4 Landscape.
  * Header personalizzato con titolo della settimana e metriche in evidenza (Redditività, Ore Suggerite, Ore Totali).
  * Layout ottimizzato per la stampa e box note dinamico a fondo pagina.
* **Installer Windows**: Pacchetto di installazione autonomo e compatto creato con **Inno Setup**.

---

## 🛠️ Tech Stack & Requisiti

* **Linguaggio**: C++ (C++17)
* **Framework GUI**: Qt 5.15 LTS
* **Architettura**: Model-View-Controller (MVC)
* **Deployment & Installazione**: `windeployqt`, Inno Setup Compiler
* **Compatibilità**: Windows 10 (Build 1607 o successive) / Windows 11

---

## 📦 Installazione e Download

Per installare ed eseguire **OrarioPlanner**:

1. Scarica l'ultimo installer `OrarioPlanner_Setup_v1.0.exe` dalla sezione [Releases](../../releases).
2. Esegui il file `.exe` e segui la procedura guidata di installazione.
3. Avvia l'applicazione dal collegamento sul desktop o dal menu Start.

---

## 🔧 Compilazione da Sorgente

Per compilare il progetto in locale con Qt Creator:

```bash
# Clona il repository
git clone [https://github.com/Smoker1210/OrarioPlanner.git](https://github.com/Smoker1210/OrarioPlanner.git)

# Entra nella directory del progetto
cd OrarioPlanner

# Apri il file .pro con Qt Creator o compila da riga di comando con qmake (Qt 5.15)
qmake orarioplanner.pro
make

```

---

## 👤 Autore
Samuele Gobbi - [@Smoker1210](https://github.com/Smoker1210)