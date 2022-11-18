#include <iostream>
#include <vector>
#include <algorithm>
#include <intrin.h>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>  

using namespace std;

//定数開始
vector<vector<long long int>> G;//入力されるグラフ
long long int n;//頂点数
long long int m;//枝数
long long int k;//パラメータ(最終的に感染する頂点の数の最大値)
long long int l;//パラメータ(免疫を持たせる頂点の数の最大値)
vector<long long int> T;//関数
long long int numbers_of_induced_subgraph_vertices;//誘導部分グラフの頂点数
vector<vector<long long int>> Induced_subgraph;//誘導部分グラフ

//定数終了
//誘導部分グラフを計算する関数
vector<bool> making_induced_subgraph(long long int induced_subgraph_n) {
	vector<bool> Exist;//誘導部分グラフを計算するのに用いる配列,頂点iが誘導部分グラフの頂点集合に属していたらexist[i]=true,属していなければexist[i]=falseである.
	Exist.resize(n);
	cout << "Existの大きさは" << Exist.size() << endl;
	for (long long int i = 0; i < n; i++) {
		Exist[i] = false;
	}
	for (long long int i = 0; i < induced_subgraph_n; i++) {
		long long int a;
		cout << i + 1 << "つ目の頂点を入力してください:" << endl;
		cin >> a;
		a--;
		Exist[a] = true;
	}
	Induced_subgraph.resize(n);
	for (long long int i = 0; i < n; i++) {
		for (long long int j = 0; j < G[i].size(); j++) {
			//iとG[i][j]がどちらも誘導部分グラフの頂点集合に属するならば
			if (Exist[i] && Exist[G[i][j]]) {
				Induced_subgraph[i].push_back(G[i][j]);
			};//
		}
	}
	return Exist;
}

//最終的に感染する頂点を求める関数(bitを使わない実装)
vector<bool> who_is_influenced_not_bit(vector<bool> Exist) {
	vector<bool> Influenced;//頂点iが感染していたらinfluenced[i]=true,そうでなければinfluenced[i]=falseである
	Influenced.resize(n);
	for (long long int i = 0; i < n; i++) {
		Influenced[i] = false;
	}
	//1回目の拡散過程の実装開始
	for (long long int i = 0; i < n; i++) {
		if (Exist[i] && T[i] == 0)Influenced[i] = true;
	}
	//1回目の拡散過程の実装終了
	//t回目の拡散過程の実装開始
	bool changed = false;
	do{
		changed = false;
		for (long long int i = 0; i < n; i++) {
			long long int count = 0;//頂点uの隣接点で感染している頂点の数を数える
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
	//t回目の拡散過程の実装終了
	return Influenced;
}

//|Y(X)|を求める関数
vector<bool> calculate_YX(vector<bool> Influenced,vector<bool> Exist) {
	vector<bool> YX;//頂点iがY(X)に属するならYX[i]=1,属さないならYX[i]=0
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
//メイン関数
int main() {
	//入力開始
	ifstream ifs("papertextsample.txt");

	if (!ifs) {
		std::cout << "Error!";
		return 1;
	}

	string s;
	long long int count = 0;
	long long int x = 0;
	long long int y = 0;

	while (getline(ifs, s, ' ')) {     // スペース（' '）で区切って，格納
		cout << "count:" << count << endl;
		if (count == 0) {//頂点数
			n = stoll(s);
			G.resize(n);//グラフの大きさ確保
			count++;
		}
		else if (count == 1) {//枝数
			m = stoll(s);
			count++;
		}
		else if (count == 2) {//パラメータk
			k = stoll(s);
			count++;
		}
		else if (count == 3) {//パラメータl
			l = stoll(s);
			count++;
		}
		else if (count > 3 && count < 4 + 2 * m && count % 2 == 0) {//グラフの枝の端点
			x = stoll(s);
			x--;
			count++;
		}
		else if (count > 3 && count < 4 + 2 * m && count % 2 == 1) {//グラフの枝のもう一つの端点
			y = stoll(s);
			y--;
			count++;
			G[x].push_back(y);
			G[y].push_back(x);
		}
		else {//頂点の閾値
			x = stoll(s);
			T.push_back(x);
			count++;
		}
	}
	//入力終了

	//入力整理開始
	for (int i = 0; i < n; i++) {
		sort(G[i].begin(), G[i].end());//グラフの隣接リストを値が小さい順に並べる(近傍多様性が合っているかどうかの確認をする時のために)
	}
	//入力整理終了
	/*入力確認開始
	cout << "頂点数:" << n << endl;
	cout << "枝数:" << m << endl;
	cout << "k:" << k << endl;
	cout << "l:" << l << endl;
	long long int edge_number = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < G[i].size(); j++) {
			cout << "枝" << edge_number + 1 << "本目:" << i << " " << G[i][j] << endl;
			edge_number++;
		}
	}
	for (int i = 0; i < n; i++) {
		cout << T[i] << endl;
	}
	入力確認終了*/

	/*多項式確認開始
	for (int i = 0; i < Monomials.size(); i++) {
		cout << Monomials[i] << endl;
	}
	多項式確認終了*/

	clock_t start = clock();    //時間測定開始

	cout << "誘導部分グラフの頂点数を入力してください:" << endl;
	cin >> numbers_of_induced_subgraph_vertices;
	vector<bool> Exist=making_induced_subgraph(numbers_of_induced_subgraph_vertices);//部分誘導グラフの作成
	vector<bool> A = who_is_influenced_not_bit(Exist);
	for (int i = 0; i < n; i++) {
		if (A[i]) {
			cout << "頂点" << i+1 << "は感染しています." << endl;
		}
		else {
			cout << "頂点" << i+1 << "は感染していません." << endl;
		}
	}
	vector <bool> YX = calculate_YX(A,Exist);
	for (int i = 0; i < n; i++) {
		if (YX[i]) {
			cout << "頂点" << i + 1 << "に免疫を付ければいい." << endl;
		}
		else {
			cout << "頂点" << i + 1 << "に免疫を付けなくていい" << endl;
		}
	}
	clock_t end = clock();     // 時間測定終了
	cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

	return 0;
}