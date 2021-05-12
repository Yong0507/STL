#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

const int PLAYER_MAX = 100000;

default_random_engine dre;
uniform_int_distribution<> uiNameLen(3, 12);
uniform_int_distribution<> uiName('a', 'z');

class Player {
private:
	string id;
	unsigned int breakout;
	unsigned int championsLeague;
public:
	Player() : breakout(0), championsLeague(0) {};

	void setId(string id) { this->id = id; }
	void setBreakout(unsigned int breakout) { this->breakout = this->breakout < breakout ? breakout : this->breakout; }
	void setChampionsLeague(unsigned int championsLeague) { this->championsLeague = this->championsLeague < championsLeague ? championsLeague : this->championsLeague; }

	string getId() { return id; }
	unsigned int getBreakout() { return breakout; }
	unsigned int getChampionsLeague() { return championsLeague; }

	// 정렬용 비교 함수
	static bool breakoutCompare(const Player& p1, const Player& p2) { return p1.breakout > p2.breakout; }
	static bool championsLeagueCompare(const Player& p1, const Player& p2) { return p1.championsLeague > p2.championsLeague; }
};

// 파일을 불러왔을 때 그냥 읽는다
vector<Player> loadRandPlayer(ifstream& in) {
	vector<Player> players(PLAYER_MAX);

	for (int i = 0; i < PLAYER_MAX; i++) {
		string id;
		unsigned int breakout, champions;

		in >> id >> breakout >> champions;
		players[i].setId(id);
		players[i].setBreakout(breakout);
		players[i].setChampionsLeague(champions);
	}
	return players;
}

// 파일을 못불러오면 100000명 랜덤으로 생성
vector<Player> makeRandPlayer() 
{
	normal_distribution<double> breakoutDistribution(0.0f, 1.0f);
	normal_distribution<double> championsLeagueDistribution(0.0f, 1.0f);

	vector<Player> players(PLAYER_MAX);        

	for (int i = 0; i < PLAYER_MAX; i++) {

		string id;
		for (int i = 0; i < uiNameLen(dre); ++i)
			id += uiName(dre);

		double breakout_Distribution = breakoutDistribution(dre);
		breakout_Distribution = clamp(breakout_Distribution, -5.0, 5.0);
		breakout_Distribution += 5.0;
		breakout_Distribution *= 2905'8870'2.6;

		double championsLeague_Distribution = championsLeagueDistribution(dre);
		championsLeague_Distribution = clamp(championsLeague_Distribution, -5.0, 5.0);
		championsLeague_Distribution += 5.0;
		championsLeague_Distribution *= 1112'6703'8.4;

		players[i].setId(id);
		players[i].setBreakout(breakout_Distribution);
		players[i].setChampionsLeague(championsLeague_Distribution);
	}
	return players;
}

vector<Player> orderByBreakout(const vector<Player>& players)				// 떼탈출 정렬
{		
	vector<Player> v(players);

	sort(v.begin(), v.end(), Player::breakoutCompare);

	return v;
}

vector<Player> orderByChampionsLeague(const vector<Player>& players)		// 챔피언스리그 정렬
{
	vector<Player> v(players);

	sort(v.begin(), v.end(), Player::championsLeagueCompare);

	return v;
}

int findPlayerIdx(vector<Player>& players, const string& name)				// 등수 찾기
{
	for (int i = 0; i < players.size(); i++)
		if (players[i].getId() == name) 
			return i;
	return -1;
}

