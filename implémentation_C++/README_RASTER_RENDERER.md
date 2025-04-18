# Tutoriel d'implémentation d'un Renderer Raster dans QGIS

Ce tutoriel explique comment implémenter un nouveau type de rendu raster dans QGIS, similaire au HillShadeRenderer existant.

## Introduction

QGIS permet de visualiser les données raster de différentes manières grâce à ses renderers. Les renderers sont des classes qui définissent comment les valeurs d'un raster sont transformées en couleurs pour l'affichage.

Exemples de renderers existants :
- SingleBandGray
- SingleBandPseudoColor
- MultiBand
- HillShade
- Contour

## Prérequis

- Connaissance de C++ et Qt
- Familiarité avec le framework QGIS
- Environnement de développement QGIS configuré
- Compréhension de base des données raster

## Structure d'un Renderer

Un renderer raster dans QGIS est généralement composé de :

1. **Classe de Renderer** - Dérive de `QgsRasterRenderer`
2. **Widget de configuration** - Dérive de `QgsRasterRendererWidget`
3. **Registre** - Pour enregistrer le renderer dans QGIS

## Étapes d'implémentation

### 1. Créer la classe de Renderer

```cpp
// qgsmynewrenderer.h
#include "qgsrasterrenderer.h"

class QgsMyNewRenderer : public QgsRasterRenderer
{
  public:
    QgsMyNewRenderer( QgsRasterInterface *input = nullptr );
    ~QgsMyNewRenderer() override;

    // Clone ce renderer
    QgsMyNewRenderer *clone() const override;

    // Retourne le type de renderer (unique)
    QString type() const override { return "mynewrenderer"; }

    // Méthode principale qui effectue le rendu
    QgsRasterBlock *block( int bandNo, const QgsRectangle &extent, int width, int height, QgsRasterBlockFeedback *feedback = nullptr ) override;

    // Autres méthodes requises...
    
  private:
    // Paramètres spécifiques à votre renderer
    double mFactor;
    double mOffset;
    // ...
};
```

### 2. Implémenter la classe de Renderer

```cpp
// qgsmynewrenderer.cpp
#include "qgsmynewrenderer.h"

QgsMyNewRenderer::QgsMyNewRenderer( QgsRasterInterface *input )
  : QgsRasterRenderer( input, "mynewrenderer" )
  , mFactor( 1.0 )
  , mOffset( 0.0 )
{
}

QgsMyNewRenderer::~QgsMyNewRenderer()
{
}

QgsMyNewRenderer *QgsMyNewRenderer::clone() const
{
  QgsMyNewRenderer *renderer = new QgsMyNewRenderer( nullptr );
  renderer->copyCommonProperties( this );
  
  // Copier les propriétés spécifiques
  renderer->mFactor = mFactor;
  renderer->mOffset = mOffset;
  
  return renderer;
}

QgsRasterBlock *QgsMyNewRenderer::block( int bandNo, const QgsRectangle &extent, int width, int height, QgsRasterBlockFeedback *feedback )
{
  // Récupérer les données raster d'entrée
  QgsRasterBlock *inputBlock = mInput->block( bandNo, extent, width, height, feedback );
  if ( !inputBlock || inputBlock->isEmpty() )
  {
    return inputBlock;
  }

  // Créer un nouveau bloc pour la sortie
  QgsRasterBlock *outputBlock = new QgsRasterBlock( Qgis::DataType::ARGB32_Premultiplied, width, height );
  
  // Traiter les données raster
  for ( int y = 0; y < height; y++ )
  {
    for ( int x = 0; x < width; x++ )
    {
      // Lire la valeur d'entrée
      double value = inputBlock->value( y, x );
      
      // Appliquer votre algorithme ici
      double newValue = value * mFactor + mOffset;
      
      // Convertir en couleur
      QRgb color = qRgba( newValue, newValue, newValue, 255 );
      
      // Stocker la couleur dans le bloc de sortie
      outputBlock->setColor( y, x, color );
    }
  }
  
  // Libérer la mémoire du bloc d'entrée
  delete inputBlock;
  
  return outputBlock;
}
```

### 3. Créer le Widget de configuration

```cpp
// qgsmynewrendererwidget.h
#include "qgsrasterrendererwidget.h"

class QgsMyNewRendererWidget : public QgsRasterRendererWidget
{
    Q_OBJECT
  public:
    QgsMyNewRendererWidget( QgsRasterLayer *layer, const QgsRectangle &extent = QgsRectangle() );
    ~QgsMyNewRendererWidget() override;
    
    // Créer un renderer à partir des paramètres du widget
    QgsRasterRenderer *renderer() override;
    
    // Synchroniser le widget avec un renderer existant
    void setFromRenderer( const QgsRasterRenderer *r ) override;
    
    // ...
    
  private:
    QLineEdit *mFactorEdit = nullptr;
    QLineEdit *mOffsetEdit = nullptr;
    // ...
};
```

### 4. Implémenter le Widget de configuration

