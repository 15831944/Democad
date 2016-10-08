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

#ifndef RS_MODIFICATION_H
#define RS_MODIFICATION_H

#include "rs_vector.h"
#include "rs_pen.h"

#include"global_Operator.h" //@zhang.y.q
class RS_AtomicEntity;
class RS_Entity;
class RS_EntityContainer;
class RS_MText;
class RS_Text;
class RS_Polyline;
class RS_Document;
class RS_Graphic;
class RS_GraphicView;

/*��������޸�
    ���磺�ƶ�����ת���ȵȣ���������Ϣ����װ��һ������
*/
/**
 * Holds the data needed for move modifications.
 */
class RS_MoveData {
public:
    int number;//���ڶ��ؿ�������ʾ�������ٴΣ�����ʱ�����һ�ŵ�ͼ��
    bool useCurrentAttributes;
    bool useCurrentLayer;
    RS_Vector offset;//������Լ����ƶ���ƫ��
};


/**
 * Holds the data needed for offset modifications.
 */
class RS_OffsetData {
public:
    int number;
    bool useCurrentAttributes;
    bool useCurrentLayer;
    RS_Vector coord;
    double distance;
};

/**
 * Holds the data needed for rotation modifications.
 */
class RS_RotateData {
public:
    int number;
    bool useCurrentAttributes;
    bool useCurrentLayer;
    RS_Vector center;//��ת����
    double angle;//��ת�Ƕ�
};



/**
 * Holds the data needed for scale modifications.
 */
class RS_ScaleData {
public:
    int number;
    bool useCurrentAttributes;
    bool useCurrentLayer;
    RS_Vector referencePoint;//���Ųο���
    RS_Vector factor;//����ϵ��
};


/**
 * Holds the data needed for mirror modifications.
 */
class RS_MirrorData {//����
public:
    bool copy;
    bool useCurrentAttributes;
    bool useCurrentLayer;
    RS_Vector axisPoint1;//������ĵ�һ����
    RS_Vector axisPoint2;//������ĵڶ�����
};


/**
 * Holds the data needed for move/rotate modifications.
 */
class RS_MoveRotateData {//�ƶ���ѡ��
public:
    int number;
    bool useCurrentAttributes;
    bool useCurrentLayer;
    RS_Vector referencePoint;//�ο���
    RS_Vector offset;//ƫ��
    double angle;//�Ƕ�
};



/**
 * Holds the data needed for rotation around two centers modifications.
 */
//�����ĵ���ת����ʲô��˼
class RS_Rotate2Data {
public:
    int number;
    bool useCurrentAttributes;
    bool useCurrentLayer;
    RS_Vector center1;
    RS_Vector center2;
    double angle1;
    double angle2;
};



/**
 * Holds the data needed for beveling modifications.
 */
//��������
class RS_BevelData {
public:
    double length1;
    double length2;
    bool trim;
};




/**
 * Holds the data needed for rounding modifications.
 */
//Բ������
class RS_RoundData {
public:
    double radius;
    bool trim;
};


/**
 * Holds the data needed for moving reference points.
 */
//�ƶ��ο��������
class RS_MoveRefData {
public:
    RS_Vector ref;
    RS_Vector offset;
};



/**
 * Holds the data needed for changing attributes.
 */
//�ı����Բ��� ������
class RS_AttributesData {
public:
    QString layer;
    RS_Pen pen;
    bool changeLayer;
    bool changeColor;
    bool changeLineType;
    bool changeWidth;
};


/**
 * Holds the data needed for pasting.
 */
//ճ������ ������
class RS_PasteData {
public:
    RS_PasteData(RS_Vector insertionPoint,
                 double factor,
                 double angle,
                 bool asInsert,
                 const QString& blockName) {

        this->insertionPoint = insertionPoint;
        this->factor = factor;
        this->angle = angle;
        this->asInsert = asInsert;
        this->blockName = blockName;
    }

    //! Insertion point.
    RS_Vector insertionPoint;//�����λ��
    //! Scale factor.
    double factor;
    //! Rotation angle.
    double angle;
    //! Paste as an insert rather than individual entities.
    bool asInsert;//��־ճ������һ�� �����롱ʵ��
    //! Name of the block to create or an empty string to assign a new auto name.
    QString blockName;//����
};


/**
 * API Class for manipulating entities.
 * There's no interaction handled in this class.
 *
 * All modifications can be undone / redone if the container
 * is a RS_Graphic.
 *
 * This class is connected to an entity container and
 * can be connected to a graphic view.
 *
 * @author Andrew Mustun
 */
/*
    ���ڲ���ʵ�����
    �����޸ģ����Ա�undone / redone  ����Ҫ������RS_Graphic������
    �ں�һ��RS_EntityContainerָ�룬һ��RS_GraphicViewָ��
*/
class RS_Modification {
public:
    RS_Modification(RS_EntityContainer& entityContainer,
                    RS_GraphicView* graphicView=NULL,
                    bool handleUndo=true);

