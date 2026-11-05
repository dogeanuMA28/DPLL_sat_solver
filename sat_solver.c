#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 128

int num_vars, num_clauses;
int *vars;
int **clauses;

int read_file(char *filename) {
    int clause_idx = 0;
    int var_idx = 0;

    FILE *in = fopen(filename, "r");
    if (!in) {
        printf("input eroare\n");
        return -1;
    }

    char line[LINE_SIZE];
    while (fgets(line, LINE_SIZE, in)) {
        // daca e comentariu merg la urmatoarea linia
        if (line[0] == 'c') {
            continue;
        }

        // citim numarul de clauze si de variabile pe care le poate avea clauza
        if (line[0] == 'p') {
            sscanf(line, "p cnf %d %d", &num_vars, &num_clauses);
            // alecarea
            vars = (int *)calloc(num_vars + 1, sizeof(int));
            clauses = (int **)calloc(num_clauses, sizeof(int *));
            for (int i = 0; i < num_clauses; i++) {
                clauses[i] = (int *)calloc(num_vars + 1, sizeof(int));
            }
        } else {
            int var = 0;
            var_idx = 0;
            
			for (int i = 0; line[i] != '0'; i++){
				// sar peste spatiu
                if (line[i] == ' ') {
					continue;
                }

                //luam numarul
                var = 0;
                // daca e negativ
                if (line[i] == '-') {
					// sarim de minus
                    i++;
					// construim numarul
                    while (line[i] >= '0' && line[i] <= '9') {
                        var = var * 10 + (line[i] - '0');
                        i++;
                    }
					// negam
                    var = -var;
                } else {
					// construim numarul
                    while (line[i] >= '0' && line[i] <= '9') {
                        var = var * 10 + (line[i] - '0');
                        i++;
                    }
                }

                // adaugam variabila in clauza
                clauses[clause_idx][var_idx++] = var;
            }
			// urmatoarea clauza
            clause_idx++;
			var_idx = 0;
        }
    }
	// inchidem fisierul la sfarsit
    fclose(in);
    return 0;
}

// verificam daca toate clauzele sunt satisfacute
int is_satisfied() {
    for (int i = 0; i < num_clauses; i++) {
        int satisfied = 0;
        for (int j = 0; clauses[i][j] != 0; j++) {
            if ((clauses[i][j] > 0 && vars[abs(clauses[i][j])] == 1) ||
			 (clauses[i][j] < 0 && vars[abs(clauses[i][j])] == -1)) {
            
			    satisfied = 1;
                break;
            }
        }
		// daca nu e satisfacut
        if (!satisfied) {
            return 0;
        }
    }
    return 1;
}

int unit_propagation() {
    int changed = 0;
	int satisfied = 0;
	int unassigned, var;

    for (int i = 0; i < num_clauses; i++) {
        unassigned = 0;
        var = 0;
        satisfied = 0;

        for (int j = 0; clauses[i][j] != 0; j++) {
            // verificam daca clauza e satisfacuta
            if ((clauses[i][j] > 0 && vars[abs(clauses[i][j])] == 1) ||
				 (clauses[i][j] < 0 && vars[abs(clauses[i][j])] == -1)) {
                
				satisfied = 1;
				continue;
            }

            // vedem cate nu sunt asignate in clauza
            if (vars[abs(clauses[i][j])] == 0) {
                unassigned++;
                var = clauses[i][j];
				// iesim mai devreme
				if (unassigned > 1) {
					break;
				}
            }
        }

        // daca clauza e satisfacuta mergem mai departe
        if (satisfied){
			continue;
		}

		// daca toate variablilele sunt puse si clauza nu e satifacuta == conflict
        if (unassigned == 0) {
            return 0;
        }

        // asigneaza variabila
        if (unassigned == 1) {
            vars[abs(var)] = (var > 0) ? 1 : -1;
            changed = 1;
        }
    }

    return changed;
}

int clause_unsat(int idx) {
    for (int i = 0; clauses[idx][i] != 0; i++) {
        if ((clauses[idx][i] > 0 && vars[abs(clauses[idx][i])] == 1) ||
		 (clauses[idx][i] < 0 && vars[abs(clauses[idx][i])] == -1) ||
		 vars[abs(clauses[idx][i])] == 0) {

		    return 0;
        }
    }
    return 1;
}

int DPLL() {
	// propagam
    while (unit_propagation()) {};
	// verificam daca este satisfacuta formula
    if (is_satisfied()) {
		return 1;
	}
	// verificam clauzele
    for (int i = 0; i < num_clauses; i++) {
        if (clause_unsat(i)) {
            return 0;
        }
    }
	// daca nu este satisfacuta incercam sa o satisfacem
    for (int i = 1; i <= num_vars; i++) {
        if (vars[i] == 0) {
			// incercam cu 1
			// facem backup
			int backup[num_vars + 1];
			memcpy(backup, vars, (num_vars + 1) * sizeof(int));
            vars[i] = 1;
            if (DPLL()) {
				return 1;
			}
			memcpy(vars, backup, (num_vars + 1) * sizeof(int));
			// altfel -1
            vars[i] = -1;
            if (DPLL()){
				return 1;
			}
			memcpy(vars, backup, (num_vars + 1) * sizeof(int));
			// altfel returnam 0
            return 0;
        }
    }
	
   return 0;
}

int write_answer(char *filename) {
	FILE *out = fopen(filename, "w");

	if (!out) {
		printf("output eroare\n");
        return -1;
	}

	 if (DPLL()) {
        fprintf(out, "s SATISFIABLE\nv ");
		for (int i = 1; i <= num_vars; i++) {
			fprintf(out, "%d ", (vars[i] < 0) ? -i:i);
		}
		fprintf(out, "0\n");
    } else {
        fprintf(out, "s UNSATISFIABLE\n");
    }
	fclose(out);
	return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("pune fisierele marlane\n");
        return 1;
    }
	// read
    if (read_file(argv[1]) < 0) {
        return 1;
    }
	// write
	if (write_answer(argv[2]) < 0) {
		return 1;
	}
    // frees
    free(vars);
    for (int i = 0; i < num_clauses; i++) {
        free(clauses[i]);
    }
    free(clauses);

    return 0;
}