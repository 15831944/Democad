#include "detpntsetdlg.h"

#define VIEW_WTH  600
#define VIEW_HGT  450
#define LEFT_AREA_WTH  250

DetPntSetDlg::DetPntSetDlg(QWidget *parent)
	: QDialog(parent)
	, m_bHasArc( false )
	, m_bReadOnly( false )
{
	//< ��ʼ����С
	resize( 700, 450 );
	setMinimumSize( 600, 320 );
	setWindowTitle( tr("̽����趨") );

	//< �����ͷ�Χ����
	QRegExp regExp("^(500|[1-4]?\\d?\\d?)$");  //< ���ֵ500
	QRegExpValidator *pReg = new QRegExpValidator(regExp, this); 
	for( int i = 0; i < pnt_set_index_count; ++i )
	{
		m_indexEdit[i] = new QLineEdit( this );
		m_indexEdit[i]->setAlignment( Qt::AlignCenter );
		m_indexEdit[i]->resize( 40, 20 );
		m_indexEdit[i]->setValidator( pReg );
		//if( i > _btmE && i < _zB )
		//	m_indexEdit[i]->setEnabled( m_bHasArc );

		//if( hasConfigFlag )
		//{
		//	if( i < _zB )
		//	{
		//		if( g_XYIndex[i] != 0 )
		//			m_indexEdit[i]->setText( QString::number(g_XYIndex[i]) );
		//	}
		//	else
		//	{
		//		if( g_ZIndex[i-_zB] != 0 )
		//			m_indexEdit[i]->setText( QString::number(g_ZIndex[i-_zB]) );
		//	}
		//}
	}

	for( int i = 0; i < 9; ++i )
	{
		m_signLbl[i] = new QLabel( this );
		m_signLbl[i]->setText( tr("-") );
		m_signLbl[i]->setStyleSheet("font-size: 20px;");
		m_signLbl[i]->setAlignment( Qt::AlignCenter );
	}

	//< OK ��ť
	m_okBtn = new RS_CustomBtn( this );
	m_okBtn->setText( tr("ȷ�ϵ�¼") );
	m_okBtn->setFocusPolicy( Qt::NoFocus );
	connect( m_okBtn, SIGNAL( clicked() ), this, SLOT( slotUploadAllIndex() ));
    
	//< �ر� ��ť
	m_closeBtn = new RS_CustomBtn( this );
	m_closeBtn->setText( tr("�� ��") );
	m_closeBtn->setFocusPolicy( Qt::NoFocus );
	connect( m_closeBtn, SIGNAL( clicked()), this, SLOT( reject() ));

	//< 
	m_SelectChkBox = new QCheckBox( this );
	m_SelectChkBox->setText( tr( "�Ƿ����Բ��̽���" ) );
	m_SelectChkBox->setStyleSheet("font-size: 14px;");
	connect( m_SelectChkBox, SIGNAL( toggled(bool) ), this, SLOT( slotHasArcChecked(bool)) );

	//< 
	m_TipText = new QTextEdit( this );
	m_TipText->setReadOnly( true );
	QString tipStr = tr("������\n\n1. ���Ϲ�ѡ������ѡ���Ƿ���Բ���ϵ�̽��㣻\n\n");
	tipStr += tr("2. ��ÿ���߻���Բ��������̽������е�Qֵ��Χ��G100P150Q_�������뱣֤ÿ������Qֵ�����Ҷ�������̽����Ϣ��\n\n");
	tipStr += tr("3. ���뷶Χ1~500��\n\n");
	tipStr += tr("4. ����XYƽ���ϵ�����̽���QֵҪ��֤������\n\n");
	tipStr += tr("5. ֱ�ӹرձ����ڿ�ȡ�����룡");
	m_TipText->setText( qstrToHTML( tipStr, 4, QColor( Qt::blue ), true ) );

	//< Z��
	m_ZIndexLbl = new QLabel( this );
	m_ZIndexLbl->setText( tr("Z��Qֵ��Χ��") );
}

DetPntSetDlg::~DetPntSetDlg()
{
	//< Qt�Զ����ٶ������ϵĳ�Ա
}

void DetPntSetDlg::paintEvent(QPaintEvent * event)
{
	QPainter painter( this );
	painter.setRenderHint( QPainter::Antialiasing, true );
	paintView( painter, 0, 0, width() - LEFT_AREA_WTH, height() );

	QDialog::paintEvent( event );
}

