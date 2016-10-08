#ifndef RS_GRID_H
#define RS_GRID_H

#include "rs_graphicview.h"
#include "rs_vector.h"

/*
    ��ʾ�����ϵ�����
*/
class RS_Grid {
public:
    RS_Grid(RS_GraphicView* graphicView);
    ~RS_Grid();

    void updatePointArray();

    /**
        * @return Array of all visible grid points.
        */
    RS_Vector* getPoints() {//���ص�����ָ��
        return pt;
    }

    /**
      *@return the closest grid point
      */
    RS_Vector snapGrid(const RS_Vector& coord) const;

    /**
        * @return Number of visible grid points.
        */
    int count() {
        return number;
    }

	/**
        * @return Number of visible grid points of axis X.
        */
    int countX() {
        return m_numberX;
    }

	/**
        * @return Number of visible grid points of axis Y.
        */
    int countY() {
        return m_numberY;
    }

    void setCrosshairType(RS2::CrosshairType chType){//����׼������
        crosshairType=chType;
    }

    RS2::CrosshairType getCrosshairType(){
        return crosshairType;
    }

    /**
        * @return Current grid spacing.
        */
    double getSpacing() {
        return spacing;
    }

    /**
        * @return Current meta grid spacing.
        */
    //double getMetaSpacing() {
    //	return metaSpacing;
    //}

    /**
        * @return Grid info for status widget.
        */
    QString getInfo() {
            return QString("%1 / %2").arg(spacing).arg(metaSpacing);
    }

    /**
        * @return Meta grid positions in X.
        */
    double* getMetaX() {
            return metaX;
    }

    /**
        * @return Number of visible meta grid lines in X.
        */
	int countMetaX() {
		return numMetaX;
	}

    /**
        * @return Meta grid positions in Y.
        */
    double* getMetaY() {
            return metaY;
    }

    /**
        * @return Number of visible meta grid lines in Y.
        */
    int countMetaY() {
        return numMetaY;
    }
    bool isIsometric() const{
        return isometric;
    }
    void setIsometric(bool b){
        isometric=b;
    }
    RS_Vector getMetaGridWidth() const {
        return metaGridWidth;
    }
    RS_Vector getCellVector()
    {
        return cellV;
    }

	void setUserGrid(double gp){
		userGrid.set(gp, gp);
	}

protected:
    RS_GraphicView* graphicView;//! Graphic view this grid is connected to.
    double spacing; //��ǰ������
    double metaSpacing;//��ǰԪ��������Ӧ�����Ǹ�С���??
	RS_Vector userGrid;

    RS_Vector* pt;//���������ָ�룬���������������
    RS_Vector baseGrid; // ����  �������½ǵĵ�
    RS_Vector cellV;// (dx,dy)??????���㲶׽����ʱ�õ���
    RS_Vector metaGridWidth;//Ԫ�����
    int number;//�����
	int m_numberX;//�����
	int m_numberY;//�����

    double* metaX;//Ԫ����xָ��//! Meta grid positions in X
    int numMetaX;//Ԫ�����ߣ���x�ϵĸ���//! Number of meta grid lines in X
    double* metaY;//Ԫ����yָ��//! Meta grid positions in Y
    int numMetaY;//Ԫ�����ߣ���y�ϵĸ���//! Number of meta grid lines in Y

    bool isometric;//metagarid�Ƿ��ǵȾ��
    RS2::CrosshairType crosshairType;//ʮ������
};

#endif
