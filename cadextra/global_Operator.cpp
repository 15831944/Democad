#include "global_Operator.h"
#include "global_Values.h"
#include "QMessageBox" //@use for debug
#include "rs.h"
#include "rs_vector.h"
#include "QAbstractButton"
#include "QObject"
#include "ErrorMessage.h"

//begin------------------new-------------
void initEn_Map(){
	if(pEn_Map==NULL)
		return ;
	for(QList<En_Line*>::iterator it=pEn_Map->begin();it!=pEn_Map->end();it++){//������En_Line
		delete *it;
	} 
	delete pEn_Map;
}

bool qlistReverse(QList<RS_Entity *> * revlist)
{
	std::list<RS_Entity *> tmpList = revlist->toStdList();
	tmpList.reverse();
	revlist->clear();
	
	for(std::list<RS_Entity *>::iterator it = tmpList.begin(); it != tmpList.end(); it++)
	{
		revlist->append(*it);
	}
    return true;
}

void loadEn_Map(RS_EntityContainer *allEntity)
{
	initEn_Map();

	pEn_Map    = new QList<En_Line*>;
	int nCount = allEntity->count();

	for(int i = 0; i < nCount; i++)
	{
		RS_Vector start=allEntity->entityAt(i)->getStartpoint();
		RS_Vector end=allEntity->entityAt(i)->getEndpoint();

		RS_Line line(start,end);

		pLine=new En_Line(line,allEntity->entityAt(i));

		pEn_Map->push_back(pLine);
	}

}

