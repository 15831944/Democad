/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/


#ifndef RS_ENTITY_H
#define RS_ENTITY_H

#include <memory>
#include <QMultiHash>

#include "rs_math.h"//����boost��ͷ�ļ��������ǲ���Ҫ��
#include "rs_pen.h"
#include "rs_undoable.h"
#include "rs_vector.h"//һЩ����3άʸ���Ĳ���

//��������һЩ����ʵ��Ĳ��������� <<������������Ҫ������Щ����ʵ����
class RS_Arc;
class RS_Block;
class RS_Circle;
class RS_Document;
class RS_EntityContainer;
class RS_Graphic;
class RS_GraphicView;
class RS_Insert;
class RS_Line;
class RS_SplineData;
class RS_Spline;
class RS_Painter;
class RS_Point;
class RS_Polyline;
class RS_Text;
class RS_Layer;
class LC_Quadratic;
class RS_GraphicView;
class RS_LineData;
class RS_ArcData;

#include "structGraphM.h"


/**
 * Base class for an entity (line, arc, circle, ...)
 *
 * @author Andrew Mustun
 */
/*
����ʵ��Ļ���
�̳���RS_Undoable������������ά�������ʵ����صģ�undo����
*/
class RS_Entity : public RS_Undoable {
public:

    //���븸����
    RS_Entity(RS_EntityContainer* parent=NULL);
    virtual ~RS_Entity();

	//add by longmm==================
#pragma warning(push)
#pragma warning(disable:4100)
	virtual void initWithCamData(const CAMData &dt){}
	virtual CAMData getCamData() {return CAMData();}
	virtual bool createMidLeadLineData(const RS_Vector& start, RS_LineData *dt ){return false;}
	virtual bool createLeadLineData(const RS_Vector& coord, RS_LineData *dt){return false;}
	virtual bool createLeadLineSideData(const RS_Vector& start, const RS_Vector& mouse,  RS_LineData *dt){return false;}

	virtual int createLeadArcLineData(const RS_Vector& start, const RS_Vector& mouse, double rd, bool tp,
		RS_LineData *dt1, RS_LineData *dt2, RS_ArcData *arcDt) {return false;}
#pragma warning(pop)
	//=================

    void init();
    virtual void initId(long int iid = -1);

	virtual bool isReversed() const{
		return false;
	};

	virtual RS_Entity* clone() = 0;

	virtual void reparent(RS_EntityContainer* parent) {
		this->parent = parent;
	}

    void resetBorders();
    void moveBorders(const RS_Vector& offset){
        minV.move(offset);
        maxV.move(offset);
    }
    void scaleBorders(const RS_Vector& center, const RS_Vector& factor){
        minV.scale(center,factor);
        maxV.scale(center,factor);
    }
    /**
     * Must be overwritten to return the rtti of this entity
     * (e.g. RS2::EntityArc).
     */
    //�豻��д������ʵ���ʵ������
    virtual RS2::EntityType rtti() const {
        return RS2::EntityUnknown;
    }

    /**
     * Identify all entities as undoable entities.
     * @return RS2::UndoableEntity
     */
    //ȷ������ʵ��ʱ����undo���Ե�
    virtual RS2::UndoableType undoRtti() {
        return RS2::UndoableEntity;
    }

    /**
     * @return Unique Id of this entity.
     */
    unsigned long int getId() const {
        return id;
    }

    /**
     * This method must be overwritten in subclasses and return the
     * number of <b>atomic</b> entities in this entity.
     */
    virtual unsigned int count() = 0;

    /**
     * This method must be overwritten in subclasses and return the
     * number of <b>atomic</b> entities in this entity including sub containers.
     */
    virtual unsigned int countDeep() = 0;


    /**
         * Implementations must return the total length of the entity
         * or a negative number if the entity has no length (e.g. a text or hatch).
         */
    virtual double getLength() const {
        return -1.0;
    }

	virtual double getAngleLength() const {
		return -1.0;
	}

