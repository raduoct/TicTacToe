#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structura pentru cerinta 1 si 2(arbore de joc si arborele SI/SAU)
typedef struct tree {
	int m[3][3];
	int n;
	int TF;
	struct tree** pos;
} tree;

// structura pentru cerinta 3 (arborele MiniMax)
typedef struct minmax {
	int data;
	int n;
	struct minmax** pos;
} minmax;

// coada folosita pentru constructia aroberului MiniMax
// functiile pentru coada sunt preluate din laboratoare
typedef struct cell {
	minmax* data;
	struct cell* next;
	struct cell* last;
} Queue;

// functie ce intoarce maximul dintre 2 valori
int mymax(int a, int b)
{
	if(a > b)
		return a;
	else
		return b;
}

// functie ce intoarce minimul dintre 2 valori
int mymin(int a, int b)
{
	if(a > b)
		return b;
	else 
		return a;
}

// ferific daca coada este coala
int queue_is_empty(Queue* q)
{
	return q == NULL;
}
// introduc in coada un nou element
Queue* enqueue(Queue* q, minmax* data)
{
	Queue* new_q = NULL;
	new_q = (Queue*)malloc(sizeof(Queue));
	if (queue_is_empty(q))
	{
		new_q->data = data;
		new_q->last = new_q;
		new_q->next = NULL;
		return new_q;
	}
	new_q->next = NULL;
	new_q->data = data;
// ultimul nod pointeaza spre el
	new_q->last = new_q;
// legam penultimul nod cu ultimul
	q->last->next = new_q;
// primul nod arata noul ultim nod
	q->last = new_q;
	return q;
}
// scot un element din coada
Queue* dequeue(Queue* q)
{
	if (queue_is_empty(q))
		return NULL;

	Queue* aux = q;
	if(q->next != NULL)
		q->next->last = q->last;
	q = q->next;
	free(aux);

	return q;
}
// returnez valoarea primului element din coada
minmax* first(Queue* q)
{
	return q->data;
}

// construirea radacinii pentru arborele  de joc
// in matricea m X este 1, O este 0 si - este -10
tree* init(int m[3][3])
{
	int i, j, k, lib = 0;
	tree* new_root = NULL;
	new_root = (tree*)malloc(sizeof(tree));
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
		{
			// calculez numarul de pozitii libere de pe tabla de x si 0
			new_root->m[i][j] = m[i][j];
			if (m[i][j] == -10)
				lib++;
		}
	}
	// aloc memorie pentru fii radacinii adica cate pozitii libere am pe tabla 
	new_root->pos = (tree**)malloc(sizeof(tree*) * lib);
	new_root->n = lib;
	for (k = 0; k < new_root->n; k++)
	{
		new_root->pos[k] = (tree*)malloc(sizeof(tree));
		// scriu ca fii fiilor sa fie NULL folosind acest aspect ca si conditie pentru functia eliberare1
		new_root->pos[k]->pos = NULL;
		// copiez matricea tatalui in fiu, fiul diferind de tata printr o singura mutare in plus
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++)
			{
				new_root->pos[k]->m[i][j] = m[i][j];
			}
		}
	}

	return new_root;
}

//functie ce verifica fiecare conditie castigatoare de x si 0
int conditie(tree* root)
{
	int i;
	//daca se realizeaza castigul pe coloana
	for (i = 0; i < 3; i++)
	{
		if ((root->m[i][0] + root->m[i][1] + root->m[i][2]) == 3)
			return 1;
		else if ((root->m[i][0] + root->m[i][1] + root->m[i][2]) == 0)
			return 1;
	}
	//daca se realizeaza castigul pe linie
	for (i = 0; i < 3; i++)
	{
		if ((root->m[0][i] + root->m[1][i] + root->m[2][i]) == 3)
			return 1;
		else if ((root->m[0][i] + root->m[1][i] + root->m[2][i]) == 0)
			return 1;
	}
	// daca se realizeaza castigul pe diagonala principala
	if ((root->m[0][0] + root->m[1][1] + root->m[2][2]) == 3)
		return 1;
	if ((root->m[0][0] + root->m[1][1] + root->m[2][2]) == 0)
		return 1;
	// daca se realizeaza castigul pe diagonala secundara
	if ((root->m[0][2] + root->m[1][1] + root->m[2][0]) == 3)
		return 1;
	if ((root->m[0][2] + root->m[1][1] + root->m[2][0]) == 0)
		return 1;
	return 0;
}

