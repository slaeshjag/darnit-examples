

int collision_test(int *obj1, int lines_1, int x1, int y1, int *obj2, int lines_2, int x2, int y2) {
	int i, j, t1, t2;


	for (i = 0; i < lines_1; i++)
		for (j = 0; j < lines_2; j++) {
			t1 = (obj2[j * 2 + 2] - obj2[j * 2]) * (obj1[i * 2 + 1] - obj2[j * 2 + 1]);
			t1 -= (obj2[j * 2 + 3] - obj2[j * 2 + 1]) * (obj1[i * 2] - obj2[j * 2]);
			t1 <<= 16;

			t2 = (obj2[j * 2 + 3] - obj2[j * 2 + 1]) * (obj1[i * 2 + 2] - obj1[j * 2]);
			t2 -= (obj2[j * 2 + 2] - obj2[j * 2]) * (obj1[i * 2 + 3] - obj1[i * 2 + 1]);
			if (t2 == 0)
				continue;
			if (t1/t2 < 0 || t1/t2 > 65536)
				continue;
			t1 = (obj1[i * 2 + 2] - obj1[i * 2]) * (obj1[i * 2 + 1] - obj2[j * 2 + 1]);
			t1 -= (obj1[i * 2 + 2] - obj1[i * 2 + 1]) * (obj1[i * 2] - obj2[j * 2]);
			t1 <<= 16;

			t2 = (obj2[j * 2 + 3] - obj2[j * 2 + 1]) * (obj1[i * 2 + 2] - obj1[i * 2]);
			t2 -= (obj2[j * 2 + 2] - obj2[j * 2]) * (obj1[i * 2 + 3] - obj1[i * 2 + 1]);
			if (t2 == 0)
				continue;
			if (t1/t2 < 0 || t1/t2 > 65536)
				continue;
			return 1;
		}

	return 0;
}
