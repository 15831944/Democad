#ifndef RS_CAMDATA_H
#define RS_CAMDATA_H

#include "rs_vector.h"

struct RS_CamData
{
	int nCADID;					// CAD�����е�ID�ţ�nCAMID��nCADID�����ж�ͼ���Ƿ��޸�

	explicit RS_CamData(CAMData dt)
	{
		nCADID        = dt.nCADID;
	}

	RS_CamData() 
	{//����Ĭ�ϳ�ʼ��
		reset();
	}

	void reset()
	{
		nCADID        = -1;
	}

};

#endif