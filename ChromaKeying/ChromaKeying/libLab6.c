/**************************

 libLab6.c
 
 Terumasa Ikeda
 
 2020.03

***************************/


/* ヘッダインクルード */

#ifndef __STDIO_H
#include <stdio.h>
#endif

#ifndef __STDLIB_H
#include <stdlib.h>
#endif

#ifndef __STDARG_H
#include <stdarg.h>
#endif

#ifndef __MATH_H
#include <math.h>
#endif

#ifndef __STRING_H
#include <string.h>
#endif


/* マクロ定数 */

#define TRUE -1
#define FALSE 0

#define FILE_NAME_FORMAT "%80s"		// ファイル名入力時の書式子
#define FILE_NAME_SIZE 100			// ファイル名文字配列のサイズ

#define FILE_HEADER_SIZE 14			// ファイルヘッダサイズ
#define INFO_HEADER_SIZE 40			// 情報ヘッダサイズ 
#define HEADER_SIZE (FILE_HEADER_SIZE + INFO_HEADER_SIZE)	// ヘッダサイズ

// 学生が自作するプログラムから参照されるマクロ定数
#define F_MIN 0		// 画素値の最小値
#define F_MAX 255	// 画素値の最大値
#define LEVEL 256	// 画素値の階調レベル
#define COLOR_NUM 3 // 色数
#define R 0			// 色番号（Red）
#define G 1			// 色番号（Green）
#define B 2			// 色番号（Blue）


/* プロトタイプ宣言 */

// 学生が自作するプログラムから参照されるのは
// maxValue(), minValue()
// makeRawArray(), readRaw(), writeRaw()
// makeBmpArray(), readBmp(), writeBmp()
// writeImageHistogram(), writeDiffHistogram()
// makeDiffArray(), readDiff(), writeDiff()

double maxValue(double a, double b);
double minValue(double a, double b);

FILE* fileOpen(char fileName[], char mode[]);
int fileClose(FILE* fp);

unsigned char** makeRawArray(int width, int height);
unsigned char** readRawFromFile(int* pWidth, int* pHeight, char fileName[]);
unsigned char** readRaw(int* pWidth, int* pHeight);
int writeRawToFile(int width, int height, unsigned char** image, char fileName[]);
void writeRaw(int width, int height, unsigned char** image);

int bytes2value(int num, ...);
unsigned char*** makeBmpArray(int width, int height);
int check_bmp_header(unsigned char header[], int* pWidth, int* pHeight, int* pStride);
unsigned char*** readBmpFromFile(int* pWidth, int* pHeight, char fileName[]);
unsigned char*** readBmp(int* pWidth, int* pHeight);
int writeBmpToFile(int width, int height, unsigned char*** image, char fileName[]);
void writeBmp(int width, int height, unsigned char*** image);

int writeImageHistogram(int level, int histCount[], double histFrequency[]);
int writeDiffHistogram(int level, int histCount[], double histFrequency[]);

int** makeDiffArray(int width, int height);
int** readDiffFromFile(int* pWidth, int* pHeight, char fileName[]);
int** readDiff(int* pWidth, int* pHeight);
int writeDiffToFile(int width, int height, int** diffData, char fileName[]);
void writeDiff(int width, int height, int** diffData);


/* 実装 */

/*****************

ユーティリティ関数

******************/


double maxValue(double a, double b)
{
	/*
	2つの数値を比較して大きい方の数値を返す
	
	Parameters
	----------
	a : double
		比較対象の数値
	b : double
		比較対象の数値
	
	Returns
	-------
	max : double
		大きい方の数値
	*/
	
	double max = 0;
	
	max = a > b ? a : b;
	
	return max;
}


double minValue(double a, double b)
{
	/*
	2つの数値を比較して小さい方の数値を返す
	
	Parameters
	----------
	a : double
		比較対象の数値
	b : double
		比較対象の数値
	
	Returns
	-------
	min : double
		小さい方の数値
	*/
	
	double min = 0;
	
	min = a < b ? a : b;

	return min;
}


/*************************

ライブラリ内で利用する関数

**************************/


FILE* fileOpen(char fileName[], char mode[])
{
	/*
	エラーチェック付きファイルオープン
	
	Parameters
	----------
	fileName : char[]
		ファイル名文字列
	mode : char[]
		ファイルオープンモード
	
	Returns
	-------
	fp : FILE* or NULL
		ファイルポインタ
	*/
	
	FILE* fp = NULL;
	int stat = FALSE;

	stat = fopen_s(&fp, fileName, mode);

	if (stat != 0)
	{
		printf("\n[ fileOpen() : ファイルオープンエラー ]\n");
		return NULL;
	}
	else if (fp == NULL || fp == 0)
	{
		printf("\n[ fileOpen() : ファイルポインタが不正です ]\n");
		return NULL;
	}

	return fp;
}


