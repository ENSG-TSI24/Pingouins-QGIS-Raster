# 📖 Guide d'utilisation du plugin QGIS Render

Le plugin **Render** vous permet d’effectuer rapidement deux traitements courants en télédétection raster directement dans QGIS :

- Calcul de l’indice **NDVI**
- Génération d’un **ombrage (hillshade)** topographique

Ce guide détaille toutes les étapes nécessaires pour utiliser efficacement le plugin.

---

## 🧱 Prérequis

Avant d’utiliser le plugin :

- Installez **QGIS 3.40 Bratislava** ou version ultérieure
- Téléchargez et installez le plugin Render depuis le dépôt GitHub (ZIP ou clonage)
- Vérifiez que vos rasters sont bien chargés dans QGIS (Menu `Couches > Ajouter une couche > Raster`)

---

## 🖼️ Étape 1 – Charger les rasters dans QGIS

### Pour le NDVI :
- Deux images raster issues de capteurs multi-spectraux
  - Bande Infrarouge proche (PIR)
  - Bande Rouge (Red)
- Exemple : Sentinel-2 (B8 et B4), Landsat 8 (B5 et B4)

### Pour l’ombrage (Hillshade) :
- Une image raster représentant un MNT ou une couche d’altitude

---

## 🚀 Étape 2 – Lancer le plugin Render

1. Ouvrez QGIS
2. Allez dans `Extensions > Render`
3. La fenêtre principale du plugin s’ouvre avec plusieurs champs à compléter

---

## 🌿 Étape 3 – Calculer un NDVI personnalisé

1. Dans le menu déroulant **"Bande PIR pour NDVI"**, sélectionnez la couche Infrarouge proche
2. Dans **"Bande Rouge pour NDVI"**, sélectionnez la couche Rouge
3. Dans le champ **"Chemin de sortie NDVI"**, cliquez sur l’icône de dossier 📁 pour choisir un emplacement sur votre disque
   - Exemple : `/home/utilisateur/Documents/ndvi_result.tif`
4. Cliquez sur `OK`
   - ✅ Le NDVI est calculé
   - ✅ Le fichier est enregistré à l’emplacement choisi
   - ✅ Une couche “NDVI” s’ajoute automatiquement dans QGIS

⚠️ Si aucun chemin n’est défini, le calcul NDVI sera ignoré.

---

## 🏔️ Étape 4 – Générer un Hillshade avec vos paramètres

1. Dans **"Image raster multi-bandes"**, sélectionnez une couche représentant un MNT ou une image d’altitude
2. Dans les champs :
   - **Altitude solaire** (°) : modifiez l'angle d’élévation (ex: 45)
   - **Azimut solaire** (°) : modifiez la direction du soleil (ex: 315)
3. Dans le champ **"Chemin de sortie ombrage"**, sélectionnez le fichier de sortie (ex : `ombrage_result.tif`)
4. Cliquez sur `OK`
   - ✅ L’ombrage est calculé avec les paramètres fournis
   - ✅ Une couche “Ombrage” est ajoutée automatiquement à QGIS

⚠️ Si le champ du chemin est vide, l’opération est annulée.

---

## 💡 Astuces de visualisation

- Clic droit sur la couche > `Propriétés > Symbologie`
- Pour le NDVI :
  - Utilisez une palette `Spectral` ou `Viridis`
  - Définissez un min/max à `-1 / 1`
- Pour l’ombrage :
  - Utilisez une échelle de gris (`Grayscale`)
  - Ajustez le contraste et la transparence pour plus de lisibilité

---

## 📂 Chemins d’enregistrement

Les fichiers NDVI et Hillshade sont **enregistrés manuellement** selon les chemins que vous définissez :

- Vous pouvez sauvegarder n’importe où (Bureau, projet, etc.)
- Format : `.tif` (GeoTIFF)

---

## 📌 Récapitulatif des options du plugin

| Option                            | Description                                               |
|----------------------------------|-----------------------------------------------------------|
| Bande PIR / Rouge                | Sélection des deux bandes pour le NDVI                   |
| Image raster multi-bandes        | Raster source pour calculer l’ombrage                    |
| Altitude solaire                 | Angle d’élévation du soleil (°)                          |
| Azimut solaire                   | Angle de direction du soleil (°)                         |
| Chemin de sortie NDVI            | Fichier GeoTIFF pour enregistrer le NDVI                 |
| Chemin de sortie ombrage         | Fichier GeoTIFF pour enregistrer l’ombrage               |

---

## 🛠️ En cas de problème

- Vérifiez que les couches sélectionnées ont la même taille et emprise
- Assurez-vous que tous les chemins de sortie sont définis
- Si une couche ne s’affiche pas, vérifiez la validité du raster dans QGIS

---

## 🧪 Exemple complet d’utilisation

1. Charger les bandes B4 et B8 de Sentinel-2
2. Charger un MNT (fichier .tif)
3. Lancer le plugin Render
4. Sélectionner :
   - PIR : B8
   - Rouge : B4
   - MNT : raster MNT
   - Altitude : 45°
   - Azimut : 315°
5. Définir les chemins de sortie
6. Valider : NDVI et ombrage générés en 1 clic 🚀

---

