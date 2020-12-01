#include "libLab6.c"


/* プロトタイプ宣言 */
void monoColorConversion(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage);


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
	monoColorConversion(width, height, inputImage, outputImage);

	/* 処理後画像の出力 */
	writeBmp(width, height, outputImage);

	/* メモリの開放 */
	free(inputImage);
	free(outputImage);

	return 0;
}


/* 単色のみガンマ変換する関数 */
void monoColorConversion(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage)
{
	int color = 0;			// 色番号
	int row = 0, col = 0;	// ループカウンタ（row：行(row)方向，col：列(column)方向）
	int c = 0;				// ループカウンタ（色番号）

	double gamma = 0.0; 	// ガンマ値


	/* ガンマ変換する色を入力 */
	printf("\n[ 色の選択（%d - Red / %d - Green / %d - Blue） ]\n", R, G, B);
	printf("色番号 : ");
	scanf_s("%d", &color);

	/* ガンマ値を設定する */
	printf("\n[ ガンマ値を入力（実数） ]\n");
	printf("ガンマ値（> 0.0）: ");
	scanf_s("%lf", &gamma);

	printf("\n[ 単色のみガンマ変換 ]\n");

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			for (c = 0; c <= 2; c++) {
				if (c == color) {
					outputImage[row][col][c] = pow(((double)inputImage[row][col][c] / 255), 1 / gamma) * 255;
				}
				else {
					outputImage[row][col][c] = inputImage[row][col][c];
				}
			}
		}
	}
}