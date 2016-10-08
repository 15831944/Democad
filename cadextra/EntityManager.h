#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <QVector>
#include "entityDefine.h"

class EntityManagerIterator;
class EntityManager;

class Common
{
public:
    void clearEntitys(QVector<DxfEntity *> &cont);
};


struct Block:Common//��ͷ
{
    OBJHANDLE	Objhandle;				// Handle
    QString		Name;		// Block name
    char		Flags;                                  // Block-type flags
    REALPOINT	BasePoint;                  // Base point
    OBJHANDLE	LayerObjhandle;		// Layer's Handle
    QVector<DxfEntity *>	Entities;

    Block(QString name, char flag, REALPOINT base, OBJHANDLE layerHd = 0):
        Name(name),Flags(flag),BasePoint(base),LayerObjhandle(layerHd)
    {}

    void clear()
    {
        clearEntitys(Entities);
        Entities.clear();
    }
};

//class EntityManagerIterator
//{
//public:
//    EntityManagerIterator();
//    EntityManagerIterator(EntityManager &m, int b, int e);
//    int blockIndex, entityIndex;
//    EntityManager &man;
//    EntityManagerIterator &	operator++ ();
//    EntityManagerIterator &	operator= ( const EntityManagerIterator & other );
//    bool	operator!= ( const EntityManagerIterator & other ) const;
//    DxfEntity *	operator-> () const;
//};


class Q_DECL_IMPORT EntityManager:Common
{
public:
    EntityManager();
    ~EntityManager();
    bool addEntity(DxfEntity *entity, int blockIndex = -1);
    int addBlock(Block *block);
    int getEntitySize();
    void clear();

    //================�������==============
//    typedef EntityManagerIterator iterator;
//    iterator begin();
//    iterator end();

    //QVector<DxfEntity *>	Entities;
    //ʵ���Ҳ��Ϊһ���飬��������ɣ��ڴ���Ĺ��������
    QVector<Block *> Blocks;
    int		BlocksNumber;
    int		CurrentBlock;//���ڲ����ʵ��ʱ����ǵ�ǰ��

    OBJHANDLE	LastObjhandle;
    int		LastDimBlkNum;
    VIEW		View;
private:

};


#endif // ENTITYMANAGER_H