// functie in care primind tatal creez toti fii acestuia si le setez si tabla tatalui fiecarui fiu
tree* adaug(tree* root)
{
	int k, i, j;
	if (root->n == 0)
		return root;
	root->pos = (tree**)malloc(sizeof(tree*) * root->n);
	for (k = 0; k < root->n; k++)
	{
		root->pos[k] = (tree*)malloc(sizeof(tree));
		root->pos[k]->pos = NULL;

		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++)
			{
				root->pos[k]->m[i][j] = root->m[i][j];
			}
		}
	}

	return root;
}
// functie in care creez totate posibilitatile de mutare viitoare(copii) prin punerea miscarii care face diferenta
// dintre fiu si tatal sau
void child(tree* root, int player)
{	
	if (conditie(root))
		return;
	if (root->n == 0)
		return;
	int j, k, i = 0;
	for (j = 0; j < 3; j++)
	{
		for (k = 0; k < 3; k++)
		{
			// conform enuntului pun miscarea ce face diferenta dintre tata si fiu unde trebuie
			if (root->m[j][k] == -10)
			{	
				root->pos[i]->n = root->n - 1;
				root->pos[i]->m[j][k] = player;
				i++;
			}
		}
	}
	for (j = 0; j < root->n; j++)
	{
		if (!conditie(root->pos[j]))
		{
			root->pos[j] = adaug(root->pos[j]);
				if (player == 0)
					child(root->pos[j], 1);
				else
					child(root->pos[j], 0);
		}
	}
	
}
// functie ce realizeaza afisarea pentru prima cerinta
void afisare(tree* root, int niv, FILE *g) {
	
	int i, j, k;
	for (i = 0; i < 3; i++) {
		// pentru fiecare nivel la care ma aflu atatea tab uri printez, incepand de la radacina ca si nivelul 0
		for (k = 0; k < niv; k++)
			fprintf(g, "\t");
		for (j = 0; j < 2; j++)
		{
			if(root->m[i][j] == -10)
				fprintf(g, "- ");

			if (root->m[i][j] == 1)
				fprintf(g, "X ");

			if (root->m[i][j] == 0)
				fprintf(g, "O ");
		}
		if (root->m[i][2] == -10)
			fprintf(g, "-");

		if (root->m[i][2] == 1)
			fprintf(g, "X");

		if (root->m[i][2] == 0)
			fprintf(g, "O");
		fprintf(g, "\n");
	}
	fprintf(g, "\n");
	if (conditie(root) || root->n == 0)
		return;
	for (i = 0; i < root->n; i++)
	{
		afisare(root->pos[i], niv+1, g);
	}
}
// realizarea eliberarii de memorie pentru cerinta 1 si 2
void eliberare1(tree* root) {

	// parcurg structura in adancime si ma intorc cand dau de frunze
	if(root->pos == NULL)
		return;

// eliberez copii nodului actuaal
	int i;
	for(i = 0; i < root->n; i++) {
		eliberare1(root->pos[i]);
		free(root->pos[i]);
	}
// eliberez portiunea unde erau alocatii copii
	free(root->pos);
}
// cand ma intorc inapoi recursiv mai intai sterg copii, memoria unde se aflau copii pana la radacina arborelui
// pe care o eliberez separat in main

