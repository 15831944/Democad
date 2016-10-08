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

#include "rs_arc.h"
#include <math.h>

#include "rs_constructionline.h"
#include "rs_linetypepattern.h"
#include "rs_information.h"
#include "rs_math.h"
#include "rs_graphicview.h"
#include "rs_painter.h"
#include "lc_quadratic.h"
#include "rs_painterqt.h"

#ifdef EMU_C99
#include "emu_c99.h"
#endif



/**
 * Default constructor.
 */
RS_Arc::RS_Arc(RS_EntityContainer* parent,const RS_ArcData& d) : RS_AtomicEntity(parent), data(d) 
{
    calculateEndpoints();
    calculateBorders();

	if (data.nCADID >= 0) {
		initId(data.nCADID);
	}
}

bool RS_Arc::createMidLeadLineData(const RS_Vector& coord1, RS_LineData *dt )
{
	if (dt == NULL) {
		return false;
	}

	*dt = RS_LineData(coord1, getMiddlePoint());
	return true;
}

bool RS_Arc::createLeadLineData(const RS_Vector& coord, RS_LineData *dt) {
	if (dt == NULL) {
		return false;
	}
	//���Ƶ���Բ�ϵĲ����������Ķ���ȥ�������ﲻ����
	RS_Vector dv;
	RS_Vector interPoint1, interPoint2;
	double ang1, ang2;

	dv = coord - data.center;
	dv = dv * (data.radius / dv.magnitude());
	interPoint1 = data.center + dv;
	interPoint2 = data.center - dv;
	ang1 = (interPoint1 - data.center).angle();
	ang2 = (interPoint2 - data.center).angle();

	if (isAngInRange(ang1) && isAngInRange(ang2)) {
		//ȡ������Ǹ�
		*dt = RS_LineData(coord, interPoint1);
		
	} else if (isAngInRange(ang1)) {

		*dt = RS_LineData(coord, interPoint1);
	} else if (isAngInRange(ang2)) {

		*dt = RS_LineData(coord, interPoint2);
	} else {
		//û�е��ڻ���
		return false;
	}

	return true;

}


bool RS_Arc::createLeadLineSideData(const RS_Vector& start, const RS_Vector& mouse,  RS_LineData *dt) {
	if (dt == NULL) {
		return false;
	} 

	double distToCenter = start.distanceTo(data.center);
	RS_Vector dv;
	if (distToCenter > data.radius) {
		//�ȿ��Ƿ�������Ķ˵�, ������е㣬Ȼ����������Ǹ������ok��
		RS_VectorSolutions allVec;

		dv = getNearestEndpoint(mouse);//�ȼ�������Ϸ��˵�
		RS_Line line(NULL, RS_LineData(start, dv));
		RS_VectorSolutions vec = RS_Information::getIntersection(&line, this, true);
		if (vec.getNumber() == 1)//���洫��true����ʾֻ�߶κͻ�ֻ��һ���㣬���������������ߣ�Ҳ����˵���յ����dv
		{
			allVec.push_back(dv);
		}

		RS_VectorSolutions sol = getTangentPoint(start);//�ټ���Ϸ�����е�
 		if (sol.getNumber() == 0) return false;
		if (0 == allVec.size())
		{
			allVec = sol;
		}
		else
		{
			RS_Vector vp(sol.getClosest(mouse));
			//if (isAngInRange(data.center.angleTo(vp)))
			{
				allVec.push_back(vp);
			}
		}

		if (allVec.size()<2)
		{
			return false;
		}
		RS_Vector rs_Bng = allVec.at(0);
		RS_Vector rs_End = allVec.at(1);

		// �жϼ���������е��Ƿ���ȷ
		if (!judgeTwoPointEqual(rs_Bng.distanceTo(data.center), data.radius, 0.000001)||
			!judgeTwoPointEqual(rs_End.distanceTo(data.center), data.radius, 0.000001))
		{
			return false;
		}

		if ((CheckDirect(rs_Bng, start, rs_End)>0 && CheckDirect(rs_Bng, mouse, rs_End)>0) ||
			(CheckDirect(rs_Bng, start, rs_End)<0 && CheckDirect(rs_Bng, mouse, rs_End)<0))
		{
			dv = getNearestPointOnEntity(mouse);
			*dt = RS_LineData(start, dv);
		}
		else
		{
			if (rs_Bng.distanceTo(mouse) < rs_End.distanceTo(mouse))
			{
				*dt = RS_LineData(start, rs_Bng);
			}
			else
			{
				*dt = RS_LineData(start, rs_End);
			}

		}
		
// 		RS_Vector po(allVec.getClosest(mouse));//��������
// 
// 		if ((po - start).squared() > RS_TOLERANCE2) {
// 			*dt = RS_LineData(start, po);
// 		} else { //��̫���������е㣬��
// 			*dt = RS_LineData(start + getTangentDirection(mouse), start);
// 		}

	} 
	else 
	{
		dv = getNearestPointOnEntity(mouse);
		//dv = getNearestEndpoint(mouse);
		*dt = RS_LineData(start, dv);
// 		RS_Vector rs_Bng = data.getStartPoint();
// 		RS_Vector rs_End = data.getEndPoint();

// 		if ((CheckDirect(rs_Bng, start, rs_End)>0 && CheckDirect(rs_Bng, mouse, rs_End)>0) ||
// 			(CheckDirect(rs_Bng, start, rs_End)<0 && CheckDirect(rs_Bng, mouse, rs_End)<0))
// 		{
// 			RS_Vector dv = getNearestPointOnEntity(mouse);
// 			*dt = RS_LineData(start, dv);
// 		}
// 		else
// 		{
// 			if (rs_Bng.distanceTo(mouse) < rs_End.distanceTo(mouse))
// 			{
// 				*dt = RS_LineData(start, rs_Bng);
// 			}
// 			else
// 			{
// 				*dt = RS_LineData(start, rs_End);
// 			}
// 
 //		}
	}

	return true;
}

