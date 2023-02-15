#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Lee.h"

int mat[NMAX][NMAX];
int mat_drum[NMAX][NMAX];
int n, m;
std::vector <sf::RectangleShape> rect;
int i_start, j_start, i_final, j_final;
float r_width = 100.0f;

bool start = false;
bool sfr = false;
bool maze = false;

bool solved = false;

bool search = true;
bool reset = true;

bool add = true;
bool del = false;

bool af1 = false;
bool af2 = false;
bool af3 = false;

int step = 1;

void interfata()
{
	if (!af1) {
		std::cout << "\nAlege punctul de start\n";
		af1 = true;
	}
	else if (!af2 && start) {
		std::cout << "\nAlege punctul final\n";
		af2 = true;
	}
	else if (!af3 && sfr) {
		std::cout << "\nDeseneaza labirintul\nApasa Space pentru a rezolva\n";
		af3 = true;
	}
}

void drawMaze(sf::RenderWindow& window, sf::Vector2i mouseClick)
{
	if (!maze) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				int k = i * m + j;

				window.draw(rect[k]);

				int rx = (int)rect[k].getPosition().x;
				int ry = (int)rect[k].getPosition().y;

				if (mouseClick.x > rx && mouseClick.x < rx + r_width &&
					mouseClick.y > ry && mouseClick.y < ry + r_width) {

					if (!start) {
						rect[k].setFillColor(sf::Color::Green);

						i_start = i + 1;
						j_start = j + 1;

						start = true;
					}
					else if (!sfr && rect[k].getFillColor() != sf::Color::Green) {
						rect[k].setFillColor(sf::Color::Blue);

						i_final = i + 1;
						j_final = j + 1;

						sfr = true;
					}
					else if (rect[k].getFillColor() != sf::Color::Green && rect[k].getFillColor() != sf::Color::Blue) {
						if (add) {
							rect[k].setFillColor(sf::Color::Black);

							mat[i + 1][j + 1] = -1;
						}
						if (del) {
							rect[k].setFillColor(sf::Color::White);

							mat[i + 1][j + 1] = 0;
						}
					}
				}
			}
		}
	}
}

void searchAnimation(sf::RenderWindow& window)
{
	window.setFramerateLimit(20);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int k = i * m + j;

			int im = i + 1;
			int jm = j + 1;

			if (mat[im][jm] == -1)
				rect[k].setFillColor(sf::Color::Black);
			else if (im == i_start && jm == j_start)
				rect[k].setFillColor(sf::Color::Green);
			else if (im == i_final && jm == j_final)
				rect[k].setFillColor(sf::Color::Blue);
			else if (mat[im][jm] == step)
				rect[k].setFillColor(sf::Color::Cyan);
			if (mat[im][jm] == step && im == i_final && jm == j_final)
				search = false;
			window.draw(rect[k]);
		}
	}
	step++;

}

void resetMaze()
{
	if (reset) {
		for (int i = 0; i < n*m; i++)
			rect[i].setFillColor(sf::Color::White);
		step = 1;
		reset = false;
	}
}

void solveAnimation(sf::RenderWindow& window)
{
	window.setFramerateLimit(5);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int k = i * m + j;

			int im = i + 1;
			int jm = j + 1;

			if (mat[im][jm] == -1)
				rect[k].setFillColor(sf::Color::Black);
			else if (im == i_start && jm == j_start)
				rect[k].setFillColor(sf::Color::Green);
			else if (im == i_final && jm == j_final)
				rect[k].setFillColor(sf::Color::Blue);
			else if (mat_drum[im][jm] == step)
				rect[k].setFillColor(sf::Color::Color(224, 47, 47, 255));

			window.draw(rect[k]);
		}
	}
	step++;
}

int main()
{	
	std::cout << "Da marimea hartii:\n";
	std::cin >> m >> n;

	int maxWidth = 900;
	int maxHeight = 900;

	int width = (int)(m * r_width);
	int height = (int)(n * r_width);
	if (width > maxWidth || height > maxHeight) {
		float c = (float)std::max(width, height) / maxWidth;
		width = (int)(width / c);
		height = (int)(height / c);
		r_width = (float)r_width / c;
	}
	
	sf::RenderWindow window(sf::VideoMode(width, height), "Labirint", sf::Style::Close);
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			sf::RectangleShape r(sf::Vector2f(r_width, r_width));
			r.setOutlineColor(sf::Color::Black);
			r.setOutlineThickness(2);
			r.setFillColor(sf::Color::White);
			r.setPosition(j*r_width, i*r_width);

			rect.push_back(r);
		}
	}
	
	while (window.isOpen()) {
		sf::Event event;
		sf::Vector2i mouseClick(-1, -1);

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				mouseClick = sf::Mouse::getPosition(window);
				if (event.key.code == sf::Mouse::Left) {
					add = true;
					del = false;
				}
				if (event.key.code == sf::Mouse::Right) {
					add = false;
					del = true;
				}
			}
			if (start && sfr) {
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Space)
						maze = true;
				}
			}
		}

		interfata();
	
		drawMaze(window, mouseClick);
		
		if (maze && !solved) {
			lee(mat, n, m, i_start, j_start, i_final, j_final, mat_drum);
			solved = true;
		}
		
		if (maze && solved) {
			if(search)
				searchAnimation(window);
			else {
				resetMaze();
				solveAnimation(window);
			}
		}

		window.display();
	}

	//std::cin.get();
	return 0;
}
