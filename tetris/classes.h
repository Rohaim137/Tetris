#include <SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;



class coords {
private:
	float x, y;
public:
	coords() {
		x = 0;
		y = 0;
	}
	coords(float a, float b) {
		x = a;
		y = b;
	}
	float get_x() {
		return x;
	}
	float get_y() {
		return y;
	}
	void set_x(float a) {
		x = a;
	}
	void set_y(float b) {
		y = b;
	}


};

class point {
private:
	float x, y;
public:
	point() {
		x = 5; y = -2;
	}
	point(float a, float b) {
		x = a;
		y = b;
	}

	coords to_coords() {

		coords C((50 + (x * 30)) + 2, (80 + (y * 30)) + 2);

		return C;
	}
	void increment_x(float i) {
		x = x + i;
	}
	void increment_y(float i) {
		y = y + i;
	}

	float get_x() {
		return x;
	}
	float get_y() {
		return y;
	}
	void set_x(float a) {
		x = a;
	}
	void set_y(float b) {
		y = b;
	}

	//resetting to top of grid
	void reset() {
		x = 5;
		y = -2;
	}

};




class well {
private:
	int grid[20][10];

public:
	well() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				grid[i][j] = -1;
			}
		}
	}

	sf::RectangleShape draw() {
		sf::RectangleShape rectangle(sf::Vector2f(300.f, 600.f));


		rectangle.setFillColor(sf::Color(255, 255, 255));
		rectangle.setPosition(50, 80);
		rectangle.setOutlineThickness(5.f);
		rectangle.setOutlineColor(sf::Color(255, 0, 0));
		return rectangle;
	}


	int* operator[](int i) {
		return grid[i];
	}

	void erase_grid() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				grid[i][j] = -1;
			}
		}

	}

	bool isEmpty(int x, int y) {//pass the x and y values
		if (x <= 9 && y < 0 && x >= 0) {
			return true;
		}
		else if (x > 9 || y > 19 || x < 0) {
			return false;
		}
		else if (grid[y][x] == -1) {
			return true;
		}
		return false;
	}

	void fill(int x, int y, int i) {
		if (y >= 0) {
			grid[y][x] = i;
		}
	}

	int check_lines() {
		int lines = 0;
		bool filled;
		for (int i = 0; i < 20; i++) {
			filled = true;
			for (int j = 0; j < 10; j++) {
				if (grid[i][j] == -1) {
					filled = false;
				}
			}
			if (filled == true) {
				lines++;
				// shift down
				for (int k = i; k > 0; k--) {
					for (int l = 0; l < 10; l++) {
						grid[k][l] = grid[k - 1][l];
					}

				}

			}
		}
		return lines;
	}

	bool gameover() {
		bool is_game_over = false;
		for (int j = 0; j < 10; j++) {
			if (grid[0][j] != -1) {
				is_game_over = true;
			}
		}
		return is_game_over;
	}




};


class tetramino {
protected:
	point top_left;
	int current_rotation;
	int R, G, B;
public:
	tetramino() {
		current_rotation = 0;
		R = 0;
		G = 0;
		B = 0;
	}

	point get_top_left() {
		return top_left;

	}

	int get_current_rotation() {
		return current_rotation;
	}

	void get_RGB(int& r, int& g, int& b) {
		r = R;
		g = G;
		b = B;
	}


	//parent draw_shape
	virtual void draw_shape(sf::RenderWindow& window) = 0;

	virtual void move_left(well grid) = 0;

	virtual void move_right(well grid) = 0;

	virtual bool falling_shape(well& grid) = 0;

	virtual void rotate(well grid) = 0;

	//to draw the building block
	void draw_square(sf::RenderWindow& window, coords C, int R, int G, int B, sf::Texture& texture) {
		sf::RectangleShape square(sf::Vector2f(26.f, 26.f));
		square.setTexture(&texture);
		square.setPosition(C.get_x(), C.get_y());
		square.setFillColor(sf::Color(255, 255, 255));

		// setting outline
		square.setOutlineThickness(2.f);
		square.setOutlineColor(sf::Color(0, 0, 0));
		window.draw(square);
	}

	void reset() {
		current_rotation = 0;
		top_left.reset();
	}





	virtual ~tetramino() {

	}


};

