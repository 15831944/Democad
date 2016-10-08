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

#pragma once
//#ifndef RS_SNAPPER_H
//#define RS_SNAPPER_H

#include "rs_entitycontainer.h"
#include "rs.h"
#include "rs_coordinateevent.h"

class RS_Entity;
class RS_GraphicView;
class RS_Vector;
class RS_Preview;
class QMouseEvent;


/*
    ��־��ǰ�Ĳ�׽ģʽ
*/
struct RS_SnapMode {
public:
    bool snapFree;     //���ɲ�׽
    bool snapGrid;     //��׽�����
    bool snapEndpoint;     //��׽�˵�
    bool snapMiddle;       //��׽�е�
    bool snapDistance;       //��׽�Ⱦ��
    bool snapCenter;       //��׽���ĵ�
    bool snapIntersection; //��׽����
    bool snapOnEntity;     //��׽ʵ���ϵĵ㣬����ʵ�����ƶ�

    RS2::SnapRestriction restriction; //���ɲ�׽ʱ�õ���Լ��

    //��׽����
    double distance; /// The distance to snap before defaulting to free snaping.

    RS_SnapMode() { hardReset(); }

    RS_SnapMode &clear(void) {
        snapFree     = false;
        snapGrid     = false;
        snapEndpoint     = false;
        snapMiddle       = false;
        snapDistance       = false;
        snapCenter       = false;
        snapOnEntity     = false;
        snapIntersection = false;
        restriction = RS2::RestrictNothing;
        return *this;
    }

    RS_SnapMode &hardReset(void) {//��λ��Ĭ�ϲ�׽ģʽ
        snapFree     = false;
        snapGrid     = false;
        snapEndpoint     = false;
        snapMiddle       = false;
        snapDistance       = false;
        snapCenter       = false;
        snapOnEntity     = false;
        snapIntersection = false;
        restriction = RS2::RestrictNothing;
        distance = 5;
        return *this;
    }
};


/*
    ������graphic view���沶׽��ĺ�������action�ӿڼ̳�
*/
class RS_Snapper {
public:
    RS_Snapper(RS_EntityContainer& container, RS_GraphicView& graphicView);
    virtual ~RS_Snapper();

    void init();
        void finish();

    /**
     * @return Pointer to the entity which was the key entity for the
     * last successful snapping action. If the snap mode is "end point"
     * the key entity is the entity whos end point was caught.
     * If the snap mode didn't require an entity (e.g. free, grid) this
     * method will return NULL.
     */
    RS_Entity* getKeyEntity() {
        return keyEntity;
    }

    /** Sets a new snap mode. */
    void setSnapMode(const RS_SnapMode& snapMode);

    RS_SnapMode *getSnapMode(void) {
        return &this->snapMode;
    }
    /** Sets a new snap restriction. */
    void setSnapRestriction(RS2::SnapRestriction /*snapRes*/) {
        //this->snapRes = snapRes;
    }

        /**
         * Sets the snap range in pixels for catchEntity().
         *
         * @see catchEntity()
         */
        void setSnapRange(int r) {
                snapRange = r;
        }

        /**manually set snapPoint*/
    RS_Vector snapPoint(const RS_Vector& coord, bool setSpot = false);
    RS_Vector snapPoint(QMouseEvent* e);
    RS_Vector snapFree(QMouseEvent* e);

    RS_Vector snapFree(const RS_Vector& coord);
    RS_Vector snapGrid(const RS_Vector& coord);
    RS_Vector snapEndpoint(const RS_Vector& coord);
    RS_Vector snapOnEntity(const RS_Vector& coord);
    RS_Vector snapCenter(const RS_Vector& coord);
    RS_Vector snapMiddle(const RS_Vector& coord);
    RS_Vector snapDist(const RS_Vector& coord);
    RS_Vector snapIntersection(const RS_Vector& coord);
    //RS_Vector snapDirect(RS_Vector coord, bool abs);

//    RS_Vector restrictOrthogonal(const RS_Vector& coord);//����ֱ��
//    RS_Vector restrictHorizontal(const RS_Vector& coord);//ˮƽ
//    RS_Vector restrictVertical(const RS_Vector& coord);//��ֱ


    //RS_Entity* catchLeafEntity(const RS_Vector& pos);
    //RS_Entity* catchLeafEntity(QMouseEvent* e);
    RS_Entity* catchEntity(const RS_Vector& pos,
                           RS2::ResolveLevel level=RS2::ResolveNone);
    RS_Entity* catchEntity(QMouseEvent* e,
                           RS2::ResolveLevel level=RS2::ResolveNone);
    // catch Entity closest to pos and of the given entity type of enType, only search for a particular entity type
    RS_Entity* catchEntity(const RS_Vector& pos, RS2::EntityType enType,
                           RS2::ResolveLevel level=RS2::ResolveNone);
    RS_Entity* catchEntity(QMouseEvent* e, RS2::EntityType enType,
                           RS2::ResolveLevel level=RS2::ResolveNone);
    RS_Entity* catchEntity(QMouseEvent* e, const QVector<RS2::EntityType>& enTypeList,
                           RS2::ResolveLevel level=RS2::ResolveNone);

    /**
     * Suspends this snapper while another action takes place.
     */
    virtual void suspend() {
                // RVT Don't delete the snapper here!
        // RVT_PORT (can be deleted)();
        snapSpot = snapCoord = RS_Vector(false);
    }

    /**
     * Resumes this snapper after it has been suspended.
     */
    virtual void resume() {
        drawSnapper();
    }

    virtual void hideOptions();
    virtual void showOptions();

    void drawSnapper();
    static unsigned int snapModeToInt(const RS_SnapMode& s);
    static RS_SnapMode intToSnapMode(unsigned int);

	RS_GraphicView* getGraphicView() {
		return graphicView;
	}

protected:
    void deleteSnapper();
    double getSnapRange() const;
    RS_EntityContainer* container;//��������ʵ������������ڲ�׽
    RS_GraphicView* graphicView;//��ͼ
    RS_Entity* keyEntity;
    RS_Vector snapCoord;//����
    RS_Vector snapSpot;//��׽��,���ڱ���һ���ƶ�ʱ��׽�ĵ�
    RS_SnapMode snapMode;//��׽ģʽ
    //RS2::SnapRestriction snapRes;
    /**
     * Snap distance for snaping to points with a
     * given distance from endpoints.
     */
    double distance;
    /**
     * Snap to equidistant middle points
     * default to 1, i.e., equidistant to start/end points
     */
    int middlePoints;
        /**
         * Snap range for catching entities.
         */
        int snapRange;//��׽��Χ
        /**
         * Show large cross hairs.
         */
        bool showCrosshairs;//��־�Ƿ���ʾʮ��
        bool finished;
};

//#endif
//EOF