    void remove();//ɾ������ ok
    void revertDirection();//�ָ�����
    //bool changeAttributes(RS_AttributesData& data);//�ı�����
    //void copy(const RS_Vector& ref, const bool cut);//��������ѡ��ʵ��

public:
//    void copyLayers(RS_Entity* e);//������
//    void copyBlocks(RS_Entity* e);//������
//    void paste(const RS_PasteData& data, RS_Graphic* source=NULL);//���

	QList<RS_Entity*> MangeCloneList(QList<RS_Entity*> addList);
    bool move(RS_MoveData& data, bool isCopy = false);//�ƶ�
    bool rotate(RS_RotateData& data);//��ת
    bool scale(RS_ScaleData& data);//����
    bool mirror(RS_MirrorData& data);//����
    bool moveRotate(RS_MoveRotateData& data);//�ƶ�����ת
    bool rotate2(RS_Rotate2Data& data);//���������ĵ���ת

	bool setNewOrigin(RS_MoveData& data);//ƽ������ϵ
	bool setOriginInitial();//��ʼ������ϵ

	/*��ʾѡ�е������߶�֮��ļн�*/
	double showAngel(const RS_Vector& trimCoord, RS_AtomicEntity* trimEntity,
		const RS_Vector& limitCoord, RS_Entity* limitEntity);

    //�ü�
	RS_AtomicEntity* trimHandle(const RS_Vector& trimCoord,
		RS_AtomicEntity* trimEntity,
		RS_VectorSolutions sol);//added by wang.k.h
	RS_Arc* trimHandleCircle(const RS_Vector& trimCoord,
		RS_AtomicEntity* trimEntity,
		RS_VectorSolutions sol);//added by wang.k.h
    int trim(const RS_Vector& trimCoord, RS_AtomicEntity* trimEntity,
              const RS_Vector& limitCoord, RS_Entity* limitEntity,
              bool both);
    bool trimAmount(const RS_Vector& trimCoord, RS_AtomicEntity* trimEntity,
                    double dist);//�����ü�����ʱ����
    bool offset(const RS_OffsetData& data);
    //bool cut(const RS_Vector& cutCoord, RS_AtomicEntity* cutEntity);//����
    bool stretch(const RS_Vector& firstCorner,//��չ������
                 const RS_Vector& secondCorner,
                 const RS_Vector& offset);

//    bool bevel(const RS_Vector& coord1, RS_AtomicEntity* entity1,//����
//               const RS_Vector& coord2, RS_AtomicEntity* entity2,
//               RS_BevelData& data);
//    bool round(const RS_Vector& coord,//Բ��
//               const RS_Vector& coord1,
//               RS_AtomicEntity* entity1,
//               const RS_Vector& coord2,
//               RS_AtomicEntity* entity2,
//               RS_RoundData& data);

//    bool explode();//�ֽ�
//    bool explodeTextIntoLetters();//�ֽ��ı�����ĸ
//    bool explodeTextIntoLetters(RS_MText* text, QList<RS_Entity*>& addList);
//    bool explodeTextIntoLetters(RS_Text* text, QList<RS_Entity*>& addList);
    bool moveRef(RS_MoveRefData& data);//�ƶ��ο���

//    bool splitPolyline(RS_Polyline& polyline,//�зֶ��߶�
//                       RS_Entity& e1, RS_Vector v1,
//                       RS_Entity& e2, RS_Vector v2,
//                       RS_Polyline** polyline1,
//                       RS_Polyline** polyline2) const;
//    RS_Polyline* addPolylineNode(RS_Polyline& polyline,//��Ӷ��߶ε�
//                                 const RS_AtomicEntity& segment,
//                                 const RS_Vector& node);
//    RS_Polyline* deletePolylineNode(RS_Polyline& polyline,//ɾ�����߶ε�
//                                    const RS_Vector& node);
//    RS_Polyline* deletePolylineNodesBetween(RS_Polyline& polyline, RS_AtomicEntity& segment,
//                                            const RS_Vector& node1, const RS_Vector& node2);
//    RS_Polyline* polylineTrim(RS_Polyline& polyline,//�޽����߶�
//                              RS_AtomicEntity& segment1,
//                              RS_AtomicEntity& segment2);

private:
    void deselectOriginals(bool remove);//ȡ����ѡʵ�壬��������־λtrue����ɾ������
    void addNewEntities(QList<RS_Entity*>& addList);//�����ʵ��
    //void copyEntity(RS_Entity* e, const RS_Vector& ref, const bool cut);//��������ʵ��

protected:
    RS_EntityContainer* container;
	RS_EntityContainer* camContainer;
    RS_Graphic* graphic;
    RS_Document* document;
	RS_Document* camdocument;
    RS_GraphicView* graphicView;
    bool handleUndo;
	RS_AtomicEntity* trimmed;
	bool trimmed_flag;
};

#endif