void DetPntSetDlg::showEvent(QShowEvent * event)
{
	//< ÿ����ʾʱҪ���¸�����ʾ״̬
	bool hasConfigFlag = false;
	m_bHasArc = false;
	for( int i = _leftB; i <= _rightBtmE; ++i )
	{
		if( g_XYIndex[i] != 0 )
		{
			hasConfigFlag = true;
			if( i >= _leftTopB )
				m_bHasArc = true;
		}
	}

	m_SelectChkBox->setChecked( m_bHasArc );
	for( int i = 0; i < pnt_set_index_count; ++i )
	{
		//if( i > _btmE && i < _zB )
			//m_indexEdit[i]->setEnabled( m_bHasArc );

		if( hasConfigFlag )
		{
			if( i < _zB )
			{
				if( g_XYIndex[i] != 0 )
					m_indexEdit[i]->setText( QString::number(g_XYIndex[i]) );
			}
			else
			{
				if( g_ZIndex[i-_zB] != 0 )
					m_indexEdit[i]->setText( QString::number(g_ZIndex[i-_zB]) );
			}
		}
		else
			m_indexEdit[i]->clear();
	}
}

#define Left_Gap  60
#define Top_Gap   60

/************************************************************************/
/* ȷ������������λ��
/************************************************************************/
void DetPntSetDlg::resizeEvent(QResizeEvent * event)
{
	int width =  this->width();
	int height = this->height();
	int viewWth = width - LEFT_AREA_WTH;
	const int editWth = 35;
	const int editHgt = 20;
	const int gapWth = 3;
	m_indexEdit[_topB]->setGeometry( viewWth / 2 - editWth - 10, Top_Gap - editHgt - gapWth, editWth, editHgt );
	m_indexEdit[_topE]->setGeometry( viewWth / 2 + 10, Top_Gap - editHgt - gapWth, editWth, editHgt );
	m_signLbl[0]->setGeometry( viewWth / 2 - 10, Top_Gap - editHgt - gapWth, 20, editHgt);

	m_indexEdit[_btmB]->setGeometry( viewWth / 2 - editWth - 10, height - Top_Gap + gapWth, editWth, editHgt);
	m_indexEdit[_btmE]->setGeometry( viewWth / 2 + 10, height - Top_Gap + gapWth, editWth, editHgt);
	m_signLbl[1]->setGeometry( viewWth / 2 - 10, height - Top_Gap + gapWth, 20, editHgt);

	m_indexEdit[_leftB]->setGeometry( Left_Gap - editWth - gapWth, height / 2 - editHgt- 10, editWth, editHgt );
	m_indexEdit[_leftE]->setGeometry( Left_Gap - editWth - gapWth, height / 2 + 10, editWth, editHgt );
	m_signLbl[6]->setGeometry( Left_Gap - editWth - gapWth + editWth / 2 - 10, height / 2 - 10, 20, 20 );

	m_indexEdit[_rightB]->setGeometry( viewWth - Left_Gap + gapWth, height / 2 - editHgt - 10, editWth, editHgt );
	m_indexEdit[_rightE]->setGeometry( viewWth - Left_Gap + gapWth, height / 2 + 10, editWth, editHgt );
	m_signLbl[7]->setGeometry( viewWth - Left_Gap + gapWth + editWth / 2 - 10, height / 2 - 10, 20, 20 );


	m_indexEdit[_leftTopB]->setGeometry( Left_Gap - editWth - 10, Top_Gap - editHgt * 2 - gapWth
		, editWth, editHgt );
	m_indexEdit[_leftTopE]->setGeometry( Left_Gap - editWth + 10, Top_Gap - editHgt
		, editWth, editHgt );
	m_signLbl[2]->setGeometry( Left_Gap - editWth - 10, Top_Gap - editHgt, 20, 20);

	m_indexEdit[_rightTopB]->setGeometry( viewWth - Top_Gap - 10, Top_Gap - editHgt * 2 - gapWth
		, editWth, editHgt);
	m_indexEdit[_rightTopE]->setGeometry( viewWth - Top_Gap + 10, Top_Gap - editHgt
		, editWth, editHgt);
	m_signLbl[3]->setGeometry( viewWth - Top_Gap - 10, Top_Gap - editHgt, 20, 20);

	m_indexEdit[_rightBtmB]->setGeometry( viewWth - Top_Gap - 10, height - Top_Gap
		, editWth, editHgt);
	m_indexEdit[_rightBtmE]->setGeometry( viewWth - Top_Gap + 10, height - Top_Gap + editHgt + gapWth
		, editWth, editHgt);
	m_signLbl[4]->setGeometry( viewWth - Top_Gap - 10, height - Top_Gap + editHgt + gapWth, 20, 20 );

	m_indexEdit[_leftBtmB]->setGeometry( Left_Gap - editWth - 10, height - Top_Gap
		, editWth, editHgt );
	m_indexEdit[_leftBtmE]->setGeometry( Left_Gap - editWth + 10, height - Top_Gap + editHgt + gapWth
		, editWth, editHgt );
	m_signLbl[5]->setGeometry( Left_Gap - editWth - 10, height - Top_Gap + editHgt + gapWth, 20, 20 );

	const int zLblWth = 85;
	const int zLblGapToCenter = 30;
	m_indexEdit[_zB]->setGeometry( viewWth / 2 - editWth / 2, height / 2 + zLblGapToCenter, editWth, editHgt );
	m_indexEdit[_zE]->setGeometry( viewWth / 2 - editWth / 2 + 20 + editWth
		, height / 2 + zLblGapToCenter, editWth, editHgt );
	m_signLbl[8]->setGeometry( viewWth / 2 - editWth / 2 + editWth, height / 2 + zLblGapToCenter, 20, 20 );
	m_ZIndexLbl->setGeometry( viewWth / 2 - editWth / 2 - zLblWth, height / 2 + zLblGapToCenter, zLblWth, editHgt ); 

	
	int nTopGap = 10;
	int nBtmGap = nTopGap;
	int nRightGap = 10;
	int nCheckBoxWth = width - viewWth - nRightGap;
	int nCheckBoxHgt = 25;
	m_SelectChkBox->setGeometry( viewWth, nTopGap, nCheckBoxWth, nCheckBoxHgt );

	//< OK ��ťλ�úʹ�С
	int btnHgt = 30;
	int btnWth = 115;
	m_closeBtn->setGeometry( width - btnWth - nRightGap, height - btnHgt - nBtmGap, btnWth, btnHgt );
	m_okBtn->setGeometry( width - btnWth * 2 - nRightGap - nRightGap, height - btnHgt - nBtmGap, btnWth, btnHgt );


	int tipTextWth = nCheckBoxWth;
	int nVGap = 10;
	int tipTextHgt = height - btnHgt - nTopGap - nBtmGap - nVGap * 2 - nCheckBoxHgt;
	m_TipText->setGeometry( viewWth, nTopGap + nCheckBoxHgt + nVGap, tipTextWth, tipTextHgt );

}

