#include "qgsndvirendererwidget.h"
#include "moc_qgsndvirendererwidget.cpp"
#include "qgsrasterlayer.h"
#include "qgsndvirenderer.h"
#include "qgsrasterdataprovider.h"
#include "qgsrasterrendererregistry.h"
#include "qgsapplication.h"
#include "qgsmessagelog.h"
#include "qgslogger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QDebug>

// Fonction pour écrire dans un fichier log personnalisé
static void writeToLogFile(const QString &message)
{
  // Utilisons un fichier dans le répertoire personnel de l'utilisateur
  QString homePath = QDir::homePath();
  QString logFilePath = homePath + "/qgis_ndvi_debug.log";
  
  QFile file(logFilePath);
  if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
  {
    QTextStream stream(&file);
    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") 
           << " - " << message << "\n";
    file.close();
    
    // Pour déboguer le fait que le fichier a bien été écrit
    qDebug() << "NDVI LOG: Wrote to log file: " << logFilePath;
  }
  else
  {
    // Si l'ouverture du fichier échoue, afficher l'erreur
    qDebug() << "NDVI LOG: Failed to open log file: " << logFilePath << " - Error: " << file.errorString();
  }
  
  // Ajout d'un message direct dans la console pour garantir que nous voyons le message
  qWarning() << "NDVI WIDGET: " << message;
}

// Fonction statique d'initialisation pour s'assurer que le renderer est correctement enregistré
static void initializeNdviRenderer()
{
  static bool initialized = false;
  if (!initialized)
  {
    // Force register widget type
    if (QgsApplication::rasterRendererRegistry())
    {
      QgsApplication::rasterRendererRegistry()->insertWidgetFunction( QStringLiteral( "ndvi" ), QgsNdviRendererWidget::create );
      qWarning() << "NDVI Renderer: Widget function registered manually!";
      initialized = true;
    }
  }
}

#define QgsDebugMsg(str) QgsDebugMsgLevel(str, 2)

QgsNdviRendererWidget::QgsNdviRendererWidget( QgsRasterLayer *layer, const QgsRectangle &extent )
  : QgsRasterRendererWidget( layer, extent )
{
  // Force register our widget in case it wasn't done
  initializeNdviRenderer();
  
  writeToLogFile( "Initializing NDVI renderer widget" );
  setupUi( this );
  
  mNirBandCombo->setLayer( mRasterLayer );
  mRedBandCombo->setLayer( mRasterLayer );
  
  if ( mRasterLayer )
  {
    writeToLogFile( QString( "Layer name: %1, Band count: %2" ).arg( mRasterLayer->name() ).arg( mRasterLayer->bandCount() ) );
    QgsRasterDataProvider *provider = mRasterLayer->dataProvider();
    if ( provider )
    {
      writeToLogFile( "Provider is valid, setting up from renderer" );
      setFromRenderer( layer->renderer() );
    }
    else
    {
      writeToLogFile( "Provider is invalid" );
    }
  }
  else
  {
    writeToLogFile( "Layer is invalid" );
  }
  
  // Connect signals to detect changes
  connect( mNirBandCombo, &QgsRasterBandComboBox::bandChanged, this, &QgsRasterRendererWidget::widgetChanged );
  connect( mRedBandCombo, &QgsRasterBandComboBox::bandChanged, this, &QgsRasterRendererWidget::widgetChanged );
  
  writeToLogFile( "NDVI renderer widget initialized" );
}

QgsRasterRenderer *QgsNdviRendererWidget::renderer()
{
  writeToLogFile( "Creating NDVI renderer" );
  
  if ( !mRasterLayer )
  {
    writeToLogFile( "Cannot create renderer - layer is null" );
    return nullptr;
  }
  
  QgsRasterDataProvider *provider = mRasterLayer->dataProvider();
  if ( !provider )
  {
    writeToLogFile( "Cannot create renderer - provider is null" );
    return nullptr;
  }
  
  const int nirBand = mNirBandCombo->currentBand();
  const int redBand = mRedBandCombo->currentBand();
  
  writeToLogFile( QString( "Creating NDVI renderer with NIR band: %1, Red band: %2" ).arg( nirBand ).arg( redBand ) );
  
  QgsNdviRenderer *renderer = new QgsNdviRenderer( provider, nirBand, redBand );
  writeToLogFile( "NDVI renderer created successfully" );
  return renderer;
}

void QgsNdviRendererWidget::setFromRenderer( const QgsRasterRenderer *renderer )
{
  writeToLogFile( "Setting up widget from renderer" );
  
  const QgsNdviRenderer *r = dynamic_cast<const QgsNdviRenderer *>( renderer );
  if ( r )
  {
    writeToLogFile( QString( "Setting from NDVI renderer - NIR band: %1, Red band: %2" ).arg( r->nirBand() ).arg( r->redBand() ) );
    mNirBandCombo->setBand( r->nirBand() );
    mRedBandCombo->setBand( r->redBand() );
  }
  else
  {
    writeToLogFile( "Not an NDVI renderer, setting default bands" );
    // Default configuration when coming from another renderer
    if ( mRasterLayer && mRasterLayer->dataProvider() )
    {
      writeToLogFile( QString( "Setting default bands - layer has %1 bands" ).arg( mRasterLayer->bandCount() ) );
      // Try to find appropriate NIR and Red bands
      // By default, use bands 4 (NIR) and 3 (Red) if available
      if ( mRasterLayer->bandCount() >= 4 )
      {
        writeToLogFile( "Using default NIR (band 4) and Red (band 3)" );
        mNirBandCombo->setBand( 4 );
        mRedBandCombo->setBand( 3 );
      }
      else if ( mRasterLayer->bandCount() >= 2 )
      {
        // Otherwise, use the first two available bands
        writeToLogFile( "Using first two bands as NIR (band 2) and Red (band 1)" );
        mNirBandCombo->setBand( 2 );
        mRedBandCombo->setBand( 1 );
      }
      else
      {
        writeToLogFile( "Not enough bands available for NDVI" );
      }
    }
    else
    {
      writeToLogFile( "Layer or provider is invalid for band setup" );
    }
  }
  writeToLogFile( "Widget setup from renderer completed" );
}
