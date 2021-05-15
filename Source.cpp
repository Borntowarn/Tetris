#include <SFML/Graphics.hpp>
#include <stdio.h> 
#include <direct.h>
#include "windows.h"
#include <SFML/Audio.hpp>

using namespace sf;

const int width = 10;
const int heigh = 20;
float Delay = 0.6;
int PROVERKA = 0;
int value_n = 0;
bool GENERAL = 0;

int field[width][heigh] = { 0 }; //����
int field1[width][heigh] = { 0 };

struct Point {
	int x;
	int y;
};

Point dop[4] = { 0 };
Point now[4] = { 0 }; //������� ��������� ������
Point pred[4] = { 0 }; //���������� ���������

Point figure[7][4]{ //������ �����

	{{0,0}, {0,1}, {1, 1}, {1, 2}}, //����� �����

	{{0,0}, {0,1}, {0, 2}, {0, 3}}, //�����

	{{0,0}, {0,1}, {0, 2}, {1, 2}}, //L

	{{0,0}, {0,1}, {1, 1}, {0, 2}}, //T

	{{0,0}, {1,0}, {1, 1}, {0, 1}}, //�����

	{{1,0}, {1,1}, {1, 2}, {0, 2}}, //J

	{{1,0}, {1,1}, {0, 1}, {0, 2}}  //����� ������

};

int max(int a, int b, int c, int d) {
	int m = 0;
	if (m < a) m = a;
	if (m < b) m = b;
	if (m < c) m = c;
	if (m < d) m = d;
	return m;
}

int min(int a, int b, int c, int d) {
	int m = 999;
	if (m > a) m = a;
	if (m > b) m = b;
	if (m > c) m = c;
	if (m > d) m = d;
	return m;
}

bool check() // �������� �����������
{
	for (int i = 0; i < 4; i++)
		if (now[i].x < 0 || now[i].x >= 10 || now[i].y >= 20) return 0;
		else if (field[now[i].x][now[i].y] != 0) return 0;

	return 1;
}

bool check2() // �������� �����������
{
	for (int i = 0; i < 4; i++)
		if (now[i].y <= 0) return 0;

	return 1;
}

bool check1() // �������� �����������
{
	for (int i = 0; i < 4; i++)
		if (field[now[i].x][now[i].y] != 0 && now[i].y >= 0) return 0;

	return 1;
}

