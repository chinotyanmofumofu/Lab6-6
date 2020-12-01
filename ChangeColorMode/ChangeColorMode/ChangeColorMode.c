#include "libLab6.c"


/* �v���g�^�C�v�錾 */
void grayScale(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage);
void sepiaColor(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage);


/* main() �֐� */
int main(void)
{
	/* �ϐ��錾 */
	unsigned char*** inputImage = NULL;		// ��f�f�[�^�z��i���摜�p�j
	unsigned char*** outputImage = NULL;	// ��f�f�[�^�z��i������摜�p�j
	int width = 0, height = 0;				// �摜�̕��iwidth�j�ƍ����iheight�j

	/* ���摜�̓��� */
	inputImage = readBmp(&width, &height);

	/* �o�͉摜�p�z��𐶐� */
	outputImage = makeBmpArray(width, height);

	/* �摜����������֐����Ăяo�� */
	grayScale(width, height, inputImage, outputImage);

	/* ������摜�̏o�� */
	writeBmp(width, height, outputImage);

	/* �摜����������֐����Ăяo�� */
	sepiaColor(width, height, inputImage, outputImage);

	/* ������摜�̏o�� */
	writeBmp(width, height, outputImage);

	/* �������̊J�� */
	free(inputImage);
	free(outputImage);

	return 0;
}


/* �J���[�摜���O���[�X�P�[��������֐� */
void grayScale(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage)
{
	int row = 0, col = 0;		// ���[�v�J�E���^�irow�F�s(row)�����Ccol�F��(column)�����j
	int c = 0;					// ���[�v�J�E���^�i�F�ԍ��j

	unsigned char pixel = 0;	// �v�Z�l


	printf("\n[ �O���[�X�P�[���� ]\n");

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			pixel = 0.299 * inputImage[row][col][0] + 0.587 * inputImage[row][col][1] + 0.114 * inputImage[row][col][2];
			for (c = 0; c <= 2; c++) {
				outputImage[row][col][c] = pixel;
			}
		}
	}

}


/* �J���[�摜���Z�s�A��������֐� */
void sepiaColor(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage)
{
	int row = 0, col = 0;	// ���[�v�J�E���^�irow�F�s(row)�����Ccol�F��(column)�����j
	int c = 0;				// ���[�v�J�E���^�i�F�ԍ��j

	int pixel[3] = { 0 };	// �v�Z�l


	printf("\n[ �Z�s�A���� ]\n");

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