    /**
     * @return Parent of this entity or NULL if this is a root entity.
     */
    RS_EntityContainer* getParent() const {
        return parent;
    }

    /**
     * Reparents this entity.
     */
    void setParent(RS_EntityContainer* p) {
        parent = p;
    }
    /** @return The center point (x) of this arc */
    //get center for entities: arc, circle and ellipse
    //���ʵ������ģ�����Բ������
    virtual RS_Vector getCenter() const {
        return RS_Vector(false);
    }
    virtual double getRadius() const {
        return RS_MAXDOUBLE;
    }
	virtual double getAngle1() const
	{
	    return RS_MAXDOUBLE;
	}
	virtual double getAngle2() const
	{
	    return RS_MAXDOUBLE;
	}
//    RS_Graphic* getGraphic();
//    RS_Block* getBlock();
//    RS_Insert* getInsert();
//    RS_Entity* getBlockOrInsert();
    RS_Document* getDocument();

//    void setLayer(const QString& name);
//    void setLayer(RS_Layer* l);
//    void setLayerToActive();
//    RS_Layer* getLayer(bool resolve = true) const;

    /**
     * Sets the explicit pen for this entity or a pen with special
     * attributes such as BY_LAYER, ..
     */
    void setPen(const RS_Pen& pen) {
        this->pen = pen;
    }


    void setPenToActive();
    RS_Pen getPen(bool resolve = true) const;

    /**
     * Must be overwritten to return true if an entity type
     * is a container for other entities (e.g. polyline, group, ...).
     */
    virtual bool isContainer() const = 0;

    /**
     * Must be overwritten to return true if an entity type
     * is an atomic entity.
     */
    virtual bool isAtomic() const = 0;

    /**
     * Must be overwritten to return true if an entity type
     * is a potential edge entity of a contour. By default
    * this returns false.
     */
    virtual bool isEdge() const {
        return false;
    }

    /**
     * @return true for all document entities (e.g. Graphics or Blocks).
     * false otherwise.
     */
    virtual bool isDocument() const {
        return false;
    }

    virtual bool setSelected(bool select);
    virtual bool toggleSelected();
    virtual bool isSelected() const;
    virtual bool isParentSelected();
    virtual bool isProcessed() const;
    virtual void setProcessed(bool on);
    virtual bool isInWindow(RS_Vector v1, RS_Vector v2);//ʵ���Ƿ��ھ��η�Χ��
    virtual bool hasEndpointsWithinWindow(const RS_Vector& /*v1*/, const RS_Vector& /*v2*/) {
        return false;
    }
    virtual bool isVisible();
    virtual void setVisible(bool v) {
        if (v) {
            setFlag(RS2::FlagVisible);
        } else {
            delFlag(RS2::FlagVisible);
        }
    }
    virtual void setHighlighted(bool on);
    virtual bool isHighlighted();

    virtual bool isLocked();

    virtual void undoStateChanged(bool undone);
    virtual bool isUndone() const;

    /**
     * Can be implemented by child classes to update the entities
     * temporary subentities. update() is called if the entity's
     * paramters or undo state changed.
     */
    virtual void update() {}

    virtual void setUpdateEnabled(bool on) {
        updateEnabled = on;
    }

    /**
     * This method doesn't do any calculations.
     * @return minimum coordinate of the entity.
     * @see calculateBorders()
     */
    RS_Vector getMin() const {
        return minV;
    }

    /**
     * This method doesn't do any calculations.
     * @return minimum coordinate of the entity.
     * @see calculateBorders()
     */
    RS_Vector getMax() const {
        return maxV;
    }

