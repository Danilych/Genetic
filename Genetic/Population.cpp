#include "Population.h"

Population::Population(int AmountOfSpecimens, std::string FinalWord)
{
	//��������� ������ ��� ������ �����
	PopSpecimens = new Specimen[AmountOfSpecimens]; 
	for (int i = 0; i < AmountOfSpecimens; i++)
		PopSpecimens[i] = Specimen(FinalWord);

	//����������� ������ ����� � �����������
	CoolnessSpecimens = new int[AmountOfSpecimens]; 
	//������ ���������
	_PopAmount = AmountOfSpecimens;
	//��������� ����� l
	_crosslen = AmountOfSpecimens;
	//������ ����� � ���������
	_GenNum = FinalWord.size();
	//���������� ����� � ���� (7 ��� ������� ����)
	_GenBits = 7;
}

Population::Population(int GenNum, int GenBits, int PopAmount, std::string FinalWord)
{
	_GenNum = GenNum; _GenBits = GenBits; _PopAmount = PopAmount;
	PopSpecimens = new Specimen[_PopAmount];
	CoolnessSpecimens = new int[_PopAmount];
	for (int i = 0; i < _PopAmount; i++)
	{
		CoolnessSpecimens[i] = 0;
		PopSpecimens[i] = Specimen(_GenNum, _GenBits);
		for (int j = 0; j < _GenNum; j++)
			CoolnessSpecimens[i] += abs(PopSpecimens[i].GetGenDataDecode(j) - (int)FinalWord[j]);

		//PopSpecimens[i].PrintChrom(); std::cout << GetSpecDecoded(i) << " <" << GetSpecCoolness(i) << ">" << std::endl;
	}
}

void Population::SelectionSlice(float l)
{
	int amount = round(_PopAmount * l);
	int temp = CoolnessSpecimens[0];

	_crosslen = amount; // �������� ����� ������ ������ ��� ���������� ������

	int help = 0;
	int min;

	for (int i = 0; i < _PopAmount; i++) {
		if (CoolnessSpecimens[i] > temp) temp = CoolnessSpecimens[i];
	}

	for (int i = 0; i < _PopAmount; i++) {
		min = temp;
		for (int j = i; j < _PopAmount; j++) {
			if (CoolnessSpecimens[j] <= min) {
				min = CoolnessSpecimens[j];
				help = j;
			}
		}
		std::swap(PopSpecimens[i], PopSpecimens[help]);
		std::swap(CoolnessSpecimens[i], CoolnessSpecimens[help]);
	}
	// ����� ������ �������� � ������� �������
}

Population Population::Cross(float PC, float Lineage, int GenNum, int GenBits, std::string FinalWord, float T)
{
	int amount = round(_PopAmount * Lineage);
	Population newone(_PopAmount, FinalWord);
	_crosslen = amount;
	//����� ��������
	int ToSlice = (Lineage * _PopAmount);
	int CurSpec = 0;
	int _PopAmountCrossed = (_PopAmount * (T));

	while (CurSpec < _PopAmountCrossed)
	{
		//�������� ��������� ���������
		int i = rand() % (ToSlice/2);
		int j = rand() % (ToSlice/2);
		//��������� � ����� ���������, ������� ����� �������� ��� ��� �� ����� ��
		newone[CurSpec] = PopSpecimens[i];
		if (CurSpec + 1 < _PopAmount)
			newone[CurSpec + 1] = PopSpecimens[j];
		//�������� ��������� ����� �������
		int GenNumberR = rand() % GenNum;
		int GenBitR = rand() % GenBits;
		int r = rand() % 101;
		//��������� �����
		if ((PC * 100) >= r)
		{    //������������� ������������ (1-��������)
			//����������� ������� � ������� ��������
			for (int ii = _GenNum - 1; ii >= GenNumberR; ii--)
				if (ii == GenNumberR)
					for (int jj = _GenBits - 1; j >= GenBitR; j--)
						newone[CurSpec].GetGenomsGenBit(ii, jj) = PopSpecimens[j].GetGenomsGenBit(ii, jj);
				else if (ii > GenNumberR)
					for (int jj = _GenBits - 1; j >= 0; j--)
						newone[CurSpec].GetGenomsGenBit(ii, jj) = PopSpecimens[j].GetGenomsGenBit(ii, jj);
			CurSpec++;
			//����������� ������� � ������� ��������
			for (int ii = _GenNum - 1; ii >= GenNumberR; ii--)
				if (CurSpec + 1 < _PopAmount)
				{
					if (ii == GenNumberR)
						for (int jj = _GenBits - 1; j >= GenBitR; j--)
							newone[CurSpec + 1].GetGenomsGenBit(ii, jj) = PopSpecimens[i].GetGenomsGenBit(ii, jj);
					else if (ii > GenNumberR)
						for (int jj = _GenBits - 1; j >= 0; j--)
							newone[CurSpec + 1].GetGenomsGenBit(ii, jj) = PopSpecimens[i].GetGenomsGenBit(ii, jj);
					CurSpec++;
				}
		}
		else CurSpec += 2;
	}

	if(_PopAmount>_PopAmountCrossed)
		for(int i = _PopAmountCrossed; i<_PopAmount;i++)
			newone[i] = PopSpecimens[i-_PopAmountCrossed];

	return newone;
}