//���ߣ�Բ��1 ��Բ�� 2 �ĺ���
// ����ֵ��0, ���ߴ����ɹ�. 1, ��Ϊ�յ��βΣ�2���㲻��ֱ���ϣ� 3��Բ�뾶����̫��;
int RS_Arc::createLeadArcLineData(const RS_Vector& start, const RS_Vector& mouse, double rd, bool tp,
	RS_LineData *dt1, RS_LineData *dt2, RS_ArcData *arcDt)
{

	if (dt1 == NULL || arcDt == NULL || dt2 == NULL) 
	{
		return 1;
	}

	//ע�����Ƶ㲻�ڻ���

	RS_Vector dv, direction;
	RS_Vector interPoint1, interPoint2;
	double ang1, ang2;

	//modified by @wang.k.h
	//������������Բ��Բ���غϣ��������£�
	if (start == data.center)
	{
		dv = mouse - data.center;
	}
	else//���غ�
	{
		dv = start - data.center;
	}//end

	direction = dv.prep();//��������
	//����Բ�ĺ�԰����
	dv = dv * (data.radius / dv.magnitude());
	interPoint1 = data.center + dv;
	interPoint2 = data.center - dv;
	ang1 = (interPoint1 - data.center).angle();
	ang2 = (interPoint2 - data.center).angle();

	double dist2center = start.distanceTo(data.center);


	if (isAngInRange(ang1))
	{

	}
	else if (isAngInRange(ang2))
	{
		interPoint1 = interPoint2;
	} 
	else 
	{
		return 4;		// ����Բ����Χ��
	}
 

	//����԰�⣬��������ڻ���
	if (/*dist2center > data.radius && isAngInRange(ang1)*/true)
	{
		//��԰�Ͻ���
		double dist = start.distanceTo(interPoint1);

		if ((dist - 2 * rd) < RS_TOLERANCE) 
		{ 
			return 3;//Բ�뾶��̫��
		}

	//��Բ
		RS_Vector center = (start - interPoint1) * (rd / dist);
		center = interPoint1 + center;
		RS_Circle circle(NULL, RS_CircleData(center, rd));

		RS_VectorSolutions sol;
		RS_Vector vp;

		//ȷ��ʵ��
		if (tp) 
		{ //Բ��1  б��
			sol = circle.getTangentPoint(start);
			if (sol.getNumber() == 0) return false;
			vp = sol.getClosest(mouse); //�е�

			*dt2 = RS_LineData(start, vp); //���������
			dt1->reset();

		}
		else 
		{ //Բ��2 ��ֱ��
			//RS_Vector direction = (start - data.center).prep();//��������
			direction = direction * (rd / direction.magnitude());
			sol = RS_VectorSolutions(center + direction, center - direction);
			vp = sol.getClosest(mouse);//�������е�

			*dt2 = RS_LineData(vp +(start - center), vp); //���������
			//ȷ��ֱ��1
			*dt1 = RS_LineData(start, dt2->startpoint);
		}

		//ȷ��������
		double extAng1 = center.angleTo(vp);//�е�Ƕ�
		double extAng2 = center.angleTo(interPoint1);//����Ƕ�

		//������ 0 - 2pi
		extAng1 = RS_Math::correctAngleTo2P(extAng1);
		extAng2 = RS_Math::correctAngleTo2P(extAng2);

		//�жϻ���˳  �� ��û�����
		RS_Arc arc(NULL, RS_ArcData(center, rd, extAng1, extAng2, false));
		if (arc.isAngInRange(center.angleTo(start))) 
		{
			*arcDt = RS_ArcData(center, rd, extAng1, extAng2, true);
		}
		else 
		{
			*arcDt = RS_ArcData(center, rd, extAng1, extAng2, false);
		}

		return 0;
	}
	
//==================�������ֱ����ֱ��==================
	if (isAngInRange(ang1) && isAngInRange(ang2))
	{
		//ȡ������Ǹ�
		*dt2 = RS_LineData(start, interPoint1);
	} 
	else if (isAngInRange(ang1)) 
	{
		*dt2 = RS_LineData(start, interPoint1);
	}
	else if (isAngInRange(ang2)) 
	{
		*dt2 = RS_LineData(start, interPoint2);
	}
	else
	{
		//û�е��ڻ���
		return 2;
	}

	arcDt->reset();
	dt1->reset();

	return 0;

}



/**
 * Creates this arc from 3 given points which define the arc line.
 *
 * @param p1 1st point.
 * @param p2 2nd point.
 * @param p3 3rd point.
 */
bool RS_Arc::createFrom3P(const RS_Vector& p1, const RS_Vector& p2,
                          const RS_Vector& p3) {
        RS_Vector vra=p2 - p1;
        RS_Vector vrb=p3 - p1;
        double ra2=vra.squared()*0.5;
        double rb2=vrb.squared()*0.5;
        double crossp=vra.x * vrb.y - vra.y * vrb.x;
        if (fabs(crossp)< RS_TOLERANCE2) {
//                 RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Arc::createFrom3P(): "
//                         "Cannot create a arc with radius 0.0.");
                return false;
        }
        crossp=1./crossp;
        data.center.set((ra2*vrb.y - rb2*vra.y)*crossp,(rb2*vra.x - ra2*vrb.x)*crossp);
        data.radius=data.center.magnitude();
        data.center += p1;
        data.angle1=data.center.angleTo(p1);
        data.angle2=data.center.angleTo(p3);
        data.reversed = RS_Math::isAngleBetween(data.center.angleTo(p2),
                                                data.angle1, data.angle2, true);
        return true;
}


