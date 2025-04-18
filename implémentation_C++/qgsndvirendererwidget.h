#ifndef QGSNDVIRENDERERWIDGET_H
#define QGSNDVIRENDERERWIDGET_H

#include "ui_qgsndvirendererwidget.h"
#include "qgis_sip.h"
#include "qgsrasterrendererwidget.h"
#include "qgis_gui.h"

/**
 * \ingroup gui
 * \brief Renderer widget for the NDVI renderer.
 */
class GUI_EXPORT QgsNdviRendererWidget : public QgsRasterRendererWidget, private Ui::QgsNdviWidget
{
    Q_OBJECT
  public:
    /**
     * \brief Renderer widget for the NDVI renderer.
     * \param layer The layer attached for this widget.
     * \param extent The current extent.
     */
    QgsNdviRendererWidget( QgsRasterLayer *layer, const QgsRectangle &extent = QgsRectangle() );
    
    /**
     * Factory method to create the renderer for this type.
     */
    static QgsRasterRendererWidget *create( QgsRasterLayer *layer, const QgsRectangle &extent ) SIP_FACTORY { return new QgsNdviRendererWidget( layer, extent ); }
    
    /**
     * Creates a new NDVI renderer based on the widget's state
     */
    QgsRasterRenderer *renderer() override SIP_FACTORY;
    
    /**
     * \brief Set the widget state from the given renderer.
     * \param renderer The renderer to take the state from.
     */
    void setFromRenderer( const QgsRasterRenderer *renderer );
    
    /**
     * Returns the NIR band number
     */
    int nirBand() const { return mNirBandNo; }
    
    /**
     * Returns the Red band number
     */
    int redBand() const { return mRedBandNo; }
    
  public slots:
    /**
     * \brief Set the NIR band
     * \param band The band number
     */
    void setNirBand( int band ) { mNirBandNo = band; }
    
    /**
     * \brief Set the Red band
     * \param band The band number
     */
    void setRedBand( int band ) { mRedBandNo = band; }
    
  private:
    int mNirBandNo;
    int mRedBandNo;
};

#endif // QGSNDVIRENDERERWIDGET_H