    /**
     * This method returns the difference of max and min returned
     * by the above functions.
     * @return size of the entity.
     * @see calculateBorders()
     * @see getMin()
     * @see getMax()
     */
    RS_Vector getSize() const {
        return maxV-minV;
    }

//    void addGraphicVariable(const QString& key, double val, int code);
//    void addGraphicVariable(const QString& key, int val, int code);
//    void addGraphicVariable(const QString& key, const QString& val, int code);

//    double getGraphicVariableDouble(const QString& key, double def);
//    int getGraphicVariableInt(const QString& key, int def);
//    QString getGraphicVariableString(const QString& key,
//                                     const QString& def);
    virtual RS_Vector getStartpoint() const {
        return RS_Vector(false);
    }
    virtual RS_Vector getEndpoint() const {
        return RS_Vector(false);
    }
    //find the local direction at end points, derived entities
    // must implement this if direction is supported by the entity type
    //�ҵ��˵�ֲ�����
    virtual double getDirection1() const {
        return 0.;
    }
    virtual double getDirection2() const {
        return 0.;
    }

    //find the tangential points seeing from given point
    //�ҵ��Ӵ���㣬����ʵ����е�
    virtual RS_VectorSolutions getTangentPoint(const RS_Vector& /*point*/) const {
        return RS_VectorSolutions();
    }
    virtual RS_Vector getTangentDirection(const RS_Vector& /*point*/)const{//���߷���
        return RS_Vector(false);
    }
    //RS2::Unit getGraphicUnit();//��ȡ��λ����ʱ��֪������Ҫ

    /**
     * Must be overwritten to get all reference points of the entity.
     */
    virtual RS_VectorSolutions getRefPoints() {
        RS_VectorSolutions ret;
        return ret;
    }