/**
 * Creates an arc from its startpoint, endpoint, start direction (angle)
 * and radius.
 *
 * @retval true Successfully created arc
 * @retval false Cannot creats arc (radius to small or endpoint to far away)
 */
//direction ��ʼ�Ƕ� �յ�-�����Ǹ�������x������ļн�
bool RS_Arc::createFrom2PDirectionRadius(const RS_Vector& startPoint,
        const RS_Vector& endPoint,
        double direction1, double radius) {

    RS_Vector ortho;//��������
    ortho.setPolar(radius, direction1 + M_PI/2.0);
    RS_Vector center1 = startPoint + ortho;
    RS_Vector center2 = startPoint - ortho;

    if (center1.distanceTo(endPoint) < center2.distanceTo(endPoint)) {
        data.center = center1;
    } else {
        data.center = center2;
    }

    data.radius = radius;
    data.angle1 = data.center.angleTo(startPoint);
    data.angle2 = data.center.angleTo(endPoint);
    data.reversed = false;

    double diff = RS_Math::correctAngle(getDirection1()-direction1);
    if (fabs(diff-M_PI)<1.0e-1) {
        data.reversed = true;
    }
    calculateEndpoints();
    calculateBorders();

    return true;
}

/**
 * Creates an arc from its startpoint, endpoint, start direction (angle)
 * and angle length.
 *
 * @retval true Successfully created arc
 * @retval false Cannot creats arc (radius to small or endpoint to far away)
 */
bool RS_Arc::createFrom2PDirectionAngle(const RS_Vector& startPoint,
                                        const RS_Vector& endPoint,
                                        double direction1, double angleLength) {
    if( fabs(fmod( angleLength, M_PI))<RS_TOLERANCE_ANGLE ) return false;
    data.radius=0.5*startPoint.distanceTo(endPoint)/sin(0.5*angleLength);

    RS_Vector ortho;
    ortho.setPolar(data.radius, direction1 + M_PI/2.0);
    RS_Vector center1 = startPoint + ortho;
    RS_Vector center2 = startPoint - ortho;

    if (center1.distanceTo(endPoint) < center2.distanceTo(endPoint)) {
        data.center = center1;
    } else {
        data.center = center2;
    }

    data.angle1 = data.center.angleTo(startPoint);
    data.reversed = false;

    double diff = RS_Math::correctAngle(getDirection1()-direction1);
    if (fabs(diff-M_PI)<1.0e-1) {
    data.angle2 = RS_Math::correctAngle(data.angle1 -angleLength);
        data.reversed = true;
    }else{
    data.angle2 = RS_Math::correctAngle(data.angle1 +angleLength);
    }
    calculateEndpoints();
    calculateBorders();

    return true;
}



/**
 * Creates an arc from its startpoint, endpoint and bulge.
 */
bool RS_Arc::createFrom2PBulge(const RS_Vector& startPoint, const RS_Vector& endPoint,
                               double bulge) {
    data.reversed = (bulge<0.0);
    double alpha = atan(bulge)*4.0;

    RS_Vector middle = (startPoint+endPoint)/2.0;
    double dist = startPoint.distanceTo(endPoint)/2.0;

    // alpha can't be 0.0 at this point
    data.radius = fabs(dist / sin(alpha/2.0));

    double wu = fabs(RS_Math::pow(data.radius, 2.0) - RS_Math::pow(dist, 2.0));
    double h = sqrt(wu);
    double angle = startPoint.angleTo(endPoint);

    if (bulge>0.0) {
        angle+=M_PI/2.0;
    } else {
        angle-=M_PI/2.0;
    }

    if (fabs(alpha)>M_PI) {
        h*=-1.0;
    }

    data.center.setPolar(h, angle);
    data.center+=middle;
    data.angle1 = data.center.angleTo(startPoint);
    data.angle2 = data.center.angleTo(endPoint);

    calculateEndpoints();
    calculateBorders();

    return true;
}



/**
 * Recalculates the endpoints using the angles and the radius.
 */
void RS_Arc::calculateEndpoints() {
    startpoint.set(data.center.x + cos(data.angle1) * data.radius,
                   data.center.y + sin(data.angle1) * data.radius);
    endpoint.set(data.center.x + cos(data.angle2) * data.radius,
                 data.center.y + sin(data.angle2) * data.radius);
}


void RS_Arc::calculateBorders() {
    double minX = std::min(startpoint.x, endpoint.x);
    double minY = std::min(startpoint.y, endpoint.y);
    double maxX = std::max(startpoint.x, endpoint.x);
    double maxY = std::max(startpoint.y, endpoint.y);
	//< ++ by liu.y.c 2016.07.14 Ĭ��Բ������XYƽ����
	double minZ = startpoint.z;
	double maxZ = minZ;

    double a1 = isReversed() ? data.angle2 : data.angle1;
    double a2 = isReversed() ? data.angle1 : data.angle2;
    if ( RS_Math::isAngleBetween(0.5*M_PI,a1,a2,false) ) {
        maxY = data.center.y + data.radius;
    }
    if ( RS_Math::isAngleBetween(1.5*M_PI,a1,a2,false) ) {
        minY = data.center.y - data.radius;
    }
    if ( RS_Math::isAngleBetween(M_PI,a1,a2,false) ) {
        minX = data.center.x - data.radius;
    }
    if ( RS_Math::isAngleBetween(0.,a1,a2,false) ) {
        maxX = data.center.x + data.radius;
    }

    minV.set(minX, minY, minZ);
    maxV.set(maxX, maxY, maxZ);
}