```cpp
// qgsmynewrendererwidget.cpp
#include "qgsmynewrendererwidget.h"
#include "qgsmynewrenderer.h"

QgsMyNewRendererWidget::QgsMyNewRendererWidget( QgsRasterLayer *layer, const QgsRectangle &extent )
  : QgsRasterRendererWidget( layer, extent )
{
  QVBoxLayout *layout = new QVBoxLayout( this );
  
  // Créer les contrôles pour les paramètres
  QGridLayout *gridLayout = new QGridLayout();
  
  // Facteur
  QLabel *factorLabel = new QLabel( tr( "Facteur:" ) );
  mFactorEdit = new QLineEdit( "1.0" );
  gridLayout->addWidget( factorLabel, 0, 0 );
  gridLayout->addWidget( mFactorEdit, 0, 1 );
  
  // Offset
  QLabel *offsetLabel = new QLabel( tr( "Offset:" ) );
  mOffsetEdit = new QLineEdit( "0.0" );
  gridLayout->addWidget( offsetLabel, 1, 0 );
  gridLayout->addWidget( mOffsetEdit, 1, 1 );
  
  layout->addLayout( gridLayout );
  layout->addStretch();
}

QgsMyNewRendererWidget::~QgsMyNewRendererWidget()
{
}

QgsRasterRenderer *QgsMyNewRendererWidget::renderer()
{
  if ( !mRasterLayer )
  {
    return nullptr;
  }
  
  QgsMyNewRenderer *renderer = new QgsMyNewRenderer( mRasterLayer->dataProvider() );
  
  // Récupérer les valeurs du widget
  renderer->setFactor( mFactorEdit->text().toDouble() );
  renderer->setOffset( mOffsetEdit->text().toDouble() );
  
  return renderer;
}

void QgsMyNewRendererWidget::setFromRenderer( const QgsRasterRenderer *r )
{
  if ( !r )
  {
    return;
  }
  
  const QgsMyNewRenderer *renderer = dynamic_cast<const QgsMyNewRenderer *>( r );
  if ( !renderer )
  {
    return;
  }
  
  // Mettre à jour les contrôles avec les valeurs du renderer
  mFactorEdit->setText( QString::number( renderer->factor() ) );
  mOffsetEdit->setText( QString::number( renderer->offset() ) );
}
```

### 5. Enregistrer votre Renderer dans QGIS

Dans la fonction principale de votre plugin ou une fonction d'initialisation :

```cpp
#include "qgsrasterlayer.h"
#include "qgsmynewrendererwidget.h"

// Fonction de fabrique pour créer votre widget
QgsRasterRendererWidget *myNewRendererWidgetFactory( QgsRasterLayer *layer, const QgsRectangle &extent )
{
  return new QgsMyNewRendererWidget( layer, extent );
}

void registerRenderer()
{
  // Enregistrer le renderer et sa fabrique de widget
  QgsRasterLayer::addRendererWidget( "mynewrenderer", QObject::tr( "Mon Nouveau Renderer" ), myNewRendererWidgetFactory );
}
```

## Exemple : Comprendre le HillShadeRenderer

Le HillShadeRenderer est un bon exemple à étudier :

- `qgshillshaderenderer.h` et `qgshillshaderenderer.cpp` pour la classe de renderer
- `qgshillshaderendererwidget.h` et `qgshillshaderendererwidget.cpp` pour le widget

Étudiez comment il calcule les ombres en fonction de l'altitude, l'azimut et d'autres paramètres.

## Conseils d'implémentation

1. **Gestion de la mémoire** : Assurez-vous de bien gérer les allocations mémoire et éviter les fuites
2. **Performance** : Optimisez votre algorithme pour qu'il soit rapide même sur de grands rasters
3. **Feedback** : Utilisez le paramètre `feedback` pour permettre l'annulation des opérations longues
4. **Sérialisation** : Implémentez les méthodes `writeXml()` et `readXml()` pour sauvegarder/charger vos paramètres
5. **Cohérence visuelle** : Suivez les conventions UI de QGIS pour votre widget

## Intégration au projet QGIS

Si vous voulez que votre renderer fasse partie du code source de QGIS :

1. Placez vos fichiers dans les répertoires appropriés :
   - Headers : `src/core/raster/` (renderer) et `src/gui/raster/` (widget)
   - Sources : idem
2. Mettez à jour les fichiers CMakeLists.txt correspondants
3. Enregistrez votre renderer dans `qgsrasterrendererregistry.cpp`
4. Ajoutez la documentation appropriée avec des commentaires Doxygen

## Ressources supplémentaires

- Documentation de l'API QGIS
- Code source des renderers existants dans QGIS
- Exemples de plugins QGIS qui utilisent des renderers personnalisés

## Exemple : Structure complète des fichiers

```
src/core/raster/
  ├── qgsmynewrenderer.h
  └── qgsmynewrenderer.cpp
src/gui/raster/
  ├── qgsmynewrendererwidget.h
  └── qgsmynewrendererwidget.cpp
```

Bonne implémentation !