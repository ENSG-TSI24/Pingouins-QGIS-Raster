#ifndef QGSNDVIRENDERER_H
#define QGSNDVIRENDERER_H

#include "qgis_core.h"
#include "qgis_sip.h"
#include "qgsrasterrenderer.h"
#include "qgsrastershader.h"
#include "qgsrastershaderfunction.h"

class QgsRasterBlock;
class QgsRectangle;
class QgsRasterInterface;
class QDomElement;

/**
 * \ingroup core
 * \brief A renderer for calculating and displaying NDVI (Normalized Difference Vegetation Index) from NIR and Red bands.
 */
class CORE_EXPORT QgsNdviRenderer : public QgsRasterRenderer
{
  public:

    /**
     * \brief A renderer for generating NDVI (Normalized Difference Vegetation Index).
     * \param input The input raster interface
     * \param nirBand The NIR (Near Infrared) band
     * \param redBand The Red band
     * \param shader Optional shader for custom colorization
     */
    QgsNdviRenderer( QgsRasterInterface *input, int nirBand, int redBand, QgsRasterShader *shader = nullptr ):
      QgsRasterRenderer( input, "ndvi" ),
      mNirBand( nirBand ),
      mRedBand( redBand ),
      mShader( shader )
    {
      mClassificationMin = -1.0;
      mClassificationMax = 1.0;
    }

    /**
     * Returns the NIR band used by the renderer
     */
    int nirBand() const { return mNirBand; }
    
    /**
     * Returns the Red band used by the renderer
     */
    int redBand() const { return mRedBand; }

    /**
     * Set the NIR band to use for NDVI calculation
     * \returns true if the band is valid
     */
    bool setNirBand( int band );

    /**
     * Set the Red band to use for NDVI calculation
     * \returns true if the band is valid
     */
    bool setRedBand( int band );

    /**
     * Returns a list of the bands used by the renderer
     */
    QList<int> usesBands() const override;

    /**
     * Creates a deep copy of this renderer
     */
    QgsNdviRenderer *clone() const override SIP_FACTORY;
    
    /**
     * Returns the renderer flags
     */
    Qgis::RasterRendererFlags flags() const override;

    /**
     * Generates a raster block for the specified extent and size
     */
    QgsRasterBlock *block( int bandNo, const QgsRectangle &extent, int width, int height, QgsRasterBlockFeedback *feedback = nullptr ) override;

    /**
     * Factory method to create a new NDVI renderer from XML
     */
    static QgsRasterRenderer *create( const QDomElement &elem, QgsRasterInterface *input ) SIP_FACTORY;

    /**
     * Writes the renderer state to XML
     */
    void writeXml( QDomDocument &doc, QDomElement &parentElem ) const override;

    /**
     * Returns the input band number used as NIR band
     */
    int inputBand() const override { return mNirBand; }
    
  private:

    int mNirBand;
    int mRedBand;
    std::unique_ptr<QgsRasterShader> mShader;
    double mClassificationMin;
    double mClassificationMax;
};

#endif // QGSNDVIRENDERER_H