RS_VectorSolutions RS_Arc::getRefPoints() {
    RS_VectorSolutions ret(startpoint, endpoint, data.center);
    return ret;
}


RS_Vector RS_Arc::getNearestEndpoint(const RS_Vector& coord, double* dist) const{
    double dist1, dist2;

	RS_Vector start = data.getStartPoint();
	RS_Vector end = data.getEndPoint();
	dist1 = (start-coord).squared();
	dist2 = (end-coord).squared();

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = sqrt(dist2);
        }
         return end;
    } else {
        if (dist!=NULL) {
            *dist = sqrt(dist1);
        }
        return start;
    }

}

RS_Vector RS_Arc::getFarthestEndpoint(const RS_Vector& coord, double* dist) const{
	double dist1, dist2;

	dist1 = (startpoint-coord).squared();
	dist2 = (endpoint-coord).squared();

	if (dist2>dist1) {
		if (dist!=NULL) {
			*dist = sqrt(dist2);
		}
		return endpoint;
	} else {
		if (dist!=NULL) {
			*dist = sqrt(dist1);
		}
		return startpoint;
	}

}


/**
  *find the tangential points from a given point, i.e., the tangent lines should pass
  * the given point and tangential points
  *
  *Author: Dongxu Li
  */
RS_VectorSolutions RS_Arc::getTangentPoint(const RS_Vector& point) const {
    RS_VectorSolutions ret;
    double r2(getRadius()*getRadius());
    if(r2<RS_TOLERANCE2) return ret; //circle too small
    RS_Vector vp(point-getCenter());
    double c2(vp.squared());
    if(c2<r2-getRadius()*2.*RS_TOLERANCE) {
        //inside point, no tangential point
        return ret;
    }
    if(c2>r2+getRadius()*2.*RS_TOLERANCE) {
        //external point
        RS_Vector vp1(-vp.y,vp.x);//���߷���
        vp1*=getRadius()*sqrt(c2-r2)/c2;
        vp *= r2/c2;
        vp += getCenter();
        if(vp1.squared()>RS_TOLERANCE2) {
            ret.push_back(vp+vp1);
            ret.push_back(vp-vp1);
            return ret;
        }
    }
    ret.push_back(point);
    return ret;
}

RS_Vector RS_Arc::getTangentDirection(const RS_Vector& point) const {
    RS_Vector vp(point-getCenter());
//    double c2(vp.squared());
//    if(c2<r2-getRadius()*2.*RS_TOLERANCE) {
//        //inside point, no tangential point
//        return RS_Vector(false);
//    }
    return RS_Vector(-vp.y,vp.x);

}

RS_Vector RS_Arc::getNearestPointOnEntity(const RS_Vector& coord,
        bool onEntity, double* dist, RS_Entity** entity)const {

    RS_Vector vec(false);//���潻��
    if (entity!=NULL) {
        *entity = const_cast<RS_Arc*>(this);
    }

    double angle = (coord-data.center).angle();
    if ( ! onEntity || isAngInRange(angle)) {
        vec.setPolar(data.radius, angle);
        vec+=data.center;
    } else {
            vec=getNearestEndpoint(coord, dist);
			return vec;
    }
    if (dist!=NULL) {
        *dist = (vec - coord).magnitude();
    }

    return vec;
}




RS_Vector RS_Arc::getFarthestPointOnEntity(const RS_Vector& coord,
	bool onEntity, double* dist, RS_Entity** entity)const {

		RS_Vector vec(false);
		if (entity!=NULL) {
			*entity = const_cast<RS_Arc*>(this);
		}

		double angle = (coord-data.center).angle();
		if ( ! onEntity || RS_Math::isAngleBetween(angle,
			data.angle1, data.angle2, isReversed())) {
				vec.setPolar(data.radius, angle);
				vec+=data.center;
		} else {
			vec=getFarthestEndpoint(coord, dist);
		}
		if (dist!=NULL) {
			*dist = fabs((vec-data.center).magnitude()-data.radius);
		}

		return vec;
}



RS_Vector RS_Arc::getNearestCenter(const RS_Vector& coord,
                                   double* dist) {
    if (dist!=NULL) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}

/*
 * get the nearest equidistant middle points
 * @coord, coordinate
 * @middlePoints, number of equidistant middle points
 *
 */