void eliberare3(minmax* root) {

	if(root->pos == NULL)
		return;


	int i;
	for(i = 0; i < root->n; i++) {
		eliberare3(root->pos[i]);
		free(root->pos[i]);
	}

	free(root->pos);
}
// functie in care verific castigul lui X
int winX(tree* root)
{
	int i;
	//daca se realizeaza castigul pe coloana
	for (i = 0; i < 3; i++)
	{
		if ((root->m[i][0] + root->m[i][1] + root->m[i][2]) == 3)
			return 1;
	}
	//daca se realizeaza castigul pe linie
	for (i = 0; i < 3; i++)
	{
		if ((root->m[0][i] + root->m[1][i] + root->m[2][i]) == 3)
			return 1;
	}
	// daca se realizeaza castigul pe diagonala principala
	if ((root->m[0][0] + root->m[1][1] + root->m[2][2]) == 3)
		return 1;
	// daca se realizeaza castigul pe diagonala secundara
	if ((root->m[0][2] + root->m[1][1] + root->m[2][0]) == 3)
		return 1;
	return 0;
}
// functie in care verific castigul lui O
int winO(tree* root)
{
	int i;
	//daca se realizeaza castigul pe coloana
	for (i = 0; i < 3; i++)
	{
		if ((root->m[i][0] + root->m[i][1] + root->m[i][2]) == 0)
			return 1;
	}
	//daca se realizeaza castigul pe linie
	for (i = 0; i < 3; i++)
	{
		 if ((root->m[0][i] + root->m[1][i] + root->m[2][i]) == 0)
			return 1;
	}
	// daca se realizeaza castigul pe diagonala principala
	if ((root->m[0][0] + root->m[1][1] + root->m[2][2]) == 0)
		return 1;
	// daca se realizeaza castigul pe diagonala secundara
	if ((root->m[0][2] + root->m[1][1] + root->m[2][0]) == 0)
		return 1;
	return 0;
}
// functie in care setez valoarea T sau F
// pentru realizarea cerintei 2 ma folosesc de int ul TF din struct tree
// in care retin 1 pentru un nod care are True sau 0 pentru un nod care are False
int TsauF(tree* root, int niv, int player)
{	
// parcurg arborele in adancime pana intalnesc frunze a caror valoare de T sau F este data de
// functiile winX sau winO, valoarea de adevar depinde de care jucator este dat in enunt
	if (conditie(root) || root->n == 0)
	{	
		//daca jucatorul din enunt este X setez pentru fiecare frunza T sau F depinzand daca acesta a castigat
		if (player)
			root->TF = winX(root);
		//daca jucatorul din enunt este O setez pentru fiecare frunza T sau F depinzand daca acesta a castigat
		else
			root->TF = winO(root);
		return root->TF;
	}
	int k;
// odata setata valoarea de adevar pentru frunze pot sa construiesc in sus arborele SI/SAU
// daca nivelul este par realizez operatia sau intre valorile de adevar ale fiilor nodului 
// pentru a determina valoarea de adevar a nodului
	if (niv % 2 == 0)
	{
// faca prima operatie de sau cu 0 si valoarea de adevar a primului fiu
// sau cu 0 nu influenteaza valoarea de adevar a operatiilor de sau dintre fii
		int rez = 0;
		for (k = 0; k < root->n; k++)
		{
// apelez recursiv valoarea de adevar a fiului care se va construi in sus de la frunze
			rez = TsauF(root->pos[k], niv + 1, player) || rez;
		}
		root->TF = rez;
		return rez;
	}

// daca nivelul este par realizez operatia si intre valorile de adevar ale fiilor nodului 
// pentru a determina valoarea de adevar a nodului
	else 
	{
// faca prima operatie de si cu 1 si valoarea de adevar a primului fiu
// si cu 1 nu influenteaza valoarea de adevar a operatiilor de si dintre fii
		int rez = 1;
		for (k = 0; k < root->n; k++)
		{
// apelez recursiv valoarea de adevar a fiului care se va construi in sus de la frunze
			rez = TsauF(root->pos[k], niv + 1, player) && rez;
		}
		root->TF = rez;
		return rez;
	}
}

