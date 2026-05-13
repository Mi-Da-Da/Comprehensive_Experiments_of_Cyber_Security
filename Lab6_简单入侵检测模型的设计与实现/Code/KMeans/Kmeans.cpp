#include "Kmeans.h"

/*****************************************************************************************
********************************* ��ȡ�����ļ����� ***************************************
*****************************************************************************************/
bool ReadTestFile(list <strMyRecord*>* pRecordList)
{
	FILE *pInFile;                                     //��¼�ļ���ָ��
	int iTestCount;                                    //����ļ��е�������Ŀ
	char szBuf[MAX_BUF_SIZE];                          //���ļ�����  MAX_BUF_SIZE = 512
	strMyRecord* pRecord;                              //������¼��ָ��

	string strTemp;
	char szSeps[] = ",\r\n";
	
	cout<<"Start reading records from test file!"<<endl;
	outfile<<"Start reading records from test file!"<<endl;	
	
	if ((pInFile = fopen("corrected_datatreat", "r")) == NULL)
	{
		cout<<"Open Test file faied !"<<endl;
		return false;
	}
	else
	{
		iTestCount = 0;
		while (fgets(szBuf, MAX_BUF_SIZE, pInFile) != NULL)
		{
			if(++iTestCount%10000 == 0)
				cout<<setiosflags(ios::left)<<setw(8)<<iTestCount<<"  lines have read ..."<<endl;

			pRecord = new  strMyRecord;
			//02 Э������
			strTemp = strtok(szBuf, szSeps);
			pRecord->fProtocolType = (float)atof(strTemp.c_str());
			//03 ��������
			strTemp = strtok(NULL, szSeps);
			pRecord->fService = (float)atof(strTemp.c_str());
			//04 ״̬��־
			strTemp = strtok(NULL, szSeps);
			pRecord->fStatusFlag = (float)atof(strTemp.c_str());
			//05 Դ��Ŀ���ֽ���
			strTemp = strtok(NULL, szSeps);
			pRecord->fSrcBytes = (float)atof(strTemp.c_str());
			//06 Ŀ�ĵ�Դ�ֽ���
			strTemp = strtok(NULL, szSeps);
			pRecord->fDestBytes = (float)atof(strTemp.c_str());
			//11 ��¼ʧ�ܴ���
			strTemp = strtok(NULL, szSeps);
			pRecord->fFailedLogins = (float)atof(strTemp.c_str());
			//14 �Ƿ���root�û�Ȩ�޲�Ҫ
			strtok(NULL, szSeps);
			//16 root�û�Ȩ�޴�ȡ����
			strTemp = strtok(NULL, szSeps);
			pRecord->fNumofRoot = (float)atof(strTemp.c_str());
			//22 �Ƿ���guest�û���¼��Ҫ
			strtok(NULL, szSeps);
			//23 2����������ͬ������Ŀ
			strTemp = strtok(NULL, szSeps);
			pRecord->fCount = (float)atof(strTemp.c_str());
			//24 2����������ͬ�˿���Ŀ
			strTemp = strtok(NULL, szSeps);
			pRecord->fSrvCount = (float)atof(strTemp.c_str());
			//27 "REJ"���������������
			strTemp = strtok(NULL, szSeps);
			pRecord->fRerrorRate = (float)atof(strTemp.c_str());
			//29 ���ӵ���ͬ�˿�������
			strTemp = strtok(NULL, szSeps);
			pRecord->fSameSrvRate = (float)atof(strTemp.c_str());
			//30 ���ӵ���ͬ�˿�������
			strTemp = strtok(NULL, szSeps);
			pRecord->fDiffSrvRate = (float)atof(strTemp.c_str());
			//33 ��ͬĿ�ĵ���ͬ�˿�������
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSrvCount = (float)atof(strTemp.c_str());
			//34 ��ͬĿ�ĵ���ͬ�˿�����������
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSameSrvRate = (float)atof(strTemp.c_str());
			//35 ��ͬĿ�ĵز�ͬ�˿�����������
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostDiffSrvRate = (float)atof(strTemp.c_str());
			//36 ��ͬĿ�ĵ���ͬԴ�˿����ӱ���
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSameSrcPortRate = (float)atof(strTemp.c_str());
			//37 ��ͬ����������ͬ�˿ڱ���
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSrvDiffHostRate = (float)atof(strTemp.c_str());
			//39 ���ӵ�ǰ������S0����ı���
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSrvSerrorRate = (float)atof(strTemp.c_str());
			//42 ���ͱ�ǩ
			strTemp = strtok(NULL, szSeps);
			pRecord->iLabel = atoi(strTemp.c_str());
			
			//������β�������¼
		  pRecordList->push_back(pRecord);
		}
		
		cout<<iTestCount<<" Records have read from test file!"<<endl;			
		outfile<<iTestCount<<" Records have read from test file!"<<endl;		
		return true;
	}
}