//���ỡ�� ������������е�
RS_Vector RS_Arc::getNearestMiddle(const RS_Vector& coord,
                                   double* dist,
                                   int middlePoints
                                   )const {

    //RS_DEBUG->print("RS_Arc::getNearestMiddle(): begin\n");
        double amin=getAngle1();
        double amax=getAngle2();

        if( !(isnormal(fabs(amin)) || isnormal(fabs(amax)))){
                //whole circle, no middle point
                if(dist != NULL) {
                        *dist=RS_MAXDOUBLE;
                }
                return RS_Vector(false);
        }
        if(isReversed()) {
                std::swap(amin,amax);
        }
        double da=fmod(amax-amin+2.*M_PI, 2.*M_PI);
        if ( da < RS_TOLERANCE ) {
                da= 2.*M_PI; // whole circle
        }
        RS_Vector vp(getNearestPointOnEntity(coord,true,dist));
        double angle=getCenter().angleTo(vp);
        int counts=middlePoints+1;
        int i( static_cast<int>(fmod(angle-amin+2.*M_PI,2.*M_PI)/da*counts+0.5));
        if(!i) i++; // remove end points
        if(i==counts) i--;
        angle=amin + da*(double(i)/double(counts));
        vp.setPolar(getRadius(), angle);
        vp.move(getCenter());

    if (dist!=NULL) {
        *dist = vp.distanceTo(coord);
    }
   // RS_DEBUG->print("RS_Arc::getNearestMiddle(): end\n");
    return vp;
}

RS_Vector RS_Arc::getNearestDist(double distance,
                                 const RS_Vector& coord,
                                 double* dist) {

    if (data.radius<RS_TOLERANCE) {
        if (dist!=NULL) {
            *dist = RS_MAXDOUBLE;
        }
        return RS_Vector(false);
    }

    double aDist = distance / data.radius;
    if (isReversed()) aDist= -aDist;
    double a;
    if(coord.distanceTo(getStartpoint()) < coord.distanceTo(getEndpoint())) {
        a=getAngle1() + aDist;
    }else {
        a=getAngle2() - aDist;
    }


    RS_Vector ret;
    ret.setPolar(data.radius, a);
    ret += getCenter();

    return ret;
}




RS_Vector RS_Arc::getNearestDist(double distance,
                                 bool startp) {

    if (data.radius<RS_TOLERANCE) {
        return RS_Vector(false);
    }

    double a;
    RS_Vector p;
    double aDist = distance / data.radius;

    if (isReversed()) {
        if (startp) {
            a = data.angle1 - aDist;
        } else {
            a = data.angle2 + aDist;
        }
    } else {
        if (startp) {
            a = data.angle1 + aDist;
        } else {
            a = data.angle2 - aDist;
        }
    }

    p.setPolar(data.radius, a);
    p += data.center;

    return p;
}


RS_Vector RS_Arc::getNearestOrthTan(const RS_Vector& coord,
                    const RS_Line& normal,
                    bool onEntity )
{
        if ( !coord.valid ) {
                return RS_Vector(false);
        }
        double angle=normal.getAngle1();
        RS_Vector vp;
        vp.setPolar(getRadius(),angle);
        QList<RS_Vector> sol;
        for(int i=0;i <= 1;i++){
                if(!onEntity ||
                   RS_Math::isAngleBetween(angle,getAngle1(),getAngle2(),isReversed())) {
                if(i){
                sol.append(- vp);
                }else {
                sol.append(vp);
                }
        }
                angle=RS_Math::correctAngle(angle+M_PI);
        }
        switch(sol.count()) {
                case 0:
                        return RS_Vector(false);
                case 2:
                        if( RS_Vector::dotP(sol[1],coord-getCenter())>0.) {
                                vp=sol[1];
                                break;
                        }
                default:
                        vp=sol[0];
        }
        return getCenter()+vp;
}


double RS_Arc::getDistanceToPoint(const RS_Vector& coord,
                                  RS_Entity** entity,
                                  RS2::ResolveLevel,
                                  double) const {
    if (entity!=NULL) {
        *entity = const_cast<RS_Arc*>(this);
    }

    // check endpoints first:
    double dist = coord.distanceTo(getStartpoint());
    if (dist<1.0e-4) {
        return dist;
    }
    dist = coord.distanceTo(getEndpoint());
    if (dist<1.0e-4) {
        return dist;
    }

	if (isAngInRange(data.center.angleTo(coord)))
/*		if (RS_Math::isAngleBetween(data.center.angleTo(coord),
			data.angle1, data.angle2,
			isReversed())) */{

        // RVT 6 Jan 2011 : Added selection by center point of arc
        double dToEdge=fabs((coord-data.center).magnitude() - data.radius);
		return dToEdge;
        /*double dToCenter=data.center.distanceTo(coord);

        if (dToEdge<dToCenter) {
            return dToEdge;
        } else {
            return dToCenter;
        }*/

    } else {
        return RS_MAXDOUBLE;
    }
}



void RS_Arc::moveStartpoint(const RS_Vector& pos) {
    // polyline arcs: move point not angle:
    //if (parent!=NULL && parent->rtti()==RS2::EntityPolyline) {
    double bulge = getBulge();
    if(fabs(bulge - M_PI/2.)<RS_TOLERANCE_ANGLE) return;

    createFrom2PBulge(pos, getEndpoint(), bulge);
    correctAngles(); // make sure angleLength is no more than 2*M_PI
    //}

    // normal arc: move angle1
    /*else {
        data.angle1 = data.center.angleTo(pos);
        calculateEndpoints();
        calculateBorders();
    }*/
}



void RS_Arc::moveEndpoint(const RS_Vector& pos) {
    // polyline arcs: move point not angle:
    //if (parent!=NULL && parent->rtti()==RS2::EntityPolyline) {
    double bulge = getBulge();
    createFrom2PBulge(getStartpoint(), pos, bulge);
    correctAngles(); // make sure angleLength is no more than 2*M_PI
    //}

    // normal arc: move angle1
    /*else {
        data.angle2 = data.center.angleTo(pos);
        calculateEndpoints();
        calculateBorders();
    }*/
}
/**
  * this function creates offset
  *@coord, position indicates the direction of offset
  *@distance, distance of offset
  * return true, if success, otherwise, false
  *
  *Author: Dongxu Li
  */