class I_block : public tetramino {
private:

public:
	I_block() {
		current_rotation = 0;
		R = 3;
		G = 252;
		B = 215;
	}
	virtual void draw_shape(sf::RenderWindow& window) {
		sf::Texture texture;
		if (!texture.loadFromFile("Iblock.png"))
		{
			// error...
		}

		if (current_rotation == 0) {

			point temp = top_left;
			for (int i = 0; i < 4; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			point temp = top_left;
			for (int i = 0; i < 4; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
		}

	}

	virtual void rotate(well grid) {
		int prev_rotation = current_rotation;
		point temp = top_left;
		current_rotation++;
		current_rotation = current_rotation % 2;
		bool empty = false;

		if (current_rotation == 0) {

			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true) {
				empty = true;
				for (int i = 0; i < 4; i++) {
					if (grid.isEmpty(top_left.get_x(), top_left.get_y() + i) == false) {
						empty = false;
					}
				}
				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}

		}
		else if (current_rotation == 1) {
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true) {
				empty = true;
				for (int i = 0; i < 4; i++) {
					if (grid.isEmpty(top_left.get_x() + i, top_left.get_y()) == false) {
						empty = false;
					}
				}
				if (empty == false) {
					top_left.increment_x(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}
	}

	virtual void move_left(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				for (int i = 0; i < 4; i++) {
					if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + i) == false) {
						blocked = true;
					}
				}
			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(-1);
		}
	}

	virtual void move_right(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() < 9) {
				blocked = false;
			}
			if (blocked == false) {
				for (int i = 0; i < 4; i++) {
					if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + i) == false) {
						blocked = true;
					}
				}
			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() < 6) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 4, top_left.get_y()) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(1);
		}
	}

	virtual bool falling_shape(well& grid) {
		bool collision = false;
		if (current_rotation == 0) {
			if ((top_left.get_y() + 3) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 4) == false) {
				collision = true;
			}

			if (collision == true) {

				for (int i = 0; i < 4; i++) {
					grid.fill(top_left.get_x(), top_left.get_y() + i, 0);

				}

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			if ((top_left.get_y()) == 19) {
				collision = true;
			}
			else {
				for (int i = 0; i < 4; i++) {
					if (grid.isEmpty(top_left.get_x() + i, top_left.get_y() + 1) == false) {
						collision = true;
					}

				}
			}
			if (collision == true) {

				for (int i = 0; i < 4; i++) {
					grid.fill(top_left.get_x() + i, top_left.get_y(), 0);


				}

				reset();
			}
			else {
				top_left.increment_y(1);
			}

		}
		if (collision == false) {
			return true;
		}
		return false;


	}

	virtual ~I_block() {

	}

};

class J_block : public tetramino {
private:
public:
	J_block() {
		current_rotation = 3;
		R = 55;
		G = 4;
		B = 184;
	}
	virtual void draw_shape(sf::RenderWindow& window) {
		sf::Texture texture;
		if (!texture.loadFromFile("Jblock.png"))
		{
			// error...
		}

		if (current_rotation == 0) {
			point temp = top_left;
			coords C = temp.to_coords();
			temp.increment_x(1);
			temp.increment_y(-2);
			draw_square(window, C, R, G, B, texture);
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			point temp = top_left;
			coords C = temp.to_coords();
			temp.increment_y(1);
			draw_square(window, C, R, G, B, texture);
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}

		}
		else if (current_rotation == 2) {
			point temp = top_left;
			temp.increment_x(1);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);
			temp.increment_x(-1);

			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}


		}
		else if (current_rotation == 3) {
			point temp = top_left;

			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
			temp.increment_x(-1);
			temp.increment_y(1);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);

		}

	}

	virtual void rotate(well grid) {
		int prev_rotation = current_rotation;
		point temp = top_left;
		current_rotation++;
		current_rotation = current_rotation % 4;
		bool empty = false;
		int len;
		if (current_rotation == 0) {
			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() - 2) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}

		}
		else if (current_rotation == 1) {
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (empty == false) {
				top_left = temp;
			}
			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}




			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}
		else if (current_rotation == 2) {
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}
			if (empty == false) {
				top_left = temp;
			}
			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}
		else if (current_rotation == 3) {
			len = 3;

			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}
			if (empty == false) {
				top_left = temp;
			}
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}



		}

	}

	virtual void move_left(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() - 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() - 2) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 2) {//180 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 3) {//270 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(-1);
		}
	}

	virtual void move_right(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 2) {//180 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 3) {//270 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(1);
		}
	}

	virtual bool falling_shape(well& grid) {
		bool collision = false;
		if (current_rotation == 0) {
			if ((top_left.get_y()) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 1);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 1);
				grid.fill(top_left.get_x() + 1, top_left.get_y() - 1, 1);
				grid.fill(top_left.get_x() + 1, top_left.get_y() - 2, 1);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			if ((top_left.get_y() + 1) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 2) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 1);
				grid.fill(top_left.get_x(), top_left.get_y() + 1, 1);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 1);
				grid.fill(top_left.get_x() + 2, top_left.get_y() + 1, 1);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 2) {
			if ((top_left.get_y() + 2) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 3) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 1);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 1);
				grid.fill(top_left.get_x(), top_left.get_y() + 1, 1);
				grid.fill(top_left.get_x(), top_left.get_y() + 2, 1);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 3) {
			if ((top_left.get_y() + 1) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 2) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 1);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 1);
				grid.fill(top_left.get_x() + 2, top_left.get_y(), 1);
				grid.fill(top_left.get_x() + 2, top_left.get_y() + 1, 1);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		if (collision == false) {
			return true;
		}
		return false;

	}

	virtual ~J_block() {

	}

};

