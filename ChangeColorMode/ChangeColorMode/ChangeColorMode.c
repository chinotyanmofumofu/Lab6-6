#include "libLab6.c"


/* プロトタイプ宣言 */
void grayScale(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage);
void sepiaColor(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage);


/* main() 関数 */
int main(void)
{
	/* 変数宣言 */
	unsigned char*** inputImage = NULL;		// 画素データ配列（原画像用）
	unsigned char*** outputImage = NULL;	// 画素データ配列（処理後画像用）
	int width = 0, height = 0;				// 画像の幅（width）と高さ（height）

	/* 原画像の入力 */
	inputImage = readBmp(&width, &height);

	/* 出力画像用配列を生成 */
	outputImage = makeBmpArray(width, height);

	/* 画像処理をする関数を呼び出す */
	grayScale(width, height, inputImage, outputImage);

	/* 処理後画像の出力 */
	writeBmp(width, height, outputImage);

	/* 画像処理をする関数を呼び出す */
	sepiaColor(width, height, inputImage, outputImage);

	/* 処理後画像の出力 */
	writeBmp(width, height, outputImage);

	/* メモリの開放 */
	free(inputImage);
	free(outputImage);

	return 0;
}


/* カラー画像をグレースケール化する関数 */
void grayScale(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage)
{
	int row = 0, col = 0;		// ループカウンタ（row：行(row)方向，col：列(column)方向）
	int c = 0;					// ループカウンタ（色番号）

	unsigned char pixel = 0;	// 計算値


	printf("\n[ グレースケール化 ]\n");

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			pixel = 0.299 * inputImage[row][col][0] + 0.587 * inputImage[row][col][1] + 0.114 * inputImage[row][col][2];
			for (c = 0; c <= 2; c++) {
				outputImage[row][col][c] = pixel;
			}
		}
	}

}


/* カラー画像をセピア調化する関数 */
void sepiaColor(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage)
{
	int row = 0, col = 0;	// ループカウンタ（row：行(row)方向，col：列(column)方向）
	int c = 0;				// ループカウンタ（色番号）

	int pixel[3] = { 0 };	// 計算値


	printf("\n[ セピア調化 ]\n");

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			pixel[0] = 0.393 * inputImage[row][col][0] + 0.796 * inputImage[row][col][1]
				+ 0.189 * inputImage[row][col][2];
			pixel[1] = 0.349 * inputImage[row][col][0] + 0.686 * inputImage[row][col][1]
				+ 0.168 * inputImage[row][col][2];
			pixel[2] = 0.272 * inputImage[row][col][0] + 0.534 * inputImage[row][col][1]
				+ 0.131 * inputImage[row][col][2];
			for (c = 0; c <= 2; c++) {
				pixel[0] = (int)maxValue(minValue(pixel[0], 255), 0);
				outputImage[row][col][0] = pixel[0];
				pixel[1] = (int)maxValue(minValue(pixel[1], 255), 0);
				outputImage[row][col][1] = pixel[1];
				pixel[2] = (int)maxValue(minValue(pixel[2], 255), 0);
				outputImage[row][col][2] = pixel[2];
			}
		}
	}

}