void DetPntSetDlg::paintView( QPainter & painter, int _x, int _y, int wth, int hgt )
{
	//< ����Բ�Ǿ���
	int nleft = Left_Gap;
	int nRight = nleft;
	int nTop = Top_Gap;
	int nBtm = nTop;
	int nRadius = 20;
	painter.setPen( QPen( Qt::darkBlue, 2, Qt::SolidLine ) );
	painter.drawRoundedRect( nleft + _x, nTop + _y, wth - 2 * nleft, hgt - 2 * nTop, nRadius, nRadius );

	//< �������߷�Χ
	const int nDashLineLth = 40;
	painter.setPen( QPen(Qt::darkGray, 1, Qt::DashLine));
	painter.drawLine( nleft + nRadius, nTop - nDashLineLth, nleft + nRadius, nTop );
	painter.drawLine( wth - nleft - nRadius, nTop - nDashLineLth, wth - nleft - nRadius, nTop);
	painter.drawLine( nleft - nDashLineLth, nTop + nRadius, nleft, nTop + nRadius );
	painter.drawLine( nleft - nDashLineLth, hgt - nBtm - nRadius, nleft, hgt - nBtm - nRadius );

	painter.drawLine( nleft + nRadius, hgt - nBtm, nleft + nRadius, hgt - nBtm + nDashLineLth );
	painter.drawLine( wth - nleft - nRadius, hgt - nBtm, wth - nleft - nRadius, hgt - nBtm + nDashLineLth);
	painter.drawLine( wth - nleft, nTop + nRadius, wth - nleft + nDashLineLth, nTop + nRadius );
	painter.drawLine( wth - nleft, hgt - nBtm - nRadius, wth - nleft + nDashLineLth, hgt - nBtm - nRadius );

	//< ��������ϵ
	int nCenterX = wth / 2 + _x;
	int nCenterY = hgt / 2 + _y;
	int nLineLth = 40;
	painter.setPen( QPen( Qt::red, 1, Qt::SolidLine ) );
	painter.drawLine( nCenterX - 2, nCenterY, nCenterX + nLineLth, nCenterY );
	painter.drawLine( nCenterX + nLineLth, nCenterY, nCenterX + nLineLth - 8, nCenterY - 4 );
	painter.drawLine( nCenterX + nLineLth, nCenterY, nCenterX + nLineLth - 8, nCenterY + 4 );
	painter.drawLine( nCenterX , nCenterY + 2, nCenterX , nCenterY - nLineLth );
    painter.drawLine( nCenterX, nCenterY - nLineLth, nCenterX + 4, nCenterY - nLineLth + 8);
	painter.drawLine( nCenterX, nCenterY - nLineLth, nCenterX - 4, nCenterY - nLineLth + 8);
	painter.drawText( QRect( nCenterX + nLineLth - 8, nCenterY + 4, 18, 18), "x" );
	painter.drawText( QRect( nCenterX + 8, nCenterY - nLineLth , 18, 18), "y");
}