class L_block : public tetramino {
private:
public:
	L_block() {
		current_rotation = 0;
		R = 214;
		G = 104;
		B = 0;
	}
	virtual void draw_shape(sf::RenderWindow& window) {

		sf::Texture texture;
		if (!texture.loadFromFile("Lblock.png"))
		{
			// error...
		}
		if (current_rotation == 0) {
			point temp = top_left;
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}
			temp.increment_x(1);
			temp.increment_y(-1);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);
		}
		else if (current_rotation == 1) {
			point temp = top_left;
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
			temp.increment_x(-3);
			temp.increment_y(1);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);

		}
		else if (current_rotation == 2) {
			point temp = top_left;
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);
			temp.increment_x(1);
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}

		}
		else if (current_rotation == 3) {
			point temp = top_left;
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
			temp.increment_x(-1);
			temp.increment_y(-1);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);

		}
	}

	virtual void rotate(well grid) {
		int prev_rotation = current_rotation;
		point temp = top_left;
		current_rotation++;
		current_rotation = current_rotation % 4;
		bool empty = false;
		int len;
		if (current_rotation == 0) {

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_x(-1);
				}
			}

			if (empty == false) {
				top_left = temp;
			}

			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_y(-1);
				}
			}


			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}

		}
		else if (current_rotation == 1) {

			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}


			if (empty == false) {
				top_left = temp;
			}

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}





			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}
		else if (current_rotation == 2) {
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}
			if (empty == false) {
				top_left = temp;
			}
			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}
		else if (current_rotation == 3) {
			len = 3;

			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}
			if (empty == false) {
				top_left = temp;
			}
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}



		}

	}
	virtual void move_left(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 2) {//180 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 3) {//270 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(-1);
		}
	}
	virtual void move_right(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 2) {//180 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 3) {//270 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y() - 1) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(1);
		}



	}
	virtual bool falling_shape(well& grid) {
		bool collision = false;
		if (current_rotation == 0) {
			if ((top_left.get_y() + 2) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 3) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 3) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 2);
				grid.fill(top_left.get_x(), top_left.get_y() + 1, 2);
				grid.fill(top_left.get_x(), top_left.get_y() + 2, 2);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 2, 2);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			if ((top_left.get_y() + 1) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 2);
				grid.fill(top_left.get_x(), top_left.get_y() + 1, 2);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 2);
				grid.fill(top_left.get_x() + 2, top_left.get_y(), 2);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 2) {
			if ((top_left.get_y() + 2) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 3) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 2);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 2);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 2);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 2, 2);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 3) {
			if ((top_left.get_y()) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 2);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 2);
				grid.fill(top_left.get_x() + 2, top_left.get_y(), 2);
				grid.fill(top_left.get_x() + 2, top_left.get_y() - 1, 2);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		if (collision == false) {
			return true;
		}
		return false;
	}
	virtual ~L_block() {

	}

};

class O_block : public tetramino {
private:
public:
	O_block() {
		current_rotation = 0;
		R = 236;
		G = 252;
		B = 3;
	}
	virtual void draw_shape(sf::RenderWindow& window) {
		sf::Texture texture;
		if (!texture.loadFromFile("Oblock.png"))
		{
			// error...
		}


		point temp = top_left;
		for (int i = 0; i < 2; i++) {
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);
			temp.increment_x(1);
		}
		temp.increment_x(-2);
		temp.increment_y(1);
		for (int i = 0; i < 2; i++) {
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);
			temp.increment_x(1);
		}
	}

	virtual void rotate(well grid) {

	}

	virtual void move_left(well grid) {
		bool blocked = true;
		if (top_left.get_x() > 0) {
			blocked = false;
		}
		if (blocked == false) {

			if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
				blocked = true;
			}
			if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
				blocked = true;
			}

		}
		if (blocked == false) {
			top_left.increment_x(-1);
		}
	}

	virtual void move_right(well grid) {

		bool blocked = true;
		if (top_left.get_x() < 8) {
			blocked = false;
		}
		if (blocked == false) {

			if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
				blocked = true;
			}
			if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
				blocked = true;
			}

		}
		if (blocked == false) {
			top_left.increment_x(1);
		}



	}

	virtual bool falling_shape(well& grid) {
		bool collision = false;
		if ((top_left.get_y() + 1) == 19) {
			collision = true;
		}
		else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
			collision = true;
		}
		else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
			collision = true;
		}

		if (collision == true) {

			grid.fill(top_left.get_x(), top_left.get_y(), 3);
			grid.fill(top_left.get_x(), top_left.get_y() + 1, 3);
			grid.fill(top_left.get_x() + 1, top_left.get_y(), 3);
			grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 3);

			reset();
		}
		else {
			top_left.increment_y(1);
		}
		if (collision == false) {
			return true;
		}
		return false;
	}

	virtual ~O_block() {

	}

};

