// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <ctime>



struct cell {
	int row;
	int col;
};

struct cellToMove {
	int number;
	int type;  // 1 or 2
};


std::vector<cell> cells1;	// store positions for all figure of types1
std::vector<cell> cells2;	// store positions for all figure of types2
Mat originalImg;			// original image
int rows;					// puzzle rows
int cols;					// puzzle cols
int height;					// cell height
int width;					// cell width
std::vector<int> border;	// vector which separate type1 of type2
int firstOrSecond = 0;		// 0 if first cell was clicked, and 1 if the second
cellToMove cell1;			// first cell which was clicked
cellToMove cell2;			// the second cell for swap
Mat puzzleImg;				// current puzzle


void mixPuzzle() {

	//set cells1 and cells2 with values of type cell in order (0,0), (0,1), (0,2) ... 
	//( it depends of rows and cols )

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cell x;
			x.row = i;
			x.col = j;
			cells1.push_back(x);
			cells2.push_back(x);
		}
	}

	//mix the figure of type1 with random
	for (int k = 0; k < cells1.size(); k++) {
		int r = k + rand() % (cells1.size() - k); 
		cell c = cells1[k];
		cells1[k] = cells1[r];
		cells1[r] = c;
	}


	//mix the figure of type2 with random
	for (int k = 0; k < cells2.size(); k++) {
		int r = k + rand() % (cells2.size() - k);
		cell c = cells2[k];
		cells2[k] = cells2[r];
		cells2[r] = c;
	}

}

//construct the image based on the position stored in cells1 and cells2
Mat constructImage() {

	Mat image(originalImg.size(), CV_8UC3);
	int k = 0;
	for (int m = 0; m < rows; m++) {
		for (int n = 0; n < cols; n++) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					if(j< border.at(i))
						image.at<Vec3b>(m*height+i, n*width+j) = originalImg.at<Vec3b>(cells1[k].row*height + i, cells1[k].col*width +j);
					else
						image.at<Vec3b>(m*height + i, n*width + j) = originalImg.at<Vec3b>(cells2[k].row*height + i, cells2[k].col*width + j);
				}
			}
			k++;
		}				
	}

	return image;
}


//swap cell1 with cell2, only if the have the same type
void swapCells(cellToMove cell1, cellToMove cell2) {

	if(cell1.type != cell2.type)
		std::cout << "You can switch cells of different types" << std::endl;
	else if (cell1.type == 1) {
		cell c = cells1[cell1.number];
		cells1[cell1.number] = cells1[cell2.number];
		cells1[cell2.number] = c;
	}
	else if (cell1.type == 2) {
		cell c = cells2[cell1.number];
		cells2[cell1.number] = cells2[cell2.number];
		cells2[cell2.number] = c;
	}

}


bool win() {
	int k = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (cells1[k].row != i || cells1[k].col != j || cells2[k].row != i || cells2[k].col != j)
				return false;
			k++;
		}
	}
	return true;
}