int fileClose(FILE* fp)
{
	/*
	エラーチェック付きファイルクローズ
	
	Parameters
	----------
	fp : FILE*
		ファイルポインタ
	
	Returns
	-------
	status : int
		ファイルクローズに成功した場合のみ TRUE
	*/
	
	if (fp == NULL)
	{
		printf("\n[ fileClose() : ファイルポインタが NULL です ]\n");
		return FALSE;
	}

	if (fp == 0)
	{
		printf("\n[ fileClose() : ファイルポインタが不正です（%p）]\n", fp);
		return FALSE;
	}

	if (fclose(fp) != 0)
	{
		printf("\n[ fileClose() : ファイルクローズエラー ]\n");
		return FALSE;
	}

	return TRUE;
}


/**********************

RAW画像を扱うための関数

***********************/


unsigned char** makeRawArray(int width, int height)
{
	/*
	RAW画像用の2次元配列を動的に生成する

	Parameters
	----------
	width : int
		画像の幅（列数）
	height : int
		画像の高さ（行数）

	Returns
	-------
	image : unsigned char**
		確保した2次元配列のポインタ
	*/

	char** image = NULL;	// 2次元配列のポインタ
	char* pointer = NULL;	// 処理する位置を表すポインタ
	int rowSize = 0;		// 要素1行分のサイズ
	int row = 0;			// ループカウンタ

	// 引数エラー
	if (width <= 0 || height <= 0)
	{
		printf("\n[ makeRawArray() : 引数エラー ]\n");
		printf("width = %d / height = %d\n", width, height);

		goto catch;
	}

	// 要素1行分のサイズを計算
	rowSize = sizeof(unsigned char) * width;

	// 全インデックスと全要素分の領域を一度に確保する
	image = (char**)malloc((sizeof(*image) + rowSize) * height);

	// メモリ確保エラー
	if (image == NULL)
	{
		printf("\n[ makeRawArray() : メモリの確保に失敗 ]\n");

		goto catch;
	}

	// 要素の先頭アドレスを取得
	pointer = (char*)(image + height);

	// 各行の先頭アドレスを生成する
	for (row = 0; row < height; row++)
	{
		*(image + row) = pointer;
		memset(pointer, 0, rowSize);	// 1行分を0で初期化

		pointer += rowSize; // 要素1行分のサイズだけ進める
	}

	return image;

catch:

	exit(EXIT_FAILURE);
}