class S_block : public tetramino {
private:
public:
	S_block() {
		current_rotation = 0;
		R = 4;
		G = 252;
		B = 0;
	}
	virtual void draw_shape(sf::RenderWindow& window) {

		sf::Texture texture;
		if (!texture.loadFromFile("Sblock.png"))
		{
			// error...
		}
		if (current_rotation == 0) {
			point temp = top_left;
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
			temp.increment_x(-1);
			temp.increment_y(-1);
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
		}
		else if (current_rotation == 1) {
			point temp = top_left;
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}
			temp.increment_x(1);
			temp.increment_y(-1);
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}

		}
	}

	virtual void rotate(well grid) {
		int prev_rotation = current_rotation;
		point temp = top_left;
		current_rotation++;
		current_rotation = current_rotation % 2;
		bool empty = false;
		int len;
		if (current_rotation == 0) {

			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_x(-1);
				}
			}

			if (empty == false) {
				top_left = temp;
			}

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_y(-1);
				}
			}


			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}

		}
		else if (current_rotation == 1) {

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}


			if (empty == false) {
				top_left = temp;
			}

			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}





			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}


	}

	virtual void move_left(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() - 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					blocked = true;
				}


			}
		}

		if (blocked == false) {
			top_left.increment_x(-1);
		}
	}

	virtual void move_right(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y() - 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}

		if (blocked == false) {
			top_left.increment_x(1);
		}


	}

	virtual bool falling_shape(well& grid) {
		bool collision = false;
		if (current_rotation == 0) {
			if ((top_left.get_y()) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 4);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 4);
				grid.fill(top_left.get_x() + 1, top_left.get_y() - 1, 4);
				grid.fill(top_left.get_x() + 2, top_left.get_y() - 1, 4);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			if ((top_left.get_y() + 2) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 3) == false) {
				collision = true;
			}


			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 4);
				grid.fill(top_left.get_x(), top_left.get_y() + 1, 4);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 4);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 2, 4);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}

		if (collision == false) {
			return true;
		}
		return false;
	}

	virtual ~S_block() {

	}
};

class T_block : public tetramino {
private:
public:
	T_block() {
		current_rotation = 0;
		R = 153;
		G = 3;
		B = 158;
	}
	virtual void draw_shape(sf::RenderWindow& window) {

		sf::Texture texture;
		if (!texture.loadFromFile("Tblock.png"))
		{
			// error...
		}
		if (current_rotation == 0) {
			point temp = top_left;
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
			temp.increment_x(-2);
			temp.increment_y(1);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);
		}
		else if (current_rotation == 1) {
			point temp = top_left;
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);

			temp.increment_x(1);
			temp.increment_y(-1);
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}
		}
		else if (current_rotation == 2) {
			point temp = top_left;
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
			temp.increment_x(-2);
			temp.increment_y(-1);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);
		}


		else if (current_rotation == 3) {
			point temp = top_left;
			for (int i = 0; i < 3; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}
			temp.increment_x(1);
			temp.increment_y(-2);
			coords C = temp.to_coords();
			draw_square(window, C, R, G, B, texture);

		}

	}

	virtual void rotate(well grid) {
		int prev_rotation = current_rotation;
		point temp = top_left;
		current_rotation++;
		current_rotation = current_rotation % 4;
		bool empty = false;
		int len;
		if (current_rotation == 0) {

			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_x(-1);
				}
			}

			if (empty == false) {
				top_left = temp;
			}

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_y(-1);
				}
			}


			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}

		}
		else if (current_rotation == 1) {

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(1);
				}
			}




			if (empty == false) {
				top_left = temp;
			}

			len = 2;

			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}

			if (empty == false) {
				top_left = temp;
			}

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}





			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}
		else if (current_rotation == 2) {
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}
			if (empty == false) {
				top_left = temp;
			}
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}

			if (empty == false) {
				top_left = temp;
			}
			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() - 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(1);
				}
			}

			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}
		else if (current_rotation == 3) {
			len = 2;

			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}
			if (empty == false) {
				top_left = temp;
			}
			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}
			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}



		}

	}

	virtual void move_left(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() - 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 2) {//180 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() - 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 3) {//270 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(-1);
		}
	}

	virtual void move_right(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 2) {//180 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y()) == false) {
					blocked = true;
				}
			}
		}
		else if (current_rotation == 3) {//270 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
					blocked = true;
				}

			}
		}
		if (blocked == false) {
			top_left.increment_x(1);
		}



	}

	virtual bool falling_shape(well& grid) {
		bool collision = false;
		if (current_rotation == 0) {
			if ((top_left.get_y() + 1) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 2, top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 5);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			if ((top_left.get_y() + 1) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
				collision = true;
			}


			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y() - 1, 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 5);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 2) {
			if ((top_left.get_y()) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 2, top_left.get_y(), 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y() - 1, 5);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 3) {
			if ((top_left.get_y() + 2) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 3) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 5);
				grid.fill(top_left.get_x(), top_left.get_y() + 1, 5);
				grid.fill(top_left.get_x(), top_left.get_y() + 2, 5);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 5);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		if (collision == false) {
			return true;
		}
		return false;

	}

	virtual ~T_block() {

	}
};