bool isCloseGraph_Sort_Check(En_Line* curLine,RS_EntityContainer *allEntity,QString dir,RS_Entity *o_en,int ls_num){
	RS_Vector cur_start=curLine->line.getStartpoint();
	RS_Vector cur_end=curLine->line.getEndpoint();

	int cur_num=ls_num;//��¼���ҵ��ڼ�������
	//���ҵ����һ���߶ε�ʱʱ��
	//���ݲ�ͬ�ķ���
	if((curLine->pEn->rtti()==RS2::EntityArc)&&cur_num!=0 || cur_num>=1){//����1���߲��ж�
		if(dir=="R"){
			/*if((cur_end==o_en->getStartpoint()||cur_end==o_en->getEndpoint())){*/
			if((checkSamePoint(cur_end,o_en->getStartpoint())||checkSamePoint(cur_end,o_en->getEndpoint()))&&(curLine->pEn!=o_en)){
				if(curLine->pEn->rtti()==RS2::EntityArc ||  curLine->pEn!=o_en){//��Բ����ʱ�� ���߲���Բ����ʱ��
					o_en->setGraphNum(*G_NUM);//��¼���ڼ�G_NUM��ͼ����

					curLine->pEn->SetLeadLinkFlag(true);//�ѱ�־λ����Ϊtrue
					curLine->pEn->setGraphNum(*G_NUM);//��¼���ڼ�G_NUM��ͼ����
					//Begin�Ƶ�ջ����
					allEntity->MoveToTop(curLine->pEn);
					//End�Ƶ�ջ����

					return true;
					}
			}
		}
		else if(dir=="L"){
			/*if((cur_start==o_en->getStartpoint()||cur_start==o_en->getEndpoint())&&(curLine->pEn!=o_en)){*/
			if((checkSamePoint(cur_start,o_en->getStartpoint())||checkSamePoint(cur_start,o_en->getEndpoint()))&&(curLine->pEn!=o_en)){
				o_en->setGraphNum(*G_NUM);//��¼���ڼ�G_NUM��ͼ����

				curLine->pEn->SetLeadLinkFlag(true);//�ѱ�־λ����Ϊtrue
				curLine->pEn->setGraphNum(*G_NUM);//��¼���ڼ�G_NUM��ͼ����
				//Begin�Ƶ�ջ����
				allEntity->MoveToTop(curLine->pEn);
				//End�Ƶ�ջ����

				return true;
			}
		}
	}

	cur_num++;

	//��ʼ����������֮��ѡһ�������Ƚ�
	for(QList<En_Line*>::iterator it=pEn_Map->begin();it!=pEn_Map->end();it++){  //@ for(...)
		RS_Vector next_start=(*it)->line.getStartpoint();
		RS_Vector next_end=(*it)->line.getEndpoint();

		//���߶������ǿɼ��ġ�û�������ߵġ�����curline�����ʱ�������Ƚ��Ƿ�����������
		if((*it)->pEn->isVisible()&&(!(*it)->pEn->GetLeadLinkFlag())&&(curLine->pEn!=(*it)->pEn)){

			if(dir=="R"){//���ұ߻��ǲ��� end_piont ��ʼ�ȶ��Ƿ��������߶�

				//dir == R ���������ֻ�Ƚ� cur_line ���յ��Ƿ�� next_line����� ���� �յ� ����
				/*if(cur_end==next_start||cur_end==next_end){*/
				if(checkSamePoint(cur_end,next_start)||checkSamePoint(cur_end,next_end)){
					/*
					��֪�߶������������ڽ�����һ���Ƚ�֮ǰ�Ȱ�cur_line�����������ʾ�Ѿ����Ƚ�
					*/
					curLine->pEn->SetLeadLinkFlag(true);

					/*��ʼ������һ���Ƚϣ����ҰѱȽϵķ��򴫽�ȥnext_line ������Ϊ��һ���Ƚϵ�cur_line 
						��Ϊnext_line������cur_Line��������next_line���end_point���Ƿ�����������dir==R
					*/
					
					//ȷ��next_line�Ĳ��ҷ���
					/*if(cur_end==next_start){*/
					if(checkSamePoint(cur_end,next_start)){
						dir="R";
					}
					else{
						dir="L";
					}

					//������һ������
					if(isCloseGraph_Sort_Check(*it,allEntity,dir,o_en,cur_num)){
						//�����ҽ���֮����߶��Ƶ���˲������������ķ��ͼ�α��G_NUM
						curLine->pEn->setGraphNum(*G_NUM);
						allEntity->MoveToTop(curLine->pEn);//�Ƶ�����

						return true;
					}
					else{
						//������ʧ�ܵ�ʱ��ԭleadLineFlag״̬Ϊfalse��
						curLine->pEn->SetLeadLinkFlag(false);

					}
				}
 			}
			else if(dir=="L"){//����� start_point ��ʼ�����Ƿ����������߶�

				//dir == L ���������ֻ�Ƚ� cur_line ������Ƿ�� next_line����� ���� �յ� ����
				/*if(cur_start==next_start||cur_start==next_end){*/
				if(checkSamePoint(cur_start,next_start)||checkSamePoint(cur_start,next_end)){
					/*
					��֪�߶������������ڽ�����һ���Ƚ�֮ǰ�Ȱ�cur_line�����������ʾ�Ѿ����Ƚ�
					*/
					curLine->pEn->SetLeadLinkFlag(true);

					/*��ʼ������һ���Ƚϣ����ҰѱȽϵķ��򴫽�ȥnext_line ������Ϊ��һ���Ƚϵ�cur_line 
						��Ϊnext_line������cur_Line��������next_line���end_point���Ƿ�����������dir==R
					*/

					//ȷ��next_line�Ĳ��ҷ���
					/*if(cur_start==next_start){*/
					if(checkSamePoint(cur_start,next_start)){
						dir="R";
					}
					else{
						dir="L";
					}

					if(isCloseGraph_Sort_Check(*it,allEntity,dir,o_en,cur_num)){
						//�����ҽ���֮����߶��Ƶ���˲������������ķ��ͼ�α��G_NUM
						curLine->pEn->setGraphNum(*G_NUM);
						allEntity->MoveToTop(curLine->pEn);//�Ƶ�����

						return true;
					}
					else{
						//������ʧ�ܵ�ʱ��ԭleadLineFlag״̬Ϊfalse��
						curLine->pEn->SetLeadLinkFlag(false);

					}
				}
			}

		}
	}//@ for(...)

	return false;
}

