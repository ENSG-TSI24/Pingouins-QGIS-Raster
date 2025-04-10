# 🛰️ Render — Plugin QGIS pour la télédétection raster

**Render** est un plugin développé pour QGIS 3.40 (Bratislava), permettant d'appliquer des traitements de télédétection sur des images raster, tels que :

- Calcul d'indice **NDVI** à partir de deux bandes raster (Infrarouge et Rouge)
- Génération d'un **ombrage** (hillshade) à partir d'un MNT ou raster d'altitude
- Visualisation et affichage interactif d'images

---

## ✅ Environnement supporté

Le plugin est compatible avec :

- **QGIS 3.40 Bratislava** (ou toute version *Long Term Release* à partir de 3.40)
- **Qt 5** (fourni avec QGIS, pas besoin d'installation manuelle)
- **Python 3.10+**
- **Bibliothèques Python** requises :
  - `numpy`
  - `osgeo` (inclus via GDAL dans QGIS)

---

## 🖥️ Installation

### 📦 1. Télécharger le plugin

Deux méthodes possibles :

#### a. Cloner depuis GitHub

```bash
git clone https://github.com/ENSG-TSI24/Pingouins-QGIS-Raster.git

#### b. Télécharger l'archive ZIP

    Télécharger ZIP

    Extraire le dossier sur votre machine

### 🗂️ 2. Installer dans QGIS

    Ouvrir QGIS

    Aller dans Extensions > Installer/Gérer les extensions

    Cliquer sur l’onglet Installer depuis un fichier ZIP

    Sélectionner le fichier ZIP ou le dossier du plugin (render)

    Valider — le plugin apparaît alors dans le menu Extensions sous "Nex Render Raster teledetection"

##  Installation spécifique par système

### 🐧 Linux (Ubuntu / Debian)

sudo apt install qgis python3-pyqt5 python3-gdal

Puis suivez l’installation depuis le ZIP ci-dessus.

### 🪟 Windows

    Télécharger QGIS 3.40 (version "long term") depuis qgis.org

    Les dépendances sont incluses avec l'installateur

    Lancer QGIS et suivre l’installation du plugin comme décrit ci-dessus

### 🍎 macOS

    Installer QGIS 3.40 depuis https://qgis.org

    Vérifier que Python et GDAL sont bien configurés via le QGIS terminal intégré

    Ensuite, installer le plugin comme décrit

## 🧪 Exemple d'utilisation

    Charger deux images raster correspondant à la bande rouge et infrarouge proche

    Sélectionner-les dans l'interface du plugin

    Lancer le calcul du NDVI

    Afficher un ombrage à partir d’un raster unique

## 🤝 Auteurs

Développé par Les Pingouins 🐧 – ENSG / TSI 2025

