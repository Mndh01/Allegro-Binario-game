#include <stdio.h>
#include <string.h>

#define PI 3.14
/// 2543->747 2843->622
#define Table_size 2843

int collision = 0;

// prototype for linked list
typedef struct list {
	char string[15];
	int length;
	int score;
	struct list* next;
	struct list* prev;
}list;

// prototype for hash table
typedef struct HASH {
	char string[15];
}HASH;

// definition of hash table
HASH H_Table[Table_size];

// first and last pointers for linked lists
list* first = NULL;
list* last = NULL;

// functions used anywhere
int init_list(char *s) {
	FILE* fp;
	fp = fopen(s, "r");
	if (!fp) {
		printf("Error with openning the file '%s'!\n\n" , s);
		return -1;
	}
	while (!feof(fp)) {
		list* p = (list*)malloc(sizeof(list));
		fscanf(fp, "%s\n", p->string);
		p->length = strlen(p->string);
		if (first == NULL) {
			first = p;
			last = p;
			first->prev = last;
			last->next = first;
		}
		else {				
			p->next = first;
			p->prev = last;
			last->next = p;
			last = p;
			first->prev = p;
		}
	}
	fclose(fp);
	return 0;
}
int char_to_int(const char* name) {
	int result = 0;
	int n = strlen(name);
	for (int i = 0; i < n; i++)
	{
		result += ((i * name[i]) + 1);
	}
	return result;
}
void free_list() {
	int i = 0;
	list* p;
	while (first != last) {
		i++;
		p = first;
		first = first->next;
		last->next = first;
		free(p);
	}
	if (first == last && first != NULL) {
		list* p = first;
		first = NULL;
		last = NULL;
		free(p);
	}
}

// functions for score evaluation part
void swap(list* temp, list* min) {
	list* hold_2 = (list*)malloc(sizeof(list));

	hold_2->length = min->length;
	hold_2->score = min->score;
	strcpy(hold_2->string, min->string);
	
	min->length = temp->length;
	min->score = temp->score;
	strcpy(min->string, temp->string);
	
	temp->length = hold_2->length;
	temp->score = hold_2->score;
	strcpy(temp->string, hold_2->string);

	free(hold_2);
}
void score_Sort()
{
	list* temp = first, *min,  *moving;

	do {
		int okay = 0;
		
		min = temp;
		moving = temp->next;


		while (moving != first) {
			if (min->score < moving->score) {
				min = moving;
				okay = 1;
			}
			moving = moving->next;			
		}
 
		if (okay) {	
			swap(temp, min);			
		}


		temp = temp->next;
		okay = 0;
		
	} while (temp != first);
	
	temp = first;
	printf("\n==== Top 10 passwords ====\n"
		"# \t Password \t score\n\n");
	for (int i = 0; i < 10; i++) {
		printf("%d. \t %s \t %d\n", i + 1, temp->string, temp->score);
		temp = temp->next;
	}
	printf("\n");

	temp = first->prev;
	printf("\n==== Worst 10 passwords ====\n"
		"# \t Password \t score\n\n");
	for (int i = 0; i < 10; i++) {
		printf("%d. \t %s \t %d\n", i + 1, temp->string, temp->score);
		temp = temp->prev;
	}
	printf("\n");
}

// functions for hashing part
int Key_Hash(char* password) {
	int key = ((password[0]) + 27 * (password[3]) + 729 * (password[5]) + password[1] + 26 * password[2] + 676 * password[4]) * 229;//229 -> 771 374
	return  key % Table_size;
}
void quadratic_probing(int key, char* password){
	int h = 1, i = key;
	while (1) {
		collision++;
		i = (i + (h * h)) % Table_size;
		if (strcmp(H_Table[i].string, "\0") == 0) {
			strcpy(H_Table[i].string, password);
			break;
		}
		h++;
	}
}
void linear_probing(int key, char* password) {
	do {
	key = (key + 1) % Table_size;	
	collision++;
	} while (strcmp(H_Table[key].string, "\0") != 0);
	if (strcmp(H_Table[key].string, "\0") == 0) {
		strcpy(H_Table[key].string, password);
	}
}
void Hash_Search(char* password) {
	collision = 0;
	int key = Key_Hash(password);
	int j, h = 1, i = key, check = 0;
	for (j = 0; j < Table_size; j++) {
		if (strcmp(H_Table[key].string, password) == 0) {
			printf("Password found in index %d with %d collision !\n\n", key, collision);
			return;
		}
		while (check != Table_size) {
			collision++;
			i = (i + (h * h)) % Table_size;
			if (strcmp(H_Table[i].string, password) == 0) {
				printf("Password found in index %d with %d collision !\n\n", i, collision);
				return;
			}
			h++;
			check++;
		}
	}
	printf("Password not found !\n\n");
	return;
}