// in afisare2 realizez afisarea pentru cerinta 2
// in care afisez valoarea de T sau F a fiecarei frunze
void afisare2(tree* root, int niv, FILE* g)
{

	int i, k;
	for (k = 0; k < niv; k++)
		fprintf(g, "\t");
	if (root->TF)
		fprintf(g, "T\n");
	else
		fprintf(g, "F\n");
	if (conditie(root) || root->n == 0)
		return;
	for (i = 0; i < root->n; i++)
	{
		afisare2(root->pos[i], niv + 1, g);
	}

}

//functia in care realizez citirea si construirea arobrelui pentru cerinta 3
minmax* citire3(minmax* root, FILE* f)
{
// char paranteza il folosesc pt a citi parantezele iar enter pt a citii spatiie albe si \n
// nev reprezinta numar elemente viitoare ce trebuie citite de pe fiecare linie din input
	int nr_linii, i, j, nr_elem = 1, elem_curent, nev, k;
	char paranteza, enter;
	minmax* x;
	
	fscanf(f, "%d%c", &nr_linii, &enter);
	Queue* q = NULL;
	q = enqueue(q, root);

	for (i = 0; i < nr_linii; i++)
	{
		// reinitializam numarul de elemente viitoare citite cu 0
		nev = 0;
		for (j = 0; j < nr_elem; j++)
		{
			fscanf(f, "%c", &paranteza);
			// scoatem nodul din coada pentru a ii construii in memorie fii 
			x = first(q);
			q = dequeue(q);

			if (paranteza == '(')
			{
				// citeste numarul de fii ce il are nodul curent
				fscanf(f, "%d", &elem_curent);
				nev = nev + elem_curent;
				// aloca memorie pt nr de fii
				x->n = elem_curent;
				x->pos = (minmax**)malloc(sizeof(minmax*) * elem_curent);
				for (k = 0; k < x->n; k++)
				{

					x->pos[k] = (minmax*)malloc(sizeof(minmax));
					x->pos[k]->pos = NULL;
					// introduce fii in coada
					q = enqueue(q, x->pos[k]);
				}
			}
			else {
				// pentru [] - este frunza
				// daca este nod frunza ii seteaza valoarea si numarul de fii autmoat 0
				fscanf(f, "%d", &elem_curent);
				x->data = elem_curent;
				x->n = 0;
			}

			// pentru paranteza de inchidere
			fscanf(f, "%c", &paranteza);
			if (j != nr_elem - 1) {
				fscanf(f, "%c", &paranteza);
			}
		}
		// numarul de noduri de pe linia i+1 ce vor fii citite devin suma nodurilor intre () de pe linia i
		nr_elem = nev;
		fscanf(f, "%c", &enter);
	}

	return root;
}

// atribui fiecarui nod valoarea corespunzatoare
// construiesc arobrele de la frunze spre radacini
// la nivel par realizez maximul dintre valoriile fiilor
// iar la nivel impar realizez minimul dintre valoriile fiilor
int fill(minmax* root, int niv)
{
// nodurile frunza sunt date din enunt
	if (root->n == 0)
		return root->data;
	int k;
// realizez maximul la nivel par
	if (niv % 2 == 0)
	{
		int rez;
		rez = fill(root->pos[0], niv + 1);
		for (k = 1; k < root->n; k++)
		{
// apelez recursiv functia pentru a parcurge in adacnime arborele pana la frunze
// de unde se va construi recursiv in sus valoarea cautata pentru fiecare fiu al nodului
// pentru valoarea nodului realizez max dintre valoriile fiilor
			rez = mymax(rez,fill(root->pos[k], niv + 1));
		}
		root->data = rez;
		return rez;
	}
// realizez minimul la nivel impar
	else 
	{
		int rez;
		rez = fill(root->pos[0], niv + 1);
		for (k = 1; k < root->n; k++)
		{
// apelez recursiv functia pentru a parcurge in adacnime arborele pana la frunze
// de unde se va construi recursiv in sus valoarea cautata pentru fiecare fiu al nodului
// pentru valoarea nodului realizez min dintre valoriile fiilor
			rez = mymin(rez,fill(root->pos[k], niv + 1));
		}
		root->data = rez;
		return rez;
	}
}