/*****************************************************************************************
**************************** ��Label��int����ת��Ϊstring���� ****************************
*****************************************************************************************/
string LabelInttoStr(int i)
{
	 string strLabel;
   switch(i)
   {
    	case 0:
    		strLabel = "normal";
    		break;
    	case 1:
    		strLabel = "dos";
    		break;
    	case 2:
    		strLabel = "probe";
    		break;
    	case 3:
    		strLabel = "ur2";
    		break;
    	case 4:
    		strLabel = "r2l";
    		break;
    	default:
    		strLabel = "!!ERROR!!";
    		break;    		
    }
    return strLabel;	
}

/*****************************************************************************************
**************************************** Main���� **************************************** 
*****************************************************************************************/
int main()
{		
	int Kvalue;                                        //Kֵ
	int iRightRcdNum;                                  //��¼������ȷ�ļ�¼��
	int TestRcdNum;                                    //�μӼ��ļ�¼����
	string strTrueLabel,strPreLabel;                   //��ʵ��ǩ��Ԥ���ǩ
	
	strMyRecord* pRecord;                              //������¼��ָ��
	list <strMyRecord*>* pTestRcdList;                 //������������
	list<strMyRecord*>::iterator TestListIter;         //��¼�����ĵ�����

	ClusterNode* pClusterNode;	                     //����ڵ�ָ��
	ClusterTree* pClusterTree;                         //������
	
	/*************************************************************************************
	*********** PART1 ����K-Means�㷨����ѵ�����ݼ����е������࣬������������ ************
	*************************************************************************************/
	
	//����������
	pClusterTree = new ClusterTree();
	
	//����KMeans�������
	//���ڵ���Ϊ1������ά��Ϊ19��18������+1����ǩ��
	CKMeans* pRootKMeans = new CKMeans(pClusterTree, ++KmeansID, 1, 19);
	
	//��ȡѵ�����еļ�¼
	cout<<"========== Step 1: Reading training data =========="<<endl;
	outfile<<"========== Step 1: Reading training data =========="<<endl;
	if(!pRootKMeans->ReadTrainingRecords())
	{
		cout<<"Failed to read training data!"<<endl;
		return -1;
	}
	
	//�û�����Kֵ��ֱ�ӽ�Kֵ����Ϊ5
	Kvalue = 5;
	cout<<"========== Step 2: Setting K value = "<<Kvalue<<" =========="<<endl;
	outfile<<"========== Step 2: Setting K value = "<<Kvalue<<" =========="<<endl;
	
	//����Kmeans�㷨
	cout<<"========== Step 3: Running K-Means algorithm =========="<<endl;
	outfile<<"========== Step 3: Running K-Means algorithm =========="<<endl;
	pRootKMeans->RunKMeans(Kvalue);
	
	//��ӡ������
	cout<<"========== Step 4: Printing cluster tree =========="<<endl;
	outfile<<"========== Step 4: Printing cluster tree =========="<<endl;
	pClusterTree->Print();
	
	//����������ӡ����־�ļ���
	pClusterTree->PrintLog();
	cout<<"Cluster tree has been saved to Log.txt"<<endl;

	/****************************************************************************************
	******* PART2 ���þ����� �Բ������ݼ��е����ݽ�������Ԥ�⣬�������׼ȷ�ʺͻ�������******
	****************************************************************************************/  
 
  	//��ȡ�����ļ�
	cout<<"========== Step 5: Reading test data =========="<<endl;
	outfile<<"========== Step 5: Reading test data =========="<<endl;
	pTestRcdList = new list<strMyRecord*>();
	if(!ReadTestFile(pTestRcdList))
	{
		cout<<"Failed to read test data!"<<endl;
		return -1;
	}
	
	//����һ����������
	ConfuseMatrix cfMatrix;
	
	//Ԥ����ȷ�ļ�¼������Ϊ0
	iRightRcdNum = 0;
	
	//������¼������Ϊ0
	TestRcdNum = 0;
	
	//������������������Ϊÿһ�������ҵ���������
	cout<<"========== Step 6: Predicting test data =========="<<endl;
	outfile<<"========== Step 6: Predicting test data =========="<<endl;
	Rstfile<<"========== Prediction Results =========="<<endl;
	
	for(TestListIter = pTestRcdList->begin(); TestListIter != pTestRcdList->end(); TestListIter++)
	{
		pRecord = (*TestListIter);
		TestRcdNum++;
		
		//�ҵ��뵱ǰ��¼��������ľ���ڵ�
		pClusterNode = pClusterTree->FindNearestCluster(pRecord);
		
		//��ȡԤ�������ǩ
		int iPreLabel = pClusterNode->GetClusterNodeLabel();
		int iTrueLabel = pRecord->iLabel;
		
		//ͳ�Ʒ�����ȷ�ļ�¼��
		if(iPreLabel == iTrueLabel)
		{
			iRightRcdNum++;
		}
		
		//���»�������
		cfMatrix.UpdateValue(iTrueLabel, iPreLabel);
		
		//�������¼д��result.txt�ļ���
		strTrueLabel = LabelInttoStr(iTrueLabel);
		strPreLabel = LabelInttoStr(iPreLabel);
		
		Rstfile<<"Record "<<TestRcdNum<<": True = "<<strTrueLabel
		       <<", Predict = "<<strPreLabel;
		if(iPreLabel == iTrueLabel)
		{
			Rstfile<<" [OK]"<<endl;
		}
		else
		{
			Rstfile<<" [WRONG]"<<endl;
		}
		
		//ÿ10000����¼��ӡһ�ν���
		if(TestRcdNum % 10000 == 0)
		{
			cout<<setiosflags(ios::left)<<setw(8)<<TestRcdNum<<"  records have been predicted ..."<<endl;
		}
	}
	
	//������ȷ������
	float fAccuracy = (float)iRightRcdNum / TestRcdNum * 100;
	
	//��ӡ����
	cout<<"========== Step 7: Results Summary =========="<<endl;
	cout<<"Total test records: "<<TestRcdNum<<endl;
	cout<<"Correctly predicted: "<<iRightRcdNum<<endl;
	cout<<"Accuracy: "<<fAccuracy<<"%"<<endl;
	
	//д����־�ļ�
	outfile<<"========== Step 7: Results Summary =========="<<endl;
	outfile<<"Total test records: "<<TestRcdNum<<endl;
	outfile<<"Correctly predicted: "<<iRightRcdNum<<endl;
	outfile<<"Accuracy: "<<fAccuracy<<"%"<<endl;
	
	//��ӡ��������
	cout<<"========== Confusion Matrix =========="<<endl;
	cfMatrix.PrintMatrix();
	
	//�������������������ļ���
	cfMatrix.PrintMatrixToLog();
	
	cout<<"========== Program Finished =========="<<endl;
	cout<<"Results have been saved to Result.txt and Log.txt"<<endl;
	
	//�����ڴ�
	delete pRootKMeans;
	delete pClusterTree;
	delete pTestRcdList;
	
	return 0;

}