// functions to do the asked main tasks
void Search() {
	list* p;
	p = first;
	int node_n = 0;
	char password[20];
	printf("Enter password to search for:"); scanf(" %[^\n]", password);
	do {
		node_n++;
		if (strcmp(p->string, password) == 0) {
			printf("password '%s' found in node %d!\n\n", p->string, node_n);
			free_list();
			return;
		}
		p = p->next;
	} while (p != first);
	printf("Password not found !\n\n");
	free_list();
}
void Sort() {
	list* p, * lp,* min, * Temp;
	char temp[20];

		for (p = first; p->next != last; p = p->next) {
			min = p;
			for (lp = p->next; lp != first; lp = lp->next)
				if (strcmp(min->string, lp->string) == 1) 
					min = lp;	
			strcpy(temp, p->string);
			strcpy(p->string, min->string);
			strcpy(min->string, temp);
		}
		Temp = first;
		printf("====First 10 passwords in ascending order====\n\n");
		for (int i = 0; i < 10; i++) {
			printf("%d.\t%s\n",i + 1, Temp->string);
			Temp = Temp->next;
		}
		printf("\n");
		//free_list();

		//init_list("10000.txt");
		for (p = first; p->next != last; p = p->next) {
			min = p;
			for (lp = p->next; lp != first; lp = lp->next)
				if (strcmp(min->string, lp->string) == -1) 
					min = lp;			
			if (min != lp) {
				strcpy(temp, p->string);
				strcpy(p->string, min->string);
				strcpy(min->string, temp);
			}
		}
		p = first;
		printf("====First 10 passwords in descending order====\n\n");
		for (int i = 0; i < 10; i++) {
			printf("%d.\t%s\n", i + 1, p->string);
			p = p->next;
		}
		printf("\n");
	//	free_list();
}
void Hash(char* s) {
	collision = 0;
	int key = 0, count = 0;
	char password[15];
	FILE* fp;
	for (int i = 0; i < Table_size; i++) {
		strcpy(H_Table[i].string, "\0");
	}
	fp = fopen(s, "r");
	if (!fp) {
		printf("Error openning file '%s'!\n\n", s);
		return;
	}

	while (!feof(fp)) {
		fscanf(fp, "%s\n", password);

		key = Key_Hash(password);

		if (strcmp(H_Table[key].string, "\0") == 0) {
			strcpy(H_Table[key].string, password);
			count++;
		}

		else {
			quadratic_probing(key, password);
		}
	}
	char s_pass[20];
	printf("\nElements inserted with no collisions:(%d)\n"
		"Collisions number for the rest of elements:(%d)\n\n"
		"Enter a password to be searched:"
		, count, collision); scanf("%s", s_pass);

	Hash_Search(s_pass);
}
void Evaluate() {
	list* p;
	int s[15] = { 0 };
	int i = 0;
	p = first;
	do {
		i++;
		p->score = 100;
		p = p->next;
	} while (p != first);
	p = first;
	do {
		if (p->length < 8) {
			p->score = p->score - 20;
		}
		for (i = 0; i <= p->length; i++) {
			s[i] = p->string[i];
		}
		for (i = 0; i <= p->length; i++) {
			if (s[i] + 1 == s[i + 1]  || s[i] == s[i + 1] || s[i] == s[i + 2] || s[i] == s[i + 1] + 1) {
				p->score -= 5;
			}
		}
		p = p->next;
	} while (p != first);
	score_Sort();
	free_list();
}

// main function
int main() {
	char txt_name[] = "100000.txt";
	int choice = 0, c = 0;
	
	while (choice != 5) {
		printf("1. Search from 100.000 list"
		       "\n2. Sort the 10.000 list"
		       "\n3. Hash the 1000 list"
		       "\n4. Evaluate the 1000 list"
		       "\n5. Exit.\n\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			strcpy(txt_name, "100000.txt");
			c = init_list(txt_name);
			if(c != -1)
			Search();
			break;
		case 2: 
			strcpy(txt_name,"10000.txt");
			c = init_list(txt_name);
			if(c != -1)
			Sort();
			break;
		case 3: 
			strcpy(txt_name, "1000.txt");
			Hash(txt_name);
			break;
		case 4:
			strcpy(txt_name, "1000.txt"); 
			c = init_list(txt_name);
			if(c != -1)
			Evaluate();
			break;
		case 5: 
			return 0;
		default: 
			printf("invalid choice...");
			break;
		}
	}
	return 0;
}