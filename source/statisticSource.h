/*  YUView - YUV player with advanced analytics toolset
*   Copyright (C) 2015  Institut für Nachrichtentechnik
*                       RWTH Aachen University, GERMANY
*
*   YUView is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   YUView is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with YUView.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STATISTICSOURCE_H
#define STATISTICSOURCE_H

#include <QPixmap>
#include <QList>
#include <QCheckBox>
#include "typedef.h"
#include "statisticsExtensions.h"

#include "ui_statisticSource.h"

typedef QList<StatisticsItem> StatisticsItemList;
typedef QVector<StatisticsType> StatisticsTypeList;

/** Virtual class.
* The Statistics source can be anything that provides statistics data. Every statistics source should provide
*  functions for getting 
*/

class statisticSource : public QObject, private Ui_statisticSource
{
  Q_OBJECT

public:
  statisticSource();
  virtual ~statisticSource();

  ValuePairList getValuesAt(int x, int y);
      
  // Get the list of all statistics that this source can provide
  StatisticsTypeList getStatisticsTypeList() { return statsTypeList; }
  // Set the attributes of the statistics that this source can provide (rendered, drawGrid...)
  bool setStatisticsTypeList(StatisticsTypeList typeList);
  // Return true if any of the statistics are actually rendered
  bool anyStatisticsRendered();

  void addPropertiesWidget(QWidget *widget);
 
  // Get the statistics with the given frameIdx/typeIdx.
  // Check cache first, if not load by calling loadStatisticToCache.
  StatisticsItemList getStatistics(int frameIdx, int typeIdx);

  // The statistic with the given frameIdx/typeIdx could not be found in the cache.
  // Load it to the cache. This has to be handeled by the child classes.
  //virtual void loadStatisticToCache(int frameIdx, int typeIdx) = 0;

  // Draw the given list of statistics to the painter
  void paintStatistics(QPainter *painter, StatisticsItemList statsList, StatisticsType statsType, int zoomFactor);

  // Get the statisticsType with the given typeID from p_statsTypeList 
  StatisticsType* getStatisticsType(int typeID);

  // When at some point in time (after the file has been created) it is dicovered that the start and end frame can have 
  void updateStartEndFrameLimit( indexRange limit );

  int lastFrameIdx;
  QSize statFrameSize;
  double frameRate;
  int sampling;

  // The set start and end frame and the limits (lower and upper)
  indexRange startEndFrame;
  indexRange startEndFrameLimit;
  // True if the user changed the start/end frame. In this case we don't update the spin boxes if updateStartEndFrameLimit is called
  bool startEndFrameChanged; 
  
  // The list of all statistics that this class can provide
  StatisticsTypeList statsTypeList;

  QHash< int, QHash< int, StatisticsItemList > > statsCache; // 2D map of type StatisticsItemList with indexing: [POC][statsTypeID]
  
signals:
  // Update the item (and maybe redraw it)
  void updateItem(bool redraw);

private:

  // Pointers to the controls that we added to the properties panel per item
  QList<QCheckBox*> itemNameCheckBoxes;
  QList<QSlider*>   itemOpacitySliders;
  QList<QCheckBox*> itemGridCheckBoxes;

  // Have the controls been created yet?
  bool controlsCreated;

private slots:

  // The slot for the start/end frame spin boxes
  void onSpinBoxChanged();

  // This slot is toggeled whenever one of the controls for the statistics is changed
  void onStatisticsControlChanged();


};

#endif