class Z_block : public tetramino {
private:
public:
	Z_block() {
		current_rotation = 1;
		R = 227;
		G = 23;
		B = 0;
	}
	virtual void draw_shape(sf::RenderWindow& window) {
		sf::Texture texture;
		if (!texture.loadFromFile("Zblock.png"))
		{
			// error...
		}
		if (current_rotation == 0) {
			point temp = top_left;
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
			temp.increment_x(-1);
			temp.increment_y(1);
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_x(1);
			}
		}
		else if (current_rotation == 1) {
			point temp = top_left;
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}
			temp.increment_x(1);
			temp.increment_y(-3);
			for (int i = 0; i < 2; i++) {
				coords C = temp.to_coords();
				draw_square(window, C, R, G, B, texture);
				temp.increment_y(1);
			}

		}
	}

	virtual void rotate(well grid) {
		int prev_rotation = current_rotation;
		point temp = top_left;
		current_rotation++;
		current_rotation = current_rotation % 2;
		bool empty = false;
		int len;
		if (current_rotation == 0) {

			len = 3;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_x(-1);
				}
			}

			if (empty == false) {
				top_left = temp;
			}

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (empty == false) {
					top_left.increment_y(-1);
				}
			}






			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}

		}
		else if (current_rotation == 1) {

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_x(-1);
				}
			}


			if (empty == false) {
				top_left = temp;
			}

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(-1);
				}
			}



			if (empty == false) {
				top_left = temp;
			}

			len = 2;
			while (empty == false && grid.isEmpty(top_left.get_x(), top_left.get_y()) == true && len > 0) {
				len--;
				empty = true;
				if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y()) == false) {
					empty = false;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					empty = false;
				}


				if (empty == false) {
					top_left.increment_y(1);
				}
			}



			if (grid.isEmpty(top_left.get_x(), top_left.get_y()) == false || empty == false) {
				current_rotation = prev_rotation;
				top_left = temp;
			}


		}

	}

	virtual void move_left(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() > 0) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() - 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}


			}
		}

		if (blocked == false) {
			top_left.increment_x(-1);
		}
	}

	virtual void move_right(well grid) {
		bool blocked = true;
		if (current_rotation == 0) {//0 degree
			if (top_left.get_x() < 7) {
				blocked = false;
			}
			if (blocked == false) {

				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 3, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}
		else if (current_rotation == 1) {//90 degree
			if (top_left.get_x() < 8) {
				blocked = false;
			}
			if (blocked == false) {
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() - 1) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y()) == false) {
					blocked = true;
				}
				if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
					blocked = true;
				}

			}
		}

		if (blocked == false) {
			top_left.increment_x(1);
		}


	}

	virtual bool falling_shape(well& grid) {
		bool collision = false;
		if (current_rotation == 0) {
			if ((top_left.get_y() + 1) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 1) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 2) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 2, top_left.get_y() + 2) == false) {
				collision = true;
			}

			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 6);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 6);
				grid.fill(top_left.get_x() + 1, top_left.get_y() + 1, 6);
				grid.fill(top_left.get_x() + 2, top_left.get_y() + 1, 6);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}
		else if (current_rotation == 1) {
			if ((top_left.get_y() + 1) == 19) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x(), top_left.get_y() + 2) == false) {
				collision = true;
			}
			else if (grid.isEmpty(top_left.get_x() + 1, top_left.get_y() + 1) == false) {
				collision = true;
			}


			if (collision == true) {

				grid.fill(top_left.get_x(), top_left.get_y(), 6);
				grid.fill(top_left.get_x(), top_left.get_y() + 1, 6);
				grid.fill(top_left.get_x() + 1, top_left.get_y(), 6);
				grid.fill(top_left.get_x() + 1, top_left.get_y() - 1, 6);

				reset();
			}
			else {
				top_left.increment_y(1);
			}
		}

		if (collision == false) {
			return true;
		}
		return false;
	}

	virtual ~Z_block() {

	}

};






