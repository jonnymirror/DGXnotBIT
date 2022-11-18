#include <iostream>
#include <vector>
#include <algorithm>
#include <intrin.h>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>  

using namespace std;

//�萔�J�n
vector<vector<long long int>> G;//���͂����O���t
long long int n;//���_��
long long int m;//�}��
long long int k;//�p�����[�^(�ŏI�I�Ɋ������钸�_�̐��̍ő�l)
long long int l;//�p�����[�^(�Ɖu���������钸�_�̐��̍ő�l)
vector<long long int> T;//�֐�
long long int numbers_of_induced_subgraph_vertices;//�U�������O���t�̒��_��
vector<vector<long long int>> Induced_subgraph;//�U�������O���t

//�萔�I��
//�U�������O���t���v�Z����֐�
vector<bool> making_induced_subgraph(long long int induced_subgraph_n) {
	vector<bool> Exist;//�U�������O���t���v�Z����̂ɗp����z��,���_i���U�������O���t�̒��_�W���ɑ����Ă�����exist[i]=true,�����Ă��Ȃ����exist[i]=false�ł���.
	Exist.resize(n);
	cout << "Exist�̑傫����" << Exist.size() << endl;
	for (long long int i = 0; i < n; i++) {
		Exist[i] = false;
	}
	for (long long int i = 0; i < induced_subgraph_n; i++) {
		long long int a;
		cout << i + 1 << "�ڂ̒��_����͂��Ă�������:" << endl;
		cin >> a;
		a--;
		Exist[a] = true;
	}
	Induced_subgraph.resize(n);
	for (long long int i = 0; i < n; i++) {
		for (long long int j = 0; j < G[i].size(); j++) {
			//i��G[i][j]���ǂ�����U�������O���t�̒��_�W���ɑ�����Ȃ��
			if (Exist[i] && Exist[G[i][j]]) {
				Induced_subgraph[i].push_back(G[i][j]);
			};//
		}
	}
	return Exist;
}

//�ŏI�I�Ɋ������钸�_�����߂�֐�(bit���g��Ȃ�����)
vector<bool> who_is_influenced_not_bit(vector<bool> Exist) {
	vector<bool> Influenced;//���_i���������Ă�����influenced[i]=true,�����łȂ����influenced[i]=false�ł���
	Influenced.resize(n);
	for (long long int i = 0; i < n; i++) {
		Influenced[i] = false;
	}
	//1��ڂ̊g�U�ߒ��̎����J�n
	for (long long int i = 0; i < n; i++) {
		if (Exist[i] && T[i] == 0)Influenced[i] = true;
	}
	//1��ڂ̊g�U�ߒ��̎����I��
	//t��ڂ̊g�U�ߒ��̎����J�n
	bool changed = false;
	do{
		changed = false;
		for (long long int i = 0; i < n; i++) {
			long long int count = 0;//���_u�̗אړ_�Ŋ������Ă��钸�_�̐��𐔂���
			if (Exist[i]&&!Influenced[i]) {
				for (long long int j = 0; j < Induced_subgraph[i].size(); j++) {
					if (Influenced[Induced_subgraph[i][j]])count++;
				}
				if (count >= T[i]) {
					Influenced[i] = true;
					changed = true;
				}
			}
		}
	} while (changed);
	//t��ڂ̊g�U�ߒ��̎����I��
	return Influenced;
}

//|Y(X)|�����߂�֐�
vector<bool> calculate_YX(vector<bool> Influenced,vector<bool> Exist) {
	vector<bool> YX;//���_i��Y(X)�ɑ�����Ȃ�YX[i]=1,�����Ȃ��Ȃ�YX[i]=0
	YX.resize(n);
	for (long long int i = 0; i < n; i++) {
		YX[i] = false;
	}
	for (long long int i = 0; i < n; i++) {
		if (!Exist[i]) {
			long long int count = 0;
			for (long long int j = 0; j < G[i].size(); j++) {
				if (Influenced[G[i][j]])count++;
			}
			if (count >= T[i])YX[i] = true;
		}
	}
	return YX;
}
//���C���֐�
int main() {
	//���͊J�n
	ifstream ifs("papertextsample.txt");

	if (!ifs) {
		std::cout << "Error!";
		return 1;
	}

	string s;
	long long int count = 0;
	long long int x = 0;
	long long int y = 0;

	while (getline(ifs, s, ' ')) {     // �X�y�[�X�i' '�j�ŋ�؂��āC�i�[
		cout << "count:" << count << endl;
		if (count == 0) {//���_��
			n = stoll(s);
			G.resize(n);//�O���t�̑傫���m��
			count++;
		}
		else if (count == 1) {//�}��
			m = stoll(s);
			count++;
		}
		else if (count == 2) {//�p�����[�^k
			k = stoll(s);
			count++;
		}
		else if (count == 3) {//�p�����[�^l
			l = stoll(s);
			count++;
		}
		else if (count > 3 && count < 4 + 2 * m && count % 2 == 0) {//�O���t�̎}�̒[�_
			x = stoll(s);
			x--;
			count++;
		}
		else if (count > 3 && count < 4 + 2 * m && count % 2 == 1) {//�O���t�̎}�̂�����̒[�_
			y = stoll(s);
			y--;
			count++;
			G[x].push_back(y);
			G[y].push_back(x);
		}
		else {//���_��臒l
			x = stoll(s);
			T.push_back(x);
			count++;
		}
	}
	//���͏I��

	//���͐����J�n
	for (int i = 0; i < n; i++) {
		sort(G[i].begin(), G[i].end());//�O���t�̗אڃ��X�g��l�����������ɕ��ׂ�(�ߖT���l���������Ă��邩�ǂ����̊m�F�����鎞�̂��߂�)
	}
	//���͐����I��
	/*���͊m�F�J�n
	cout << "���_��:" << n << endl;
	cout << "�}��:" << m << endl;
	cout << "k:" << k << endl;
	cout << "l:" << l << endl;
	long long int edge_number = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < G[i].size(); j++) {
			cout << "�}" << edge_number + 1 << "�{��:" << i << " " << G[i][j] << endl;
			edge_number++;
		}
	}
	for (int i = 0; i < n; i++) {
		cout << T[i] << endl;
	}
	���͊m�F�I��*/

	/*�������m�F�J�n
	for (int i = 0; i < Monomials.size(); i++) {
		cout << Monomials[i] << endl;
	}
	�������m�F�I��*/

	clock_t start = clock();    //���ԑ���J�n

	cout << "�U�������O���t�̒��_������͂��Ă�������:" << endl;
	cin >> numbers_of_induced_subgraph_vertices;
	vector<bool> Exist=making_induced_subgraph(numbers_of_induced_subgraph_vertices);//�����U���O���t�̍쐬
	vector<bool> A = who_is_influenced_not_bit(Exist);
	for (int i = 0; i < n; i++) {
		if (A[i]) {
			cout << "���_" << i+1 << "�͊������Ă��܂�." << endl;
		}
		else {
			cout << "���_" << i+1 << "�͊������Ă��܂���." << endl;
		}
	}
	vector <bool> YX = calculate_YX(A,Exist);
	for (int i = 0; i < n; i++) {
		if (YX[i]) {
			cout << "���_" << i + 1 << "�ɖƉu��t����΂���." << endl;
		}
		else {
			cout << "���_" << i + 1 << "�ɖƉu��t���Ȃ��Ă���" << endl;
		}
	}
	clock_t end = clock();     // ���ԑ���I��
	cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

	return 0;
}