bool RS_Arc::offset(const RS_Vector& coord, const double& distance) {
    double r0(coord.distanceTo(getCenter()));
    if(r0 > getRadius()){
        //external
        r0 = getRadius()+ fabs(distance);
    }else{
        r0 = getRadius()- fabs(distance);
        if(r0<RS_TOLERANCE) {
            return false;
        }
    }
    setRadius(r0);
    calculateEndpoints();
    calculateBorders();
    return true;
}
QVector<RS_Entity* > RS_Arc::offsetTwoSides(const double& distance) const
{
    QVector<RS_Entity*> ret(0,NULL);
    ret<<new RS_Arc(NULL,RS_ArcData(getCenter(),getRadius()+distance,getAngle1(),getAngle2(),isReversed()));
    if(getRadius()>distance)
    ret<<new RS_Arc(NULL,RS_ArcData(getCenter(),getRadius()-distance,getAngle1(),getAngle2(),isReversed()));
    return ret;
}

/**
      * implementations must revert the direction of an atomic entity
      */
void RS_Arc::revertDirection(){
    std::swap(data.angle1,data.angle2);
    std::swap(startpoint,endpoint);
    data.reversed = ! data.reversed;
}
/**
 * make sure angleLength() is not more than 2*M_PI
 */
void RS_Arc::correctAngles() {
        double *pa1= & data.angle1;
        double *pa2= & data.angle2;
        if (isReversed()) std::swap(pa1,pa2);
        *pa2 = *pa1 + fmod(*pa2 - *pa1, 2.*M_PI);
        if ( fabs(getAngleLength()) < RS_TOLERANCE_ANGLE ) *pa2 += 2.*M_PI;
}

void RS_Arc::trimStartpoint(const RS_Vector& pos) {
    data.angle1 = data.center.angleTo(pos);
    correctAngles(); // make sure angleLength is no more than 2*M_PI
    calculateEndpoints();
    calculateBorders();
}



void RS_Arc::trimEndpoint(const RS_Vector& pos) {
    data.angle2 = data.center.angleTo(pos);
    correctAngles(); // make sure angleLength is no more than 2*M_PI
    calculateEndpoints();
    calculateBorders();
}

/**
  *@ trimCoord, mouse point
  *@  trimPoint, trim to this point
  */
 //����ü��㣬��ѡ���޽�ʵ����������꣬���ݽǶȹ�ϵ
RS2::Ending RS_Arc::getTrimPoint(const RS_Vector& trimCoord,
                                 const RS_Vector& trimPoint) {

    //double angEl = data.center.angleTo(trimPoint);
    double angMouse = data.center.angleTo(trimCoord);
    double angTrim = data.center.angleTo(trimPoint);

    if( RS_Math::isAngleBetween(angMouse , data.angle1, angTrim, isReversed())) {

        return RS2::EndingEnd;
    } else {

        return RS2::EndingStart;
    }
}

 //���뽻�㼯�ϣ���ѡ���޼�ʵ��ʱ�������꣬�����н�����ѡ���ü���
RS_Vector RS_Arc::prepareTrim(const RS_Vector& mousePoint,
                              const RS_VectorSolutions& trimSol) {
//special trimming for ellipse arc
    if( ! trimSol.hasValid() ) return (RS_Vector(false));
    if( trimSol.size() == 1 ) return (trimSol.get(0));
    double aMin=getAngle1();
    double aMax=getAngle2();
    if(isReversed()) qSwap(aMin, aMax);//arc���˵�Ƕ�
    const double angleMouse=getArcAngle(mousePoint);//���λ�ýǶ�
    RS_Vector vp=trimSol.at(0);
    double da=fabs(fmod(getArcAngle(vp) - angleMouse, 2.*M_PI)); //ÿ����������λ�õļн�

    for(size_t i=1; i<trimSol.size(); i++){
        const double da1=fabs(fmod(getArcAngle(trimSol.at(i)) - angleMouse, 2.*M_PI));
        if(da1<da){
            vp=trimSol.at(i);
            da=da1;
        }
    }//�ҵ��н���С���Ǹ�����

    if( da < fabs(fmod(aMin - angleMouse, 2.*M_PI)) && da < fabs(fmod(aMax - angleMouse, 2.*M_PI))) return vp;
    bool revDirection=( da > fabs(fmod(aMin - angleMouse, 2.*M_PI)))?true:false;

    vp=trimSol.at(0);
    da=RS_Math::getAngleDifference(angleMouse, getArcAngle(vp), revDirection);

    for(size_t i=1; i<trimSol.size(); i++){
        const double da1=RS_Math::getAngleDifference(angleMouse, getArcAngle(trimSol.at(i)), revDirection);
        if(da1<da){
            vp=trimSol.at(i);
            da=da1;
        }
    }
    return vp;

}



void RS_Arc::reverse() {
    std::swap(data.angle1,data.angle2);
    data.reversed = !data.reversed;
//    calculateEndpoints();
    std::swap(startpoint,endpoint);
    //reversing the order of start/end doesn't change position
//    calculateBorders();
}


void RS_Arc::move(const RS_Vector& offset) {
    data.center.move(offset);
    startpoint.move(offset);
    endpoint.move(offset);
    moveBorders(offset);
}