unsigned char** readRawFromFile(int* pWidth, int* pHeight, char fileName[])
{
	/*
	指定されたRAW画像ファイルからデータを読み込む

	Parameters
	----------
	pWidth : int*
		幅ポインタ,
	pHeight : int*
		高さポインタ
	fileName : char[]
		ファイル名

	Returns
	-------
	image : unsigned char** or NULL
		画像データを保持した2次元配列のポインタ
	*/

	FILE* fp = NULL;				// ファイルポインタ
	unsigned char** image = NULL;	// 2次元配列のポインタ
	unsigned char pixel = 0;		// 1画素分のデータ
	int row= 0, col = 0;			// ループカウンタ

	// 画像ファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "rb")) == NULL)
	{
		// ファイルオープンエラー
		printf("\n[ readRawFromFile() : %s が開けません ]\n", fileName);

		goto catch;
	}
	
	// 2次元配列を取得
	image = makeRawArray(*pWidth, *pHeight);

	for (row = 0; row < *pHeight; row++)
	{
		for (col = 0; col < *pWidth; col++)
		{

			// 1画素分のデータを読み込む
			if (fread(&pixel, sizeof(unsigned char), 1, fp) != 1)
			{
				// エラー処理
				printf("\n[ readRawFromFile() : データの読み込みに失敗 ]\n");

				goto catch;
			}
			
			image[row][col] = pixel;
		}
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return image;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	if (image != NULL) free(image);	// メモリ開放

	return NULL;
}


unsigned char** readRaw(int* pWidth, int* pHeight)
{
	/*
	ファイル名入力処理をしてRAW画像ファイルからデータを読み出す

	Parameters
	----------
	pWidth : int*
		幅ポインタ,
	pHeight : int*
		高さポインタ

	Returns
	-------
	image : unsigned char**
		画像データを保持した2次元配列のポインタ
	*/

	char fileName[FILE_NAME_SIZE] = { '\0' };	// ファイル名入力用文字列
	unsigned char** image = NULL;				// 2次元配列のポインタ


	/* RAW画像の情報を取得 */
	printf("\n[ RAW画像の情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	do
	{
		printf("　幅 (0以上) : ");
		scanf_s("%d", pWidth);
	} while (*pWidth <= 0);

	do
	{
		printf("高さ (0以上) : ");
		scanf_s("%d", pHeight);
	} while (*pHeight <= 0);

	/* RAW画像データの読み込み */
	image = readRawFromFile(pWidth, pHeight, fileName);

	if (image != NULL)
	{
		printf("\n[ %s を読み込みました ]\n", fileName);

		return image;
	}
	else
	{
		printf("\n[ readRaw() : %s の読み込みに失敗しました ]\n", fileName);

		exit(EXIT_FAILURE);
	}
}


int writeRawToFile(int width, int height, unsigned char** image, char fileName[])
{
	/*
	指定されたRAW画像ファイルへデータを書き込む

	Parameters
	----------
	width : int
		幅
	height : int
		高さ
	image : unsigned char**
		画像データを保持した2次元配列のポインタ
	fileNme : char[]
		ファイル名

	Returns
	-------
	status : int
		書き込み完了フラグ
	*/

	FILE* fp = NULL;			// ファイルポインタ
	unsigned char pixel = 0;	// 1画素分のデータ
	int row = 0, col = 0;		// ループカウンタ

	// 画像ファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "wb")) == NULL)
	{
		// エラー処理
		printf("\n[ writeRawToFile() : %s が開けません ]\n", fileName);

		goto catch;
	}

	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{

			// 書き込む画素値を取得
			pixel = image[row][col];

			// 画素値を書き込む
			if (fwrite(&pixel, sizeof(unsigned char), 1, fp) != 1)
			{
				// エラー処理
				printf("\n[ writeRawToFile() : データの書き込みに失敗 ]\n");

				goto catch;
			}
		}
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return TRUE;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	return FALSE;
}


void writeRaw(int width, int height, unsigned char** image)
{
	/*
	ファイル名入力処理をしてRAW画像データをファイルに書き出す

	Parameters
	----------
	width : int
		幅
	height : int
		高さ
	image : unsigned char**
		画像データを保持した2次元配列のポインタ
	*/

	char fileName[FILE_NAME_SIZE];	// ファイル名入力用文字列
	int succeeded = FALSE;			// 書き込み完了フラグ

	/* 出力する画像ファイル名を取得 */
	printf("\n[ RAW画像の情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	/* RAW画像データの書き込み */
	succeeded = writeRawToFile(width, height, image, fileName);

	if (succeeded == TRUE)
	{
		printf("\n[ %s にデータを書き込みました ]\n", fileName);
	}
	else
	{
		printf("\n[ writeRaw() : %s への書き込みに失敗しました ]\n", fileName);

		if (image != NULL) free(image);	// メモリ開放

		exit(EXIT_FAILURE);
	}
}


/**********************

BMP画像を扱うための関数

***********************/


int bytes2value(int num, ...)
{
	/*
	複数のバイト列（リトルエンディアン）を連結して一つの数値として返す

	Prameters
	---------
	num : int
		バイト数
	args : int
		バイトデータ...

	Returns
	-------
	value : int
		計算後の数値
	*/

	va_list list;	// 引数リスト
	int value = 0;	// 計算値
	int count;		// ループカウンタ

	va_start(list, num);

	for (count = 0; count < num; count++)
	{
		value += va_arg(list, unsigned char) * (int)pow(256, count);
	}

	va_end(list);

	return value;
}


unsigned char*** makeBmpArray(int width, int height)
{
	/*
	BMP画像用の3次元配列を動的に生成する

	Parameters
	----------
	width : int
		画像の幅（列数）
	height : int
		画像の高さ（行数）

	Returns
	-------
	image : unsigned char***
		確保した3次元配列のポインタ
	*/
	
	char*** image = NULL;		// 2次元配列のポインタ
	char** pColIndex = NULL;	// 行列インデックスを表すポインタ
	char* pointer = NULL;		// 処理する位置を表すポインタ
	int colorsSize = 0;			// 1画素分のサイズ
	int row = 0, col = 0;		// ループカウンタ

	// 引数エラー
	if (width <= 0 || height <= 0)
	{
		printf("\n[ makeBmpArray() : 引数エラー ]\n");
		printf("width = %d / height = %d\n", width, height);

		goto catch;
	}

	// 1画素分のサイズを計算
	colorsSize = sizeof(unsigned char) * COLOR_NUM;

	// 全インデックスと全要素分の領域を一度に確保する
	image = (char***)malloc((sizeof(*image) + sizeof(**image) * width + colorsSize * width) * height);

	// メモリ確保エラー
	if (image == NULL)
	{
		printf("\n[ makeBmpArray() : メモリの確保に失敗 ]\n");

		goto catch;
	}
	
	// 行列インデックスの先頭アドレスを取得
	pColIndex = (char**)(image + height);

	// 要素の先頭アドレスを取得
	pointer = (char*)(pColIndex + height * width);

	// 各行列の先頭アドレスを生成する
	for (row = 0; row < height; row++)
	{
		*(image + row) = pColIndex;

		for (col = 0; col < width; col++)
		{
			*(pColIndex + col) = pointer;
			memset(pointer, 0, colorsSize);	// 1画素分を0で初期化

			pointer += colorsSize; // 1画素分のサイズだけ進める
		}

		pColIndex += width; // 要素1行分のサイズだけ進める
	}

	return image;

catch:

	exit(EXIT_FAILURE);
}


int check_bmp_header(unsigned char header[], int* pWidth, int* pHeight, int* pStride)
{
	/*
	BMP画像のヘッダをチェックし情報を取得する

	Parameters
	----------
	header : unsigned char[]
		ヘッダ部バイト列
	pWidth : int*
		幅ポインタ
	pHeight : int*
		高さポインタ
	pStride : int*
		1行分のバイト数ポインタ

	Returns
	-------
	status : int
		チェック完了フラグ
	*/

	// ヘッダパラメータ
	int offset = 0;
	int infoHeaderSize = 0;
	int plane = 0;
	int bpp = 0;
	int fileSize = 0;
	int dataSize = 0;

	int width = 0, height = 0;	// 幅（width）と高さ（height）
	int stride = 0;				// 4[byte] 境界に合わせた1行分のバイト数

	// MagicNumberチェック（"BM"）
	if (header[0x00] != 'B' || header[0x01] != 'M')
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（Magic Number）]\n");
		printf("%c%c (%02x %02x)\n", header[0x00], header[0x01], header[0x00], header[0x01]);

		return FALSE;
	}

	// データオフセットチェック
	offset = bytes2value(4, header[0x0a], header[0x0b], header[0x0c], header[0x0d]);
	if (offset != HEADER_SIZE)
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（データオフセット）]\n");
		printf("offset = %d (%02x %02x %02x %02x)\n", offset, header[0x0a], header[0x0b], header[0x0c], header[0x0d]);

		return FALSE;
	}

	// 情報ヘッダサイズチェック
	infoHeaderSize = bytes2value(4, header[0x0e], header[0x0f], header[0x10], header[0x11]);
	if (infoHeaderSize != INFO_HEADER_SIZE)
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（情報ヘッダサイズ）]\n");
		printf("infoHeaderSize = %d (%02x %02x %02x %02x)\n", infoHeaderSize, header[0x0e], header[0x0f], header[0x10], header[0x11]);

		return FALSE;
	}

	// プレーン数チェック（常に1）
	plane = bytes2value(2, header[0x1a], header[0x1b]);
	if (plane != 1)
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（プレーン数）]\n");
		printf("plane = %d (%02x %02x)\n", plane, header[0x1a], header[0x1b]);

		return FALSE;
	}

	// BPPチェック（24[bit]）
	bpp = bytes2value(2, header[0x1c], header[0x1d]);
	if (bpp != 24)
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（Bit Per Pixel）]\n");
		printf("bpp = %d (%02x %02x)\n", bpp, header[0x1c], header[0x1d]);

		return FALSE;
	}

	// 画像サイズ（幅）チェック
	width = bytes2value(4, header[0x12], header[0x13], header[0x14], header[0x15]);
	if (width <= 0)
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（幅）]\n");
		printf("width = %d (%02x %02x %02x %02x)\n", width, header[0x12], header[0x13], header[0x14], header[0x15]);

		return FALSE;
	}

	// 画像サイズ（高さ）チェック
	height = bytes2value(4, header[0x16], header[0x17], header[0x18], header[0x19]);
	if (height <= 0)
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（高さ）]\n");
		printf("height = %d (%02x %02x %02x %02x)\n", height, header[0x16], header[0x17], header[0x18], header[0x19]);

		return FALSE;
	}

	// 4[byte] 境界に合わせた1行分のバイト数を計算
	stride = (width * COLOR_NUM + 3) / 4 * 4;

	// ファイルサイズチェック
	fileSize = bytes2value(4, header[0x02], header[0x03], header[0x04], header[0x05]);
	if (fileSize != (stride * height + HEADER_SIZE))
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（ファイルサイズ）]\n");
		printf("fileSize = %d (%02x %02x %02x %02x)\n", fileSize, header[0x02], header[0x03], header[0x04], header[0x05]);
		
		return FALSE;
	}

	// データサイズチェック
	dataSize = bytes2value(4, header[0x22], header[0x23], header[0x24], header[0x25]);
	if (dataSize != (stride * height))
	{
		printf("\n[ readBmpFromFile() : BMPヘッダエラー（データサイズ）]\n");
		printf("dataSize = %d (%02x %02x %02x %02x)\n", dataSize, header[0x22], header[0x23], header[0x24], header[0x25]);

		return FALSE;
	}

	#ifdef DEBUG
		printf("%c%c (%02x %02x)\n", header[0x00], header[0x01], header[0x00], header[0x01]);
		printf("offset = %d (%02x %02x %02x %02x)\n", offset, header[0x0a], header[0x0b], header[0x0c], header[0x0d]);
		printf("infoHeaderSize = %d (%02x %02x %02x %02x)\n", infoHeaderSize, header[0x0e], header[0x0f], header[0x10], header[0x11]);
		printf("plane = %d (%02x %02x)\n", plane, header[0x1a], header[0x1b]);
		printf("bpp = %d (%02x %02x)\n", bpp, header[0x1c], header[0x1d]);
		printf("w = %d (%02x %02x %02x %02x)\n", width, header[0x12], header[0x13], header[0x14], header[0x15]);
		printf("h = %d (%02x %02x %02x %02x)\n", height, header[0x16], header[0x17], header[0x18], header[0x19]);
		printf("stride = %d\n", stride);
		printf("fileSize = %d (%02x %02x %02x %02x)\n", fileSize, header[0x02], header[0x03], header[0x04], header[0x05]);
		printf("dataSize = %d (%02x %02x %02x %02x)\n", dataSize, header[0x22], header[0x23], header[0x24], header[0x25]);
	#endif

	*pWidth = width;
	*pHeight = height;
	*pStride = stride;

	return TRUE;
}