int main()
{
	bool space = 1;

	srand(time(0));

	ContextSettings settings;
	settings.antialiasingLevel = 8; //�����������

	int New = 0;

	Clock clock; // ����� �����


	RenderWindow window(VideoMode(900, 900), "Tetris");

	Music music;//������� ������ ������

	music.openFromFile("zvuk1.wav");//��������� ����

	Texture texture;
	texture.loadFromFile("2.png");

	Texture texture1;
	texture1.loadFromFile("over.png");

	Texture Theme;
	Theme.loadFromFile("Theme.png");

	Sprite Over(texture1);

	Sprite theme(Theme);

	Sprite kubick(texture);
	int cvet = 0;
	kubick.setTextureRect(IntRect(cvet * 36, 0, 36, 36));

	Point a{ 0,0 }; // �������� �����
	Point b{ 0,0 };

	int n; // ��� ������
	int n_new; // ����� ������

	float timer = 0;

	bool povorot = 0;

	n = rand() % 6;
	n_new = rand() % 6;

	bool over = 0;

	Event event;

	bool prov = 1;

	bool newprov = 0;

	while (window.isOpen()) // ������� ���� ����������
	{	
		while (window.pollEvent(event))
			if (event.type == Event::KeyPressed || GENERAL)
				if (event.key.code == Keyboard::Space || GENERAL) {
					GENERAL = 1;
					music.play();
					while (!over)
					{

						//����� 
						float time = clock.getElapsedTime().asSeconds();
						timer += time;
						//�����

						prov = 1;

						space = 1;

						a.x = 0;
						a.y = 0;
						povorot = 0;
						PROVERKA = 0;

						// ����� ������
						if (New == 0) {
							for (int i = 0; i < 4; i++) {
								now[i].x = figure[n][i].x + 4;
								now[i].y = figure[n][i].y;
							}
							if (check()) {
								New = 1;
								value_n++;
								if (Delay > 0.2) if (value_n % 20 == 0) Delay -= 0.05;
							}
							else { // ���� ��� ����� �� ������ ������ ������ �������
								over = 1;
								PROVERKA = 1;
							}
						}

						// ��������� ������� � �����
						while (window.pollEvent(event))
						{
							if (event.type == Event::Closed)
							{
								window.close(); //������ �������
								music.stop();
								return 0;
							}

							if (event.type == Event::KeyPressed) {
								if (event.key.code == Keyboard::Left) a.x -= 1; //����� �����
								else if (event.key.code == Keyboard::Right) a.x += 1; //����� ������
								else if (event.key.code == Keyboard::Up) povorot = 1; // �������
								else if (Keyboard::isKeyPressed(Keyboard::Down)) a.y += 1;
							}


						}

						for (int i = 0; i < 4; i++) { //����� ����
							dop[i] = now[i];
							now[i].x += a.x;
							now[i].y += a.y;
						}


						if (!check() || over) {
							for (int i = 0; i < 4; i++)
								now[i] = dop[i];
						}

						if (timer > Delay) { // ���� �� ��������
							for (int i = 0; i < 4; i++) { dop[i] = now[i]; now[i].y += 1; }
							timer = 0;
							if (!check2()) over = 1;
							if (!check() || over) {
								PROVERKA = 1;
								for (int i = 0; i < 4; i++) now[i] = dop[i];
							}
						}
						clock.restart();

						if (povorot)
							if (n != 4) { //�������

								Point center = now[1];

								for (int i = 0; i < 4; i++) {
									dop[i] = now[i];
									int q = now[i].y;
									int q1 = now[i].x;
									now[i].x = center.x - (q - center.y);
									now[i].y = center.y + (q1 - center.x);
								}

								// �������� � �����
								int min_x = min(now[0].x, now[1].x, now[2].x, now[3].x);
								int max_x = max(now[0].x, now[1].x, now[2].x, now[3].x);

								if (min_x < 0) for (int i = 0; i < 4; i++) now[i].x += abs(min_x);
								if (max_x > 9) for (int i = 0; i < 4; i++) now[i].x -= max_x % 9;
								//

								if (!check()) {
									for (int i = 0; i < 4; i++)
										now[i] = dop[i];
								}
							}

						window.clear(Color::White);

						window.draw(theme);


						if (PROVERKA) {

							if (over) { // ���� ����� �� ������ ������, ������ ������ �����
								while (!check1())
									for (int i = 0; i < 4; i++)
										now[i].y--;
								for (int i = 0; i < 4; i++)
									if (now[i].y >= 0)
										field[now[i].x][now[i].y] = cvet % 7 + 1;
							}

							else {
								for (int i = 0; i < 4; i++) field[now[i].x][now[i].y] = cvet % 7 + 1;
								cvet++;
								n = n_new;
								n_new = 0 + rand() % 6;
								New = 0;
								PROVERKA = 0;
							}

						}
						else {
							for (int i = 0; i < 4; i++) {
								kubick.setPosition(now[i].x * 36, now[i].y * 36);
								kubick.move(270, 90);
								window.draw(kubick);
							}
							for (int i = 0; i < 4; i++) { // ���� ������
								kubick.setTextureRect(IntRect(((cvet + 1) % 7) * 36, 0, 36, 36));
								kubick.setPosition(figure[n_new][i].x * 36, figure[n_new][i].y * 36);
								kubick.move(693, 90);
								window.draw(kubick);
							}
							kubick.setTextureRect(IntRect((cvet) * 36, 0, 36, 36));

						}

						////////////////////////////////// �������� �����
						int w = 19;

						for (int i = 19; i >= 0; i--) {
							int k = 0;
							for (int q = 0; q < 10; q++) if (field[q][i] == 0) {
								k++;
								break;
							}
							if (k != 0) {
								for (int q = 0; q < 10; q++) field1[q][w] = field[q][i];
								w--;
							}
						}

						if (w != 19)
							for (int a = 0; a <= 9; a++)
								for (int b = 0; b <= 19; b++) {
									field[a][b] = field1[a][b];
								}

						////////////////////////////////// �������� �����

						for (int a = 0; a <= 9; a++)
							for (int b = 0; b <= 19; b++) {
								if (field[a][b] != 0) {
									kubick.setTextureRect(IntRect((field[a][b] - 1) * 36, 0, 36, 36));
									kubick.setPosition(a * 36, b * 36);
									kubick.move(270, 90);
									window.draw(kubick);
								}
							}
						window.display();
						kubick.setTextureRect(IntRect((cvet % 7) * 36, 0, 36, 36));
					}

					// ��� ���������� ���� ������� ������ ���
					for (int a = 0; a <= 9; a++)
						for (int b = 0; b <= 19; b++) {
							if (field[a][b] != 0) {
								kubick.setTextureRect(IntRect((field[a][b] - 1) * 36, 0, 36, 36));
								kubick.setPosition(a * 36, b * 36);
								kubick.move(270, 90);
								window.draw(kubick);
							}
						}
					window.display();



					while (space) { // ����� ����, ���� �� ������

						if (prov) //��������� ��������
							for (int i = 0; i < 50; i++) {
								Over.setColor(Color(255, 255, 255, i));
								Sleep(15);
								window.draw(Over);
								window.display();
							}
						prov = 0;

						while (window.pollEvent(event)) {
							if (event.type == Event::Closed) { window.close(); return 0; } //������ �������

							if (event.type == Event::KeyPressed)
								if (event.key.code == Keyboard::Space) {
									space = false;
									over = 0;
									//////////////// ���������

									for (int a = 0; a <= 9; a++)
										for (int b = 0; b <= 19; b++) {
											field[a][b] = 0;
											field1[a][b] = 0;
										}
									Point dop[4] = { 0 };
									Point now[4] = { 0 };
									Point pred[4] = { 0 };

									value_n = 0;
									Delay = 0.6;


									////////////////
								}
						}
						window.display();
						music.stop();
					}
				}
	}

	return 0;
}