//*just use to resetCloseGraph: this is a local fuction*//
bool existEntity(RS_Entity* en,QList<RS_Entity*> &stack){
	for(int i=0;i!=stack.count();i++)
		if(en==stack.at(i))
			return true;

	return false;
}

void resetCloseGraph(RS_Entity* en,RS_EntityContainer* allEntity){
	QList<unsigned int> stack_reset;
	QList<RS_Entity*> statck_entity;


	//�����д����õ�ͼ���ҵ� ���� stack_reset ��
	for(int i=0;i!=(*G_NUM);i++){
		for(int j=0;j!=allEntity->count();j++){

			if(allEntity->entityAt(j)->getGraphNum()==i && allEntity->entityAt(j)->isSelected() &&allEntity->entityAt(j)->getGraphNum()!=0){
				bool _check=true;

				for(int n=0;n!=stack_reset.count();n++){
					if(stack_reset.at(n)==allEntity->entityAt(j)->getGraphNum()){
						_check=false;
					}
				}

				if(_check){
					stack_reset.push_back(i);
				}
			}

		}
	}

	//����stack_reset����ͼ��
	for(int i=0;i!=stack_reset.count();i++){

		for(int j=0;j!=allEntity->count();j++){
			if(allEntity->entityAt(j)->getGraphNum()==stack_reset.at(i)){
				allEntity->entityAt(j)->setGraphNum(0);

				if (allEntity->entityAt(j)->isLead())
				{
					allEntity->entityAt(j)->setSelected(true);
				}
				else{
					allEntity->entityAt(j)->SetLeadLinkFlag(false);
				}
			}
		}
	}
	moveNoGraphToBottom(allEntity);
	
}

void moveNoGraphToBottom(RS_EntityContainer *allEntity){
	//�Ѳ����ڷ��ͼ�ε�entity �Ƶ�ĩβ
	for(int i=0;i!=allEntity->count();i++){
		if(allEntity->firstEntity()->getGraphNum()!=0)
			break;
		allEntity->MoveToBottom(allEntity->firstEntity());
	}
}

void deleteLeadByG_NUM(unsigned int num,RS_EntityContainer *allEntity){
	QList<RS_Entity*> statck_entity;

	//Find lead line
	for(int i=0;i!=allEntity->count();i++){
		if(num==allEntity->entityAt(i)->getGraphNum()&&allEntity->entityAt(i)->isLead()){
			statck_entity.push_back(allEntity->entityAt(i));
		}
	}

	//delete lead lien
	for(int i=0;i!=statck_entity.count();i++){

		for(int j=0;j!=allEntity->count();j++){
			if(allEntity->entityAt(j)==statck_entity.at(i)){
				allEntity->getEntities().removeAt(j);
			}
		}
	}

}

bool checkSamePoint(RS_Vector point1, RS_Vector point2, long double t)
{
 	if (t <= g_dCadCalAccuracy)
	{
		t = g_dCadCalAccuracy;
		//qWarning()<<"g_dCadCalAccuracy:"<<g_dCadCalAccuracy;
	}

	long double x1=point1.x;
	long double y1=point1.y;

	long double x2=point2.x;
	long double y2=point2.y;

	long double defX=fabs(x1-x2);
	long double defY=fabs(y1-y2);

	if(defX <= t && defY <= t)
	{
		return true;
	}
	else
	{
		return false;
	}

}

//end--------------------new-------------


