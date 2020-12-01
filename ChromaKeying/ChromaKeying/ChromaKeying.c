#include "libLab6.c"


/* �v���g�^�C�v�錾 */
void chromaKeying(int width, int height, unsigned char*** inputImage, unsigned char*** gbImage, unsigned char*** outputImage);


/* main() �֐� */
int main(void)
{
	/* �ϐ��錾 */
	unsigned char*** inputImage = NULL;		// ��f�f�[�^�z��i���摜�p�j
	unsigned char*** gbImage = NULL;		// ��f�f�[�^�z��i�O���[���o�b�N�摜�p�j
	unsigned char*** outputImage = NULL;	// ��f�f�[�^�z��i������摜�p�j
	int width = 0, height = 0;				// �摜�̕��iwidth�j�ƍ����iheight�j

	/* ���摜�̓��� */
	printf("\n[ ���摜�̓��� ]\n");
	inputImage = readBmp(&width, &height);

	/* �O���[���o�b�N�摜�̓��� */
	printf("\n[ �O���[���o�b�N�摜�̓��� ]\n");
	gbImage = readBmp(&width, &height);

	/* �o�͉摜�p�z��𐶐� */
	outputImage = makeBmpArray(width, height);

	/* �摜����������֐����Ăяo�� */
	chromaKeying(width, height, inputImage, gbImage, outputImage);

	/* ������摜�̏o�� */
	writeBmp(width, height, outputImage);

	/* �������̊J�� */
	free(inputImage);
	free(gbImage);
	free(outputImage);

	return 0;
}


/* �N���}�L�[����������֐� */
void chromaKeying(int width, int height, unsigned char*** inputImage, unsigned char*** gbImage, unsigned char*** outputImage)
{
	int row = 0, col = 0;	// ���[�v�J�E���^�irow�F�s(row)�����Ccol�F��(column)�����j
	int c = 0;				// ���[�v�J�E���^�i�F�ԍ��j
	double alpha = 0.0;		// ���ߓx


	/* ���ߓx����� */
	printf("\n[ ���ߓx�i�����j ]\n");
	printf("���ߓx [0.0 - 1.0] : ");
	scanf_s("%lf", &alpha);

	printf("\n[ �N���}�L�[���� ]\n");

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