void RS_Arc::rotate(const RS_Vector& center, const double& angle) {
    //RS_DEBUG->print("RS_Arc::rotate");
    data.center.rotate(center, angle);
    data.angle1 = RS_Math::correctAngle(data.angle1+angle);
    data.angle2 = RS_Math::correctAngle(data.angle2+angle);
    calculateEndpoints();
    calculateBorders();
    //RS_DEBUG->print("RS_Arc::rotate: OK");
}

void RS_Arc::rotate(const RS_Vector& center, const RS_Vector& angleVector) {
    //RS_DEBUG->print("RS_Arc::rotate");
    data.center.rotate(center, angleVector);
    double angle(angleVector.angle());
    data.angle1 = RS_Math::correctAngle(data.angle1+angle);
    data.angle2 = RS_Math::correctAngle(data.angle2+angle);
    calculateEndpoints();
    calculateBorders();
   // RS_DEBUG->print("RS_Arc::rotate: OK");
}



void RS_Arc::scale(const RS_Vector& center, const RS_Vector& factor) {
    // negative scaling: mirroring
    if (factor.x<0.0) {
        mirror(data.center, data.center + RS_Vector(0.0, 1.0));
        //factor.x*=-1;
    }
    if (factor.y<0.0) {
        mirror(data.center, data.center + RS_Vector(1.0, 0.0));
        //factor.y*=-1;
    }

    data.center.scale(center, factor);
    data.radius *= factor.x;
    data.radius = fabs( data.radius );
//    calculateEndpoints();
    //todo, does this handle negative factors properly?
    startpoint.scale(center,factor);
    endpoint.scale(center,factor);
    scaleBorders(center,factor);
//    calculateBorders();
}



void RS_Arc::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2)
{
    data.center.mirror(axisPoint1, axisPoint2);
    setReversed( ! isReversed() );
    double a= (axisPoint2 - axisPoint1).angle()*2;
    setAngle1(RS_Math::correctAngle(a - getAngle1()));
    setAngle2(RS_Math::correctAngle(a - getAngle2()));
    correctAngles(); // make sure angleLength is no more than 2*M_PI
    calculateEndpoints();
    calculateBorders();
}



void RS_Arc::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    if(fabs(fabs(getAngleLength()-M_PI)-M_PI)<RS_TOLERANCE_ANGLE){
        move(offset);
        return;
    }
    if (ref.distanceTo(startpoint)<1.0e-4) {
        moveStartpoint(startpoint+offset);
    }else if (ref.distanceTo(endpoint)<1.0e-4) {
        moveEndpoint(endpoint+offset);
    }
    correctAngles(); // make sure angleLength is no more than 2*M_PI
}



void RS_Arc::stretch(const RS_Vector& firstCorner,
                     const RS_Vector& secondCorner,
                     const RS_Vector& offset) {

    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
    else {
        if (getStartpoint().isInWindow(firstCorner,
                                       secondCorner)) {
            moveStartpoint(getStartpoint() + offset);
        }
        if (getEndpoint().isInWindow(firstCorner,
                                     secondCorner)) {
            moveEndpoint(getEndpoint() + offset);
        }
    }
    correctAngles(); // make sure angleLength is no more than 2*M_PI
}

/** find the visible part of the arc, and call drawVisible() to draw */
//�ҵ����Ӵ��ڵĲ��֣�Ȼ��drawVisibleȥ����
void RS_Arc::draw(RS_Painter* painter, RS_GraphicView* view,
                  double& patternOffset) 
{
    drawVisible(painter,view,patternOffset);
}

/** directly draw the arc, assuming the whole arc is within visible window */
void RS_Arc::drawVisible(RS_Painter* painter, RS_GraphicView* view,
                  double& patternOffset) 
{
	//qDebug() << "liuyc draw arc!";

    if (painter==NULL || view==NULL) 
	{
        return;
    }
    //visible in grahic view
    if(isVisibleInWindow(view)==false) 
	{
		return;
	}

	RS_Pen oldpen = painter->getPen();

    RS_Vector  cp = view->toGui( getCenter() );
    double     ra = getRadius() * view->getFactor().x;
    double length = getLength() * view->getFactor().x;
    patternOffset -= length;

    // simple style-less lines
	if ( !isSelected() && ( getPen().getLineType()==RS2::SolidLine ) ) 
	{
		painter->drawArc(cp, ra, getAngle1(), getAngle2(), isReversed());

		return;
	}

	//< ѡ��ǰ������Ҫ������
	RS_LineTypePattern * pat;
	if (isSelected()) 
	{
		pat = &patternSelected;
	} 
	else 
	{
		pat = view->getPattern( getPen().getLineType() );
	}

    if ( pat == NULL || ra < 0.5) 
	{//avoid division by zero from small ra
        RS_DEBUG->print(RS_Debug::D_WARNING, "Invalid line pattern, drawing arc using solid line");
        painter->drawArc(cp, ra,
                         getAngle1(),getAngle2(),
                         isReversed());
        return;
    }

//    patternOffset=fmod(patternOffset - length -0.5*pat->totalLength,pat->totalLength)+0.5*pat->totalLength;

    if ( ra < RS_TOLERANCE_ANGLE){
        return;
    }

    // Pen to draw pattern is always solid:
    RS_Pen pen = painter->getPen();
    pen.setLineType( RS2::SolidLine );
    painter->setPen(pen);



    // create scaled pattern:
    QVector<double> da(0);
    double patternSegmentLength(pat->totalLength);
    double ira=1./ra;
    int i(0);          // index counter
    if(pat->num>0) {
        double dpmm=static_cast<RS_PainterQt*>(painter)->getDpmm();
        da.resize(pat->num);
        while(i<pat->num){
            //        da[j] = pat->pattern[i++] * styleFactor;
            //fixme, stylefactor needed
            da[i] =dpmm*(isReversed()? -fabs(pat->pattern[i]):fabs(pat->pattern[i]));
            if( fabs(da[i]) < 1. ) da[i] = (da[i]>=0.)?1.:-1.;
            da[i] *= ira;
            i++;
        }
    }else {
        //invalid pattern

        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Arc::draw(): invalid line pattern\n");
        painter->drawArc(cp,
                         ra,
                         getAngle1(), getAngle2(),
                         isReversed());
        return;
    }

    //    bool done = false;
    double total=fmod(patternOffset-0.5*patternSegmentLength,patternSegmentLength)-0.5*patternSegmentLength;

    double a1(RS_Math::correctAngle(getAngle1()));
    double a2(RS_Math::correctAngle(getAngle2()));

    if(isReversed()) {//always draw from a1 to a2, so, patternOffset is is automatic
        if(a1<a2+RS_TOLERANCE_ANGLE) a2 -= 2.*M_PI;
        total = a1 - total*ira; //in angle
    }else{
        if(a2<a1+RS_TOLERANCE_ANGLE) a2 += 2.*M_PI;
        total = a1 + total*ira; //in angle
    }
    double limit(fabs(a1-a2));
    double t2;
    double a11,a21;

    for(int j=0; fabs(total-a1)<limit ;j=(j+1)%i) {
        t2=total+da[j];

        if(pat->pattern[j]>0.0) {

            if (fabs(t2-a2)<limit) {
                a11=(fabs(total-a2)<limit)?total:a1;
                a21=(fabs(t2-a1)<limit)?t2:a2;
                painter->drawArc(cp, ra,
                                 a11,
                                 a21,
                                 isReversed());
            }
        }
        total=t2;
    }
}