void updatePlayer(vector<Player>& players) 
{
	normal_distribution<double> chooseDistribution(0.0f, 1.0f);
	normal_distribution<double> breakoutDistribution(0.0f, 1.0f);
	normal_distribution<double> championsLeagueDistribution(0.0f, 1.0f);

	double breakout_Distribution = breakoutDistribution(dre);
	breakout_Distribution = clamp(breakout_Distribution, -5.0, 5.0);
	breakout_Distribution += 5.0;
	breakout_Distribution *= 2905'8870'2.6;

	double championsLeague_Distribution = championsLeagueDistribution(dre);
	championsLeague_Distribution = clamp(championsLeague_Distribution, -5.0, 5.0);
	championsLeague_Distribution += 5.0;
	championsLeague_Distribution *= 1112'6703'8.4;

	for (Player& player : players) {
		// 대략 절반씩 나눠서 출력. 
		if (chooseDistribution(dre) < 0)					// 평균값은 0이므로 , 대략 0 이하 50%, 0 이상 50% 정도가 될 것임 
			player.setBreakout(breakout_Distribution);
		else 
			player.setChampionsLeague(championsLeague_Distribution);
	}
}

void findPlayer(vector<Player>& players, string& player) 
{
	int idx = findPlayerIdx(players, player);

	if (idx == -1) {
		cout << "Not Exist" << '\n';
		return;
	}

	vector<Player> breakouts = orderByBreakout(players);
	vector<Player> champions = orderByChampionsLeague(players);

	int bIdx = findPlayerIdx(breakouts, player);
	int cIdx = findPlayerIdx(champions, player);

	cout << " << Breakout >> " << endl;
	// 0등이 아닌경우
	if (bIdx) 
		cout << "[ " << bIdx - 1 << " ] " << breakouts[bIdx - 1].getId() << " 상위 : " << ((bIdx - 1) * 100.f / PLAYER_MAX) << "% 점수 : " << breakouts[bIdx - 1].getBreakout() << endl;
		cout << "[ " << bIdx << " ] " << breakouts[bIdx].getId() << " 상위 : " << (bIdx * 100.f / PLAYER_MAX) << "% 점수 : " << breakouts[bIdx].getBreakout() << endl;
	// 마지막이 아닌경우
	if ((bIdx + 1) != PLAYER_MAX) 
		cout << "[ " << bIdx + 1 << " ] " << breakouts[bIdx + 1].getId() << " 상위 : " << ((bIdx + 1) * 100.f / PLAYER_MAX) << "% 점수 : " << breakouts[bIdx + 1].getBreakout() << endl;

	cout << "<< Champions League >> " << endl;
	if (cIdx) 
		cout << "[ " << cIdx - 1 << " ] " << champions[cIdx - 1].getId() << " 상위 : " << ((cIdx - 1) * 100.f / PLAYER_MAX) << "% 점수 : " << champions[cIdx - 1].getChampionsLeague() << endl;
		cout << "[ " << cIdx << " ] " << champions[cIdx].getId() << " 상위 : " << (cIdx * 100.f / PLAYER_MAX) << "% 점수 : " << champions[cIdx].getChampionsLeague() << endl;
	if ((cIdx + 1) != PLAYER_MAX) 
		cout << "[ " << cIdx + 1 << " ] " << champions[cIdx + 1].getId() << " 상위 : " << ((cIdx + 1) * 100.f / PLAYER_MAX) << "% 점수 : " << champions[cIdx + 1].getChampionsLeague() << endl;
}

void savePlayer(vector<Player>& players, const string& filename) {
	ofstream out;
	out.open(filename);

	for (Player& player : players) out << player.getId() << ' ' << player.getBreakout() << ' ' << player.getChampionsLeague() << '\n';

	out.close();
}

int main()
{
	string filename = "players.txt"; // 파일 명
	ifstream in;
	in.open(filename);
	vector<Player> players;
	players.reserve(10'0000);
	players = in.is_open() ? loadRandPlayer(in) : makeRandPlayer(); // 파일이 존재하면 load, 없으면 새로 생성
	in.close(); // 다 읽었으면 close

	while (true) {
		cout << "[Menu]" << endl << "[1] Update [2] Find [3] Exit" << endl;
		int select;
		cin >> select;

		if (select == 1) {
			updatePlayer(players);
			cout << "Updated.. " << endl;
		}
		else if (select == 2) {
			cout << "User's id ? ";
			string name; 
			cin >> name;
			findPlayer(players, name);
		}
		else if (select == 3) 
			break;
	}

	savePlayer(players, filename);

	return 0;
}