//Begin Other Way
bool isHasLead(QList<RS_Entity*> *pHave, RS_Document* document, RS_EntityContainer *container)
{
	if(pHave == NULL || container == NULL)
	{
		return false;
	}

	int count     = pHave->count();
	bool bShowQMb = false;

	// �������������е�ʵ��
	int nEntityAt = 0;
	RS_Entity* e  = container->entityAt(nEntityAt);
	while (e != NULL)
	{
		if (e->isLead() && e->isVisible()/* && !e->isUndone()*/)
		{
			RS_Entity *rs_Temp = e->leadLinkEntity();

			for (int i=0; i<count; i++)
			{
				if (rs_Temp == pHave->at(i))
				{
					if (!bShowQMb)
					{
						bShowQMb = true;
						ErrorMessage err;
						if(3 == err.showErrorMessage(G_NDEMO_CAD_ERR_NUM+4,0))
						{
							//��������undo�ṹ����ʱ����
							if (document!=NULL)
							{
								document->startUndoCycle();
								e->setUndoState(true);
								document->addUndoable(e);
								document->endUndoCycle();
							}
							break;
						}
						else
						{
							return true;
						}
					}
					else
					{
						//��������undo�ṹ����ʱ����
						if (document!=NULL) 
						{
							document->startUndoCycle();
							e->setUndoState(true);
							document->addUndoable(e);
							document->endUndoCycle();
						}
					}				
				}

			}
			
		}

		// ��ȡ��һ��ʵ��
		nEntityAt++;
		e = container->entityAt(nEntityAt);
	}

	return false;

}

//local function
bool isInHave(RS_Entity* en,QList<RS_Entity*> *pHave){
	if(pHave==NULL)
		return false;

	int count=pHave->count();

	for(int i=0;i<count;i++){
		if(en==pHave->at(i))
			return true;
	}

	return false;
}