// realizez afisarea pentru cerinta 3
void afisare3(minmax* root, int niv, FILE* g)
{

	int i, k;
	for (k = 0; k < niv; k++)
		fprintf(g, "\t");
	fprintf(g, "%d\n", root->data);
	if (root->n == 0)
		return;
	for (i = 0; i < root->n; i++)
	{
		afisare3(root->pos[i], niv + 1, g);
	}

}

// functie in care realizez cerinta 1
void cerinta1(char* infile, char* outfile) {
	FILE* f = fopen(infile, "r");

	int i, j, player, m[3][3];
	char c, sp;
	tree* root = NULL;

	fscanf(f, "%c%c", &c, &sp);
	if (c == 'O')
		player = 0;
	else
		player = 1;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(f, "%c%c", &c, &sp);

			if (c == '-')
				m[i][j] = -10;
			else if (c == 'O')
				m[i][j] = 0;
			else
				m[i][j] = 1;
		}
	}
// construiesc arborele cu ajutorul functiilor init si child
	root = init(m);
	child(root, player);
// realizez afisarea 
	FILE *g = fopen(outfile, "w");
	afisare(root, 0, g);

	fclose(f);
	fclose(g);
// eliberez memoria
	eliberare1(root);
	free(root);
}
// functia in care realizez cerinta 2
void cerinta2(char* infile, char* outfile) {
	FILE* f = fopen(infile, "r");

	int i, j, player, m[3][3];
	char c, sp;
	tree* root = NULL;

	fscanf(f, "%c%c", &c, &sp);
	if (c == 'O')
		player = 0;
	else
		player = 1;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(f, "%c%c", &c, &sp);

			if (c == '-')
				m[i][j] = -10;
			else if (c == 'O')
				m[i][j] = 0;
			else
				m[i][j] = 1;
		}
	}
// construiesc arobrele 
	root = init(m);
	child(root, player);
// setez valoarea de T sau F a fiecarui nod
	FILE *g = fopen(outfile, "w");
	TsauF(root, 0, player);
// realizez 
	afisare2(root, 0, g);

	fclose(f);
	fclose(g);

	eliberare1(root);
	free(root);
}

void cerinta3(char* infile, char* outfile) {

	FILE *f = fopen(infile, "r");
	FILE *g = fopen(outfile, "w");
// in citire3 citesc si construiesc arborele din punct de vedere al memoriei
	minmax* root = NULL;
	root = (minmax*)malloc(sizeof(minmax));
	root = citire3(root, f);
// in functia fill atribui fiecarui nod valoarea corecta
	fill(root, 0);
	afisare3(root, 0, g);

	fclose(f);
	fclose(g);
// in functia eliberare3 eliberez memoria pentru noua structura arborescenta, asemanator cu cerinta 1 si 2
	eliberare3(root);
	free(root);
}

// in main verific conform argumentelor din linia de comanda pentru care cerinta rulez rezolvarea corespunzatoare
int main(int argc, char **argv)
{
	if(strcmp(argv[1], "-c1") == 0)
		cerinta1(argv[2], argv[3]);
	
	if(strcmp(argv[1], "-c2") == 0)
		cerinta2(argv[2], argv[3]);

	if(strcmp(argv[1], "-c3") == 0)
		cerinta3(argv[2], argv[3]);
	
	return 0;
}