/************************************************************************/
/* �ۺ��������ù�ѡ����������
/************************************************************************/
void DetPntSetDlg::slotHasArcChecked( bool flag)
{
	m_bHasArc = flag;
	for( int i = _leftTopB; i <= _rightBtmE; ++i )
	{
		m_indexEdit[i]->setEnabled( flag );
	}
}

void DetPntSetDlg::slotUploadAllIndex()
{
	qDebug() << "liuyc: ready to upload all index!";
	if( !inputLegal( ui_input ) )
	{
		QString msgContent = "���������������밴�չ����������ı����ź����µ�����㣡����\n";
		msgContent += "1. ÿ��������޲���������\n";
		msgContent += "2. XY���б�������\n";
		msgContent += "3. XY�������շ�Χ��Z������Χ���غϲ���\n";
		msgContent += "4. ����ı����ű����Ѿ�����ͨ��G100ָ�������ݺ�\n";
		msgContent += "5. XYƽ�������������ţ�";
		popMessageBox( "Lynuc CAD Warning",  msgContent , 5, QColor(255, 0, 0), true );
	}
	else
		this->accept();
}

bool DetPntSetDlg::inputLegal( int inputmode /* = ui_input */ )
{
	QList<unsigned short> tmpIndexList;
	if( inputmode == ui_input )  //< ����ǽ��������򽫽����ϵ�ֵ��ֵ��ȫ�ֱ�����
	{
		for( int i = 0; i < _zB; ++i )
		{
			g_XYIndex[i] = m_indexEdit[i]->text().toUInt();
		}
		for( int i = _zB; i <= _zE; ++i )
		{
			g_ZIndex[i - _zB] = m_indexEdit[i]->text().toUInt();
		}
	}

	//< �����е�ֵ���浽һ����ʱ��������
	for( int i = 0; i < _zB; i += 2 )
	{
		if( g_XYIndex[i] != 0 && g_XYIndex[i+1] != 0 )
		{
			if( g_XYIndex[i] > g_XYIndex[i+1] )
			{
				qDebug() << __FUNCTION__ << "liuyc: input ilegal! _ 0";
				return false;
			}

			tmpIndexList.append( g_XYIndex[i] );
			tmpIndexList.append( g_XYIndex[i+1] );
		}
	}
	if( tmpIndexList.isEmpty() || tmpIndexList.count() % 2 != 0 )
	{
		qDebug() << __FUNCTION__ << "liuyc: input ilegal! _ 1";
		return false;
	}

	//< ȷ������XY������Χ������
	for( int i = 0; i < tmpIndexList.count(); i += 2)
	{
		for( int j = 0; j < tmpIndexList.count(); j += 2 )
		{
			if( i == j )
				continue;

			if( (tmpIndexList.at(i) >= tmpIndexList.at(j) && tmpIndexList.at(i) <= tmpIndexList.at(j+1)) 
				|| (tmpIndexList.at(i+1) >= tmpIndexList.at(j) && tmpIndexList.at(i+1) <= tmpIndexList.at(j+1)))  
			{
				qDebug() << __FUNCTION__ << "liuyc: input ilegal! _ 2";
				return false;
			}
		}
	}

	//< xy index ��������ж��Ƿ�����
	qSort( tmpIndexList.begin(), tmpIndexList.end() );
	for( int i = 2; i < tmpIndexList.count(); i+=2 )
	{
		if( tmpIndexList.at(i) != tmpIndexList.at(i-1) + 1 )
		{
			qDebug() << __FUNCTION__ << "liuyc: input ilegal! _ 3";
			return false;
		}
	}
	//< �ж�XY��Χ�Ƿ���Z��Χ���غϵĲ���
	if( g_ZIndex[0] != 0 && g_ZIndex[1] != 0 )  
	{
		if( g_ZIndex[0] > g_ZIndex[1] )
		{
			qDebug() << "liuyc: input ilegal! _ 4";
			return false;
		}
		if( (g_ZIndex[0] <= tmpIndexList.first() && g_ZIndex[1] >= tmpIndexList.first())
			|| ( g_ZIndex[0] <= tmpIndexList.last() && g_ZIndex[1] >= tmpIndexList.last())
			|| (tmpIndexList.first() <= g_ZIndex[0] && tmpIndexList.last() >= g_ZIndex[0])
			|| (tmpIndexList.first() <= g_ZIndex[1] && tmpIndexList.last() >= g_ZIndex[1]))
		{
			qDebug() << "liuyc: input ilegal! _ 5";
			return false;
		}
	}

	//< ����ж�����ı����������Ƿ��б�������
#ifndef Q_OS_WIN
	for( int i = tmpIndexList.first(); i <= tmpIndexList.last(); ++i )
	{
		if( !g_SavedData.contains(i) )
		{
			qDebug() << "liuyc: input ilegal! _ 6";
			return false;
		}
	}
#endif

	if( g_ZIndex[0] != 0 && g_ZIndex[1] != 0 )
	{
#ifndef Q_OS_WIN
		for( int i = g_ZIndex[0]; i <= g_ZIndex[1]; ++i )
		{
			if( !g_SavedData.contains(i) )
			{
				qDebug() << "liuyc: input ilegal! _ 6";
				return false;
			}
		}
#endif
		//< Z��ı����������浽ȫ�ֱ�����ȥ
		g_Index[Z_MIN] = g_ZIndex[0];
		g_Index[Z_MAX] = g_ZIndex[1];
		g_ZSrcDataBeginNum = g_Index[Z_MIN];
		g_ZSrcDataEndNum   = g_Index[Z_MAX];
		g_ZDataCount = g_ZSrcDataEndNum - g_ZSrcDataBeginNum + 1;
	}
	else
		g_ZDataCount = 0;

	//< XY��ı�����������
	g_XYSrcDataBeginNum = tmpIndexList.first();
	g_XYSrcDataEndNum   = tmpIndexList.last();
	g_XYSrcDataCount    = g_XYSrcDataEndNum - g_XYSrcDataBeginNum + 1;
	g_XYDesDataCount    = g_XYSrcDataCount;
	qDebug() << "liuyc xy : " << g_XYSrcDataBeginNum << "~" << g_XYSrcDataEndNum << "  count = " << g_XYSrcDataCount;
	qDebug() << "liuyc z  : " << g_ZSrcDataBeginNum  << "~" << g_ZSrcDataEndNum  << "  count = " << g_ZDataCount;
	
	//< ��ֱ���ϵ�̽���Ĳ��ָ�ֵ��֮ǰ�Ľṹ��ȥ 
	//< 2016.10.18 Ϊ�˴����޸�����С������������һ��ȫ�ֱ�����������֮���ڴ浽ԭ����g_Index��ȥ
	for( int i = _leftB; i <= _btmE; ++i )  
	{
		g_Index[i] = g_XYIndex[i];
	}

	return true;
}