QList<RS_Entity*>* checkIsCloseFigure(En_Line* curLine,RS_EntityContainer* allEntity,QString dir,RS_Entity* o_en,QList<RS_Entity*>* pHave,int ls_num){
	if(pHave==NULL)
		pHave=new QList<RS_Entity*>;

	/*��ʼ������Բ�����*/
	if(curLine->pEn->rtti()==RS2::EntityCircle){
		pHave->push_back(curLine->pEn);

		return pHave;
	}
	/*����������Բ�����*/

	RS_Vector cur_start=curLine->line.getStartpoint();
	RS_Vector cur_end=curLine->line.getEndpoint();

	int cur_num=ls_num;//��¼���ҵ��ڼ�������
	//���ҵ����һ���߶ε�ʱʱ��
	//���ݲ�ͬ�ķ���
	if((curLine->pEn->rtti()==RS2::EntityArc)&&cur_num!=0 || cur_num>=1){//����1���߲��ж�
		if(dir=="R"){
			if((checkSamePoint(cur_end,o_en->getStartpoint())||checkSamePoint(cur_end,o_en->getEndpoint()))&&(curLine->pEn!=o_en)){
				pHave->push_back(curLine->pEn); //��ӵ�ͼ���� pHave

				return pHave;
					
			}
		}
		else if(dir=="L"){
			if((checkSamePoint(cur_start,o_en->getStartpoint())||checkSamePoint(cur_start,o_en->getEndpoint()))&&(curLine->pEn!=o_en)){
				pHave->push_back(curLine->pEn); //��ӵ�ͼ���� pHave

				return pHave;
			}
		}
	}

	cur_num++;

	//modified begin by @wang.k.h
	//����Ƿ�������ͼ��
	int counter=0;
	for(QList<En_Line*>::iterator it=pEn_Map->begin();it!=pEn_Map->end();it++)
	{  //@ for(...)
		RS_Vector next_start=(*it)->line.getStartpoint();
		RS_Vector next_end=(*it)->line.getEndpoint();

		//���߶������ǿɼ��ġ��������ߡ�������pHave������curline�����ʱ�������Ƚ��Ƿ�����������
		if((*it)->pEn->isVisible()&&(!isInHave((*it)->pEn,pHave))&&(curLine->pEn!=(*it)->pEn)&&(!(*it)->pEn->isLead()))
		{
			if((dir=="R" && (checkSamePoint(cur_end,next_start)||checkSamePoint(cur_end,next_end))) ||
				(dir=="L" && (checkSamePoint(cur_start,next_start)||checkSamePoint(cur_start,next_end)))) 
				counter++;
		}
	}
	//������ͼ�ν��д���
	if(counter >= 2)
	{
		//qWarning()<<"ͼ��������!";
		return NULL;
	}
	//modified end 

	//��ʼ����������֮��ѡһ�������Ƚ�
	for(QList<En_Line*>::iterator it=pEn_Map->begin();it!=pEn_Map->end();it++){  //@ for(...)
		RS_Vector next_start=(*it)->line.getStartpoint();
		RS_Vector next_end=(*it)->line.getEndpoint();
		
		//���߶������ǿɼ��ġ��������ߡ�������pHave������curline�����ʱ�������Ƚ��Ƿ�����������
		if((*it)->pEn->isVisible()&&(!isInHave((*it)->pEn,pHave))&&(curLine->pEn!=(*it)->pEn)&&(!(*it)->pEn->isLead())){

			if(dir=="R"){//���ұ߻��ǲ��� end_piont ��ʼ�ȶ��Ƿ��������߶�

				//dir == R ���������ֻ�Ƚ� cur_line ���յ��Ƿ�� next_line����� ���� �յ� ����
				if(checkSamePoint(cur_end,next_start)||checkSamePoint(cur_end,next_end)){
					/*
					��֪�߶������������ڽ�����һ���Ƚ�֮ǰ�Ȱ�cur_line->pEn ����pHave
					*/
					pHave->push_back(curLine->pEn);

					/*��ʼ������һ���Ƚϣ����ҰѱȽϵķ��򴫽�ȥnext_line ������Ϊ��һ���Ƚϵ�cur_line 
						��Ϊnext_line������cur_Line��������next_line���end_point���Ƿ�����������dir==R
					*/
					
					//ȷ��next_line�Ĳ��ҷ���
					/*if(cur_end==next_start){*/
					if(checkSamePoint(cur_end,next_start)){
						dir="R";
					}
					else{
						dir="L";
					}

					//������һ������
					return checkIsCloseFigure(*it,allEntity,dir,o_en,pHave,cur_num);
				}
 			}
			else if(dir=="L"){//����� start_point ��ʼ�����Ƿ����������߶�

				//dir == L ���������ֻ�Ƚ� cur_line ������Ƿ�� next_line����� ���� �յ� ����
				/*if(cur_start==next_start||cur_start==next_end){*/
				if(checkSamePoint(cur_start,next_start)||checkSamePoint(cur_start,next_end)){
					/*
					��֪�߶������������ڽ�����һ���Ƚ�֮ǰ�Ȱ�cur_line->pEn ����pHave
					*/
					pHave->push_back(curLine->pEn);

					/*��ʼ������һ���Ƚϣ����ҰѱȽϵķ��򴫽�ȥnext_line ������Ϊ��һ���Ƚϵ�cur_line 
						��Ϊnext_line������cur_Line��������next_line���end_point���Ƿ�����������dir==R
					*/

					//ȷ��next_line�Ĳ��ҷ���
					/*if(cur_start==next_start){*/
					if(checkSamePoint(cur_start,next_start)){
						dir="R";
					}
					else{
						dir="L";
					}

					return checkIsCloseFigure(*it,allEntity,dir,o_en,pHave,cur_num);
				}
			}

		}
	}//@ for(...)

	delete pHave;
	pHave=NULL;

	return pHave;
}

int deleteLeadByLeadLinkEntity(RS_EntityContainer* allEntity, RS_Entity* en/* =NULL */){
	QList<RS_Entity*> statck_entity;

	//Find lead line
	for(int i=0;i!=allEntity->count();i++){
		if(en==allEntity->entityAt(i)->getLeadLinkEntity()&&allEntity->entityAt(i)->isLead()){
			statck_entity.push_back(allEntity->entityAt(i));
		}
	}

	//delete lead lien
	for(int i=0;i!=statck_entity.count();i++){

		for(int j=0;j!=allEntity->count();j++){
			if(allEntity->entityAt(j)==statck_entity.at(i)){
				allEntity->getEntities().removeAt(j);
			}
		}
	}
	return statck_entity.count();
}
//End Other Way


