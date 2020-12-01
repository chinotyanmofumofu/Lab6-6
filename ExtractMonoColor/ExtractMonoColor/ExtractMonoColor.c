#include "libLab6.c"


/* �v���g�^�C�v�錾 */
void extractMonoColor(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage);


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
	extractMonoColor(width, height, inputImage, outputImage);

	/* ������摜�̏o�� */
	writeBmp(width, height, outputImage);

	/* �������̊J�� */
	free(inputImage);
	free(outputImage);

	return 0;
}


/* �P�F�摜�𐶐�����֐� */
void extractMonoColor(int width, int height, unsigned char*** inputImage, unsigned char*** outputImage)
{
	int color = 0;			// �F�ԍ�
	int row = 0, col = 0;	// ���[�v�J�E���^�irow�F�s(row)�����Ccol�F��(column)�����j
	int c = 0;				// ���[�v�J�E���^�i�F�ԍ��j

	/* �P�F�ɂ���F����� */
	printf("\n[ �F�̑I���i%d - Red / %d - Green / %d - Blue�j ]\n", R, G, B);
	printf("�F�ԍ� : ");
	scanf_s("%d", &color);

	printf("\n[ �P�F�摜�̐��� ]\n");

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
}