    /**
     * Must be overwritten to get the closest endpoint to the
     * given coordinate for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest endpoint. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest endpoint.
     */
    //���������ҵ�����˵�
    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL)const = 0;

	 /**
     * Must be overwritten to get the closest endpoint to the
     * given coordinate for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest endpoint. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest endpoint.
     */
    //���������ҵ���Զ�˵�
    virtual RS_Vector getFarthestEndpoint(const RS_Vector& coord,
		double* dist = NULL)const{ return RS_Vector(false);};

    /**
     * Must be overwritten to get the closest coordinate to the
    * given coordinate which is on this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between \p coord and the point. The passed pointer can
     * also be \p NULL in which case the distance will be lost.
     *
     * @return The closest coordinate.
     */
    //����һ�㣬����ʵ��������������
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& /*coord*/,
                                              bool onEntity = true, double* dist = NULL,
                                              RS_Entity** entity = NULL) const = 0;

	/**
     * Must be overwritten to get the farthest coordinate to the
    * given coordinate which is on this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between \p coord and the point. The passed pointer can
     * also be \p NULL in which case the distance will be lost.
     *
     * @return The closest coordinate.
     */
    //����һ�㣬����ʵ�����������Զ��
    virtual RS_Vector getFarthestPointOnEntity(const RS_Vector& /*coord*/,
                                              bool onEntity = true, double* dist = NULL,
											  RS_Entity** entity = NULL) const{return RS_Vector(false);};

    /**
     * Must be overwritten to get the (nearest) center point to the
     * given coordinate for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest center point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest center point.
     */
    //�ҵ�������ĵ�
    //dist���������е�㵽coord�ľ���
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL) = 0;

    /**
     * Must be overwritten to get the (nearest) middle point to the
     * given coordinate for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest middle point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest middle point.
     */
    //�е�
    virtual RS_Vector getMiddlePoint(void)const{
        return RS_Vector(false);
    }
    //����е�
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL,
                                       int middlePoints = 1
            ) const
	{
		return RS_Vector( false );
	}

    /**
     * Must be overwritten to get the nearest point with a given
     * distance to the endpoint to the given coordinate for this entity.
     *
     * @param distance Distance to endpoint.
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL) = 0;

    /**
     * Must be overwritten to get the point with a given
     * distance to the start- or endpoint to the given coordinate for this entity.
     *
     * @param distance Distance to endpoint.
     * @param startp true = measured from Startpoint, false = measured from Endpoint
     *
     * @return The point with the given distance to the start- or endpoint.
     */
    virtual RS_Vector getNearestDist(double /*distance*/,
                                     bool /*startp*/) {
        return RS_Vector(false);
    }

    /**
     * Must be overwritten to get the nearest reference point for this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual RS_Vector getNearestRef(const RS_Vector& coord,
                                    double* dist = NULL) {
        RS_VectorSolutions s = getRefPoints();

        return s.getClosest(coord, dist);
    }

    /**
     * Gets the nearest reference point of this entity if it is selected.
         * Containers re-implement this method to return the nearest reference
         * point of a selected sub entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual RS_Vector getNearestSelectedRef(const RS_Vector& coord,
                                            double* dist = NULL) {
        if (isSelected()) {
            return getNearestRef(coord, dist);
        }
        else {
            return RS_Vector(false);
        }
    }

    /**
     * Must be overwritten to get the shortest distance between this
     * entity and a coordinate.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param entity Pointer which will contain the (sub-)entity which is
     *               closest to the given point or NULL if the caller is not
     *               interested in this information.
     * @param level The resolve level.
     *
     * @sa RS2::ResolveLevel
     *
     * @return The measured distance between \p coord and the entity.
     */
    virtual RS_Vector getNearestOrthTan(const RS_Vector& /*coord*/,
                                        const RS_Line& /*normal*/,
                                        bool onEntity = false);
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity = NULL,
                                      RS2::ResolveLevel level = RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE) const = 0;

    virtual bool isPointOnEntity(const RS_Vector& coord,
                                 double tolerance=RS_TOLERANCE) const;

    /**
     * Implementations must offset the entity by the given direction and distance.
     */
    virtual bool offset(const RS_Vector& /*coord*/, const double& /*distance*/) {return false;}

    /**
     * Implementations must offset the entity by the distance at both directions
     * used to generate tangential circles
     */
    virtual QVector<RS_Entity* > offsetTwoSides(const double& /*distance*/) const
    {
        return QVector<RS_Entity* >();
    }
    /**
          * implementations must revert the direction of an atomic entity
          */
    virtual void revertDirection(){}
    /**
     * Implementations must move the entity by the given vector.
     */
    virtual void move(const RS_Vector& offset) = 0;

    /**
     * Implementations must rotate the entity by the given angle around
     * the given center.
     */
    virtual void rotate(const RS_Vector& center, const double& angle) = 0;
    virtual void rotate(const RS_Vector& center, const RS_Vector& angleVector) = 0;

    /**
     * Implementations must scale the entity by the given factors.
     */
    virtual void scale(const RS_Vector& center, const RS_Vector& factor) = 0;

    /**
     * Acts like scale(RS_Vector) but with equal factors.
     * Equal to scale(center, RS_Vector(factor, factor)).
     */
    virtual void scale(const RS_Vector& center, const double& factor) {
        scale(center, RS_Vector(factor, factor));
    }
    virtual void scale(const RS_Vector& factor) {
        scale(RS_Vector(0.,0.), factor);
    }
    /**
     * Implementations must mirror the entity by the given axis.
     */
    //�������߽��з���
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) = 0;

    //��չ
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);

    /**
         * Implementations must drag the reference point(s) of all
         * (sub-)entities that are very close to ref by offset.
         */
    virtual void moveRef(const RS_Vector& /*ref*/,
                         const RS_Vector& /*offset*/) {
        return;
    }

    /**
         * Implementations must drag the reference point(s) of selected
         * (sub-)entities that are very close to ref by offset.
         */
    virtual void moveSelectedRef(const RS_Vector& /*ref*/,
                                 const RS_Vector& /*offset*/) {
        return;
    }


    /** whether the entity's bounding box intersects with visible portion of graphic view */
    //ʵ��ı߽���Ƿ�Ϳ��ӷ�Χ�ཻ����ʵ���Ƿ�ɼ�
    virtual bool isVisibleInWindow(RS_GraphicView* view) const;
    /**
     * Implementations must draw the entity on the given device.
     */
    //�ڸ��������ϻ���ʵ��
    virtual void draw(RS_Painter* painter, RS_GraphicView* view,
                      double& patternOffset ) = 0;

    //����һ���������ӣ�ò�ƺʹ�ӡ�й�
    double getStyleFactor(RS_GraphicView* view);

    QString getUserDefVar(const QString& key) const;
    QList<QString> getAllKeys();
    void setUserDefVar(QString key, QString val);
    void delUserDefVar(QString key);

    friend std::ostream& operator << (std::ostream& os, RS_Entity& e);

    /** Recalculates the borders of this entity. */
    virtual void calculateBorders() = 0;
    /** whether the entity is on a helpLayer */
    //! helpLayer contains entities of infinite length, helpLayer doesn't show up in print
    //bool isHelpLayer(bool typeCheck = false) const; // ignore certain entity types for helpLayer check

    /** return the equation of the entity
    for quadratic,
    return a vector contains:
    m0 x^2 + m1 xy + m2 y^2 + m3 x + m4 y + m5 =0 //һ��ͨʽ
    for linear:
    m0 x + m1 y + m2 =0
    **/
    //����ʵ��ͼ�εķ���ʽ
    virtual LC_Quadratic getQuadratic() const;

	void addRelativeEntity(RS_Entity *et) {
		if (et == NULL) {
			return;
		}

		relativeEntitys.append(et);
	}

	//������ʾ�Ƿ��ͷ�����ڴ�
	void clearAllRelativeEntity(bool del = false) {
		if (del) {
			while (!relativeEntitys.isEmpty())
				delete relativeEntitys.takeFirst();
		} else
			relativeEntitys.clear();
	}

	QListIterator<RS_Entity*> createRelativeIterator() {
		return QListIterator<RS_Entity*>(relativeEntitys);
	}


	bool isLead() {
		return lead;
	}

	void setLead(bool ld) {
		lead= ld;
	}

	RS2::LeadType leadType() {
		return m_leadType;
	}

	void setLeadType(RS2::LeadType tp) {
		m_leadType = tp;
	}

	void setLeadLinkEntity(RS_Entity *et) {
		m_pLeadLinkEntity = et;
	}

	RS_Entity * leadLinkEntity() {
		return m_pLeadLinkEntity;
	}

	void SetLeadLinkFlag(bool bFlag)
	{
		m_bLeadLink = bFlag;
	}

	bool GetLeadLinkFlag()
	{
		return m_bLeadLink;
	}

	//Begin Modify @zhang.y.q
	void setGraphNum(unsigned int mum){
		m_graphNum=mum;
	}
	unsigned int getGraphNum(){
		return m_graphNum;
	}
	QList<RS_Entity*>& getRelativeEntitys(){
		return relativeEntitys;
	}
	RS_Entity* getLeadLinkEntity(){
		return m_pLeadLinkEntity;
	}