/*
* Description: 
*	�ж�����������ͼ���Ƿ��Ƿ��ͼ��,�Ƿ�������
* Parameters:
*	RS_Entity *entity            : �ж�ͼ�ε����
*	RS_EntityContainer *container: �洢����ͼ�����ݵ�����
*	QList<RS_Entity*>* pHave     : �洢���ͼ��
* Return Value:
*	int:
*		0,�Ƿ��ͼ����û������;
*		1,���Ƿ��ͼ��;
*		2,ͼ��������;
*/
int checkClosedGraph(RS_Entity *entity, RS_EntityContainer *container, QList<RS_Entity*>* pHave)
{
	RS_Vector rs_Start = entity->getStartpoint();			// ��ȡ���λ��
	RS_Vector rs_End   = entity->getEndpoint();				// �յ�λ��, �����λ�õ����յ�λ��ʱ��Ϊ���ͼ��
	RS_Entity *Temp    = entity;

	pHave->push_back(entity);

	// �����λ�õ����յ�λ��ʱ��Ϊ���ͼ��
	if (judgeTwoPointEqual(rs_Start, rs_End, g_dCadCalAccuracy))
	{
		return 0;
	}

	// ���λ�ò������յ�λ��ʱ
	while (!judgeTwoPointEqual(rs_Start, rs_End, g_dCadCalAccuracy))
	{
		// �������������е�ʵ��
		int nEntityAt = 0;
		int nCount    = 0;		// ��¼���յ���ȵĵ�ĸ���
		RS_Entity* e  = container->entityAt(nEntityAt);
		RS_Entity* s  = NULL;
		while (e != NULL)
		{
			if ((!e->isLead()) && e->isVisible() && (e->getId() != Temp->getId()))
			{
				if (judgeTwoPointEqual(rs_End, e->getStartpoint(), g_dCadCalAccuracy) 
					|| judgeTwoPointEqual(rs_End, e->getEndpoint(), g_dCadCalAccuracy))
				{
					s = e;
					nCount++;
				}

			}

			// ͼ��������
			if (nCount >= 2)		
			{
				return 2;			
			}

			// ��ȡ��һ��ʵ��
			nEntityAt++;
			e = container->entityAt(nEntityAt);
		}

		if (nCount == 1)
		{
			Temp = s;
			pHave->push_back(Temp);
			if (judgeTwoPointEqual(rs_End, s->getStartpoint(), g_dCadCalAccuracy))
			{
				rs_End = s->getEndpoint();
			}
			else if (judgeTwoPointEqual(rs_End, s->getEndpoint(), g_dCadCalAccuracy))
			{
				rs_End = s->getStartpoint();
			}
		}

		// ���Ƿ��ͼ��;
		if (nCount == 0 && !judgeTwoPointEqual(rs_Start, rs_End, g_dCadCalAccuracy))
		{
			return 1;
		}

	}

	return 0;
}


/*
* Funciton Name: judgeTwoPointEqual()
*	�ж��������Ƿ����
* INPUT��
*	Point3D firP:��һ���� 
*	Point3D secP:�ڶ�����
*	double dToleranceError: �������
* OUTPUT��
*   bool
*/
bool judgeTwoPointEqual(RS_Vector firP, RS_Vector secP, double dToleranceError)		
{
	//qWarning() <<"dToleranceError:"<<dToleranceError;
	if (dToleranceError<0)
	{
		dToleranceError = -dToleranceError;
	}

	if (firP.x - secP.x > -dToleranceError && firP.x - secP.x < dToleranceError &&
		firP.y - secP.y > -dToleranceError && firP.y - secP.y < dToleranceError &&
		firP.z - secP.z > -dToleranceError && firP.z - secP.z < dToleranceError)
	{
		return true;
	}

	return false;
}