
#ifndef RS_DOCUMENT_H
#define RS_DOCUMENT_H

//#include "rs_layerlist.h"
#include "rs_entitycontainer.h"
#include "rs_undo.h"
#include "Parameter.h"

#include "structGraphM.h"
#include "rs_filterdxfrw.h"

//class RS_BlockList;


/*
    �̳���RS_EntityContainer
    �����ı���Ļ��࣬һ���ı��������graphics��blocks��������graphic views����ʾ
    ������һ����Ч�Ļ������ڻ���ͼ�Σ�һ���ļ���
    ֪���Լ��Ƿ��޸�
    ������������ͼԪ���ڴ�ģ�ͣ��Ͷ�ͼԪ�ڴ�ģ�͵Ĳ���
*/

class RS_Document : public RS_EntityContainer,
    public RS_Undo {
public:
    RS_Document(RS_GraphicView *pgv, RS_EntityContainer* parent=NULL);
    virtual ~RS_Document();

	void newDoc();
	bool save(RS2::FormatType type = RS2::FormatUnknown);
	bool saveAs(const QString &filename, RS2::FormatType type = RS2::FormatUnknown);
	bool saveDxfFile(const QString &filename = QString());
	bool open(const QString &filename, bool &bEmpty);
	bool openDxfFile(const QString &filename, bool &bEmpty);
//    virtual bool loadTemplate(const QString &filename, RS2::FormatType type) = 0;

	virtual CAMDataLink *getCamListReference(CAMDataLink *pOldData = NULL);
	virtual void addEntityWithCamList(CAMDataLink *pData, bool isClear);

	bool undo() {//��������޸ı�׼
		setModified(true);
		setNeedAfreash(true);
		return RS_Undo::undo();
	}

	bool redo() {
		setModified(true);
		setNeedAfreash(true);
		return RS_Undo::redo();
	}

    virtual bool isDocument() const {
        return true;
    }

    virtual void removeUndoable(RS_Undoable* u) {
        if (u!=NULL && u->undoRtti()==RS2::UndoableEntity) {
            removeEntity((RS_Entity*)u);
        }
    }


    RS_Pen getActivePen() const {
        return activePen;
    }

    void setActivePen(RS_Pen p) {
        activePen = p;
    }


    QString getFilename() const {
        return fileName;
    }
	
 
    QString getAutoSaveFilename() const {
        return autosaveFilename;
    }

	virtual void setModified(bool m) {
		modified = m;
		if( m )
			g_pQPublicSignal->SendGetNewContainer( gv->getContainer() );
	}
	
    virtual bool isModified() const {
        return modified;
    }
	
	void setNeedAfreash(bool na) {
		needAfreash = na;
	}

	bool isNeedAfreash() {
		return needAfreash;
	}

	bool isNewFile() {
		return fileName.isEmpty();
	}

	void setFilename(const QString& fn) {
		fileName = fn;
	}

	void setSaveType(RS2::FormatType type) {
		saveType = type;
	}

	RS2::FormatType getSaveType() {
		return saveType;
	}

    virtual void startUndoCycle() {
		setModified(true);
		setNeedAfreash(true);
		RS_Undo::startUndoCycle();
	}

    void setGraphicView(RS_GraphicView * g) {gv = g;}
    RS_GraphicView* getGraphicView() {return gv;}

	RS_Entity * getLinkEntity(RS_Entity *ld);//��ȡ��ǰ������ص�ʵ�壬û�о�ΪNULL
	void appendToCamList(CAMDataLink *cm);

protected:

    bool modified;//�ļ������Ƿ��޸�
	bool needAfreash; //��־��ͼ���ݺ�ת�����Cam�����Ƿ�һ�£��Ƿ���Ҫˢ��cam����
    RS_Pen activePen;//����
    QString fileName;
	RS2::FormatType saveType;
    QString autosaveFilename;

	RS2::FormatType formatType;
    //�����������ڵ�RS_GraphicView������ʾ�����
    RS_GraphicView * gv;//used to read/save current view

	CAMDataLink *camList; //����ͷ
	CAMDataLink *camListEnd;//����β

	RS_FilterDXFRW *dxfFilter;//���ڱ���dxf�ļ�������

};


#endif
