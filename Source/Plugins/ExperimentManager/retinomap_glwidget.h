#ifndef RETINOMAP_GLWIDGET_H
#define RETINOMAP_GLWIDGET_H

#include "GLWidgetWrapper.h"

#define RETINOMAP_WIDGET_CYCLEDURATION_TR	"duration of one cycle in TR"
#define RETINOMAP_WIDGET_WEDGESPAN_DEGREES	"wedge span in degrees"
#define RETINOMAP_WIDGET_CHECK_AMOUNT		"nr of checks"
#define RETINOMAP_WIDGET_RING_AMOUNT		"nr of rings"
#define RETINOMAP_WIDGET_ROTATION_DIRECTION	"rotation direction"
#define RETINOMAP_WIDGET_FLICKRSPEED_HZ		"flickr speed in Hz"
#define RETINOMAP_WIDGET_XPIXEL_AMOUNT		"stimulus pixel no x"
#define RETINOMAP_WIDGET_YPIXEL_AMOUNT		"stimulus pixel no y"
#define RETINOMAP_WIDGET_CORTMAG_FACTOR		"cortical mag factor"

class RetinoMap_glwidget : public GLWidgetWrapper
{
	Q_OBJECT

public:
	RetinoMap_glwidget(QWidget *parent = NULL);
	~RetinoMap_glwidget();
	RetinoMap_glwidget(const RetinoMap_glwidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

public slots:
	bool start();
	bool stop();
	bool setBlockTrialDomNodeList(QDomNodeList *pDomNodeList);

protected:
	//void init();
	void initBlockTrial();
	//bool loadBlockTrial();
	void paintEvent(QPaintEvent *event);

private:
	void initialize();

	QBrush background;
	//QBrush circleBrush;
	QFont textFont;
	//QPen circlePen;
	QPen textPen;
	QString textContent;

public:
	QTime trialTime;
	int flickr_speed;
	int flickr_switch;
	int xwidth;
	int ywidth;
	float xstart_rel;
	float ystart_rel;
	float startAngle;
	float cycle_dur;
	float TR;
	float x_size_stim;
	float y_size_stim;
	float wedge_deg;
	float wedge_nr_checks;
	float wedge_nr_rings;
	int rot_dir;
	float cort_mag_factor;
	int cent_gap;
	int draw_page;
	int disp_page;
	int nFlickSpeed;
	float xWedge;
	float yWedge;
	float cur_ysize;
	float cur_xpt;
	float cur_ypt;
	float startAngle_temp;
	QColor fix_color;
	int fix_width;
};
Q_DECLARE_METATYPE(RetinoMap_glwidget)
#endif // RETINOMAP_GLWIDGET_H
