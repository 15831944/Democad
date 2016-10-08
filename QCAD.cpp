#include <QCAD.h>
#include "democad.h"

QCAD::QCAD( void )
{
	m_pCad = new DemoCad;
}

QCAD::~QCAD( void )
{
	if( m_pCad != NULL )
	{
		delete m_pCad;
		m_pCad = NULL;
	}
}

void QCAD::onSetPos(int nx, int ny, int width, int height)
{
	if( m_pCad != NULL )
        m_pCad->setGeometry( nx, ny, width, height );
}

void QCAD::onShow()
{
	if( m_pCad != NULL )
		m_pCad->show();
}

void QCAD::onOriginalInit()
{
	if( m_pCad != NULL )
		m_pCad->getGraphicView()->resetSize();
}

void QCAD::onSetParent(QWidget *parent /* = 0 */)
{
	if( m_pCad != NULL && parent != NULL )
		m_pCad->setParent( parent );
}

void QCAD::onSetSubWindow(bool bTag /* = false */)
{
	if( bTag )
		m_pCad->setWindowFlags( Qt::SubWindow );
}

void QCAD::setDXFFilePath(QString path)
{
	if( m_pCad != NULL )
	    return;
}

void QCAD::getCADDataList(QList<CADData * > &cadData)
{
	//< ��������е�ʵ������
	cadData.clear();

	//< 
	if( m_pCad != NULL )
	{
		for(QList<RS_Entity *>::iterator it = m_pCad->getGraphicView()->getContainer()->getEntities().begin();
			it != m_pCad->getGraphicView()->getContainer()->getEntities().end();
			it++)
		{
			if(!(*it)->isVisible()) continue;

			CADData * tmpData = new CADData;
			
			//< ���Ϊ������ֱ�Ӽ��뵽ʵ��������ȥ
			if( (*it)->isLead() )
			{
				tmpData->isLead = true;
				cadData.append( tmpData );
				continue;
			}
			
			//< ����ʵ������
			switch( (*it)->rtti() )
			{
			case RS2::EntityPoint:
				tmpData->nType = CADData::cad_entity_point;
				break;
			case RS2::EntityArc:
				tmpData->nType = CADData::cad_entity_arc;
				break;
			case RS2::EntityLine:
				tmpData->nType = CADData::cad_entity_line;
				break;
			case RS2::EntitySpline:
				tmpData->nType = CADData::cad_antity_spline;
			default:
				break;
			}

			//< ����ʵ�����ݣ������Ͳ���ȷ��ֱ�������ô�ѭ��
			if( tmpData->nType < 0 )
			{
				delete tmpData;
				tmpData = NULL;
				continue;
			}



			
		}
	}
}