unsigned char*** readBmpFromFile(int* pWidth, int* pHeight, char fileName[])
{
	/*
	指定されたBMP画像ファイルからデータを読み込む

	Parameters
	----------
	pWidth : int*
		幅ポインタ,
	pHeight : int*
		高さポインタ
	fileName : char[]
		ファイル名

	Returns
	-------
	image : unsigned char*** or NULL
		画像データを保持した3次元配列のポインタ
	*/
	
	FILE* fp = NULL;				// ファイルポインタ
	unsigned char header[HEADER_SIZE] = { 0 };	// ヘッダ部のデータ配列
	unsigned char*** image = NULL;	// 3次元配列のポインタ
	unsigned char* buffer = NULL;	// バッファ用ポインタ
	unsigned char* target = NULL;	// 処理する位置を表すポインタ
	int stride = 0;	// 4[byte] 境界に合わせた1行分のバイト数
	int row = 0, col = 0;	// ループカウンタ

	// 画像ファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "rb")) == NULL)
	{
		// ファイルオープンエラー
		printf("\n[ readBmpFromFile() : %s が開けません ]\n", fileName);

		goto catch;
	}

	// BMP ヘッダの読み込み
	if (fread(header, HEADER_SIZE, 1, fp) != 1)
	{
		// エラー処理
		printf("\n[ readBmpFromFile() : ヘッダの読み込みに失敗 ]\n");

		goto catch;
	}

	// ヘッダチェック
	if (check_bmp_header(header, pWidth, pHeight, &stride) == FALSE)
	{
		// エラー処理
		printf("\n[ readBmpFromFile() : 不正なヘッダ ]\n");

		goto catch;
	}

	// 3次元配列を取得
	image = makeBmpArray(*pWidth, *pHeight);

	// 1行分のデータを読み込むためのバッファを確保
	buffer = malloc(stride);

	// バッファエラー処理
	if (buffer == NULL)
	{
		printf("\n[ readBmpFromFile() : バッファの確保に失敗 ]\n");

		goto catch;
	}

	// BMP はデータが上下反転されて記録されているため行ループは降順にする
	for (row = (*pHeight) - 1; row >= 0; row--)
	{
		// 1 行分のデータを読み込む
		if (fread(buffer, stride, 1, fp) != 1)
		{
			// エラー処理
			printf("\n[ readBmpFromFile() : データの読み込みに失敗 ]\n");

			goto catch;
		}

		target = buffer;

		// 各画素のデータを取得
		for (col = 0; col < (*pWidth); col++)
		{
			image[row][col][2] = *target;
			image[row][col][1] = *(target + 1);
			image[row][col][0] = *(target + 2);

			target += COLOR_NUM;
		}
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return image;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	if (image != NULL) free(image);	// メモリ開放
	if (buffer != NULL) free(buffer);	// メモリ開放

	return NULL;
}

unsigned char*** readBmp(int* pWidth, int* pHeight)
{
	/*
	ファイル名入力処理をしてBMP画像ファイルからデータを読み出す

	Parameters
	----------
	pWidth : int*
		幅ポインタ,
	pHeight : int*
		高さポインタ

	Returns
	-------
	image : unsigned char***
		画像データを保持した3次元配列のポインタ
	*/

	char fileName[FILE_NAME_SIZE] = { '\0' };	// ファイル名入力用文字列
	unsigned char*** image = NULL;				// 3次元配列のポインタ

	/* 入力する画像ファイル名を取得 */
	printf("\n[ BMP画像の情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	/* BMP画像データの読み込み */
	image = readBmpFromFile(pWidth, pHeight, fileName);

	if (image != NULL)
	{
		printf("\n[ %s を読み込みました ]\n", fileName);

		return image;
	}
	else
	{
		printf("\n[ readBmp() : %s の読み込みに失敗しました ]\n", fileName);

		exit(EXIT_FAILURE);
	}
}


int writeBmpToFile(int width, int height, unsigned char*** image, char fileName[])
{
	/*
	ファイルポインタへBMP画像データを書き込む

	Parameters
	----------
	width : int
		幅
	height : int
		高さ
	image : unsigned char***
		画像データを保持した3次元配列のポインタ
	fileNme : char[]
		ファイル名

	Returns
	-------
	status : int
		読み込み完了フラグ
	*/

	FILE* fp = NULL;					// ファイルポインタ
	unsigned char header[HEADER_SIZE] = { 0 };	// ヘッダ部のデータ配列
	unsigned char* buffer = NULL;		// バッファ用ポインタ
	unsigned char* pointer = NULL;		// 処理する位置を表すポインタ
	int fileSize = 0, dataSize = 0;		// ヘッダパラメータ
	int w = 0, h = 0;					// 幅（width）と高さ（height）
	int stride = 0;						// 4[byte] 境界に合わせた1行分のバイト数
	int row = 0, col = 0, count = 0;	// ループカウンタ

	// 画像ファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "wb")) == NULL)
	{
		// エラー処理
		printf("\n[ writeBmpToFile() : %s が開けません ]\n", fileName);

		goto catch;
	}

	/* 4[byte] 境界に合わせた1行分のバイト数を計算 */
	stride = (width * COLOR_NUM + 3) / 4 * 4;

	/* ヘッダ情報の生成 */

	// ファイルタイプ文字列
	header[0x00] = 'B';
	header[0x01] = 'M';

	// ファイルサイズ
	fileSize = stride * height + HEADER_SIZE;
	for (count = 0x02; count <= 0x05; count++)
	{
		header[count] = fileSize % 256;
		fileSize = fileSize / 256;
	}

	header[0x0a] = HEADER_SIZE;		// 画像データまでのオフセット（54 [Byte]）
	header[0x0e] = INFO_HEADER_SIZE;		// 情報ヘッダサイズ（40 [Byte]）

	// 画像サイズ（幅）
	w = width;
	for (count = 0x12; count <= 0x15; count++)
	{
		header[count] = w % 256;
		w = w / 256;
	}

	// 画像サイズ（高さ）	
	h = height;
	for (count = 0x16; count <= 0x19; count++)
	{
		header[count] = h % 256;
		h = h / 256;
	}

	header[0x1a] = 1;		// プレーン数（常に 1）
	header[0x1c] = 24;		// Bit Per Pixel（24 [bit]）

	// 画像データサイズ
	dataSize = stride * height;
	for (count = 0x22; count <= 0x25; count++)
	{
		header[count] = dataSize % 256;
		dataSize = dataSize / 256;
	}

	// ヘッダ部をファイルに書き込む
	if (fwrite(header, HEADER_SIZE, 1, fp) != 1)
	{
		// エラー処理
		printf("\n[ writeBmpToFile() : ヘッダ部の書き込みに失敗 ]\n");

		goto catch;
	}
	
	// 1行分のデータを書き込むためのバッファを確保
	buffer = malloc(stride);

	// バッファエラー処理
	if (buffer == NULL)
	{
		printf("\n[ writeBmpFromFile() : バッファの確保に失敗 ]\n");

		goto catch;
	}

	memset(buffer, 0, stride);	// バッファを0で初期化

	// BMP はデータが上下反転されて記録されているため行ループは降順にする
	for (row = height - 1; row >= 0; row--)
	{
		pointer = buffer;

		// バッファに1画素分のデータを書き込む
		for (col = 0; col < width; col++)
		{
			*pointer = image[row][col][2];
			*(pointer + 1) = image[row][col][1];
			*(pointer + 2) = image[row][col][0];

			// 次の画素に進める
			pointer += COLOR_NUM;
		}

		// 1行分のデータを書き込む
		if (fwrite(buffer, stride, 1, fp) != 1)
		{
			// エラー処理
			printf("\n[ writeBmpToFile() : データ部の書き込みに失敗 ]\n");

			goto catch;
		}
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return TRUE;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	if (buffer != NULL) free(buffer);	// メモリ開放

	return FALSE;
}


void writeBmp(int width, int height, unsigned char*** image)
{
	/*
	ファイル名入力処理をしてBMP画像データをファイルに書き出す

	Parameters
	----------
	width : int
		幅
	height : int
		高さ
	image : unsigned char***
		画像データを保持した3次元配列のポインタ
	*/

	char fileName[FILE_NAME_SIZE];	// ファイル名入力用文字列
	int succeeded = FALSE;			// 読み込み完了フラグ

	/* 出力する画像ファイル名を取得 */
	printf("\n[ BMP画像の情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	/* BMP画像データの書き込み */
	succeeded = writeBmpToFile(width, height, image, fileName);

	if (succeeded == TRUE)
	{
		printf("\n[ %s にデータを書き込みました ]\n", fileName);
	}
	else
	{
		printf("\n[ writeBmp() : %s への書き込みに失敗しました ]\n", fileName);

		if (image != NULL) free(image);	// メモリ開放

		exit(EXIT_FAILURE);
	}
}


/**************************

ヒストグラム CSV 出力用関数

***************************/


int writeImageHistogram(int level, int histCount[], double histFrequency[])
{
	/*
	ファイル名入力処理をして画素値ヒストグラムデータを CSV 形式でファイルに書き出す

	Parameters
	----------
	lebel : int
		画素値の階調レベル
	histCount : int[]
		画素値の発生数データ配列
	histFrequency : double[]
		画素値の発生頻度データ配列

	Returns
	-------
	status : int
		書き込み完了フラグ
	*/

	FILE* fp = NULL;				// ファイルポインタ
	char fileName[FILE_NAME_SIZE];	// ファイル名入力用文字列
	int count = 0;					// ループカウンタ

	/* 出力する画像ファイル名を取得 */
	printf("\n[ CSVファイルの情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	// 画像ファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "wb")) == NULL)
	{
		// エラー処理
		printf("\n[ writeImageHistogram() : %s が開けません ]\n", fileName);

		goto catch;
	}

	/* ヒストグラムデータの書き込み */
	fprintf(fp, "画素値,発生頻度[%%],発生数\n");

	for (count = 0; count < level; count++)
	{
		fprintf(fp, "%d,%.4f,%d\n", count, histFrequency[count] * 100.0, histCount[count]);
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return TRUE;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	return FALSE;
}


int writeDiffHistogram(int diffLevel, int histCount[], double histFrequency[])
{
	/*
	ファイル名入力処理をして差分値ヒストグラムデータを CSV 形式でファイルに書き出す

	Parameters
	----------
	diffLevel : int
		差分値の階調レベル
	histCount : int[]
		差分値の発生数データ配列
	histFrequency : double[]
		差分値の発生頻度データ配列

	Returns
	-------
	status : int
		書き込み完了フラグ
	*/

	FILE* fp = NULL;				// ファイルポインタ
	char fileName[FILE_NAME_SIZE];	// ファイル名入力用文字列
	int count = 0;					// ループカウンタ

	/* 出力する画像ファイル名を取得 */
	printf("\n[ CSVファイルの情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	// 画像ファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "wb")) == NULL)
	{
		// エラー処理
		printf("\n[ writeDiffHistogram() : %s が開けません ]\n", fileName);

		goto catch;
	}

	/* ヒストグラムデータの書き込み */
	fprintf(fp, "差分値,発生頻度[%%],発生数\n");

	for (count = 0; count < diffLevel; count++)
	{
		fprintf(fp, "%d,%.4f,%d\n", count - (diffLevel / 2), histFrequency[count] * 100.0, histCount[count]);
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return TRUE;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	return FALSE;
}


/*******************

差分データ処理用関数

********************/


int** makeDiffArray(int width, int height)
{
	/*
	差分データ用の2次元配列を動的に生成する

	Parameters
	----------
	width : int
		画像の幅（列数）
	height : int
		画像の高さ（行数）

	Returns
	-------
	diffData : int**
		確保した2次元配列のポインタ
	*/

	int** diffData = NULL;	// 2次元配列のポインタ
	int* pointer = NULL;	// 処理する位置を表すポインタ
	int rowSize = 0;		// 要素1行分のサイズ
	int row = 0;			// ループカウンタ

	// 引数エラー
	if (width <= 0 || height <= 0)
	{
		printf("\n[ makeDiffArray() : 引数エラー ]\n");
		printf("width = %d / height = %d\n", width, height);

		goto catch;
	}

	// 要素1行分のサイズを計算
	rowSize = sizeof(int) * width;

	// 全インデックスと全要素分の領域を一度に確保する
	diffData = (int**)malloc((sizeof(*diffData) + rowSize) * height);

	// メモリ確保エラー
	if (diffData == NULL)
	{
		printf("\n[ makeDiffArray() : メモリの確保に失敗 ]\n");

		goto catch;
	}

	// 要素の先頭アドレスを取得
	pointer = (int*)(diffData + height);

	// 各行の先頭アドレスを生成する
	for (row = 0; row < height; row++)
	{
		*(diffData + row) = pointer;
		memset(pointer, 0, rowSize);	// 1行分を0で初期化

		pointer += width; // 要素1行分のサイズだけ進める
	}

	return diffData;

catch:

	exit(EXIT_FAILURE);
}


int** readDiffFromFile(int* pWidth, int* pHeight, char fileName[])
{
	/*
	指定された差分データファイルからデータを読み込む

	Parameters
	----------
	pWidth : int*
		幅ポインタ,
	pHeight : int*
		高さポインタ
	fileName : char[]
		ファイル名

	Returns
	-------
	diffData : int** or NULL
		差分データを保持した2次元配列のポインタ
	*/

	FILE* fp = NULL;		// ファイルポインタ
	int** diffData = NULL;	// 2次元配列のポインタ
	int pixel = 0;			// 1画素分のデータ
	int row= 0, col = 0;	// ループカウンタ

	// 差分データファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "rb")) == NULL)
	{
		// ファイルオープンエラー
		printf("\n[ readDiffFromFile() : %s が開けません ]\n", fileName);

		goto catch;
	}
	
	// 2次元配列を取得
	diffData = makeDiffArray(*pWidth, *pHeight);

	for (row = 0; row < *pHeight; row++)
	{
		for (col = 0; col < *pWidth; col++)
		{

			// 1画素分のデータを読み込む
			if (fread(&pixel, sizeof(int), 1, fp) != 1)
			{
				// エラー処理
				printf("\n[ readDiffFromFile() : データの読み込みに失敗 ]\n");

				goto catch;
			}
			
			diffData[row][col] = pixel;
		}
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return diffData;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	if (diffData != NULL) free(diffData);	// メモリ開放

	return NULL;
}


int** readDiff(int* pWidth, int* pHeight)
{
	/*
	ファイル名入力処理をして差分データファイルからデータを読み出す

	Parameters
	----------
	pWidth : int*
		幅ポインタ,
	pHeight : int*
		高さポインタ

	Returns
	-------
	diffData : int**
		差分データを保持した2次元配列のポインタ
	*/

	char fileName[FILE_NAME_SIZE] = { '\0' };	// ファイル名入力用文字列
	int** diffData = NULL;						// 2次元配列のポインタ


	/* 差分データファイルの情報を取得 */
	printf("\n[差分データファイルの情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	do
	{
		printf("　幅 (0以上) : ");
		scanf_s("%d", pWidth);
	} while (*pWidth <= 0);

	do
	{
		printf("高さ (0以上) : ");
		scanf_s("%d", pHeight);
	} while (*pHeight <= 0);

	/* 差分データの読み込み */
	diffData = readDiffFromFile(pWidth, pHeight, fileName);

	if (diffData != NULL)
	{
		printf("\n[ %s を読み込みました ]\n", fileName);

		return diffData;
	}
	else
	{
		printf("\n[ readDiff() : %s の読み込みに失敗しました ]\n", fileName);

		exit(EXIT_FAILURE);
	}
}


int writeDiffToFile(int width, int height, int** diffData, char fileName[])
{
	/*
	指定された差分データファイルへ差分データを書き込む

	Parameters
	----------
	width : int
		幅
	height : int
		高さ
	diffData : int**
		差分データを保持した2次元配列のポインタ
	fileNme : char[]
		ファイル名

	Returns
	-------
	status : int
		書き込み終了フラグ
	*/

	FILE* fp = NULL;			// ファイルポインタ
	int pixel = 0;				// 1画素分のデータ
	int row = 0, col = 0;		// ループカウンタ

	// 差分データファイルをオープンしてファイルポインタを取得
	if ((fp = fileOpen(fileName, "wb")) == NULL)
	{
		// エラー処理
		printf("\n[ writeDiffToFile() : %s が開けません ]\n", fileName);

		goto catch;
	}

	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{

			// 書き込む差分値を取得
			pixel = diffData[row][col];

			// 差分値を書き込む
			if (fwrite(&pixel, sizeof(int), 1, fp) != 1)
			{
				// エラー処理
				printf("\n[ writeDiffToFile() : データの書き込みに失敗 ]\n");

				goto catch;
			}
		}
	}

	fileClose(fp);	// ファイルポインタを閉じる

	return TRUE;

catch:

	fileClose(fp);	// ファイルポインタを閉じる

	return FALSE;
}


void writeDiff(int width, int height, int** diffData)
{
	/*
	ファイル名入力処理をして差分データをファイルに書き出す

	Parameters
	----------
	width : int
		幅
	height : int
		高さ
	diffData : int**
		差分データを保持した2次元配列のポインタ
	*/

	char fileName[FILE_NAME_SIZE];	// ファイル名入力用文字列
	int succeeded = FALSE;			// 書き込み完了フラグ

	/* 出力する差分データファイル名を取得 */
	printf("\n[ 差分データファイルの情報を入力 ]\n");
	printf("　ファイル名 : ");
	scanf_s(FILE_NAME_FORMAT, fileName, FILE_NAME_SIZE - 1);

	/* 差分データの書き込み */
	succeeded = writeDiffToFile(width, height, diffData, fileName);

	if (succeeded == TRUE)
	{
		printf("\n[ %s にデータを書き込みました ]\n", fileName);
	}
	else
	{
		printf("\n[ writeDiff() : %s への書き込みに失敗しました ]\n", fileName);

		if (diffData != NULL) free(diffData);	// メモリ開放

		exit(EXIT_FAILURE);
	}
}
