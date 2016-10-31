#include "showallqdlg.h"
#include <QScrollBar>

ShowAllQDlg::ShowAllQDlg(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowTitle( tr("̽���¼����") );
	resize( 600, 450 );
	m_QTable = new DetQTable( this );
	
}

ShowAllQDlg::~ShowAllQDlg()
{

}

void ShowAllQDlg::resizeEvent(QResizeEvent * event)
{
	m_QTable->setGeometry( 10, 10, width() - 20, height() - 20 );
}




DetQTable::DetQTable(  QWidget * parent /* = NULL */ )
	: QTableView( parent )
{
	this->setAutoFillBackground( true );

	//< ��������model
	m_sourceModel = new DetQTableModel(this);
	m_proxyModel  = new QSortFilterProxyModel(this);
	m_proxyModel->setSourceModel(m_sourceModel);
	this->setModel(m_proxyModel);

	//< ����ί��
	m_itemDelegate = new DetQTableDelegate(this);
	setItemDelegate(m_itemDelegate);

	this->verticalHeader()->hide();
	this->setSelectionMode( QAbstractItemView::SingleSelection );
	this->horizontalHeader()->setStretchLastSection(true);
	this->verticalHeader()->setStretchLastSection(true);

	//< ���ñ����ʽ
	this->setStyleSheet("QTableView{ \
						padding:4px 4px 5px 4px; \
					    background: rgb(220,220,220); \
		                gridline-color: #FFFFFF; \
						border: 2px solid #FFFFFF \
						} \
		                QTableCornerButton::section{ \
						background:Transparent; \
		                border: 1px solid #FFFFFF; \
						margin: 0px 1px 1px 0px; \
						} \
						");

	//this->verticalScrollBar()->setMinimumHeight( 100 );
}

void DetQTable::resizeEvent( QResizeEvent *event )
{
	int width =  this->width();
	int height = this->height();

	const int wthFirst = 55;
	const int wthOther = ( width - wthFirst - 10 ) / ( table_header_count - 1 );

	this->horizontalHeader()->resizeSection(_Q_index, wthFirst );
    this->horizontalHeader()->resizeSection(_x_coor, wthOther );
	this->horizontalHeader()->resizeSection(_y_coor, wthOther );  //< ���һ�в����ã�ֱ���Զ���Ӧ���
}




QVariant DetQTableModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}

	QString text = "";
	if( orientation == Qt::Vertical )
	{
		//< �б�ͷ��ʱ���趨
	}
	else
	{
		switch( section )
		{
		case _Q_index:
			text = tr("������");
			break;
		case _x_coor:
			text = tr(" x ����");
			break;
		case _y_coor:
			text = tr(" y ����");
			break;
		case _z_coor:
			text = tr(" z ����");
			break;
		default:
			break;
		}
	}

	return text;
}

QVariant DetQTableModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}

	int row    = index.row();
	int column = index.column();
	QString value = NULL;

	RS_Vector tmpVec = g_SavedData.value( row+1 , RS_Vector(0., 0., 0. ) );
	if( role == Qt::DisplayRole )
	{	
		switch( column )
		{
		case _Q_index: //< ��¼���
			value = QString::number( row + 1 );
			break;
		case _x_coor:  //< NC�ļ��к�
			value = QString::number( tmpVec.x, 'd', 6 );
			break;
		case _y_coor:
			value = QString::number( tmpVec.y, 'd', 6 );
			break;
		case _z_coor:
			value = QString::number( tmpVec.z, 'd', 6 );
			break;
		default:
			break;
		}

		return (value + " ");  //< ������һ���ո�Ϊ�����ۣ����ݲ�����д��
	}

	return QVariant();
}

QModelIndex DetQTableModel::index(int row, int column, const QModelIndex &parent ) const
{
	return createIndex(row, column, 0);
}

void DetQTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_ASSERT(index.isValid());

	QStyleOptionViewItemV4 opt = option;

	int row    = index.row();
	int column = index.column();

	//painter->setFont( FONT_LIGHT_12_SIMHEI );
	painter->setPen(QColor(0, 0, 0));

	if ((option.state & QStyle::State_Selected)
		&& (option.state & QStyle::State_Active))
	{
		painter->setPen(QColor(255, 255, 255));
		painter->fillRect(option.rect, QBrush(QColor(102, 102, 153)));       
	}

	if(column == _Q_index)
	{
		opt.widget->style()->drawItemText(painter, opt.rect, Qt::AlignCenter, opt.palette, true, index.data().toString());
	}
	else
	{
		opt.widget->style()->drawItemText(painter, opt.rect
			, Qt::AlignRight | Qt::AlignVCenter, opt.palette, true, index.data().toString());
	}
}