#include <SFML/Graphics.hpp>
//#include <bits/stdc++.h>
#include <stdio.h> 
#include <direct.h>

using namespace sf;

const int width = 10;
const int heigh = 20;
float delay = 0.3;
int PROVERKA = 0;

int field[width][heigh] = { 0 }; //поле
int field1[width][heigh] = { 0 };

struct Point {
	int x;
	int y;
};

Point dop[4] = { 0 };
Point now[4] = { 0 }; //текущее положение фигуры
Point pred[4] = { 0 }; //предыдущее положение

Point figure[7][4]{ //массив фигур

	{{0,0}, {0,1}, {1, 1}, {1, 2}}, //червь слева

	{{0,0}, {0,1}, {0, 2}, {0, 3}}, //палка
	
	{{0,0}, {0,1}, {0, 2}, {1, 2}}, //L

	{{0,0}, {0,1}, {1, 1}, {0, 2}}, //T

	{{0,0}, {1,0}, {1, 1}, {0, 1}}, //квадр

	{{1,0}, {1,1}, {1, 2}, {0, 2}}, //J

	{{1,0}, {1,1}, {0, 1}, {0, 2}}  //червь справа

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

bool check() // проверка ограничений
{
	for (int i = 0; i < 4; i++)
		if (now[i].x < 0 || now[i].x >= 10 || now[i].y >= 20) return 0;
		else if (field[now[i].x][now[i].y] != 0) return 0;

	return 1;
}

bool check1(int i) // проверка ограничений
{
		if (now[i].x < 0 || now[i].x >= 10 || now[i].y >= 20 || now[i].y < 0) return 0;
		else if (field[now[i].x][now[i].y] != 0) return 0;

	return 1;
}

int main()
{
	bool space = 1;

	srand(time(0));

	ContextSettings settings;
	settings.antialiasingLevel = 8; //сглаживание

	int New = 0;

	Clock clock; // класс часов


	RenderWindow window(VideoMode(900, 900), "Tetris");


	char current_work_dir[255];
	char current_work_dir1[255];

	_getcwd(current_work_dir, sizeof(current_work_dir));
	_getcwd(current_work_dir1, sizeof(current_work_dir));


	strcat_s(current_work_dir, "\\2.png");
	strcat_s(current_work_dir1, "\\over.png");

	Texture texture;
	texture.loadFromFile(current_work_dir);

	Texture texture1;
	texture.loadFromFile(current_work_dir1);

	Sprite Over(texture1);

	Sprite kubick(texture);
	int cvet = 0;
	kubick.setTextureRect(IntRect(cvet * 36, 0, 36, 36));

	Point a{ 0,0 }; // смещение точки
	Point b{ 0,0 };

	int n; // тип фигуры

	float timer = 0;

	bool povorot = 0;

	n = rand() % 6;

	bool over = 1;

	while (window.isOpen()) // Главный цикл приложения
	{
		while (over)
		{
			//время 
			float time = clock.getElapsedTime().asSeconds();
			timer += time;
			//время

			a.x = 0;
			a.y = 0;
			delay = 0.3;
			povorot = 0;
			int res = 0;

			if (New == 0) {
				for (int i = 0; i < 4; i++) {
					now[i].x = figure[n][i].x;
					now[i].y = figure[n][i].y;
					if (!check1(i)) res++;
				}
				if (res == 4) New = 1;
				else {
					for (int i = 0; i < 4; i++) {
						if (!check1(i)) {
							kubick.setPosition(now[i].x * 36, now[i].y * 36);
							kubick.move(270, 90);
							window.draw(kubick);
						}
					}
					over = 0;
				}
			}

			//if (!over) break;
			Event event; // Обработка событий в цикле
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed) window.close(); //нажали крестик

				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::Left) a.x -= 1; //сдвиг влево
					else if (event.key.code == Keyboard::Right) a.x += 1; //сдвиг вправо
					else if (event.key.code == Keyboard::Up) povorot = 1; // поворот
					else if (Keyboard::isKeyPressed(Keyboard::Down)) a.y += 1;
				}


			}

			for (int i = 0; i < 4; i++) { //право лево
				dop[i] = now[i];
				now[i].x += a.x;
				now[i].y += a.y;
			}

			if (!check()) {
				for (int i = 0; i < 4; i++)
					now[i] = dop[i];
			}

			if (timer > delay) { // вниз со временем
				for (int i = 0; i < 4; i++) { dop[i] = now[i]; now[i].y += 1; }
				timer = 0;
				if (!check()) {
					PROVERKA = 1;
					for (int i = 0; i < 4; i++) now[i] = dop[i];
				}
			}
			clock.restart();

			if (povorot) { //поворот

				Point center = now[1];

				for (int i = 0; i < 4; i++) {
					dop[i] = now[i];
					int q = now[i].y;
					int q1 = now[i].x;
					now[i].x = center.x - (q - center.y);
					now[i].y = center.y + (q1 - center.x);
				}

				if (!check()) {
					for (int i = 0; i < 4; i++)
						now[i] = dop[i];
				}
			}

			window.clear(Color::White);

			if (PROVERKA) {
				for (int i = 0; i < 4; i++) field[now[i].x][now[i].y] = cvet % 7 + 1;
				cvet++;
				n = 0 + rand() % 6;
				New = 0;
				PROVERKA = 0;
			}
			else
				for (int i = 0; i < 4; i++) {
					kubick.setPosition(now[i].x * 36, now[i].y * 36);
					kubick.move(270, 90);
					window.draw(kubick);
					pred[i] = now[i];
				}

			////////////////////////////////// удаление строк
			int w = 19;

			for (int i = 19; i > 0; i--) {
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

			////////////////////////////////// удаление строк

			for (int a = 0; a <= 9; a++)
				for (int b = 0; b <= 19; b++) {
					if (field[a][b] != 0) {
						kubick.setTextureRect(IntRect((field[a][b] - 1) * 36, 0, 36, 36));
						kubick.setPosition(a * 36, b * 36);
						kubick.move(270, 90);
						window.draw(kubick);
						kubick.setTextureRect(IntRect((cvet % 7) * 36, 0, 36, 36));
					}
				}

			window.display();
		}
		while (space) {
			Event event1;

			while (window.pollEvent(event1)) {
				if (event1.type == Event::Closed) window.close();
				if (event1.type == Event::KeyPressed) if (event1.key.code == sf::Keyboard::Space) space = 0;
			}

			Texture texture1;
			texture.loadFromFile(current_work_dir1);

			Sprite oover(texture);

			window.clear(Color::White);

			window.draw(oover);

		}
	}

	return 0;
}