Specimen& Population::operator[](int i)
{
	return PopSpecimens[i];
}

void Population::Aptitude(std::string& FinalWord)
{
	if(CoolnessSpecimens == NULL)
		CoolnessSpecimens = new int[_PopAmount];

	//����� ����������� ��� ������ �����
	for (int i = 0; i < _PopAmount; i++)
	{
		CoolnessSpecimens[i] = 0;
		for (int j = 0; j < _GenNum; j++)
			//������� �����������
			CoolnessSpecimens[i] += abs(PopSpecimens[i].GetGenDataDecode(j) - (int)FinalWord[j]);
	}
}

float Population::AvgCool()
{
	float avg = 0;
	for(int i = 0;i<_PopAmount;i++)
		avg += CoolnessSpecimens[i];

	avg = avg / (float)_PopAmount;
	return avg;
}

int Population::GetSpecCoolness(int CurSpec)
{
	return CoolnessSpecimens[CurSpec];
}

Population Population::Mutate(float PM, std::string FinalWord)
{
	Population mutated(_PopAmount, FinalWord);
	float r;
	
	for (int m = 0; m < _PopAmount; m++) {
		for (int i = 0; i < _GenNum; i++) {
			for (int j = 0; j < _GenBits; j++)
			{
				r = (rand() % 101) / 100.0;
				mutated[m].GetGenomsGenBit(i,j) = PopSpecimens[m].GetGenomsGenBit(i,j);
				if (PM > r)
					mutated[m].GetGenomsGenBit(i, j) = !PopSpecimens[m].GetGenomsGenBit(i, j);
			}
		}
	}
	
	return mutated;
}

void Population::Sort()
{
	Specimen* TempSpec;
	int TempCool;

	for (int i = 1; i < _PopAmount; i++)
		for (int j = i - 1; j >= 0; j--)
			if (CoolnessSpecimens[j] > CoolnessSpecimens[j + 1])
			{
				TempSpec = &PopSpecimens[j];
				PopSpecimens[j] = PopSpecimens[j + 1];
				PopSpecimens[j + 1] = *TempSpec;

				TempCool = CoolnessSpecimens[j];
				CoolnessSpecimens[j] = CoolnessSpecimens[j + 1];
				CoolnessSpecimens[j + 1] = TempCool;
			}
}

std::string Population::GetSpecDecoded(int CurSpec)
{
	std::string ResultStr = "";
	for (int i = 0; i < _GenNum; i++) 
		ResultStr += char(PopSpecimens[CurSpec].GetGenDataDecode(i));
	
	return ResultStr;
}
