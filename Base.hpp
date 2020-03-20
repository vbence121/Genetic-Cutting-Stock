#pragma once
int getRandom(int from, int to) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(from, to);
	int toReturn = std::round(dist6(rng));
	return toReturn;
}


int LKO(int a, int b)
{
	if (a == 0)
		return b;
	return LKO(b % a, a);
}

int LKOarr(std::vector<int> arr)
{
	int n = arr.size();
	int result = arr[0];
	for (int i = 1; i < n; i++)
	{
		result = LKO(arr[i], result);

		if (result == 1)
		{
			return 1;
		}
	}
	return result;
}


class Base {
	int width;
	int height;
	int absHeight;
	int absWidth;
	double ID;
	bool accepted = false;
public:
	Base(int w, int h, int absH, int absW) : width(w), height(h), absHeight(absH), absWidth(absW) {
		ID = 0.5 * (width + height) * (width + height + 1) + height;
		if (width == 0 || height == 0) {
			std::cout << "NULL ERTEK, HIBA" << std::endl;
		}
		//std::cout << ID << std::endl;
	}
	int getX() {
		return absWidth;
	}
	int getY() {
		return absHeight;
	}
	void setAccepted(bool a) {
		accepted = a;
	}
	bool getAccepted() {
		return accepted;
	}
	double getID() {
		return ID;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	std::vector<Base*> horizontalCut(int h, int cutWidth) {
		std::vector<Base*> toReturn;
		if ((h - cutWidth) > 0) {
			//if(h!=height){
			int newAbsH = this->absHeight;
			int newAbsW = this->absWidth;
			Base* ret1 = new Base(width, h - cutWidth, newAbsH, newAbsW);
			toReturn.push_back(ret1);
			//}
		}
		if ((h + cutWidth) < height) {
			//if(h!=0){
			int newAbsH = this->absHeight + (h + cutWidth);
			int newAbsW = this->absWidth;
			Base* ret2 = new Base(width, height - (h + cutWidth), newAbsH, newAbsW);
			toReturn.push_back(ret2);
			//}
		}
		return toReturn;
	}
	std::vector<Base*> verticalCut(int w, int cutWidth) {
		std::vector<Base*> toReturn;
		if ((w - cutWidth) > 0) {
			//if(w!=width){
			int newAbsH = this->absHeight;
			int newAbsW = this->absWidth;
			Base* ret1 = new Base(w - cutWidth, height, newAbsH, newAbsW);
			toReturn.push_back(ret1);
			//}
		}
		if ((w + cutWidth) < width) {
			//if(w!=0){
			int newAbsH = this->absHeight;
			int newAbsW = this->absWidth + w + cutWidth;
			Base* ret2 = new Base(width - (w + cutWidth), height, newAbsH, newAbsW);
			toReturn.push_back(ret2);
			//}
		}
		return toReturn;
	}
};