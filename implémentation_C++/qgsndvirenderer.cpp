#include "qgsndvirenderer.h"
#include "qgsrasterblock.h"
#include "qgsrastershader.h"
#include "qgsrasterinterface.h"
#include "qgsrasteriterator.h"
#include "qgssymbollayerutils.h"

#include <QDomDocument>
#include <QDomElement>
#include <QColor>

QgsNdviRenderer *QgsNdviRenderer::clone() const
{
  QgsNdviRenderer *renderer = new QgsNdviRenderer( mInput, mNirBand, mRedBand, nullptr );
  renderer->copyCommonProperties( this );
  return renderer;
}

Qgis::RasterRendererFlags QgsNdviRenderer::flags() const
{
  return Qgis::RasterRendererFlags( Qgis::RasterRendererFlag::InternalLayerOpacityHandling );
}

QList<int> QgsNdviRenderer::usesBands() const
{
  QList<int> bandList;
  if ( mRedBand > 0 )
  {
    bandList << mRedBand;
  }
  if ( mNirBand > 0 )
  {
    bandList << mNirBand;
  }
  return bandList;
}

QgsRasterBlock *QgsNdviRenderer::block( int, QgsRectangle const &extent, int width, int height, QgsRasterBlockFeedback *feedback )
{
  std::unique_ptr<QgsRasterBlock> outputBlock = std::make_unique<QgsRasterBlock>();
  if ( !mInput || mNirBand <= 0 || mRedBand <= 0 )
  {
    return outputBlock.release();
  }

  std::unique_ptr<QgsRasterBlock> nirBlock( mInput->block( mNirBand, extent, width, height, feedback ) );
  if ( !nirBlock || nirBlock->isEmpty() )
  {
    return outputBlock.release();
  }

  std::unique_ptr<QgsRasterBlock> redBlock( mInput->block( mRedBand, extent, width, height, feedback ) );
  if ( !redBlock || redBlock->isEmpty() )
  {
    return outputBlock.release();
  }

  if ( !outputBlock->reset( Qgis::DataType::ARGB32_Premultiplied, width, height ) )
  {
    return outputBlock.release();
  }

  const QRgb myDefaultColor = renderColorForNodataPixel();

  for ( int i = 0; i < width * height; i++ )
  {
    if ( feedback && feedback->isCanceled() )
    {
      break;
    }

    bool isNirNoData = nirBlock->isNoData( i );
    bool isRedNoData = redBlock->isNoData( i );

    if ( isNirNoData || isRedNoData )
    {
      outputBlock->setColor( i, myDefaultColor );
      continue;
    }

    double nirValue = nirBlock->valueAndNoData( i, isNirNoData );
    double redValue = redBlock->valueAndNoData( i, isRedNoData );

    double ndviValue = 0.0;
    double denominator = nirValue + redValue;
    
    if ( denominator != 0.0 )
    {
      ndviValue = ( nirValue - redValue ) / denominator;
    }
    else
    {
      outputBlock->setColor( i, myDefaultColor );
      continue;
    }

    if ( ndviValue < -1.0 || ndviValue > 1.0 )
    {
      outputBlock->setColor( i, myDefaultColor );
      continue;
    }

    QRgb color;
    if ( mShader )
    {
      int r = 0, g = 0, b = 0, a = 255;
      if ( mShader->shade( ndviValue, &r, &g, &b, &a ) )
      {
        color = qRgba( r, g, b, a );
      }
      else
      {
        color = myDefaultColor;
      }
    }
    else
    {
      int green = qBound( 0, static_cast<int>( ( ndviValue + 1.0 ) * 127.5 ), 255 );
      int red = qBound( 0, static_cast<int>( ( 1.0 - ndviValue ) * 127.5 ), 255 );
      color = qRgba( red, green, 0, 255 );
    }

    if ( mOpacity < 1.0 )
    {
      color = qRgba( qRed( color ), qGreen( color ), qBlue( color ),
                    static_cast<int>( qAlpha( color ) * mOpacity ) );
    }

    outputBlock->setColor( i, color );
  }

  return outputBlock.release();
}

void QgsNdviRenderer::writeXml( QDomDocument &doc, QDomElement &parentElem ) const
{
  if ( parentElem.isNull() )
  {
    return;
  }

  QDomElement rasterRendererElem = doc.createElement( QStringLiteral( "rasterrenderer" ) );
  rasterRendererElem.setAttribute( QStringLiteral( "type" ), QStringLiteral( "ndvi" ) );
  rasterRendererElem.setAttribute( QStringLiteral( "band" ), -1 ); 
  rasterRendererElem.setAttribute( QStringLiteral( "nirBand" ), mNirBand );
  rasterRendererElem.setAttribute( QStringLiteral( "redBand" ), mRedBand );

  if ( mShader )
  {
    mShader->writeXml( doc, rasterRendererElem ); 
  }

  QDomElement minMaxElem = doc.createElement( QStringLiteral( "ndviClassing" ) );
  minMaxElem.setAttribute( QStringLiteral( "min" ), QString::number( mClassificationMin ) );
  minMaxElem.setAttribute( QStringLiteral( "max" ), QString::number( mClassificationMax ) );
  rasterRendererElem.appendChild( minMaxElem );

  _writeXml( doc, rasterRendererElem );

  parentElem.appendChild( rasterRendererElem );
}

QgsRasterRenderer *QgsNdviRenderer::create( const QDomElement &elem, QgsRasterInterface *input )
{
  if ( elem.isNull() )
  {
    return nullptr;
  }

  int nirBand = elem.attribute( QStringLiteral( "nirBand" ), QStringLiteral( "1" ) ).toInt();
  int redBand = elem.attribute( QStringLiteral( "redBand" ), QStringLiteral( "0" ) ).toInt();
  
  QgsRasterShader *shader = nullptr;
  QDomElement shaderElem = elem.firstChildElement( QStringLiteral( "rastershader" ) );
  if ( !shaderElem.isNull() )
  {
    shader = new QgsRasterShader();
    shader->readXml( shaderElem );
  }

  QgsNdviRenderer *r = new QgsNdviRenderer( input, nirBand, redBand, shader );
  r->readXml( elem );

  QDomElement minMaxElem = elem.firstChildElement( QStringLiteral( "ndviClassing" ) );
  if ( !minMaxElem.isNull() )
  {
    r->mClassificationMin = minMaxElem.attribute( QStringLiteral( "min" ), QStringLiteral( "-1.0" ) ).toDouble();
    r->mClassificationMax = minMaxElem.attribute( QStringLiteral( "max" ), QStringLiteral( "1.0" ) ).toDouble();
  }

  return r;
}

bool QgsNdviRenderer::setNirBand( int band )
{
  if ( band < 1 ) 
  {
    return false;
  }
  mNirBand = band;
  return true;
}

bool QgsNdviRenderer::setRedBand( int band )
{
  if ( band < 1 )
  {
    return false;
  }
  mRedBand = band;
  return true;
}