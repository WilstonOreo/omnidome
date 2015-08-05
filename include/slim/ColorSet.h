#ifndef SLIM_COLORSET_H_
#define SLIM_COLORSET_H_

#include <QWidget>
#include <QPalette>

namespace slim
{
  /// Simplies color access from a palette for a widget
  struct ColorSet
  {
    /**@brief Default constructor
       @param _parent Pointer to parent widget 
     **/
    ColorSet(QWidget* _parent);

    QColor color(QPalette::ColorRole) const;
    QPalette::ColorGroup colorGroup() const;

    /// Window aka Background color
    QColor window() const;

    /// Window text aka Foreground color
    QColor windowText() const;

    /**@brief Base color 
     * @param _alternate Returns alternate color (used for alternted rows)
     **/
    QColor base(bool _alternate = false) const;

    /// Button color (ColorRole = QPalette::Button)
    QColor button() const;

    /// Tooltip color (ColorRole = QPalette::ToolTipBase)
    QColor toolTipBase() const;
    
    /// Tooltip text color (ColorRole = QPalette::ToolTipText)
    QColor toolTipText() const;

    /// Text color (ColorRole = QPalette::Text)
    QColor text() const;

    /// Button text color (ColorRole = QPalette::ButtonText) 
    QColor buttonText() const;

    /// Bright text color (ColorRole = QPalette::BrightText)
    QColor brightText() const;

    /// Light color (ColorRole = QPalette::Light)
    QColor light() const;
    
    /// Mid light color (ColorRole = QPalette::Midlight)
    QColor midlight() const;

    /// Dark color (ColorRole = QPalette::Dark)
    QColor dark() const;

    /// Shadow color (ColorRole = QPalette::Shadow)
    QColor shadow() const;

    /// Highlight color (ColorRole = QPalette::Highlight)
    QColor highlight() const;
    
    /// HighlightText color (ColorRole = QPalette::HighlightText)
    QColor highlightedText() const;

    /// Link color (ColorRole = QPalette::Link)
    QColor link() const;
    
    /// Visited link color (ColorRole = QPalette::LinkVisited)
    QColor linkVisited() const;

  private:
    /// Pointer to parent widget
    QWidget* parent_;
  };
}

#endif /* SLIM_COLORSET_H_ */
