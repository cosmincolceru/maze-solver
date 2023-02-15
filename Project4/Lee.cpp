#include <iostream>
#include <queue>
#include "Lee.h"

using namespace std;

#define DIR 4
#if DIR == 4
	int di[DIR] = { -1, 0, 1, 0 };
	int dj[DIR] = {  0, 1, 0,-1 };
#elif DIR == 8
	int di[DIR] = { -1,-1, 0, 1, 1, 1, 0,-1 };
	int dj[DIR] = {  0, 1, 1, 1, 0,-1,-1,-1 };
#endif

void afisare_mat(int mat[NMAX][NMAX], int n, int m)
{
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++)
			cout << mat[i][j] << " ";
		cout << "\n";
	}
	cout << "\n";
}

static bool ok(int mat[NMAX][NMAX], int n, int m, int i, int j) {
	if (i < 1 || i > n || j < 1 || j > m)
		return false;
	if (mat[i][j] != 0)
		return false;
	return true;
}

void lee(int mat[NMAX][NMAX], int n, int m, int i_start, int j_start, int i_final, int j_final, int mat_drum[NMAX][NMAX])
{
	bool path = false;

	queue < pair <int, int> > coada;
	coada.push(make_pair(i_start, j_start));

	mat[i_start][j_start] = 1;

	while (!coada.empty() && !path) {
		int i = coada.front().first;
		int j = coada.front().second;
		coada.pop();


		for (int k = 0; k < DIR; k++) {
			int i_nou = i + di[k];
			int j_nou = j + dj[k];

			if (ok(mat, n, m, i_nou, j_nou)) {
				coada.push(make_pair(i_nou, j_nou));
				mat[i_nou][j_nou] = mat[i][j] + 1;

				if (i_nou == i_final && j_nou == j_final)
					path = true;
			}
		}
	}

	if (!path)
		cout << "\nNu exista drum\n";
	else {
		int i = i_final;
		int j = j_final;

		mat_drum[i][j] = mat[i][j];
		while (mat[i][j] != 1) {
			for (int k = 0; k < DIR; k++) {
				int i_nou = i + di[k];
				int j_nou = j + dj[k];

				if (mat[i_nou][j_nou] == mat[i][j] - 1) {
					i = i_nou;
					j = j_nou;

					mat_drum[i][j] = mat[i][j];

					break;
				}
			}
		}
	}
}