void MyCallBackFunc1(int event, int x, int y, int flags, void* param)
{

	if (event == EVENT_LBUTTONDOWN) {

		int cellRow = y / height;
		int cellCol = x / width;

		int pixelPosCol = x % width;
		int pixelPosRow = y % height;

		if (firstOrSecond == 0) {
			firstOrSecond = 1;

			cell1.number = cellRow * cols + cellCol;
			if (pixelPosCol < border.at(pixelPosRow)) {
				cell1.type = 1;
				for (int j = cellCol*width; j < cellCol*width + border.at(0); j++) {
					puzzleImg.at<Vec3b>(cellRow*height, j)[2] = 255;
					puzzleImg.at<Vec3b>(cellRow*height, j)[1] = 255;
					puzzleImg.at<Vec3b>(cellRow*height, j)[0] = 0;
				}
				for (int i = cellRow * height; i < ((cellRow + 1) * height -1 ); i++) {
					puzzleImg.at<Vec3b>(i, cellCol*width)[2] = 255;
					puzzleImg.at<Vec3b>(i, cellCol*width)[1] = 255;
					puzzleImg.at<Vec3b>(i, cellCol*width)[0] = 0;
				}
				for (int j = cellCol*width; j < cellCol*width + border.at(height); j++) {
					puzzleImg.at<Vec3b>((cellRow+1)*height-1, j)[2] = 255;
					puzzleImg.at<Vec3b>((cellRow+1)*height-1, j)[1] = 255;
					puzzleImg.at<Vec3b>((cellRow+1)*height-1, j)[0] = 0;
				}
				
			}			
			else {
				cell1.type = 2;

				for (int j = cellCol*width + border.at(0); j < (cellCol+1)*width; j++) {
					puzzleImg.at<Vec3b>(cellRow*height, j)[2] = 255;
					puzzleImg.at<Vec3b>(cellRow*height, j)[1] = 255;
					puzzleImg.at<Vec3b>(cellRow*height, j)[0] = 0;
				}
				for (int i = cellRow * height; i < ((cellRow + 1) * height -1 ); i++) {
					puzzleImg.at<Vec3b>(i, (cellCol + 1)*width - 1)[2] = 255;
					puzzleImg.at<Vec3b>(i, (cellCol + 1)*width - 1)[1] = 255;
					puzzleImg.at<Vec3b>(i, (cellCol + 1)*width - 1)[0] = 0;
				}
				for (int j = cellCol*width + border.at(height); j < (cellCol + 1)*width - 1; j++) {
					puzzleImg.at<Vec3b>((cellRow+1)*height-1, j)[2] = 255;
					puzzleImg.at<Vec3b>((cellRow+1)*height-1, j)[1] = 255;
					puzzleImg.at<Vec3b>((cellRow+1)*height-1, j)[0] = 0;
				}
			}




			for (int i = 0; i < height; i++) {
				puzzleImg.at<Vec3b>(cellRow*height + i, (cellCol*width) + border.at(i))[2] = 255;
				puzzleImg.at<Vec3b>(cellRow*height + i, (cellCol*width) + border.at(i))[1] = 255;
				puzzleImg.at<Vec3b>(cellRow*height + i, (cellCol*width) + border.at(i))[0] = 0;
			}
			
			imshow("Puzzle", puzzleImg);
			


		}

		else {
			
			cell2.number = cellRow * cols + cellCol;
			if (pixelPosCol < border.at(pixelPosRow))
				cell2.type = 1;
			else
				cell2.type = 2;

			if (cell1.type == cell2.type) {
				firstOrSecond = 0;
				swapCells(cell1, cell2);
				puzzleImg = constructImage();
				imshow("Puzzle", puzzleImg);
				if (win()) {
					std::cout << "You WON!" << std::endl;
				}
			}
			else {
				std::cout << "You can switch cells of different types" << std::endl;
			}			
		}	
	}

}


void constructBorderVector() {

	if (width >= height) {

		int middle = height / 2;

		for (int i = -middle; i <= middle; i++) {
			border.push_back(i + width / 2);
		}

		if (height % 2 == 1)
			border.push_back(middle + 1 + width / 2);
	}
	else {
		for (int i = 0; i <= height; i++) {
			border.push_back( width / 2);
		}
	}
	
}



void resetGame() {
	cells1.clear();
	cells2.clear();
	border.clear();
	firstOrSecond = 0;
}

void startPuzzle() {

	srand(time(NULL));
	resetGame();
	char fname[MAX_PATH];
	Mat img;
	while (openFileDlg(fname))
	{
		 img = imread(fname);
		 break;
	}
	
	printf("\n Number of rows = ");
	scanf("%d", &rows);
	printf("\n Number of columns = ");
	scanf("%d", &cols);

	namedWindow("Puzzle", 1);
	setMouseCallback("Puzzle", MyCallBackFunc1, NULL);

	int r = img.rows - (img.rows % rows);
	int c = img.cols - (img.cols % cols);

	resize(img, originalImg, Size(c, r));

	height = r / rows;
	width = c / cols;

	constructBorderVector();


	imshow("Original", originalImg);

	mixPuzzle();

	puzzleImg = constructImage();
	imshow("Puzzle", puzzleImg);
	waitKey();
}

int main()
{	
	int op;
	do
	{
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Start Puzzle Game\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d", &op);

		switch (op)
		{
		case 1:
			startPuzzle();
			break;
		}
		waitKey();
	} while (op != 0);
	return 0;
}