/**
 * @return Middle point of the entity.
 */
RS_Vector RS_Arc::getMiddlePoint() const {
    double a=RS_Math::correctAngleTo2P( getAngle1() );
    double b=RS_Math::correctAngleTo2P( getAngle2() );

    if (isReversed()) {
		if(a > b)
			a =b+ RS_Math::correctAngle(a-b)*0.5;
		else
			a = b+ RS_Math::correctAngle(a-b + M_PI * 2.)*0.5;
    }
	else
	{
		if(a < b)
		{
			a += RS_Math::correctAngle(b-a)*0.5;
		}
		else
		{
			a += RS_Math::correctAngle(b-a + M_PI * 2.)*0.5;
		}
        
    }
    RS_Vector ret(a);
    return getCenter() + ret*getRadius();
}



/**
 * @return Angle length in rad.
 */
//���ػ���
double RS_Arc::getAngleLength() const {
    double ret;
    double a=getAngle1();
    double b=getAngle2();

    if (isReversed()) std::swap(a,b);
    ret = RS_Math::correctAngle(b-a);
    // full circle:
    //����ǶȱϾ���Բ������Ϊ�ǻ�����2*M_PI
    if (fabs(fmod(ret,2.*M_PI))<RS_TOLERANCE_ANGLE) {
        ret = 2*M_PI;
    }

    return ret;
}



/**
 * @return Length of the arc.
 */
double RS_Arc::getLength() const {
    return getAngleLength()*data.radius;
}



/**
 * Gets the arc's bulge (tangens of angle length divided by 4).
 */
double RS_Arc::getBulge() const {
    double bulge = tan(fabs(getAngleLength())/4.0);
    if (isReversed()) {
        bulge*=-1;
    }
    return bulge;
}

/** return the equation of the entity
for quadratic,

return a vector contains:
m0 x^2 + m1 xy + m2 y^2 + m3 x + m4 y + m5 =0

for linear:
m0 x + m1 y + m2 =0
**/
LC_Quadratic RS_Arc::getQuadratic() const
{
    std::vector<double> ce(6,0.);
    ce[0]=1.;
    ce[2]=1.;
    ce[5]=-data.radius*data.radius;
    LC_Quadratic ret(ce);
    ret.move(data.center);
    return ret;
}


/**
 * Dumps the point's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_Arc& a) {
    os << " Arc: " << a.data << "\n";
    return os;
}

/*
* Funciton Name: calAngle() 
*	����Բ������ʼ��Ƕ�
* INPUT��
*	RS_Vector  Point��Բ����ʼ��
*	RS_Vector cPoint��Բ��
* OUTPUT��
*   double�����ػ���
*/
double RS_Arc::calAngle(RS_Vector Point, RS_Vector cPoint)
{
	double r     = sqrt((Point.x-cPoint.x)*(Point.x-cPoint.x) + (Point.y-cPoint.y)*(Point.y-cPoint.y));
	double x0    = cPoint.x + r;
	double y0    = cPoint.y;
	double angle = acos((2*r*r - ((Point.x-x0)*(Point.x-x0) + (Point.y-y0)*(Point.y-y0)))/(2*r*r));

	if (Point.x>=cPoint.x && Point.y>=cPoint.y)				// 1����
	{
		return angle;
	}
	else if (Point.x<=cPoint.x && Point.y>=cPoint.y)		// 2����	
	{
		return angle;
	}
	else if (Point.x<=cPoint.x && Point.y<=cPoint.y)		// 3����
	{
		return (2*3.14159 - angle);
	}
	else if (Point.x>=cPoint.x && Point.y<=cPoint.y)		// 4����
	{
		return (2*3.14159 - angle);
	}

	return 0.0;
}