class game {
private:
	well Well;
	int score;
	int lines;
	int level;

public:
	game() {
		score = 0;
		lines = 0;
		level = 1;
	}
	void set_up(sf::RenderWindow& window, tetramino* types[]) {
		sf::Texture texture0;
		if (!texture0.loadFromFile("Iblock.png"))
		{
			// error...
		}
		sf::Texture texture1;
		if (!texture1.loadFromFile("Jblock.png"))
		{
			// error...
		}
		sf::Texture texture2;
		if (!texture2.loadFromFile("Lblock.png"))
		{
			// error...
		}
		sf::Texture texture3;
		if (!texture3.loadFromFile("Oblock.png"))
		{
			// error...
		}
		sf::Texture texture4;
		if (!texture4.loadFromFile("Sblock.png"))
		{
			// error...
		}
		sf::Texture texture5;
		if (!texture5.loadFromFile("Tblock.png"))
		{
			// error...
		}
		sf::Texture texture6;
		if (!texture6.loadFromFile("Zblock.png"))
		{
			// error...
		}

		sf::Texture texture;
		if (!texture.loadFromFile("background.png"))
		{
			// error...
		}
		sf::RectangleShape well_grid = Well.draw();
		well_grid.setTexture(&texture);
		window.draw(well_grid);
		int R, G, B;
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (Well[i][j] != -1) {
					types[Well[i][j]]->get_RGB(R, G, B);
					sf::RectangleShape square(sf::Vector2f(26.f, 26.f));
					if (Well[i][j] == 0) {
						square.setTexture(&texture0);
					}
					else if (Well[i][j] == 1) {
						square.setTexture(&texture1);
					}
					else if (Well[i][j] == 2) {
						square.setTexture(&texture2);
					}
					else if (Well[i][j] == 3) {
						square.setTexture(&texture3);
					}
					else if (Well[i][j] == 4) {
						square.setTexture(&texture4);
					}
					else if (Well[i][j] == 5) {
						square.setTexture(&texture5);
					}
					else if (Well[i][j] == 6) {
						square.setTexture(&texture6);
					}

					point p(j, i);
					coords C = p.to_coords();
					square.setPosition(C.get_x(), C.get_y());
					square.setFillColor(sf::Color(255, 255, 255));

					// setting outline
					square.setOutlineThickness(2.f);
					square.setOutlineColor(sf::Color(0, 0, 0));
					window.draw(square);
				}

			}
		}


	}

	void generate_tetramino(sf::RenderWindow& window, tetramino* types[], int block) {


		types[block]->draw_shape(window);


	}

	bool collision(sf::RenderWindow& window, tetramino* types[], int block) {
		bool falling = types[block]->falling_shape(Well);
		if (falling == true) {
			return false;
		}
		else {
			return true;
		}
	}

	void right(sf::RenderWindow& window, tetramino* types[], int block) {

		types[block]->move_right(Well);
	}

	void left(sf::RenderWindow& window, tetramino* types[], int block) {

		types[block]->move_left(Well);
	}

	void rotate(sf::RenderWindow& window, tetramino* types[], int block) {
		types[block]->rotate(Well);
	}

	void check_lines() {
		int new_lines = Well.check_lines();
		lines += new_lines;
		score += (new_lines * 100);
	}
	void check_level() {
		level = ((lines / 10) % 8) + 1;
	}
	int get_level() {
		return level;
	}

	bool well_filled() {
		if (Well.gameover()) {
			return true;
		}
		return false;
	}
	void gameover(sf::RenderWindow& window) {


		sf::RectangleShape rectangle(sf::Vector2f(1300.f, 700.f));
		rectangle.setFillColor(sf::Color(0, 0, 0));
		rectangle.setPosition(0, 0);
		window.draw(rectangle);

		window.draw(rectangle);

		sf::Font font;
		if (!font.loadFromFile("ARCADECLASSIC.ttf"))
		{
			// error...
			cout << "Unable to load the required font!\n";
		}

		sf::Text gameover, score_word, score_number, leaderboard;

		// select the font
		gameover.setFont(font); // font is a sf::Font

		// set the string to display
		gameover.setString("GAMEOVER!");

		// set the character size
		gameover.setCharacterSize(100); // in pixels, not points!

		// set the color
		gameover.setFillColor(sf::Color::Red);

		// set the text style
		//gameover.setStyle(sf::Text::Bold);


		// set the position
		gameover.setPosition(420.f, 25.f);

		window.draw(gameover);


		// SCORE
		const string score_str = to_string(score);
		// select the font
		score_word.setFont(font); // font is a sf::Font
		score_number.setFont(font);

		// set the string to display
		score_word.setString("Score  ");
		score_number.setString(score_str);

		// set the character size
		score_word.setCharacterSize(60); // in pixels, not points!
		score_number.setCharacterSize(60);

		// set the color
		score_word.setFillColor(sf::Color::White);
		score_number.setFillColor(sf::Color::White);

		// set the text style
		//score_word.setStyle(sf::Text::Bold | sf::Text::Underlined);


		// set the position
		score_word.setPosition(420.f, 150.f);
		score_number.setPosition(620.f, 150.f);

		window.draw(score_word);
		window.draw(score_number);


		//Leaderboard
		// select the font
		leaderboard.setFont(font); // font is a sf::Font

		// set the string to display
		leaderboard.setString("Leaderboard");

		// set the character size
		leaderboard.setCharacterSize(40); // in pixels, not points!

		// set the color
		leaderboard.setFillColor(sf::Color::White);

		// set the text style
		//leaderboard.setStyle(sf::Text::Bold | sf::Text::Underlined);


		// set the position
		leaderboard.setPosition(420.f, 250.f);

		window.draw(leaderboard);

		// Names and scores
		ifstream read_name("names.txt");
		ifstream read_score("scores.txt");
		string p1, p2, p3, p4, p5;
		string n1, n2, n3, n4, n5;
		string str1, str2, str3, str4, str5;

		getline(read_name, p1);
		getline(read_score, n1);
		str1 = "1  " + p1 + "   " + n1;

		getline(read_name, p2);
		getline(read_score, n2);
		str2 = "2  " + p2 + "   " + n2;
		//cout << str2 << endl;
		getline(read_name, p3);
		getline(read_score, n3);
		str3 = "3  " + p3 + "   " + n3;
		//cout << str3 << endl;
		getline(read_name, p4);
		getline(read_score, n4);
		str4 = "4  " + p4 + "   " + n4;
		//cout << str4 << endl;
		getline(read_name, p5);
		getline(read_score, n5);
		str5 = "5  " + p5 + "   " + n5;
		//cout << str5 << endl;



		read_name.close();
		read_score.close();
		sf::Text name1, name2, name3, name4, name5;
		// select the font
		name1.setFont(font); // font is a sf::Font
		name2.setFont(font);
		name3.setFont(font);
		name4.setFont(font);
		name5.setFont(font);

		// set the string to display
		name1.setString(str1);
		name2.setString(str2);
		name3.setString(str3);
		name4.setString(str4);
		name5.setString(str5);

		// set the character size
		name1.setCharacterSize(30); // in pixels, not points!
		name2.setCharacterSize(30);
		name3.setCharacterSize(30);
		name4.setCharacterSize(30);
		name5.setCharacterSize(30);

		// set the color
		name1.setFillColor(sf::Color::White);
		name2.setFillColor(sf::Color::White);
		name3.setFillColor(sf::Color::White);
		name4.setFillColor(sf::Color::White);
		name5.setFillColor(sf::Color::White);

		// set the position
		name1.setPosition(420.f, 300.f);
		name2.setPosition(420.f, 340.f);
		name3.setPosition(420.f, 380.f);
		name4.setPosition(420.f, 420.f);
		name5.setPosition(420.f, 460.f);

		window.draw(name1);
		window.draw(name2);
		window.draw(name3);
		window.draw(name4);
		window.draw(name5);


	}

	void manage_leaderboard(string playerInput, bool& new_highscore) {

		//use the same files for input and output
		//store the data in a vector and sort the data in descending order
		//then write back the first 5 values to the file
		ifstream read_name("names.txt");
		ifstream read_score("scores.txt");
		ofstream write_name("temp_names.txt");
		ofstream write_score("temp_scores.txt");
		string board_name;
		string board_score;
		int i;
		//bool new_highscore = false;
		bool alreadyused = true;
		for (i = 0; i < 5; i++) {
			if (alreadyused) {
				getline(read_name, board_name);
				getline(read_score, board_score);
			}
			int board_score_int = stoi(board_score);
			if (board_score_int < score && new_highscore == false) {
				write_name << playerInput << endl;
				write_score << score << endl;
				new_highscore = true;
				alreadyused = false;
			}
			else {
				write_name << board_name << endl;
				write_score << board_score << endl;
				alreadyused = true;
			}
		}

		read_name.close();
		read_score.close();
		write_name.close();
		write_score.close();

		remove("scores.txt");
		remove("names.txt");
		rename("temp_names.txt", "names.txt");
		rename("temp_scores.txt", "scores.txt");

	}

	void input_name(sf::RenderWindow& window, string& playerInput, sf::Text& playerText, sf::Event& event, sf::Font& font) {


		//Name input
		// select the font
		playerText.setFont(font); // font is a sf::Font

		// set the string to display
		playerInput += event.text.unicode;
		playerText.setString(playerInput);

		// set the character size
		playerText.setCharacterSize(60); // in pixels, not points!

		// set the color
		playerText.setFillColor(sf::Color::White);

		// set the text style
		playerText.setStyle(sf::Text::Bold);


		// set the position
		playerText.setPosition(375.f, 230.f);

	}
	void starting_text(sf::RenderWindow& window) {
		sf::Font font;
		if (!font.loadFromFile("ARCADECLASSIC.ttf"))
		{
			// error...
			cout << "Unable to load the required font!\n";
		}


		sf::Text text;

		// printing "Enter your name: "

		// select the font
		text.setFont(font); // font is a sf::Font


		// set the string to display
		text.setString("Enter your name");

		// set the character size
		text.setCharacterSize(60); // in pixels, not points!


		// set the color
		text.setFillColor(sf::Color::White);


		// set the text style
		/*text.setStyle(sf::Text::Bold | sf::Text::Underlined);*/


		// set the position
		text.setPosition(375.f, 130.f);


		window.draw(text);


	}


	void display_info(sf::RenderWindow& window, string& playerInput) {
		sf::Font font;
		if (!font.loadFromFile("ARCADECLASSIC.ttf"))
		{
			// error...
			cout << "Unable to load the required font!\n";
		}

		const string score_str = "Score  " + to_string(score);
		const string lines_str = "Lines   " + to_string(lines);
		const string level_str = "Level  " + to_string(level);


		sf::Text score_number, lines_number, level_number, player_name;

		// Player

		sf::RectangleShape rectangle(sf::Vector2f(600, 360));
		rectangle.setFillColor(sf::Color(0, 0, 0));
		rectangle.setPosition(370, 80);
		rectangle.setOutlineThickness(5.f);
		rectangle.setOutlineColor(sf::Color(255, 255, 255));

		window.draw(rectangle);

		// select the font
		 // font is a sf::Font
		player_name.setFont(font);

		// set the string to display
		string player_full = "Player  " + playerInput;
		player_name.setString(player_full);

		// set the character size
		 // in pixels, not points!
		player_name.setCharacterSize(60);

		// set the color
		player_name.setFillColor(sf::Color::White);

		// set the text style
		/*player_word.setStyle(sf::Text::Bold);*/


		// set the position
		player_name.setPosition(400.f, 80.f);

		window.draw(player_name);




		// SCORE
		// select the font
		 // font is a sf::Font
		score_number.setFont(font);

		// set the string to display

		score_number.setString(score_str);

		// set the character size
		 // in pixels, not points!
		score_number.setCharacterSize(60);

		// set the color

		score_number.setFillColor(sf::Color::White);

		// set the text style
		/*score_word.setStyle(sf::Text::Bold | sf::Text::Underlined);
	*/

	// set the position

		score_number.setPosition(400.f, 160.f);


		window.draw(score_number);


		// LINES

		// select the font
		 // font is a sf::Font
		lines_number.setFont(font);

		// set the string to display

		lines_number.setString(lines_str);

		// set the character size
		// in pixels, not points!
		lines_number.setCharacterSize(60);

		// set the color

		lines_number.setFillColor(sf::Color::White);

		// set the text style
		//lines_word.setStyle(sf::Text::Bold | sf::Text::Underlined);


		// set the position

		lines_number.setPosition(400.f, 250.f);


		window.draw(lines_number);


		// LEVEL

		// select the font
		// font is a sf::Font
		level_number.setFont(font);

		// set the string to display

		level_number.setString(level_str);

		// set the character size
		 // in pixels, not points!
		level_number.setCharacterSize(60);

		// set the color

		level_number.setFillColor(sf::Color::White);

		// set the text style
		//level_word.setStyle(sf::Text::Bold | sf::Text::Underlined);


		// set the position

		level_number.setPosition(400.f, 340.f);


		window.draw(level_number);




	}
	void hide(sf::RenderWindow& window) {
		sf::RectangleShape rectangle(sf::Vector2f(350.f, 75.f));
		rectangle.setFillColor(sf::Color(0, 0, 0));
		rectangle.setPosition(0, 0);

		window.draw(rectangle);
	}

};
