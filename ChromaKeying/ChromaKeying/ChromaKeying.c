#include "libLab6.c"


/* プロトタイプ宣言 */
void chromaKeying(int width, int height, unsigned char*** inputImage, unsigned char*** gbImage, unsigned char*** outputImage);


/* main() 関数 */
int main(void)
{
	/* 変数宣言 */
	unsigned char*** inputImage = NULL;		// 画素データ配列（原画像用）
	unsigned char*** gbImage = NULL;		// 画素データ配列（グリーンバック画像用）
	unsigned char*** outputImage = NULL;	// 画素データ配列（処理後画像用）
	int width = 0, height = 0;				// 画像の幅（width）と高さ（height）

	/* 原画像の入力 */
	printf("\n[ 原画像の入力 ]\n");
	inputImage = readBmp(&width, &height);

	/* グリーンバック画像の入力 */
	printf("\n[ グリーンバック画像の入力 ]\n");
	gbImage = readBmp(&width, &height);

	/* 出力画像用配列を生成 */
	outputImage = makeBmpArray(width, height);

	/* 画像処理をする関数を呼び出す */
	chromaKeying(width, height, inputImage, gbImage, outputImage);

	/* 処理後画像の出力 */
	writeBmp(width, height, outputImage);

	/* メモリの開放 */
	free(inputImage);
	free(gbImage);
	free(outputImage);

	return 0;
}


/* クロマキー合成をする関数 */
void chromaKeying(int width, int height, unsigned char*** inputImage, unsigned char*** gbImage, unsigned char*** outputImage)
{
	int row = 0, col = 0;	// ループカウンタ（row：行(row)方向，col：列(column)方向）
	int c = 0;				// ループカウンタ（色番号）
	double alpha = 0.0;		// 透過度


	/* 透過度を入力 */
	printf("\n[ 透過度（実数） ]\n");
	printf("透過度 [0.0 - 1.0] : ");
	scanf_s("%lf", &alpha);

	printf("\n[ クロマキー合成 ]\n");

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			for (c = 0; c <= 2; c++) {
				if (c == color) {
					outputImage[row][col][c] = inputImage[row][col][c];
				}
				else {
					outputImage[row][col][c] = 0;
				}
			}
		}

}