// 	void setEntityType(QString tp){
// 		m_entityType=tp;
// 	}
// 	QString getEntityType(){
// 		return m_entityType;
// 	}
	//End Modify @zhang.y.q

protected:
    RS_EntityContainer* parent;//ʵ��ĸ��ף���������ʵ������

    //������������ʵ��ı߽磬�������ʵ���Ƿ�ɼ�ʱ�õ�
    RS_Vector minV;//ʵ����ռ��С����ֵ
    RS_Vector maxV;//�������ֵ

    //RS_Layer* layer;//ʵ�������Ĳ�
    unsigned long int id;//ʵ��Ψһ��id
    RS_Pen pen;//�����ʵ��Ļ�������
    bool updateEnabled;//�Ƿ��Զ�����

private:
	QList<RS_Entity*> relativeEntitys;
    QMultiHash<QString, QString> varList;
	bool lead;//��־�Ƿ�Ϊ����
	RS2::LeadType m_leadType;
	RS_Entity *m_pLeadLinkEntity;

	// add by zhou.g.x�޸�һ��ʵ���Ͽ������������ߵ�bug
	bool m_bLeadLink;			// ��ʵ���Ƿ���������������ʵ��

	unsigned int m_graphNum;//��ʾ�������һ�������ͼ�ε� 0��ʾ�����ڷ��ͼ�� @zhang.y.q
//	QString m_entityType;//��ʾһ��ʵ�������
};

#endif
